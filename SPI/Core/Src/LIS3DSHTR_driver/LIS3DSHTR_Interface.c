#include "LIS3DSHTR_driver/LIS3DSHTR_Interface.h"
#include "LIS3DSHTR_driver/LIS3DSHTR_Registers.h"
#include "LIS3DSHTR_driver/LIS3DSHTR_object.h"


/* ========================================================================== */
/* 0. SPI makros (Internal use only)                             */
/* ========================================================================== */

#define BUFFERS_SIZE 2
#define TX_BUFFER_SIZE BUFFERS_SIZE
#define RX_BUFFER_SIZE BUFFERS_SIZE

/* ========================================================================== */
/* 0. STATIC HELPER FUNCTIONS (Internal use only)                             */
/* ========================================================================== */
uint8_t* LISDSHTR_Get_Register_Pointer(struct LIS3DSHTR_Object *obj, uint8_t reg_addr) {
    if (reg_addr >= 0x40 && reg_addr <= 0x7F) {
        struct LIS3DSH_SM_CFGs *sm = (reg_addr >= 0x60) ? &obj->SM2 : &obj->SM1;
        uint8_t local_addr = (reg_addr >= 0x60) ? (reg_addr - 0x20) : reg_addr;

        if (local_addr >= 0x40 && local_addr <= 0x4F) {
            return &sm->STx[local_addr - 0x40];
        }

        switch (local_addr) {
            case 0x50: return &sm->TIM4;
            case 0x51: return &sm->TIM3;
            case 0x52: return &sm->TIM2_L;
            case 0x53: return &sm->TIM2_H;
            case 0x54: return &sm->TIM1_L;
            case 0x55: return &sm->TIM1_H;
            case 0x56: return &sm->THRS2;
            case 0x57: return &sm->THRS1;
            case 0x58: return (reg_addr >= 0x60) ? &sm->DES : NULL; /* DES występuje tylko w SM2 (0x78) */
            case 0x59: return &sm->MASK_B;
            case 0x5A: return &sm->MASK_A;
            case 0x5B: return &sm->SETT;
            case 0x5C: return &sm->PR;
            case 0x5D: return &sm->TC_L;
            case 0x5E: return &sm->TC_H;
            case 0x5F: return &sm->OUTS;
            default:   return NULL;
        }
    }

    /* 2. Obsługa standardowej mapy rejestrów bazowych */
    switch (reg_addr) {
        /* Device Info & ID */
        case LIS3DSH_REG_OUT_T:        return &obj->OUT_T;
        case LIS3DSH_REG_INFO1:        return &obj->INFO1;
        case LIS3DSH_REG_INFO2:        return &obj->INFO2;
        case LIS3DSH_REG_WHO_AM_I:     return &obj->WHO_AM_I;

        /* User Calibrations */
        case LIS3DSH_REG_OFF_X:        return &obj->OFF_X;
        case LIS3DSH_REG_OFF_Y:        return &obj->OFF_Y;
        case LIS3DSH_REG_OFF_Z:        return &obj->OFF_Z;
        case LIS3DSH_REG_CS_X:         return &obj->CS_X;     /* Constant shift registers (13h-15h) */
        case LIS3DSH_REG_CS_Y:         return &obj->CS_Y;
        case LIS3DSH_REG_CS_Z:         return &obj->CS_Z;

        /* Core Status and Core Configurations */
        case LIS3DSH_REG_STAT:         return &obj->STAT;
        case LIS3DSH_REG_CTRL_REG4:    return &obj->CTRL_REGS.CTRL_REG4;
        case LIS3DSH_REG_CTRL_REG1:    return &obj->SM1.CTRL_REG; /* Zgodnie z nową strukturą obiektu */
        case LIS3DSH_REG_CTRL_REG2:    return &obj->SM2.CTRL_REG; 
        case LIS3DSH_REG_CTRL_REG3:    return &obj->CTRL_REGS.CTRL_REG3;
        case LIS3DSH_REG_CTRL_REG5:    return &obj->CTRL_REGS.CTRL_REG5;
        case LIS3DSH_REG_CTRL_REG6:    return &obj->CTRL_REGS.CTRL_REG6;
        case LIS3DSH_REG_STATUS:       return &obj->STATUS;
        
        /* System Timers & Extra Thresholds */
        case LIS3DSH_REG_LC_L:                     return ((uint8_t*)&obj->LC);     /* Long-counter Low byte (16h) */
        case LIS3DSH_REG_LC_H:                     return ((uint8_t*)&obj->LC) + 1; /* Long-counter High byte (17h) */
        case LIS3DSH_REG_VFC_1:                     return &obj->VFC_1;
        case LIS3DSH_REG_VFC_2:                     return &obj->VFC_2;
        case LIS3DSH_REG_VFC_3:                     return &obj->VFC_3;
        case LIS3DSH_REG_VFC_4:                     return &obj->VFC_4;
        case LIS3DSH_REG_THRS3:        return &obj->THRS3;

        /* Raw Acceleration Deliverables */
        case LIS3DSH_REG_OUT_X_L:      return &obj->OUT_REGS.OUT_X_L;
        case LIS3DSH_REG_OUT_X_H:      return &obj->OUT_REGS.OUT_X_H;
        case LIS3DSH_REG_OUT_Y_L:      return &obj->OUT_REGS.OUT_Y_L;
        case LIS3DSH_REG_OUT_Y_H:      return &obj->OUT_REGS.OUT_Y_H;
        case LIS3DSH_REG_OUT_Z_L:      return &obj->OUT_REGS.OUT_Z_L;
        case LIS3DSH_REG_OUT_Z_H:      return &obj->OUT_REGS.OUT_Z_H;

        /* FIFO Buffers Management */
        case LIS3DSH_REG_FIFO_CTRL:    return &obj->FIFO_CTRL;
        case LIS3DSH_REG_FIFO_SRC:     return &obj->FIFO_SRC;

        default:
            /* Invalid or reserved register address */
            return NULL;
    }
}


/**
 * @brief  Writes multiple bytes to specific LIS3DSHTR registers via SPI.
 * @note   Zero-buffer implementation. Eliminates intermediate local array allocations
 * by transmitting the command header and user data back-to-back within a single CS frame.
 * @param[in,out] dev       Pointer to the LIS3DSHTR device handle structure.
 * @param[in]     reg_addr  The target physical register address in the sensor.
 * @param[in]     data      Pointer to the source buffer containing data to write.
 * @param[in]     length    Number of bytes to be written to the sensor.
 * @retval HAL_StatusTypeDef HAL_OK if successful, HAL_ERROR or HAL_BUSY/TIMEOUT otherwise.
 */
HAL_StatusTypeDef LIS3DSHTR_SPI_WriteReg(LIS3DSHTR_HandleTypeDef *dev, uint8_t reg_addr, uint8_t *data, uint16_t length) {
    
    /* 1. Input Parameter Validation (Guard Clauses) */
    if (length == 0 || data == NULL || dev == NULL) {
        return HAL_ERROR;
    }

    /* Command byte for the SPI write operation (allocated safely on stack, 1 byte only) */
    uint8_t command_byte = 0;

    /* 2. Construct the SPI Control Byte (Address + Direction/Increment Flags) */
    if (length > 1) {
        /* Enable address auto-increment for multi-byte burst write */
        command_byte = LIS3DSH_SPI_WRITE | LIS3DSH_SPI_MS_INCR | reg_addr;
    } else {
        /* Keep the address static for a single-byte register write */
        command_byte = LIS3DSH_SPI_WRITE | LIS3DSH_SPI_MS_STAY | reg_addr;
    }

    /* 3. Physical Hardware Transmission */
    
    /* CS Low - Select the device to start the SPI transaction */
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET); 

    /* Transmit the 1-byte command/address header */
    HAL_StatusTypeDef status = HAL_SPI_Transmit(dev->hspi, &command_byte, 1, LIS3DSHTR_SPI_TIMEOUT);

    /* If the header was sent successfully, transmit the entire user data buffer directly */
    if (status == HAL_OK) {
        /* Stream data straight from the user-provided pointer, no local copying needed */
        status = HAL_SPI_Transmit(dev->hspi, data, length, LIS3DSHTR_SPI_TIMEOUT);
    }
    
    /* CS High - Deselect the device to finalize the SPI write frame */
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET); 

    /* 4. Local Register Shadow Copy Update */
    if (status == HAL_OK) {
        for (uint16_t i = 0; i < length; i++) {
            /* Retrieve the memory pointer to the corresponding field in the local structure */
            uint8_t *reg_ptr = LIS3DSHTR_Get_Register_Pointer(&dev->data, reg_addr + i);
            
            /* If the register is mapped within the structure, update its local value */
            if (reg_ptr != NULL) {
                *reg_ptr = data[i];
            }
        }
    }

    /* Return the final hardware operation status */
    return status;
}

/**
 * @brief  Reads multiple bytes from LIS3DSHTR registers directly into the shadow copy.
 * @note   Zero-buffer implementation. This eliminates stack-allocated dynamic arrays (VLA)
 * and prevents shared-memory race conditions by streaming data directly into the
 * device handle's local register mapping.
 * @param[in,out] dev       Pointer to the LIS3DSHTR device handle structure.
 * @param[in]     reg_addr  The starting physical register address to read from.
 * @param[in]     length    Number of bytes to read from the sensor.
 * @retval HAL_StatusTypeDef HAL_OK if successful, HAL_ERROR or HAL_BUSY/TIMEOUT otherwise.
 */
HAL_StatusTypeDef LIS3DSHTR_SPI_ReadRegs(LIS3DSHTR_HandleTypeDef *dev, uint8_t reg_addr, uint16_t length) {
    
    /* 1. Input Parameter Validation */
    if (length == 0 || dev == NULL) {
        return HAL_ERROR;
    }

    /* Command byte for the SPI read operation (always 1 byte allocated on stack) */
    uint8_t command_byte = 0;

    /* 2. Construct the SPI Command Byte (Read Bit + Address + Increment Flag) */
    if (length > 1) {
        /* Enable address auto-increment for multi-byte burst read */
        command_byte = LIS3DSH_SPI_READ | LIS3DSH_SPI_MS_INCR | reg_addr;
    } else {
        /* Keep the address static for a single-byte register read */
        command_byte = LIS3DSH_SPI_READ | LIS3DSH_SPI_MS_STAY | reg_addr;
    }

    /* 3. Physical Hardware Transaction */
    
    /* CS Low - Select the device to initiate SPI communication */
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET); 

    /* Transmit the 1-byte command/address header */
    HAL_StatusTypeDef status = HAL_SPI_Transmit(dev->hspi, &command_byte, 1, LIS3DSHTR_SPI_TIMEOUT);

    /* 4. Stream Data Directly Into Shadow Copy Memory */
    if (status == HAL_OK) {
        for (uint16_t i = 0; i < length; i++) {
            /* Map the virtual memory location to the hardware register address offset */
            uint8_t *reg_ptr = LIS3DSHTR_Get_Register_Pointer(&dev->data, reg_addr + i);
            
            if (reg_ptr != NULL) {
                /* Receive 1 byte directly into the structure field memory location */
                status = HAL_SPI_Receive(dev->hspi, reg_ptr, 1, LIS3DSHTR_SPI_TIMEOUT);
            } else {
                /* Dummy read buffer: maintains SPI clock alignment if the current register 
                   address is skipped or unmapped in our software configuration */
                uint8_t dummy_byte;
                status = HAL_SPI_Receive(dev->hspi, &dummy_byte, 1, LIS3DSHTR_SPI_TIMEOUT);
            }

            /* Break early if any hardware bus error occurs during streaming */
            if (status != HAL_OK) {
                break;
            }
        }
    }

    /* CS High - Deselect the device to terminate the SPI frame */
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET); 

    /* Return the actual operation status (HAL_OK / HAL_TIMEOUT / HAL_ERROR) */
    return status;
}


/* ========================================================================== */
/* 1. CORE DEVICE MANAGEMENT & INITIALIZATION                                 */
/* ========================================================================== */


HAL_StatusTypeDef LIS3DSH_SetDataRate_And_PowerMode(LIS3DSHTR_HandleTypeDef *dev, LIS3DSH_DataRate data_rate ) {
    dev->data.CTRL_REGS.CTRL_REG4 &= ~LIS3DSH_CR4_ODR_MASK; // Clear the ODR bits
    uint8_t new_ctrl_reg4 = dev->data.CTRL_REGS.CTRL_REG4 | (data_rate << 4); // Set the new data rate
    HAL_StatusTypeDef status = LIS3DSHTR_SPI_WriteReg(dev, LIS3DSH_REG_CTRL_REG4, &new_ctrl_reg4, 1);
    if(status == HAL_OK) {
        dev->data.CTRL_REGS.CTRL_REG4 = new_ctrl_reg4; // Update local shadow copy only if SPI write was successful
    }
    // Implementation for setting data rate and power mode
    return status;
}


HAL_StatusTypeDef LIS3DSH_SetFullScale(LIS3DSHTR_HandleTypeDef *dev, LIS3DHS_FullScaleMode full_scale ){
    dev->data.CTRL_REGS.CTRL_REG5 &= ~LIS3DSH_CR5_FSCALE_MASK; // Clear the FS bits
    uint8_t new_ctrl_reg5 = dev->data.CTRL_REGS.CTRL_REG5 | (full_scale << 3); // Set the new full scale
    HAL_StatusTypeDef status = LIS3DSHTR_SPI_WriteReg(dev, LIS3DSH_REG_CTRL_REG5, &new_ctrl_reg5, 1);
    if(status == HAL_OK) {
        dev->data.CTRL_REGS.CTRL_REG5 = new_ctrl_reg5; // Update local shadow copy only if SPI write was successful
    }
    return status;
}


HAL_StatusTypeDef LIS3DSH_EnableInterrupt(LIS3DSHTR_HandleTypeDef *dev, LIS3DSH_InterruptConfig_t *config){
    dev->data.CTRL_REGS.CTRL_REG3 &= ~(LIS3DSH_CR3_DR_EN);
    
}

HAL_StatusTypeDef LIS3DSH_Init(LIS3DSHTR_HandleTypeDef *dev, LIS3DSH_DataRate data_rate) {
    HAL_StatusTypeDef status = HAL_OK;

    //First we chec is there any device online
    status = LIS3DSHTR_SPI_ReadRegs(dev, LIS3DSH_REG_WHO_AM_I, 1);
    if(status != HAL_OK){
        return HAL_ERROR;
    }




    return status;
}
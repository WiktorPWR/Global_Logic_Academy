#include "LIS3DSHTR_driver/LIS3DSHTR_Interface.h"
#include "LIS3DSHTR_driver/LIS3DSHTR_Registers.h"
#include "LIS3DSHTR_driver/LIS3DSHTR_object.h"

/* ========================================================================== */
/* 0. STATIC HELPER FUNCTIONS (Internal use only)                             */
/* ========================================================================== */

/**
 * @brief  Writes a single byte to a specific LIS302DL register.
 */
static HAL_StatusTypeDef LIS302DL_SPI_WriteReg(LIS302DL_HandleTypeDef *dev, uint8_t reg_addr, uint8_t data) {

    
    return status;
}

/**
 * @brief  Reads one or multiple bytes from LIS302DL registers.
 */
static HAL_StatusTypeDef LIS302DL_SPI_ReadRegs(LIS302DL_HandleTypeDef *dev, uint8_t reg_addr, uint8_t *data, uint16_t length) {
    /* If length > 1, enable auto-increment bit */
    uint8_t ms_bit = (length > 1) ? LIS302DL_SPI_MS_INCR : LIS302DL_SPI_MS_STAY;
    uint8_t tx_reg = LIS302DL_SPI_READ | ms_bit | reg_addr;

    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(dev->hspi, &tx_reg, 1, LIS302DL_SPI_TIMEOUT);
    if (status == HAL_OK) {
        status = HAL_SPI_Receive(dev->hspi, data, length, LIS302DL_SPI_TIMEOUT);
    }
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

    return status;
}



/* ========================================================================== */
/* 1. CORE DEVICE MANAGEMENT & INITIALIZATION                                 */
/* ========================================================================== */

HAL_StatusTypeDef LIS302DL_Check_ID(LIS302DL_HandleTypeDef *dev) {
    uint8_t id = 0;
    HAL_StatusTypeDef status = LIS302DL_SPI_ReadRegs(dev, LIS302DL_REG_WHO_AM_I, &id, 1);

    if (status != HAL_OK) {
        return status;
    }

    if (id == LIS302DL_WHO_AM_I_VAL) {
        return HAL_OK;
    } else {
        return HAL_ERROR;
    }
}

HAL_StatusTypeDef LIS302DL_Init(LIS302DL_HandleTypeDef *dev) {
    /* Fixed macro syntax to prevent preprocessor expansion errors */
    #define RX_SIZE (LIS302DL_END_ADDRESS - LIS302DL_START_ADDRESS + 1)
    
    uint8_t register_map[RX_SIZE] = {0};
    
    HAL_StatusTypeDef status = LIS302DL_SPI_ReadRegs(dev, LIS302DL_START_ADDRESS, register_map, RX_SIZE);
    
    if (status != HAL_OK) {
        return status; /* Abort initialization if SPI read fails */
    }

    /* Populate the local shadow structure with default hardware states */
    for (uint8_t i = LIS302DL_START_ADDRESS; i <= LIS302DL_END_ADDRESS; i++) {
        uint8_t array_index = i - LIS302DL_START_ADDRESS; 
        
        switch (i) {
            case LIS302DL_REG_CTRL_REG1:
                dev->data.CTRL_REGS.CRTL_REG1 = register_map[array_index];
                break;
            case LIS302DL_REG_CTRL_REG2:
                dev->data.CTRL_REGS.CRTL_REG2 = register_map[array_index];
                break;
            case LIS302DL_REG_CTRL_REG3:
                dev->data.CTRL_REGS.CRTL_REG3 = register_map[array_index];
                break;
            case LIS302DL_REG_HP_FILTER_RESET:
                dev->data.HP_FILTER_RESET = register_map[array_index];
                break;
            case LIS302DL_REG_STATUS_REG:
                dev->data.STATUS_REG = register_map[array_index];
                break;
            case LIS302DL_REG_OUT_X:
                dev->data.OUT_REGS.OUT_X = register_map[array_index];
                break;
            case LIS302DL_REG_OUT_Y:
                dev->data.OUT_REGS.OUT_Y = register_map[array_index];
                break;
            case LIS302DL_REG_OUT_Z:
                dev->data.OUT_REGS.OUT_Z = register_map[array_index];
                break;
            case LIS302DL_REG_FF_WU_CFG_1:
                dev->data.FF_WU_CFGS_1.FF_WU_CFG = register_map[array_index];
                break;
            case LIS302DL_REG_FF_WU_SRC_1:
                dev->data.FF_WU_CFGS_1.FF_WU_SRC = register_map[array_index];
                break;
            case LIS302DL_REG_FF_WU_THS_1:
                dev->data.FF_WU_CFGS_1.FF_WU_THS = register_map[array_index];
                break;
            case LIS302DL_REG_FF_WU_DURATION_1:
                dev->data.FF_WU_CFGS_1.FF_WU_DURATION = register_map[array_index];
                break;
            case LIS302DL_REG_FF_WU_CFG_2:
                dev->data.FF_WU_CFGS_2.FF_WU_CFG = register_map[array_index];
                break;
            case LIS302DL_REG_FF_WU_SRC_2:
                dev->data.FF_WU_CFGS_2.FF_WU_SRC = register_map[array_index];
                break;
            case LIS302DL_REG_FF_WU_THS_2:
                dev->data.FF_WU_CFGS_2.FF_WU_THS = register_map[array_index];
                break;
            case LIS302DL_REG_FF_WU_DURATION_2:
                dev->data.FF_WU_CFGS_2.FF_WU_DURATION = register_map[array_index];
                break;
        }
    }
    
    return HAL_OK;
}

HAL_StatusTypeDef LIS302DL_CtrlReg1_Config(LIS302DL_HandleTypeDef *dev, uint8_t data_rate, uint8_t power_mode, uint8_t full_scale, uint8_t stp, uint8_t stm, uint8_t zen, uint8_t yen, uint8_t xen) {
    /* 1. Get the current shadow value from RAM */
    uint8_t actual_reg_value = dev->data.CTRL_REGS.CRTL_REG1;

    /* 2. Clear specific bits to be overwritten */
    actual_reg_value &= (uint8_t)~(LIS302DL_CR1_DR_MASK | LIS302DL_CR1_PD_MASK | LIS302DL_CR1_FS_MASK | 
                          LIS302DL_CR1_STP     | LIS302DL_CR1_STM     | LIS302DL_CR1_ZEN     | 
                          LIS302DL_CR1_YEN     | LIS302DL_CR1_XEN);

    /* 3. Combine base value with new configuration */
    uint8_t config_value = actual_reg_value | data_rate | power_mode | full_scale | stp | stm | zen | yen | xen;

    /* 4. Execute SPI Write transaction */
    HAL_StatusTypeDef status = LIS302DL_SPI_WriteReg(dev, LIS302DL_REG_CTRL_REG1, config_value);

    /* 5. Update local shadow structure upon success */
    if (status == HAL_OK) {
        dev->data.CTRL_REGS.CRTL_REG1 = config_value;
    }

    return status;
}

HAL_StatusTypeDef LIS302DL_CtrlReg2_Config(LIS302DL_HandleTypeDef *dev, uint8_t sim, uint8_t boot, uint8_t fds, uint8_t hp_ff_wu2, uint8_t hp_ff_wu1) {
    uint8_t actual_reg_value = dev->data.CTRL_REGS.CRTL_REG2;

    actual_reg_value &= ~(LIS302DL_CR2_SIM_MASK | LIS302DL_CR2_BOOT | LIS302DL_CR2_FDS | 
                          LIS302DL_CR2_HP_FF_WU2 | LIS302DL_CR2_HP_FF_WU1);

    uint8_t config_value = actual_reg_value | sim | boot | fds | hp_ff_wu2 | hp_ff_wu1;

    HAL_StatusTypeDef status = LIS302DL_SPI_WriteReg(dev, LIS302DL_REG_CTRL_REG2, config_value);

    if (status == HAL_OK) {
        dev->data.CTRL_REGS.CRTL_REG2 = config_value;
    }

    return status;
}

HAL_StatusTypeDef LIS302DL_CtrlReg3_Config(LIS302DL_HandleTypeDef *dev, uint8_t i1_cfg, uint8_t i2_cfg, uint8_t ppod, uint8_t ihl) {
    uint8_t actual_reg_value = dev->data.CTRL_REGS.CRTL_REG3; 

    actual_reg_value &= (uint8_t)~(LIS302DL_CR3_I1CFG_MASK | LIS302DL_CR3_I2CFG_MASK | LIS302DL_CR3_PP_OD | LIS302DL_CR3_IHL);

    uint8_t config_value = actual_reg_value | i1_cfg | i2_cfg | ppod | ihl;

    HAL_StatusTypeDef status = LIS302DL_SPI_WriteReg(dev, LIS302DL_REG_CTRL_REG3, config_value);

    if (status == HAL_OK) {
        dev->data.CTRL_REGS.CRTL_REG3 = config_value;
    }

    return status;
}

HAL_StatusTypeDef LIS302DL_FF_WU_CFG(LIS302DL_HandleTypeDef *dev, enum LIS302DL_Interrut_Block interrupt_block, uint8_t aoi, uint8_t lir, uint8_t zhie, uint8_t zlie, uint8_t yhie, uint8_t ylie, uint8_t xhie, uint8_t xlie) {
    struct FF_WU_CFGs *ff_wu_cfg;
    uint8_t reg_address;

    if (interrupt_block == LIS302DL_INT_BLOCK_1) {
        ff_wu_cfg = &dev->data.FF_WU_CFGS_1;
        reg_address = LIS302DL_REG_FF_WU_CFG_1;
    } else {
        ff_wu_cfg = &dev->data.FF_WU_CFGS_2;
        reg_address = LIS302DL_REG_FF_WU_CFG_2;
    }

    uint8_t actual_reg_value = ff_wu_cfg->FF_WU_CFG;

    actual_reg_value &= (uint8_t)~(LIS302DL_FF_WU_CFG_AOI | LIS302DL_FF_WU_CFG_LIR | 
                          LIS302DL_FF_WU_CFG_ZHIE | LIS302DL_FF_WU_CFG_ZLIE | 
                          LIS302DL_FF_WU_CFG_YHIE | LIS302DL_FF_WU_CFG_YLIE | 
                          LIS302DL_FF_WU_CFG_XHIE | LIS302DL_FF_WU_CFG_XLIE);

    uint8_t config_value = actual_reg_value | aoi | lir | zhie | zlie | yhie | ylie | xhie | xlie;

    HAL_StatusTypeDef status = LIS302DL_SPI_WriteReg(dev, reg_address, config_value);

    if (status == HAL_OK) {
        ff_wu_cfg->FF_WU_CFG = config_value;
    }

    return status;
}

HAL_StatusTypeDef LIS302DL_FF_WU_THS(LIS302DL_HandleTypeDef *dev, enum LIS302DL_Interrut_Block interrupt_block, uint8_t dcrm, uint8_t threshold) {
    struct FF_WU_CFGs *ff_wu_cfg;
    uint8_t reg_address;
    
    if (interrupt_block == LIS302DL_INT_BLOCK_1) {
        ff_wu_cfg = &dev->data.FF_WU_CFGS_1;
        reg_address = LIS302DL_REG_FF_WU_THS_1;
    } else {
        ff_wu_cfg = &dev->data.FF_WU_CFGS_2;
        reg_address = LIS302DL_REG_FF_WU_THS_2;
    }

    uint8_t actual_reg_value = ff_wu_cfg->FF_WU_THS;

    actual_reg_value &= (uint8_t)~(LIS302DL_FF_WU_THS_DCRM | LIS302DL_FF_WU_THS_MASK);

    uint8_t config_value = actual_reg_value | dcrm | (threshold & LIS302DL_FF_WU_THS_MASK);

    HAL_StatusTypeDef status = LIS302DL_SPI_WriteReg(dev, reg_address, config_value);

    if (status == HAL_OK) {
        ff_wu_cfg->FF_WU_THS = config_value;
    }

    return status;
}

HAL_StatusTypeDef LIS302DL_FF_WU_DURATION(LIS302DL_HandleTypeDef *dev, enum LIS302DL_Interrut_Block interrupt_block, uint8_t duration) {
    struct FF_WU_CFGs *ff_wu_cfg;
    uint8_t reg_address;
    
    if (interrupt_block == LIS302DL_INT_BLOCK_1) {
        ff_wu_cfg = &dev->data.FF_WU_CFGS_1;
        reg_address = LIS302DL_REG_FF_WU_DURATION_1;
    } else {
        ff_wu_cfg = &dev->data.FF_WU_CFGS_2;
        reg_address = LIS302DL_REG_FF_WU_DURATION_2;
    }

    /* Duration register utilizes only lower 8 bits */
    uint8_t config_value = duration & 0xFF; 

    HAL_StatusTypeDef status = LIS302DL_SPI_WriteReg(dev, reg_address, config_value);

    if (status == HAL_OK) {
        ff_wu_cfg->FF_WU_DURATION = config_value;
    }

    return status;
}

/* ========================================================================== */
/* 2. DATA ACQUISITION & ACCELERATION READING                                 */
/* ========================================================================== */

HAL_StatusTypeDef LIS302DL_Read_Status_Register(LIS302DL_HandleTypeDef *dev) {
    return LIS302DL_SPI_ReadRegs(dev, LIS302DL_REG_STATUS_REG, &dev->data.STATUS_REG, 1);
}

HAL_StatusTypeDef LIS302DL_Read_Acceleration(LIS302DL_HandleTypeDef *dev) {
    uint8_t buffer[6] = {0};
    
    /* * Auto-increment read for 6 bytes starting from LIS302DL_REG_OUT_X (0x29).
     * The bytes will map to:
     * buffer[0] -> OUT_X (0x29)
     * buffer[1] -> Dummy / Not used (0x2A)
     * buffer[2] -> OUT_Y (0x2B)
     * buffer[3] -> Dummy / Not used (0x2C)
     * buffer[4] -> OUT_Z (0x2D)
     * buffer[5] -> Dummy / Not used (0x2E)
     */
    HAL_StatusTypeDef status = LIS302DL_SPI_ReadRegs(dev, LIS302DL_REG_OUT_X, buffer, 6);

    if (status == HAL_OK) {
        dev->data.OUT_REGS.OUT_X = buffer[0];
        dev->data.OUT_REGS.OUT_Y = buffer[2];
        dev->data.OUT_REGS.OUT_Z = buffer[4];
    }

    return status;
}

HAL_StatusTypeDef LIS302DL_Read_FF_WU_SRC(LIS302DL_HandleTypeDef *dev, enum LIS302DL_Interrut_Block interrupt_block) {
    struct FF_WU_CFGs *ff_wu_cfg;
    uint8_t reg_address;

    if (interrupt_block == LIS302DL_INT_BLOCK_1) {
        ff_wu_cfg = &dev->data.FF_WU_CFGS_1;
        reg_address = LIS302DL_REG_FF_WU_SRC_1;
    } else {
        ff_wu_cfg = &dev->data.FF_WU_CFGS_2;
        reg_address = LIS302DL_REG_FF_WU_SRC_2;
    }

    return LIS302DL_SPI_ReadRegs(dev, reg_address, &ff_wu_cfg->FF_WU_SRC, 1);
}




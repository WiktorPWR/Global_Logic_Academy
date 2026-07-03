#include "accelerometer_functions.h"
#include "LIS302DL_driver/LIS302DL_Registers.h"

uint8_t new_data_available = 0;// this is flag for it


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == CS_I2C_SPI_Pin){
        //we have new data available from the accelerometer
        new_data_available = 1;
    }
}


HAL_StatusTypeDef Accelerometer_Init(LIS302DL_HandleTypeDef *dev){
    HAL_StatusTypeDef status;
    
    status = LIS302DL_Check_ID(dev);
    if(status != HAL_OK){
        return status;
    }

    status = LIS302DL_Init(dev);
    if(status != HAL_OK){
        return status;
    }

    status = LIS302DL_CtrlReg1_Config(dev, LIS302DL_CR1_DR_100HZ, LIS302DL_CR1_PD_ACTIVE, LIS302DL_CR1_FS_2G, LIS302DL_CR1_STP_DIS, LIS302DL_CR1_STM_DIS, LIS302DL_CR1_ZEN, LIS302DL_CR1_YEN, LIS302DL_CR1_XEN);
    if(status != HAL_OK){
        return status;
    }

    
    status = LIS302DL_CtrlReg2_Config(dev, 0x00, 0x00, 0x00, 0x00, 0x00);
    if(status != HAL_OK){
        return status;
    }

    
    status = LIS302DL_CtrlReg3_Config(dev, LIS302DL_CR3_I1CFG_DATA_READY, LIS302DL_CR3_I2CFG_GND, 0x00, 0x00);
    if(status != HAL_OK){
        return status;
    }

    return HAL_OK; 
}

HAL_StatusTypeDef Accelerometer_Read(LIS302DL_HandleTypeDef *dev){
    HAL_StatusTypeDef status;
    
    status = LIS302DL_Read_Acceleration(dev);
    if(status != HAL_OK){
        return status;
    }
    return HAL_OK;
}

HAL_StatusTypeDef Accelerometer_Status(LIS302DL_HandleTypeDef *dev){
    HAL_StatusTypeDef status;
    status = LIS302DL_Read_Status_Register(dev);
    if(status != HAL_OK){
        return status;
    }
    return HAL_OK;
}

HAL_StatusTypeDef Accelerometer_Read_Status_and_Values(LIS302DL_HandleTypeDef *dev){
    HAL_StatusTypeDef status;
    status = LIS302DL_Read_Status_Register(dev);
    if(status != HAL_OK){
        return status;
    }
    status = LIS302DL_Read_Acceleration(dev);
    if(status != HAL_OK){
        return status;
    }
    return HAL_OK;
}
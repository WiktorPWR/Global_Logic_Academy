#include "LIS302DL_Interface.h"
#include "LIS302DL_Registers.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

HAL_StatusTypeDef LIS302DL_Check_ID(void){
    uint8_t ID;
    HAL_SPI_Receive(&hspi1, &ID, 1, HAL_MAX_DELAY);
    if(ID == LIS302DL_ID){
        return HAL_OK;
    }
    else{
        return HAL_ERROR;
    }
}

HAL_StatusTypeDef LIS302DL_Data_Rate_Config(uint8_t data_rate){
    uint8_t reg_value = LIS302DL_REG_CTRL_REG1;
    HAL_SPI_Receive(&hspi1, &reg_value, 1, HAL_MAX_DELAY);
    if(data_rate == LIS302DL_CR1_DR_100HZ){
        reg_value &= ~LIS302DL_CR1_DR_MASK; // Clear DR bit for 100Hz
    }
    else if(data_rate == LIS302DL_CR1_DR_400HZ){
        reg_value |= LIS302DL_CR1_DR_MASK; // Set DR bit for 400Hz
    }
    else{
        return HAL_ERROR; // Invalid data rate
    }
    HAL_SPI_Transmit(&hspi1, &reg_value, 1, HAL_MAX_DELAY);
    return HAL_OK;
}
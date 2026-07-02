#include "LIS302DL_driver/LIS302DL_Interface.h"
#include "LIS302DL_driver/LIS302DL_Registers.h"
#include "LIS302DL_driver/LIS302DL_object.h"

extern SPI_HandleTypeDef hspi1;


/* ========================================================================== */
/* 1. CORE DEVICE MANAGEMENT & INITIALIZATION                                 */
/* ========================================================================== */


HAL_StatusTypeDef LIS302DL_Check_ID(void){
    uint8_t reg_value = LIS302DL_SPI_READ | LIS302DL_SPI_MS_STAY | LIS302DL_REG_WHO_AM_I;
    HAL_StatusTypeDef status;

    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);
    
    status = HAL_SPI_Transmit(&hspi1, &reg_value, 1, HAL_MAX_DELAY);
    if(status != HAL_OK){
        HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET);
        return status;
    }

    uint8_t ID;
    HAL_SPI_Receive(&hspi1, &ID, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET);

    if(ID == LIS302DL_ID){
        return HAL_OK;
    }
    else{
        return HAL_ERROR;
    }
}

HAL_StatusTypeDef LIS302DL_Core_Config(uint8_t data_rate, uint8_t power_mode, uint8_t full_scale, 
                                       uint8_t stp, uint8_t stm, uint8_t zen, uint8_t yen, uint8_t xen)
{
    HAL_StatusTypeDef status;
    uint8_t help_reg_value;
    uint8_t actual_reg_value = 0;

    // --- TRANSACTION 1: READ CURRENT VALUE OF CTRL_REG1 ---
    help_reg_value = LIS302DL_SPI_READ | LIS302DL_SPI_MS_STAY | LIS302DL_REG_CTRL_REG1;

    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(&hspi1, &help_reg_value, 1, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET);
        return status;
    }
    status = HAL_SPI_Receive(&hspi1, &actual_reg_value, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET);

    if(status != HAL_OK) {
        return status;
    }

    // --- BITMASKING: CLEAR ALL BITS THAT WE ARE GOING TO UPDATE ---
    // We clear all 8 bits of CTRL_REG1 using their specific bit masks
    actual_reg_value &= ~(LIS302DL_CR1_DR_MASK | LIS302DL_CR1_PD_MASK | LIS302DL_CR1_FS_MASK | 
                          LIS302DL_CR1_STP     | LIS302DL_CR1_STM     | LIS302DL_CR1_ZEN     | 
                          LIS302DL_CR1_YEN     | LIS302DL_CR1_XEN);

    // --- TRANSACTION 2: WRITE NEW CONFIGURATION TO CTRL_REG1 ---
    help_reg_value = LIS302DL_SPI_WRITE | LIS302DL_SPI_MS_STAY | LIS302DL_REG_CTRL_REG1;
    
    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(&hspi1, &help_reg_value, 1, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET);
        return status;
    }

    // Combine cleared old values with the new individual arguments passed by the user
    uint8_t config_value = actual_reg_value | data_rate | power_mode | full_scale | 
                           stp | stm | zen | yen | xen;
                           
    status = HAL_SPI_Transmit(&hspi1, &config_value, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET);

    return status;
}

/* ========================================================================== */
/* 2. DATA ACQUISITION & ACCELERATION READING                                 */
/* ========================================================================== */


HAL_StatusTypeDef LIS302DL_Read_XYZ_Values(int8_t *x, int8_t *y, int8_t *z){
    HAL_SPI_Receive(&hspi1, (uint8_t *)x, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, (uint8_t *)y, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, (uint8_t *)z, 1, HAL_MAX_DELAY);
    return HAL_OK;
}
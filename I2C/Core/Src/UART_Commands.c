#include "UART_Commands.h"
#include "PCA9685/Interface.h"
#include "main.h"
#include <stdio.h>  /* Required for sscanf */
#include <stdlib.h> /* Required for atoi */

extern UART_HandleTypeDef huart3;

HAL_StatusTypeDef CANAL_ON(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANAL_ON command received\r\n", 27, HAL_MAX_DELAY);

    int canal_number = 0;
    char tx_buffer[50];
    
    /* Parse the command string (e.g., "CANAL_ON 5") */
    if (sscanf((char *)data, "%*s %d", &canal_number) != 1) {
        HAL_UART_Transmit(&huart3, (uint8_t *)"Error: No canal number provided\r\n", 34, HAL_MAX_DELAY);
        return HAL_ERROR;
    }

    /* Print out which channel is being modified */
    int msg_len = sprintf(tx_buffer, "Turning ON channel: %d\r\n", canal_number);
    HAL_UART_Transmit(&huart3, (uint8_t *)tx_buffer, msg_len, HAL_MAX_DELAY);

    /* Set the PWM duty cycle for the specified canal to 100% (fully on) */
    canal_set_pwm_duty_cycle((uint8_t)canal_number, 100);
    
    return HAL_OK;
}

HAL_StatusTypeDef CANAL_OFF(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANAL_OFF command received\r\n", 28, HAL_MAX_DELAY);

    int canal_number = 0;
    char tx_buffer[50];

    /* Parse the command string (e.g., "CANAL_OFF 5") */
    if (sscanf((char *)data, "%*s %d", &canal_number) != 1) {
        HAL_UART_Transmit(&huart3, (uint8_t *)"Error: No canal number provided\r\n", 34, HAL_MAX_DELAY);
        return HAL_ERROR;
    }

    /* Print out which channel is being modified */
    int msg_len = sprintf(tx_buffer, "Turning OFF channel: %d\r\n", canal_number);
    HAL_UART_Transmit(&huart3, (uint8_t *)tx_buffer, msg_len, HAL_MAX_DELAY);

    /* Set the PWM duty cycle for the specified canal to 0% (fully off) */
    canal_set_pwm_duty_cycle((uint8_t)canal_number, 0);
    
    return HAL_OK;
}

HAL_StatusTypeDef CANAL_DUTY_CYCLE(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANAL_DUTY_CYCLE command received\r\n", 34, HAL_MAX_DELAY);

    int canal_number = 0;
    int duty_cycle_value = 0;
    char tx_buffer[60];

    /* Parse the command string (e.g., "CANAL_DUTY_CYCLE 5 75") */
    if (sscanf((char *)data, "%*s %d %d", &canal_number, &duty_cycle_value) != 2) {
        HAL_UART_Transmit(&huart3, (uint8_t *)"Error: Invalid parameters provided\r\n", 36, HAL_MAX_DELAY);
        return HAL_ERROR;
    }

    /* Print out which channel is being modified and the target duty cycle */
    int msg_len = sprintf(tx_buffer, "Setting channel %d duty cycle to: %d%%\r\n", canal_number, duty_cycle_value);
    HAL_UART_Transmit(&huart3, (uint8_t *)tx_buffer, msg_len, HAL_MAX_DELAY);

    /* Set the PWM duty cycle for the specified canal */
    canal_set_pwm_duty_cycle((uint8_t)canal_number, (uint8_t)duty_cycle_value);
    
    return HAL_OK;
}

HAL_StatusTypeDef CANALS_FREQUENCY(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANALS_FREQUENCY command received\r\n", 35, HAL_MAX_DELAY);

    int frequency_value = 0;

    /* Parse the command string (e.g., "CANALS_FREQUENCY 200") */
    if (sscanf((char *)data, "%*s %d", &frequency_value) != 1) {
        HAL_UART_Transmit(&huart3, (uint8_t *)"Error: No frequency value provided\r\n", 35, HAL_MAX_DELAY);
        return HAL_ERROR;
    }

    /* Set the PWM frequency globally for all channels (supports range up to 1000 Hz) */
    canals_set_pwm_frequency((uint16_t)frequency_value);

    return HAL_OK;
}

HAL_StatusTypeDef CANALS_ON(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANALS_ON command received\r\n", 30, HAL_MAX_DELAY);
    
    /* Set all 16 channels to 100% duty cycle simultaneously */
    canals_set_pwm_duty_cycle(100);
    
    return HAL_OK;
}

HAL_StatusTypeDef CANALS_OFF(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANALS_OFF command received\r\n", 31, HAL_MAX_DELAY);
    
    /* Set all 16 channels to 0% duty cycle simultaneously */
    canals_set_pwm_duty_cycle(0);
    
    return HAL_OK;
}


HAL_StatusTypeDef CANALS_SLEEP_SET_MODE(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANALS_SLEEP_SET_MODE command received\r\n", 39, HAL_MAX_DELAY);
    sleep_procedure();
    return HAL_OK;
}


HAL_StatusTypeDef CANALS_SLEEP_CLEAR_MODE(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANALS_SLEEP_CLEAR_MODE command received\r\n", 41, HAL_MAX_DELAY);
    wake_up_procedure();
    return HAL_OK;
}
#include "UART_Commands.h"


extern UART_HandleTypeDef huart3;

HAL_StatusTypeDef CANAL_ON(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANAL_ON command received\r\n", 27, HAL_MAX_DELAY);
    //we need to implement the logic for turning on the canal here
    return HAL_OK;
}
HAL_StatusTypeDef CANAL_OFF(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANAL_OFF command received\r\n", 28, HAL_MAX_DELAY);
    //we need to implement the logic for turning off the canal here
    return HAL_OK;
}
HAL_StatusTypeDef CANAL_DUTY_CYCLE(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANAL_DUTY_CYCLE command received\r\n", 34, HAL_MAX_DELAY);
    //we need to implement the logic for setting the duty cycle here
    return HAL_OK;
}
HAL_StatusTypeDef CANALS_FREQUENCY(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANALS_FREQUENCY command received\r\n", 35, HAL_MAX_DELAY);
    //we need to implement the logic for setting the frequency here
    return HAL_OK;
}
HAL_StatusTypeDef CANALS_ON(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANALS_ON command received\r\n", 30, HAL_MAX_DELAY);
    //we need to implement the logic for turning on all canals here
    return HAL_OK;
}
HAL_StatusTypeDef CANALS_OFF(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANALS_OFF command received\r\n", 31, HAL_MAX_DELAY);
    //we need to implement the logic for turning off all canals here
    return HAL_OK;
}

HAL_StatusTypeDef CANAL_STATUS(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANAL_STATUS command received\r\n", 31, HAL_MAX_DELAY);
    //we need to implement the logic for getting the status of a canal here
    return HAL_OK;
}

HAL_StatusTypeDef CANALS_STATUS(uint8_t *data, uint16_t size){
    HAL_UART_Transmit(&huart3, (uint8_t *)"CANALS_STATUS command received\r\n", 32, HAL_MAX_DELAY);
    //we need to implement the logic for getting the status of all canals here
    return HAL_OK;
}

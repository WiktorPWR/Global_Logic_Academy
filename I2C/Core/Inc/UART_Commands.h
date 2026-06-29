#ifndef UART_COMMANDS_H
#define UART_COMMANDS_H

#include "main.h"

HAL_StatusTypeDef CANAL_ON(uint8_t *data, uint16_t size);
HAL_StatusTypeDef CANAL_OFF(uint8_t *data, uint16_t size);
HAL_StatusTypeDef CANAL_DUTY_CYCLE(uint8_t *data, uint16_t size);
HAL_StatusTypeDef CANALS_FREQUENCY(uint8_t *data, uint16_t size);
HAL_StatusTypeDef CANALS_ON(uint8_t *data, uint16_t size);
HAL_StatusTypeDef CANALS_OFF(uint8_t *data, uint16_t size);

HAL_StatusTypeDef CANAL_STATUS(uint8_t *data, uint16_t size);
HAL_StatusTypeDef CANALS_STATUS(uint8_t *data, uint16_t size);

HAL_StatusTypeDef CANALS_SLEEP_SET_MODE(uint8_t *data, uint16_t size);
HAL_StatusTypeDef CANALS_SLEEP_CLEAR_MODE(uint8_t *data, uint16_t size);

#endif // UART_COMMANDS_H
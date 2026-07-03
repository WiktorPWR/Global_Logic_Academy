#ifndef ACCELEROMETER_FUNCTIONS_H
#define ACCELEROMETER_FUNCTIONS_H

#include "LIS302DL_driver/LIS302DL_Interface.h"

extern volatile uint8_t new_data_available; // this is flag for it



HAL_StatusTypeDef Accelerometer_Init(LIS302DL_HandleTypeDef * dev);
HAL_StatusTypeDef Accelerometer_Read(LIS302DL_HandleTypeDef * dev);
HAL_StatusTypeDef Accelerometer_Status(LIS302DL_HandleTypeDef * dev);
HAL_StatusTypeDef Accelerometer_Read_Status_and_Values(LIS302DL_HandleTypeDef *dev);

#endif
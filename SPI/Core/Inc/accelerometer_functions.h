#ifndef ACCELEROMETER_FUNCTIONS_H
#define ACCELEROMETER_FUNCTIONS_H

#include "LIS3DSHTR_driver/LIS3DSHTR_Interface.h"

extern volatile uint8_t new_data_available; // this is flag for it

HAL_StatusTypeDef LIS3DSH_Init(LIS3DSHTR_HandleTypeDef *dev, LIS3DSH_DataRate data_rate);

#endif
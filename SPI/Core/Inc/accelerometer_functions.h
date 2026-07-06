#ifndef ACCELEROMETER_FUNCTIONS_H
#define ACCELEROMETER_FUNCTIONS_H

#include "LIS3DSHTR_driver/LIS3DSHTR_Interface.h"

extern volatile uint8_t new_data_available; // this is flag for it

HAL_StatusTypeDef Accelerometer_Read(LIS3DSHTR_HandleTypeDef *dev);



#endif
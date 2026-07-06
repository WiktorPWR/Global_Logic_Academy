#include "accelerometer_functions.h"
#include "LIS3DSHTR_driver\LIS3DSHTR_Registers.h"

volatile uint8_t new_data_available = 0;// this is flag for it


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == MEMS_INT1_Pin){
        //we have new data available from the accelerometer
        new_data_available = 1;
    }
}



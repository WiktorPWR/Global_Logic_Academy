#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H

#include "main.h"

#define LED_PWM_FREQUENCY 1000
#define CLOCK_FREQUENCY 84000000

HAL_StatusTypeDef LED_Init(TIM_HandleTypeDef *htim);

typedef enum{
    LED_GREEN,
    LED_RED,
    LED_BLUE,
    LED_ORANGE
}LED_Color_t;


HAL_StatusTypeDef LED_PWM_DutyCycle(TIM_HandleTypeDef *htim,LED_Color_t color, uint8_t duty_cycle);

HAL_StatusTypeDef LED_Angles_Update(int16_t ax, int16_t ay, int16_t az,TIM_HandleTypeDef *htim);

#endif // LED_INTERFACE_H
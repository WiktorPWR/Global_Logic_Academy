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


HAL_StatusTypeDef LED_PWM_DutyCycle(LED_Color_t color, uint8_t duty_cycle);

#endif // LED_INTERFACE_H
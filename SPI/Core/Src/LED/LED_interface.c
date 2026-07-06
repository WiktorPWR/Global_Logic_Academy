#include "LED\LED_interface.h"
#include "main.h"


HAL_StatusTypeDef LED_Init(TIM_HandleTypeDef *htim) {
   
    uint32_t desired_timer_clock = 1000000; 

   
    uint32_t prescaler_value = (CLOCK_FREQUENCY / desired_timer_clock) - 1;

 
    uint32_t arr_value = (desired_timer_clock / LED_PWM_FREQUENCY) - 1;

 
    htim->Init.Prescaler         = (uint16_t)prescaler_value;
    htim->Init.Period            = arr_value;
    htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim->Init.RepetitionCounter = 0;

    if (HAL_TIM_PWM_Init(htim) != HAL_OK) {
        return HAL_ERROR;
    }

    if (HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1) != HAL_OK) return HAL_ERROR;
    if (HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2) != HAL_OK) return HAL_ERROR;
    if (HAL_TIM_PWM_Start(htim, TIM_CHANNEL_3) != HAL_OK) return HAL_ERROR;
    if (HAL_TIM_PWM_Start(htim, TIM_CHANNEL_4) != HAL_OK) return HAL_ERROR;

    return HAL_OK;
}
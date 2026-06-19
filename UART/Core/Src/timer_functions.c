#include "main.h"
#include "buttons_functions.h"

extern TIM_HandleTypeDef htim3;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   if(htim->Instance == TIM3)
   {
      HAL_TIM_Base_Stop_IT(&htim3);

      if(!HAL_GPIO_ReadPin(button_pressed.GPIO_Port, button_pressed.GPIO_Pin))
      {
        button_pressed.confirmation = 1;
      }else
      {
        button_pressed.confirmation = 0;
        button_pressed.GPIO_Pin = 0;
        button_pressed.GPIO_Port = NULL;
      }
      
   }

   Interrupts_GPIO_enable();
}
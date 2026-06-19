#include "buttons_functions.h"
#include "leds.h"

extern TIM_HandleTypeDef htim3;

volatile Button_pressed button_pressed = {0, NULL, 0};


/**
 * @brief  Disables GPIO external interrupts in the NVIC controller.
 * @details Blocks further interrupt requests from EXTI lines 5-9 and 10-15 to lock out 
 * bounce noise while a button press is being verified.
 * @retval None
 */
void Interrupts_GPIO_disable(void) {
    NVIC_DisableIRQ(EXTI9_5_IRQn);
    NVIC_DisableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief  Enables GPIO external interrupts in the NVIC controller.
 * @details Re-enables handling of interrupt requests from EXTI lines 5-9 and 10-15 
 * allowing the system to detect new button press events.
 * @retval None
 */
void Interrupts_GPIO_enable(void) {
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   switch (GPIO_Pin) {
        case SWT1_Pin:
            button_pressed.GPIO_Pin = SWT1_Pin;
            button_pressed.GPIO_Port = SWT1_GPIO_Port;
            button_pressed.confirmation = 0;
            break;
        case SWT2_Pin:
            button_pressed.GPIO_Pin = SWT2_Pin;
            button_pressed.GPIO_Port = SWT2_GPIO_Port;
            button_pressed.confirmation = 0;

            break;
        case SWT3_Pin:
            button_pressed.GPIO_Pin = SWT3_Pin;
            button_pressed.GPIO_Port = SWT3_GPIO_Port;
            button_pressed.confirmation = 0;
            break;
        case SWT4_Pin:
            button_pressed.GPIO_Pin = SWT4_Pin;
            button_pressed.GPIO_Port = SWT4_GPIO_Port;
            button_pressed.confirmation = 0;
            break;
        case SWT5_Pin:
            button_pressed.GPIO_Pin = SWT5_Pin;
            button_pressed.GPIO_Port = SWT5_GPIO_Port;
            button_pressed.confirmation = 0;
            break;
        default:
            break;
   }

   Interrupts_GPIO_disable();

   HAL_TIM_Base_Start_IT(&htim3);
}


void button_pressed_functionality(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin) {
        case SWT1_Pin:
            toggle_led_state(LD3_Pin, LD3_GPIO_Port);   
            break;
        case SWT2_Pin:
            toggle_all_leds();
            break;
        case SWT3_Pin:
            toggle_led_state(LD4_Pin, LD4_GPIO_Port);
            break;
        case SWT4_Pin:
            toggle_led_state(LD5_Pin, LD5_GPIO_Port);
            break;
        case SWT5_Pin:
            toggle_led_state(LD6_Pin, LD6_GPIO_Port);
            break;
        default:
            break;
    }

    button_pressed.confirmation = 0;
    button_pressed.GPIO_Pin = 0;
    button_pressed.GPIO_Port = NULL;
    Interrupts_GPIO_enable();
}
#include "buttons_functions.h"
#include "leds.h"
#include "uart.h"


volatile struct Button_pressed button_pressed = {0, NULL, 0};

volatile uint32_t last_press_time = 0;

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
    if(HAL_GetTick() - last_press_time < DEBOUNCE_TIME_MS){
        // If the time since the last confirmed button press is less than DEBOUNCE_TIME_MS, ignore this interrupt (debouncing)
        return;
    }
    last_press_time = HAL_GetTick();
   switch (GPIO_Pin) {
        case SWT1_Pin:
            button_pressed.GPIO_Pin = SWT1_Pin;
            button_pressed.GPIO_Port = SWT1_GPIO_Port;
            button_pressed.confirmation = 1;
            break;
        case SWT2_Pin:
            button_pressed.GPIO_Pin = SWT2_Pin;
            button_pressed.GPIO_Port = SWT2_GPIO_Port;
            button_pressed.confirmation = 1;

            break;
        case SWT3_Pin:
            button_pressed.GPIO_Pin = SWT3_Pin;
            button_pressed.GPIO_Port = SWT3_GPIO_Port;
            button_pressed.confirmation = 1;
            break;
        case SWT4_Pin:
            button_pressed.GPIO_Pin = SWT4_Pin;
            button_pressed.GPIO_Port = SWT4_GPIO_Port;
            button_pressed.confirmation = 1 ;
            break;
        case SWT5_Pin:
            button_pressed.GPIO_Pin = SWT5_Pin;
            button_pressed.GPIO_Port = SWT5_GPIO_Port;
            button_pressed.confirmation = 1;
            break;
        default:
            break;
   }
}


void button_pressed_functionality(uint16_t GPIO_Pin)
{
    enum Commands command;
    switch (GPIO_Pin) {
        case SWT1_Pin:
            toggle_led_state(LD3_Pin, LD3_GPIO_Port);   
            command = TOGGLE_BLUE_LED;
            break;
        case SWT2_Pin:
            toggle_all_leds();
            command = TOGGLE_ALL_LEDS;
            break;
        case SWT3_Pin:
            toggle_led_state(LD4_Pin, LD4_GPIO_Port);
            command = TOGGLE_RED_LED;
            break;
        case SWT4_Pin:
            toggle_led_state(LD5_Pin, LD5_GPIO_Port);
            command = TOGGLE_GREEN_LED;
            break;
        case SWT5_Pin:
            toggle_led_state(LD6_Pin, LD6_GPIO_Port);
            command = TOGGLE_ORANGE_LED;
            break;
        default:
            break;
    }

    print_led_status(command);

    button_pressed.confirmation = 0;
    button_pressed.GPIO_Pin = 0;
    button_pressed.GPIO_Port = NULL;
}
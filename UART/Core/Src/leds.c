#include "leds.h"

volatile struct LEDS leds = {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET};

void toggle_led_state(uint16_t GPIO_Pin, GPIO_TypeDef* GPIO_Port){
    if(GPIO_Pin == LD3_Pin)
    {
        if(leds.blue_led_status == GPIO_PIN_SET)
        {
            leds.blue_led_status = GPIO_PIN_RESET;
        }else{
            leds.blue_led_status = GPIO_PIN_SET;
        }
    }else if(GPIO_Pin == LD4_Pin)
    {
        if(leds.red_led_status == GPIO_PIN_SET)
        {
            leds.red_led_status = GPIO_PIN_RESET;
        }else{
            leds.red_led_status = GPIO_PIN_SET;
        }
    }else if(GPIO_Pin == LD5_Pin)
    {
        if(leds.orange_led_status == GPIO_PIN_SET)
        {
            leds.orange_led_status = GPIO_PIN_RESET;
        }else{
            leds.orange_led_status = GPIO_PIN_SET;
        }
    }else if(GPIO_Pin == LD6_Pin)
    {
        if(leds.green_led_status == GPIO_PIN_SET)
        {
            leds.green_led_status = GPIO_PIN_RESET;
        }else{
            leds.green_led_status = GPIO_PIN_SET;
        }
    }
    HAL_GPIO_TogglePin(GPIO_Port, GPIO_Pin);
}

void toggle_all_leds(){
    toggle_led_state(LD3_Pin, LD3_GPIO_Port);
    toggle_led_state(LD4_Pin, LD4_GPIO_Port);
    toggle_led_state(LD5_Pin, LD5_GPIO_Port);
    toggle_led_state(LD6_Pin, LD6_GPIO_Port);
}
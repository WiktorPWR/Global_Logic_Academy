#ifndef LEDS
#define LEDS

#include "main.h"

struct LEDs {
    GPIO_PinState blue_led_status;
    GPIO_PinState red_led_status;
    GPIO_PinState orange_led_status;
    GPIO_PinState green_led_status;
}

extern volatile struct LEDS leds;

void init_leds_states();

void toggle_all_leds();

void toggle_led_state(uint16_t GPIO_Pin, GPIO_TypeDef* GPIO_Port);

#endif
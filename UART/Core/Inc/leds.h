#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

struct LEDS {
    GPIO_PinState blue_led_status;
    GPIO_PinState red_led_status;
    GPIO_PinState orange_led_status;
    GPIO_PinState green_led_status;
};

extern volatile struct LEDS leds;

void toggle_led_state(uint16_t GPIO_Pin, GPIO_TypeDef* GPIO_Port);
void toggle_all_leds(void);

#endif /* LEDS_H */
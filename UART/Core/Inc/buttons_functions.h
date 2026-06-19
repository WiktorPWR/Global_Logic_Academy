#ifndef __BUTTONS_FUNCTIONS_H
#define __BUTTONS_FUNCTIONS_H

#include "main.h"


struct Button_pressed{
    uint16_t GPIO_Pin;
    GPIO_TypeDef* GPIO_Port;
    uint8_t confirmation;
};

extern volatile struct Button_pressed button_pressed;

void Interrupts_GPIO_disable(void);

void Interrupts_GPIO_enable(void);

void button_pressed_functionality(uint16_t GPIO_Pin);

#endif /* __BUTTONS_FUNCTIONS_H */
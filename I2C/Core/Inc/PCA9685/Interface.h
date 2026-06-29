#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

void canal_set_pwm_duty_cycle(uint8_t canal, uint8_t duty_cycle);

void canals_set_pwm_frequency(uint16_t frequency);

void canals_set_pwm_duty_cycle(uint8_t duty_cycle);

void canals_sleep_set_mode(uint8_t mode);

void wake_up_procedure(void);

void sleep_procedure(void);

void pca9685_init(void);

#endif
#ifndef UART_H
#define UART_H

#include "main.h"

#define RX_BUFFER_SIZE 10

#define TX_BUFFER_SIZE 100

extern uint8_t rx_buffer[RX_BUFFER_SIZE];

extern uint8_t tx_buffer[TX_BUFFER_SIZE];

enum Commands {
    TOGGLE_BLUE_LED = 0x01,
    TOGGLE_RED_LED = 0x02,
    TOGGLE_ORANGE_LED = 0x03,
    TOGGLE_GREEN_LED = 0x04,
    TOGGLE_ALL_LEDS = 0x05,
    WRONG_COMMAND = 0xFF,
};





#endif /* UART_H */
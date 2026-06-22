#ifndef UART_H
#define UART_H

#include "main.h"

// Zwiększyłem delikatnie bufor do 16 bajtów, aby bez problemu mieścił 
// komendę "LED ALL\r\n" oraz dawał lekki zapas na błędy terminala
#define RX_BUFFER_SIZE 16

#define TX_BUFFER_SIZE 100

// Sprzętowy bufor dla DMA
extern uint8_t rx_buffer[RX_BUFFER_SIZE];

// Główny bufor do składania poszatkowanych paczek w całe linie (Stream)
extern uint8_t main_buffer[RX_BUFFER_SIZE];
extern uint16_t main_idx;

// Bufor nadawczy
extern uint8_t tx_buffer[TX_BUFFER_SIZE];

enum Commands {
    TOGGLE_BLUE_LED = 0x01,
    TOGGLE_RED_LED = 0x02,
    TOGGLE_ORANGE_LED = 0x03,
    TOGGLE_GREEN_LED = 0x04,
    TOGGLE_ALL_LEDS = 0x05,
    WRONG_COMMAND = 0xFF,
};

void print_led_status(enum Commands command);

#endif /* UART_H */
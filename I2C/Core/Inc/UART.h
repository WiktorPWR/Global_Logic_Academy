#ifndef UART_H
#define UART_H

#define UART_RX_BUFFER_SIZE 256

extern volatile uint8_t uart3_rx_buffer[UART_RX_BUFFER_SIZE];

#endif
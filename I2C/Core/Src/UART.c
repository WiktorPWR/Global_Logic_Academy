#include "main.h"
#include "UART.h"

extern UART_HandleTypeDef huart3;

volatile uint8_t uart3_rx_buffer[UART_RX_BUFFER_SIZE];
volatile uint8_t uart3_rx_index = 0;

void process_uart3_data(uint8_t *data, uint16_t size, uint8_t *overflow_data, uint8_t overflow_size) {
    // Implement your data processing logic here
    // For example, you can print the received data or parse it
}

HAL_UARTEx_ReceiveToIdle_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size){
    if(huart == &huart3){
        
        if(uart3_rx_index > Size){
            uint8_t overflow_size = UART_RX_BUFFER_SIZE - uart3_rx_index;
            process_uart3_data(uart3_rx_buffer + uart3_rx_index, overflow_size, uart3_rx_buffer, Size);
        }else{
            process_uart3_data(uart3_rx_buffer + uart3_rx_index, Size - uart3_rx_index, NULL, NULL);
            
        }
        if(Size == UART_RX_BUFFER_SIZE){
            uart3_rx_index = 0;
        }else{
            uart3_rx_index = Size;
        }
    }
}


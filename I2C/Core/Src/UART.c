#include "main.h"
#include "UART.h"

extern UART_HandleTypeDef huart3;

volatile uint8_t uart3_rx_buffer[UART_RX_BUFFER_SIZE];
volatile uint8_t uart3_rx_index = 0;


HAL_StatusTypeDef process_uart_data(uint8_t *data, uint16_t size, uint8_t *overflow_data, uint8_t overflow_size) {
    if(data == NULL){
        return HAL_ERROR;
    }

    if(size == 0){
        return HAL_ERROR;
    }

    if(overflow_data == NULL){
        // Process the data normally
        //For now we just simply send a echo back the received data
        HAL_UART_Transmit(&huart3, data, size, HAL_MAX_DELAY);
        return HAL_OK;
    }else{
        uint16_t full_data_size = size + overflow_size;
        char full_data[full_data_size + 1]; // +1 for null terminator
    
        memcpy(full_data, data, size);
        
        if(overflow_size > 0){
            memcpy(full_data + size, overflow_data, overflow_size);
        }

        full_data[full_data_size] = '\0'; // Null terminate the string

        HAL_UART_Transmit(&huart3, (uint8_t *)full_data, full_data_size, HAL_MAX_DELAY);

        return HAL_OK;
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart == &huart3){
        if(uart3_rx_index > Size){
            uint8_t overflow_size = UART_RX_BUFFER_SIZE - uart3_rx_index;
            process_uart_data((uint8_t *)(uart3_rx_buffer + uart3_rx_index), overflow_size, (uint8_t *)uart3_rx_buffer, Size);
        }else{
            process_uart_data((uint8_t *)(uart3_rx_buffer + uart3_rx_index), Size - uart3_rx_index, NULL, 0);
            
        }
        if(Size == UART_RX_BUFFER_SIZE){
            uart3_rx_index = 0;
        }else{
            uart3_rx_index = Size;
        }
    }
}


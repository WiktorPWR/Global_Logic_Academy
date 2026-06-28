#include "main.h"
#include "UART.h"
#include "UART_Commands.h"

extern UART_HandleTypeDef huart3;

volatile uint8_t uart3_rx_buffer[UART_RX_BUFFER_SIZE];
volatile uint8_t uart3_rx_index = 0;

struct UART_Command{
    HAL_StatusTypeDef (*process_command)(uint8_t *data, uint16_t size);
    const char *command_name;
};

const struct UART_Command uart_commands[] = {
    {CANAL_ON, "CANAL_ON"},
    {CANAL_OFF, "CANAL_OFF"},
    {CANAL_DUTY_CYCLE, "CANAL_DUTY_CYCLE"},
    {CANALS_FREQUENCY, "CANALS_FREQUENCY"},
    {CANALS_ON, "CANALS_ON"},
    {CANALS_OFF, "CANALS_OFF"},
    {CANAL_STATUS, "CANAL_STATUS"},
    {CANALS_STATUS, "CANALS_STATUS"}
};

#define UART_COMMANDS_COUNT (sizeof(uart_commands) / sizeof(uart_commands[0]))


void parse_and_execute_command(const char* command, uint16_t size){
    for(int i = 0; i < UART_COMMANDS_COUNT; i++){
        if(strcmp(command, uart_commands[i].command_name) == 0){
            // Command found, execute it
            HAL_StatusTypeDef status = uart_commands[i].process_command((uint8_t *)command, size);
            if(status != HAL_OK){
                HAL_UART_Transmit(&huart3, (uint8_t *)"Error executing command\r\n", 26, HAL_MAX_DELAY);
            }
            return;
        }else{
            HAL_UART_Transmit(&huart3, (uint8_t *)"Unknown command\r\n", 17, HAL_MAX_DELAY);
        }
    }
}



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
        parse_and_execute_command((const char*)data, size);
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
        parse_and_execute_command((const char*)full_data, full_data_size);
        return HAL_OK;
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart == &huart3){
        HAL_StatusTypeDef status;
        if(uart3_rx_index > Size){
            uint8_t overflow_size = UART_RX_BUFFER_SIZE - uart3_rx_index;
            status = process_uart_data((uint8_t *)(uart3_rx_buffer + uart3_rx_index), overflow_size, (uint8_t *)uart3_rx_buffer, Size);
        }else{
            status = process_uart_data((uint8_t *)(uart3_rx_buffer + uart3_rx_index), Size - uart3_rx_index, NULL, 0);
        }

        if(status != HAL_OK){
            HAL_UART_Transmit(&huart3, (uint8_t *)"Error processing UART data\r\n", 29, HAL_MAX_DELAY);
        }

        if(Size == UART_RX_BUFFER_SIZE){
            uart3_rx_index = 0;
        }else{
            uart3_rx_index = Size;
        }
    }
}


#include "uart.h"
#include <string.h>
#include <stdio.h>

uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};

uint8_t tx_buffer[TX_BUFFER_SIZE] = {0};

extern UART_HandleTypeDef huart3;


static enum Commands UART_parse_data(uint8_t *data, uint16_t size){
    //The message formt is LED1 LED2 LED3 LED4 and ,,LED ALL"
    if(size < 4){
        return WRONG_COMMAND;
    }
    //first 3 bytes should be "LED"
    if(data[0] == 'L' && data[1] == 'E' && data[2] == 'D')
    {
        switch (data[3]) {
            case '1':
                return TOGGLE_BLUE_LED;
            case '2':
                return TOGGLE_RED_LED;
            case '3':
                return TOGGLE_ORANGE_LED;
            case '4':
                return TOGGLE_GREEN_LED;
            case ' ':
                if(data[4] == 'A' && data[5] == 'L' && data[6] == 'L')
                {
                    return TOGGLE_ALL_LEDS;
                }
                break;
            default:
                return WRONG_COMMAND;
        }
    }

    return WRONG_COMMAND;
}

// static void send_error_message(){
//     const char *error_message = "WRONG COMMAND\r\n";
//     HAL_UART_Transmit_DMA(&huart3, (uint8_t *)error_message, strlen(error_message));
// }

void print_led_status(enum Commands command){
    // while(huart3.gState != HAL_UART_STATE_READY){
    //     // Wait for the UART to be ready for transmission
    // }
    switch (command) {
        case TOGGLE_BLUE_LED:
            snprintf((char *)tx_buffer, sizeof(tx_buffer), "BLUE LED TOGGLED, STATE :%d \r\n", leds.blue_led_status);
            break;
        case TOGGLE_RED_LED:
            snprintf((char *)tx_buffer, sizeof(tx_buffer), "RED LED TOGGLED, STATE :%d \r\n", leds.red_led_status);
            break;
        case TOGGLE_ORANGE_LED:
            snprintf((char *)tx_buffer, sizeof(tx_buffer), "ORANGE LED TOGGLED, STATE :%d \r\n", leds.orange_led_status);
            break;
        case TOGGLE_GREEN_LED:
            snprintf((char *)tx_buffer, sizeof(tx_buffer), "GREEN LED TOGGLED, STATE :%d \r\n", leds.green_led_status);
            break;
        case TOGGLE_ALL_LEDS:
            snprintf((char *)tx_buffer, sizeof(tx_buffer), "ALL LEDS TOGGLED, STATE :%d ,%d, %d, %d \r\n", leds.blue_led_status, leds.red_led_status, leds.orange_led_status, leds.green_led_status);
            break;
        default:
            snprintf((char *)tx_buffer, sizeof(tx_buffer), "UNKNOWN COMMAND\r\n");
            break;
    }
    
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)tx_buffer, strlen((char *)tx_buffer));
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART3)
    {
        // Process received data in rx_buffer
        enum Commands command = UART_parse_data(rx_buffer, Size);
        switch (command)
        {
            case TOGGLE_BLUE_LED:
                toggle_led_state(LD3_Pin, LD3_GPIO_Port);
                break;
            case TOGGLE_RED_LED:
                toggle_led_state(LD4_Pin, LD4_GPIO_Port);
                break;
            case TOGGLE_ORANGE_LED:
                toggle_led_state(LD5_Pin, LD5_GPIO_Port);
                break;
            case TOGGLE_GREEN_LED:
                toggle_led_state(LD6_Pin, LD6_GPIO_Port);
                break;
            case TOGGLE_ALL_LEDS:
                toggle_all_leds();
                break;
            default:
                // send_error_message();
                break;
        }
        print_led_status(command);

        
        memset(rx_buffer, 0, RX_BUFFER_SIZE);

        // Restart the DMA reception for the next data
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_buffer, RX_BUFFER_SIZE);
    }
}



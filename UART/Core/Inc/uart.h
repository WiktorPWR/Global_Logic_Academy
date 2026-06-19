#ifndef UART_H
#define UART_H

enum Commands {
    TOGGLE_BLUE_LED = 0x01,
    TOGGLE_RED_LED = 0x02,
    TOGGLE_ORANGE_LED = 0x03,
    TOGGLE_GREEN_LED = 0x04,
    TOGGLE_ALL_LEDS = 0x05
};

enum Commands UART_parse_data(uint8_t *data, uint16_t size);




#endif /* UART_H */
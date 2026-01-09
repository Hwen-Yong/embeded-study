#ifndef UART_H
#define UART_H

#include "main.h"

#define UART_RXBUF_LEN 100

extern uint8_t UART_RX_BUFFER[UART_RXBUF_LEN];
extern uint8_t UART_RX_DATA;
#endif // !UART_H

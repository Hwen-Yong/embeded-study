#ifndef UART_H
#define UART_H

#include "main.h"

#define UART_RXBUF_LEN 100

extern uint8_t UART_RX_BUFFER[UART_RXBUF_LEN];
extern uint8_t UART_RX_DATA;

void HAL_UART_RxCpltFunction(UART_HandleTypeDef *huart,uint8_t pdata[], uint8_t size);
#endif // !UART_H

#include "uart.h"
#include "oled.h"
#include "usart.h"

uint8_t UART_RX_BUFFER[UART_RXBUF_LEN] = {};
uint8_t UART_RX_DATA;

static uint16_t rx_len = 0;

static uint8_t rx_state = 0; // 0:正常接收 1:接收到\r等待\n

void clear_buffer()
{
    // 清空接收缓冲区，并将rx_index置零
    while (rx_len-- > 1)
    {
        UART_RX_BUFFER[rx_len] = 0;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (UART_RX_DATA == '\r')
    {
        rx_state = 1;
    }
    else if (UART_RX_DATA == '\n' && rx_state == 1)
    {
        clear_buffer();
        rx_state = 0;
    }
    else
    {
        UART_RX_BUFFER[rx_len++] = UART_RX_DATA;

        // 如果RX_STATE==1 说明之前收到的时\r。但此时收到的不是\n
        if (rx_state == 1)
        {
            rx_state = 0;
            clear_buffer();
        }
    }
    HAL_UART_Receive_IT(&huart1, &UART_RX_DATA, 1);
}
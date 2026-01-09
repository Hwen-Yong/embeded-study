#include "uart.h"
#include "oled.h"
#include "usart.h"
#include "stdio.h"

uint8_t UART_RX_BUFFER[UART_RXBUF_LEN] = {};
uint8_t UART_RX_DATA;
#define UARTX huart1

static uint16_t rx_len = 0;

static uint8_t rx_state = 0; // 0:正常接收 1:接收到\r等待\n

/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)           /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t"); /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");   /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* MDK下需要重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *file)
{
    HAL_UART_Transmit(&UARTX, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

//GCC环境下需要重新_write函数
int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        HAL_UART_Transmit(&UARTX, (uint8_t *)&ptr[i], 1, HAL_MAX_DELAY);
    }
    return len;
}

#endif
/******************************************************************************************/

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
        HAL_UART_RxCpltFunction(&UARTX, UART_RX_BUFFER, rx_len);

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
    HAL_UART_Receive_IT(&UARTX, &UART_RX_DATA, 1);
}

__weak void HAL_UART_RxCpltFunction(UART_HandleTypeDef *huart, uint8_t pdata[], uint8_t size)
{
    // 接收到完整的一行数据，进行处理
    HAL_UART_Transmit(huart, pdata, rx_len, 1000);        // 回显收到的数据
    HAL_UART_Transmit(huart, (uint8_t *)"\r\n", 2, 1000); // 发送换行
}
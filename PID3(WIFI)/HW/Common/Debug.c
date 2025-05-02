#include "stm32f1xx_hal.h"
#include "usart.h"
#include <stdio.h>

/**
 * @brief  重定向 printf 到 UART1
 * @param  ch: 要发送的字符
 * @param  f: 文件指针（标准库要求）
 * @retval 发送的字符
 */
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

/**
 * @brief  初始化 Debug 功能（初始化 UART）
 */
void Debug_Init(void) {
    MX_USART1_UART_Init();
    MX_USART3_UART_Init();
}

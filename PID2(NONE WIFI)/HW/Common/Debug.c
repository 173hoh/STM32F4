#include "stm32f1xx_hal.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define UART_BUFFER_SIZE 64

uint8_t uart1_rx_data;                      // 单字节接收缓冲
uint8_t uart1_rx_buffer[UART_BUFFER_SIZE];  // 存放完整字符串
uint16_t uart1_rx_index = 0;                // 记录接收长度

/**
 * @brief  向 UART1 发送数据
 */
void UART1_Send(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(&huart1, data, size, HAL_MAX_DELAY);
}

/**
 * @brief  向 UART3 发送数据
 */
void UART3_Send(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(&huart3, data, size, HAL_MAX_DELAY);
}

/**
 * @brief  UART1 发送字符串（带格式化功能）
 */
void UART1_Printf(const char *format, ...) {
    char buffer[256];  // 发送缓冲区
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    UART1_Send((uint8_t *)buffer, strlen(buffer));  // 发送格式化后的字符串
}

/**
 * @brief  重定向 printf 到 UART3
 */
int fputc(int ch, FILE *f) {
    UART3_Send((uint8_t *)&ch, 1);  // 只向 UART3 发送调试信息
    return ch;
}

/**
 * @brief  初始化 UART1 和 UART3
 */
void Debug_Init(void) {
    MX_USART1_UART_Init();
    MX_USART3_UART_Init();
    HAL_UART_Receive_IT(&huart1, &uart1_rx_data, 1);  // 启动 UART1 接收中断
}

/**
 * @brief  UART1 接收回调函数：收到数据后转发到 UART3
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        // 处理接收到的字节 
        if (uart1_rx_data == '\n' || uart1_rx_index >= UART_BUFFER_SIZE - 1) {
            // 碰到换行符，或缓冲区满了，就打印整个字符串
            uart1_rx_buffer[uart1_rx_index] = '\0';  // 终止字符串
            printf("USART1 RX: %s\r\n", uart1_rx_buffer);

            // 清空缓冲区
            uart1_rx_index = 0;
        } else {
            // 继续存储数据
            uart1_rx_buffer[uart1_rx_index++] = uart1_rx_data;
        }

        // 重新启动 UART1 接收
        HAL_UART_Receive_IT(&huart1, &uart1_rx_data, 1);
    }
}

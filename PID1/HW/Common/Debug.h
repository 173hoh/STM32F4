#ifndef __DEBUG_H
#define __DEBUG_H

#include "usart.h"

extern UART_HandleTypeDef *USART_DEBUG;  // 让其他文件能访问 USART_DEBUG

void Debug_Init(void);

#endif

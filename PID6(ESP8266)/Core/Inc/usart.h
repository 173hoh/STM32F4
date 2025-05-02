#ifndef __USART_H
#define __USART_H

#include "stm32f1xx_hal.h"
#include "string.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);
void USART1_SendString(char *str);
void UsartPrintf(UART_HandleTypeDef USARTx, char *fmt,...);

#endif

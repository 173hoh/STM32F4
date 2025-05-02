/*******************************************************************************
  * 文件：Debug.c
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-8-10
  * 说明：调试打印功能，printf
*******************************************************************************/
#include "usart.h"
#include "Debug.h"

// 定义 USART_DEBUG，确保 onenet.c 能正确使用
UART_HandleTypeDef *USART_DEBUG = &huart2;

/*
重定向到 usart3
*/
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(USART_DEBUG, (uint8_t *)&ch, 1, 0xffff);
	return ch;
}

/*******************************************************************************
  * 函数名：Debug_Init
  * 功  能：初始化
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Debug_Init(void)
{
	MX_USART3_UART_Init();
}
/***********************************END OF FILE********************************/

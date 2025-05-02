#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "sys.h" 

	 void USART2_Config(uint32_t MyBaudRate);
	 void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
	 void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
	 void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
	 void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
   void USART2_IRQHandler(void);		 
#endif
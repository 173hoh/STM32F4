#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H 
#include "sys.h"	  	
#include "led.h"
#include "pid.h"
void uart1_init(u32 bound);
void USART1_IRQHandler(void);
#endif


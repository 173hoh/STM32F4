#ifndef __WS2812_H
#define __WS2812_H
#include "stm32f4xx.h"                  // Device header
void Timer1_Init(void);
void dma2_Init(void);
void WS2812B_send(uint32_t GRB_Data);
void LED_OFF(void);
void LED_ON(void);
void RGB_Show(uint8_t (*color)[3], uint16_t len);
#endif



#ifndef __OLED_H
#define __OLED_H

#include "stm32f1xx_hal.h"

//#define OLED_ADDRESS  0x3C << 1  // SSD1306 I2C µØÖ· (0x3C ×óÒÆ 1 Î»)

void OLED_Init();
void OLED_Clear();
void OLED_Show_String(uint8_t x, uint8_t y, char *str);

#endif

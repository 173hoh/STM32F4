#ifndef __ALLINIT_H
#define __ALLINIT_H

#include "stm32f10x.h"                  // Device header
#include "delay.h"

extern u8 temp;              // 存储温度值
extern u8 humi;                 // 存储湿度值
extern float ppm;

void Hardware_init(void);
void Hardware_read(void);
void Hardware_read(); 
#endif

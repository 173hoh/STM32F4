#ifndef WATER_H
#define WATER_H

#include "stm32f1xx_hal.h"

void WATER_Init(void);
uint16_t WATER_ReadRaw();
float WATER_ReadLEVEL(void);

#endif

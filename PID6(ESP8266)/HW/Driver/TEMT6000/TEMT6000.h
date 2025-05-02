#ifndef TEMT6000_H
#define TEMT6000_H

#include "stm32f1xx_hal.h"

void TEMT6000_Init(void);
uint16_t TEMT6000_ReadRaw(void);
float TEMT6000_ReadLux(void);

#endif

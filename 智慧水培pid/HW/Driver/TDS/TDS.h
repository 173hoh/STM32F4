#ifndef TDS_H
#define TDS_H

#include "stm32f1xx_hal.h"

void TDS_Init(void);
uint16_t TDS_ReadRaw();
float TDS_ReadTDS(void);

#endif

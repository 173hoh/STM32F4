#ifndef PH_H
#define PH_H

#include "stm32f1xx_hal.h"

void PH_Init(void);
uint16_t PH_ReadRaw(void);
float PH_Value_Conversion(void);

#endif

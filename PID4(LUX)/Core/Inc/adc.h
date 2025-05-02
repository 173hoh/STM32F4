#ifndef ADC_H
#define ADC_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"


extern ADC_HandleTypeDef hadc1;

void MX_ADC1_Init(void);

#endif

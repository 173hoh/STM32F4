#ifndef __MQ2_H
#define __MQ2_H

#include "stm32f10x.h"

/**
 * @brief MQ2 ≥ı ºªØ
 */
 void ADC_init(void);
 uint16_t ADC_GetStableValue(uint8_t ch);
 uint16_t mq2_get_adc_value(void);
uint16_t light_get_adc_value(void);
float mq2_get_voltage(void);
float light_get_voltage(void);
float mq2_get_ppm(void);
float light_get_lux(void);

#endif /* __MQ2_H */

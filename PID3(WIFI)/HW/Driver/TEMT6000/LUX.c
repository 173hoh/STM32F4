#include "LUX.h"
#include "adc.h"

#define TEMT6000_ADC_CHANNEL ADC_CHANNEL_5  // PA5 -> ADC1_IN5

void TEMT6000_Init(void) {
    MX_ADC1_Init();  // 确保 ADC 初始化
}

uint16_t TEMT6000_ReadRaw(void) {
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = TEMT6000_ADC_CHANNEL;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;  // 适用于 STM32F103

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t adcValue = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    return adcValue;
}

float TEMT6000_ReadLux(void) {
    uint16_t raw = TEMT6000_ReadRaw();
    float voltage = (3.3 * raw) / 4095.0;  // ADC 12-bit 分辨率
    float lux = voltage * 2000;  // 经验换算：1V ≈ 2000 Lux
    return lux;
}

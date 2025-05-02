#include "WATER.h"
#include "adc.h"

#define WATER_ADC_CHANNEL ADC_CHANNEL_3  // PA7 -> ADC1_IN7

void WATER_Init(void) {
    MX_ADC1_Init();  // 确保 ADC 初始化
}

uint16_t WATER_ReadRaw() {
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = WATER_ADC_CHANNEL;
    sConfig.Rank = 3;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;  // 适用于 STM32F103

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    HAL_ADC_GetValue(&hadc1);  // 读取一次但不使用

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    uint16_t adcValue = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    return adcValue;
}

float WATER_ReadLEVEL(void) {
    uint16_t raw = WATER_ReadRaw();
		//printf("raw=%d",raw);
    float voltage = (3.3* raw)/4095.0;  // ADC 12-bit 分辨率
		//printf("voltage=%f",voltage);
    float water_level = 100.0 * voltage - 20.0;  // 线性换算（示例参数）
	if (water_level < 0) {
        water_level = 0;  // 防止出现负值
    }
    return water_level;
}

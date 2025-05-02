#include "TDS.h"
#include "adc.h"

#define TDS_ADC_CHANNEL ADC_CHANNEL_7  // PA7 -> ADC1_IN7

void TDS_Init(void) {
    MX_ADC1_Init();  // 确保 ADC 初始化
}

uint16_t TDS_ReadRaw() {
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = TDS_ADC_CHANNEL;
    sConfig.Rank = 2;
    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;  // 适用于 STM32F103

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    uint16_t adcValue = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    return adcValue;
}

float TDS_ReadTDS(void) {
    uint16_t raw = TDS_ReadRaw();
		//printf("raw=%d",raw);
    float voltage = (3.3* raw)/4095.0;  // ADC 12-bit 分辨率
		//printf("voltage=%f",voltage);
    float tds = -865.86*voltage+2047.19-750;  // 经验换算：1V ≈ 200 Lux
	if(tds<100){tds=10;}
		//printf("tds=%f ppm\r\n",tds);
    return tds;
}

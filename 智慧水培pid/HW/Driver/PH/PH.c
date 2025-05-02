#include "PH.h"
#include "adc.h"
#define PH_ADC_CHANNEL ADC_CHANNEL_6  // PA5 -> ADC1_IN5

void PH_Init(void) {
    MX_ADC2_Init();  // 确保 ADC 初始化
}
uint16_t PH_ReadRaw()
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = PH_ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;  // 适用于 STM32F103

  HAL_ADC_ConfigChannel(&hadc2, &sConfig);

  HAL_ADC_Start(&hadc2);
  HAL_ADC_PollForConversion(&hadc2, 10);
  uint16_t adcValue = HAL_ADC_GetValue(&hadc2);
  HAL_ADC_Stop(&hadc2);

  return adcValue;
}

float PH_Value_Conversion()
{
		float PH_Value=0;
		float ADC_ConvertedValueLocal;  
		uint16_t ADC_ConvertedValue = PH_ReadRaw();
	  ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4095*3.3; // 读取转换的AD值
	
		PH_Value=-5.7541*ADC_ConvertedValueLocal+16.654;
	//printf("PH=%f",PH_Value);
	if(PH_Value<=0.0){PH_Value=0.0;}
	if(PH_Value>=14.0){PH_Value=14.0;}
	
		  /*显示电压*/
	return PH_Value;
	
}

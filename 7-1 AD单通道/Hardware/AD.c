#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC分频
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ADC专属输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//ADC规则组
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//如果是连续转换ADC_SoftwareStartConvCmd只用触发一次就可以了
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);//ADC结构体
	
	ADC_Cmd(ADC1, ENABLE);//ADC开启
	
	ADC_ResetCalibration(ADC1);//ADC校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//复位校准状态为0表示完成
	ADC_StartCalibration(ADC1);//启动校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);//等待校准是否完成
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//触发转换
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//状态标识位，EOC是规则组注入组转换完成标识位
	return ADC_GetConversionValue(ADC1);//ADC获取返转换值
}//启动等待读取一体化
 
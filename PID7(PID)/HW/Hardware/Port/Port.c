/*******************************************************************************
  * 文件：Port.c
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-10-3
  * 说明：端口配置初始化
*******************************************************************************/
#include "Port.h"
//#include "stm32f1xx_hal_gpio.h"
/*******************************************************************************
  * 函数名：Port_SetMode
  * 功  能：GPIO设置输入或输出模式
  * 参  数：*GPIOx 引脚组号
			GPIO_Pin引脚号
			u32Mode输入或输出模式
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Port_SetMode(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t u32Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = u32Mode;
	//GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
/***********************************END OF FILE********************************/
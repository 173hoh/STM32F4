/*******************************************************************************
  * 文件：DS18b20.h
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-8-10
  * 说明：无
*******************************************************************************/
#ifndef _DS18B20_H_
#define _DS18B20_H_
#include "Typedefine.h"
#include "Port.h"
#include "Debug.h"
/*******************************************************************************
Macro definitions
*******************************************************************************/
#define DS18B20_DQSet()					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
#define DS18B20_DQReset()				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
#define DS18B20_DQModeOutput()			Port_SetMode(GPIOB, GPIO_PIN_6, GPIO_MODE_OUTPUT_PP)
#define DS18B20_DQModeInput()			Port_SetMode(GPIOB, GPIO_PIN_6, GPIO_MODE_INPUT)
#define DS18B20_DIORead()				HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)
/*******************************************************************************
Global functions
*******************************************************************************/
void DS18B20_Init(void);
void DS18B20_Control(void);
uint32_t DS18B20_GetTemper(void);
bool DS18B20_GetErrState(void);
void DS18B20_Task100Ms(void);
void DS18B20_TaskSec(void);
#endif
/***********************************END OF FILE********************************/
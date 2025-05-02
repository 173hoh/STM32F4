/*******************************************************************************
  * 文件：PID.h
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-8-10
  * 说明：电加热加热水，温度控制，PID算法
*******************************************************************************/
#ifndef _PID_H_
#define _PID_H_

#include "gpio.h"

#define HeaterOn()					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define HeaterOff()					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)

typedef struct
{
	int32_t s32Error;//当前误差
    int32_t s32LastError;//上次误差
	int32_t s32ErrSum;//误差积分
	int32_t s32ErrDiffer;//当前误差的微分		
}PIDError_ts;

typedef union
{
	uint8_t byte;
	struct
	{
		uint8_t bPIDCycle:		1;//0控制周期到标志		
		uint8_t bHeaterOn:		1;//加热管开标志
		uint8_t b2:				1;
		uint8_t b3:				1;
		uint8_t b4:				1;
		uint8_t b5:				1;
		uint8_t b6:				1;
		uint8_t b7:				1;
	}bt;
}PIDFlag_tu;
void PID_Init(void);
bool PID_GetHeaterState(void);
void PID_ModifySetTemper(uint32_t u16Temper);
uint32_t PID_GetSetTemper(void);
void PID_Control(void);
void PID_Task100Ms(void);
#endif
/***********************************END OF FILE********************************/
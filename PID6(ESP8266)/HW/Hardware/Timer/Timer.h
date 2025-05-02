/*******************************************************************************
  * 文件：Timer.h
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-10-11
  * 说明：定时器
*******************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_
#include "Typedefine.h"
#include <stdbool.h>  // 确保 bool 被正确定义
/*******************************************************************************
Typedefine
*******************************************************************************/
typedef struct 
{
	uint16_t u16MsCount;
	uint8_t u8SecCount;
	uint8_t u8MinCount;
	uint8_t u8HourCount;
}TimerCount_ts;

typedef union
{	
	struct
	{
		uint8_t bPerMs:			1;
		uint8_t bPer2Ms:		1;
		uint8_t bPer5Ms:		1;
		uint8_t bPer10Ms:		1;
		uint8_t bPer20Ms:		1;
		uint8_t bPer50Ms:		1;
		uint8_t bPer100Ms:		1;
		uint8_t bPer200Ms:		1;
		uint8_t bPer500Ms:		1;
		uint8_t bPerSec:		1;
		uint8_t bPerMin:		1;
		uint8_t bPerHour:		1;
		uint8_t b12:			1;
		uint8_t b13:			1;
		uint8_t b14:			1;
		uint8_t b15:			1;
	}bt;
	struct
	{
		uint8_t lowb;		  
		uint8_t highb;
	}byte;
	uint16_t word;
}TimerFlag_tu;
/*******************************************************************************
Global Functions
*******************************************************************************/
void Timer_Init(void);
void Timer4_Init(void);
void Timer_Control(void);
bool Timer_GetMsState(void);
bool Timer_Get2MsState(void);
bool Timer_Get5MsState(void);
bool Timer_Get10MsState(void);
bool Timer_Get20MsState(void);
bool Timer_Get50MsState(void);
bool Timer_Get100MsState(void);
bool Timer_Get200MsState(void);
bool Timer_Get500MsState(void);
bool Timer_GetSecState(void);
bool Timer_GetMinState(void);
bool Timer_GetHourState(void);
void Timer_ClearMsState(void);
void Timer_Clear2MsState(void);
void Timer_Clear5MsState(void);
void Timer_Clear10MsState(void);
void Timer_Clear20MsState(void);
void Timer_Clear50MsState(void);
void Timer_Clear100MsState(void);
void Timer_Clear200MsState(void);
void Timer_Clear500MsState(void);
void Timer_ClearSecState(void);
void Timer_ClearMinState(void);
void Timer_ClearHourState(void);
void delay_us(uint16_t x);
#endif
/***********************************END OF FILE********************************/
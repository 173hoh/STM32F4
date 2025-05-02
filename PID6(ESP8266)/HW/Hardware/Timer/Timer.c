/*******************************************************************************
  * 文件：Timer.c
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-10-11
  * 说明：定时器
*******************************************************************************/
#include "Timer.h"
#include "tim.h"
#include <string.h>
#include "PID.h"
static TimerCount_ts sTimerCount;
static TimerFlag_tu uTimerState;
static TimerFlag_tu uTimerFlag;
/*******************************************************************************
  * 函数名：TimerInit
  * 功  能：定时器初始化
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Timer_Init(void)
{
	memset(&sTimerCount, 0, sizeof(sTimerCount));	
	uTimerState.word = 0;
	uTimerFlag.word = 0;
	HAL_TIM_Base_Start_IT(&htim2);	
}
void Timer4_Init(void)
{
//	memset(&sTimerCount, 0, sizeof(sTimerCount));	
//	uTimerState.word = 0;
//	uTimerFlag.word = 0;
	HAL_TIM_Base_Start_IT(&htim4);	
}
/*******************************************************************************
  * 函数名：Timer_Ms
  * 功  能：毫秒函数
  * 参  数：无
  * 返回值：无
  * 说  明：在1ms中断里运行
*******************************************************************************/
void Timer_Ms(void)
{
	sTimerCount.u16MsCount ++;
	uTimerState.bt.bPerMs = 1;//ms
	uTimerFlag.bt.bPerMs = 1;
	if ((sTimerCount.u16MsCount & 1) == 1)
	{
		uTimerFlag.bt.bPer2Ms = 1;//2ms
	}
	if((sTimerCount.u16MsCount % 5) == 0)
	{
		uTimerFlag.bt.bPer5Ms = 1;//5ms
		if((sTimerCount.u16MsCount % 10) == 0)
		{
			uTimerFlag.bt.bPer10Ms = 1;//10ms
		   	if((sTimerCount.u16MsCount % 20) == 0)
			{
				uTimerFlag.bt.bPer20Ms = 1;//20ms
				
			}
			if((sTimerCount.u16MsCount % 50) == 0)
			{
				uTimerFlag.bt.bPer50Ms = 1;//50ms
				if((sTimerCount.u16MsCount % 100) == 0)
				{
					uTimerFlag.bt.bPer100Ms = 1;//100ms
					if((sTimerCount.u16MsCount % 200) == 0)
					{
						uTimerFlag.bt.bPer200Ms = 1;//200ms						
					}
					if((sTimerCount.u16MsCount % 500) == 0)
					{
						uTimerFlag.bt.bPer500Ms = 1;//500ms						
					}
				}
			}
		}
	}	
	if (sTimerCount.u16MsCount >= 1000)
	{
		sTimerCount.u16MsCount = 0;
		uTimerState.bt.bPerSec = 1;
		uTimerFlag.bt.bPerSec = 1;
	}
}
void Timer4_Task(void)
{
    static uint32_t count = 0;
    count++;
    printf("TIM4 500ms 任务执行，第 %lu 次\n", count);
}

/*******************************************************************************
  * 函数名：HAL_TIM_PeriodElapsedCallback
  * 功  能：定时器中断
  * 参  数：无
  * 返回值：无
  * 说  明：1ms中断
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        Timer_Ms();  // 1ms 任务
    }
    else if (htim->Instance == TIM4)
    {
        Timer4_Task();  // 500ms 任务
    }
}

/*******************************************************************************
  * 函数名：Timer_Sec
  * 功  能：秒函数
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Timer_Sec(void)
{
	if(uTimerState.bt.bPerSec)
	{
		uTimerState.bt.bPerSec = 0;
		sTimerCount.u8SecCount ++;
		if(sTimerCount.u8SecCount >= 60)
		{
			sTimerCount.u8SecCount = 0;
			uTimerState.bt.bPerMin = 1;
			uTimerFlag.bt.bPerMin = 1;
		}
	}
}
/*******************************************************************************
  * 函数名：Timer_Min
  * 功  能：分钟函数
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Timer_Min(void)
{
	if(uTimerState.bt.bPerMin)
	{
		uTimerState.bt.bPerMin = 0;
		sTimerCount.u8MinCount ++;
		if(sTimerCount.u8MinCount >= 60)
		{
			sTimerCount.u8MinCount = 0;
			uTimerState.bt.bPerHour = 1;
			uTimerFlag.bt.bPerHour = 1;
		}
	}
}
/*******************************************************************************
  * 函数名：Timer_GetMsState
  * 功  能：返回各时间状态
  * 参  数：无
  * 返回值：uTimerState.bt.bPerMs等状态
  * 说  明：无
*******************************************************************************/
bool Timer_GetMsState(void)
{
	return (uTimerFlag.bt.bPerMs ? 1 : 0);
}
bool Timer_Get2MsState(void)
{
	return (uTimerFlag.bt.bPer2Ms ? 1 : 0);
}
bool Timer_Get5MsState(void)
{
	return (uTimerFlag.bt.bPer5Ms ? 1 : 0);
}
bool Timer_Get10MsState(void)
{
	return (uTimerFlag.bt.bPer10Ms ? 1 : 0);
}
bool Timer_Get20MsState(void)
{
	return (uTimerFlag.bt.bPer20Ms ? 1 : 0);
}
bool Timer_Get50MsState(void)
{
	return (uTimerFlag.bt.bPer50Ms ? 1 : 0);
}
bool Timer_Get100MsState(void)
{
	return (uTimerFlag.bt.bPer100Ms ? 1 : 0);
}
bool Timer_Get200MsState(void)
{
	return (uTimerFlag.bt.bPer200Ms ? 1 : 0);
}
bool Timer_Get500MsState(void)
{
	return (uTimerFlag.bt.bPer500Ms ? 1 : 0);
}
bool Timer_GetSecState(void)
{
	return (uTimerFlag.bt.bPerSec ? 1 : 0);
}
bool Timer_GetMinState(void)
{
	return (uTimerFlag.bt.bPerMin ? 1 : 0);
}
bool Timer_GetHourState(void)
{
	return (uTimerFlag.bt.bPerHour ? 1 : 0);
}
/*******************************************************************************
  * 函数名：Timer_ClearMsState等
  * 功  能：各状态清零
  * 参  数：无
  * 返回值：无
  * 说  明：获取该状态后须清零
*******************************************************************************/
void Timer_ClearMsState(void)
{
	uTimerFlag.bt.bPerMs = 0;
}
void Timer_Clear2MsState(void)
{
	uTimerFlag.bt.bPer2Ms = 0;
}
void Timer_Clear5MsState(void)
{
	uTimerFlag.bt.bPer5Ms = 0;
}
void Timer_Clear10MsState(void)
{
	uTimerFlag.bt.bPer10Ms = 0;
}
void Timer_Clear20MsState(void)
{
	uTimerFlag.bt.bPer20Ms = 0;
}
void Timer_Clear50MsState(void)
{
	uTimerFlag.bt.bPer50Ms = 0;
}
void Timer_Clear100MsState(void)
{
	uTimerFlag.bt.bPer100Ms = 0;
}
void Timer_Clear200MsState(void)
{
	uTimerFlag.bt.bPer200Ms = 0;
}
void Timer_Clear500MsState(void)
{
	uTimerFlag.bt.bPer500Ms = 0;
}
void Timer_ClearSecState(void)
{
	uTimerFlag.bt.bPerSec = 0;
}
void Timer_ClearMinState(void)
{
	uTimerFlag.bt.bPerMin = 0;
}
void Timer_ClearHourState(void)
{
	uTimerFlag.bt.bPerHour = 0;
}
/*******************************************************************************
  * 函数名：Timer_Control
  * 功  能：定时控制函数
  * 参  数：无
  * 返回值：无
  * 说  明：在主循环中运行，保证秒和分钟函数持续计时
*******************************************************************************/
void Timer_Control(void)
{
	Timer_Sec();
	Timer_Min();
}
/*******************************************************************************
  * 函数名：delay_us
  * 功  能：us延时
  * 参  数：x延时时间
  * 返回值：无
  * 说  明：使用TIM3
*******************************************************************************/
void delay_us(uint16_t x)
{
	HAL_TIM_Base_Start(&htim3);
	__HAL_TIM_SetCounter(&htim3,0);// 对上次延时产生的计数清零
	while( x > __HAL_TIM_GetCounter(&htim3) ) 
	{
	}
	HAL_TIM_Base_Stop(&htim3);
}
/***********************************END OF FILE********************************/
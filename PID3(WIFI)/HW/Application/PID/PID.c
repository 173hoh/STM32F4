/*******************************************************************************
  * 文件：PID.c
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-8-10
  * 说明：电加热加热水，温度控制，PID算法
*******************************************************************************/
#include "Timer.h"
#include "DS18B20.h"
#include "PID.h"
#include "string.h"
#include "Debug.h"

#define HEATER_ON											0x01
#define HEATER_OFF											0x00
#define SET_TEMPER_DEFAULT									(45*100)//默认设定45
#define PID_CONTROL_PERIOD									(600)//PID调节周期，单位100ms
#define DELAY_TIME											 30//*100ms,延时，温度稳定后再控制
static uint8_t u8PID_Delay100Ms;//上电延时，温度稳定后再控制
static uint16_t u32PID_SetTemper;//设定温度*10
static uint16_t u16PID_HeaterOn100Ms;//电加热需要开启的时间
static uint16_t u16PID_Cycle100Ms;//PID周期计时
static PIDError_ts sPID_Para;
static PIDFlag_tu uPID_Flag;

const uint16_t u16PID_Coefficient[] =//放大100倍
{
	4000,//Kp
	1,//Ki
	0//Kd
};
/*******************************************************************************
  * 函数名：PID_SetHeaterState
  * 功  能：开关电加热
  * 参  数：bState电加热状态
  * 返回值：无
  * 说  明：同时设置标志位
*******************************************************************************/
void PID_SetHeaterState(bool bState)
{
	if (bState == HEATER_OFF)
	{
		HeaterOff();
		uPID_Flag.bt.bHeaterOn = 0;
	}else
	{
		HeaterOn();
		uPID_Flag.bt.bHeaterOn = 1;
	}
}
/*******************************************************************************
  * 函数名：PID_GetHeaterState
  * 功  能：返回电加热状态
  * 参  数：uPID_Flag.bt.bHeaterOn
  * 返回值：无
  * 说  明：无
*******************************************************************************/
bool PID_GetHeaterState(void)
{
	return (uPID_Flag.bt.bHeaterOn ? 1 : 0);
}
/*******************************************************************************
  * 函数名：PID_ModifySetTemper
  * 功  能：修改设定温度
  * 参  数：u32Temper温度值
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void PID_ModifySetTemper(uint32_t u32Temper)
{
	u32PID_SetTemper = u32Temper;
}
/*******************************************************************************
  * 函数名：PID_GetSetTemper
  * 功  能：获取设定温度
  * 参  数：u32PID_SetTemper设定温度值
  * 返回值：无
  * 说  明：无
*******************************************************************************/
uint32_t PID_GetSetTemper(void)
{
	return u32PID_SetTemper;
}
/*******************************************************************************
  * 函数名：PID_HeaterOnTimeCalculate
  * 功  能：电加热开启时间计算
  * 参  数：u32SetTemper设定温度
			u32WaterTemper实际水温
  * 返回值：u16HeaterOn100ms开启时间，单位100ms
  * 说  明：位置型PID算法
*******************************************************************************/
uint16_t PID_HeaterOnTimeCalculate(uint32_t u32SetTemper, uint32_t u32WaterTemper)
{
	uint16_t u16HeaterOn100ms = 0;
	int32_t s32Percent = 0;//输出的百分比*100，占空比
	
	sPID_Para.s32Error = (int32_t)u32SetTemper - (int32_t)u32WaterTemper;//误差
	if (sPID_Para.s32Error > (5 * 100))//误差大，输出100%
	{
		s32Percent = 100;
		memset(&sPID_Para, 0x00, sizeof(sPID_Para));//PID参数清零
	}else if (sPID_Para.s32Error < (-5 * 100))//负误差过大，输出0
	{
		s32Percent = 0;
		memset(&sPID_Para, 0x00, sizeof(sPID_Para));//PID参数清零
	}else
	{
		sPID_Para.s32ErrSum	+= sPID_Para.s32Error;//积分
		sPID_Para.s32ErrDiffer = sPID_Para.s32Error - sPID_Para.s32LastError;//误差微分
		s32Percent = (int32_t)u16PID_Coefficient[0] * sPID_Para.s32Error//比例
					+ (int32_t)u16PID_Coefficient[1] * sPID_Para.s32ErrSum//积分
					+ (int32_t)u16PID_Coefficient[2] * sPID_Para.s32ErrDiffer;//微分
		s32Percent /= 10000;//系数*100，温度*100，所以要除以1000
		sPID_Para.s32LastError = sPID_Para.s32Error;//保存上次误差
		if (s32Percent < 5)//最少5%，防止继电器开启时间过短
		{
			s32Percent = 0;
		}else
		{
			if (s32Percent > 100)
			{
				s32Percent = 100;
			}
		}
	}	
	u16HeaterOn100ms = (uint16_t)(s32Percent * PID_CONTROL_PERIOD / 100);//百分比应除以100
	return u16HeaterOn100ms;
}
/*******************************************************************************
  * 函数名：App_PID_CycleProcess
  * 功  能：控制周期定时计算
  * 参  数：无
  * 返回值：无
  * 说  明：100ms运行一次
*******************************************************************************/
void PID_CycleProcess(void)
{
	float fTemper = 0.0;
	
	if (u8PID_Delay100Ms >= DELAY_TIME)//延时到
	{
		if (u16PID_Cycle100Ms < PID_CONTROL_PERIOD)
		{
			u16PID_Cycle100Ms++;		
		}else
		{
			u16PID_Cycle100Ms = 0;
			uPID_Flag.bt.bPIDCycle = 0;//控制周期到
		}
		if (((u16PID_Cycle100Ms % 10) == 0) && (u16PID_Cycle100Ms > 0))//每秒打印一次温度
		{
			fTemper = (float)DS18B20_GetTemper() / 100.0;
			printf(" %.1f", fTemper);
		}
	}else
	{
		u16PID_Cycle100Ms = 0;
	}
}
/*******************************************************************************
  * 函数名：PID_HeaterControl
  * 功  能：电加热控制
  * 参  数：无
  * 返回值：无
  * 说  明：计算出需要开启的时间，再与运行时间比较
*******************************************************************************/
void PID_HeaterControl(void)
{
	uint32_t u32WaterTemper = 0;
	if (u8PID_Delay100Ms >= DELAY_TIME)//延时到
	{
		if (!uPID_Flag.bt.bPIDCycle)//控制周期到
		{
			uPID_Flag.bt.bPIDCycle = 1;
			u32WaterTemper = DS18B20_GetTemper();
			u16PID_HeaterOn100Ms = PID_HeaterOnTimeCalculate(u32PID_SetTemper, u32WaterTemper);
		}
		if (u16PID_Cycle100Ms <= u16PID_HeaterOn100Ms)//一个周期内，已过的时间小于加热管应该开启的时间
		{
			if (u16PID_HeaterOn100Ms > 0)//防止两个都等于0时加热管开
			{
				PID_SetHeaterState(HEATER_ON);
			}
		}else
		{
			PID_SetHeaterState(HEATER_OFF);
		}
	}	
}
/*******************************************************************************
  * 函数名：PID_Delay100MsProcess
  * 功  能：延时时间处理
  * 参  数：无
  * 返回值：无
  * 说  明：上电延时几秒，待温度稳定后再控制加热管
*******************************************************************************/
void PID_Delay100MsProcess(void)
{
	if (u8PID_Delay100Ms < DELAY_TIME)
	{
		u8PID_Delay100Ms++;
	}
}
/*******************************************************************************
  * 函数名：PID_Init
  * 功  能：初始化
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void PID_Init(void)
{
	u32PID_SetTemper = SET_TEMPER_DEFAULT;
	memset(&sPID_Para, 0x00, sizeof(sPID_Para));
	u8PID_Delay100Ms = 0;
	u16PID_HeaterOn100Ms = 0;  
	uPID_Flag.byte = 0;
	u16PID_Cycle100Ms = 0;
	PID_SetHeaterState(HEATER_OFF);
}
/*******************************************************************************
  * 函数名：PID_Task100Ms
  * 功  能：每10ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void PID_Task100Ms(void)
{
	PID_Delay100MsProcess();
	PID_CycleProcess();
	PID_HeaterControl();
}
/***********************************END OF FILE********************************/
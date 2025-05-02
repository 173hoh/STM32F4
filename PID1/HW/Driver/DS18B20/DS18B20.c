/*******************************************************************************
  * 文件：DS18B20.c
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-8-10
  * 说明：温度传感器DS18B20驱动
*******************************************************************************/
#include "DS18b20.h"
//#include "Func.h"
#include "Timer.h"
#include "string.h"

static uint32_t u32DS18B20_RawData[6];//未转换的数据
static uint32_t u32DS18B20_Temper;
static bool bDS18B20_ErrState;//Ds18b20不存在为故障
static uint8_t u8DS18B20_ErrSec;
/*******************************************************************************
  * 函数名：DS18B20_set
  * 功  能：DS18B20复位
  * 参  数：无
  * 返回值：无
  * 说  明：复位
*******************************************************************************/
void DS18B20_Reset(void)
{
	DS18B20_DQModeOutput();//设置为输出
	DS18B20_DQReset();//低电平
	delay_us(750);//750us
	DS18B20_DQSet();//高电平
	delay_us(15);//15us	
}
/*******************************************************************************
  * 函数名：DS18B20_Check
  * 功  能：检测DS18B20是否存在
  * 参  数：无
  * 返回值：1不存在，0存在
  * 说  明：无
*******************************************************************************/
uint8_t DS18B20_Check(void)
{
	uint8_t u8Retry = 0;
	DS18B20_DQModeInput();//设置为输入
	while ((DS18B20_DIORead() == 1)&&(u8Retry < 200))
	{
		u8Retry++;
		delay_us(1);
	}
	
	if (u8Retry >= 200)
	{
		return 1;
	}else
	{
		u8Retry = 0;
	}
	while ((DS18B20_DIORead() == 0) && (u8Retry < 240))
	{
		u8Retry++;
		delay_us(1);
	}
	if(u8Retry >= 120)
	{
		return 1;
	}
	return 0;	
}
/*******************************************************************************
  * 函数名：DS18B20_WriteByte
  * 功  能：向DS18B20写入一个字节
  * 参  数：u8Data:要写入的数据
  * 返回值：无
  * 说  明：
*******************************************************************************/
void DS18B20_WriteByte(uint8_t u8Data)
{
	uint8_t tempIndex,tempData;
	DS18B20_DQModeOutput();//设置为输出
	for (tempIndex = 1; tempIndex <= 8; tempIndex++)
	{
		tempData = (u8Data & 0x01);
		u8Data >>= 1;
		if (tempData == 1)
		{
			DS18B20_DQReset();//低电平
			delay_us(2);
			DS18B20_DQSet();//高电平
			delay_us(60);//延时60us
		}else
		{
			DS18B20_DQReset();//低电平
			delay_us(60);//延时60us
			DS18B20_DQSet();//高电平
			delay_us(2);
		}		
	}
}
/*******************************************************************************
  * 函数名：DS18B20_ReadBit
  * 功  能：从DS18B20读取一个位
  * 参  数：无
  * 返回值：1或0
  * 说  明：无
*******************************************************************************/
uint8_t DS18B20_ReadBit(void)
{
	uint8_t u8Data = 0;
	DS18B20_DQModeOutput();//设置为输出
	DS18B20_DQReset();//低电平
	delay_us(2);
	DS18B20_DQSet();//高电平
	DS18B20_DQModeInput();//设置为输入
	delay_us(12);
	u8Data = ((DS18B20_DIORead() == 1) ? 1 : 0);
	delay_us(50);
	return u8Data;
}
/*******************************************************************************
  * 函数名：DS18B20_ReadByte
  * 功  能：从DS18B20读取一个字节
  * 参  数：无
  * 返回值：u8Data读出的数据
  * 说  明：无
*******************************************************************************/
uint8_t DS18B20_ReadByte(void)
{
	uint8_t i,j, u8Ddata = 0;
	
	for (i = 1; i <= 8; i++)
	{		
		j = DS18B20_ReadBit();
		u8Ddata = (j << 7) | (u8Ddata >> 1);
	}	
	return u8Ddata;
}
/*******************************************************************************
  * 函数名：DS18B20_Start
  * 功  能：开始温度转换
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void DS18B20_Start(void)
{
	DS18B20_Reset();
	DS18B20_Check();
	DS18B20_WriteByte(0xCC);//跳过ROM
	DS18B20_WriteByte(0x44);//温度转换	
}
/*******************************************************************************
  * 函数名：DS18B20_Init
  * 功  能：初始化
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void DS18B20_Init(void)
{
	bDS18B20_ErrState = 0;
	u8DS18B20_ErrSec = 0;
	memset(&u32DS18B20_RawData, 0x00, sizeof(u32DS18B20_RawData));
	DS18B20_DQModeOutput();//设置为输出
	DS18B20_DQSet();//高电平
	DS18B20_Reset();
	//return Drv_Ds18b20Check();
	bDS18B20_ErrState = DS18B20_Check();
}
/*******************************************************************************
  * 函数名：DS18B20_DataMove
  * 功  能：数据滑动一次
  * 参  数：数组及数组长度
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void DS18B20_DataMove(uint32_t *dataPtr, uint8_t length)
{
	uint8_t i;
	for (i = 0; i < (length - 1); i ++)
	{
	 	*(dataPtr + i) = *(dataPtr + i + 1);
	}
}
/*******************************************************************************
  * 函数名：DS18B20_MedianAverageFilter
  * 功  能：中值平均滤波算法
  * 参  数：Array[]序列
  * 返回值：滤波后的值
  * 说  明：为作除法简便，本函数输入参数长度为2^n+2，本程序中长度为6;
  			与DS18B20_DataMove函数构成滑动中值平均滤波算法
*******************************************************************************/ 
uint32_t DS18B20_MedianAverageFilter(uint32_t Array[])
{
	uint8_t tempIndex;
	uint32_t max, min, sum;
	
	max = 0x00;
	min = 0x07FF;
	sum = 0x00;
	for(tempIndex = 0; tempIndex < 6; tempIndex++)
	{
		if (Array[tempIndex] > max)
		{
			max = Array[tempIndex];
		}
		if (Array[tempIndex] < min)
		{
			min = Array[tempIndex];
		}
		sum += Array[tempIndex];
	}
	sum -= min;
	sum -= max;
	sum = sum >> 2;
	return sum;
}
/*******************************************************************************
  * 函数名：DS18B20_TemperProcess
  * 功  能：温度处理
  * 参  数：无
  * 返回值：无
  * 说  明：滤波
*******************************************************************************/
void DS18B20_TemperProcess(void)
{
	uint8_t tempLSB,tempMSB;
	uint32_t temp;
	DS18B20_DataMove(u32DS18B20_RawData, (sizeof(u32DS18B20_RawData))>>2);
	DS18B20_Start();
	DS18B20_Reset();
	DS18B20_Check();
	DS18B20_WriteByte(0xCC);//跳过ROM	
	DS18B20_WriteByte(0xBE);
	tempLSB = DS18B20_ReadByte();
	tempMSB = DS18B20_ReadByte();	
	u32DS18B20_RawData[5] = ((uint32_t)tempMSB << 8) + (uint32_t)tempLSB;
	u32DS18B20_RawData[5] &= 0x07FF;
	temp = DS18B20_MedianAverageFilter(u32DS18B20_RawData);
	u32DS18B20_Temper = (temp * 625 /100);//应该*0.0625,扩大100倍,*6.25,
}
/*******************************************************************************
  * 函数名：DS18B20_GetTemper
  * 功  能：返回温度
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
uint32_t DS18B20_GetTemper(void)
{
	return u32DS18B20_Temper;
}
/*******************************************************************************
  * 函数名：DS18B20_ErrStateProcess
  * 功  能：DS18B20不存在认为是故障
  * 参  数：无
  * 返回值：无
  * 说  明：每秒检测一次,连续3s不存在认为故障
*******************************************************************************/
void DS18B20_ErrStateProcess(void)
{
	if (DS18B20_Check() == 1)
	{
		if (u8DS18B20_ErrSec < 3)
		{	
			u8DS18B20_ErrSec++;
		}
	}else
	{
		u8DS18B20_ErrSec = 0;
	}
	if (u8DS18B20_ErrSec >= 3)
	{
		bDS18B20_ErrState = 1;
	}
}
/*******************************************************************************
  * 函数名：DS18B20_GetErrState
  * 功  能：返回故障状态
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
bool DS18B20_GetErrState(void)
{
	return bDS18B20_ErrState;
}
/*******************************************************************************
  * 函数名：DS18B20_Task100Ms
  * 功  能：每100ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void DS18B20_Task100Ms(void)
{
	DS18B20_TemperProcess();
}
/*******************************************************************************
  * 函数名：DS18B20_TaskSec
  * 功  能：每秒执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void DS18B20_TaskSec(void)
{
	DS18B20_ErrStateProcess();
}
/***********************************END OF FILE********************************/
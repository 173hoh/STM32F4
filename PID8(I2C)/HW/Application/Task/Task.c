#include "cJSON.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h" // 添加标准整数类型定义
#include "DS18B20.h"
#include "PID.h"
#include "TEMT6000.h"
#include "TDS.h"
#include "DHT11.h" 
#include "PH.h"
#include "WATER.h"

extern float fTemper;
volatile uint8_t disable_tim4_task = 0;  // 0: 允许执行任务, 1: 禁止执行任务
int Temp, Tds,Water;  
int Humi, Lux;  
int PH;

char formattedJson[256];

char text[] = "{\"Temp\":%d,\"Humi\":%d,\"PH\":%d,\"Tds\":%d,\"Lux\":%d,\"Water\":%d}";

void send(char str[])
{
  UART1_Printf("%s", str);
	printf("%s",str);
	
}

// 生成 JSON 格式的虚拟数据
//void update_sensor_data()
//{
//		uint8_t humidity, temperature;
//    uint16_t Lux      = 	(uint16_t) TEMT6000_ReadLux();     
//    uint16_t Humi     =		(uint16_t) DHT11_Task(humidity, temperature);     
//    uint16_t Tds      = 	(uint16_t) TDS_ReadTDS();
//    uint16_t Temp 		= 	(uint16_t) DS18B20_TaskSec();    
//    uint16_t PH       = 	(uint16_t)	PH_Value_Conversion();
//	
//    sprintf(formattedJson, text,Temp,Humi,PH,Tds,Lux);
//    send(formattedJson);
//}

/*******************************************************************************
  * 函数名：Task_Init
  * 功  能：初始化
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Task_Init(void)
{
	Timer_Init();
	
	TEMT6000_Init();
	DHT11_Init();
	TDS_Init();
	PH_Init();
	DS18B20_Init();
	WATER_Init();
	//Display_Init();
	//Key_Init();
	 PID_Init();
}
/*******************************************************************************
  * 函数名：Task_MsProcess
  * 功  能：每ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
//void Task_MsProcess(void)
//{
//	if (Timer_GetMsState() == 1)
//	{
//		Timer_ClearMsState();
//		
//	}
//}
/*******************************************************************************
  * 函数名：Task_2MsProcess
  * 功  能：每2ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
//void Task_2MsProcess(void)
//{
//	if (Timer_Get2MsState() == 1)
//	{
//		Timer_Clear2MsState();
//	}
//}
/*******************************************************************************
  * 函数名：Task_5MsProcess
  * 功  能：每5ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
//void Task_5MsProcess(void)
//{
//	if (Timer_Get5MsState() == 1)
//	{
//		Timer_Clear5MsState();
//	}
//}
/*******************************************************************************
  * 函数名：Task_10MsProcess
  * 功  能：每10ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
//void Task_10MsProcess(void)
//{
//	if (Timer_Get10MsState() == 1)
//	{
//		Timer_Clear10MsState();
//		
//	}
//}
/*******************************************************************************
  * 函数名：Task_20MsProcess
  * 功  能：每20ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
//void Task_20MsProcess(void)
//{
//	if (Timer_Get20MsState() == 1)
//	{
//		Timer_Clear20MsState();
//	}
//}
/*******************************************************************************
  * 函数名：Task_50MsProcess
  * 功  能：每50ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
//void Task_50MsProcess(void)
//{
//	if (Timer_Get50MsState() == 1)
//	{
//		Timer_Clear50MsState();
//		
//		
//	}
//}
/*******************************************************************************
  * 函数名：Task_100MsProcess
  * 功  能：每100ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Task_100MsProcess(void)
{
	if (Timer_Get100MsState() == 1)
	{
		Timer_Clear100MsState();
		DS18B20_Task100Ms();
		PID_Task100Ms();
		//printf("Temp=%d",Temp);
	}
}
/*******************************************************************************
  * 函数名：Task_200MsProcess
  * 功  能：每200ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Task_200MsProcess(void)
{
	if (Timer_Get200MsState() == 1)
	{
		Timer_Clear200MsState();
		
		//printf("Humi=%d",Humi);
		
//		Water  = (int16_t)WATER_ReadLEVEL();
//		printf("WATER=%d",Water);
	}
}
/*******************************************************************************
  * 函数名：Task_500MsProcess
  * 功  能：每500ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Task_500MsProcess(void)
{
	uint8_t humidity = 0, temperature = 0; 
	if (Timer_Get500MsState() == 1)
	{
		Timer_Clear500MsState();
		Temp = fTemper;
		Humi  = (int16_t)DHT11_Task(&humidity, &temperature);
		Lux = (int)(TEMT6000_ReadLux());
		//printf("Lux=%d",Lux);
		
		
		PH   = PH_Value_Conversion();  
		Tds  = (int)TDS_ReadTDS();
		Water  = (int16_t)WATER_ReadLEVEL();
		
		//printf("PH=%d",PH);
		
			//printf("Tds=%d",Tds);
		
		
		//Display_Task500Ms();
		
		//disable_tim4_task = 1;  // 禁止定时任务
		//Temp = 23;
		//Temp = fTemper;
		//PID_Task100Ms();
		//disable_tim4_task = 0;  // 允许定时任务
	}
}

/*******************************************************************************
  * 函数名：Task_SecProcess
  * 功  能：每Sec执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Task_SecProcess(void)
{
    if (Timer_GetSecState() == 1)
    {
      Timer_ClearSecState();
			
			//printf("WATER=%d",Water);
			sprintf(formattedJson, text,Temp,Humi,PH,Tds,Lux,Water);
			send(formattedJson);
			///printf("Temp=%d, Humi=%d, PH=%d, Tds=%d, Lux=%d\n",Temp, Humi, PH, Tds, Lux);
    }
}

/*******************************************************************************
  * 函数名：Task_SecProcess
  * 功  能：每Sec执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Task_MinProcess(void)
{
	if (Timer_GetMinState() == 1)
	{
		Timer_ClearMinState();	
		//sprintf(formattedJson, text,Temp,Humi,PH,Tds,Lux);
		//send(formattedJson);
	}
}

/*******************************************************************************
  * 函数名：Task_Control
  * 功  能：任务总控制
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Task_Control(void)
{
	//Task_2MsProcess();
	//Task_20MsProcess();
	Task_100MsProcess();
	Task_200MsProcess();
	Task_500MsProcess();
	Task_SecProcess();
	Task_MinProcess();
}

/***********************************END OF FILE********************************/
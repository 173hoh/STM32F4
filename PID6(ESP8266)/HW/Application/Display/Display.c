/*******************************************************************************
  * 文件：Display.c
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-11-21
  * 说明：数码管显示控制，由TM1638驱动
*******************************************************************************/
#include "TM1638.h"
#include "DS18B20.h"
#include "PID.h"
#include "string.h"
static TM1638Tube_ts sDisplay_TubeData;//数码管显示的内容
static Byte_tu uDisplay_LedData;//Led显示的内容
static DisplayFlag_tu uDisplay_Flag;
/*******************************************************************************
  * 函数名：Display_Init
  * 功  能：初始化
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Display_Init(void)
{
	TM1638_Switch(0);//关闭显示
	TM1638_SetBrightness(7);//开显示，设定亮度7级
	TM1638_WriteCmd(0x44);//固定地址模式
	uDisplay_LedData.byte = 0;
	uDisplay_Flag.byte = 0;
	memset(&sDisplay_TubeData, 0x00, sizeof(sDisplay_TubeData));
}
/*******************************************************************************
  * 函数名：Display_TubeTemperProcess
  * 功  能：数码管显示温度
  * 参  数：pTube显示的第一个数码管的地址
			u32Temper温度
  * 返回值：无
  * 说  明：u32Temper为实际温度*100，显示1位小数，共需4位数码管
*******************************************************************************/
void Display_TubeTemperProcess(uint8_t *pTube, uint32_t u32Temper)
{
	uint16_t u16Integeral = 0;//整数部分
	uint8_t u8Decimal = 0;//小数部分
	u16Integeral = (uint16_t)(u32Temper / 100);	
	u8Decimal = (uint8_t)(u32Temper % 100);
	if ((u8Decimal % 10) >= 5)//余数，四舍五入
	{
		u8Decimal /= 10;
		u8Decimal += 1;
		if (u8Decimal > 9)
		{
			u8Decimal = 0;
			u16Integeral += 1;
		}
	}else
	{
		u8Decimal /= 10;
	}
	*(pTube + 3) = u8Decimal;//小数部分
	*(pTube + 2) = (u16Integeral % 10) + TUBE_DISPLAY_DECIMAL_PIONT_OFFSET;//个位，加小数点
	if (u16Integeral < 10)//1位整数
	{
		*(pTube + 1) = TUBE_DISPLAY_NULL;
		*pTube = TUBE_DISPLAY_NULL;
	}else if (u16Integeral < 100)//2位整数
	{
		*(pTube + 1) = ((u16Integeral / 10) % 10);//十位
	   	*pTube = TUBE_DISPLAY_NULL;
	}else//3位整数
	{
		*(pTube + 1) = ((u16Integeral / 10) % 10);//十位	   	
		*pTube = (u16Integeral / 100);//百位
	}	
}
/*******************************************************************************
  * 函数名：Display_TubeDataProcess
  * 功  能：数码管显示内容处理
  * 参  数：无
  * 返回值：无
  * 说  明：左边4位数码管显示设定温度，右边4位数码管显示实际温度
*******************************************************************************/
void Display_TubeDataProcess(void)
{	
	uint32_t u32Temper = 0;//温度
	memset(&sDisplay_TubeData, 0x00, sizeof(sDisplay_TubeData));
	u32Temper = DS18B20_GetTemper();//已经扩大100倍，实际2位小数，显示时四舍五入为1位小数	
	Display_TubeTemperProcess(&sDisplay_TubeData.tube4, u32Temper);//显示实际温度
	if (Key_GetSetModeState() == 1)//设定模式下闪烁显示正在设定的温度
	{
		if (uDisplay_Flag.bt.bSettingBlink)
		{
			u32Temper = Key_GetSettingTemper();
			Display_TubeTemperProcess(&sDisplay_TubeData.tube0, u32Temper);//显示设定温度
		}else
		{			
			sDisplay_TubeData.tube3 = TUBE_DISPLAY_NULL;
			sDisplay_TubeData.tube2 = TUBE_DISPLAY_NULL;
			sDisplay_TubeData.tube1 = TUBE_DISPLAY_NULL;
			sDisplay_TubeData.tube0 = TUBE_DISPLAY_NULL;
		}
	}else//非设定模式
	{
		u32Temper = PID_GetSetTemper();
		Display_TubeTemperProcess(&sDisplay_TubeData.tube0, u32Temper);//显示设定温度
	}
	TM1638_TubeDisplay(sDisplay_TubeData);
}
/*******************************************************************************
  * 函数名：Display_LedDataProcess
  * 功  能：Led显示内容处理
  * 参  数：无
  * 返回值：无
  * 说  明：最右侧1位LED，显示加热管开关状态
*******************************************************************************/
void Display_LedDataProcess(void)
{
	uint8_t u8Data = 0;
	u8Data |= (PID_GetHeaterState() << 0);
	uDisplay_LedData.byte = u8Data;
	TM1638_LedDisplay(uDisplay_LedData.byte);
}
/*******************************************************************************
  * 函数名：Display_TubeBlinkFlagProcess
  * 功  能：闪烁标志每500秒取反
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Display_TubeBlinkFlagProcess(void)
{
	uDisplay_Flag.bt.bSettingBlink = ~uDisplay_Flag.bt.bSettingBlink;
}
/*******************************************************************************
  * 函数名：Display_Task100Ms
  * 功  能：每100ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Display_Task100Ms(void)
{
	Display_TubeDataProcess();
	Display_LedDataProcess();
}
/*******************************************************************************
  * 函数名：Display_Task500Ms
  * 功  能：每秒执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Display_Task500Ms(void)
{
	Display_TubeBlinkFlagProcess();
}
/*******************************************************************************
  * 函数名：Display_TaskSec
  * 功  能：每秒执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Display_TaskSec(void)
{
	//Display_TubeDataProcess();
	//Display_LedDataProcess();
	//Display_TubeBlinkFlagProcess();
}
/***********************************END OF FILE********************************/

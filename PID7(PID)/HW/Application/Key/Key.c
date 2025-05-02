/*******************************************************************************
  * 文件：Key.c
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-11-26
  * 说明：按键，由Tm1638驱动
*******************************************************************************/
#include "TM1638.h"
#include "Key.h"
#include "PID.h"
#include <stdbool.h>


static uint8_t u8Key_Name;//按键名称
static uint8_t u8KeyState;//按键状态，按下或释放
static KeyFlag_tu uKey_Flag;
static uint32_t u32Key_SettingTemper;//正在设定的温度值

#define KEY_RELEASED					0x00//键被释放
#define KEY_PRESSED						0x01//键被按下
/*******************************************************************************
  * 函数名：Key_Init
  * 功  能：初始化
  * 参  数：无
  * 返回值：无
  * 说  明：无
*******************************************************************************/
void Key_Init(void)
{
	u8Key_Name = KEY_NULL;
	uKey_Flag.byte = 0;
	u8KeyState = KEY_RELEASED;
	u32Key_SettingTemper = 0;
}
/*******************************************************************************
  * 函数名：Key_ScanProcess
  * 功  能：按键扫描处理
  * 参  数：无
  * 返回值：无
  * 说  明：去抖动，每20ms扫描一次，扫描时按键被释放才会执行案件的操作；
			仅支持单键、单击
*******************************************************************************/
void Key_ScanProcess(void)
{
	uint8_t u8KeyNum;
	u8KeyNum = TM1638_ReadKey();//获取键值
	if (u8KeyNum != 0)//有键按下
	{
		u8KeyState = KEY_PRESSED;
		u8Key_Name = u8KeyNum;//保存按键值
	}else
	{
		u8KeyState = KEY_RELEASED;
	}
	if (u8KeyState == KEY_RELEASED)//按键释放
	{
		switch (u8Key_Name)
		{
			case KEY_SET://设置键
			{
				if (uKey_Flag.bt.bSetMode)
				{
					uKey_Flag.bt.bSetMode = 0;//退出设置模式
					PID_ModifySetTemper(u32Key_SettingTemper);//修改设定温度
				}else
				{
					uKey_Flag.bt.bSetMode = 1;//进入设置模式
					u32Key_SettingTemper = PID_GetSetTemper();//读取设定温度
				}			
			}break;
			case KEY_UP://向上键
			{
				if (uKey_Flag.bt.bSetMode)//设置模式
				{
					if (u32Key_SettingTemper < (80 * 100))
					{
						u32Key_SettingTemper += 10;
					}
				}
			}break;
			case KEY_DOWN://向下键
			{
				if (uKey_Flag.bt.bSetMode)//设置模式
				{
					if (u32Key_SettingTemper > (30 * 100))
					{
						u32Key_SettingTemper -= 10;
					}
				}
			}break;
			default:break;			
		}
		u8Key_Name = KEY_NULL;
	}		
}
/*******************************************************************************
  * 函数名：Key_GetSetModeState
  * 功  能：返回设定模式状态
  * 参  数：无
  * 返回值：0正常模式，1设定模式
  * 说  明：无
*******************************************************************************/
bool Key_GetSetModeState(void)
{
	return (uKey_Flag.bt.bSetMode ? 1 : 0);
}
/*******************************************************************************
  * 函数名：Key_GetSettingTemper
  * 功  能：获取正在设定的温度值
  * 参  数：无
  * 返回值：u16Key_SettingTemper
  * 说  明：在按键确认之前不能修改真正的设定温度值，供显示用，闪烁
*******************************************************************************/
uint32_t Key_GetSettingTemper(void)
{
	return u32Key_SettingTemper;
}
/*******************************************************************************
  * 函数名：Key_Task20Ms
  * 功  能：每20ms执行的任务
  * 参  数：无
  * 返回值：无
  * 说  明：按键扫描，每20ms执行一次，去抖动
*******************************************************************************/
void Key_Task20Ms(void)
{
	Key_ScanProcess();
}
/***********************************END OF FILE********************************/
/*******************************************************************************
  * 文件：Display.h
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-11-21
  * 说明：数码管显示控制，由TM1638驱动
*******************************************************************************/
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

typedef union
{
	uint8_t byte;
	struct
	{
		uint8_t bSettingBlink:	1;//设定温度闪烁
		uint8_t b1:				1;
		uint8_t b2:				1;
		uint8_t b3:				1;
		uint8_t b4:				1;
		uint8_t b5:				1;
		uint8_t b6:				1;
		uint8_t b7:				1;
	}bt;
}DisplayFlag_tu;

void Display_Init(void);
void Display_Task100Ms(void);
void Display_Task500Ms(void);
void Display_TaskSec(void);
#endif
/***********************************END OF FILE********************************/
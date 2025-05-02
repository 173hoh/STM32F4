/*******************************************************************************
  * 文件：Key.h
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-11-26
  * 说明：按键，由Tm1638驱动
*******************************************************************************/
#ifndef _KEY_H_
#define _KEY_H_

#define KEY_NULL								0x00//无
#define KEY_SET									0x01//设置键
#define KEY_UP									0x03//向上键
#define KEY_DOWN								0x05//向下键
#include <stdbool.h>

typedef union
{
	uint8_t byte;
	struct
	{
		uint8_t bSetMode:		1;//0正常，1进入设定模式
		uint8_t b1:				1;
		uint8_t b2:				1;
		uint8_t b3:				1;
		uint8_t b4:				1;
		uint8_t b5:				1;
		uint8_t b6:				1;
		uint8_t b7:				1;
	}bt;
}KeyFlag_tu;


void Key_Init(void);
bool Key_GetSetModeState(void);
uint32_t Key_GetSettingTemper(void);
void Key_Task20Ms(void);
#endif
/***********************************END OF FILE********************************/
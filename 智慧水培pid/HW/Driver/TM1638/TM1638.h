/*******************************************************************************
  * 文件：TM1638.h
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2021-11-20
  * 说明：TM1638驱动
*******************************************************************************/
#ifndef _TM1638_H_
#define _TM1638_H_

#include "Typedefine.h"
#include "Port.h"
/*******************************************************************************
Macro definitions
*******************************************************************************/
#define TUBE_DISPLAY_NULL					26
#define TUBE_DISPLAY_DECIMAL_PIONT_OFFSET	16
#define TM1638_STBSet()					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
#define TM1638_STBReset()				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)
#define TM1638_CLKSet()					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)
#define TM1638_CLKReset()				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)
#define TM1638_DIOSet()					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)
#define TM1638_DIOReset()				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
#define TM1638_DIOModeOutput()			Port_SetMode(GPIOB, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP)
#define TM1638_DIOModeInput()			Port_SetMode(GPIOB, GPIO_PIN_9, GPIO_MODE_INPUT)
#define TM1638_DIORead()				HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)
/*******************************************************************************
Typedef definitions
*******************************************************************************/
typedef struct
{
	uint8_t tube0;
	uint8_t tube1;
	uint8_t tube2;
	uint8_t tube3;
	uint8_t tube4;
	uint8_t tube5;
	uint8_t tube6;
	uint8_t tube7;
}TM1638Tube_ts;
/*******************************************************************************
Global functions
*******************************************************************************/
void TM1638_Init(void);
void TM1638_TubeDisplay(TM1638Tube_ts data);
void TM1638_LedDisplay(uint8_t data);
uint8_t TM1638_ReadKey(void);
void TM1638_WriteCmd(uint8_t u8Cmd);
void TM1638_SetBrightness(uint8_t u8Brt);
void TM1638_Switch(bool bState);
#endif
/***********************************END OF FILE********************************/
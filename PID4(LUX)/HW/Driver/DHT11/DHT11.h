#ifndef __DHT11_h__
#define __DHT11_h__
 
#include "main.h"
 
#define DHT11       GPIO_PIN_7
#define GPIO_DHT11  GPIOB
#define DHT11_Input HAL_GPIO_ReadPin(GPIO_DHT11,DHT11)
#define DHT11_Low   HAL_GPIO_WritePin(GPIO_DHT11,DHT11,GPIO_PIN_RESET)
#define DHT11_High  HAL_GPIO_WritePin(GPIO_DHT11,DHT11,GPIO_PIN_SET)
 
void DHT11_Init(void);
void DHT11_Read_Out_Intput(uint8_t cmd);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Huim_Temp(uint8_t *HuimH , uint8_t *HuimL , uint8_t *TempH , uint8_t *TempL);
uint8_t DHT11_Check(void);
uint8_t DHT11_Task(uint8_t *humidity, uint8_t *temperature);
#endif
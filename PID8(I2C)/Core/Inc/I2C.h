#ifndef __I2C_H
#define __I2C_H

#include "stm32f1xx_hal.h"

// 定义 SCL 和 SDA 引脚
#define I2C_SCL_PIN    GPIO_PIN_8
#define I2C_SDA_PIN    GPIO_PIN_9
#define I2C_GPIO_PORT  GPIOB

// I2C 操作函数
void Soft_I2C_Init(void);
void Soft_I2C_Start(void);
void Soft_I2C_Stop(void);
void Soft_I2C_Send_Byte(uint8_t data);
uint8_t Soft_I2C_Read_Byte(uint8_t ack);
uint8_t Soft_I2C_Wait_Ack(void);
void Soft_I2C_Send_Ack(void);
void Soft_I2C_Send_NAck(void);

#endif

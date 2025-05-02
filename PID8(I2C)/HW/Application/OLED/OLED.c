#include "stm32f1xx_hal.h"
#include "OLED_Font.h"

/* OLED I2C 引脚配置 */
#define OLED_SCL_GPIO_PORT    GPIOB
#define OLED_SCL_PIN          GPIO_PIN_8
#define OLED_SDA_GPIO_PORT    GPIOB
#define OLED_SDA_PIN          GPIO_PIN_9

/* 软件 I2C 模拟 SDA & SCL */
#define OLED_SCL_H() HAL_GPIO_WritePin(OLED_SCL_GPIO_PORT, OLED_SCL_PIN, GPIO_PIN_SET)
#define OLED_SCL_L() HAL_GPIO_WritePin(OLED_SCL_GPIO_PORT, OLED_SCL_PIN, GPIO_PIN_RESET)
#define OLED_SDA_H() HAL_GPIO_WritePin(OLED_SDA_GPIO_PORT, OLED_SDA_PIN, GPIO_PIN_SET)
#define OLED_SDA_L() HAL_GPIO_WritePin(OLED_SDA_GPIO_PORT, OLED_SDA_PIN, GPIO_PIN_RESET)

/* 读取 SDA 状态 */
#define OLED_SDA_READ() HAL_GPIO_ReadPin(OLED_SDA_GPIO_PORT, OLED_SDA_PIN)

/* 软件延时 (自定义微秒级延时) */
void HAL_Delay_us(uint16_t us)
{
    uint16_t delay = us * (SystemCoreClock / 8 / 1000000); // 计算循环次数
    while (delay--);
}

/* OLED GPIO 初始化 */
void OLED_I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 开启 GPIOB 时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* 配置 SCL 和 SDA 为开漏输出 */
    GPIO_InitStruct.Pin = OLED_SCL_PIN | OLED_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_SCL_GPIO_PORT, &GPIO_InitStruct);

    /* 拉高 SDA 和 SCL */
    OLED_SCL_H();
    OLED_SDA_H();
}

/* I2C 开始信号 */
void OLED_I2C_Start(void)
{
    OLED_SDA_H();
    OLED_SCL_H();
    HAL_Delay_us(5);
    OLED_SDA_L();
    HAL_Delay_us(5);
    OLED_SCL_L();
}

/* I2C 停止信号 */
void OLED_I2C_Stop(void)
{
    OLED_SDA_L();
    OLED_SCL_H();
    HAL_Delay_us(5);
    OLED_SDA_H();
    HAL_Delay_us(5);
}

/* I2C 发送一个字节 */
void OLED_I2C_SendByte(uint8_t Byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (Byte & 0x80)
            OLED_SDA_H();
        else
            OLED_SDA_L();
        
        HAL_Delay_us(2);
        OLED_SCL_H();
        HAL_Delay_us(5);
        OLED_SCL_L();
        HAL_Delay_us(2);
        Byte <<= 1;
    }
    
    /* 额外的时钟用于应答信号 */
    OLED_SCL_H();
    HAL_Delay_us(5);
    OLED_SCL_L();
}

/* OLED 写命令 */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78); // OLED 设备地址
    OLED_I2C_SendByte(0x00); // 发送命令模式
    OLED_I2C_SendByte(Command);
    OLED_I2C_Stop();
}

/* OLED 写数据 */
void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78); // OLED 设备地址
    OLED_I2C_SendByte(0x40); // 发送数据模式
    OLED_I2C_SendByte(Data);
    OLED_I2C_Stop();
}

/* OLED 设置光标 */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (X & 0x0F));
}

/* OLED 清屏 */
void OLED_Clear(void)
{
    for (uint8_t j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        for (uint8_t i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/* OLED 显示字符 */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
    for (uint8_t i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
    for (uint8_t i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);
    }
}

/* OLED 显示字符串 */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    while (*String)
    {
        OLED_ShowChar(Line, Column++, *String++);
    }
}

/* OLED 初始化 */
void OLED_Init(void)
{
    HAL_Delay(100); // 上电延时
    OLED_I2C_Init(); // I2C 端口初始化

    OLED_WriteCommand(0xAE); // 关闭显示
    OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8);
    OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3);
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xA1);
    OLED_WriteCommand(0xC8);
    OLED_WriteCommand(0xDA);
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81);
    OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9);
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0x8D);
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF); // 开启显示

    OLED_Clear(); // 清屏
}

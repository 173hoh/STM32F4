#include "I2C.h"

// 配置 GPIO 为开漏输出
static void I2C_GPIO_Config_Output(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // 开漏模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);
}

// 配置 GPIO 为输入模式
static void I2C_GPIO_Config_Input(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);
}

// 软件 I2C 初始化
void Soft_I2C_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE(); // 使能 GPIOB 时钟
    I2C_GPIO_Config_Output();     // 配置 PB8/PB9 为开漏输出
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
}

// 产生 I2C 起始信号
void Soft_I2C_Start(void) {
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_RESET);
}

// 产生 I2C 停止信号
void Soft_I2C_Stop(void) {
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
}

// 发送 1 字节数据
void Soft_I2C_Send_Byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        if (data & 0x80) {
            HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_RESET);
        }
        HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_RESET);
        data <<= 1;
    }
}

// 等待应答信号
uint8_t Soft_I2C_Wait_Ack(void) {
    uint8_t timeout = 0;
    I2C_GPIO_Config_Input(); // SDA 配置为输入模式
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_Delay(1);

    while (HAL_GPIO_ReadPin(I2C_GPIO_PORT, I2C_SDA_PIN)) {
        timeout++;
        if (timeout > 250) {
            Soft_I2C_Stop();
            return 1; // 无应答
        }
    }

    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_RESET);
    I2C_GPIO_Config_Output(); // 重新设置 SDA 为输出
    return 0;
}

// 发送 ACK
void Soft_I2C_Send_Ack(void) {
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_RESET);
}

// 发送 NACK
void Soft_I2C_Send_NAck(void) {
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_RESET);
}

// 读取 1 字节数据
uint8_t Soft_I2C_Read_Byte(uint8_t ack) {
    uint8_t data = 0;
    I2C_GPIO_Config_Input(); // SDA 配置为输入

    for (uint8_t i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
        HAL_Delay(1);
        data <<= 1;
        if (HAL_GPIO_ReadPin(I2C_GPIO_PORT, I2C_SDA_PIN)) {
            data |= 0x01;
        }
        HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_RESET);
    }

    I2C_GPIO_Config_Output(); // SDA 重新配置为输出
    if (ack) {
        Soft_I2C_Send_Ack();
    } else {
        Soft_I2C_Send_NAck();
    }
    return data;
}

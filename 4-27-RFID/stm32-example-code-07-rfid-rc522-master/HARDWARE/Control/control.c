#include "stm32f10x.h"  // 标准库头文件

void control_Init(void) {
    // 1. 使能 GPIOB 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 2. 配置 PB14（蜂鸣器） 和 PB15（风扇）为推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 3. 初始关闭蜂鸣器和风扇
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);  // 蜂鸣器 OFF
    GPIO_ResetBits(GPIOB, GPIO_Pin_15);  // 风扇 OFF
}

// 蜂鸣器控制
void Buzzer_On(void)  { GPIO_SetBits(GPIOB, GPIO_Pin_14); }
void Buzzer_Off(void) { GPIO_ResetBits(GPIOB, GPIO_Pin_14); }

// 风扇控制
void Fan_On(void)     { GPIO_SetBits(GPIOB, GPIO_Pin_15); }
void Fan_Off(void)    { GPIO_ResetBits(GPIOB, GPIO_Pin_15); }

#include "stm32f4xx.h"                  // Device header

#include "delay.h"
#include "ws2812.h"

#include "string.h"


//uint16_t LED_Buffer[1536];//12个灯*24位=288位数据
/*
*函数名称：Timer1_Init
*功能描述：配置TIM1_CH3输出PWM
*传入参数：无
*返回值  ：无
*/
void Timer1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);      /*使能GPIOE时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);       /*使能TIM1时钟*/

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);   /*将GPIOE13重映射到TIM1的输出通道上*/
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;               /*将引脚功能配置为复用*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;               /*推挽输出*/
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;                 /*上拉*/
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* 定时器周期 : T =(arr + 1) * (PSC + 1) / Tck.   arr:周期值 PSC:预分频值  Tck: 系统时钟频率 */
  TIM_TimeBaseStructure.TIM_Period = 210 - 1;                     /* T = (TIM_Period + 1)*(0+1)/168M  = 800kHz*/
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                        /* 0：不预分频 */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         /* 0：时钟分频因子设为1，不分频*/
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     /*向上计数*/
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                /*重复计数值为0，不使用重复计数*/
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                /*PWM1模式，计数值小于比较值输出高电平，大于输出低电平*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    /*使能输出通道*/
  TIM_OCInitStructure.TIM_Pulse = 0;                               /*设置占空比为0， 1 ~ TIM_TimeBaseStructure.TIM_Period */
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        /*设置输出极性高电平有效*/
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /*禁用互补输出通道*/
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;      /*设置互补输出通道极性高电平有效*/
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Set;      /*设置空闲时输出高电平*/
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;   /*设置互补通道空闲时输出低电平*/ 
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);                         /*将上述配置应用到定时器一的通道三上*/
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);                /*使能通道三的输出比较寄存器的预装载功能*/

  TIM_Cmd(TIM1, ENABLE);                                           /*使能TIM1定时器*/
  TIM_CtrlPWMOutputs(TIM1,ENABLE);                                 /*使能TIM1的PWM输出*/
}

uint16_t g_ledDataBuffer[1536];//64*24
/*
*函数名称：dma2_Init
*功能描述：配置从源地址到TIM1_CCR3的DMA数据传输
*传入参数：无
*返回值  ：无
*/
void dma2_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;  // 提前声明变量

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);                        /*使能DMA2时钟*/

  DMA_DeInit(DMA2_Stream6);                                                   /*对DMA2_Stream6进行默认值初始化*/

  /* DMA2 Stream6 Config for PWM1 by TIM1_CH1 */
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;                              /*配置DMA通道，通道0*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM1->CCR3);         /*配置外设基地址：TIM1的CCR3寄存器*/
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_ledDataBuffer;          /*配置源数据的基地址*/
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                     /*设置搬运方向：存储器到外设*/
  DMA_InitStructure.DMA_BufferSize = 42;                                      /*设置传输数据的大小*/
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            /*外设基地址不自增*/
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     /*源数据基地址自增*/
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; /*设置外设数据大小为半字(16bit)*/
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         /*设置存储器数据大小为半字(16bit)*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               /*设置DMA为普通模式*/
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         /*DMA优先级为高*/
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                      /*禁用DMA的FIFO模式*/
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;               /*设置DMA的FIFO阈值为满*/
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                 /*设置内存突发传输模式为单次传输*/
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;         /*设置外设突发传输模式为单次传输*/

  DMA_Init(DMA2_Stream6, &DMA_InitStructure);                                 /*将上述配置应用到 DMA2_Stream6*/

  TIM_DMACmd(TIM1, TIM_DMA_CC3, ENABLE);                                      /*将TIM1的DMA请求映射到通道三并使能*/
}

/*
*函数名称：RGB_Show
*功能描述：通过DMA控制器从内存中读取数据，然后将数据传输到TIM1的CC3通道，以驱动RGB灯。
*传入参数：
*返回值  ：无
*/


void WS2812B_send(uint32_t GRB_Data)//发送颜色数据 0X 00 00 00 
{
	uint16_t i,j,z=0;

	for(j=0;j<65;j++)
	{
		for(i=0;i<24;i++)
		{
			if(0x800000&((GRB_Data << i)))  // 比较每一位的值，是0码还是1码
			{
				g_ledDataBuffer[z]=130;
			}
			else
			{
				g_ledDataBuffer[z]=60;
			}
			z++;
		}
	
	}

   DMA_SetCurrDataCounter(DMA2_Stream6, 1560);         /*指定要传输的数据量*/
   TIM_Cmd(TIM1, ENABLE);                                    /*启用TIM1定时器*/
   TIM_DMACmd(TIM1, TIM_DMA_CC3, ENABLE);                    /*将TIM1的DMA请求映射到通道三并使能*/
   DMA_Cmd(DMA2_Stream6, ENABLE);                            /*启用DMA2_Stream6，开始数据传输*/
   while(!DMA_GetFlagStatus(DMA2_Stream6, DMA_FLAG_TCIF6));  /*等待DMA传输完成，使用循环检查DMA传输完成标志*/
   DMA_Cmd(DMA2_Stream6, DISABLE);                           /*禁用DMA2流6，停止数据传输*/
   DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);              /*清除DMA传输完成标志*/
   TIM_Cmd(TIM1, DISABLE);                                   /*禁用TIM1定时器，停止计时*/

}

void LED_OFF(void)
{
	WS2812B_send(0X000000);
}

void LED_ON(void)
{
		WS2812B_send(0XFFFFFF);
		delay_ms(500);
				
		WS2812B_send(0X2F2F2F);
		delay_ms(500);
				
		WS2812B_send(0X0F0F0F);
		delay_ms(500);
	
		WS2812B_send(0X020202);
		delay_ms(500);
		
	  WS2812B_send(0X0F0F0F);
		delay_ms(500);
	
		WS2812B_send(0X000000);
		delay_ms(500);

}


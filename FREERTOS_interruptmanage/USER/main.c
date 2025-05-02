#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "timer.h"
//#include "usmart.h"
#include "sram.h"
#include "malloc.h" 
#include "freertos_demo.h"
/************************************************
 ALIENTEK 探索者STM32F407开发板 实验37
 内存管理实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
 1、#define INCLUDE_vTaskSuspend			        1
 2、调用vTaskSuspend(task1_handler);
 3、#define INCLUDE_xTaskResumeFromISR	        1
 4、HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);改为HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
 5、HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);       //抢占优先级为2，子优先级为1，改为HAL_NVIC_SetPriority(EXTI2_IRQn,5,0);

************************************************/

int main(void)
{
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);             	//初始化USART
	//usmart_dev.init(84); 		    //初始化USMART
	LED_Init();						//初始化LED	
	//KEY_Init();						//初始化KEY
	TIM3_Init(10000 - 1, 8400 - 1);  // 30秒中断
	TIM5_Init(10000 - 1, 8400 - 1);  // 30秒中断
	//LCD_Init();                     //初始化LCD
	//SRAM_Init();					//初始化外部SRAM  
	
//	my_mem_init(SRAMIN);			//初始化内部内存池
//	my_mem_init(SRAMEX);			//初始化外部内存池
//	my_mem_init(SRAMCCM);			//初始化CCM内存池
	while(1)
	{
			freertos_demo();
	}
	
}


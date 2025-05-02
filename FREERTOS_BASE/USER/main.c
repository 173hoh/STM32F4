#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"
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
************************************************/

int main(void)
{
	
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	//uart_init(115200);             	//初始化USART
	//usmart_dev.init(84); 		    //初始化USMART
	LED_Init();						//初始化LED	
	//KEY_Init();						//初始化KEY
	//LCD_Init();                     //初始化LCD
	//SRAM_Init();					//初始化外部SRAM  
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	my_mem_init(SRAMEX);			//初始化外部内存池
	my_mem_init(SRAMCCM);			//初始化CCM内存池
	
// 	POINT_COLOR=RED;//设置字体为红色 
//	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
//	LCD_ShowString(30,70,200,16,16,"MALLOC TEST");	
//	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,"2014/5/15");   
//	LCD_ShowString(30,130,200,16,16,"KEY0:Malloc  KEY2:Free");
//	LCD_ShowString(30,150,200,16,16,"KEY_UP:SRAMx KEY1:Read"); 
// 	POINT_COLOR=BLUE;//设置字体为蓝色 
//	LCD_ShowString(30,170,200,16,16,"SRAMIN");
//	LCD_ShowString(30,190,200,16,16,"SRAMIN  USED:   %");
//	LCD_ShowString(30,210,200,16,16,"SRAMEX  USED:   %");
//	LCD_ShowString(30,230,200,16,16,"SRAMCCM USED:   %");
	freertos_demo();
 	
}


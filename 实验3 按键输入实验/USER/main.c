#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "ws2812.h"
#include "temt6000.h"
#include "pwm.h"

//ALIENTEK 探索者STM32F407开发板 实验3
//按键输入实验-库函数版本 
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司    
//作者：正点原子 @ALIENTEK 


int main(void)
{ 
	//u16 pwmval=0;    
	//u8 dir=1;
  u16 adcx;
	float voltage = 0 ;
	float temp;
  u8 key;           //保存键值
	delay_init(168);  //初始化延时函数
	Timer1_Init();
	dma2_Init();
	LED_Init();				//初始化LED端口 
	BEEP_Init();      //初始化蜂鸣器端口
	KEY_Init();       //初始化与按键连接的硬件接口
	Adc_Init();
	uart_init(115200);          // 初始化串口波特率为115200
	TIM12_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.     
	LED0=0;				  	//先点亮红灯
	
	while(1)
	{
		LED_ON();
		adcx=Get_Adc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
		voltage = (float)(adcx *10.000/4096);//ADC采样值到电压的转换
		printf("亮度:%.3f\r\n",voltage);
		
		TIM_SetCompare1(TIM12,300);	//修改比较值，修改占空比
//		key=KEY_Scan(0);		//得到键值
//	   	if(key)
//		{						   
//			switch(key)
//			{				 
//				case WKUP_PRES:	//控制蜂鸣器
//					BEEP=!BEEP;
//					break;
//				case KEY0_PRES:	//控制LED0翻转
//					LED0=!LED0;
//					break;
//				case KEY1_PRES:	//控制LED1翻转	 
//					LED1=!LED1;
//					break;
//				case KEY2_PRES:	//同时控制LED0,LED1翻转 
//					LED0=!LED0;
//					LED1=!LED1;
//					break;
//			}
//		}else delay_ms(10); 
//	}

	}
}

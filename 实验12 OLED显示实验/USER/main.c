#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "delay.h"    
#include "sys.h"
#include "oled.h"

int main(void)
{
	float tn=-985.56;
	int aaaa=12356;
	delay_init(168);     	 //延时函数初始化	  
	OLED_Init();
	
	OLED_ShowString(1,1,"Welcome!");
	delay_ms(1000);
	OLED_Clear();
	
	while(1) 
	{
		OLED_ShowChar(0,0,'a');
    OLED_ShowChar(1,1,'A');
		OLED_ShowString(1,6,"HelloWorld!");
    OLED_ShowNum(2,1,12345,5,12);
		OLED_ShowFloat(3,1,12.783,1);//size=1,一位小数，size=2,2位小数，size=3，三个小数	 
	  OLED_ShowFloat(3,2,-12.3,1);//size=1,一位小数，size=2,2位小数，size=3，三个小数	 
		OLED_ShowFloat(3,3,tn,2);//size=1,一位小数，size=2,2位小数，size=3，三个小数	 
		OLED_ShowFloat(3,4,-58.378,3);//size=1,一位小数，size=2,2位小数，size=3，三个小数	 
		delay_ms(800);
		tn=tn+85.55;
		delay_ms(500);
		
		OLED_ShowFloat(0,3,tn,2);//size=1,一位小数，size=2,2位小数，size=3，三个小数	
		delay_ms(500);
	}	  

}


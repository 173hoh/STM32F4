#include "motor.h"
#include "delay.h"
#include "timer.h"
#include "oled.h"
void Set_PWM(int PWM1,int PWM2)
{

			if(PWM1>7200)
				PWM1=7200;
			else if(PWM1<-7200)
				PWM1=-7200;

			if(PWM2>7200)
				PWM2=7200;
			else if(PWM2<-7200)
				PWM2=-7200;

			if(PWM1>0)
			{ 
				AIN1=0;
				AIN2=1;
				TIM_SetCompare1(TIM1,7200-PWM1);
		
			}
				else if(PWM1<0)
			{
				AIN1=1;
				AIN2=0;
				TIM_SetCompare1(TIM1,7200-(-PWM1));
			}
				
					else if(PWM1==0)
			{				
				AIN1=0;
				AIN2=0;

			}

			if(PWM2>0)
			{
				BIN1=0;
				BIN2=1;
			TIM_SetCompare4(TIM1,7200-PWM2);
			}
				else if(PWM2<0)
			{
				BIN1=1;
				BIN2=0;
			TIM_SetCompare4(TIM1,7200-(-PWM2));
			}
					else if(PWM2==0)
			{
				BIN1=1;
				BIN2=1;
			}

}


void motor_init()
{
	motor_gpio_init();         //gpio初始化
	TIM1_PWM_Init(7200-1,1-1); //TIM1的pwm模式初始化
	TIM3_encoder_init();       //TIM3的编码器模式初始化
	TIM2_encoder_init();       //TIM2的编码器模式初始化
}
void motor_gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   

	//正反转控制口初始化：
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


int read_encoder3(void)
{
	int speed;
	speed = (short)TIM_GetCounter(TIM3); //1.采集编码器的计数值并保存
	TIM_SetCounter(TIM3,0);              //2.将定时器的计数值清零
	
	return speed;
}

int read_encoder2(void)
{
	int speed;
	speed = (short)TIM_GetCounter(TIM2); //1.采集编码器的计数值并保存
	TIM_SetCounter(TIM2,0);              //2.将定时器的计数值清零
	return speed;
}

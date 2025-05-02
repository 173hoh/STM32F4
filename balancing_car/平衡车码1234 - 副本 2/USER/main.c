#include "stm32f10x.h"
#include "sys.h" 
#include "Delay.h"
#include "inv_mpu.h"
//更改了OLED的文件，显示更方便                                                                                                       

float Pitch,Roll,Yaw;						//角度
short gyrox,gyroy,gyroz;				//陀螺仪--角速度
short aacx,aacy,aacz;						//加速度
int Encoder_Left,Encoder_Right;	//编码器数据（速度）

int PWM_MAX=7200,PWM_MIN=-7200;	//PWM限幅变量
int MOTO1,MOTO2;								//电机装载变量


int main(void)	
{
	

	delay_init();
	NVIC_Config();
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
	Encoder_TIM3_Init();
	Encoder_TIM4_Init();
	
	EXTI9_5_IRQHandler();
	
	Motor_Init();
	PWM_Init_TIM2(0,7199);
  while(1)	
	{
		OLED_ShowSignedNum(1,1,Encoder_Left,4);
		OLED_ShowSignedNum(2,1,Encoder_Right,4);		
		OLED_ShowSignedNum(3,1,gyrox,5);
		OLED_ShowSignedNum(4,1,Roll,5);	
	} 	
}





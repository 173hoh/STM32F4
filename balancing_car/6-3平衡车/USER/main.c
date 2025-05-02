#include "sys.h"
//#include "usart.h"		
#include "delay.h"	
#include "led.h"   
#include "key.h"   
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "oled.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "bluetooth.h"
#include "tracking.h"

/*
-------------------------------------IO口对应功能表-------------------------------------------------------
电机驱动：                B12->BIN2       B13->BIN1      B14->AIN1     B15->AIN2
	PWM   ：                A8 ->PWMA                        A11->PWMB
  OLED  :                 B5 ->SCL                         B4 ->SDA
MUP6050 :                 B6 ->SCL                         B9 ->SDA
电机编码器：              B7 B6 一组                       A0 A1一组
蓝牙    :                 RXD ->B10                        TXD->TXD
按键    :                 1(上)->A2     2(左)->A3          3(下)->A4        4(右)->A5
循迹    :                 Left -> B0              Mid  ->B1             Right ->A6

                    电机负值为正方向，速度最大值在60左右


*/





int Encoder_Left,Encoder_Right;       //左右编码器的脉冲计数
short gyrox,gyroy,gyroz;							//陀螺仪原始数据 
float pitch,roll,yaw; 								//欧拉角
u8 stop_flag=0;    										//小车停止标志位
u8 Tracking_flag = 0;									//循迹开启标志位
u8 Key_flag=0;												//按键规划标志位
extern int Moto1,Moto2;  							//电机PWM变量

unsigned char* Exchange(float dat,unsigned char buf[6]);		//浮点型转字符串函数声明
void display_In(void);
void keyboard(void);
void calculation(void);
/**************************************************************************
函数功能：主函数
入口参数：无
返回  值：无
**************************************************************************/

int main(void)
 { 


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    						  //延时函数初始化	  
	uart1_init(9600);	 							//串口初始化为9600
	LED_Init();		  								//初始化与LED连接的硬件接口
	MiniBalance_PWM_Init(7199,0);   //初始化PWM 10KHZ，用于驱动电机 ,PWM最多7200
	TIM3_PWM_Init(7199,0);
	MPU_Init();											//初始化MPU6050
	OLED_Init();                    //OLED初始化	
	OLED_Clear();	
	Encoder_Init_TIM3();            
  Encoder_Init_TIM4(); 
	PID_Init();											//PID初始化
	 
	calculation();
//	while(mpu_dmp_init())
// 	{
//			LED0  = 0;
//			LED0  = 1;
//			LED0  = 0;
//	} 				//进行DMP初始化

//	TIM3_Config(999,71);		//防止中断影响初始化，最后进行定时器中断的定义
	           

 	while(1)
	{
		delay_ms(500);
		LED0= ~LED0;

	} 	
}
 
/**************************************************************************
函数功能：OLED显示数据函数
入口参数：未知
返回  值：无
**************************************************************************/
void display_In()
{	
	
			unsigned char str[6]; 
			OLED_ShowString(0,24,"Kd=",12);
			OLED_ShowNum(24,24,pid.Kd*10,3,12);	
			
			OLED_ShowString(0,36,"Kpv=",12);
			OLED_ShowNum(32,36,pid.Kp_speed,2,12);	
	
			OLED_ShowString(48,36,"Kp=",12);
			OLED_ShowNum(64,36,pid.Kp,2,12);	
	
			OLED_ShowString(80,36,"Kiv=",12);
			OLED_ShowNum(112,36,pid.Ki_speed*100,2,12);	
	
			OLED_ShowString(48,24,"Yaw= ",12);
			OLED_ShowString(88,24,Exchange(yaw,str),12);

			
			OLED_ShowString(82,0,"fg=",12);
			OLED_ShowNum(106,0,Key_flag,1,12);
			
			OLED_ShowString(0,0,"Angle=",12);
			OLED_ShowString(50,0,Exchange(pitch,str),12);	
			OLED_ShowString(0,12,"L=",12);
			OLED_ShowNum(16,12,Encoder_Left,5,12);
			OLED_ShowString(56,12,"R=",12);			
			OLED_ShowNum(72,12,Encoder_Right,5,12);
	
	
	
							/*最下一行*/
			OLED_ShowString(64,48,"V=",12);
			OLED_ShowNum(80,48,pid.left,4,12);
			
			OLED_ShowNum(32,48,myabs(Moto1),4,12);	
			OLED_ShowString(0,48,"PWM=",12);
			
			OLED_Refresh();

}




/**************************************************************************
函数功能：整体计算PID，读取角度值等等
入口参数：未知
返回  值：无
						本应该不写在这，但是因为各种原因只能放这
**************************************************************************/
void calculation(void)
{
if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);									//得到陀螺仪数据
			Encoder_Left=Read_Encoder(2);                             //===读取编码器的值
			Encoder_Right=Read_Encoder(4); 

			pid.Pv = pitch*10;																				//角度*十倍											
			if(Turn_Off(pid.Pv)) stop_flag=1;													//检测是否小车异常，异常就停止
			else stop_flag=0;
									
			Moto1 = balance(pid.Pv) + velocity(Encoder_Left,Encoder_Right)-turn(yaw);		//获得PWM输出值
			Moto2 =balance(pid.Pv) + velocity(Encoder_Left,Encoder_Right)+turn(yaw);
			Xianfu_Pwm();																							//对PWM进行限幅
			Set_Pwm(Moto1,Moto2);  																		//设置PWM
		}
}


/**************************************************************************
函数功能：浮点型转化为字符, 数据放大100蓓  1° = 100      0.1°= 10 
入口参数：浮点型
返回  值：无
**************************************************************************/
unsigned char* Exchange(float dat,unsigned char buf[6])
{
	signed char i = 0,t=0;
	unsigned char buf2[6];
	unsigned int num;
	
		if(dat<0)
		{
			dat = -dat;
			buf[t++] = '-';
		}
		 dat =  dat*100;
		 num = (unsigned int)dat;
		while(num>0)
		{
			buf2[i++] = num%10+'0';
			num/=10;
		}
		while(--i>=0)
		{
				buf[t++] = buf2[i];
		}
		//让数据后几位刷新为0，解决数据滞留在OLED上问题
		while(t<5)
		{
			buf[t++]  = ' ';
		}
		buf[t] = '\0';
		return buf;
}



#include <stm32f10x.h>
#include "delay.h"
#include "oled.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "motor.h"
#include "usart2.h"
#include "mpuexti.h"
extern float pitch,roll,yaw;  //欧拉角测量值
float zhongzhi=0;             //roll理论值（小车平衡时的角度）
extern int velocity;          //速度测量值（编码器脉冲数，非真实速度）
extern float velocity_sum;    //速度积分
//直立环PD参数:
//调完速度环后精调
float Kp=490,Kd=2000 ; //乘0.6
//速度环PI参数:
float VKp=200 ,VKi=0.805;    
//转向环PI参数:
float AKp=1.5,AKD=0;
int main()
{//定时器初始化在串口初始化之前
	 int CNT=1000000;
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //2位抢占优先级，2位响应优先级
	 delay_init();               										//延时函数初始化
	 motor_init();
 //USART2_Config(115200);	//电机的 IO  pwm  编码器 初始化
	 OLED_Init();
	 OLED_Clear();//清屏	 
	 MPU_Init(); //MPU6050初始化	
   DMP_Init(); 
	 MPU_exti_init();
/*******************************************************	
                  到此所有硬件准备完毕
********************************************************/ 
	while(1)
	{ 
		OLED_ShowAngle(pitch,yaw);
		OLED_Showshuzhi(read_encoder3(),read_encoder2());
	}
}

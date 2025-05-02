#include "tracking.h"
#include "pid.h"

/**************************************************************************
函数功能：循迹模块IO口初始化
入口参数：无
返回  值：无
**************************************************************************/
void Tracking_Init(void)
{
	
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能A\B时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能A\B时钟
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;//PB 0 1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
} 



/**************************************************************************
函数功能：循迹模块，逻辑运算
入口参数：无
返回  值：无
**************************************************************************/	
extern float yaw;
void Tracking_detection(void)
{

	if((Left_Tracking==1&&Right_Tracking == 1&&Mid_Tracking==1))
	{
		pid.Angle_turn = pid.Angle_turn;
	}
	else if(Left_Tracking==1&&Right_Tracking == 0&&Mid_Tracking==1)  //轻度左转
	{
		pid.Angle_turn +=0.1;
	}
	else if(Left_Tracking==0&&Right_Tracking == 1&&Mid_Tracking==1)//轻度右转
	{
		pid.Angle_turn -=0.1;
	}
	else if(Left_Tracking==1&&Right_Tracking == 0&&Mid_Tracking==0)  //左转,并把速度变0
	{
		pid.Angle_turn +=2.5;
	}
	else if(Left_Tracking==0&&Right_Tracking == 1&&Mid_Tracking==0)//右转
	{
		pid.Angle_turn -=2.5;
	}
	else if(Mid_Tracking==1&&Left_Tracking==0&&Right_Tracking==0)  //直走
	{
		
	}
	else if(EndRight_Tracking==1)
		pid.Angle_turn +=45;
	else
	{
		
	}
  
}





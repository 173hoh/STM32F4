#include "pid.h"
 PID pid;

/**************************************************************************
函数功能：PID数据初始化
入口参数：无
返回  值：无
**************************************************************************/
void PID_Init()
{
	          /*平衡PID环控制参数初始化*/
		pid.Sv =  0;		
		pid.Kp = 90;
		pid.Kd = 4;  
											 // 35 2
							/*速度PID环控制参数初始化*/		
		pid.Kp_speed = 155;    
		pid.Ki_speed = 0.4;
		pid.EK_speed = 0;
		pid.SEK_speed = 1;
		pid.left = 0;
		pid.right = 0;				//速度初始值
		                  //65 2
							/*转向PID环控制参数初始化*/
		pid.Kp_turn =35;
		pid.Angle_turn = 0;
}

/**************************************************************************
函数功能：以下三个函数，分别计算各个环的PID值，并返回
入口参数：未知
返回  值：无
**************************************************************************/


				/*小车平衡环部分，微分+比例控制   PD
	        微分变量为直接读取的加速度*/
extern short gyroy;
int balance(float Angle)
{  

    signed  int  Bias;
	  int balance;
	  Bias=(Angle-pid.Sv);
	  balance=pid.Kp*Bias+gyroy*pid.Kd;  
	  return balance;
}

				/*小车速度环部分， 积分+比例控制*/  //PI
int velocity(int encoder_left,int encoder_right)
{    
	     
		int velocity;			
		pid.EK_speed = pid.left-(encoder_left+encoder_right);
		pid.SEK_speed += pid.EK_speed;
		velocity = pid.Kp_speed*pid.EK_speed +(int)(pid.Ki_speed*pid.SEK_speed);
		return velocity;
}

				/*小车转向环部分，比例控制*/
int turn(float gyro)//转向控制
{
    int Turn;
	  float Bias;
			
			if(pid.Angle_turn <= -180&&gyro>0)
				Bias = (gyro-(pid.Angle_turn+360))*10;	
			else if(pid.Angle_turn <= -360&&gyro<0)
				Bias = (gyro-(pid.Angle_turn+360))*10;	
			else if(pid.Angle_turn >= 180&&gyro<0)
				 Bias = (gyro-(pid.Angle_turn-360))*10;
			else if(pid.Angle_turn >= 360&&gyro>0)
					Bias = (gyro-(pid.Angle_turn-360))*10;
		  else
				 Bias = (gyro-pid.Angle_turn)*10;	//阈值

	  Turn = (int)(Bias*pid.Kp_turn); 
	
				/*进行转向速度的单独限幅*/
		if(Turn>=1500)	Turn =1500;
		else if(Turn<=-1500)Turn =-1500;
			
		return Turn;
}

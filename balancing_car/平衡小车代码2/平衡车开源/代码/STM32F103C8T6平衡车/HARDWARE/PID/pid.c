
#include "pid.h"

float err;               //此次误差
float last_err;          //上次误差
float err_sum=0;         //误差累加
float err_difference;    //误差的差值

extern float VKp,VKi;    //速度环参数
float filt_velocity;     //滤波后的速度
float last_filt_velocity;//上一次的滤波后的速度
float velocity_sum=0;    //速度的累加
extern float AKp,AKD; 

//直立环：
int vertical_PID_value(float measure,float calcu)
{

	err = measure - calcu;             //误差                    
	err_difference = err - last_err;   //误差的差值
	last_err = err;                    //此次误差记录为“上次误差”
	
	return Kp*err + Kd*err_difference;
	
}


//速度环：
int velocity_PID_value(int velocity)
{
	float a=0.3;                                           //滤波系数（反映滤波程度）
	filt_velocity = a*velocity + (1-a)*last_filt_velocity; //一阶速度滤波
	velocity_sum +=  filt_velocity;                        //速度的累加                                       //累加限幅
	last_filt_velocity = filt_velocity;                    //此次速度记录为“上次速度”

	return VKp*filt_velocity + VKi*velocity_sum;
}
//转向环
int turn(int encoder_left, int encoder_right, int gyro)
{
	int Turn;
	float Bias;
//	if(1==Flag_Left) Turn_Amplitude=1100;
//	else if(1==Flag_Right) Turn_Amplitude=-1100;
	Bias=gyro-0;
	Turn=-Bias*AKp;
//	Turn+=Amplitude;
	
	return Turn;
}



//其中Turn+=Amplitude; 转向遥控叠加在转向控制里面。


//I限幅：
void I_xianfu(int max)
{
	if(velocity_sum>max)  velocity_sum=max;
	if(velocity_sum<-max) velocity_sum=-max;
}

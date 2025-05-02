/******************************************************  
 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 平衡小车（开源版）
 作者：WangGuanNan
 QQ：1501451224
 //////////////////////////////////////////////////////
 创建时间：2021/7/11
 版本：V1.0 仅平衡
******************************************************/
#ifndef __PID_H
#define __PID_H
extern float Kp,Ki,Kd;   //直立环参数
int vertical_PID_value(float measure,float calcu); //直立环
int velocity_PID_value(int velocity);              //速度环
int turn(int encoder_left, int encoder_right, int gyro);
void I_xianfu(int max);                            //pwm限幅
#endif

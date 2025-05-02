#ifndef __PID_H__
#define __PID_H__
#include "sys.h"
//PID结构体
typedef struct 
{
	signed int  Sv;			//用户设定平衡位置值
	float Pv;
					/*平衡环参数设定 */
	float  Kd;					//平衡微分项系数
	signed int  Kp;			//平衡比例项系数
	

	
					/*速度环参数设定 */
	signed int left;      //目标左速度
	signed int right;     //目标右速度
	float  Ki_speed;				//速度环积分项系数
	signed int Kp_speed;		//速度环比例项系数
	signed int EK_speed;  	//速度偏差
	signed int SEK_speed;   //历史偏差之和

					/*转向环参数设定 */
	float Kp_turn;					//转向环比例项系数
	float Angle_turn;				//目标转向角度
	
}PID;

extern PID pid;

int balance(float Angle);
void PID_Init(void);
int velocity(int encoder_left,int encoder_right);
int turn(float gyro);

#endif

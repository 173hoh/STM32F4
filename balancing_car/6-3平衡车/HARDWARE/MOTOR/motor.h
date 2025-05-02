#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
#define PWMA   TIM2->CCR1  //PA0
#define AIN2   PAout(2)
#define AIN1   PAout(3)
#define BIN1   PAout(4)
#define BIN2   PAout(5)
#define PWMB   TIM2->CCR2  //PA1
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
int myabs(int a);
void Set_Pwm(int moto1,int moto2);
void Xianfu_Pwm(void);
u8 Turn_Off(signed int angle);
//void TIM3_PWM_Init(u16 arr,u16 psc);
#endif

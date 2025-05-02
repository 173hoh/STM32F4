#ifndef __MOTOR_H
#define __MOTOR_H
#define AIN2   PBout(15)
#define AIN1   PBout(14)
#define BIN1   PBout(12)
#define BIN2   PBout(13)
void motor_init(void);
void motor_gpio_init(void);
int read_encoder3(void);
int read_encoder2(void);
void Set_PWM(int PWM1,int PWM2);
#endif

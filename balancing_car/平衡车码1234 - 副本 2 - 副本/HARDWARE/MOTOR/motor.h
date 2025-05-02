#ifndef  _MOTOR_H
#define  _MOTOR_H

#include "sys.h" 







#define Ain1  PAout(3)
#define Ain2  PAout(2)

#define Bin1  PAout(4)/*¡¾4¡¿*/
#define Bin2  PAout(5)




void Motor_Init(void);/*¡¾5¡¿*/
void Limit(int *motoA,int *motoB);
int GFP_abs(int p);
void Load(int moto1,int moto2);
#endif


/* PID.h - 电加热温度控制 PID算法优化版 头文件 */
#ifndef PID_H
#define PID_H

#include <stdint.h>
#include <stdbool.h>
#define HeaterOn()					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define HeaterOff()					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
/* PID 相关标志位结构体 */
typedef union {
    uint8_t byte;
    struct {
        uint8_t bHeaterOn  : 1;  // 加热器状态标志
        uint8_t bPIDCycle  : 1;  // PID 计算周期标志
        uint8_t reserved   : 6;
    } bt;
} PIDFlag_tu;

/* PID 误差计算结构体 */
typedef struct {
    int32_t s32Error;      // 当前误差
    int32_t s32LastError;  // 上一次误差
    int32_t s32ErrSum;     // 误差累计值
    int32_t s32ErrDiffer;  // 误差变化率
} PIDError_ts;

/* PID 相关函数声明 */
void PID_Init(void);
void PID_Task100Ms(void);
void PID_ModifySetTemp(uint32_t temp);
uint32_t PID_GetSetTemp(void);
void PID_SetHeaterState(bool state);
bool PID_GetHeaterState(void);

#endif /* PID_H */

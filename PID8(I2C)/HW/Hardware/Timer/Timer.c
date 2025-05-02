#include "Timer.h"
#include "tim.h"
#include <string.h>
#include "PID.h"

static TimerCount_ts sTimerCount;
static TimerFlag_tu uTimerState;
static TimerFlag_tu uTimerFlag;

void Timer_Init(void)
{
    memset(&sTimerCount, 0, sizeof(sTimerCount));
    uTimerState.word = 0;
    uTimerFlag.word = 0;
    HAL_TIM_Base_Start_IT(&htim2);
}

void Timer4_Init(void)
{
    HAL_TIM_Base_Start_IT(&htim4);
}

void Timer_Ms(void)
{
    sTimerCount.u16MsCount++;

    if ((sTimerCount.u16MsCount % 100) == 0)
    {
        uTimerFlag.bt.bPer100Ms = 1; // 100ms
        if ((sTimerCount.u16MsCount % 200) == 0)
        {
            uTimerFlag.bt.bPer200Ms = 1; // 200ms
        }
        if ((sTimerCount.u16MsCount % 500) == 0)
        {
            uTimerFlag.bt.bPer500Ms = 1; // 500ms
        }
    }

    if (sTimerCount.u16MsCount >= 1000) // 1秒
    {
        sTimerCount.u16MsCount = 0;
        uTimerFlag.bt.bPerSec = 1;
    }
}

void Timer4_Task(void)
{
    static uint32_t count = 0;
    count++;
    printf("TIM4 500ms 任务执行，第 %lu 次\n", count);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        Timer_Ms();
    }
    else if (htim->Instance == TIM4)
    {
        Timer4_Task();
    }
}

void Timer_Sec(void)
{
    if (uTimerState.bt.bPerSec)
    {
        uTimerState.bt.bPerSec = 0;
        sTimerCount.u8SecCount++;
        if (sTimerCount.u8SecCount >= 60)
        {
            sTimerCount.u8SecCount = 0;
            uTimerFlag.bt.bPerMin = 1;
        }
    }
}

void Timer_Min(void)
{
    if (uTimerState.bt.bPerMin)
    {
        uTimerState.bt.bPerMin = 0;
        sTimerCount.u8MinCount++;
        if (sTimerCount.u8MinCount >= 60)
        {
            sTimerCount.u8MinCount = 0;
            uTimerFlag.bt.bPerHour = 1;
        }
    }
}

bool Timer_Get100MsState(void) { return uTimerFlag.bt.bPer100Ms; }
bool Timer_Get200MsState(void) { return uTimerFlag.bt.bPer200Ms; }
bool Timer_Get500MsState(void) { return uTimerFlag.bt.bPer500Ms; }
bool Timer_GetSecState(void) { return uTimerFlag.bt.bPerSec; }
bool Timer_GetMinState(void) { return uTimerFlag.bt.bPerMin; }
bool Timer_GetHourState(void) { return uTimerFlag.bt.bPerHour; }

void Timer_Clear100MsState(void) { uTimerFlag.bt.bPer100Ms = 0; }
void Timer_Clear200MsState(void) { uTimerFlag.bt.bPer200Ms = 0; }
void Timer_Clear500MsState(void) { uTimerFlag.bt.bPer500Ms = 0; }
void Timer_ClearSecState(void) { uTimerFlag.bt.bPerSec = 0; }
void Timer_ClearMinState(void) { uTimerFlag.bt.bPerMin = 0; }
void Timer_ClearHourState(void) { uTimerFlag.bt.bPerHour = 0; }

void Timer_Control(void)
{
    Timer_Sec();
    Timer_Min();
}

void delay_us(uint16_t x)
{
    HAL_TIM_Base_Start(&htim3);
    __HAL_TIM_SetCounter(&htim3, 0);
    while (x > __HAL_TIM_GetCounter(&htim3));
    HAL_TIM_Base_Stop(&htim3);
}

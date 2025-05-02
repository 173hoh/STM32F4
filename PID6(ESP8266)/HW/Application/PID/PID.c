/* PID.c - 电加热温度控制 PID算法优化版 */
#include "Timer.h"
#include "DS18B20.h"
#include "PID.h"
#include "string.h"
#include "Debug.h"

#define HEATER_ON  1
#define HEATER_OFF 0
#define DEFAULT_SET_TEMP 4500 // 默认设定 45.00°C
#define PID_PERIOD 600 // PID 调节周期，单位 100ms
#define DELAY_TIME 30  // 温度稳定延迟时间，单位 100ms

static uint8_t pidDelayCounter = 0;
static uint16_t pidSetTemp = DEFAULT_SET_TEMP;
static uint16_t heaterOnTime = 0;
static uint16_t pidCycleCounter = 0;
static PIDError_ts pidParams = {0};
static PIDFlag_tu pidFlags = {0};

const uint16_t pidCoeffs[] = {4000, 1, 0}; // 放大100倍的Kp, Ki, Kd

void PID_SetHeaterState(bool state) {
    state ? HeaterOn() : HeaterOff();
    pidFlags.bt.bHeaterOn = state;
}

bool PID_GetHeaterState(void) {
    return pidFlags.bt.bHeaterOn;
}

void PID_ModifySetTemp(uint32_t temp) {
    pidSetTemp = temp;
}

uint32_t PID_GetSetTemp(void) {
    return pidSetTemp;
}

uint16_t PID_ComputeHeaterTime(uint32_t setTemp, uint32_t currentTemp) {
    int32_t percent = 0;
    pidParams.s32Error = (int32_t)setTemp - (int32_t)currentTemp;

    if (pidParams.s32Error > 50) {
        percent = 100;
        memset(&pidParams, 0, sizeof(pidParams));
    } else if (pidParams.s32Error < -50 && currentTemp > 4500) {
        percent = 0;
        memset(&pidParams, 0, sizeof(pidParams));
    } else {
        pidParams.s32ErrSum += pidParams.s32Error;
        pidParams.s32ErrDiffer = pidParams.s32Error - pidParams.s32LastError;
        percent = (pidCoeffs[0] * pidParams.s32Error +
                   pidCoeffs[1] * pidParams.s32ErrSum +
                   pidCoeffs[2] * pidParams.s32ErrDiffer) / 10000;
        pidParams.s32LastError = pidParams.s32Error;
        percent = percent < 5 ? 0 : (percent > 100 ? 100 : percent);
    }
    return (uint16_t)(percent * PID_PERIOD / 100);
}

void PID_CycleProcess(void) {
    if (pidDelayCounter >= DELAY_TIME) {
        pidCycleCounter = (pidCycleCounter < PID_PERIOD) ? pidCycleCounter + 1 : 0;
        if (pidCycleCounter % 10 == 0) {
            printf(" %.1f", fTemper);
        }
    } else {
        pidCycleCounter = 0;
    }
}

void PID_HeaterControl(void) {
    if (pidDelayCounter >= DELAY_TIME && !pidFlags.bt.bPIDCycle) {
        pidFlags.bt.bPIDCycle = 1;
        heaterOnTime = PID_ComputeHeaterTime(pidSetTemp, (uint32_t)(fTemper * 100));
    }
    PID_SetHeaterState(pidCycleCounter <= heaterOnTime && heaterOnTime > 0);
}

void PID_Init(void) {
    pidSetTemp = DEFAULT_SET_TEMP;
    memset(&pidParams, 0, sizeof(pidParams));
    pidDelayCounter = heaterOnTime = pidCycleCounter = 0;
    pidFlags.byte = 0;
    PID_SetHeaterState(HEATER_OFF);
}

void PID_Task100Ms(void) {
    //DS18B20_Task100Ms();
    // fTemper = DS18B20_GetTemper() / 100.0f;
    if (pidDelayCounter < DELAY_TIME) pidDelayCounter++;
    PID_CycleProcess();
    PID_HeaterControl();
}

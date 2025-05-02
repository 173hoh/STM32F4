/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS 移植实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 数字越大，优先级越大
 ****************************************************************************************************
 */

#include "freertos_demo.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO       1
#define START_TASK_STACK_SIZE 128
TaskHandle_t start_task_handler;
StaticTask_t start_task_tcb;
StackType_t start_task_stack[START_TASK_STACK_SIZE];
void start_task(void *pvParameters);
/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO       2
#define TASK1_STACK_SIZE 128
TaskHandle_t task1_handler;
StaticTask_t task1_tcb;
StackType_t task1_stack[TASK1_STACK_SIZE];
void task1(void *pvParameters);
/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO       3
#define TASK2_STACK_SIZE 128
TaskHandle_t task2_handler;
StaticTask_t task2_tcb;
StackType_t task2_stack[TASK2_STACK_SIZE];
void task2(void *pvParameters);
/******************************************************************************************************/
/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO       4
#define TASK3_STACK_SIZE 128
TaskHandle_t task3_handler;
StaticTask_t task3_tcb;
StackType_t task3_stack[TASK3_STACK_SIZE];
void task3(void *pvParameters);
/**********************************************************************/
StaticTask_t idle_task_tcb;
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];
/*空闲任务内存分配*/
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}
/*********************************************************************/
StaticTask_t timer_task_tcb;
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH];
/*软件定时器内存分配*/
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer   = &timer_task_tcb;
    *ppxTimerTaskStackBuffer = timer_task_stack;
    *pulTimerTaskStackSize   = configTIMER_TASK_STACK_DEPTH;
}
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{
    start_task_handler = xTaskCreateStatic((TaskFunction_t)start_task,       /*任务函数指针*/
                                           (char *)"start_task",             /*函数名*/
                                           (uint32_t)START_TASK_STACK_SIZE,  /*堆栈大小*/
                                           (void *)NULL,                     /*任务函数参数*/
                                           (UBaseType_t)START_TASK_PRIO,     /*任务优先级*/
                                           (StackType_t *)start_task_stack,  /*任务堆栈*/
                                           (StaticTask_t *)&start_task_tcb); /*任务控制块指针*/
    vTaskStartScheduler();                                                   // 关闭任务调度器
}
/*start_task创建完三个函数之后自删，防止创建使堆栈溢出*/
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();                                          /*进入临界区*/
    task1_handler = xTaskCreateStatic((TaskFunction_t)task1,       /*任务函数指针*/
                                      (char *)"task1",             /*函数名*/
                                      (uint32_t)TASK1_STACK_SIZE,  /*堆栈大小*/
                                      (void *)NULL,                /*任务函数参数*/
                                      (UBaseType_t)TASK1_PRIO,     /*任务优先级*/
                                      (StackType_t *)task1_stack,  /*任务堆栈*/
                                      (StaticTask_t *)&task1_tcb); /*任务控制块指针*/

    task2_handler = xTaskCreateStatic((TaskFunction_t)task2,       /*任务函数指针*/
                                      (char *)"task2",             /*函数名*/
                                      (uint32_t)TASK2_STACK_SIZE,  /*堆栈大小*/
                                      (void *)NULL,                /*任务函数参数*/
                                      (UBaseType_t)TASK2_PRIO,     /*任务优先级*/
                                      (StackType_t *)task2_stack,  /*任务堆栈*/
                                      (StaticTask_t *)&task2_tcb); /*任务控制块指针*/

    task3_handler = xTaskCreateStatic((TaskFunction_t)task3,       /*任务函数指针*/
                                      (char *)"task3",             /*函数名*/
                                      (uint32_t)TASK3_STACK_SIZE,  /*堆栈大小*/
                                      (void *)NULL,                /*任务函数参数*/
                                      (UBaseType_t)TASK3_PRIO,     /*任务优先级*/
                                      (StackType_t *)task3_stack,  /*任务堆栈*/
                                      (StaticTask_t *)&task3_tcb); /*任务控制块指针*/
    vTaskDelete(start_task_handler);
    taskEXIT_CRITICAL(); /*退出临界区，保护不想被打断的程序段*/
}
/*任务一，实现LED0每500ms翻转1次*/
void task1(void *pvParameters)
{
    while (1) {
        LED0_TOGGLE();
        vTaskDelay(500);
    }
}
/*任务二，实现LED1每500ms翻转1次*/
void task2(void *pvParameters)
{
    while (1) {
        LED1_TOGGLE();
        vTaskDelay(1000);
    }
}
/*任务三，判断按键key0，按下key0删除task1*/
void task3(void *pvParameters)
{
    uint8_t key = 0;
    while (1) {
        key = KEY_Scan(0);
        if (key == KEY0_PRES) {
            if (task1_handler != NULL) {
                printf("删除task1");
                vTaskDelete(task1_handler);
                task1_handler = NULL;
            }
        }
        vTaskDelay(1000);
    }
}
/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */

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
void start_task(void *pvParameters);
/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO       2
#define TASK1_STACK_SIZE 128
TaskHandle_t task1_handler;
void task1(void *pvParameters);
/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO       3
#define TASK2_STACK_SIZE 128
TaskHandle_t task2_handler;
void task2(void *pvParameters);
/******************************************************************************************************/
/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO       4
#define TASK3_STACK_SIZE 128
TaskHandle_t task3_handler;
void task3(void *pvParameters);

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{
    xTaskCreate((TaskFunction_t)start_task,
                (const char *)"start_task",
                (uint16_t)START_TASK_STACK_SIZE, // 无入口参数
                (void *)NULL,
                (UBaseType_t)START_TASK_PRIO,
                (TaskHandle_t *)&start_task_handler);
    vTaskStartScheduler(); // 关闭任务调度器
}
/*start_task创建完三个函数之后自删，防止创建使堆栈溢出*/
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); /*进入临界区*/
    xTaskCreate((TaskFunction_t)task1,
                (const char *)"task1",
                (uint16_t)TASK1_STACK_SIZE, // 无入口参数
                (void *)NULL,
                (UBaseType_t)TASK1_PRIO,
                (TaskHandle_t *)&task1_handler);

    xTaskCreate((TaskFunction_t)task2,
                (const char *)"task2",
                (uint16_t)TASK2_STACK_SIZE, // 无入口参数
                (void *)NULL,
                (UBaseType_t)TASK2_PRIO,
                (TaskHandle_t *)&task2_handler);

    xTaskCreate((TaskFunction_t)task3,
                (const char *)"task3",
                (uint16_t)TASK3_STACK_SIZE, // 无入口参数
                (void *)NULL,
                (UBaseType_t)TASK3_PRIO,
                (TaskHandle_t *)&task3_handler);
    vTaskDelete(NULL);
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
                task1_handler=NULL;
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

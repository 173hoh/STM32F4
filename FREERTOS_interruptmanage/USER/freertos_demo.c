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
#include "delay.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO       1
#define START_TASK_STACK_SIZE 256
TaskHandle_t start_task_handler;
void start_task(void *pvParameters);
/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO       2
#define TASK1_STACK_SIZE 256
TaskHandle_t task1_handler;
void task1(void *pvParameters);

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

    vTaskDelete(NULL);
    taskEXIT_CRITICAL(); /*退出临界区，保护不想被打断的程序段*/
}
/*任务一，实现LED0每500ms翻转1次*/
void task1(void *pvParameters)
{
    uint8_t task1_num = 0;
    while (1) {
        if (++task1_num == 5) {
            task1_num = 0;
            printf("关闭中断！！！！！！\r\n");
            taskENTER_CRITICAL();  // 进入临界区
            // 禁用特定的中断，例如外部中断或定时器中断
            portDISABLE_INTERRUPTS();
            delay_ms(5000);
            portENABLE_INTERRUPTS();  // 恢复中断
            printf("打开中断！！！！！！\r\n");
            taskEXIT_CRITICAL();  // 退出临界区
        }
        LED0 = ~LED0;
        vTaskDelay(1000);  // 延时1秒，LED翻转
    }
}

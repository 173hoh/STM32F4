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
#include "key.h"
#include "oled.h"
#include "DHT11.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
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
				u8 temperature;  	    
				u8 humidity;
				u8 temp_buf[3];  // 用于存储温度字符串
				u8 humi_buf[3];  // 用于存储湿度字符串	
	while(DHT11_Init())
	{
		OLED_ShowString(0,40,"err",12,1);  
		vTaskDelay(1000);
	}	
		OLED_ShowString(0,0,"temp",12,1);  
		OLED_ShowString(0,10,"humi",12,1);  
	while(1)
	{
		DHT11_Read_Data(&temperature,&humidity);		//读取温湿度值			
		sprintf(temp_buf, "%2d", temperature);
		sprintf(humi_buf, "%2d", humidity);
		OLED_ShowString(36,0,temp_buf,12,1);  
		OLED_ShowString(36,10,humi_buf,12,1); 
		OLED_Refresh();	 
		vTaskDelay(10);
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
	float pitch,roll,yaw; 			//欧拉角
	short aacx,aacy,aacz;			//加速度传感器原始数据
	short gyrox,gyroy,gyroz;		//陀螺仪原始数据
	
	while(mpu_dmp_init())
	{
		OLED_ShowString(0,20,"err",12,1);  
		vTaskDelay(1000);
	}
		OLED_ShowString(0,30,"pitch",12,1);  
		OLED_ShowString(0,40,"roll",12,1);  
		OLED_ShowString(0,50,"yaw",12,1);  
	
    while (1) 
		{
      if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
      {
		    MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		    MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			}
			OLED_ShowNum(36,30,pitch,3,12,1);  
			OLED_ShowNum(36,40,roll,3,12,1); 
			OLED_ShowNum(36,50,yaw,3,12,1);
			OLED_Refresh();	 
			vTaskDelay(1000);
		}
}
/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */

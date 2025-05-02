#include "stm32f10x.h"

#include "dht11.h"
#include "ADC.h"
#include "ws2812B.h"
#include "control.h"
#include "usart.h"
#include "delay.h"
u8 temp;              // 存储温度值
u8 humi;              // 存储湿度值
float ppm;
float lux;

void Hardware_init()
{
    SysTick_Config(SystemCoreClock / 1000);  // 1ms 节拍

    delay_init();                // delay初始化
    uart_init(115200);            // 串口初始化
    uart2_init(115200);  //初始化串口1，接HC-05蓝牙模块
	
		OLED_Init(); 
		OLED_ShowString(1, 1, "libarary");
		OLED_ShowString(2, 1, "temp");
		OLED_ShowString(3, 1, "humi");
		OLED_ShowString(4, 1, "MQ2");
		OLED_ShowString(4, 7, "Lux");
    ADC_init();         
    control_Init();        // 初始化
    PWM_WS2812B_Init(WS2812B_ARR);	//WS2812B初始化		
    WS2812B_Reset();								//复位
		PWM_WS2812B_White(8);   // 如果光照小于200，执行白光显示
    while(DHT11_Init())
    {
        printf("DHT11 Error \r\n");
        delay_ms(1000);
    }
}

void Hardware_read()
{
    char str[100];
    DHT11_Read_Data(&temp, &humi);
    ppm = mq2_get_ppm(); 
    delay_ms(100);
    lux = light_get_lux();
		OLED_ShowNum(2, 7, (uint32_t)temp, 2);
		OLED_ShowNum(3, 7, (uint32_t)humi, 2);
		OLED_ShowNum(4, 5, (uint32_t)ppm, 1);
		OLED_ShowNum(4, 11, (uint32_t)lux, 3);
    printf("temp= %d ,humi= %d,MQ2= %.2f ppm,light= %.2f lux\r\n", temp, humi, ppm, lux);
    sprintf(str, "temp= %d ,humi= %d, MQ2= %.2f ppm,light= %.2f lux\r\n", temp, humi, ppm, lux);
    USART2_SendString(USART2, str);
}

void Hardware_control()
{
    static uint32_t last_time = 0;
    uint32_t now = millis();

    // 每隔 1000ms 执行一次控制任务
    if (now - last_time >= 5000)
    {
        last_time = now;

        // 读取传感器数据
        Hardware_read();

        // 定义三项阈值
        uint8_t temp_threshold = 30;
        uint8_t humi_threshold = 60;
        float ppm_threshold = 4.0f;
        float lux_threshold = 200.0f;  // 新增：光照阈值

        // 判断是否有任意一项超标
        uint8_t alarm = 0;

        if (temp > temp_threshold || humi > humi_threshold || ppm > ppm_threshold)
        {
            alarm = 1;
        }

        // 判断光照是否过低
        if (lux < lux_threshold)
        {
            PWM_WS2812B_White(8);   // 如果光照小于200，执行白光显示
        }
				else
				{
					 PWM_WS2812B_Off(8);  // 彻底熄灭8个灯

				}

        // 控制风扇、蜂鸣器和LED灯
        if (alarm)
        {
            Fan_On();
            Buzzer_On();
            delay_ms(500);
            Buzzer_Off();
            PWM_WS2812B_Red(8);   // 红灯报警
        }
        else
        {
            Fan_Off();
            Buzzer_Off();
            //PWM_WS2812B_Green(8); // 绿灯正常
        }
    }
}

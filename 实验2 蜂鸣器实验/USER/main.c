#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "rc522.h"
#include "dht11.h"   // RC522 RFID模块的驱动及通信支持

//ALIENTEK 探索者STM32F407开发板 实验2
//蜂鸣器实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司    
//作者：正点原子 @ALIENTEK

int main(void)
{ 
	u8 temperature;              // 存储温度值
	u8 humidity;                 // 存储湿度值
	u8 temp_buf[3];              // 存储温度字符串
	u8 humi_buf[3];              // 存储湿度字符串
 
	delay_init(168);		   //初始化延时函数
	uart_init(115200);          // 初始化串口波特率为115200
	LED_Init();				    //初始化LED端口 
	OLED_Init();
	RC522_Init();
	DHT11_Init();
	OLED_ShowString(3,1,"SUM");
	OLED_ShowString(4,1,"Free");
	while(1)
	{ 
		//t++;
		DHT11_Read_Data(&temperature, &humidity); // 读取温湿度
		printf("Temp:%d C\r\n", temperature);
		printf("Humi:%d %%\r\n", humidity);
		sprintf(temp_buf, "%2d", temperature);    // 将温度转换为字符串
		sprintf(humi_buf, "%2d", humidity);       // 将湿度转换为字符串
		OLED_ShowString(1,1,"temp");  
		OLED_ShowString(2,1,"humi");  
		OLED_ShowString(1, 5, temp_buf);          // 在OLED上显示温度
		OLED_ShowString(2, 5, humi_buf);          // 在OLED上显示湿度
	 
		//printf("ID:");
			//RC522_Handel(); // 处理RFID模块操作			
			
	}
}

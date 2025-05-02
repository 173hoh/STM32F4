#pragma diag_suppress 161   // 阻止编译器显示 "Warning:  #161-D: unrecognized #pragma" 警告，因为在 vscode 中使用了 #pragma region 来折叠代码

#include "gd32f4xx.h"
#include "stdio.h"
#include "systick.h"
#include "test.h"
#include "board_uart.h"

#include "board_led.h"
#include "beep.h"
#include "bmp180.h"
#include "dht11.h"
#include "dwin.h"
#include "esp8266.h"
#include "flame.h"
#include "fpm383.h"
#include "hx1838.h"
#include "ky037.h"
#include "motor.h"
#include "mq2.h"
#include "rc522.h"
#include "relay.h"
#include "sr602.h"
#include "step2p4w.h"
#include "su03t.h"
#include "tc1508.h"
#include "temt6000.h"
#include "ws2812.h"


#pragma region 一些模块的回调设置
void beep() {
    
    beep_init();
    delay_1ms(2500);

    beep_beep(4, 100, 300);
    delay_1ms(2000);

    beep_deinit();
    delay_1ms(3000);
}

void bmp180() {
    printf("开始测试 气压传感器\r\n");
    
    bmp180_init();
    bmp180_calibrate();
    delay_1ms(2000);

    float t = bmp180_get_temperature();
    float p = bmp180_get_pressure();

    printf("当前环境温度为 %.2f 摄氏度, 大气压强为 %.2f 帕\r\n", t, p);
	
    delay_1ms(11000);

    printf("气压传感器 测试结束\r\n");
    bmp180_deinit();
    delay_1ms(3000);
}
void esp8266() {
    printf("开始测试 WiFi 模块\r\n");
   
    esp8266_peripheral_init();
    delay_1ms(3000);

    if(send_command("AT\r\n", "OK\r\n", 2000, 2) == Success){
        printf("WiFi 模块正常，测试结束\r\n");
    } else {
        printf("WiFi 模块异常，请检查\r\n");
    }
		
		uint8_t esp8266_init();
    esp8266_peripheral_deinit();
    delay_1ms(3000);
}
# 模板目录说明

├─ app
├─ board
├─ bsp
├─ cmsis
├─ doc
├─ library
├─ module
└─ project

* app:      存放一些项目逻辑、模块调用等文件，main 入口也在此处，主要由用户编写
* board:    存放核心开发板的板载资源及初始化
* bsp:      存放片上外设支持包 (板级支持包)，比如 ADC、TIMER 等，供各模块调用或编写时参考
* cmsis:    存放 CMSIS 标准文件，主要为 系统时钟、系统中断 等
* doc:      存放开发中用到的相关文档
* library:  存放标准库文件，本例中为 gd32f4xx 标准固件库
* module:   存放各种传感器模块代码，供用户在 app 目录中调用
* project:  存放工程文件，目前包含 keil5 和 vscode 的 EIDE 项目两种工程，编译结果相同

---

## app
- main.c

## board
- board_led.c
- board_uart.c
- board.c

## bsp
- bsp_adc.c
- bsp_basic_timer.c
- bsp_led.c
- bsp_pwm.c
- bsp_usart.c

## module
- beep.c
- bmp180.c
- dht11.c
- dwin.c
- esp8266.c
- flame.c
- fpm383.c
- hx1838.c
- ky037.c
- motor.c
- mq2.c
- rc522.c
- relay.c
- sr602.c
- su03t.c
- tc1508.c
- temt6000.c
- ws2812.c

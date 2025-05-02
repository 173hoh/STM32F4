/**
 * @file    test.c
 * @brief   用语音等方式对各传感器模块进行测试
 *
 * @version 2024-08-15, V1.0, yanf, 厦门芯力量
 */

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


/** @brief  存储测试过程中的一些临时值，比如红外遥控接收码等 */
uint16_t test_temp = 0;

/** @brief  用来指示一个模块是正在测试过程中还是已经结束 */
uint8_t test_end = 0;

/** @brief  在一些循环中用来计时以退出循环 */
uint16_t test_time = 0;


#pragma region 一些模块的回调设置

void test_dwin_callback(uint16_t addr, uint8_t code) {
    test_temp = addr;
}


void test_ir_callback(uint8_t code) {
    test_temp = code;
}


void test_sr602_callback(void) {
    test_temp = 1;
}

#pragma endregion

#pragma region 智能语音模块功能函数

/**
 * @brief   给智能语音模块发送指令
 *
 * @param   code: 在智能语音模块(su-03t)中定义好的指令码
 */
void test_ai_send(uint8_t code) {
    uint8_t data[] = { 0xAA, 0x55, code, 0x55, 0xAA };

    su03t_send_bytes(data, 5);
}


/**
 * @brief   给智能语音模块发送带参数的指令
 *
 * @param   code: 在智能语音模块(su-03t)中定义好的指令码
 * @param   arg1: 参数值，在智能语音固件中会解析此参数
 */
void test_ai_send_arg(uint8_t code, int arg1) {
    uint8_t data[9] = {0};

    data[0] = 0xAA;
    data[1] = 0x55;
    data[2] = code;
    data[3] = arg1 & 0xFF;
    data[4] = (arg1 >> 8) & 0xFF;
    data[5] = (arg1 >> 16) & 0xFF;
    data[6] = (arg1 >> 24) & 0xFF;
    data[7] = 0x55;
    data[8] = 0xAA;

    su03t_send_bytes(data, 9);
}


/**
 * @brief   给智能语音模块发送带两个参数的指令
 *
 * @param   code: 在智能语音模块(su-03t)中定义好的指令码
 * @param   arg1: 第一个参数值
 * @param   arg2: 第二个参数值
 */
void test_ai_send_arg2(uint8_t code, int arg1, int arg2) {
    uint8_t data[13] = {0};

    data[0] = 0xAA;
    data[1] = 0x55;
    data[2] = code;
    data[3] = arg1 & 0xFF;
    data[4] = (arg1 >> 8) & 0xFF;
    data[5] = (arg1 >> 16) & 0xFF;
    data[6] = (arg1 >> 24) & 0xFF;
    data[7] = arg2 & 0xFF;
    data[8] = (arg2 >> 8) & 0xFF;
    data[9] = (arg2 >> 16) & 0xFF;
    data[10] = (arg2 >> 24) & 0xFF;
    data[11] = 0x55;
    data[12] = 0xAA;

    su03t_send_bytes(data, 13);
}

#pragma endregion

#pragma region 各模块测试代码

/**
 * @brief   测试超时设定
 *
 */
void test_timeout(void)
{
    printf("测试等待超时，请在随后单独测试，现在将进入下一项测试\r\n");
    test_ai_send(0x09);
    delay_1ms(5000);
}


void test_beep(uint8_t code) {
    printf("开始测试 蜂鸣器\r\n");
    test_ai_send(code);

    beep_init();
    delay_1ms(2500);

    printf("蜂鸣器鸣叫三声\r\n");
    test_ai_send(code + 1);
    delay_1ms(2500);
    beep_beep(4, 100, 300);
    delay_1ms(2000);

    printf("蜂鸣器 测试结束\r\n");
    test_ai_send(code + 4);
    beep_deinit();
    delay_1ms(3000);
}


void test_bmp180(uint8_t code) {
    printf("开始测试 气压传感器\r\n");
    test_ai_send(code);

    bmp180_init();
    bmp180_calibrate();
    delay_1ms(2000);

    float t = bmp180_get_temperature();
    float p = bmp180_get_pressure();

    printf("当前环境温度为 %.2f 摄氏度, 大气压强为 %.2f 帕\r\n", t, p);
    test_ai_send_arg2(code + 2, t / 1, p / 1);
    delay_1ms(11000);

    printf("气压传感器 测试结束\r\n");
    test_ai_send(code + 4);
    bmp180_deinit();
    delay_1ms(3000);
}


void test_dht11(uint8_t code) {
    printf("开始测试 温湿度传感器\r\n");
    test_ai_send(code);

    dht11_init();
    delay_1ms(3000);

    float tem, hum;
    dht11_read_data(&tem, &hum);

    if (tem < 1 && hum < 1) {
        printf("温湿度传感器异常，请检查\r\n");
        test_ai_send(code + 3);
    } else {
        printf("当前温度为 %.2f 摄氏度, 相对湿度为 %.2f%%\r\n", tem, hum);
        test_ai_send_arg2(code + 2, tem / 1, hum / 1);
        delay_1ms(8000);

        printf("温湿度传感器 测试结束\r\n");
        test_ai_send(code + 4);
    }

    dht11_deinit();
    delay_1ms(3000);
}


void test_dwin(uint8_t code) {
    printf("开始测试 屏幕\r\n");
    test_ai_send(code);

    dwin_init(test_dwin_callback);
    delay_1ms(2000);

    printf("请点击屏幕左下方的 [数值调节] 图标\r\n");
    test_ai_send(code + 1);

    test_time = 0;
    test_temp = 0;

    while (test_temp  == 0 && test_time++ < 150) {
        delay_1ms(100);
    }

    if (test_time <= 150) {
        printf("读取到触摸地址值 %#x, 屏幕触摸正常\r\n", test_temp);
        test_ai_send(code + 3);
        delay_1ms(4000);

        printf("屏幕 测试结束\r\n");
        test_ai_send(code + 4);
    } else {
        test_timeout();
    }

    dwin_deinit();
    delay_1ms(3000);
}


void test_esp8266(uint8_t code) {
    printf("开始测试 WiFi 模块\r\n");
    test_ai_send(code);

    esp8266_peripheral_init();
    delay_1ms(3000);

    if(send_command("AT\r\n", "OK\r\n", 2000, 2) == Success){
        printf("WiFi 模块正常，测试结束\r\n");
        test_ai_send(code + 4);
    } else {
        printf("WiFi 模块异常，请检查\r\n");
        test_ai_send(code + 3);
    }

    esp8266_peripheral_deinit();
    delay_1ms(3000);
}


void test_flame(uint8_t code) {
    printf("开始测试 火焰传感器\r\n");
    test_ai_send(code);

    flame_init(0);
    delay_1ms(3000);

    uint16_t adc_val_sum = 0;

    for (uint8_t i = 0; i < 10; i++) {
        adc_val_sum += flame_get_channel_data();
        delay_1ms(100);
    }

    uint16_t adc_val = adc_val_sum / 10;

    printf("模拟信号连续十次采样平均值为 %d\r\n", adc_val);
    test_ai_send_arg(0x0A, adc_val);
    delay_1ms(7000);

    if (adc_val < 200) {
        printf("采样值太小，请检查火焰传感器\r\n");
        test_ai_send(code + 3);
    } else {
        printf("火焰传感器 测试结束\r\n");
        test_ai_send(code + 4);
    }

    flame_deinit();
    delay_1ms(3000);
}


void test_fpm383(uint8_t code) {
    printf("开始测试 指纹传感器\r\n");
    test_ai_send(code);

    fingerprint_init();
    delay_1ms(2500);

    printf("指纹传感器开启绿色呼吸灯\r\n");
    test_ai_send(code + 1);
    delay_1ms(2000);
    fp_led_breathe(1, 100, 20, 100);
    delay_1ms(4000);

    printf("请将手指放在指纹传感器上\r\n");
    test_ai_send(code + 2);
    delay_1ms(3000);

    test_time = 0;

    while (fp_finger_state() == 0 && test_time++ < 150) {
        delay_1ms(100);
    }

    if (test_time <= 150) {
        printf("检测到手指按压\r\n");
        test_ai_send(code + 3);
        delay_1ms(3000);

        printf("指纹传感器 测试结束\r\n");
        test_ai_send(code + 4);
    } else {
        test_timeout();
    }

    delay_1ms(2500);
    fp_led_switch(0, 0);
    delay_1ms(500);
    fingerprint_deinit();
    delay_1ms(500);
}


void test_hx1838(uint8_t code) {
    printf("开始测试 红外接收模块\r\n");
    test_ai_send(code);

    infrared_init(test_ir_callback);
    delay_1ms(2500);

    printf("请按下红外遥控器按键\r\n");
    test_ai_send(code + 1);
    delay_1ms(2000);

    test_time = 0;
    test_temp = 0;

    while (test_temp == 0 && test_time++ < 150) {
        delay_1ms(100);
    }

    if (test_time <= 150) {
        uint8_t ir_code = (uint8_t)test_temp;
        printf("收到红外按键码 %#x, 对应按键为 %c\r\n", ir_code, ir_convert_code(ir_code));
        test_ai_send_arg(code + 3, ir_code);
        delay_1ms(4000);

        printf("红外接收模块 测试结束\r\n");
        test_ai_send(code + 4);
    } else {
        test_timeout();
    }

    infrared_deinit();
    delay_1ms(3000);
}


void test_ky037(uint8_t code) {
    printf("开始测试 声音传感器\r\n");
    test_ai_send(code);

    ky037_init(NULL);
    delay_1ms(2000);

    uint16_t adc_val_sum = 0;

    for (uint8_t i = 0; i < 10; i++) {
        adc_val_sum += ky037_get_channel_data();
        delay_1ms(100);
    }

    uint16_t adc_val = adc_val_sum / 10;

    printf("模拟信号连续十次采样平均值为 %d\r\n", adc_val);
    test_ai_send_arg(0x0A, adc_val);
    delay_1ms(6000);

    printf("声音传感器 测试结束\r\n");
    test_ai_send(code + 4);
    ky037_deinit();
    delay_1ms(3000);
}


void test_led(uint8_t code) {
    printf("开始测试 LED 发光二极管\r\n");
    test_ai_send(code);

    led_init(LED_ALL);
    delay_1ms(3000);

    printf("板载 LED1、LED2、LED3 开始闪烁\r\n");
    test_ai_send(code + 1);
    delay_1ms(2500);

    for (uint8_t i = 0; ; i++) {
        led_toggle(LED_ALL);
        delay_1ms(30);

        if (i == 100) {
            printf("熄灭全部板载 LED\r\n");
            test_ai_send(code + 2);
        } else if (i == 150) {
            break;
        }
    }

    led_off(LED_ALL);
    delay_1ms(2000);

    printf("板载 LED 测试结束\r\n");
    test_ai_send(code + 4);
    led_deinit(LED_ALL);
    delay_1ms(3000);
}


void test_motor(uint8_t code) {
    printf("开始测试 直流电机\r\n");
    test_ai_send(code);

    motor_init();
    delay_1ms(2000);

    printf("电机转速 20%%\r\n");
    test_ai_send(code + 1);
    delay_1ms(2000);

    motor_start();
    motor_set_speed(20);
    delay_1ms(3000);
    motor_stop();
    delay_1ms(2000);

    printf("电机全速转动\r\n");
    test_ai_send(code + 2);
    delay_1ms(2000);

    motor_start();
    motor_set_speed(100);
    delay_1ms(3000);

    printf("停止电机转动\r\n");
    test_ai_send(code + 3);
    delay_1ms(2000);
    motor_stop();
    delay_1ms(2000);

    printf("直流电机 测试结束\r\n");
    test_ai_send(code + 4);
    motor_deinit();
    delay_1ms(3000);
}


void test_mq2(uint8_t code) {
    printf("开始测试 烟雾传感器\r\n");
    test_ai_send(code);

    mq2_init(NULL);
    delay_1ms(2000);

    uint16_t adc_val_sum = 0;

    for (uint8_t i = 0; i < 10; i++) {
        adc_val_sum += mq2_get_channel_data();
        delay_1ms(100);
    }

    uint16_t adc_val = adc_val_sum / 10;

    printf("模拟信号连续十次采样平均值为 %d\r\n", adc_val);
    test_ai_send_arg(0x0A, adc_val);
    delay_1ms(6000);

    if (adc_val < 150) {
        printf("采样值太小，请检查烟雾传感器\r\n");
        test_ai_send(code + 3);
        delay_1ms(2000);
    } else {
        printf("烟雾传感器 测试结束\r\n");
        test_ai_send(code + 4);
    }

    mq2_deinit();
    delay_1ms(3000);
}


void test_rc522(uint8_t code) {
    printf("开始测试 射频感应模块\r\n");
    test_ai_send(code);

    RC522_Init();
    RC522_Rese();
    delay_1ms(3000);

    printf("请刷卡测试 (将卡片贴紧传感器)\r\n");
    test_ai_send(code + 1);
    delay_1ms(2000);

    uint8_t rfid[4] = {0};

    test_time = 0;

    while (test_time++ < 150) {
        if (PcdRequest(PICC_REQALL, rfid) == MI_OK) {
            if (PcdAnticoll(rfid) == MI_OK) {
                printf("读卡成功，卡号为: %x %x %x %x\r\n", rfid[0], rfid[1], rfid[2], rfid[3]);
                test_ai_send(code + 3);
                break;
            }
        }
        delay_1ms(100);
    }

    if (test_time <= 150) {
        delay_1ms(2000);
        printf("射频感应模块 测试结束\r\n");
        test_ai_send(code + 4);
        delay_1ms(3000);
    } else {
        test_timeout();
    }
}


void test_relay(uint8_t code) {
    printf("开始测试 继电器模块\r\n");
    test_ai_send(code);

    relay1_init();
    relay2_init();
    delay_1ms(2500);

    printf("继电器 1 通断 3 次\r\n");
    test_ai_send(code + 1);
    delay_1ms(2000);

    for (uint8_t i = 0; i < 3; i++) {
        relay1_on();
        delay_1ms(500);
        relay1_off();
        delay_1ms(500);
    }
    delay_1ms(500);

    printf("继电器 2 通断 3 次\r\n");
    test_ai_send(code + 2);
    delay_1ms(2000);

    for (uint8_t i = 0; i < 3; i++) {
        relay2_on();
        delay_1ms(500);
        relay2_off();
        delay_1ms(500);
    }
    delay_1ms(500);

    printf("继电器模块 测试结束\r\n");
    test_ai_send(code + 4);
    relay_deinit();
    delay_1ms(3000);
}


void test_sr602(uint8_t code) {
    printf("开始测试 红外感应模块\r\n");
    test_ai_send(code);

    sr602_init(test_sr602_callback);
    delay_1ms(3000);

    printf("请用手掌遮挡在红外感应模块前方\r\n");
    test_ai_send(code + 1);
    delay_1ms(4000);

    test_time = 0;
    test_temp = 0;

    while (test_temp == 0 && test_time++ < 150) {
        delay_1ms(100);
    }

    if (test_time <= 150) {
        printf("感应到人体移动\r\n");
        test_ai_send(code + 3);
        delay_1ms(3000);

        printf("红外感应模块 测试结束\r\n");
        test_ai_send(code + 4);
    } else {
        test_timeout();
    }

    sr602_deinit();
    delay_1ms(3000);
}


void test_stepper24(uint8_t code) {
    printf("开始测试 步进电机\r\n");
    test_ai_send(code);

    stepper_2_phase_4_wire_init();
    delay_1ms(2000);

    printf("正转测试\r\n");
    test_ai_send(code + 1);
    delay_1ms(1000);

    stepper24_angle(1, 360 * 14.5);
    delay_1ms(4000);

    printf("反转测试\r\n");
    test_ai_send(code + 2);
    delay_1ms(1000);

    stepper24_angle(0, 360 * 14.5);
    delay_1ms(4000);

    printf("步进电机 测试结束\r\n");
    test_ai_send(code + 4);

    stepper24_deinit();
    delay_1ms(3000);
}


void test_stepper_tc1508(uint8_t code) {
    printf("开始测试 步进电机\r\n");
    test_ai_send(code);

    tc1508_stepper_init();
    delay_1ms(2000);

    printf("正转测试\r\n");
    test_ai_send(code + 1);
    delay_1ms(1000);

    tc1508_angle(1, 360 * 14.5);
    delay_1ms(4000);

    printf("反转测试\r\n");
    test_ai_send(code + 2);
    delay_1ms(1000);

    tc1508_angle(0, 360 * 14.5);
    delay_1ms(4000);

    printf("步进电机 测试结束\r\n");
    test_ai_send(code + 4);

    tc1508_deinit();
    delay_1ms(3000);
}


void test_temt6000(uint8_t code) {
    printf("开始测试 光敏传感器\r\n");
    test_ai_send(code);

    temt6000_init();
    delay_1ms(2000);

    uint16_t adc_val_sum = 0;

    for (uint8_t i = 0; i < 10; i++) {
        adc_val_sum += temt6000_get_channel_data();
        delay_1ms(100);
    }

    uint16_t adc_val = adc_val_sum / 10;

    printf("模拟信号连续十次采样平均值为 %d\r\n", adc_val);
    test_ai_send_arg(0x0A, adc_val);
    delay_1ms(6000);

    printf("光敏传感器 测试结束\r\n");
    test_ai_send(code + 4);
    temt6000_deinit();
    delay_1ms(3000);
}


void test_ws2812(uint8_t code) {
    printf("开始测试 彩色灯环\r\n");
    test_ai_send(code);

    ws2812_init();
    delay_1ms(2500);

    printf("点亮 1 颗红灯\r\n");
    test_ai_send(code + 1);
    delay_1ms(1500);
    ws2812_set_color(0xFF, 0, 0);
    delay_1ms(3000);

    printf("点亮 8 颗绿灯\r\n");
    test_ai_send(code + 2);
    delay_1ms(1500);

    for (uint8_t i = 0; i < 8; i++) {
        ws2812_set_color(0, 0xFF, 0);
    }
    delay_1ms(3000);

    printf("熄灭全部灯珠\r\n");
    test_ai_send(code + 3);
    delay_1ms(1500);

    for (uint8_t i = 0; i < 8; i++) {
        ws2812_set_color(0, 0, 0);
    }
    delay_1ms(2000);

    printf("彩色灯环 测试结束\r\n");
    test_ai_send(code + 4);
    ws2812_deinit();
    delay_1ms(3000);
}


/**
 * @brief   模块测试的入口，测试单个传感器模块
 *
 * @param   code: 在智能语音固件中定义的指令码
 */
void test_one(uint8_t code) {
    switch (code) {
        case 0x10:
            test_dwin(code);
            break;

        case 0x1A:
            test_mq2(code);
            break;

        case 0x20:
            if (EXPANSION_BOARD_VERSION < 1.7) {
                test_stepper24(code);
            } else {
                test_stepper_tc1508(code);
            }
            break;

        case 0x2A:
            test_rc522(code);
            break;

        case 0x30:
            test_dht11(code);
            break;

        case 0x3A:
            test_bmp180(code);
            break;

        case 0x40:
            test_relay(code);
            break;

        case 0x4A:
            test_sr602(code);
            break;

        case 0x50:
            test_ky037(code);
            break;

        case 0x5A:
            test_led(code);
            break;

        case 0x60:
            test_hx1838(code);
            break;

        case 0x6A:
            test_ws2812(code);
            break;

        case 0x70:
            test_motor(code);
            break;

        case 0x7A:
            test_flame(code);
            break;

        case 0x80:
            test_beep(code);
            break;

        case 0x8A:
            test_esp8266(code);
            break;

        case 0x90:
            test_fpm383(code);
            break;

        case 0x9A:
            test_temt6000(code);
            break;
    }

    test_end = 1;
}


/**
 * @brief   模块测试的入口，对全部模块逐个进行测试
 *
 */
void test_all() {
    uint8_t code = 0x10;
    delay_1ms(1000);
    printf("\r\n==============================\r\n");

    while (code <= 0x9A) {
        test_end = 0;

        test_one(code);
        printf("==============================\r\n");

        while (!test_end) {
            delay_1ms(1000);
        }

        // 在智能语音固件中，各模块指令码以 10 1A 20 2A 30 ... 的方式分隔定义
        if (code % 16 == 0) {
            code += 10;
        } else {
            code += 6;
        }
    }

    printf("全部模块测试结束\r\n");
    test_ai_send(0x05);
}

#pragma endregion


/**
 * @brief   接收智能语音模块(su-03t)发送来的数据
 *
 * @param   recv: 数据
 * @param    len: 数据长度
 */
void test_ai_callback(uint8_t *recv, uint8_t len) {
    // printf("\r\n==>  ");
    // for (uint8_t i = 0; i < len; i++) {
    //     printf("%x ", recv[i]);
    // }
    // printf("\r\n");

    if (recv[2] == 0xEE) {
        if (recv[3] == 0xEE) {          // 握手
            printf("正在准备模块自检\r\n");
            delay_1ms(2000);
            test_ai_send(0x01);
            printf("现在我已为模块自检做好准备，请在确认已接好屏幕和所有传感器模块后，跟我说确认自检吧。\r\n");
        } else if (recv[3] == 0xE0) {   // 全部测试
            printf("模块自检即将开始，我会从电源接口位置开始，按顺时针方向逐个测试。\r\n");
            delay_1ms(8000);
            test_all();
        } else {                        // 单项测试
            test_one(recv[3]);
        }

        su03t_receive_clear();
    }
}

#pragma endregion

/**
 * @brief   在测试前，对一些回调函数进行设置
 *
 */
void test_init(void) {
    su03t_init(115200, test_ai_callback);
}

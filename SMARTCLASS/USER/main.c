// *******************************
// ** 1. 系统核心与基本功能相关头文件 **
// *******************************
#include "sys.h"      // 系统初始化、外设配置和通用功能
#include "delay.h"    // 延时功能，用于时间控制
#include "usart.h"    // 串口通信功能，支持数据收发

// ********************************
// ** 2. 显示与用户交互相关头文件 **
// ********************************
#include "oled.h"     // OLED显示屏的驱动及显示功能
// ****************************
// ** 3. 存储管理相关头文件 **
// ****************************
#include "sram.h"        // 外部SRAM的初始化及数据存储支持
#include "malloc.h"      // 动态内存分配管理
#include "sdio_sdcard.h" // SD卡接口及读写功能
#include "ff.h"          // FAT文件系统功能，管理文件存储
#include "exfuns.h"      // 文件系统的扩展功能支持

// *************************
// ** 4. 外设驱动头文件 **
// *************************
#include "rc522.h"   // RC522 RFID模块的驱动及通信支持

// ***************************
// ** 5. 调试与开发工具头文件 **
// ***************************
#include "usmart.h"  // 智能调试工具，用于函数调用调试

// 主函数
int main(void)
{        
    u32 total, free, i;          // 存储SD卡容量信息
    u8 res = 0;                  // 用于存储函数返回值
    u8 temperature;              // 存储温度值
    u8 humidity;                 // 存储湿度值
    u8 temp_buf[3];              // 存储温度字符串
    u8 humi_buf[3];              // 存储湿度字符串
    FIL file;                    // 文件句柄

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置系统中断优先级分组2
    delay_init(168);            // 初始化延时函数
    uart_init(115200);          // 初始化串口波特率为115200
    OLED_Init();                // 初始化OLED显示屏
    OLED_Clear();               // 清屏
    usmart_dev.init(84);        // 初始化USMART（调试工具）
    W25QXX_Init();              // 初始化W25Q128存储器
    RC522_Init();               // 初始化RC522（RFID模块）
    my_mem_init(SRAMIN);        // 初始化内部SRAM内存池
    my_mem_init(SRAMCCM);       // 初始化CCM内存池

    // 初始化DHT11温湿度传感器
    while(DHT11_Init()) {
        printf("DHT11 Error\r\n");
        OLED_ShowString(1, 1, "DHT11 Init Error");
        delay_ms(400);           // 初始化失败，延时400ms后重试
    }

    // 检测SD卡是否初始化成功
    while(SD_Init()) {
        OLED_ShowString(3, 1, "SD Init Error");
        delay_ms(500);
    }

    exfuns_init();              // 初始化文件系统相关变量
    res = f_mount(fs[0], "0:", 1);    // 挂载SD卡
    if (res != FR_OK) {
        OLED_ShowString(7, 1, "SD Mount Failed");
        printf("SD Mount Failed: %d\r\n", res);
        delay_ms(500);
    }

    res = f_mount(fs[0], "1:", 1);  // 挂载FLASH存储器
    if (res != FR_OK) {
        OLED_ShowString(7, 1, "Flash Mount Failed");
        printf("Flash Mount Failed: %d\r\n", res);
        delay_ms(500);
    }
    
    printf("f_mount(0:) return: %d\r\n", res);

    if (res == 0X0D) { // FR_NO_FILESYSTEM，通常表示没有文件系统
        OLED_ShowString(7, 1, "No FS, Try Create");
        delay_ms(500);

        // **创建 test.txt 并写入数据**
        res = f_open(&file, "1:test.txt", FA_CREATE_ALWAYS | FA_WRITE);
        if (res == FR_OK) {
            f_puts("Hello, W25QXX!", &file);
            f_close(&file);
            OLED_ShowString(7, 1, "File Created");
        } else {
            OLED_ShowString(7, 1, "Create Failed");
            printf("File creation failed, error code: %d\r\n", res);
        }
        delay_ms(1000);
    }

    // **尝试打开 test.txt 读取内容**
    res = f_open(&file, "1:test.txt", FA_READ | FA_OPEN_EXISTING);
    if (res == FR_OK) {
        OLED_ShowString(7, 1, "OK");
        printf("OK\r\n");
        f_close(&file);
    } else {
        OLED_ShowString(7, 1, "Failed");
        printf("Failed\r\n");
    }

    delay_ms(1000);

    // 获取SD卡容量信息
    while (exf_getfree("0", &total, &free)) {
        OLED_ShowString(3, 1, "Error2");
        delay_ms(200);
    }
    OLED_ShowString(3, 1, "SUM");
    OLED_ShowString(4, 1, "Free");
    OLED_ShowNum(3, 5, total, 4);
    OLED_ShowNum(4, 5, free, 4);

    // 主循环
    while (1) {
        RC522_Handel(); // 处理RFID模块操作

        // 显示温湿度信息
        DHT11_Read_Data(&temperature, &humidity); // 读取温湿度
        printf("Temp:%d C\r\n", temperature);
        printf("Humi:%d %%\r\n", humidity);
        sprintf(temp_buf, "%2d", temperature);    // 将温度转换为字符串
        sprintf(humi_buf, "%2d", humidity);       // 将湿度转换为字符串
        OLED_ShowString(1,1,"Temp:");  
        OLED_ShowString(2,1,"Humi:");  
        OLED_ShowString(1, 5, temp_buf);          // 在OLED上显示温度
        OLED_ShowString(2, 5, humi_buf);          // 在OLED上显示湿度

        delay_ms(200);                            // 延时200ms
    }

    OLED_Clear(); // 清屏
}

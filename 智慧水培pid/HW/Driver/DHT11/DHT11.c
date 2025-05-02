#include "DHT11.h"  // 包含 DHT11 传感器的驱动文件
#include "Timer.h"
#include "Port.h"
#include "Debug.h"
uint32_t Time = 0; // 定义一个全局变量，用于记录时间
 
// 初始化 DHT11 传感器的 GPIO 引脚
void DHT11_Init()
{	 
	 if (DHT11_Check() == 0)
    {
        // 输出错误信息
			printf("DHT11 sensor initialization failed!\r\n");
        // 可以在此处添加其他错误处理或恢复措施
        return;  // 返回失败，跳过后续操作
		}
} 

float DHT11_Task(uint8_t *humidity, uint8_t *temperature)
{
    uint8_t HumH, HumL, TempH, TempL,humi;
    
    if (DHT11_Read_Huim_Temp(&HumH, &HumL, &TempH, &TempL)) {
        *humidity = HumH;         // 获取整数部分的湿度
        *temperature = TempH;      // 获取整数部分的温度 
//				humi= HumH;
			printf("humidity=%d\r\n",HumH);
        return HumH;                  // 读取成功
    }
    
    return HumH;  // 读取失败
}

// 设置 DHT11 引脚为输入或输出模式
void DHT11_Read_Out_Intput(uint8_t cmd)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0}; // 创建 GPIO 初始化结构体，并将其清零
	if(cmd) // 如果 cmd 为真，设置为输出模式
	{
		GPIO_InitStruct.Pin = DHT11; // 选择 DHT11 引脚
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 配置为推挽输出模式
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 配置为低速
		HAL_GPIO_Init(GPIO_DHT11, &GPIO_InitStruct); // 初始化 GPIO 引脚
	}
	else // 如果 cmd 为假，设置为输入模式
	{
		GPIO_InitStruct.Pin = DHT11; // 选择 DHT11 引脚
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // 配置为输入模式
		GPIO_InitStruct.Pull = GPIO_NOPULL; // 不使用上拉或下拉电阻
		HAL_GPIO_Init(GPIO_DHT11, &GPIO_InitStruct); // 初始化 GPIO 引脚
	}
}
 
// 读取 DHT11 传感器中的一个字节数据
uint8_t DHT11_Read_Byte(void)
{
	uint8_t Data = 0; // 初始化数据变量为 0
	
	for(uint8_t i=0; i<8; i++) // 读取 8 位数据
	{
		DHT11_Read_Out_Intput(0); // 设置为输入模式
		while((DHT11_Input == RESET) && (++Time < 1000)); // 等待引脚电平变为高，并计时
		Time = 0; // 重置计时器
		
		Data <<= 1; // 左移 1 位，为下一位数据做准备
		delay_us(40); // 延迟 40 微秒
		if(DHT11_Input == SET) // 如果读取到的数据位为 1
		{
			Data |= 0x01; // 将数据变量的最低位设置为 1
			while((DHT11_Input == SET) && (++Time < 1000)); // 等待引脚电平变为低，并计时
		}
	}
	return Data; // 返回读取到的字节数据
}
 
// 读取 DHT11 传感器的湿度和温度数据
uint8_t DHT11_Read_Huim_Temp(uint8_t *HuimH , uint8_t *HuimL , uint8_t *TempH , uint8_t *TempL)
{
	uint8_t Data[5]; // 定义一个数据数组，用于存储读取到的 5 字节数据
	
	DHT11_Read_Out_Intput(1); // 设置为输出模式
	DHT11_Low; // 设置引脚电平为低，开始数据传输
	HAL_Delay(20); // 延迟 20 毫秒
	DHT11_High; // 设置引脚电平为高，结束初始化
	delay_us(40); // 延迟 40 微秒
	
	DHT11_Read_Out_Intput(0); // 设置为输入模式
	while((DHT11_Input == RESET) && (++Time < 1000)); // 等待引脚电平变为高，并计时
	Time = 0; // 重置计时器
	while((DHT11_Input == SET) && (++Time < 1000)); // 等待引脚电平变为低，并计时
	Time = 0; // 重置计时器
	
	for(uint8_t i=0; i<5; i++) // 读取 5 字节数据
	{
		Data[i]=DHT11_Read_Byte(); // 调用读取字节函数
	}
	HAL_Delay(1); // 延迟 1 毫秒
	if(Data[0] + Data[1] + Data[2] + Data[3] == Data[4]) // 校验数据的校验和
	{
		*HuimH = Data[0]; // 将湿度高字节赋值给 HuimH
		*HuimL = Data[1]; // 将湿度低字节赋值给 HuimL
		*TempH = Data[2]; // 将温度高字节赋值给 TempH
		*TempL = Data[3]; // 将温度低字节赋值给 TempL
	}
	else 
	{
		return 0; // 校验和错误，返回 0
	}
	return 1; // 数据读取成功，返回 1
}
uint8_t DHT11_Check(void)
{
    uint8_t retry = 0;
    // 尝试读取 DHT11 数据，最大重试次数为3
    while(retry < 3)
    {
        // 发送初始化信号
        DHT11_Read_Out_Intput(1); // 设置为输出模式
        DHT11_Low;  // 设置引脚电平为低，开始数据传输
        HAL_Delay(20);  // 延迟 20 毫秒
        DHT11_High;  // 设置引脚电平为高，结束初始化
        delay_us(40); // 延迟 40 微秒

        // 等待 DHT11 响应并读取数据
        DHT11_Read_Out_Intput(0);  // 设置为输入模式
        if ((DHT11_Input == RESET) && (++Time < 1000)) {
            Time = 0;  // 重置计时器
            retry++;
            HAL_Delay(100);  // 延迟并重试
        } else {
            return 1;  // 检测到 DHT11 正常工作
        }
    }
    return 0;  // 检测失败
}

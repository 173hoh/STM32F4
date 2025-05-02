#include "bluetooth.h"

/**************************************************************************
函数功能：串口1初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void uart1_init(u32 bound)
{  	 
	  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能UGPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1时钟
	//USART1_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  //USART1_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
  USART_Init(USART1, &USART_InitStructure);     //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1
}

/**************************************************************************
函数功能：串口1接收中断
入口参数：无
返回  值：无
**************************************************************************/
u8 r=0,l=0;
void USART1_IRQHandler(void)
{	

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
	{	  
	 static	int uart_receive=0;//蓝牙接收相关变量
  	 uart_receive=USART_ReceiveData(USART1); 

	   switch (uart_receive)
		 {
			 case  '1':
				 pid.left = 35;pid.right = 35;         break;//向后
		   case  '2': 
				 pid.left = -35;pid.right = -35;      break;//向前
			 case  '3':
				  r= 0; l=1; 		break;//左转
			 case  '4':	
				 r= 1; l=0;    break;//右转		
			 case  '5': 
				 pid.left = 0;pid.right = 0;  r= 0; l=0;          break;//原地
			 default: LED0 = ~LED0;				//反转小灯
			 /*
			 小车  编码器 正值为--->向后运动
			              负值为--->向前运动         （以正看OLED为正方向）
						 
			 */
			
		 }
	}  											 
} 





/********************************************************************************************************

*++++++++++++++硬件连接 ++++++++++++++	*

*-------------------------------------* 
*STM32F103C8T6       RC522读卡模块		*		
*PA4（SPI1_NSS）    	SDA							*
*PA5（SPI1_SCK）			SCK							*
*PA6（SPI1_MISO）		  MISO						*
*PA7（SPI1_MOSI）		  MOSI						*
*PB12（RST）					RST							*
*PB13（IRQ）					IRQ							*
*-------------------------------------*

*************************************************************************/

///////////////////////////////////////////////////////////
/*头文件包含区*/
#include "Allinit.h"
#include "rc522.h"
#include "OLED.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
///////////////////////////////////////////////////////////
/*宏定义区*/

///////////////////////////////////////////////////////////
/*外部变量声明区*/
typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} Status;

char BOOK0ID[]="8E610302";		//管理员的adminCardID卡号
char BOOK1ID[]="F4DDE800";	//我的校园卡
char BOOK2ID[]="24CB0F05";	//我的校园卡
uint8_t Card_Type1[2];  
uint8_t Card_ID[4]; 
uint8_t Card_KEY[6] = {0xff,0xff,0xff,0xff,0xff,0xff};    //{0x11,0x11,0x11,0x11,0x11,0x11};   //密码
uint8_t Card_Data[16];
uint8_t status;										//读卡状态
char str[128];

/*函数声明区*/
void RFID_init(void);
void process_card(void);
///////////////////////////////////////////////////////////

/*
*********************************************************************************************************
* 函 数 名: main
* 功能说明: 主函数
* 形 参：无
* 返 回 值: 无
*********************************************************************************************************
*/

int main(void)
{
		Hardware_init();
		RFID_init();  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		printf("请刷卡...\r\n");
    while(1)
    {
				Hardware_read();
				Hardware_control(); 
        process_card();
			
    }
}

/*********************************************************************************************************
* 函 数 名: RFID_init 
* 功能说明: RFID初始化
* 形    参：无
* 返 回 值: 无
*********************************************************************************************************/
void RFID_init(void)
{
	RC522_IO_Init();	 //RC522端口初始化
	PcdReset();        //复位RC522
	PcdAntennaOff();   //关闭天线
	delay_ms(100);		 //等待天线关闭
	PcdAntennaOn();    //开启天线
	Card_Type1[0]=0x04;
	Card_Type1[1]=0x00;	
}
/*
*********************************************************************************************************
* 函 数 名: RC522_Authenticat0e
* 功能说明: RC522刷卡认证
* 形    参：无
* 返 回 值: FAILED/PASSED  1/0
*********************************************************************************************************
*/
void process_card(void)
{
    if (PcdRequest(0x52, Card_Type1) == MI_OK)
    {
        if (PcdAnticoll(Card_ID) == MI_OK)
        {
            char cardID[9];
            sprintf(cardID, "%02X%02X%02X%02X", Card_ID[0], Card_ID[1], Card_ID[2], Card_ID[3]);
            printf("检测到卡号：%s\r\n", cardID);

            // 检查是否已经刷过该卡（防止重复）
            if (strstr(str, cardID) != NULL)
            {
                printf("该卡已刷过，忽略。\r\n");
                return;
            }

            // 根据卡号添加对应书名和作者
            if (strcmp(cardID, BOOK0ID) == 0)
            {
                strcat(str, "soul by James Clear,");
                printf("添加书籍：soul by James Clear\r\n");
            }
            else if (strcmp(cardID, BOOK1ID) == 0)
            {
                strcat(str, "Flipped by Wendelin Van Draanen,");
                printf("添加书籍：Flipped by Wendelin Van Draanen\r\n");
            }
            else if (strcmp(cardID, BOOK2ID) == 0)
            {
                strcat(str, "Little Women by Louisa May Alcott,");
                printf("添加书籍：Little Women by Louisa May Alcott\r\n");
            }
            else
            {
                strcat(str, "Unknown Book by Unknown Author,");
                printf("未知卡片，添加Unknown Book by Unknown Author\r\n");
            }

            printf("当前记录：%s\r\n", str);
            USART2_SendString(USART2, str);
        }
    }
}


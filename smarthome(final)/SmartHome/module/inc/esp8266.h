#ifndef __ESP8266_H
#define __ESP8266_H

#include "gd32f4xx.h"
#include "systick.h"
#include "string.h"
#include "stdio.h"

#define ESP8266_RX_RCU							RCU_GPIOA
#define ESP8266_RX_PORT							GPIOA
#define ESP8266_RX_PIN							GPIO_PIN_10
#define ESP8266_RX_GPIO_AF						GPIO_AF_7

#define ESP8266_TX_RCU							RCU_GPIOA
#define ESP8266_TX_PORT							GPIOA
#define ESP8266_TX_PIN							GPIO_PIN_9
#define ESP8266_TX_GPIO_AF						GPIO_AF_7

#define ESP8266_UART_RCU						RCU_USART0
#define ESP8266_USART							USART0
#define ESP8266_USART_BAUDRATE					115200
#define ESP8266_USART_IRQ						USART0_IRQn

#define ESP8266_MAX_RECV_LEN					512
#define ESP8266_MAX_SEND_LEN					512

#define Success									1U
#define Failure									0U

struct esp8266_send_data{
	float temperature;
	float humidity;
	uint32_t illumination;
	uint32_t pressure;
	uint16_t noise;
	uint16_t concentration;
	char alarm[16];
	char alarmType[32];
};
struct esp8266_receive_data{
	char color[16];
	char commandType[16];
	char device[32];
	char houseNumber[16];
	char mode[16];
	char keys[16];
};

extern struct esp8266_send_data esp8266_send;
extern struct esp8266_receive_data esp8266_receive;
extern char esp8266_rx_buffer1[ESP8266_MAX_RECV_LEN/4];
extern char esp8266_rx_buffer2[ESP8266_MAX_RECV_LEN];
extern char esp8266_tx_buffer[ESP8266_MAX_SEND_LEN];
extern char get_wifi_command[64];
extern char set_user_command[64];
extern char publish_topic[64];
extern char subscribe_topic[64];
extern char publish_topic_command[256];
extern char subscribe_topic_command[128];
extern char new_data_flag;
extern uint16_t rev_buffer1_len;
extern uint16_t rev_buffer2_len;
extern uint8_t rev_flag;

void esp8266_peripheral_init(void);
void esp8266_peripheral_deinit(void);
uint8_t send_command(char *command,char *response,uint32_t time_out,uint8_t petry);
void topic_init(void);
void esp8266_publish_data_update(void);
uint8_t esp8266_init(void);
void esp8266_clr_txbuf(void);
void esp8266_clear_old_data(void);
void esp8266_get_new_data(void);

////
void esp8266_uart_send_byte(uint8_t ch);
#endif

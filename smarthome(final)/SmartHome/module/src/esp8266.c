/**
 * @file    esp8266.c
 * @brief   ESP8266 WiFi模块
 *
 * @version 2024-08-10, V1.0, cjy, 厦门芯力量
 */


#include "esp8266.h"

//两个结构体，分别用于发送和接收数据
struct esp8266_send_data esp8266_send={.temperature = 0.0,.humidity=0,.illumination=0,.pressure=0,\
                                        .noise=0,.concentration=0,.alarm="false",.alarmType="火焰警报"};
struct esp8266_receive_data esp8266_receive;

/** @brief  从esp8266模块接收数据时用的缓存1的长度 */
uint16_t rev_buffer1_len = 0;
/** @brief  从esp8266模块接收数据时用的缓存2的长度 */
uint16_t rev_buffer2_len = 0;
/** @brief  接收到新的数据标志位，为1时标志着收到了新的数据 */
uint8_t rev_flag = 0;
/** @brief  接收模式标志位，值为1时接收发出命令的返回值，为0时，接收MQTT传输过来的数据 */
unsigned char rxbuffer_mod = 1;
/** @brief  从esp8266接收数据时用的缓存1 */
char esp8266_rx_buffer1[ESP8266_MAX_RECV_LEN/4];
/** @brief  从esp8266接收数据时用的缓存2 */
char esp8266_rx_buffer2[ESP8266_MAX_RECV_LEN];
/** @brief  从esp8266发送数据时用的缓存 */
char esp8266_tx_buffer[ESP8266_MAX_SEND_LEN];
/** @brief  小屋编号 */
char *smart_number = "01";
/** @brief  esp8266连接WIFI的ssid */
char *ssid = "ZWH";//205
/** @brief  esp8266连接WIFI的密码 */
char *password = "1500208912";//88889999
/** @brief  连接到mqtt服务器使用的用户名，是可以修改 */
char *user = "mqtt_xmxll";
/** @brief  用于存放获取wifi的指令 */
char get_wifi_command[64];
/** @brief  用于存放接入mqtt用户设置的指令 */
char set_user_command[64];
/** @brief  用于存放mqtt发布的话题 */
char publish_topic[64];
/** @brief  用于存放mqtt订阅的话题 */
char subscribe_topic[64];
/** @brief  用于存放mqtt发布话题的指令 */
char publish_topic_command[256];
/** @brief  用于存放mqtt订阅话题的指令 */
char subscribe_topic_command[128];
/** @brief  订阅到新数据的标志位，为1是代表有新数据 */
char new_data_flag = 0;


/**
  * @brief  ESP8266使用的IO引脚初始化
  * @param  无
  * @retval 无
  */
void esp8266_gpio_init(void)
{
    rcu_periph_clock_enable(ESP8266_RX_RCU);

    gpio_mode_set(ESP8266_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, ESP8266_RX_PIN);
    gpio_output_options_set(ESP8266_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,ESP8266_RX_PIN);
    gpio_af_set(ESP8266_RX_PORT, ESP8266_RX_GPIO_AF, ESP8266_RX_PIN);

    rcu_periph_clock_enable(ESP8266_TX_RCU);

    gpio_mode_set(ESP8266_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, ESP8266_TX_PIN);
    gpio_output_options_set(ESP8266_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,ESP8266_TX_PIN);
    gpio_af_set(ESP8266_TX_PORT, ESP8266_TX_GPIO_AF, ESP8266_TX_PIN);
}

/**
  * @brief  ESP8266使用的串口初始化
  * @param  无
  * @retval 无
  */
void esp8266_uart_init(void)
{
    rcu_periph_clock_enable(ESP8266_UART_RCU);

    usart_deinit(ESP8266_USART);
    usart_baudrate_set(ESP8266_USART, ESP8266_USART_BAUDRATE);
    usart_parity_config(ESP8266_USART, USART_PM_NONE);
    usart_word_length_set(ESP8266_USART, USART_WL_8BIT);
    usart_stop_bit_set(ESP8266_USART, USART_STB_1BIT);

    usart_enable(ESP8266_USART);
    usart_transmit_config(ESP8266_USART, USART_TRANSMIT_ENABLE);
    usart_receive_config(ESP8266_USART, USART_RECEIVE_ENABLE);

    nvic_irq_enable(USART0_IRQn, 1, 2);
    usart_interrupt_enable(ESP8266_USART, USART_INT_RBNE);

}

/**
  * @brief  ESP8266使用的外设初始化
  * @param  无
  * @retval 无
  */
void esp8266_peripheral_init(void)
{
    esp8266_gpio_init();
    esp8266_uart_init();
}


/**
 * @brief   ESP8266 解除初始化
 *
 */
void esp8266_peripheral_deinit(void)
{
    nvic_irq_disable(USART0_IRQn);
    usart_disable(ESP8266_USART);
    usart_deinit(ESP8266_USART);
    rcu_periph_clock_disable(ESP8266_UART_RCU);
    rcu_periph_clock_disable(ESP8266_TX_RCU);
    rcu_periph_clock_disable(ESP8266_RX_RCU);
}

/**
  * @brief  使用串口发送一个byte到esp8266
  * @param  无
  * @retval 无
  */
void esp8266_uart_send_byte(uint8_t ch)
{
    usart_data_transmit( ESP8266_USART ,(uint8_t)ch);
    while(RESET == usart_flag_get(ESP8266_USART, USART_FLAG_TBE));
}

/**
  * @brief  使用串口发送一组数据到esp8266
  * @param  data:数组的地址
  * @param  size:数组的大小
  * @retval 无
  */
void esp8266_uart_transmit(char *data,uint16_t size)
{
    do{
        esp8266_uart_send_byte(*data++);
        size--;
    }while(size != 0);
}

/**
  * @brief  清空接收数据缓存区1
  * @param  无
  * @retval 无
  */
void esp8266_clr_rxbuf(void)
{
    memset(esp8266_rx_buffer1,0,sizeof(esp8266_rx_buffer1));
    rev_buffer1_len = 0;
}

/**
  * @brief  清空发送数据缓存区
  * @param  无
  * @retval 无
  */
void esp8266_clr_txbuf(void)
{
    memset(esp8266_tx_buffer,0,ESP8266_MAX_SEND_LEN);
}

/**
  * @brief  向ESP8266发送指令
  * @param  command：指令的首地址
  * @param  response：ESP8266返回的数据中要出现的字符串
  * @param  time_out:等待超时的时间，单位为毫秒
  * @param  petry:重试次数
  * @retval 返回值为Success代表指令发送成功，为Failure代表指令发布失败
  */
uint8_t send_command(char *command,char *response,uint32_t time_out,uint8_t petry)
{
    uint8_t n;
    uint32_t time_cont=0;
    rxbuffer_mod = 1;//表明开始进行数据接收
    esp8266_clr_rxbuf();//清空 ESP8266 的接收缓冲区
    for(n=0;n<petry;n++){
        esp8266_uart_transmit((char *)command,strlen(command));
//        printf("\r\n***************send****************\r\n");
//        printf("%s\r\n",command);

        while(time_cont<time_out){
            delay_1ms(100);
            time_cont+=100;
            if(strstr((char *)esp8266_rx_buffer1,(char *)response) != NULL ){
//                printf("\r\n***************receive****************\r\n");
//                printf("%s\r\n",esp8266_rx_buffer1);
                rxbuffer_mod = 0;
                esp8266_clr_rxbuf();
                return Success;
            }
        }
    }
//    printf("\r\n***************receive err****************\r\n");
//    printf("%s\r\n",esp8266_rx_buffer1);
    esp8266_clr_rxbuf();
    rxbuffer_mod = 0;
    return Failure;
}

/**
  * @brief  订阅和发布的话题初始化
  * @param  无
  * @retval 无
  */
void topic_init(void)
{
    sprintf(publish_topic,"smarthouse_remote_subscribe*%s",smart_number);
    sprintf(subscribe_topic,"smarthouse_subscribe*%s",smart_number);

    sprintf(subscribe_topic_command,"AT+MQTTSUB=0,\"%s\",1\r\n",subscribe_topic);
}

/**
  * @brief  更新需要发布的数据
  * @param  无
  * @retval 无
  */
void esp8266_publish_data_update(void)
{
    memset(publish_topic_command,0,256);
    sprintf(publish_topic_command,"{\\\"temperature\\\":\\\"%.2f\\\"\\,\\\"humidity\\\":\\\"%.2f\\\"\\,\\\"illumination\\\":\\\"%d\\\"\\,\\\"pressure\\\":\\\"%d\\\"\\,\\\"noise\\\":\\\"%d\\\"\\,\\\"concentration\\\":\\\"%d\\\"\\,\\\"alarm\\\":\\\"%s\\\"\\,\\\"alarmType\\\":\\\"%s\\\"}",\
                    esp8266_send.temperature,esp8266_send.humidity,esp8266_send.illumination,esp8266_send.pressure,esp8266_send.noise,esp8266_send.concentration,esp8266_send.alarm,esp8266_send.alarmType);

}

/**
  * @brief  初始化wifi信息
  * @param  无
  * @retval 无
  */
void wifi_info_init(void)
{
    sprintf(get_wifi_command,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
}

/**
  * @brief  初始化mqtt用户信息
  * @param  无
  * @retval 无
  */
void mqtt_user_init(void)
{
    sprintf(set_user_command,"AT+MQTTUSERCFG=0,1,\"%s\",\"mqtt:root\",\"xmxllAdmin2020.\",0,0,\"\"\r\n",user);
}


//使用这个初始化，因为esp01s上电启动运行固件，所以调用前要注意esp01s上电时间到这个函数被调用的时间.
//如果在main函数始初初始化，最好延时几秒等esp01s模块上电初始化完成后再调用，测试的时候延时5s可以用。
/**
  * @brief  初始化esp8266连接MQTT服务器，并订阅话题
  * @param  无
  * @retval 初始化成功返回Success，失败返回Failure
  */
uint8_t esp8266_init(void)
{
    uint8_t err_flag = 0;
    wifi_info_init();
    topic_init();
    mqtt_user_init();
    if(send_command("AT\r\n","OK\r\n",1000,2) == Success){
    }else{
        err_flag = 0;
        goto err;
    }
    if(send_command("ATE0\r\n","OK\r\n",1000,2) == Success){
    }else{
        err_flag = 1;
        goto err;
    }
    if(send_command("AT+CWMODE=1\r\n","OK\r\n",1000,2) == Success){
    }else{
        err_flag = 2;
        goto err;
    }
    if(send_command(get_wifi_command,"OK\r\n",10000,2) == Success){
    }else{
        err_flag = 3;
        goto err;
    }
    if(send_command(set_user_command,"OK\r\n",1000,2) == Success){
    }else{
        err_flag = 4;
        goto err;
    }
    if(send_command("AT+MQTTCONN=0,\"47.99.144.16\",1883,0\r\n","OK\r\n",1000,2) == Success){
    }else{
        err_flag = 5;
        goto err;
    }
    if(send_command(subscribe_topic_command,"OK\r\n",1000,2) == Success){
    }else{
        err_flag = 6;
        goto err;
    }
    return Success;

err:

    printf("esp8266 init err code:%d",err_flag);
    return Failure;

}

/**
  * @brief  串口0中断，用于接收数据
  * @param  无
  * @retval 无
  */
void USART0_IRQHandler(void)
{
    if(usart_interrupt_flag_get(ESP8266_USART, USART_INT_FLAG_RBNE) == SET){
        usart_interrupt_flag_clear(ESP8266_USART, USART_INT_FLAG_RBNE);
        if(rxbuffer_mod == 1){
            if (rev_buffer1_len < (ESP8266_MAX_RECV_LEN/4)){
                esp8266_rx_buffer1[rev_buffer1_len++] = usart_data_receive(ESP8266_USART);
            }
        }else if(rxbuffer_mod == 0){
            if (rev_buffer2_len < ESP8266_MAX_RECV_LEN){
                esp8266_rx_buffer2[rev_buffer2_len++] = usart_data_receive(ESP8266_USART);
                if(strstr(esp8266_rx_buffer2,"}\r\n") != NULL){
                    //秉承中断快进快出原则，尽量不要在中断内处理大量数据
                    new_data_flag = 1;
                }
            }
        }
    }
}




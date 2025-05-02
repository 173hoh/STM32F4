/**
 * @file    board_uart.c
 * @brief   板载串口 (UART3) 的驱动
 *
 * @version 2024-07-23, V1.0, yanf, 厦门芯力量
 */

#include "gd32f4xx.h"
#include "board_uart.h"
#include <stdio.h>


/** @brief   收到数据后的回调函数 */
uart_onboard_recv_callback ob_recv_handler;

/** @brief  串口数据接收缓存 (ob 前缀 = onboard) */
uint8_t     ob_recv_buffer[UART_ONBOARD_RECEIVE_BUFFER_LENGTH];

/** @brief  串口已接收到的数据长度 */
uint16_t    ob_recv_count = 0;


/**
 * @brief   板载串口初始化配置
 *
 * @param   band_rate: 串口通讯波特率
 * @param    callback: 收到数据时的回调函数
 */
void uart_onboard_init(uint32_t band_rate, uart_onboard_recv_callback callback)
{
		//启用 UART3 和 GPIOA 的时钟，以使能 UART3 和 GPIOA 相关的操作
    rcu_periph_clock_enable(RCU_UART3);
    rcu_periph_clock_enable(RCU_GPIOA);
		//将 GPIOA 的 0 号引脚和 1 号引脚配置为 UART3 的 TX（传输）和 RX（接收）引脚，设置为复用功能
		gpio_af_set(GPIOA, GPIO_AF_8, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);
		//复位 UART3 并设置波特率为传入的参数 band_rate
    usart_deinit(UART3);                                        // 复位
    usart_baudrate_set(UART3, band_rate);                       // 设置波特率 (以下无设置采用默认值：8位数据、1位停止、无校验位)
		//启用 UART3，并使能传输和接收功能
    usart_enable(UART3);
    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
    usart_receive_config(UART3, USART_RECEIVE_ENABLE);

    if (callback) {
        ob_recv_handler = callback;
	  //启用 UART3 的中断，配置中断优先级，并使能接收缓冲区非空中断（USART_INT_RBNE）和空闲中断（USART_INT_IDLE）
        nvic_irq_enable(UART3_IRQn, 2, 2);                      // 配置中断优先级
        usart_interrupt_enable(UART3, USART_INT_RBNE);          // 读数据缓冲区非空中断和溢出错误中断
        usart_interrupt_enable(UART3, USART_INT_IDLE);          // 空闲检测中断
    }
}


/**
 * @brief   板载串口解除初始化及停止中断
 *
 */
//停止 UART3 的工作，关闭中断，并关闭 UART3 和 GPIOA 的时钟
void uart_onboard_deinit(void)
{
    nvic_irq_disable(UART3_IRQn);
    usart_disable(UART3);
    rcu_periph_clock_disable(RCU_UART3);
    rcu_periph_clock_disable(RCU_GPIOA);
}


/**
 * @brief   板载串口发送一个字节数据
 *
 * @param   byte: 一个字节
 */
//（发送缓冲区为空标志）置位，表示数据已成功发送
void uart_onboard_send_byte(uint8_t byte)
{
    usart_data_transmit(UART3, byte);

    while (RESET == usart_flag_get(UART3, USART_FLAG_TBE));
}


/**
 * @brief   板载串口发送字节数组
 *
 * @param   bytes: 字节数组
 * @param     len: 数组长度
 */
//遍历数组，每次调用 uart_onboard_send_byte 函数发送一个字节，直到所有字节发送完成
void uart_onboard_send_bytes(uint8_t *bytes, uint8_t len)
{
    while (len--) {
        uart_onboard_send_byte(*bytes++);
    }
}


/**
 * @brief   重置接收缓存和接收计数
 *
 */
//遍历接收缓冲区，将其内容置为 0，并将接收字节计数 ob_recv_count 重置为 0
void uart_onboard_receive_clear(void)
{
    for(uint16_t i = 0; i < ob_recv_count; i++) {
        ob_recv_buffer[i] = 0;
    }

    ob_recv_count = 0;
}


/**
 * @brief   板载串口(UART3)接收中断，接收到的数据交给回调函数处理
 */
void UART3_IRQHandler(void)
{
		//如果接收缓冲区非空（USART_INT_FLAG_RBNE）标志置位，调用 usart_data_receive 函数从 UART3 接收寄存器读取一个字节的数据
    if (usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE) == SET) {
        uint16_t abyte = usart_data_receive(UART3);
		//如果接收的字节数未超过缓冲区大小，则将接收到的字节存入接收缓冲区，并更新接收计数器
        if (ob_recv_count < UART_ONBOARD_RECEIVE_BUFFER_LENGTH) {
            ob_recv_buffer[ob_recv_count++] = abyte;
        }
    } else if (usart_interrupt_flag_get(UART3, USART_INT_FLAG_IDLE) == SET) {
        usart_data_receive(UART3);

        if (ob_recv_count > 0) {
            ob_recv_handler(ob_recv_buffer, ob_recv_count);
        }
    }
}

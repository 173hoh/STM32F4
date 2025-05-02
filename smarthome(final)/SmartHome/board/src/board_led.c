/**
 * @file    board_led.c
 * @brief   板载 LED 的驱动
 *
 * @version 2024-07-23, V1.0, yanf, 厦门芯力量
 */


#include "gd32f4xx.h"
#include "board_led.h"


/**
 * @brief   板载 LED GPIO 初始化
 *
 * @param   ledx: led_num 枚举值，使用 LED_ALL 可一次初始化全部
 */
void led_init(led_num ledx)
{
    if ((ledx & LED1) == LED1) {
				//使能对应 LED 的时钟 
        rcu_periph_clock_enable(LED1_RCU);
				//设置对应 GPIO 的模式为输出模式
        gpio_mode_set(LED1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_PIN);
				//设置输出类型为推挽输出，速度为 50MHz
        gpio_output_options_set(LED1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED1_PIN);
				//将该 LED 对应的 GPIO 引脚置低，熄灭 LED
        gpio_bit_reset(LED1_PORT, LED1_PIN);
    }

    if ((ledx & LED2) == LED2) {
        rcu_periph_clock_enable(LED2_RCU);

        gpio_mode_set(LED2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED2_PIN);
        gpio_output_options_set(LED2_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED2_PIN);

        gpio_bit_reset(LED2_PORT, LED2_PIN);
    }

    if ((ledx & LED3) == LED3) {
        rcu_periph_clock_enable(LED3_RCU);

        gpio_mode_set(LED3_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED3_PIN);
        gpio_output_options_set(LED3_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED3_PIN);

        gpio_bit_reset(LED3_PORT, LED3_PIN);
    }
}


/**
 * @brief   板载 LED GPIO 解除初始化
 *
 * @param   ledx: led_num 枚举值
 */
void led_deinit(led_num ledx)
{
    if ((ledx & LED1) == LED1) {
				//将对应的 LED 引脚置低 
        gpio_bit_reset(LED1_PORT, LED1_PIN);
				//关闭 LED 的时钟 
        rcu_periph_clock_disable(LED1_RCU);
    }

    if ((ledx & LED2) == LED2) {
        gpio_bit_reset(LED2_PORT, LED2_PIN);
        rcu_periph_clock_disable(LED2_RCU);
    }

    if ((ledx & LED3) == LED3) {
        gpio_bit_reset(LED3_PORT, LED3_PIN);
        rcu_periph_clock_disable(LED3_RCU);
    }
}


/**
 * @brief   点亮 LED
 *
 * @param   ledx: led_num 枚举值
 */
void led_on(led_num ledx)
{
    if ((ledx & LED1) == LED1) {
				//将对应 LED 的 GPIO 引脚置高 
        gpio_bit_set(LED1_PORT, LED1_PIN);
    }

    if ((ledx & LED2) == LED2) {
        gpio_bit_set(LED2_PORT, LED2_PIN);
    }

    if ((ledx & LED3) == LED3) {
        gpio_bit_set(LED3_PORT, LED3_PIN);
    }
}


/**
 * @brief   熄灭 LED
 *
 * @param   ledx: led_num 枚举值
 */
void led_off(led_num ledx)
{
    if ((ledx & LED1) == LED1) {
				//将对应 LED 的 GPIO 引脚置低
        gpio_bit_reset(LED1_PORT, LED1_PIN);
    }

    if ((ledx & LED2) == LED2) {
        gpio_bit_reset(LED2_PORT, LED2_PIN);
    }

    if ((ledx & LED3) == LED3) {
        gpio_bit_reset(LED3_PORT, LED3_PIN);
    }
}


/**
 * @brief   切换 LED 的亮灭
 *
 * @param   ledx: led_num 枚举值
 */
void led_toggle(led_num ledx)
{
    if ((ledx & LED1) == LED1) {
				//切换 LED 的状态
        gpio_bit_toggle(LED1_PORT, LED1_PIN);
    }

    if ((ledx & LED2) == LED2) {
        gpio_bit_toggle(LED2_PORT, LED2_PIN);
    }

    if ((ledx & LED3) == LED3) {
        gpio_bit_toggle(LED3_PORT, LED3_PIN);
    }
}

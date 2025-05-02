/**
 * @file    step2p4w.c
 * @brief   两相四线步进电机四拍驱动
 *
 * @remark  此驱动适用于比赛专用套件 V1.6 及之前版本的步进电机，V1.7 开始使用 TC1508 驱动芯片
 *
 * @version 2024-08-07, V1.0, yanf, 厦门芯力量
 */

#include "gd32f4xx.h"
#include "step2p4w.h"
#include "bsp_basic_timer.h"
#include "stdio.h"


/** @brief   存储电机步进总的节拍数 */
static uint32_t     step24_beats;

/** @brief   存储电机转动的方向 */
static uint8_t      step24_direction;


/**
  * @brief   步进电机按拍数旋转，拍数为 0 ~ 3 或 3 ~ 0 之间循环
  *
  * @param   dir: 方向：1：顺时针，0：逆时针
  * @param  step: 拍数：0 ~ 3 之间
  */
void stepper24_step4_action(uint8_t dir, uint8_t step) {
    step = (dir == 0) ? (3 - step) : step;

    switch (step) {
        case 0:
            gpio_bit_set(STEP24_PORT, STEP24_PIN_A);
            gpio_bit_reset(STEP24_PORT, STEP24_PIN_B);
            break;
        case 1:
            gpio_bit_reset(STEP24_PORT, STEP24_PIN_A);
            gpio_bit_reset(STEP24_PORT, STEP24_PIN_B);
            break;
        case 2:
            gpio_bit_reset(STEP24_PORT, STEP24_PIN_A);
            gpio_bit_set(STEP24_PORT, STEP24_PIN_B);
            break;
        case 3:
            gpio_bit_set(STEP24_PORT, STEP24_PIN_A);
            gpio_bit_set(STEP24_PORT, STEP24_PIN_B);
            break;
    }
}


/**
  * @brief   步进电机按整数角度旋转
  *
  * @remark  此款步进电机的步距角为 18 度，即 20 个节拍后转 360 度
  *
  * @param   direction: 转动方向：1：顺时针(正转)，0：逆时针(反转)
  * @param       angle: 电机转轴旋转角度
  */
void stepper24_angle(uint8_t direction, uint32_t angle)
{
	bsp_timer6_disable();

	step24_direction = direction;
	step24_beats = (angle * 20) / 360;

    gpio_bit_set(STEP24_PORT, STEP24_ENABLE);
	bsp_timer6_enable();
}


/**
  * @brief   定时器更新中断刷新步进电机的节拍
  *
  */
void stepper24_timer6_update()
{
    if (step24_beats != 0) {
        stepper24_step4_action(step24_direction, (3 - step24_beats-- % 4));
    } else {
        stepper24_stop();
    }
}


/**
  * @brief   两相四线步进电机引脚配置及初始化
  *
  */
void stepper_2_phase_4_wire_init(void)
{
	bsp_timer6_init(100, stepper24_timer6_update);

    rcu_periph_clock_enable(STEP24_RCU);

    gpio_mode_set(STEP24_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, STEP24_PIN_A | STEP24_PIN_B | STEP24_ENABLE);
    gpio_output_options_set(STEP24_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, STEP24_PIN_A | STEP24_PIN_B | STEP24_ENABLE);

    gpio_bit_reset(STEP24_PORT, STEP24_PIN_A | STEP24_PIN_B);
}


/**
 * @brief   解除初始化
 *
 */
void stepper24_deinit(void)
{
    rcu_periph_clock_disable(STEP24_RCU);
    bsp_timer6_deinit();
}


/**
  * @brief   停止步进电机转动
  *
  */
void stepper24_stop(void)
{
    bsp_timer6_disable();

    step24_beats = 0;
	gpio_bit_reset(STEP24_PORT, STEP24_PIN_A | STEP24_PIN_B);
    gpio_bit_reset(STEP24_PORT, STEP24_ENABLE);
}

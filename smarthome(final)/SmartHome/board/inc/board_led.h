#ifndef __BOARD_LED_H
#define __BOARD_LED_H

#define LED3_RCU            RCU_GPIOF
#define LED3_PORT           GPIOF
#define LED3_PIN            GPIO_PIN_6

#define LED2_RCU            RCU_GPIOA
#define LED2_PORT           GPIOA
#define LED2_PIN            GPIO_PIN_4

#define LED1_RCU            RCU_GPIOC
#define LED1_PORT           GPIOC
#define LED1_PIN            GPIO_PIN_10


typedef enum {
    LED1        = 1 << 0,
    LED2        = 1 << 1,
    LED3        = 1 << 2,
    LED_ALL     = 0x0F
} led_num;


void led_init(led_num ledx);
void led_deinit(led_num ledx);
void led_on(led_num ledx);
void led_off(led_num ledx);
void led_toggle(led_num ledx);

#endif

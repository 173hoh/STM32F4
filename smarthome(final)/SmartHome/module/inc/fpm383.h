#ifndef __FMP383C_H
#define __FMP383C_H


/**  @brief 应答包格式 */
typedef struct {
    uint32_t password;
    uint8_t cmd1;
    uint8_t cmd2;
    uint32_t errcode;
    uint8_t* data;
    uint16_t data_len;
} fp_ack_struct;


void fingerprint_init(void);
void fingerprint_deinit(void);

void fp_led_touch(uint8_t color);
void fp_led_switch(uint8_t on_off, uint8_t color);
void fp_led_blink(uint8_t color, uint8_t on_10ms, uint8_t off_10ms, uint8_t period);
void fp_led_breathe(uint8_t color, uint8_t max_duty, uint8_t min_duty, uint8_t frequency);

void fp_sleep(uint8_t mode);

void fp_enroll(uint8_t reg_idx);
uint8_t fp_enroll_check(void);
uint8_t fp_finger_state(void);

void fp_receive_clear(void);

#endif



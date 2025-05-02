#ifndef __STEPPER_2_PHASE_4_WIRE_H
#define __STEPPER_2_PHASE_4_WIRE_H

#define STEP24_RCU          RCU_GPIOF
#define STEP24_PORT         GPIOF
#define STEP24_PIN_A        GPIO_PIN_4
#define STEP24_PIN_B        GPIO_PIN_2
#define STEP24_ENABLE       GPIO_PIN_3


void stepper_2_phase_4_wire_init(void);
void stepper24_deinit(void);
void stepper24_stop(void);
void stepper24_angle(uint8_t direction, uint32_t angle);

#endif

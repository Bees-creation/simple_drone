/**
 * @date 2026/9/19
 * @note Providing API for generating complementary output pwm signal.
 * @copyright https://github.com/Bees-creation (c) 2026
 */

#ifndef _PWM_GENERATOR_H
#define _PWM_GENERATOR_H

#include "stm32f0xx_ll_tim.h"

void pwm_set(uint16_t pulse);
void pwm_start(void);

#endif

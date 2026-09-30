/**
 * @date 2026/9/29
 * @note Providing API for phase changing.
 * @copyright https://github.com/Bees-creation (c) 2026
 */

#ifndef _PHASE_H
#define _PHASE_H

#include "stm32f0xx_ll_tim.h"

void timer_interrupt_start(void);
void motor_sync(void);

void Phase_Openloop_Callback(void);

#endif

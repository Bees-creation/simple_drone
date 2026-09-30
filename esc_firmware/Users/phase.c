/**
 * @date 2026/9/29
 * @note Providing API for phase changing.
 * @copyright https://github.com/Bees-creation (c) 2026
 */

#include "phase.h"

// 启动参数
#define OPENLOOP_DUTY      (80)           // 8%
#define OPENLOOP_INTERVAL  (200 * 1000)   // 200  ms
#define STARTUP_INTERVAL   (1000 * 1000)  // 1000 ms

// 运行参数
uint32_t current_duty = OPENLOOP_DUTY;
uint32_t commutation_interval = OPENLOOP_INTERVAL;

// 换相步数
volatile uint8_t current_step = 0;

// 换相顺序
static uint8_t commutation_sequence[6][3] =
{
//  0 = 低电平
//  1 = 高电平
//  2 = 悬空
//  { A, B, C }
    { 1, 0, 2 },
    { 1, 2, 0 },
    { 2, 1, 0 },
    { 0, 1, 2 },
    { 0, 2, 1 },
    { 2, 0, 1 },
};

void Set_Phase_State(uint8_t phase, uint8_t state, uint32_t duty)
{
    uint32_t ch_main, ch_comp;

    switch (phase) {
        case 0:  // A相
            ch_main = LL_TIM_CHANNEL_CH1;
            ch_comp = LL_TIM_CHANNEL_CH1N;
            break;
        case 1:  // B相
            ch_main = LL_TIM_CHANNEL_CH2;
            ch_comp = LL_TIM_CHANNEL_CH2N;
            break;
        case 2:  // C相
            ch_main = LL_TIM_CHANNEL_CH3;
            ch_comp = LL_TIM_CHANNEL_CH3N;
            break;
        default:
            return;
    }

    switch (state) {
        case 1:  // PWM 相，上桥 PWM，下桥关断
            switch (phase) {
                case 0: LL_TIM_OC_SetCompareCH1(TIM1, duty); break;
                case 1: LL_TIM_OC_SetCompareCH2(TIM1, duty); break;
                case 2: LL_TIM_OC_SetCompareCH3(TIM1, duty); break;
            }
            LL_TIM_CC_EnableChannel(TIM1, ch_main);
            LL_TIM_CC_DisableChannel(TIM1, ch_comp);
            break;

        case 0:  // 低电平相，上桥关断，下桥常通
            switch (phase) {
                case 0: LL_TIM_OC_SetCompareCH1(TIM1, 0); break;
                case 1: LL_TIM_OC_SetCompareCH2(TIM1, 0); break;
                case 2: LL_TIM_OC_SetCompareCH3(TIM1, 0); break;
            }
            LL_TIM_CC_EnableChannel(TIM1, ch_main | ch_comp);
            break;

        case 2:  // 悬空相，上下桥都关断
            LL_TIM_CC_DisableChannel(TIM1, ch_main | ch_comp);
            break;
    }
}

void Commutate_Step(void)
{
    uint8_t *step = commutation_sequence[current_step];
    Set_Phase_State(0, step[0], current_duty);
    Set_Phase_State(1, step[1], current_duty);
    Set_Phase_State(2, step[2], current_duty);

    LL_TIM_SetAutoReload(TIM14, commutation_interval);

    current_step = (current_step + 1) % 6;
}

void timer_interrupt_start(void)
{
    LL_TIM_EnableIT_UPDATE(TIM14);
    LL_TIM_EnableCounter(TIM14);
}

void motor_sync(void)
{
    uint8_t *step = commutation_sequence[current_step];
    Set_Phase_State(0, step[0], current_duty);
    Set_Phase_State(1, step[1], current_duty);
    Set_Phase_State(2, step[2], current_duty);

    for (volatile uint32_t i = 0; i < STARTUP_INTERVAL; i++);
}

void Phase_Openloop_Callback(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM14) && LL_TIM_IsEnabledIT_UPDATE(TIM14))
    {
        LL_TIM_ClearFlag_UPDATE(TIM14);
        Commutate_Step();
    }
}

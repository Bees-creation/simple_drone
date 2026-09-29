/**
 * @date 2026/9/19
 * @note Providing API for generating complementary output pwm signal.
 * @copyright https://github.com/Bees-creation (c) 2026
 */

#include "pwm_generator.h"

void pwm_set(uint16_t pulse)
{
    LL_TIM_OC_SetCompareCH1(TIM1, pulse);
    LL_TIM_OC_SetCompareCH2(TIM1, pulse);
    LL_TIM_OC_SetCompareCH3(TIM1, pulse);
}

void pwm_start()
{
    // 使能所有输出 (设置 BDTR 寄存器的 MOE 位)
    LL_TIM_EnableAllOutputs(TIM1);

    // 使能各个通道的输出
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1 | LL_TIM_CHANNEL_CH1N);
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2 | LL_TIM_CHANNEL_CH2N);
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3 | LL_TIM_CHANNEL_CH3N);

    // 启动计数器
    LL_TIM_EnableCounter(TIM1);
}

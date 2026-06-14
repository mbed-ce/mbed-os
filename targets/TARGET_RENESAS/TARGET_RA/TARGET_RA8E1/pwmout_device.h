#pragma once

#include "PeripheralNames.h"
#include "hal_data.h"

const timer_instance_t *pwm_timer_lookup(PWMName name)
{
    switch (name) {
        case PWM_GPT0: return &g_timer_gpt0;
        case PWM_GPT1: return &g_timer_gpt1;
        case PWM_GPT2: return &g_timer_gpt2;
        case PWM_GPT3: return &g_timer_gpt3;
        case PWM_GPT4: return &g_timer_gpt4;
        case PWM_GPT5: return &g_timer_gpt5;
        case PWM_GPT10: return &g_timer_gpt10;
        case PWM_GPT11: return &g_timer_gpt11;
        case PWM_GPT12: return &g_timer_gpt12;
        case PWM_GPT13: return &g_timer_gpt13;
        case PWM_AGT0: return &g_timer_agt0;
        case PWM_AGT1: return &g_timer_agt1;
        default:       return NULL;
    }
}

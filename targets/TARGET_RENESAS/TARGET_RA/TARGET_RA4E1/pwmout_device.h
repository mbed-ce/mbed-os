/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "PeripheralNames.h"
#include "hal_data.h"

const timer_instance_t *pwm_timer_lookup(PWMName name)
{
    switch (name) {
        case PWM_GPT1: return &g_timer_gpt1;
        case PWM_GPT2: return &g_timer_gpt2;
        case PWM_GPT4: return &g_timer_gpt4;
        case PWM_GPT5: return &g_timer_gpt5;
        case PWM_AGT0: return &g_timer_agt0;
        case PWM_AGT1: return &g_timer_agt1;
        case PWM_AGT2: return &g_timer_agt2;
        case PWM_AGT3: return &g_timer_agt3;
        case PWM_AGT5: return &g_timer_agt5;
        default:       return NULL;
    }
}

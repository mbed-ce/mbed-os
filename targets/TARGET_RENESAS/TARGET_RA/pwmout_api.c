/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "pwmout_api.h"
#include "mbed_assert.h"
#include "pinmap.h"
#include "bsp_api.h"
#include "PeripheralPins.h"
#include "pwmout_device.h"
#include <string.h>

#if MBED_CONF_TARGET_LSE_AVAILABLE
#define AGT_LOW_FREQ_CLOCK AGT_CLOCK_SUBCLOCK
#else
#define AGT_LOW_FREQ_CLOCK AGT_CLOCK_LOCO
#endif

void pwmout_init(pwmout_t *obj, PinName pin)
{
    MBED_ASSERT(obj);

    // peripheral = PWMName
    uint32_t periph = pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(periph != (uint32_t)NC);

    // function = pwm_output_t
    uint32_t func = pinmap_find_function(pin, PinMap_PWM);
    MBED_ASSERT(func != (uint32_t)NC);

    pin_function(pin, func);

    PWMName      timer_id = (PWMName) periph;
    pwm_output_t unit     = (pwm_output_t) RA_PIN_CHANNEL(func);

    obj->pin      = pin;
    obj->pwm_unit = unit;
    obj->p_timer  = pwm_timer_lookup(timer_id);
    obj->period_counts = 0;
    obj->duty_counts   = 0;
    obj->initialized   = 0;

    MBED_ASSERT(obj->p_timer != NULL);

    /* 1. Identify timer type and bit-width dynamically via FSP API pointer and p_ctrl */
    if (obj->p_timer->p_api == &g_timer_on_gpt) {
        obj->timer_type = PWM_TIMER_TYPE_GPT;
    } else if (obj->p_timer->p_api == &g_timer_on_agt) {
        obj->timer_type = PWM_TIMER_TYPE_AGT;
    } else {
        // Unsupported timer driver API
        MBED_ASSERT(0);
    }

    /* 2. Deep-copy FSP default configurations into the local object RAM */
    memcpy(&obj->cfg, obj->p_timer->p_cfg, sizeof(timer_cfg_t));
    if (obj->timer_type == PWM_TIMER_TYPE_GPT) {
        if (obj->p_timer->p_cfg->p_extend) {
            memcpy(&obj->ext_cfg.gpt_ext, obj->p_timer->p_cfg->p_extend, sizeof(gpt_extended_cfg_t));
        }
        obj->cfg.p_extend = &obj->ext_cfg.gpt_ext;
    } else {
        if (obj->p_timer->p_cfg->p_extend) {
            memcpy(&obj->ext_cfg.agt_ext, obj->p_timer->p_cfg->p_extend, sizeof(agt_extended_cfg_t));
        }
        obj->cfg.p_extend = &obj->ext_cfg.agt_ext;
    }

    /* 3. Open and Start the timer using local configuration */
    fsp_err_t err = obj->p_timer->p_api->open(obj->p_timer->p_ctrl, &obj->cfg);
    if (FSP_SUCCESS != err && FSP_ERR_ALREADY_OPEN != err) {
        MBED_ASSERT(0);
    }

    if(obj->timer_type == PWM_TIMER_TYPE_GPT) {
        // Cast p_ctrl to gpt_instance_ctrl_t to check bit-width variant
        gpt_instance_ctrl_t *p_gpt_ctrl = (gpt_instance_ctrl_t *) obj->p_timer->p_ctrl;
        if (p_gpt_ctrl->variant == TIMER_VARIANT_16_BIT) {
            obj->is_16bit = true;
        } else {
            obj->is_16bit = false;
        }
    }
    else {
        // Cast p_ctrl to agt_instance_ctrl_t to check if it's AGTW (32-bit)
        agt_instance_ctrl_t *p_agt_ctrl = (agt_instance_ctrl_t *) obj->p_timer->p_ctrl;
        if (p_agt_ctrl->is_agtw) {
            obj->is_16bit = false; // AGTW is 32-bit
        } else {
            obj->is_16bit = true;  // Regular AGT is 16-bit
        }
    }

    pwmout_period_us(obj, 1000);
    pwmout_write(obj, 0.0f);

    err = obj->p_timer->p_api->start(obj->p_timer->p_ctrl);
    if (FSP_SUCCESS != err) {
        MBED_ASSERT(0);
    }
    obj->initialized = 1;
}

void pwmout_free(pwmout_t *obj)
{
    if (!obj || !obj->initialized) {
        return;
    }

    obj->p_timer->p_api->stop(obj->p_timer->p_ctrl);
    obj->p_timer->p_api->close(obj->p_timer->p_ctrl);

    obj->initialized = 0;
}

void pwmout_period_us(pwmout_t *obj, int us)
{
    MBED_ASSERT(obj && obj->p_timer);

    if (us <= 0) {
        us = 1;
    }

    uint64_t max_count = obj->is_16bit ? 0xFFFFULL : 0xFFFFFFFFULL;

    timer_source_div_t best_div = TIMER_SOURCE_DIV_1;
    uint32_t period_counts = 0;
    bool needs_reopen = false;

    if (obj->timer_type == PWM_TIMER_TYPE_GPT) {
        // PCLKD Frequency (GPT Clock Source)
        uint32_t pclkd_freq = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD);

        // GPT Prescaler List: Div 1, 2, 4, 8, 16, 32, 64, 256, 1024
        static const timer_source_div_t gpt_divs[] = {
            TIMER_SOURCE_DIV_1,   TIMER_SOURCE_DIV_2,   TIMER_SOURCE_DIV_4,
            TIMER_SOURCE_DIV_8,   TIMER_SOURCE_DIV_16,  TIMER_SOURCE_DIV_32,
            TIMER_SOURCE_DIV_64,  TIMER_SOURCE_DIV_128, TIMER_SOURCE_DIV_256,
            TIMER_SOURCE_DIV_512, TIMER_SOURCE_DIV_1024
        };

        for (size_t i = 0; i < sizeof(gpt_divs)/sizeof(gpt_divs[0]); i++) {
            if(gpt_divs[i] == TIMER_SOURCE_DIV_128 || gpt_divs[i] == TIMER_SOURCE_DIV_512) {
                // Skip unsupported divisors
                continue;
            }
            uint32_t div_val = 1U << i;
            uint64_t clk_hz = (uint64_t)pclkd_freq / div_val;
            uint64_t counts = (clk_hz * (uint64_t)us) / 1000000ULL;

            if (counts <= max_count) {
                best_div = gpt_divs[i];
                period_counts = (counts == 0) ? 1 : (uint32_t)counts;
                break;
            }
        }

        if (obj->cfg.source_div != best_div) {
            obj->cfg.source_div = best_div;
            needs_reopen = true;
        }
    }
    else { // AGT Timer
        // Frequency mapping logic: Low frequency (< 500Hz, i.e., us >= 2000) uses LOCO/SUBCLOCK (32768Hz); High frequency uses PCLKB
        if (us >= 2000) {
            // Switch to LOCO/SUBCLOCK 32768Hz clock source
            if (obj->ext_cfg.agt_ext.count_source != AGT_LOW_FREQ_CLOCK) {
                obj->ext_cfg.agt_ext.count_source = AGT_LOW_FREQ_CLOCK;
                needs_reopen = true;
            }

            // AGT with LOCO/SUBCLOCK supports Div 1 to Div 128
            static const timer_source_div_t agt_loco_divs[] = {
                TIMER_SOURCE_DIV_1,  TIMER_SOURCE_DIV_2,  TIMER_SOURCE_DIV_4,
                TIMER_SOURCE_DIV_8,  TIMER_SOURCE_DIV_16, TIMER_SOURCE_DIV_32,
                TIMER_SOURCE_DIV_64, TIMER_SOURCE_DIV_128
            };

            for (size_t i = 0; i < sizeof(agt_loco_divs)/sizeof(agt_loco_divs[0]); i++) {
                uint32_t div_val = 1U << i;
                uint64_t clk_hz = 32768ULL / div_val;
                uint64_t counts = (clk_hz * (uint64_t)us) / 1000000ULL;

                if (counts <= max_count) {
                    best_div = agt_loco_divs[i];
                    period_counts = (counts == 0) ? 1 : (uint32_t)counts;
                    break;
                }
            }
        } else {
            // Use PCLKB high-frequency clock source
            if (obj->ext_cfg.agt_ext.count_source != AGT_CLOCK_PCLKB) {
                obj->ext_cfg.agt_ext.count_source = AGT_CLOCK_PCLKB;
                needs_reopen = true;
            }

            uint32_t pclkb_freq = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);
            // AGT with PCLKB supports Div 1, 2, 4, 8
            static const timer_source_div_t agt_pclkb_divs[] = {
                TIMER_SOURCE_DIV_1, TIMER_SOURCE_DIV_2, TIMER_SOURCE_DIV_4, TIMER_SOURCE_DIV_8
            };

            for (size_t i = 0; i < sizeof(agt_pclkb_divs)/sizeof(agt_pclkb_divs[0]); i++) {
                uint32_t div_val = 1U << i;
                uint64_t clk_hz = (uint64_t)pclkb_freq / div_val;
                uint64_t counts = (clk_hz * (uint64_t)us) / 1000000ULL;

                if (counts <= max_count) {
                    best_div = agt_pclkb_divs[i];
                    period_counts = (counts == 0) ? 1 : (uint32_t)counts;
                    break;
                }
            }
        }

        if (obj->cfg.source_div != best_div) {
            obj->cfg.source_div = best_div;
            needs_reopen = true;
        }
    }

    // Defensive fallback: Clamp to max counter value if limits are exceeded and run at current prescaler
    if (period_counts == 0) {
        period_counts = (uint32_t)max_count;
    }

    obj->cfg.period_counts = period_counts;

    /* Re-apply settings via Close -> Open if prescaler (source_div) or clock source (count_source) has changed */
    if (needs_reopen && obj->initialized) {
        obj->p_timer->p_api->stop(obj->p_timer->p_ctrl);
        obj->p_timer->p_api->close(obj->p_timer->p_ctrl);

        fsp_err_t err = obj->p_timer->p_api->open(obj->p_timer->p_ctrl, &obj->cfg);
        MBED_ASSERT(FSP_SUCCESS == err);

        obj->p_timer->p_api->start(obj->p_timer->p_ctrl);
    } else {
        // Update period directly if neither prescaler nor clock source has changed
        fsp_err_t err = obj->p_timer->p_api->periodSet(obj->p_timer->p_ctrl, period_counts);
        MBED_ASSERT(FSP_SUCCESS == err);
    }

    obj->period_counts = period_counts;

    if (obj->duty_counts > 0) {
        float duty = (float) obj->duty_counts / (float) obj->period_counts;
        pwmout_write(obj, duty);
    }
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

void pwmout_write(pwmout_t *obj, float value)
{
    MBED_ASSERT(obj && obj->p_timer);

    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;

    if (obj->period_counts == 0) {
        pwmout_period_us(obj, 1000);
    }

    uint32_t duty = (uint32_t)((float) obj->period_counts * value);
    if(duty >= obj->period_counts)
    {
        duty = obj->period_counts - 1;
    }
    fsp_err_t err = obj->p_timer->p_api->dutyCycleSet(
        obj->p_timer->p_ctrl,
        duty,
        obj->pwm_unit
    );
    MBED_ASSERT(FSP_SUCCESS == err);

    obj->duty_counts = duty;
}

float pwmout_read(pwmout_t *obj)
{
    MBED_ASSERT(obj);

    if (obj->period_counts == 0) {
        return 0.0f;
    }

    return (float) obj->duty_counts / (float) obj->period_counts;
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    MBED_ASSERT(obj && obj->p_timer);

    if (us <= 0) {
        pwmout_write(obj, 0.0f);
        return;
    }

    timer_info_t info;
    fsp_err_t err = obj->p_timer->p_api->infoGet(obj->p_timer->p_ctrl, &info);
    MBED_ASSERT(FSP_SUCCESS == err);

    uint64_t counts = ((uint64_t) info.clock_frequency * (uint64_t) us) / 1000000ULL;

    if (obj->period_counts == 0) {
        pwmout_period_us(obj, 1000);
    }

    if (counts >= obj->period_counts) {
        pwmout_write(obj, 1.0f);
    } else {
        float duty = (float) counts / (float) obj->period_counts;
        pwmout_write(obj, duty);
    }
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}


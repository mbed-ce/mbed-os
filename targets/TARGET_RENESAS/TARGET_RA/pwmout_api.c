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

    fsp_err_t err = obj->p_timer->p_api->open(obj->p_timer->p_ctrl, obj->p_timer->p_cfg);
    if (FSP_SUCCESS != err && FSP_ERR_ALREADY_OPEN != err) {
        MBED_ASSERT(0);
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

    timer_info_t info;
    fsp_err_t err = obj->p_timer->p_api->infoGet(obj->p_timer->p_ctrl, &info);
    MBED_ASSERT(FSP_SUCCESS == err);

    uint64_t counts = ((uint64_t) info.clock_frequency * (uint64_t) us) / 1000000ULL;
    if (counts == 0) {
        counts = 1;
    }

    // TODO change prescaler when counts overflows
    err = obj->p_timer->p_api->periodSet(obj->p_timer->p_ctrl, (uint32_t) counts);
    MBED_ASSERT(FSP_SUCCESS == err);

    obj->period_counts = (uint32_t) counts;

    if (obj->duty_counts > 0) {
        float duty = (float) obj->duty_counts / (float) obj->period_counts;
        pwmout_write(obj, duty);
    }
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
        duty--;
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
        obj->period_counts = (uint32_t) counts;
        err = obj->p_timer->p_api->periodSet(obj->p_timer->p_ctrl, obj->period_counts);
        MBED_ASSERT(FSP_SUCCESS == err);
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

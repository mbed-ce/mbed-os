/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed_assert.h"
#include "pwmout_api.h"
#include "cmsis.h"
#include "PeripheralPinMaps.h"

#include <math.h>
#include <stdlib.h>

// Change to 1 to enable debug prints of what's being calculated.
// Must comment out the critical section calls in PwmOut to use.
#define LPC1768_PWMOUT_DEBUG 0

#if LPC1768_PWMOUT_DEBUG
#include <stdio.h>
#include <inttypes.h>
#endif

#define TCR_CNT_EN       0x00000001
#define TCR_RESET        0x00000002

__IO uint32_t *PWM_MATCH[] = {
    &(LPC_PWM1->MR0),
    &(LPC_PWM1->MR1),
    &(LPC_PWM1->MR2),
    &(LPC_PWM1->MR3),
    &(LPC_PWM1->MR4),
    &(LPC_PWM1->MR5),
    &(LPC_PWM1->MR6)
};

#define TCR_PWM_EN       0x00000008

static unsigned int pwm_clocks_per_us;

void pwmout_init_direct(pwmout_t *obj, const PinMap *pinmap)
{
    // determine the channel
    PWMName pwm = (PWMName)pinmap->peripheral;

    obj->pwm = pwm;
    obj->MR = PWM_MATCH[pwm];

    // ensure the power is on
    LPC_SC->PCONP |= 1 << 6;

    // Set PWM clock to CCLK / 4.  This means that the PWM counter will increment every (4 / SystemCoreClock) seconds,
    // or 41.7ns (1us/24) with default clock settings.
    LPC_SC->PCLKSEL0 &= ~(0x3 << 12);     // pclk = /4
    LPC_PWM1->PR = 0;                     // no pre-scale

    // ensure single PWM mode
    LPC_PWM1->MCR = 1 << 1; // reset TC on match 0

    // enable the specific PWM output
    LPC_PWM1->PCR |= 1 << (8 + pwm);

    // Calculate microseconds -> clocks conversion, factoring in the prescaler of 4 we set earlier.
    pwm_clocks_per_us = SystemCoreClock / 4 / 1000000;

    // default to 20ms: standard for servos, and fine for e.g. brightness control
    pwmout_period_ms(obj, 20);
    pwmout_write(obj, 0);

    // Wire pinout
    pin_function(pinmap->pin, pinmap->function);
    pin_mode(pinmap->pin, PullNone);
}

void pwmout_init(pwmout_t *obj, PinName pin)
{
    PinMap pinmap;
    pinmap.pin = pin;
    pinmap.function = pinmap_find_function(pin, PinMap_PWM);
    pinmap.peripheral = pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pinmap.peripheral != NC);

    pwmout_init_direct(obj, &pinmap);
}

void pwmout_free(pwmout_t *obj)
{
    // From testing, it seems like if you just disable the output by clearing the the bit in PCR, the output can get stuck 
    // at either 0 or 1 depending on what level the PWM was at when it was disabled.
    // Instead, we just set the duty cycle of the output to 0 so that it's guaranteed to go low.
    *obj->MR = 0;
    LPC_PWM1->LER = 1 << obj->pwm;
}

void pwmout_write(pwmout_t *obj, float value)
{
    if (value < 0.0f) {
        value = 0.0;
    } else if (value > 1.0f) {
        value = 1.0;
    }

    // set channel match to percentage
    uint32_t v = (uint32_t)lroundf((float)(LPC_PWM1->MR0) * value);

    // workaround for the LPC1768 PWM1[1] errata - bad stuff happens for output 1 if the MR register equals the MR0 register
    // for the module (this could happen if the user tries to set exactly 100% duty cycle).  To avoid this, 
    // if the match register value would equal MR0, increment it again to make it greater than MR0.  This doesn't
    // cause any side effects so long as we make sure MR0 is less than UINT32_MAX - 1.
    if (v == LPC_PWM1->MR0) {
        v++;
    }

    *obj->MR = v;

#if LPC1768_PWMOUT_DEBUG
    printf("Calculated MR=%" PRIu32 " for duty cycle %.06f (MR0 = %" PRIu32 ")\n", v, value, LPC_PWM1->MR0);
#endif

    // accept on next period start
    LPC_PWM1->LER = 1 << obj->pwm;
}

float pwmout_read(pwmout_t *obj)
{
    float v = (float)(*obj->MR) / (float)(LPC_PWM1->MR0);
    return (v > 1.0f) ? (1.0f) : (v);
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t *obj, int us)
{
    // If the passed value is larger than this, it will overflow the MR0 register and bad stuff will happen
    const uint32_t max_period_us = (UINT32_MAX - 2) / pwm_clocks_per_us;

    // calculate number of ticks
    if((uint32_t)us > max_period_us)
    {
        us = max_period_us;
    }
    if(us < 1)
    {
        us = 1;
    }
    uint32_t new_mr0_val = pwm_clocks_per_us * us;

    // set reset
    LPC_PWM1->TCR = TCR_RESET;

    // Scale the pulse width to preserve the duty ratio.  Must use 64-bit math as the numerator can fairly easily overflow 32 bits.
    uint32_t new_mr_val = (*obj->MR * ((uint64_t)new_mr0_val)) / LPC_PWM1->MR0;
#if LPC1768_PWMOUT_DEBUG
    printf("Changing MR0 from %" PRIu32 " to %" PRIu32 ", changing MR from %" PRIu32 " to %" PRIu32 " to preserve duty cycle\n", LPC_PWM1->MR0, new_mr0_val, *obj->MR, new_mr_val);
#endif
    *obj->MR = new_mr_val;

    // set the global match register.  Note that based on testing we do *not* need to subtract 1 here,
    // e.g. setting MR0 to 4 causes the PWM to reset every 4 clocks.  This appears to be because the internal
    // counter starts at 1 from reset, not 0.
    LPC_PWM1->MR0 = new_mr0_val;

    // set the channel latch to update value at next period start
    LPC_PWM1->LER = 1 << 0;

    // enable counter and pwm, clear reset
    LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;
}

int pwmout_read_period_us(pwmout_t *obj)
{
    // Add half a us worth of clocks for correct integer rounding.
    return (LPC_PWM1->MR0 + pwm_clocks_per_us/2) / pwm_clocks_per_us;
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    // calculate number of ticks
    uint32_t v = pwm_clocks_per_us * us;

    // workaround for PWM1[1] - Never make it equal MR0, else we get 1 cycle dropout.  See pwmout_write() for more details.
    if (v == LPC_PWM1->MR0) {
        v++;
    }

    // set the match register value
    *obj->MR = v;

    // set the channel latch to update value at next period start
    LPC_PWM1->LER = 1 << obj->pwm;
}

int pwmout_read_pulsewidth_us(pwmout_t *obj)
{
    uint32_t mr_ticks = *(obj->MR);
    if(mr_ticks > LPC_PWM1->MR0)
    {
        mr_ticks = LPC_PWM1->MR0;
    }
    // Add half a us worth of clocks for correct integer rounding.
    return (mr_ticks + pwm_clocks_per_us/2) / pwm_clocks_per_us;
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}

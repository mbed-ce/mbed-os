/*
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "us_ticker_api.h"
#include "mbed_critical.h"
#include "hardware/timer.h"

#define US_TICKER_COUNTER_BITS        32u
#define US_TICKER_FREQ                1000000

/* us ticker is driven by 1MHz clock and counter length is 32 bits */
const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        US_TICKER_FREQ,
        US_TICKER_COUNTER_BITS
    };
    return &info;
}

static const uint8_t alarm_num = 0;
static const uint8_t irq_num = TIMER_IRQ_0; // IRQ corresponding to the above alarm

static bool us_ticker_inited = false;

static void us_ticker_irq_handler_internal() {
    timer_hw->intr = 1u << alarm_num; // Clear the alarm ISR bit
    us_ticker_irq_handler();
}

void us_ticker_init(void)
{
    if(us_ticker_inited) {
        /* calling init again should cancel current interrupt */
        us_ticker_disable_interrupt();
    }
    else {
        // Note: We don't use the RPi Pico alarm callback mechanism, because it isn't compatible
        // with manually firing the interrupt.  So, we attach a function but then immediately replace
        // the driver IRQ handler with our own.
        hardware_alarm_claim(alarm_num);
        hardware_alarm_set_callback(alarm_num, (hardware_alarm_callback_t)us_ticker_irq_handler_internal);

        irq_remove_handler(irq_num, irq_get_vtable_handler(irq_num));
        irq_set_exclusive_handler(irq_num, &us_ticker_irq_handler_internal);

        us_ticker_inited = true;
    }

}

uint32_t (us_ticker_read)()
{
    return us_ticker_read();
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    core_util_critical_section_enter();

    uint64_t _timestamp = (uint64_t)timestamp;

    if (timestamp < time_us_32()) {
        //32 bit timestamp has been wrapped
        //We need to provide a 64 bit timestamp able to fire the irq for this round
        _timestamp = (((time_us_64() >> 32) + 1) << 32) + timestamp;
    } else {
        //Then, at the next round, wrap the 64 bit timer to follow the 32 bit one
        if ((time_us_64() >> 32) > 0) {
            uint64_t current_time = time_us_64();
            uint64_t wrapped_time = current_time - 0xFFFFFFFF;
            timer_hw->timelw = (uint32_t)wrapped_time;
            timer_hw->timehw = 0;
        }
    }
    absolute_time_t target = { _timestamp };
    hardware_alarm_set_target(alarm_num, target);

    core_util_critical_section_exit();
}

void us_ticker_fire_interrupt(void)
{
    irq_set_pending(irq_num);
}

void us_ticker_disable_interrupt(void)
{
    hardware_alarm_cancel(alarm_num);
}

void us_ticker_clear_interrupt(void)
{
    hardware_alarm_cancel(alarm_num);
}

void us_ticker_free(void)
{
    hardware_alarm_unclaim(alarm_num);
}

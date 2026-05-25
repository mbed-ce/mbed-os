/* mbed Microcontroller Library
 * Copyright (c) 2026 Jamie Smith
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

#include "lp_ticker_api.h"
#include "mbed_critical.h"

#include "hardware/powman.h"

// To increase time precision (with no real downside), we run the ticker at a faster
// clock than the usual 1ms. Per the datasheet, the recommended fastest clock is
// 16 ticks/ms, giving a quite respectable time resolution of 62.5us.
#define MBED_PICO_AON_TIMER_TICKS_PER_MS 16

void lp_ticker_init(void) {

    // Initialize the timer, using the recommended method of "lying" to the SDK about
    // how fast the clock is. (if we gave it the real clock values it would tick at 1kHz)
#if MBED_CONF_TARGET_LPTICKER_USE_LPOSC
    powman_timer_set_1khz_tick_source_lposc_with_hz(32768 / MBED_PICO_AON_TIMER_TICKS_PER_MS);
#else
    powman_timer_set_1khz_tick_source_xosc_with_hz(XOSC_HZ / MBED_PICO_AON_TIMER_TICKS_PER_MS);
#endif

    // Disable the alarm, in case it's currently enabled
    powman_timer_disable_alarm();

    // Start the timer counting
    powman_timer_start();

    // Make sure the IRQ can fire
    irq_set_enabled(POWMAN_IRQ_TIMER, true);
}

void lp_ticker_free(void) {
    powman_timer_stop();
}

uint32_t lp_ticker_read(void) {
    // Note: This doesn't actually return ms, because of MBED_PICO_AON_TIMER_TICKS_PER_MS
    return powman_timer_get_ms();
}

void lp_ticker_set_interrupt(const timestamp_t alarm_ts) {
    core_util_critical_section_enter();

    // Annoyingly, the Mbed ticker API always works in terms of 32-bit values, even though
    // on RP2xxx the hardware supports 64 bit timers. So we have to "expand" the provided
    // 32-bit alarm timestamp into a 64-bit number
    const uint64_t now_64 = powman_timer_get_ms();
    const uint32_t now_upper_half = now_64 >> 32;
    const uint32_t now_lower_half = now_64 & 0xFFFFFFFFUL;

    uint64_t alarm_ts_64;
    if (alarm_ts < now_lower_half) {
        // Alarm TS will be after the next rollover of the lower counter
        alarm_ts_64 = ((uint64_t)(now_upper_half + 1) << 32) | alarm_ts;
    } else {
        // Alarm TS is before the next rollover
        alarm_ts_64 = ((uint64_t)now_upper_half << 32) | alarm_ts;
    }

    // Note: This also clears any pending alarm
    powman_timer_enable_alarm_at_ms(alarm_ts_64);

    core_util_critical_section_exit();
}

void lp_ticker_disable_interrupt(void) {
    powman_timer_disable_alarm();
}

void lp_ticker_clear_interrupt(void) {
    if(powman_hw->timer & POWMAN_TIMER_ALARM_BITS) {
        // Per comments in the SDK, the alarm must be disabled first before clearing, otherwise it will retrigger.
        powman_timer_disable_alarm();
        powman_clear_alarm();
    }
}

void lp_ticker_fire_interrupt(void) {
    NVIC_SetPendingIRQ(POWMAN_IRQ_TIMER_IRQn);
}

const ticker_info_t *lp_ticker_get_info(void) {
    static const ticker_info_t info = {
       MBED_PICO_AON_TIMER_TICKS_PER_MS * 1000,
        32
    };
    return &info;
}
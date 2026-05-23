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


#include "sleep_api.h"

#include "pico/runtime_init.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/rosc.h"

void hal_sleep(void) {
    // For this function we will sleep the core without turning off any clocks.

    // Request non-deep (shallow?) sleep
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

    // wait for interrupt
    __WFI();
}

#if PICO_RP2040
#define DEEPSLEEP_KEEP_ENABLED_CLOCKS_EN0 (CLOCKS_SLEEP_EN0_CLK_RTC_RTC_BITS | CLOCKS_SLEEP_EN0_CLK_SYS_PADS_BITS | CLOCKS_SLEEP_EN0_CLK_SYS_IO_BITS)
#endif

static void reconfigure_clock_tree_for_deepsleep() {
    // Slow down CLK_REF and CLK_SYS by running them directly off the XOSC
    clock_configure_undivided(clk_ref,
                    CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC,
                    0, // No aux mux
                     XOSC_HZ);

    clock_configure_undivided(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF,
                    0, /// Using glitchless mux
                    XOSC_HZ);

#if PICO_RP2040
    // CLK RTC = ideally XOSC (12MHz) / 256 = 46875Hz but could be rosc
    clock_configure(clk_rtc,
                    0, // No GLMUX
                    CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_XOSC_CLKSRC,
                    XOSC_HZ,
                    46875);
#endif

    // CLK PERI = clk_sys. Used as reference clock for Peripherals. No dividers so just select and enable
    clock_configure_undivided(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    XOSC_HZ);

    // Disable other clocks.
    clock_stop(clk_adc);
    clock_stop(clk_usb);
#if PICO_RP2350
    clock_stop(clk_hstx);
#endif
    pll_deinit(pll_sys);
    pll_deinit(pll_usb);

    // rosc not needed
    rosc_disable();
}

static void reset_clock_tree_after_deepsleep() {
    // First reenable the ROSC as apparently this is needed for runtime_init_clocks() to complete successfully
    rosc_enable();

    // Now we can use the regular Pico SDK function to reconfigure the clocks.
    runtime_init_clocks();
}

void hal_deepsleep(void) {
    // Note: See example of sleep code here:
    // https://github.com/raspberrypi/pico-extras/blob/eb071cd88a8d7f6227ae55413e773454d1455168/src/rp2_common/pico_sleep/sleep.c#L159

    // When asleep, turn off all clocks other than GPIO, LP ticker, and watchdog.
    CLOCKS->SLEEP_EN0 = DEEPSLEEP_KEEP_ENABLED_CLOCKS_EN0;
    CLOCKS->SLEEP_EN1 = 0;

    reconfigure_clock_tree_for_deepsleep();

    // Request deep sleep
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // wait for interrupt
    __WFI();

    reset_clock_tree_after_deepsleep();

    // Reenable clocks during sleep (in anticipation of a future hal_sleep() call)
    CLOCKS->SLEEP_EN0 = 0xFFFFFFFF;
    CLOCKS->SLEEP_EN1 = 0xFFFFFFFF;
}

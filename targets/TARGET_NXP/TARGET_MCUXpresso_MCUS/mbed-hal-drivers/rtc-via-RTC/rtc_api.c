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
#include "rtc_api.h"

#if DEVICE_RTC || DEVICE_LPTICKER

#include "pinmap.h"
#include "fsl_rtc.h"
#include "mbed_error.h"
#include "mbed_wait_api.h"
#include "PeripheralPins.h"

static bool rtc_enabled = false;

void rtc_init(void)
{
    rtc_config_t rtcConfig;

    if (!rtc_enabled) {
        CLOCK_EnableClock(kCLOCK_Rtc0);

        /*Init the RTC with default configuration*/
        RTC_GetDefaultConfig(&rtcConfig);

#if !(defined(FSL_FEATURE_RTC_HAS_CLOCK_OUTPUT) && FSL_FEATURE_RTC_HAS_CLOCK_OUTPUT == 0)
        // Enable clock output to other peripherals (in particular, the LPTMR).
        // Gotta love that this was added via an SDK update, changing the default behavior.
        // No notes, Freescale. Incredible work here. Only took me an hour of debugging to find this...
        rtcConfig.clockOutput = true;
#endif

        /* Setup the 32K RTC OSC */
        RTC_Init(RTC, &rtcConfig);

        /* Enable the RTC 32KHz oscillator */
        RTC->CR |= RTC_CR_OSCE_MASK;

        /* Start the RTC time counter */
        RTC_StartTimer(RTC);

        // Check that RTC is actually counting
        const uint16_t origTPR = RTC->TPR;
        wait_us(500);
        if(RTC->TPR == origTPR) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_RTC, EDEADLK), \
                   "No clock detected on ERCLK32K, RTC oscillator missing or nonfunctional?\n");
        }
    }

    rtc_enabled = true;
}

void rtc_free(void)
{
    if (rtc_enabled) {
        // Do not actually disable the RTC peripheral as it drives the LP ticker clock as well
        rtc_enabled = false;
    }
}

/*
 * Little check routine to see if the RTC has been initialized and time has been set
 * 0 = Disabled, 1 = Enabled
 */
int rtc_isenabled(void)
{
    return rtc_enabled;
}

time_t rtc_read(void)
{
    return (time_t)RTC->TSR;
}

void rtc_write(time_t t)
{
    RTC_StopTimer(RTC);
    RTC->TSR = t;
    RTC_StartTimer(RTC);
}

#endif

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
#include "PeripheralPins.h"

static bool rtc_enabled = false;

void rtc_init(void)
{
    rtc_config_t rtcConfig;

    if (!rtc_enabled) {
        RTC_GetDefaultConfig(&rtcConfig);
        RTC_Init(RTC, &rtcConfig);

        RTC_StartTimer(RTC);
    }

    rtc_enabled = true;
}

void rtc_free(void)
{
    if (rtc_enabled) {
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
        /* Gate the module clock */
        CLOCK_DisableClock(kCLOCK_Rtc0);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
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

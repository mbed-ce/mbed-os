/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtc_api.h"
#include "bsp_api.h"
#include "hal_data.h"
#include "mbed_mktime.h"

static bool rtc_opened = false;

void rtc_init(void)
{
    if (rtc_opened) {
        return;
    }

    fsp_err_t err = R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    if (FSP_SUCCESS == err) {
        rtc_opened = true;
    }

}

int rtc_isenabled(void)
{
    if (!rtc_opened) {
        return 0;
    }
    return 1;
}

time_t rtc_read(void)
{
    if (!rtc_opened) {
        rtc_init();
    }

    rtc_time_t rtc_time;
    fsp_err_t err = R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &rtc_time);
    if (FSP_SUCCESS != err) {
        return 0;
    }

    struct tm t;
    memset(&t, 0, sizeof(t));
    t.tm_sec  = rtc_time.tm_sec;
    t.tm_min  = rtc_time.tm_min;
    t.tm_hour = rtc_time.tm_hour;
    t.tm_mday = rtc_time.tm_mday;
    t.tm_mon  = rtc_time.tm_mon;        // struct tm: 0-11
    t.tm_year = rtc_time.tm_year;    // struct tm: since 1900

    time_t ts = 0;
    if (_rtc_maketime(&t, &ts, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return 0;
    }

    return ts;
}

void rtc_write(time_t t)
{
    if (!rtc_opened) {
        rtc_init();
    }

    struct tm timeinfo;
    if (_rtc_localtime(t, &timeinfo, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return;
    }

    rtc_time_t rtc_time;
    rtc_time.tm_sec  = timeinfo.tm_sec;
    rtc_time.tm_min  = timeinfo.tm_min;
    rtc_time.tm_hour = timeinfo.tm_hour;
    rtc_time.tm_mday = timeinfo.tm_mday;
    rtc_time.tm_mon  = timeinfo.tm_mon;
    rtc_time.tm_year = timeinfo.tm_year;
    rtc_time.tm_wday = timeinfo.tm_wday;

    (void) R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &rtc_time);
}

void rtc_free(void)
{
    if (!rtc_opened) {
        return;
    }
}

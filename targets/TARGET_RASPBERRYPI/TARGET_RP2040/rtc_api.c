#if DEVICE_RTC

#include "rtc_api.h"
#include "hardware/rtc.h"
#include "hardware/structs/rtc.h"
#include "mbed_mktime.h"

void rtc_init(void)
{
    _rtc_init();
}

void rtc_free(void)
{
   /* RTC clock can not be reset */
}

int rtc_isenabled(void)
{
    return rtc_running();
}

time_t rtc_read(void)
{
    struct tm timeinfo;
    time_t t;
    datetime_t date;
    
    if (!rtc_get_datetime(&date)) {
        return 0;
    } 

    /* Setup a tm structure based on the RTC
    struct tm :
        tm_sec      seconds after the minute 0-61
        tm_min      minutes after the hour 0-59
        tm_hour     hours since midnight 0-23
        tm_mday     day of the month 1-31
        tm_mon      months since January 0-11
        tm_year     years since 1900
        tm_yday     information is ignored by _rtc_maketime
        tm_wday     information is ignored by _rtc_maketime
        tm_isdst    information is ignored by _rtc_maketime
    */
    timeinfo.tm_year = date.year - 1900;
    timeinfo.tm_mon = date.month - 1;
    timeinfo.tm_mday = date.day;
    timeinfo.tm_wday = date.dotw;
    timeinfo.tm_hour = date.hour;
    timeinfo.tm_min = date.min;
    timeinfo.tm_sec = date.sec;

    if (_rtc_maketime(&timeinfo, &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return 0;
    }

    return t;
}

void rtc_write(time_t t)
{
    struct tm timeinfo;
    datetime_t date;

    if (_rtc_localtime(t, &timeinfo, RTC_4_YEAR_LEAP_YEAR_SUPPORT) == false) {
        return;
    }
    
    /* Setup a datetime_t structure based on the RTC
     struct datetime_t
        year;     0..4095
        month;    1..12, 1 is January
        day;      1..28,29,30,31 depending on month
        dotw;     0..6, 0 is Sunday
        hour;     0..23
        min;      0..59
        sec;      0..59
    */
    date.year = timeinfo.tm_year + 1900;
    date.month = timeinfo.tm_mon + 1;
    date.day = timeinfo.tm_mday;
    date.dotw = timeinfo.tm_wday;
    date.hour = timeinfo.tm_hour;
    date.min = timeinfo.tm_min;
    date.sec = timeinfo.tm_sec;

    rtc_set_datetime(&date);
    return;
}

#endif // DEVICE_RTC
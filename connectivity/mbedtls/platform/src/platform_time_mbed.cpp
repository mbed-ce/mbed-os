/* Copyright (c) 2026 Mbed OS Community Edition contributors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbedtls/platform_time.h"

#if defined(MBEDTLS_PLATFORM_MS_TIME_ALT)

#if DEVICE_LPTICKER
#include "drivers/LowPowerClock.h"
using MbedTlsClock = mbed::LowPowerClock;
#else
#include "drivers/HighResClock.h"
using MbedTlsClock = mbed::HighResClock;
#endif

extern "C" mbedtls_ms_time_t mbedtls_ms_time(void)
{
    return static_cast<mbedtls_ms_time_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(MbedTlsClock::now().time_since_epoch()).count());
}

#endif

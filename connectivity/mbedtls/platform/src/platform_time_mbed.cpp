/* Copyright (c) 2026 Mbed OS Community Edition contributors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbedtls/platform_time.h"
#include "platform/mbed_power_mgmt.h"

#if defined(MBEDTLS_PLATFORM_MS_TIME_ALT)

extern "C" mbedtls_ms_time_t mbedtls_ms_time(void)
{
    return static_cast<mbedtls_ms_time_t>(mbed_uptime() / 1000);
}

#endif

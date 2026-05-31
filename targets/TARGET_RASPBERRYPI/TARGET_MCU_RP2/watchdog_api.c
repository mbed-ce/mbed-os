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

#include "watchdog_api.h"
#include "hardware/watchdog.h"
#include "hardware/structs/watchdog.h"

#if DEVICE_WATCHDOG

static watchdog_config_t watchdogConfig;

// On RP2040, an errata makes the watchdog hardware count twice as fast as intended, so
// its resolution and max time are halved.
#if PICO_RP2040
#define WATCHDOG_MAX_VALUE 0x7fffff
#define WATCHDOG_FREQUENCY 500 // Hz
#else
#define WATCHDOG_MAX_VALUE 0xffffff
#define WATCHDOG_FREQUENCY 1000 // Hz
#endif

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    watchdogConfig = *config;
    if ( config->timeout_ms < 0x1 || config->timeout_ms > WATCHDOG_MAX_VALUE ) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    watchdog_enable(config->timeout_ms, true);

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    watchdog_update();
}

watchdog_status_t hal_watchdog_stop(void)
{
    watchdog_disable();
    return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    return watchdogConfig.timeout_ms;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;

    features.max_timeout = WATCHDOG_MAX_VALUE;
    features.update_config = true;
    features.disable_watchdog = true;

    // SDK configures the watchdog underlying counter to run at 1MHz
    features.clock_typical_frequency = WATCHDOG_FREQUENCY;
    features.clock_max_frequency = WATCHDOG_FREQUENCY;

    return features;
}

#endif // DEVICE_WATCHDOG

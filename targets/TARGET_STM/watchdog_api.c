/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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


#ifdef DEVICE_WATCHDOG

#include "watchdog_api.h"
#include "device.h"
#include "mbed_error.h"

#if defined(TARGET_STM32WB0)
#include "mbed_wait_api.h"
#include "rtc_clock_source.h"

#if (MBED_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_LSE_OR_LSI) && MBED_CONF_TARGET_LSE_AVAILABLE
#define WB0_IWDG_USES_LSE 1
#define IWDG_CLOCK_VALUE LSE_VALUE
#else
#define WB0_IWDG_USES_LSE 0
#define IWDG_CLOCK_VALUE LSI_VALUE
#endif
#else
#define IWDG_CLOCK_VALUE LSI_VALUE
#endif

#define MAX_IWDG_PR 0x6 // Max value of Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR)
#define MAX_IWDG_RL 0xFFFUL // Max value of Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR).

// Convert Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR) to a prescaler_divider value.
#define PR2PRESCALER_DIV(PR_BITS) \
    (4UL << (PR_BITS))

// Convert Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR)
// and Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR)
// to a timeout value [ms].
#define PR_RL2UINT64_TIMEOUT_MS(PR_BITS, RL_BITS) \
    ((PR2PRESCALER_DIV(PR_BITS)) * (RL_BITS) * 1000ULL / (IWDG_CLOCK_VALUE))

// Convert Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR) and a timeout value [ms]
// to Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR)
#define PR_TIMEOUT_MS2RL(PR_BITS, TIMEOUT_MS) \
    (((TIMEOUT_MS) * (IWDG_CLOCK_VALUE) / (PR2PRESCALER_DIV(PR_BITS)) + 999UL) / 1000UL)

#define MAX_TIMEOUT_MS_UINT64 PR_RL2UINT64_TIMEOUT_MS(MAX_IWDG_PR, MAX_IWDG_RL)
#if (MAX_TIMEOUT_MS_UINT64 > UINT32_MAX)
#define MAX_TIMEOUT_MS UINT32_MAX
#else
#define MAX_TIMEOUT_MS (MAX_TIMEOUT_MS_UINT64 & 0xFFFFFFFFUL)
#endif

#define INVALID_IWDG_PR ((MAX_IWDG_PR) + 1) // Arbitrary value used to mark an invalid PR bits value.

// Pick a minimal Prescaler_divider bits (PR) value suitable for given timeout.
static uint8_t pick_min_iwdg_pr(const uint32_t timeout_ms)
{
    for (uint8_t pr = 0; pr <= MAX_IWDG_PR; pr++) {
        // Check that max timeout for given pr is greater than
        // or equal to timeout_ms.
        if (PR_RL2UINT64_TIMEOUT_MS(pr, MAX_IWDG_RL) >= timeout_ms) {
            return pr;
        }
    }
    return INVALID_IWDG_PR;
}

static IWDG_HandleTypeDef IwdgHandle;

#if defined(TARGET_STM32WB0)
static void enable_iwdg_clock(void)
{
    RCC_OscInitTypeDef oscillator = {0};

#if WB0_IWDG_USES_LSE
    if (!__HAL_RCC_GET_LSE_READYFLAG()) {
        oscillator.OscillatorType = RCC_OSCILLATORTYPE_LSE;
        oscillator.LSEState = RCC_LSE_ON;
#if MBED_CONF_TARGET_LSE_BYPASS
        oscillator.OscillatorType |= RCC_OSCILLATORTYPE_LSE_BYPASS;
        oscillator.LSEBYPASSState = RCC_LSE_BYPASS_ON;
#else
        oscillator.LSEBYPASSState = RCC_LSE_BYPASS_OFF;
#endif
        if (HAL_RCC_OscConfig(&oscillator) != HAL_OK) {
            error("Cannot initialize watchdog clock with LSE\n");
        }
    }
    __HAL_RCC_RTC_WDG_BLEWKUP_CLK_CONFIG(RCC_RTC_WDG_BLEWKUP_CLKSOURCE_LSE);
#else
    if (!__HAL_RCC_GET_LSI_READYFLAG()) {
        oscillator.OscillatorType = RCC_OSCILLATORTYPE_LSI;
        oscillator.LSIState = RCC_LSI_ON;
        if (HAL_RCC_OscConfig(&oscillator) != HAL_OK) {
            error("Cannot initialize watchdog clock with LSI\n");
        }
    }
    __HAL_RCC_RTC_WDG_BLEWKUP_CLK_CONFIG(RCC_RTC_WDG_BLEWKUP_CLKSOURCE_LSI);
#endif

    if (__HAL_RCC_WDG_IS_CLK_DISABLED()) {
        __HAL_RCC_WDG_CLK_ENABLE();
        /* RCC requires two slow-clock cycles before the first IWDG access. */
        wait_us(100);
    }
}
#endif

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    const uint8_t pr = pick_min_iwdg_pr(config->timeout_ms);
    if (pr == INVALID_IWDG_PR) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }
    const uint32_t rl = PR_TIMEOUT_MS2RL(pr, config->timeout_ms);

    IwdgHandle.Instance = IWDG;

    IwdgHandle.Init.Prescaler = pr;
    IwdgHandle.Init.Reload    = rl;
#if defined IWDG_WINR_WIN
    IwdgHandle.Init.Window = IWDG_WINDOW_DISABLE;
#endif

#if defined(TARGET_STM32WB0)
    /* WB0 shares its low-speed clock source between RTC, watchdog and BLE wake-up. */
    enable_iwdg_clock();
#endif

    if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK) {
        error("HAL_IWDG_Init error\n");
    }

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    HAL_IWDG_Refresh(&IwdgHandle);
}

watchdog_status_t hal_watchdog_stop(void)
{
    return WATCHDOG_STATUS_NOT_SUPPORTED;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    // Wait for the Watchdog_prescaler_value_update bit (PVU) of
    // Status_register (IWDG_SR) to be reset.
    while (READ_BIT(IWDG->SR, IWDG_SR_PVU)) {
    }
    // Read Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR).
    const uint8_t pr = (IWDG->PR & IWDG_PR_PR_Msk) >> IWDG_PR_PR_Pos;

    // Wait for the Watchdog_counter_reload_value_update bit (RVU) of
    // Status_register (IWDG_SR) to be reset.
    while (READ_BIT(IWDG->SR, IWDG_SR_RVU)) {
    }
    // Read Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR).
    const uint32_t rl = (IWDG->RLR & IWDG_RLR_RL_Msk) >> IWDG_RLR_RL_Pos;

    return PR_RL2UINT64_TIMEOUT_MS(pr, rl);
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;
    features.max_timeout = MAX_TIMEOUT_MS;
    features.update_config = true;
    features.disable_watchdog = false;

    /* STM32 IWDG normally uses LSI; WB0 uses its selected shared low-speed clock. */
    features.clock_typical_frequency = IWDG_CLOCK_VALUE;

    /*  See LSI oscillator characteristics in Data Sheet */
#if defined(STM32F1)
    features.clock_max_frequency = 60000;
#elif defined(STM32L0) || defined(STM32L1)
    features.clock_max_frequency = 56000;
#elif defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
    features.clock_max_frequency = 47000;
#elif defined(STM32F0) || defined(STM32F3)
    features.clock_max_frequency = 50000;
#elif defined(STM32H7) || defined(STM32L4) || defined(STM32U5) || defined(STM32H5)
    features.clock_max_frequency = 33600;
#elif defined(STM32G0) || defined(STM32L5) || defined(STM32G4) || defined(STM32WB) || defined(STM32WL) || defined(STM32U0)
    features.clock_max_frequency = 34000;
#elif defined(STM32WB0)
#if WB0_IWDG_USES_LSE
    features.clock_max_frequency = LSE_VALUE;
#else
    features.clock_max_frequency = 49000;
#endif
#else
#error "unsupported target"
#endif

    return features;
}

#endif // DEVICE_WATCHDOG

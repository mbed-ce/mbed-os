/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "watchdog_api.h"
#include "hal_data.h"
#include "mbed_error.h"

extern const wdt_instance_t g_wdt0;

static uint32_t s_timeout_ms = 0;
static uint32_t s_effective_timeout_ms = 0;

static const wdt_timeout_t s_timeouts[] = {
    WDT_TIMEOUT_1024,
    WDT_TIMEOUT_4096,
    WDT_TIMEOUT_8192,
    WDT_TIMEOUT_16384,
};

static const uint32_t s_timeout_cycles[] = {
    1024u,
    4096u,
    8192u,
    16384u,
};

static const wdt_clock_division_t s_divs[] = {
    WDT_CLOCK_DIVISION_4,
    WDT_CLOCK_DIVISION_64,
    WDT_CLOCK_DIVISION_128,
    WDT_CLOCK_DIVISION_256,
    WDT_CLOCK_DIVISION_512,
    WDT_CLOCK_DIVISION_2048,
    WDT_CLOCK_DIVISION_8192,
};

static const uint32_t s_div_values[] = {
    4u,
    64u,
    128u,
    256u,
    512u,
    2048u,
    8192u,
};

static bool pick_wdt_params(uint32_t requested_ms,
                            wdt_timeout_t *p_timeout,
                            wdt_clock_division_t *p_div,
                            uint32_t *p_effective_ms)
{
    const uint32_t pclkb = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);

    if ((pclkb == 0U) || (requested_ms == 0U)) {
        return false;
    }

    bool found = false;
    uint64_t best_cycles = UINT64_MAX;
    size_t best_i = 0;
    size_t best_j = 0;

    for (size_t i = 0; i < sizeof(s_timeouts) / sizeof(s_timeouts[0]); i++) {
        for (size_t j = 0; j < sizeof(s_divs) / sizeof(s_divs[0]); j++) {
            const uint64_t cycles = (uint64_t)s_timeout_cycles[i] * (uint64_t)s_div_values[j];
            if ((cycles * 1000ULL) >= ((uint64_t)requested_ms * (uint64_t)pclkb) &&
				(cycles * 1000ULL) <= ((uint64_t)requested_ms * (uint64_t)pclkb * 2ULL)) {
                if ((!found) || (cycles < best_cycles)) {
                    found = true;
                    best_cycles = cycles;
                    best_i = i;
                    best_j = j;
                }
            }
        }
    }

    if (!found) {
        return false;
    }

    uint32_t effective_ms =
        (uint32_t)((best_cycles * 1000ULL) / (uint64_t)pclkb);

    if (effective_ms == 0U) {
        effective_ms = 1U;
    }

    *p_timeout      = s_timeouts[best_i];
    *p_div          = s_divs[best_j];
    *p_effective_ms = effective_ms;

    return true;
}

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    if ((config == NULL) || (config->timeout_ms == 0U)) {
        return WATCHDOG_STATUS_NOT_SUPPORTED;
    }

    wdt_timeout_t timeout_sel;
    wdt_clock_division_t div_sel;
    uint32_t effective_ms = 0;

    if (!pick_wdt_params(config->timeout_ms, &timeout_sel, &div_sel, &effective_ms)) {
        return WATCHDOG_STATUS_NOT_SUPPORTED;
    }

    s_timeout_ms           = config->timeout_ms;
    s_effective_timeout_ms = effective_ms;

    wdt_cfg_t cfg = *g_wdt0.p_cfg;

    cfg.timeout        = timeout_sel;
    cfg.clock_division = div_sel;

    fsp_err_t err = g_wdt0.p_api->open(g_wdt0.p_ctrl, &cfg);
    if (FSP_SUCCESS != err) {
        return WATCHDOG_STATUS_NOT_SUPPORTED;
    }

    err = g_wdt0.p_api->refresh(g_wdt0.p_ctrl);
    if (FSP_SUCCESS != err) {
        return WATCHDOG_STATUS_NOT_SUPPORTED;
    }

    return WATCHDOG_STATUS_OK;
}

watchdog_status_t hal_watchdog_stop(void)
{
    return WATCHDOG_STATUS_NOT_SUPPORTED;
}

void hal_watchdog_kick(void)
{
    fsp_err_t err = g_wdt0.p_api->refresh(g_wdt0.p_ctrl);
    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_WATCHDOG, MBED_ERROR_FAILED_OPERATION),
                   "WDT refresh failed");
    }
}

uint32_t hal_watchdog_get_reload_value(void)
{
    return s_effective_timeout_ms;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;
	const uint32_t pclkb = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);
	uint64_t max_cycles = 16384ULL * 8192ULL;
    features.max_timeout = (uint32_t)((max_cycles * 1000ULL) / (uint64_t)pclkb);
    features.update_config = false;
    features.disable_watchdog = false;
    features.clock_typical_frequency = 24000;
    features.clock_max_frequency = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB) / 4;
    return features;
}

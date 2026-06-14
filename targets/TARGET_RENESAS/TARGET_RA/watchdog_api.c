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
    WDT_CLOCK_DIVISION_8192,
};

static const uint32_t s_div_values[] = {
    4u,
    64u,
    128u,
    256u,
    512u,
    8192u,
};

static void pick_wdt_params(uint32_t requested_ms,
                            wdt_timeout_t *p_timeout,
                            wdt_clock_division_t *p_div,
                            uint32_t *p_effective_ms)
{
    uint32_t pclkb = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);
    uint32_t best_diff = 0xFFFFFFFFu;
    uint32_t best_ms   = 0;
    wdt_timeout_t best_timeout = WDT_TIMEOUT_1024;
    wdt_clock_division_t best_div = WDT_CLOCK_DIVISION_4;

    for (size_t i = 0; i < sizeof(s_timeouts) / sizeof(s_timeouts[0]); i++) {
        for (size_t j = 0; j < sizeof(s_divs) / sizeof(s_divs[0]); j++) {
            uint64_t cycles = (uint64_t)s_timeout_cycles[i] * (uint64_t)s_div_values[j];
            uint32_t ms = (uint32_t)((cycles * 1000u + pclkb / 2u) / pclkb);

            uint32_t diff = (ms > requested_ms) ? (ms - requested_ms) : (requested_ms - ms);
            if (diff < best_diff) {
                best_diff   = diff;
                best_ms     = ms;
                best_timeout = s_timeouts[i];
                best_div     = s_divs[j];
            }
        }
    }

    *p_timeout      = best_timeout;
    *p_div          = best_div;
    *p_effective_ms = best_ms;
}

void watchdog_init(const watchdog_config_t *config)
{
    fsp_err_t err;

    s_timeout_ms = config->timeout_ms;

    wdt_cfg_t cfg = *g_wdt0.p_cfg;

    wdt_timeout_t timeout_sel;
    wdt_clock_division_t div_sel;
    uint32_t effective_ms = 0;

    pick_wdt_params(s_timeout_ms, &timeout_sel, &div_sel, &effective_ms);

    cfg.timeout        = timeout_sel;
    cfg.clock_division = div_sel;

    s_effective_timeout_ms = effective_ms;

    err = g_wdt0.p_api->open(g_wdt0.p_ctrl, &cfg);
    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_WATCHDOG, MBED_ERROR_CODE_INITIALIZATION_FAILED),
                   "WDT open failed");
    }
}

void watchdog_start(void)
{
    // Started on open
}

void watchdog_stop(void)
{
    MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_WATCHDOG, MBED_ERROR_CODE_UNSUPPORTED),
               "WDT cannot be stopped on RA");
}

void watchdog_kick(void)
{
    fsp_err_t err = g_wdt0.p_api->refresh(g_wdt0.p_ctrl);
    if (FSP_SUCCESS != err) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_WATCHDOG, MBED_ERROR_FAILED_OPERATION),
                   "WDT refresh failed");
    }
}

uint32_t watchdog_get_reload_value(void)
{
    return s_effective_timeout_ms;
}

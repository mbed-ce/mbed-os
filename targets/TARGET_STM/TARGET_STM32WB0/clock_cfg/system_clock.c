/* SPDX-License-Identifier: Apache-2.0 */

/**
 * This file configures the STM32WB0 system clock as follows:
 *--------------------------------------------------------------------
 * System clock source | 1- USE_RC64MPLL (HSE-assisted internal 64 MHz)
 *                     | 2- USE_DIRECT_HSE (external 32 MHz crystal)
 *                     | 3- USE_HSI (internal 64 MHz clock)
 *--------------------------------------------------------------------
 * SYSCLK (MHz)        |       64       |       32       |       64
 * Flash wait states   |        1       |        0       |        1
 *--------------------------------------------------------------------
 */

#include "stm32wb0x.h"
#include "mbed_error.h"
#include "rtc_clock_source.h"

// System clock source is selected with target.clock-source in targets.json5.
#define USE_RC64MPLL   0x8U
#define USE_DIRECT_HSE 0x4U
#define USE_HSI        0x2U

#if ((CLOCK_SOURCE) & (USE_RC64MPLL | USE_DIRECT_HSE)) && (HSE_VALUE != 32000000U)
#error "STM32WB0 RC64MPLL and direct HSE require HSE_VALUE=32000000"
#endif

#if ((CLOCK_SOURCE) & USE_RC64MPLL)
uint8_t SetSysClock_RC64MPLL(void);
#endif

#if ((CLOCK_SOURCE) & USE_DIRECT_HSE)
uint8_t SetSysClock_DIRECT_HSE(void);
#endif

#if ((CLOCK_SOURCE) & USE_HSI)
uint8_t SetSysClock_HSI(void);
#endif

static void restore_rtc_clock(void)
{
    static uint8_t initial_clock_setup_done;

    // The RTC calendar is retained across reset, but its clocking may not be.
    // Later calls to SetSysClock restore the system clock after sleep and must
    // not restart RTC clocking.
    __HAL_RCC_RTC_CLK_ENABLE();
    if (!initial_clock_setup_done && ((RTC->ISR & RTC_ISR_INITS) != 0U)) {
        RCC_OscInitTypeDef rtc_clock = {0};
#if (MBED_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_LSE_OR_LSI) && MBED_CONF_TARGET_LSE_AVAILABLE
        rtc_clock.OscillatorType = RCC_OSCILLATORTYPE_LSE;
        rtc_clock.LSEState = RCC_LSE_ON;
#if MBED_CONF_TARGET_LSE_BYPASS
        rtc_clock.OscillatorType |= RCC_OSCILLATORTYPE_LSE_BYPASS;
        rtc_clock.LSEBYPASSState = RCC_LSE_BYPASS_ON;
#else
        rtc_clock.LSEBYPASSState = RCC_LSE_BYPASS_OFF;
#endif
#else
        rtc_clock.OscillatorType = RCC_OSCILLATORTYPE_LSI;
        rtc_clock.LSIState = RCC_LSI_ON;
#endif
        if (HAL_RCC_OscConfig(&rtc_clock) != HAL_OK) {
            error("RTC clock restore failed\n");
        }
    }
    initial_clock_setup_done = 1U;
}

static void configure_smps_clock(void)
{
    RCC_PeriphCLKInitTypeDef peripheral_clock = {0};

    peripheral_clock.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
    peripheral_clock.SmpsDivSelection = RCC_SMPSCLK_DIV4;
    if (HAL_RCCEx_PeriphCLKConfig(&peripheral_clock) != HAL_OK) {
        error("SMPS clock configuration failed\n");
    }
}

void SetSysClock(void)
{
    restore_rtc_clock();

#if ((CLOCK_SOURCE) & USE_RC64MPLL)
    // Prefer the 64 MHz HSE-assisted clock when an HSE crystal is fitted.
    if (SetSysClock_RC64MPLL() == 0U)
#endif
    {
#if ((CLOCK_SOURCE) & USE_DIRECT_HSE)
        // Fall back to the external 32 MHz crystal without the 64 MHz PLL.
        if (SetSysClock_DIRECT_HSE() == 0U)
#endif
        {
#if ((CLOCK_SOURCE) & USE_HSI)
            // The internal 64 MHz oscillator needs no external components.
            if (SetSysClock_HSI() == 0U)
#endif
            {
                error("SetSysClock failed\n");
            }
        }
    }

    configure_smps_clock();
}

#if ((CLOCK_SOURCE) & USE_RC64MPLL)
MBED_WEAK uint8_t SetSysClock_RC64MPLL(void)
{
    RCC_OscInitTypeDef oscillator = {0};
    RCC_ClkInitTypeDef clock = {0};

    oscillator.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    oscillator.HSEState = RCC_HSE_ON;
    if (HAL_RCC_OscConfig(&oscillator) != HAL_OK) {
        return 0U;
    }

    clock.SYSCLKSource = RCC_SYSCLKSOURCE_RC64MPLL;
    clock.SYSCLKDivider = RCC_RC64MPLL_DIV1;
    if (HAL_RCC_ClockConfig(&clock, FLASH_WAIT_STATES_1) != HAL_OK) {
        return 0U;
    }

    return 1U;
}
#endif

#if ((CLOCK_SOURCE) & USE_DIRECT_HSE)
MBED_WEAK uint8_t SetSysClock_DIRECT_HSE(void)
{
    RCC_OscInitTypeDef oscillator = {0};
    RCC_ClkInitTypeDef clock = {0};

    oscillator.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    oscillator.HSEState = RCC_HSE_ON;
    if (HAL_RCC_OscConfig(&oscillator) != HAL_OK) {
        return 0U;
    }

    clock.SYSCLKSource = RCC_SYSCLKSOURCE_DIRECT_HSE;
    clock.SYSCLKDivider = RCC_DIRECT_HSE_DIV1;
    if (HAL_RCC_ClockConfig(&clock, FLASH_WAIT_STATES_0) != HAL_OK) {
        return 0U;
    }

    return 1U;
}
#endif

#if ((CLOCK_SOURCE) & USE_HSI)
MBED_WEAK uint8_t SetSysClock_HSI(void)
{
    RCC_ClkInitTypeDef clock = {0};

    clock.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    clock.SYSCLKDivider = RCC_RC64MPLL_DIV1;
    if (HAL_RCC_ClockConfig(&clock, FLASH_WAIT_STATES_1) != HAL_OK) {
        return 0U;
    }

    return 1U;
}
#endif

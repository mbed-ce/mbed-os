/* SPDX-License-Identifier: Apache-2.0 */
#include "stm32wb0x.h"
#include "mbed_error.h"

void SetSysClock(void)
{
    RCC_ClkInitTypeDef clock = {0};
    RCC_PeriphCLKInitTypeDef peripheral = {0};

    // The RTC calendar is retained across reset, but its clocking may not be.
    // Restore it during early clock setup so a running calendar does not pause.
    __HAL_RCC_RTC_CLK_ENABLE();
#if MBED_CONF_TARGET_LSE_AVAILABLE
    if ((RTC->ISR & RTC_ISR_INITS) != 0U) {
        RCC_OscInitTypeDef rtc_clock = {0};
        rtc_clock.OscillatorType = RCC_OSCILLATORTYPE_LSE;
        rtc_clock.LSEState = RCC_LSE_ON;
        rtc_clock.LSEBYPASSState = RCC_LSE_BYPASS_OFF;
        if (HAL_RCC_OscConfig(&rtc_clock) != HAL_OK) {
            error("RTC clock restore failed\n");
        }
    }
#endif

    clock.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    clock.SYSCLKDivider = RCC_RC64MPLL_DIV1;
    if (HAL_RCC_ClockConfig(&clock, FLASH_WAIT_STATES_1) != HAL_OK) {
        error("SetSysClock failed\n");
    }

    peripheral.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
    peripheral.SmpsDivSelection = RCC_SMPSCLK_DIV4;
    if (HAL_RCCEx_PeriphCLKConfig(&peripheral) != HAL_OK) {
        error("SMPS clock configuration failed\n");
    }
}

/* SPDX-License-Identifier: Apache-2.0 */
#include "stm32wb0x.h"
#include "mbed_error.h"

void SetSysClock(void)
{
    RCC_ClkInitTypeDef clock = {0};
    RCC_PeriphCLKInitTypeDef peripheral = {0};

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

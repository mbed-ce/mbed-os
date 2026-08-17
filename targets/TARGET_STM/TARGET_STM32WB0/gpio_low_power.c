/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#include "gpio_low_power.h"
#include "cmsis.h"
#include "platform/mbed_toolchain.h"

/* Configure board-connected GPIOs for Deepstop. A custom target may override
 * this function with the pull configuration required by its hardware.
 * The PWR pulls affect the pads only while the APC bit is active in Deepstop.
 */
MBED_WEAK void gpio_deep_sleep_prepare(void)
{
#if defined(TARGET_NUCLEO_WB09KE)
    /* Configuration used by ST's NUCLEO-WB09KE BLE power-consumption example. */
    HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_A,
                              PWR_GPIO_BIT_0 | PWR_GPIO_BIT_1 |
                              PWR_GPIO_BIT_2 | PWR_GPIO_BIT_3);
    HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_A,
                                PWR_GPIO_BIT_8 | PWR_GPIO_BIT_9 |
                                PWR_GPIO_BIT_10 | PWR_GPIO_BIT_11);
    HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_B,
                                PWR_GPIO_BIT_0 | PWR_GPIO_BIT_3 |
                                PWR_GPIO_BIT_6 | PWR_GPIO_BIT_7);
    /* PB12/PB13 are OSC32_OUT/OSC32_IN on NUCLEO_WB09KE. Preserve them
     * whenever LSE is the selected RTC clock. Availability alone is not
     * sufficient: an application may select LSI on a board that has LSE.
     */
    if (__HAL_RCC_GET_RTC_WDG_BLEWKUP_CLK_CONFIG() ==
            RCC_RTC_WDG_BLEWKUP_CLKSOURCE_LSE) {
        HAL_PWREx_DisableGPIOPullUp(PWR_GPIO_B,
                                   PWR_GPIO_BIT_12 | PWR_GPIO_BIT_13);
        HAL_PWREx_DisableGPIOPullDown(PWR_GPIO_B,
                                     PWR_GPIO_BIT_12 | PWR_GPIO_BIT_13);
    } else {
        HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_B,
                                    PWR_GPIO_BIT_12 | PWR_GPIO_BIT_13);
    }
    HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_B,
                              PWR_GPIO_BIT_1 | PWR_GPIO_BIT_2 |
                              PWR_GPIO_BIT_4 | PWR_GPIO_BIT_5 |
                              PWR_GPIO_BIT_14 | PWR_GPIO_BIT_15);
    HAL_PWREx_EnablePullUpPullDownConfig();
#endif
}

/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MBED_STM32WB0_GPIO_LOW_POWER_H
#define MBED_STM32WB0_GPIO_LOW_POWER_H

#ifdef __cplusplus
extern "C" {
#endif

/** Configure board-connected GPIOs before entering Deepstop.
 *
 * A custom target may override the weak implementation to configure the PWR
 * pull registers for its own external circuitry.
 */
void gpio_deep_sleep_prepare(void);

#ifdef __cplusplus
}
#endif

#endif

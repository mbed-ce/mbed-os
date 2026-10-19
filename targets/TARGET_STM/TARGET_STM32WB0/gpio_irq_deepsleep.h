/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MBED_GPIO_IRQ_DEEPSLEEP_H
#define MBED_GPIO_IRQ_DEEPSLEEP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Prepare GPIO IRQ before Deepstop for next wakeup. */
void gpio_irq_prepare_deepsleep(void);

/** Dispatch callbacks for GPIO pins reported by the PWR wake-up logic. */
void gpio_irq_dispatch_deepsleep_wakeup(uint32_t wakeup_sources);

#ifdef __cplusplus
}
#endif

#endif

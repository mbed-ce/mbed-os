/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MBED_PIN_DEVICE_H
#define MBED_PIN_DEVICE_H

#include "cmsis.h"
#include "stm32wb0x_ll_gpio.h"
#include "objects.h"

extern const uint32_t ll_pin_defines[16];

static inline void stm_pin_DisconnectDebug(PinName pin)
{
    (void) pin;
}

static inline void stm_pin_PullConfig(GPIO_TypeDef *gpio, uint32_t ll_pin, uint32_t pull_config)
{
    switch (pull_config) {
        case GPIO_PULLUP:
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_UP);
            break;
        case GPIO_PULLDOWN:
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_DOWN);
            break;
        default:
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_NO);
            break;
    }
}

static inline void stm_pin_SetAFPin(GPIO_TypeDef *gpio, PinName pin, uint32_t afnum)
{
    uint32_t ll_pin = ll_pin_defines[STM_PIN(pin)];
    if (STM_PIN(pin) > 7) {
        LL_GPIO_SetAFPin_8_15(gpio, ll_pin, afnum);
    } else {
        LL_GPIO_SetAFPin_0_7(gpio, ll_pin, afnum);
    }
}

#endif

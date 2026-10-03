/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "mbed_assert.h"
#include "PeripheralPins.h"
#include "mbed_error.h"
#include "fsl_clock.h"

#include "pinmap_device.h"

// Only some MIMXRT-series chips have keeper functionality
#ifdef IOMUXC_SW_PAD_CTL_PAD_PKE_MASK
#define HAS_KEEPER 1
#else
#define HAS_KEEPER 0
#endif

void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t volatile * muxregister = gpio_port_to_iomux_sw_mux_ctl[(pin >> GPIO_PORT_SHIFT) - 1];
    uint32_t daisyregister;

    CLOCK_EnableClock(kCLOCK_Iomuxc);

    /* Get mux register address */
    if (muxregister == 0) {
        muxregister = get_iomux_sw_mux_ctl(pin);
    } else {
        muxregister = muxregister + ((pin & 0xFF) * 4);
    }

    /* Write to the mux register */
    *muxregister = IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(function) |
                                          IOMUXC_SW_MUX_CTL_PAD_SION((function >> SION_BIT_SHIFT) & 0x1);

    /* If required write to the input daisy register */
    daisyregister = (function >> DAISY_REG_SHIFT) & 0xFFF;
    if (daisyregister != 0) {
        daisyregister = daisyregister + IOMUXC_BASE;
        *((volatile uint32_t *)daisyregister) = IOMUXC_SELECT_INPUT_DAISY(((function >> DAISY_REG_VALUE_SHIFT) & 0xF));
    }
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t gpio_number = pin >> GPIO_PORT_SHIFT;
    uint32_t reg;
    uint32_t * volatile muxregister = gpio_port_to_iomux_sw_mux_ctl[gpio_number - 1];

    if (muxregister == 0) {
        muxregister = get_iomux_sw_mux_ctl(pin);
    } else {
        muxregister = muxregister + ((pin & 0xFF) * 4);
    }

    /* Get pad register address */
    uint32_t * volatile configregister = muxregister + mux_ctl_to_pad_ctl_offset[gpio_number - 1];

    reg = *configregister;
    switch (mode) {
        case PullNone:
            /* Write 0 to the PUE bit & 1 to the PKE bit to set the pad to keeper mode */
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
#if HAS_KEEPER
            reg |= (IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
#endif
            break;
        case PullDown:
            /* Write 1 to PKE & PUE bit to enable the pull configuration and 0 to PUS bit for 100K pull down */
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUE_MASK;
#if HAS_KEEPER
            reg |= IOMUXC_SW_PAD_CTL_PAD_PKE_MASK;
#endif
            break;
        case PullUp_47K:
            /* Write 1 to PKE & PUE bit to enable the pull configuration and 1 to PUS bit for 47K pull up*/
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUS(1);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUE_MASK;
#if HAS_KEEPER
            reg |= IOMUXC_SW_PAD_CTL_PAD_PKE_MASK;
#endif
            break;
        case PullUp_100K:
            /* Write 1 to PKE & PUE bit to enable the pull configuration and 2 to PUS bit for 100K pull up*/
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUS(2);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUE_MASK;
#if HAS_KEEPER
            reg |= IOMUXC_SW_PAD_CTL_PAD_PKE_MASK;
#endif
            break;
        case PullUp_22K:
            /* Write 1 to PKE & PUE bit to enable the pull configuration and 3 to PUS bit for 22K pull up*/
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUS(3);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUE_MASK;
#if HAS_KEEPER
            reg |= IOMUXC_SW_PAD_CTL_PAD_PKE_MASK;
#endif
            break;
        default:
            break;
    }

    /* Below settings for DSE and SPEED fields per test results */
    reg = (reg & ~(IOMUXC_SW_PAD_CTL_PAD_DSE_MASK | IOMUXC_SW_PAD_CTL_PAD_SPEED_MASK)) |
           IOMUXC_SW_PAD_CTL_PAD_DSE(6) | IOMUXC_SW_PAD_CTL_PAD_SPEED(2);

    reg = (reg & ~IOMUXC_SW_PAD_CTL_PAD_DSE_MASK) | IOMUXC_SW_PAD_CTL_PAD_DSE(6);
#ifdef IOMUXC_SW_PAD_CTL_PAD_SPEED
    reg = (reg & ~IOMUXC_SW_PAD_CTL_PAD_SPEED_MASK) | IOMUXC_SW_PAD_CTL_PAD_SPEED(2);
#endif

    /* Write value to the pad register */
    *configregister = reg;
}

void pin_mode_opendrain(PinName pin, bool enable)
{
    MBED_ASSERT(pin != (PinName)NC);

    uint32_t gpio_number = pin >> GPIO_PORT_SHIFT;
    uint32_t volatile * muxregister = gpio_port_to_iomux_sw_mux_ctl[gpio_number - 1];

    if (muxregister == 0) {
        muxregister = get_iomux_sw_mux_ctl(pin);
    } else {
        muxregister = muxregister + ((pin & 0xFF) * 4);
    }

    /* Get pad register address */
    uint32_t * volatile configregister = muxregister + mux_ctl_to_pad_ctl_offset[gpio_number - 1];

    if (enable) {
        *configregister |= IOMUXC_SW_PAD_CTL_PAD_ODE_MASK;
    } else {
        *configregister &= ~IOMUXC_SW_PAD_CTL_PAD_ODE_MASK;
    }
}


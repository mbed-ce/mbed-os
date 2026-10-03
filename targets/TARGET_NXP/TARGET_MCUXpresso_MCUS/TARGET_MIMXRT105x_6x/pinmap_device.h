/* mbed Microcontroller Library
* Copyright (c) 2026 Jamie Smith
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

#pragma once

/* Array of IOMUX base address. */
static uint32_t volatile * const gpio_port_to_iomux_sw_mux_ctl[FSL_FEATURE_SOC_IGPIO_COUNT] = {
    &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_00],
    &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_00],
    0u, // Multiple possibilities
    &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_00],
    &IOMUXC_SNVS->SW_MUX_CTL_PAD_WAKEUP
};

// Offset from SW_MUX_CTL_PAD_xxx to SW_PAD_CTL_PAD_xxx
static const size_t mux_ctl_to_pad_ctl_offset [FSL_FEATURE_SOC_IGPIO_COUNT] = {
    0x1F0,
    0x1F0,
    0x1F0,
    0x1F0,
    0x10
};

/* If the entry is 0 in the above array this function is called to get the mux ctl register address */
static inline uint32_t volatile * get_iomux_sw_mux_ctl(PinName pin)
{
    int32_t gpio_pin = pin & 0xFF;

    if ((gpio_pin >= 0) && (gpio_pin < 12)) {
        uint32_t volatile * const base_addr = &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B1_00];
        return base_addr + (gpio_pin * 4);
    } else if ((gpio_pin >= 12) && (gpio_pin < 18)) {
        uint32_t volatile * const base_addr = &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B0_00];
        return base_addr + ((gpio_pin - 12) * 4);
    } else /* ((gpio_pin >= 18) && (gpio_pin < 28)) */ {
        uint32_t volatile * const base_addr = &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_32];
        return base_addr + ((gpio_pin - 18) * 4);
    }
}
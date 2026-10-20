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

#include "stddef.h"
#include "fsl_device_registers.h"

/* Array of IOMUX base address. */
static uint32_t volatile * const gpio_port_to_iomux_sw_mux_ctl[FSL_FEATURE_SOC_IGPIO_COUNT] = {
    &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_00],
    &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_32],
    &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_01],
    &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_33],
    &IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B1_11],
    &IOMUXC_LPSR->SW_MUX_CTL_PAD[kIOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO_LPSR_00],
    // Next 6 entries unused, mirrors of the lower 6 GPIO ports
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    0u, // Multiple possibilities
    &IOMUXC_SNVS->SW_MUX_CTL_PAD_WAKEUP_DIG
};

// Offset from SW_MUX_CTL_PAD_xxx to SW_PAD_CTL_PAD_xxx
static const size_t mux_ctl_to_pad_ctl_offset [FSL_FEATURE_SOC_IGPIO_COUNT] = {
    // Main IOMUXC
    0x244,
    0x244,
    0x244,
    0x244,
    0x244,

    // LPSR IOMUXC
    0x40,

    // Unused, mirrors
    0,
    0,
    0,
    0,
    0,
    0,

    // SNVS IOMUXC
    0x40
};

/* If the entry is 0 in the above array this function is called to get the mux ctl register address */
static inline uint32_t volatile * get_iomux_sw_mux_ctl(PinName pin)
{
    // Should never be called.
    return NULL;
}
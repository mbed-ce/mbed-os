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
#include "gpio_api.h"

#include "fsl_rtc.h"
#include "fsl_clock_config.h"
#include "rtc_api.h"

// called before main
void mbed_sdk_init()
{
    BOARD_BootClockRUN();
#ifdef LPUART0
    CLOCK_SetLpuartClock(2U); // Select OSCERCLK for LPUART clock source
#endif

    // Ensure RTC oscillator is enabled (as this also drives LP ticker)
    rtc_init();
}

// Change the NMI pin to an input. This allows NMI pin to
//  be used as a low power mode wakeup.  The application will
//  need to change the pin back to NMI_b or wakeup only occurs once!
void NMI_Handler(void)
{
    gpio_t gpio;
    gpio_init_in(&gpio, PTA4);
}

uint32_t pit_get_clock() {
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

#if DEVICE_QSPI
// Get the QSPI clock frequency
uint32_t qspi_get_freq(void)
{
    return CLOCK_GetFreq(kCLOCK_McgPll0Clk);
}
#endif
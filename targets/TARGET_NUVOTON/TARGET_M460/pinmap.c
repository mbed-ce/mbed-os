/*
 * Copyright (c) 2022, Nuvoton Technology Corporation
 *
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
#include "pinmap.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "mbed_error.h"

/**
 * Configure pin multi-function
 */
void pin_function(PinName pin, int data)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);
    __IO uint32_t *GPx_MFPx = ((__IO uint32_t *) &SYS->GPA_MFP0) + port_index * 4 + (pin_index / 4);
    uint32_t MFP_Msk = NU_MFP_MSK(pin_index);

    // E.g.: SYS->GPA_MFP0  = (SYS->GPA_MFP0 & (~SYS_GPA_MFP0_PA0MFP_Msk) ) | SYS_GPA_MFP0_PA0MFP_SC0_CLK  ;
    *GPx_MFPx  = (*GPx_MFPx & (~MFP_Msk)) | data;
}

/**
 * Configure pin pull-up/pull-down
 */
void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);

    uint32_t mode_intern = GPIO_MODE_INPUT;

    switch (mode) {
        case InputOnly:
            mode_intern = GPIO_MODE_INPUT;
            break;

        case PushPullOutput:
            mode_intern = GPIO_MODE_OUTPUT;
            break;

        case OpenDrain:
            mode_intern = GPIO_MODE_OPEN_DRAIN;
            break;

        case QuasiBidirectional:
            mode_intern = GPIO_MODE_QUASI;
            break;

        default:
            /* H/W doesn't support separate configuration for input pull mode/direction.
             * We expect upper layer would have translated input pull mode/direction
             * to I/O mode */
            return;
    }

    GPIO_SetMode(gpio_base, 1 << pin_index, mode_intern);

    /* Invalid combinations of PinMode/PinDirection
     *
     * We assume developer would avoid the following combinations of PinMode/PinDirection
     * which are invalid:
     * 1. InputOnly/PIN_OUTPUT
     * 2. PushPullOutput/PIN_INPUT
     */
}

/* List of UART peripherals excluded from testing */
#if DEVICE_SERIAL
const PeripheralList *pinmap_uart_restricted_peripherals()
{
    static const int peripherals[] = {
        USB_UART,       // Dedicated to USB VCOM
    };

    static const PeripheralList peripheral_list = {
        sizeof peripherals / sizeof peripherals[0],
        peripherals
    };
    return &peripheral_list;
}
#endif

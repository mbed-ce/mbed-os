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
#include "pinmap.h"
#include "mbed_error.h"
#include "fsl_common.h"
#include "PeripheralNames.h"

void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);
    clock_ip_name_t gpio_clocks[] = GPIO_CLOCKS;
    uint32_t pin_number = pin & 0x1F;
    uint8_t port_number = pin / 32;
    uint32_t reg;

    CLOCK_EnableClock(gpio_clocks[port_number]);
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* Set the DIGIMODE bit */
    IOCON->PIO[port_number][pin_number] |= IOCON_PIO_DIGIMODE_MASK;

    reg = IOCON->PIO[port_number][pin_number];
    reg = (reg & ~IOCON_PIO_FUNC_MASK) | (function & MBED_LPC_PIN_FUNCTION_FUNC_Msk);
    IOCON->PIO[port_number][pin_number] = reg;
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_number = pin & 0x1F;
    uint8_t port_number = pin / 32;
    uint32_t reg = IOCON->PIO[port_number][pin_number] & ~(IOCON_PIO_MODE_MASK | IOCON_PIO_OD_MASK);

    // The "type I" fast-mode I2C pins have totally different bitfields available and do not support
    // a lot of standard options
#if TARGET_MCU_LPC546XX
    const bool is_type_i_pin = (pin == P0_13 || pin == P0_14 || pin == P3_23 || pin == P3_24);
#else
#error "Don't know which pins are I-type pins."
#endif

    if(mode & PullDown) {
        if(is_type_i_pin) {
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_INVALID_OPERATION), "Cannot enable pulldown on Type-I GPIO pin", pin);
        }

        reg |= IOCON_PIO_MODE(1);
    }
    else if(mode & PullUp) {
        if(is_type_i_pin) {
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_INVALID_OPERATION), "Cannot enable pullup on Type-I GPIO pin", pin);
        }

        reg |= IOCON_PIO_MODE(2);
    }

    if(mode & OpenDrain) {
        if(is_type_i_pin) {
            MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_INVALID_OPERATION), "Cannot enable open drain  on Type-I GPIO pin", pin);
        }

        reg |= IOCON_PIO_OD_MASK;
    }

    IOCON->PIO[port_number][pin_number] = reg;
}

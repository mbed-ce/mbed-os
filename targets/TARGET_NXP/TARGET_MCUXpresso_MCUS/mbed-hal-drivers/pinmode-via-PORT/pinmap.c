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
#include "fsl_port.h"

/* Array of PORT peripheral base address. */
static PORT_Type *const port_addrs[] = PORT_BASE_PTRS;

const size_t num_ports = sizeof(port_addrs) / sizeof(port_addrs[0]);

static void assert_valid_pin(PinName pin) {
    // If someone passes an invalid PinName (e.g. DAC0_OUT), this would result in a crash / undefined behavior
    // due to accessing past the end of port_addrs. Detect this and bail.
    if ((size_t)(pin >> GPIO_PORT_SHIFT) >= num_ports) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_GPIO, MBED_ERROR_CODE_INVALID_ARGUMENT), "Invalid PinName passed to pin_function()/pin_mode()!", pin);
    }
}

void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);
    assert_valid_pin(pin);
    clock_ip_name_t port_clocks[] = PORT_CLOCKS;
    CLOCK_EnableClock(port_clocks[pin >> GPIO_PORT_SHIFT]);

    PORT_SetPinMux(port_addrs[pin >> GPIO_PORT_SHIFT], pin & 0xFF, (port_mux_t)function);
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    assert_valid_pin(pin);
    uint32_t instance = pin >> GPIO_PORT_SHIFT;
    uint32_t pinName = pin & 0xFF;
    PORT_Type *base = port_addrs[instance];
    uint32_t reg = base->PCR[pinName];

    // Clear interrupt (for now)
    reg &= ~(PORT_PCR_ISF_MASK);

    if (mode & PullUp) {
        /* Write 1 to the PE & PS bits to enable the pull up configuration */
        reg |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
    }
    else if (mode & PullDown) {
        /* Write 0 to PS bit and 1 to the PE bit to enable the pull configuration */
        reg &= ~PORT_PCR_PS_MASK;
        reg |= PORT_PCR_PE_MASK;
    }
    else {
        /* Write 0 to the PE and PS bits */
        reg &= ~(PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
    }

    // Enable open-drain mode if requested and supported
#ifdef PORT_PCR_ODE_MASK
    if (mode & OpenDrain) {
        reg |= PORT_PCR_ODE_MASK;
    }
#endif

    base->PCR[pinName] = reg;
}

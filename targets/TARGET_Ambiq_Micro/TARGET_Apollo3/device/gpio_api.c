/*
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "mbed_assert.h"
#include "gpio_api.h"
#include "PeripheralPins.h"

/** Set the given pin as GPIO
 *
 * @param pin The pin to be set as GPIO
 * @return The GPIO port mask for this pin
 **/
uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    return (uint32_t)AM_HAL_GPIO_BIT(pin);
}

/** Checks if gpio object is connected (pin was not initialized with NC)
 * @param obj The GPIO object
 * @return 0 if object was initialized with NC
 * @return non-zero if object was initialized with a valid PinName
 **/
int gpio_is_connected(const gpio_t *obj)
{
    MBED_ASSERT(obj != NULL);
    return (int)(((PinName)obj->pad == (PinName)NC) ? 0 : 1);
}

/** Initialize the GPIO pin
 *
 * @param obj The GPIO object to initialize
 * @param pin The GPIO pin to initialize (may be NC)
 */
void gpio_init(gpio_t *obj, PinName pin)
{
    MBED_ASSERT(obj != NULL);
    obj->pad = (ap3_uart_pad_t)pin;
    return;
}
/** Set the input pin mode
 *
 * @param obj  The GPIO object (must be connected)
 * @param mode The pin mode to be set
 */
void gpio_mode(gpio_t *obj, PinMode mode)
{
    MBED_ASSERT(gpio_is_connected(obj));
    am_hal_gpio_pincfg_allow_t pinConfigBools = {0};

    obj->cfg.uFuncSel = AP3_PINCFG_FUNCSEL_GPIO; // gpio

    // Configure pull-up or pull-down
    pinConfigBools.ePullup = true;

    if(!(mode & OpenDrain) && obj->isOutput) {
        // Push-pull output, do not allow pullup
        obj->cfg.ePullup = AM_HAL_GPIO_PIN_PULLUP_NONE;
    }
    else if (mode & (PinMode)PullUp) {
        MBED_ASSERT(obj->pad != IO_20); // pullup not supported on IO 20

        if(obj->pad == IO_0 || obj->pad == IO_1 || obj->pad == IO_5 || obj->pad == IO_6 || obj->pad == IO_8 
            || obj->pad == IO_9 || obj->pad == IO_25 || obj->pad == IO_27 || obj->pad == IO_39 || 
            obj->pad == IO_40 || obj->pad == IO_42 || obj->pad == IO_43 || obj->pad == IO_48 || obj->pad == IO_49) {
            // These pads (with I2C support) need a different constant to get the same resistance
            obj->cfg.ePullup = AM_HAL_GPIO_PIN_PULLUP_24K;
        }
        else{
            obj->cfg.ePullup = AM_HAL_GPIO_PIN_PULLUP_WEAK;
        }
    }
    else if (mode & (PinMode)PullDown) {
        MBED_ASSERT(obj->pad == IO_20); // pulldown only supported on IO 20
        obj->cfg.ePullup = AM_HAL_GPIO_PIN_PULLDOWN;
    }
    else {
        obj->cfg.ePullup = AM_HAL_GPIO_PIN_PULLUP_NONE;
    }

    // Configure output type
    obj->openDrain = mode & OpenDrain;
    pinConfigBools.eGPOutcfg = true;
    if(obj->isOutput) {
        obj->cfg.eGPOutcfg = obj->openDrain ? AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN : AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL;
    }
    else {
        obj->cfg.eGPOutcfg = AM_HAL_GPIO_PIN_OUTCFG_DISABLE;
    }


    MBED_CHECK_AM_HAL_CALL(ap3_hal_gpio_pinconfig_partial((uint32_t)(obj->pad), obj->cfg, pinConfigBools));
}

/** Set the pin direction
 *
 * @param obj       The GPIO object (must be connected)
 * @param direction The pin direction to be set
 */
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(gpio_is_connected(obj));
    MBED_ASSERT(direction < (PinDirection)PIN_DIR_ELEMENTS);
    am_hal_gpio_pincfg_allow_t pinConfigBools= {0};

    // Always enable the input on the pin, so that we can read it if it is open drain.
    pinConfigBools.eGPInput = true;
    pinConfigBools.eGPRdZero = true;
    obj->cfg.eGPInput = AM_HAL_GPIO_PIN_INPUT_ENABLE;
    obj->cfg.eGPRdZero = AM_HAL_GPIO_PIN_RDZERO_READPIN;

    if (direction == (PinDirection)PIN_INPUT) {
        obj->isOutput = false;
        obj->cfg.eGPOutcfg = AM_HAL_GPIO_PIN_OUTCFG_DISABLE;
        pinConfigBools.eGPOutcfg = true;
    } else if (direction == (PinDirection)PIN_OUTPUT) {
        obj->isOutput = true;
        obj->cfg.eGPOutcfg = obj->openDrain ? AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN : AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL;
        pinConfigBools.eGPOutcfg = true;
        obj->cfg.eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA;
        pinConfigBools.eDriveStrength = true;

        // Clear any configured pullup if set to open-drain
        if(!obj->openDrain) {
            pinConfigBools.ePullup = true;
            obj->cfg.ePullup = AM_HAL_GPIO_PIN_PULLUP_NONE;
        }
    } else {
        MBED_ASSERT(false);
    }

    ap3_hal_gpio_pinconfig_partial((uint32_t)(obj->pad), obj->cfg, pinConfigBools);
}

/** Set the output value
 *
 * @param obj   The GPIO object (must be connected)
 * @param value The value to be set
 */
void gpio_write(gpio_t *obj, int value)
{
    MBED_ASSERT(gpio_is_connected(obj));
    (value) ? am_hal_gpio_output_set(obj->pad) : am_hal_gpio_output_clear(obj->pad);
}

/** Read the input value
 *
 * @param obj The GPIO object (must be connected)
 * @return An integer value 1 or 0
 */
int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(gpio_is_connected(obj));

    return am_hal_gpio_input_read(obj->pad);
}

/** Get the pins that support all GPIO tests
 *
 * Return a PinMap array of pins that support GPIO. The
 * array is terminated with {NC, NC, 0}.
 *
 * Targets should override the weak implementation of this
 * function to provide the actual pinmap for GPIO testing.
 *
 * @return PinMap array
 */
const PinMap *gpio_pinmap(void)
{
    return PinMap_GPIO;
}

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

#ifndef MBED_OBJECTS_GPIO_H
#define MBED_OBJECTS_GPIO_H

#include "am_hal_gpio.h"
#include "PinNames.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t ap3_gpio_pad_t;

typedef enum {
    PIN_INPUT = 0x00,
    PIN_OUTPUT,

    PIN_DIR_ELEMENTS
} PinDirection;

typedef enum {

    PullNone = 0,

    // Supported on all IOs except IO 20. Actual resistance 13-27kOhm
    PullUp = 1 << 0,

    // Only supported on IO 20, Actual resistance 26-40kOhm
    PullDown = 1 << 1,

    OpenDrain = 1 << 2,
    OpenDrainPullUp = OpenDrain | PullUp, ///< Open-drain mode with pull up. Supported on all IOs except IO 20.
    OpenDrainPullDown = OpenDrain | PullDown, ///< Open-drain mode with pull down. Only supported on IO 20.
    OpenDrainNoPull = OpenDrain, ///< Open-drain mode with no pullup/pulldown. Supported on all IOs.

    PullDefault = PullNone
} PinMode;

typedef struct _gpio_t {
    ap3_gpio_pad_t pad;
    am_hal_gpio_pincfg_t cfg;
    bool openDrain; ///< Whether the pin is configured open drain as of the last gpio_mode() call
    bool isOutput; ///< Whether the pin is configured as an output as of the last gpio_dir() call
} gpio_t;

typedef struct ap3_gpio_irq_control_t {
    ap3_gpio_pad_t pad;
    uint32_t id;
    void *handler;
    uint8_t events;
} ap3_gpio_irq_control_t;

typedef struct gpio_irq_s {
    ap3_gpio_irq_control_t *control;
    bool irq_requested_enabled;
} gpio_irq_s;

#define AP3_PINCFG_FUNCSEL_GPIO 3

#define AP3_GPIO_MAX_PADS (50)
#define PADREG_FLD_76_S 6
#define PADREG_FLD_FNSEL_S 3
#define PADREG_FLD_DRVSTR_S 2
#define PADREG_FLD_INPEN_S 1
#define PADREG_FLD_PULLUP_S 0

#define GPIOCFG_FLD_INTD_S 3
#define GPIOCFG_FLD_OUTCFG_S 1
#define GPIOCFG_FLD_INCFG_S 0

#ifdef __cplusplus
}
#endif

#endif // MBED_OBJECTS_GPIO_H

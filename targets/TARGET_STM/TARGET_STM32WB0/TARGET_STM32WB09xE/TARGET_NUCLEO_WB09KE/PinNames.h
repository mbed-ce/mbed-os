/* SPDX-License-Identifier: Apache-2.0 */
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#define GPIO_PINMAP_READY 1

typedef enum {
    PA_0 = 0x00, PA_1 = 0x01, PA_2 = 0x02, PA_3 = 0x03,
    PA_8 = 0x08, PA_9 = 0x09, PA_10 = 0x0A, PA_11 = 0x0B,
    PB_0 = 0x10, PB_1 = 0x11, PB_2 = 0x12, PB_3 = 0x13,
    PB_4 = 0x14, PB_5 = 0x15, PB_6 = 0x16, PB_7 = 0x17,
    PB_12 = 0x1C, PB_13 = 0x1D, PB_14 = 0x1E, PB_15 = 0x1F,
    NC = (int) 0xFFFFFFFF,

    CONSOLE_TX = PA_1,
    CONSOLE_RX = PB_0,

    LED1 = PB_1,
    LED2 = PB_4,
    LED3 = PB_2,
    BUTTON1 = PA_0,
    BUTTON2 = PB_5,
    BUTTON3 = PB_14,

    DEBUG_SWDIO = PA_2,
    DEBUG_SWCLK = PA_3,
    RCC_OSC32_IN = PB_13,
    RCC_OSC32_OUT = PB_12
} PinName;

#endif

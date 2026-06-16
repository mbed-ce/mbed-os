/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#define RA_PIN_MODE_ANALOG (0x0U)
#define RA_PIN_MODE_PERIPHERAL_PP (0x1U)
#define RA_PIN_MODE_PERIPHERAL_OD (0x2U)
#define RA_PIN_MODE_IRQ (0x3U)

#define RA_PIN_PULL_NONE        (0x0U)
#define RA_PIN_PULL_UP          (0x1U)
#define RA_PIN_PULL_DOWN        (0x2U)

#define RA_PIN_DATA(MODE, PULL, ALT) \
    (((MODE) << 28) | \
     ((PULL) << 24) | \
     ((ALT >> 24)  << 16))

#define RA_PIN_DATA_EXT(MODE, PULL, ALT, CHANNEL) \
    (((MODE)    << 28) | \
     ((PULL)    << 24) | \
     ((ALT >> 24)     << 16) | \
     ((CHANNEL) << 8))

#define RA_PIN_MODE(FUNC)      (((FUNC) >> 28) & 0xF)
#define RA_PIN_PULL(FUNC)      (((FUNC) >> 24) & 0xF)
#define RA_PIN_ALT(FUNC)       (((FUNC) >> 16) & 0xFF)
#define RA_PIN_CHANNEL(FUNC)   (((FUNC) >> 8 ) & 0xFF)

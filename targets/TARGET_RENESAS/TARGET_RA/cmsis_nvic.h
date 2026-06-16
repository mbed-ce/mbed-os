/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CMSIS_NVIC_H
#define CMSIS_NVIC_H

#include <stdint.h>

#include "vector_data.h"   // FSP generated

/* Cortex-M has 16 system exceptions */
#define NVIC_NUM_SYSTEM_EXCEPTIONS   (16)

/* Total number of vectors = system exceptions + IRQs */
#define NVIC_NUM_VECTORS  (NVIC_NUM_SYSTEM_EXCEPTIONS + VECTOR_DATA_IRQ_COUNT)

#endif

/* SPDX-License-Identifier: Apache-2.0 */
#ifndef MBED_CMSIS_H
#define MBED_CMSIS_H

#include "stm32wb0x.h"

extern uint32_t __vector_ram_start__;
extern uint32_t __vector_table_size__;

#define NVIC_NUM_VECTORS ((uint32_t)(uintptr_t)&__vector_table_size__) / sizeof(uint32_t)
#define NVIC_RAM_VECTOR_ADDRESS (uint32_t *)&__vector_ram_start__

#endif

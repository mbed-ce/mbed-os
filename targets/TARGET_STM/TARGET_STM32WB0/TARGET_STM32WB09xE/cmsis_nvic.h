/* SPDX-License-Identifier: Apache-2.0 */
#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#define NVIC_NUM_VECTORS 48
/* 0x000-0x0ff is reserved by the WB0 ROM/runtime RAM preamble. */
#define NVIC_RAM_VECTOR_ADDRESS (MBED_RAM_BANK_SRAM1_START + 0x100)

#endif

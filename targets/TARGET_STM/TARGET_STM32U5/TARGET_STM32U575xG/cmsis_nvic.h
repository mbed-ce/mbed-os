/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016-2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
*/

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#if !defined(MBED_CONFIGURED_ROM_BANK_FLASH_START)
#define MBED_CONFIGURED_ROM_BANK_FLASH_START  0x8000000
#endif

#if !defined(MBED_CONFIGURED_ROM_BANK_FLASH_SIZE)
#define MBED_CONFIGURED_ROM_BANK_FLASH_SIZE  0x100000 // 1MB
#endif

#if !defined(MBED_RAM_BANK_SRAM1_2_START)
#define MBED_RAM_BANK_SRAM1_2_START  0x20000000
#endif

#if !defined(MBED_RAM_BANK_SRAM1_2_SIZE)
#define MBED_RAM_BANK_SRAM1_2_SIZE  0x40000 // 256K
#endif

#if !defined(MBED_RAM_BANK_SRAM_3_SIZE)
#define MBED_RAM_BANK_SRAM_3_SIZE  0x80000 // 512K
#endif

#define NVIC_NUM_VECTORS        142
#define NVIC_RAM_VECTOR_ADDRESS MBED_RAM_BANK_SRAM1_2_START

#endif

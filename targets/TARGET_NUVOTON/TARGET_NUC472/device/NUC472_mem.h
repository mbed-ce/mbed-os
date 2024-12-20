/*
 * Copyright (c) 2022, Nuvoton Technology Corporation
 *
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

#ifndef __NUC472_MEM_H__
#define __NUC472_MEM_H__

/* Default memory specification
 *
 * Flash size:  512KiB
 * SRAM size:   64KiB
 */

/* Physical IROM1 start/size */
#if !defined(MBED_ROM_BANK_IROM1_START)
#define MBED_ROM_BANK_IROM1_START 0x0
#endif
#if !defined(MBED_ROM_BANK_IROM1_SIZE)
#define MBED_ROM_BANK_IROM1_SIZE 0x80000
#endif 

/* Physical IRAM1 start/size */
#if !defined(MBED_RAM_BANK_IRAM1_START)
#define MBED_RAM_BANK_IRAM1_START 0x20000000
#endif
#if !defined(MBED_RAM_BANK_IRAM1_SIZE)
#define MBED_RAM_BANK_IRAM1_SIZE 0x10000
#endif

/* Configured IROM1 start/size */
#if !defined(MBED_CONFIGURED_ROM_BANK_IROM1_START)
#define MBED_CONFIGURED_ROM_BANK_IROM1_START MBED_ROM_BANK_IROM1_START
#endif
#if !defined(MBED_CONFIGURED_ROM_BANK_IROM1_SIZE)
#define MBED_CONFIGURED_ROM_BANK_IROM1_SIZE MBED_ROM_BANK_IROM1_SIZE
#endif 

/* Configured IRAM1 start/size */
#if !defined(MBED_CONFIGURED_RAM_BANK_IRAM1_START)
#define MBED_CONFIGURED_RAM_BANK_IRAM1_START MBED_RAM_BANK_IRAM1_START
#endif
#if !defined(MBED_CONFIGURED_RAM_BANK_IRAM1_SIZE)
#define MBED_CONFIGURED_RAM_BANK_IRAM1_SIZE MBED_RAM_BANK_IRAM1_SIZE
#endif

#if defined(MBED_CONFIGURED_RAM_BANK_EBIRAM1_START) &&  \
    (MBED_CONFIGURED_RAM_BANK_EBIRAM1_SIZE != 0x0)
#define NU_HAVE_EBIRAM1     1
#else
#define NU_HAVE_EBIRAM1     0
#endif

#if NU_HAVE_EBIRAM1 && MBED_CONF_TARGET_EBI_ENABLE
#define NU_ENABLE_EBIRAM1   1
#else
#define NU_ENABLE_EBIRAM1   0
#endif

#endif  /* __NUC472_MEM_H__ */

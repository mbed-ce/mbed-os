/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef FLASH_DEVICE_H
#define FLASH_DEVICE_H

/* RA4T1: 256KB Code Flash, 4KB Data Flash */
#ifndef RA_CF_START
#define RA_CF_START      (0x00000000u)
#endif
#ifndef RA_CF_SIZE
#define RA_CF_SIZE       (256u * 1024u)
#endif
#ifndef RA_CF_WRITE_SIZE
#define RA_CF_WRITE_SIZE  (128u)
#endif

#ifndef RA_DF_START
#define RA_DF_START      (0x08000000u)
#endif
#ifndef RA_DF_SIZE
#define RA_DF_SIZE       (4u * 1024u)
#endif
#ifndef RA_DF_SECTOR_SIZE
#define RA_DF_SECTOR_SIZE (64u)
#endif
#ifndef RA_DF_WRITE_SIZE
#define RA_DF_WRITE_SIZE  (4u)
#endif

#endif

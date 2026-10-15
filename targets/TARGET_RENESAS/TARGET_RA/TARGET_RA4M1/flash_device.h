/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef FLASH_DEVICE_H
#define FLASH_DEVICE_H

/* RA4M1: 256KB Code Flash (LP controller, 2KB blocks, 8-byte CF writes),
 * 8KB Data Flash (1KB blocks, byte writes).
 * The first 16KB of code flash are reserved for the Arduino UNO R4
 * bootloader, the mbed visible flash starts at 0x4000. */
#ifndef RA_CF_START
#define RA_CF_START      (0x00004000u)
#endif
#ifndef RA_CF_SIZE
#define RA_CF_SIZE       (240u * 1024u)
#endif
#ifndef RA_CF_SECTOR_SIZE
#define RA_CF_SECTOR_SIZE (BSP_FEATURE_FLASH_LP_CF_BLOCK_SIZE)
#endif
#ifndef RA_CF_WRITE_SIZE
#define RA_CF_WRITE_SIZE  (BSP_FEATURE_FLASH_LP_CF_WRITE_SIZE)
#endif

#ifndef RA_DF_START
#define RA_DF_START      (0x40100000u)
#endif
#ifndef RA_DF_SIZE
#define RA_DF_SIZE       (8u * 1024u)
#endif
#ifndef RA_DF_SECTOR_SIZE
#define RA_DF_SECTOR_SIZE (BSP_FEATURE_FLASH_LP_DF_BLOCK_SIZE)
#endif
#ifndef RA_DF_WRITE_SIZE
#define RA_DF_WRITE_SIZE  (BSP_FEATURE_FLASH_LP_DF_WRITE_SIZE)
#endif

#endif

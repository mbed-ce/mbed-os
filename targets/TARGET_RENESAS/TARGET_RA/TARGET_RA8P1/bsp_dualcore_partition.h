/* mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Dual-core memory partition of the R7KA8P1KF (RA8P1).
 *
 * The RA8P1 combines a Cortex-M85 (CPU0, boot core) and a Cortex-M33 (CPU1,
 * held in reset out of POR). The on-chip memories are shared between the two
 * cores and have to be partitioned by software. The layout below matches the
 * official Renesas/RT-Thread sdk-bsp-ra8p1-titan-mini BSP (board/bsp_linker_info.h)
 * so that Mbed CE images interoperate with the vendor dual-core examples:
 *
 *   Code MRAM (1 MB @ 0x02000000):
 *     CPU0 (M85) [0x02000000, 0x020C0000)   768 KB
 *     CPU1 (M33) [0x020C0000, 0x02100000)   192 KB
 *   SRAM (0x22000000):
 *     CPU0 (M85) [0x22000000, 0x22174000)   1488 KB
 *     shared     [0x22174000, 0x22194000)   128 KB  (rpmsg-lite vring pool etc.)
 *     CPU1 (M33) [0x22194000, 0x221D4000)   256 KB
 *   ITCM/DTCM (0x00000000/0x20000000, 128 KB each): private to CPU0
 *   CTCM/STCM (0x00000000/0x20000000, 64 KB each):  private to CPU1
 *
 * The Mbed targets express the same partition through memory_bank_config
 * (MCU_RA8P1_CM85 / MCU_RA8P1_CM33 in targets.json5); these macros are the
 * BSP-facing view of it. They enable R_BSP_SecondaryCoreStart() (bsp_common.h)
 * which releases CPU1 from reset:
 *
 *     R_CPU_CTRL->CPU1INITVTOR = BSP_PARTITION_FLASH_CPU1_S_START;
 *     R_CPU_CTRL->CPU1WAITCR   = 0;
 *     R_CPU_CTRL->CPU1ACTCSR   = 0xA500 | 1;   // key + ACTREQ
 */

#ifndef BSP_DUALCORE_PARTITION_H_
#define BSP_DUALCORE_PARTITION_H_

/* Code flash (Code MRAM) */
#define BSP_PARTITION_FLASH_CPU0_S_START (0x02000000)
#define BSP_PARTITION_FLASH_CPU0_S_SIZE  (0xC0000)
#define BSP_PARTITION_FLASH_CPU1_S_START (0x020C0000)
#define BSP_PARTITION_FLASH_CPU1_S_SIZE  (0x30000)

/* SRAM */
#define BSP_PARTITION_RAM_CPU0_S_START   (0x22000000)
#define BSP_PARTITION_RAM_CPU0_S_SIZE    (0x174000)
#define BSP_PARTITION_SHARED_MEM_START   (0x22174000)
#define BSP_PARTITION_SHARED_MEM_SIZE    (0x20000)
#define BSP_PARTITION_RAM_CPU1_S_START   (0x22194000)
#define BSP_PARTITION_RAM_CPU1_S_SIZE    (0x40000)

/* Core-local TCMs (aliased at the same local address on both cores) */
#define BSP_PARTITION_ITCM_CPU0_S_START  (0x00000000)
#define BSP_PARTITION_ITCM_CPU0_S_SIZE   (0x20000)
#define BSP_PARTITION_DTCM_CPU0_S_START  (0x20000000)
#define BSP_PARTITION_DTCM_CPU0_S_SIZE   (0x20000)
#define BSP_PARTITION_CTCM_CPU1_S_START  (0x00000000)
#define BSP_PARTITION_CTCM_CPU1_S_SIZE   (0x10000)
#define BSP_PARTITION_STCM_CPU1_S_START  (0x20000000)
#define BSP_PARTITION_STCM_CPU1_S_SIZE   (0x10000)

#endif /* BSP_DUALCORE_PARTITION_H_ */

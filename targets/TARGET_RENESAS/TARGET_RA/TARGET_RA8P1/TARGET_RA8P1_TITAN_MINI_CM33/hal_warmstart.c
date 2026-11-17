/* mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Secondary-core (Cortex-M33) warm start hook for the RA8P1 Titan Mini.
 *
 * Unlike the Cortex-M85 variant (see ../TARGET_RA8P1_TITAN_MINI/
 * hal_warmstart.c) the CPU1 image performs no board-level initialization:
 * clocks, option bytes, pin configuration and the SDRAM controller were
 * already set up by the primary core before it released CPU1 from reset,
 * and the mbed APIs configure individual pins on demand through
 * pinmap_config()/R_BSP_PinAccessEnable(). Leaving this hook empty also
 * keeps CPU1 from racing the M85 for the PFS/SDRAM registers during boot.
 */

#include "bsp_api.h"

void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
        /* C runtime environment has not been setup so you cannot use globals. System clocks are not setup. */
    }

    if (BSP_WARM_START_POST_CLOCK == event)
    {
        /* C runtime environment has not been setup so you cannot use globals. Clocks have been initialized. */
    }
    else if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment, system clocks, and pins are all setup. */
    }
    else
    {
        /* Do nothing */
    }
}

/*
 * mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

/*******************************************************************************************************************//**
 * @addtogroup BOARD_EMBEDFIRE_RA4T1
 * @brief Board specific code for the Embedfire RA4T1 Board
 *
 * This include file is specific to the Embedfire RA4T1 board.
 *
 * @{
 **********************************************************************************************************************/

#ifndef BOARD_INIT_H
#define BOARD_INIT_H

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
void bsp_init(void * p_args);

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/** @} (end addtogroup BOARD_EMBEDFIRE_RA4T1) */

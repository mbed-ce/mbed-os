# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RA8P1 Titan Mini - Cortex-M33 (CPU1) view.
#
# The board files of the Cortex-M85 target (TARGET_RA8P1_TITAN_MINI) are
# reused: hal_data.c, common_data.c, pin_data.c, board_init.c and
# PeripheralPins.c are core-agnostic FSP configuration (their interrupt
# numbers resolve through the #if defined(VECTOR_NUMBER_*) guards from
# whichever vector_data.h is in the include path). Only the CPU1-local ICU
# vector table and the secondary-core warm start hook are specific to this
# target; they live in this folder, which is searched before the M85 board
# folder.

add_library(mbed-ra8p1-titan-mini-cm33 INTERFACE)

target_include_directories(mbed-ra8p1-titan-mini-cm33
    INTERFACE
        .
        ../TARGET_RA8P1_TITAN_MINI
)

target_sources(mbed-ra8p1-titan-mini-cm33
    INTERFACE
        vector_data.c
        hal_warmstart.c
        ../TARGET_RA8P1_TITAN_MINI/hal_data.c
        ../TARGET_RA8P1_TITAN_MINI/common_data.c
        ../TARGET_RA8P1_TITAN_MINI/pin_data.c
        ../TARGET_RA8P1_TITAN_MINI/board_init.c
        ../TARGET_RA8P1_TITAN_MINI/PeripheralPins.c
)

target_link_libraries(mbed-ra8p1-titan-mini-cm33 INTERFACE mbed-ra8p1-cm33)

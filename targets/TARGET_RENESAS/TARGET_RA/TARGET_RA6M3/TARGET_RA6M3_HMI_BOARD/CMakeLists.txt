# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Board-specific files only: common_data.c / hal_data.c / vector_data.c /
# hal_warmstart.c / bsp_clock_cfg.h are shared in the parent TARGET_RA6M3
# directory with the other RA6M3 boards.
add_library(mbed-ra6m3-hmi-board INTERFACE)
target_include_directories(mbed-ra6m3-hmi-board
    INTERFACE
        .
)
target_sources(mbed-ra6m3-hmi-board
    INTERFACE
        board_init.c
        pin_data.c
        PeripheralPins.c
)
target_link_libraries(mbed-ra6m3-hmi-board INTERFACE mbed-ra6m3)

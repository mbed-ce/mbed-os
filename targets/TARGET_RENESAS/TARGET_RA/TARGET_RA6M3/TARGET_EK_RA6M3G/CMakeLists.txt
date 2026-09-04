# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# EK-RA6M3G (Renesas evaluation kit for R7FA6M3AF, 176-pin LQFP)
# common_data.c / hal_data.c / vector_data.c / hal_warmstart.c / bsp_clock_cfg.h
# are shared with the other RA6M3 boards in the parent TARGET_RA6M3 directory.
add_library(mbed-ek-ra6m3g INTERFACE)
target_include_directories(mbed-ek-ra6m3g
    INTERFACE
        .
)
target_sources(mbed-ek-ra6m3g
    INTERFACE
        board_init.c
        pin_data.c
        PeripheralPins.c
)
target_link_libraries(mbed-ek-ra6m3g INTERFACE mbed-ra6m3)

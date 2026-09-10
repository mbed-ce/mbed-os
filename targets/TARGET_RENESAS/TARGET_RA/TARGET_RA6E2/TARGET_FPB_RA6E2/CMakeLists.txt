# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-fpb-ra6e2 INTERFACE)
target_include_directories(mbed-fpb-ra6e2
    INTERFACE
        .
)
target_sources(mbed-fpb-ra6e2
    INTERFACE
        board_init.c
        common_data.c
        hal_warmstart.c
        hal_data.c
        pin_data.c
        vector_data.c
        PeripheralPins.c
)
target_link_libraries(mbed-fpb-ra6e2 INTERFACE mbed-ra6e2)

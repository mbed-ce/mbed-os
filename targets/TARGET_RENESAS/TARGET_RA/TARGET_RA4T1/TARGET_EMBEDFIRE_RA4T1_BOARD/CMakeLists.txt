# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-embedfire-ra4t1-board INTERFACE)
target_include_directories(mbed-embedfire-ra4t1-board
    INTERFACE
        .
)
target_sources(mbed-embedfire-ra4t1-board
    INTERFACE
        board_init.c
        common_data.c
        hal_warmstart.c
        hal_data.c
        pin_data.c
        vector_data.c
        PeripheralPins.c
)
target_link_libraries(mbed-embedfire-ra4t1-board INTERFACE mbed-ra4t1)

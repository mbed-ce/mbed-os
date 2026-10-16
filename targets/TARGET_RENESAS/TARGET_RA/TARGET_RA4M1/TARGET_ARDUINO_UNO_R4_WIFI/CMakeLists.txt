# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-arduino-uno-r4-wifi INTERFACE)
target_include_directories(mbed-arduino-uno-r4-wifi
    INTERFACE
        .
)
target_sources(mbed-arduino-uno-r4-wifi
    INTERFACE
        board_init.c
        common_data.c
        hal_warmstart.c
        hal_data.c
        pin_data.c
        vector_data.c
        PeripheralPins.c
)
target_link_libraries(mbed-arduino-uno-r4-wifi INTERFACE mbed-ra4m1)

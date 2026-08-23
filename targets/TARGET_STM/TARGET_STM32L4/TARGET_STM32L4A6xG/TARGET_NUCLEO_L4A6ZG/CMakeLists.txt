# Copyright (c) 2024 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-l4a6zg INTERFACE)

target_sources(mbed-nucleo-l4a6zg
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-l4a6zg
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l4a6zg INTERFACE mbed-stm32l4a6xg)

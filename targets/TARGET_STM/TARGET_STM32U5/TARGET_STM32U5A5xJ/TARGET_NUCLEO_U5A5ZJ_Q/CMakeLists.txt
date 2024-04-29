# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-u5a5zj-q INTERFACE)

target_sources(mbed-nucleo-u5a5zj-q
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-u5a5zj-q
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-u5a5zj-q INTERFACE mbed-stm32u5a5xj)

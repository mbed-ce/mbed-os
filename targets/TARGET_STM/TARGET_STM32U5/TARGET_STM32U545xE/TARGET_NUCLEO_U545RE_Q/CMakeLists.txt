# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-u545re-q INTERFACE)

target_sources(mbed-nucleo-u545re-q
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-u545re-q
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-u545re-q INTERFACE mbed-stm32u545xe)

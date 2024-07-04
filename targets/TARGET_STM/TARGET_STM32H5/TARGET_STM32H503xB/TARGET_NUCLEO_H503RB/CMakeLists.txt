# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-h503rb INTERFACE)

target_sources(mbed-nucleo-h503rb
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-h503rb
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-h503rb INTERFACE mbed-stm32h503xb)

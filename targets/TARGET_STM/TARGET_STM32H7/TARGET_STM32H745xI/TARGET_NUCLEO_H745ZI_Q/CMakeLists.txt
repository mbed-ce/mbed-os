# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-h745zi-q INTERFACE)

target_sources(mbed-nucleo-h745zi-q
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-h745zi-q
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-h745zi-q INTERFACE mbed-stm32h745xi-cm7)

add_library(mbed-nucleo-h745zi-q-cm7 ALIAS mbed-nucleo-h745zi-q)
add_library(mbed-nucleo-h745zi-q-cm4 ALIAS mbed-nucleo-h745zi-q)
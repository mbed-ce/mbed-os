# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-h563zi INTERFACE)

target_sources(mbed-nucleo-h563zi 
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-h563zi 
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-h563zi  INTERFACE mbed-stm32h563xi)

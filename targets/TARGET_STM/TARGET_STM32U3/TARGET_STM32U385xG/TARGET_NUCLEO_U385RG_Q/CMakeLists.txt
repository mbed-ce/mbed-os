# Copyright (c) 2025 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-u385rg-q INTERFACE)

target_sources(mbed-nucleo-u385rg-q
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-u385rg-q
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-u385rg-q INTERFACE mbed-stm32u385xg)

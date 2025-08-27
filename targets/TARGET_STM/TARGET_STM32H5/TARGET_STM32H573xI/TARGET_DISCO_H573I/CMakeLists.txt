# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-h573i INTERFACE)

target_sources(mbed-disco-h573i
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-h573i
    INTERFACE
        .
)

target_link_libraries(mbed-disco-h573i  INTERFACE mbed-stm32h573xi)

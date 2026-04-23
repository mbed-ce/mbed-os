# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F303K8 EXCLUDE_FROM_ALL)

add_library(mbed-stm32f303x8 INTERFACE)

target_include_directories(mbed-stm32f303x8
    INTERFACE
        .
)

target_link_libraries(mbed-stm32f303x8 INTERFACE mbed-stm32f3)

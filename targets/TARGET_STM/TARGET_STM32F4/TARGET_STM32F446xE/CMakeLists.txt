# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F446RE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F446ZE EXCLUDE_FROM_ALL)

add_library(mbed-stm32f446xe INTERFACE)

target_include_directories(mbed-stm32f446xe
    INTERFACE
        .
)

target_link_libraries(mbed-stm32f446xe INTERFACE mbed-stm32f4)

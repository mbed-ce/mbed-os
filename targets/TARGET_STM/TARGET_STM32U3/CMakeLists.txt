# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32U385xG EXCLUDE_FROM_ALL)

add_library(mbed-stm32u3 INTERFACE)

target_sources(mbed-stm32u3
    INTERFACE
)

target_include_directories(mbed-stm32u3
    INTERFACE
        .
)

target_link_libraries(mbed-stm32u3 INTERFACE mbed-stm mbed-stm32u3cube-fw)

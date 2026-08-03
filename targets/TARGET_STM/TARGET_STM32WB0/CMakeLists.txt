# Copyright (c) 2026 MbedCE Community Contributors
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB09xE EXCLUDE_FROM_ALL)

add_library(mbed-stm32wb0 INTERFACE)

target_sources(mbed-stm32wb0 INTERFACE
    clock_cfg/system_clock.c
)

target_include_directories(mbed-stm32wb0 INTERFACE .)
target_link_libraries(mbed-stm32wb0 INTERFACE mbed-stm mbed-stm32wb0cube-fw)

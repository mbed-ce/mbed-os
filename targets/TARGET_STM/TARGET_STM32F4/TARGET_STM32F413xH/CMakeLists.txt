# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_F413ZH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F413ZH EXCLUDE_FROM_ALL)

add_library(mbed-stm32f413xh INTERFACE)

target_include_directories(mbed-stm32f413xh
    INTERFACE
        .
)

target_link_libraries(mbed-stm32f413xh INTERFACE mbed-stm32f4)

# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_F769NI EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f769xx.S)
endif()

add_library(mbed-stm32f769xi INTERFACE)

target_sources(mbed-stm32f769xi
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32f769xi
    INTERFACE
        .
)

target_link_libraries(mbed-stm32f769xi INTERFACE mbed-stm32f7)

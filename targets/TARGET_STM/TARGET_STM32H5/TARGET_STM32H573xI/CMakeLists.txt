# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_H573I EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32h573xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32H573xI.ld)
endif()

add_library(mbed-stm32h573xi INTERFACE)

target_sources(mbed-stm32h573xi
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32h573xi
    INTERFACE
        .
)

target_link_libraries(mbed-stm32h573xi INTERFACE mbed-stm32h5)

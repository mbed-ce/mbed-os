# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32h7b3xxq.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32h7b3xxq.S)
endif()

add_library(mbed-stm32h7b3xiq INTERFACE)

target_include_directories(mbed-stm32h7b3xiq
    INTERFACE
        .
)

target_sources(mbed-stm32h7b3xiq
    INTERFACE
        ${STARTUP_FILE}
)

target_link_libraries(mbed-stm32h7b3xiq INTERFACE mbed-stm32h7)

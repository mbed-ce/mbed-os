# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_H7A3ZI_Q EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32h7a3xxq.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32h7a3xxq.S)
endif()

add_library(mbed-stm32h7a3xiq INTERFACE)

target_sources(mbed-stm32h7a3xiq
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32h7a3xiq
    INTERFACE
        .
)

target_link_libraries(mbed-stm32h7a3xiq INTERFACE mbed-stm32h7)

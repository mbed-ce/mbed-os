# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_L486RG EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l486xx.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l486xx.S)
endif()

add_library(mbed-stm32l486xg INTERFACE)

target_sources(mbed-stm32l486xg
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32l486xg
    INTERFACE
        .
)

target_link_libraries(mbed-stm32l486xg INTERFACE mbed-stm32l4)

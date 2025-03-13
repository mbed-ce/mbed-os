# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_ADV_WISE_1510 EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l443xx.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l443xx.S)
endif()


add_library(mbed-stm32l443xc INTERFACE)

target_sources(mbed-stm32l443xc
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

target_include_directories(mbed-stm32l443xc
    INTERFACE
        .
)

target_link_libraries(mbed-stm32l443xc INTERFACE mbed-stm32l4)

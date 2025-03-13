# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_L476VG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_L476RG EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l476xx.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l476xx.S)
endif()


add_library(mbed-stm32l476xg INTERFACE)

target_sources(mbed-stm32l476xg
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32l476xg
    INTERFACE
        .
)

target_link_libraries(mbed-stm32l476xg INTERFACE mbed-stm32l4)

# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F412ZG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_WIO_EMW3166 EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f412zx.S)
endif()

add_library(mbed-stm32f412xg INTERFACE)

target_include_directories(mbed-stm32f412xg
    INTERFACE
        .
)

target_sources(mbed-stm32f412xg
    INTERFACE
        ${STARTUP_FILE}
)

target_link_libraries(mbed-stm32f412xg INTERFACE mbed-stm32f4)

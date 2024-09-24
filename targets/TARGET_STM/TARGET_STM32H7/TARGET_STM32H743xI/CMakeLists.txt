# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_H743ZI2 EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32h743xx.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32h743xx.S)
endif()

add_library(mbed-stm32h743xi INTERFACE)

target_sources(mbed-stm32h743xi
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32h743xi
    INTERFACE
        .
)

target_link_libraries(mbed-stm32h743xi INTERFACE mbed-stm32h7)

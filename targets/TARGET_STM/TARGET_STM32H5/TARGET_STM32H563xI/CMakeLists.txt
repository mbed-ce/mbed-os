# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_H563ZI EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32h563xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32H563xI.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32h563xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32h563xI.sct)
endif()

add_library(mbed-stm32h563xi INTERFACE)

target_sources(mbed-stm32h563xi
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32h563xi
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32h563xi ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32h563xi INTERFACE mbed-stm32h5)

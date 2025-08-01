# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_B_U585I_IOT02A EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32u585xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32u585xi.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32u585xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32u585xi.sct)
endif()

add_library(mbed-stm32u585xi INTERFACE)

target_include_directories(mbed-stm32u585xi
    INTERFACE
        .
)

target_sources(mbed-stm32u585xi
    INTERFACE
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-stm32u585xi ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32u585xi INTERFACE mbed-stm32u5)

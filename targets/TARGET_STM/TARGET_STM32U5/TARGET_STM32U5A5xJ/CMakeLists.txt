# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_U5A5ZJ_Q EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32u5a5xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32u5a5xj.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32u5a5xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32u5a5xj.sct)
endif()

add_library(mbed-stm32u5a5xj INTERFACE)

target_include_directories(mbed-stm32u5a5xj
    INTERFACE
        .
)

target_sources(mbed-stm32u5a5xj
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

mbed_set_linker_script(mbed-stm32u5a5xj ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32u5a5xj INTERFACE mbed-stm32u5)

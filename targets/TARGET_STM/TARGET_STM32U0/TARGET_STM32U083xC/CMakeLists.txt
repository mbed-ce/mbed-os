# Copyright (c) 2025 Jan Kamidra
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_U083RC EXCLUDE_FROM_ALL)

set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32u083xx.s)
set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32U083xx.ld)

add_library(mbed-stm32u083xc INTERFACE)

target_sources(mbed-stm32u083xc
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32u083xc
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32u083xc ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32u083xc INTERFACE mbed-stm32u0)

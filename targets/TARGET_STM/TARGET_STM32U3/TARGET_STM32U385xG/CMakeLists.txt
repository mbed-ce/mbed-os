# Copyright (c) 2025 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_U385RG_Q EXCLUDE_FROM_ALL)

set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32u385xx.s)
set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32U385xx.ld)

add_library(mbed-stm32u385xg INTERFACE)

target_sources(mbed-stm32u385xg
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32u385xg
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32u385xg ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32u385xg INTERFACE mbed-stm32u3)

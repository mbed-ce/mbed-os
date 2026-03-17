# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_F413ZH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F413ZH EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f413xx.S)
endif()

add_library(mbed-stm32f413xh INTERFACE)

target_include_directories(mbed-stm32f413xh
    INTERFACE
        .
)

target_sources(mbed-stm32f413xh
    INTERFACE
        ${STARTUP_FILE}
)

target_link_libraries(mbed-stm32f413xh INTERFACE mbed-stm32f4)

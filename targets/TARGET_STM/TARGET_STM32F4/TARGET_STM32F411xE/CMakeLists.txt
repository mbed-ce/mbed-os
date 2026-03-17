# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_MTS_DRAGONFLY_F411RE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MTS_MDOT_F411RE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F411RE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_BLACKPILL_F411CE EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f411xe.S)
endif()

add_library(mbed-stm32f411xe INTERFACE)

target_include_directories(mbed-stm32f411xe
    INTERFACE
        .
)

target_sources(mbed-stm32f411xe
    INTERFACE
        ${STARTUP_FILE}
)

target_link_libraries(mbed-stm32f411xe INTERFACE mbed-stm32f4)

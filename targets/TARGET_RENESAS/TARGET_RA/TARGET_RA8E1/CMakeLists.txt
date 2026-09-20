# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RA8E1
add_library(mbed-ra8e1 INTERFACE)

target_include_directories(mbed-ra8e1
    INTERFACE
        .
)

target_sources(mbed-ra8e1
    INTERFACE
        uart_device.c
        gpio_irq_device.c
        PeripheralPins.c
)

target_link_libraries(mbed-ra8e1 INTERFACE mbed-renesas-ra)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    mbed_set_linker_script(mbed-ra8e1 RA8E1.ld)
endif()

add_subdirectory(TARGET_FPB_RA8E1 EXCLUDE_FROM_ALL)

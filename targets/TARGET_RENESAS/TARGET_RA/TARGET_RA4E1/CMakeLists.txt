# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RA4E1
add_library(mbed-ra4e1 INTERFACE)

target_include_directories(mbed-ra4e1
    INTERFACE
        .
)

target_sources(mbed-ra4e1
    INTERFACE
        uart_device.c
		gpio_irq_device.c
		PeripheralPins.c
)

target_link_libraries(mbed-ra4e1 INTERFACE mbed-renesas-ra)
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
	mbed_set_linker_script(mbed-ra4e1 RA4E1.ld)
endif()

add_subdirectory(TARGET_FPB_RA4E1 EXCLUDE_FROM_ALL)

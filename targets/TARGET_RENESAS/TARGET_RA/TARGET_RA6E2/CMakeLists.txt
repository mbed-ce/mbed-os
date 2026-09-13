# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RA6E2
add_library(mbed-ra6e2 INTERFACE)

target_include_directories(mbed-ra6e2
    INTERFACE
        .
)

target_sources(mbed-ra6e2
    INTERFACE
        uart_device.c
		PeripheralPins.c
)

target_link_libraries(mbed-ra6e2 INTERFACE mbed-renesas-ra)
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
	mbed_set_linker_script(mbed-ra6e2 RA6E2.ld)
endif()

add_subdirectory(TARGET_FPB_RA6E2 EXCLUDE_FROM_ALL)

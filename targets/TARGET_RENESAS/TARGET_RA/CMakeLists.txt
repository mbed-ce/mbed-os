# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-renesas-ra INTERFACE)

add_subdirectory(TARGET_RA4E1 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_RA6E2 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_RA8E1 EXCLUDE_FROM_ALL)

add_subdirectory(fsp)

target_include_directories(mbed-renesas-ra
    INTERFACE
        .
)

target_sources(mbed-renesas-ra
    INTERFACE
		gpio_api.c
		gpio_irq_api.c
		port_api.c
		us_ticker.c
        serial_api.c
        pinmap.c
        mbed_overrides.cpp
)

target_link_libraries(mbed-renesas-ra INTERFACE mbed-cmsis-cortex-m)

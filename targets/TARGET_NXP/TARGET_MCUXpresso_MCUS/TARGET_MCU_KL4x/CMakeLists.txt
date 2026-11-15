# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_FRDM EXCLUDE_FROM_ALL)

target_include_directories(mbed-mcu-kl43z INTERFACE .)

target_sources(mbed-mcu-kl43z
    INTERFACE
        fsl_clock_config.c
        PeripheralPins.c
        mbed_overrides.c
        startup_MKL43Z4.S
        lpuart_device.c
)

mbed_set_linker_script(mbed-mcu-kl43z ${CMAKE_CURRENT_SOURCE_DIR}/MKL43Z256xxx4.ld)

target_link_libraries(mbed-mcu-kl43z INTERFACE mbed-frdm-kl43z)

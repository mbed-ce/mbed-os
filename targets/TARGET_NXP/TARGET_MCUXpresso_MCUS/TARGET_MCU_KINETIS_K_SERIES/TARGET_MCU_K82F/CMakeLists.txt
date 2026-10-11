# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mcu-k82f INTERFACE .)
target_sources(mbed-mcu-k82f
    INTERFACE
        PeripheralPins.c
        lpuart_device.c
        startup_MK82F25615.S
)

add_subdirectory(TARGET_FRDM_K82F)
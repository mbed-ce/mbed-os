# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mcu-k64f INTERFACE .)
target_sources(mbed-mcu-k64f
    INTERFACE
        PeripheralPins.c
        uart_serial_device.c
)

add_subdirectory(TARGET_FRDM_K64F)
add_subdirectory(TARGET_HEXIWEAR)
add_subdirectory(TARGET_SDT64B)
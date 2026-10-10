# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-mcu-kinetis-k-series INTERFACE .)
target_sources(mbed-mcu-kinetis-k-series
    INTERFACE
        mbed_overrides.c
)
mbed_set_linker_script(mbed-mcu-kinetis-k-series ${CMAKE_CURRENT_SOURCE_DIR}/Kinetis-K-Series.ld)

add_subdirectory(TARGET_MCU_K22F)
add_subdirectory(TARGET_MCU_K64F)
add_subdirectory(TARGET_MCU_K66F)
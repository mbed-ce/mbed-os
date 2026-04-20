# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RP235x sources
target_sources(mbed-mcu-rp235x
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-mcu-rp235x
    INTERFACE
        .
)

if(MCU_RP235X IN_LIST MBED_TARGET_LABELS)
    pico_compile_boot_stage2(
        mbed-mcu-rp235x
        ${CMAKE_CURRENT_SOURCE_DIR}/../../pico-sdk/src/rp2350
    )
endif()

add_subdirectory(TARGET_RASPBERRY_PI_PICO_2)
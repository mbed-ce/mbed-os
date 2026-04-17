# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RP2040 sources
target_sources(mbed-mcu-rp2040
    INTERFACE
        PeripheralPins.c
)

if(MCU_RP2040 IN_LIST MBED_TARGET_LABELS)
    pico_compile_boot_stage2(
        mbed-mcu-rp2040
        ${CMAKE_CURRENT_SOURCE_DIR}/../../pico-sdk/src/rp2040
    )
endif()

add_subdirectory(TARGET_RASPBERRY_PI_PICO)
add_subdirectory(TARGET_RASPBERRY_PI_PICO_W)
add_subdirectory(TARGET_SFE_THING_PLUS_RP2040)
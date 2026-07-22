# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RP2040 sources
target_sources(mbed-mcu-rp2040
    INTERFACE
        rtc_api.c
        reset_reason.c
)
target_include_directories(mbed-mcu-rp2040
        INTERFACE
        .
)

add_subdirectory(TARGET_RASPBERRY_PI_PICO)
add_subdirectory(TARGET_RASPBERRY_PI_PICO_W)
add_subdirectory(TARGET_SFE_THING_PLUS_RP2040)
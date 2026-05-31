# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RP235x sources
target_include_directories(mbed-mcu-rp235x
    INTERFACE
        .
)

target_sources(mbed-mcu-rp235x
    INTERFACE
        lp_ticker_api.c
        reset_reason.c
)

if(MCU_RP235X IN_LIST MBED_TARGET_LABELS)
    pico_compile_boot_stage2(
        mbed-mcu-rp235x
        ${CMAKE_CURRENT_SOURCE_DIR}/../../pico-sdk/src/rp2350
    )
endif()

add_subdirectory(TARGET_RASPBERRY_PI_PICO_2)
add_subdirectory(TARGET_OLIMEX_RP2350_PICO2_XL)
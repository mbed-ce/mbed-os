# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_MCU_RP2040 EXCLUDE_FROM_ALL)

target_include_directories(mbed-mcu-rp2
    INTERFACE
        .
)

target_sources(mbed-mcu-rp2
    INTERFACE
        analogin_api.c
        flash_api.c
        gpio_api.c
        i2c_api.c
        lp_ticker.c
        mbed_overrides.c
        pwmout_api.c
        reset_reason.c
        serial_api.c
        spi_api.c
        rtc_api.c
        us_ticker.c
        USBPhy_RP2.cpp
        watchdog_api.c
)
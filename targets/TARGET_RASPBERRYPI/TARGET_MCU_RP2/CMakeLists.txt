# Copyright (c) 2026 Jamie Smith. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_MCU_RP2040 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MCU_RP235X EXCLUDE_FROM_ALL)

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
        mbed_overrides.c
        pwmout_api.c
        reset_reason.c
        serial_api.c
        spi_api.c
        us_ticker.c
        USBPhy_RP2.cpp
        watchdog_api.c
        crt0_mbed.S
        PeripheralPins.c
        sleep_api.c
)

# RPxxxx linker script
mbed_set_linker_script(mbed-mcu-rp2 ${CMAKE_CURRENT_SOURCE_DIR}/memmap_default_mbed.ld)
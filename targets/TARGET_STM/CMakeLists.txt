# Copyright (c) 2025 MbedCE Community Contributors (Jan Kamidra)
# SPDX-License-Identifier: Apache-2.0

include(${CMAKE_CURRENT_SOURCE_DIR}/tools/stm32.cmake)

set(_stm32_family "")
get_stm32_family(_stm32_family)

add_subdirectory(TARGET_${_stm32_family} EXCLUDE_FROM_ALL)

add_library(mbed-stm INTERFACE)

target_include_directories(mbed-stm
    INTERFACE
        .
)

target_sources(mbed-stm
    INTERFACE
        USBPhy_STM32.cpp
        analogin_api.c
        analogout_api.c
        can_api.c
        gpio_api.c
        gpio_irq_api.c
        hal_tick_overrides.c
        i2c_api.c
        lp_ticker.c
        mbed_crc_api.c
        mbed_overrides.c
        ospi_api.c
        pinmap.c
        port_api.c
        pwmout_api.c
        qspi_api.c
        reset_reason.c
        rtc_api.c
        serial_api.c
        sleep.c
        stm_spi_api.c
        trng_api.c
        us_ticker.c
        watchdog_api.c
		stm_dma_utils.c
)

target_link_libraries(mbed-stm INTERFACE mbed-cmsis-cortex-m)

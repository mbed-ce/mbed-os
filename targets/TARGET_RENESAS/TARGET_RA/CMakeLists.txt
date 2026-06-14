# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_RA4E1 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_RA6E2 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_RA8E1 EXCLUDE_FROM_ALL)

add_library(mbed-renesas-ra INTERFACE)

target_include_directories(mbed-renesas-ra
    INTERFACE
        .
        ./RTT
        ./ra/fsp/inc
        ./ra/fsp/inc/api
        ./ra/fsp/inc/instances
        ./ra/fsp/src/bsp/cmsis/Device/RENESAS/Include
        ./ra/fsp/src/bsp/mcu/all
)

target_sources(mbed-renesas-ra
    INTERFACE
        ./RTT/SEGGER_RTT.c
		./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.c
		./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.c
        ./ra/fsp/src/bsp/mcu/all/bsp_clocks.c
        ./ra/fsp/src/bsp/mcu/all/bsp_common.c
        ./ra/fsp/src/bsp/mcu/all/bsp_delay.c
        ./ra/fsp/src/bsp/mcu/all/bsp_group_irq.c
        ./ra/fsp/src/bsp/mcu/all/bsp_guard.c
        ./ra/fsp/src/bsp/mcu/all/bsp_io.c
        ./ra/fsp/src/bsp/mcu/all/bsp_ipc.c
        ./ra/fsp/src/bsp/mcu/all/bsp_irq.c
        ./ra/fsp/src/bsp/mcu/all/bsp_macl.c
        ./ra/fsp/src/bsp/mcu/all/bsp_ospi_b.c
        ./ra/fsp/src/bsp/mcu/all/bsp_register_protection.c
        ./ra/fsp/src/bsp/mcu/all/bsp_sdram.c
        ./ra/fsp/src/bsp/mcu/all/bsp_security.c
		gpio_api.c
		gpio_irq_api.c
		port_api.c
		us_ticker.c
        analog_api.c
        analogout_api.c
        trng_api.c
        watchdog_api.c
        pwmout_api.c
        crc_api.c
        flash_api.c
        rtc_api.c
        serial_api.c
        spi_api.c
        i2c_api.c
        can_api.c
        reset_reason.c
        pinmap.c
        mbed_overrides.cpp
)

target_link_libraries(mbed-renesas-ra INTERFACE mbed-cmsis-cortex-m)

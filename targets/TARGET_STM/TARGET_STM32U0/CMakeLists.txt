# Copyright (c) 2025 Jan Kamidra
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32U083xC EXCLUDE_FROM_ALL)

add_library(mbed-stm32u0 INTERFACE)

target_sources(mbed-stm32u0
    INTERFACE
    clock_cfg/system_clock.c
    analogin_device.c
    analogout_device.c
    flash_api.c
    gpio_irq_device.c
    i2c_device.c
    spi_api.c
    pwmout_device.c
    serial_device.c
)

target_include_directories(mbed-stm32u0
    INTERFACE
        .
)

target_link_libraries(mbed-stm32u0 INTERFACE mbed-stm mbed-stm32u0cube-fw)

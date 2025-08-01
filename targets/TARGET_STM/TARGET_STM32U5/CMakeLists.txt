# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32U545xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32U575xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32U575xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32U585xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32U5A5xJ EXCLUDE_FROM_ALL)

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32u5 INTERFACE)

target_include_directories(mbed-stm32u5
    INTERFACE
        .
)

target_sources(mbed-stm32u5
    INTERFACE
        clock_cfg/system_clock.c
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        i2c_device.c
        serial_device.c
        spi_api.c
        cache.c
)

target_link_libraries(mbed-stm32u5 INTERFACE mbed-stm mbed-stm32u5cube-fw)

# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32H723xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H725xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H735xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H743xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H745xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H747xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H750xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H753xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H7A3xIQ EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H7B3xIQ EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32h7 INTERFACE)

target_sources(mbed-stm32h7
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        i2c_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_include_directories(mbed-stm32h7
    INTERFACE
        .
)

target_link_libraries(mbed-stm32h7 INTERFACE mbed-stm mbed-stm32h7cube-fw)


# Add clock file depending on the frequency
if("STM32H7_480MHZ" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-stm32h7 INTERFACE clock_cfg/TARGET_STM32H7_480MHZ/system_clock.c)
elseif("STM32H7_550MHZ" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-stm32h7 INTERFACE clock_cfg/TARGET_STM32H7_550MHZ/system_clock.c)
elseif("STM32H7_280MHZ" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-stm32h7 INTERFACE clock_cfg/TARGET_STM32H7_280MHZ/system_clock.c)
endif()

# Add linker scripts & system init files
add_subdirectory(linker_scripts)
add_subdirectory(system_init_files)
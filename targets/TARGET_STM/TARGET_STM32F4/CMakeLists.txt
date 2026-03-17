# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F401xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F411xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F412xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F413xH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F429xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F439xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F446xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F469xI EXCLUDE_FROM_ALL)

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32f4 INTERFACE)

target_include_directories(mbed-stm32f4
    INTERFACE
        .
)

target_sources(mbed-stm32f4
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_link_libraries(mbed-stm32f4 INTERFACE mbed-stm mbed-stm32f4cube-fw)

# Add clock file depending on the frequency
if("STM32F4_84MHZ" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-stm32f4 INTERFACE clock_cfg/TARGET_STM32F4_84MHZ/system_clock.c)
elseif("STM32F4_96_100MHZ" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-stm32f4 INTERFACE clock_cfg/TARGET_STM32F4_96_100MHZ/system_clock.c)
elseif("STM32F4_100MHZ" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-stm32f4 INTERFACE clock_cfg/TARGET_STM32F4_100MHZ/system_clock.c)
elseif("STM32F4_168_180MHZ" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-stm32f4 INTERFACE clock_cfg/TARGET_STM32F4_168_180MHZ/system_clock.c)
elseif("STM32F4_180MHZ" IN_LIST MBED_TARGET_LABELS)
    target_sources(mbed-stm32f4 INTERFACE clock_cfg/TARGET_STM32F4_180MHZ/system_clock.c)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    mbed_set_linker_script(mbed-stm32f401xe linker_scripts/STM32F4_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f411xe linker_scripts/STM32F4_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f412xg linker_scripts/STM32F4_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f413xh linker_scripts/STM32F4_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f429xi linker_scripts/STM32F4_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f439xi linker_scripts/STM32F4_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f446xe linker_scripts/STM32F4_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f469xi linker_scripts/STM32F4_COMMON.ld)
endif()
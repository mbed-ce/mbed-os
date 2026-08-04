# Copyright (c) 2026 MbedCE Community Contributors
# SPDX-License-Identifier: Apache-2.0

mbed_setup_submodule(cmsis-device-wb0 CHECK_FILE Source/Templates/system_stm32wb0x.c IF_LABEL MCU_STM32WB0)
mbed_setup_submodule(stm32wb0x-hal-driver CHECK_FILE Src/stm32wb0x_hal.c IF_LABEL MCU_STM32WB0)

add_library(mbed-stm32wb0cube-fw INTERFACE)

target_sources(mbed-stm32wb0cube-fw INTERFACE
    cmsis-device-wb0/Source/Templates/system_stm32wb0x.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_cortex.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_dma.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_flash.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_flash_ex.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_gpio.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_pwr.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_pwr_ex.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_rcc.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_rcc_ex.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_rtc.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_rtc_ex.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_tim.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_tim_ex.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_uart.c
    stm32wb0x-hal-driver/Src/stm32wb0x_hal_uart_ex.c
    stm32wb0x-hal-driver/Src/stm32wb0x_ll_gpio.c
    stm32wb0x-hal-driver/Src/stm32wb0x_ll_pwr.c
    stm32wb0x-hal-driver/Src/stm32wb0x_ll_rcc.c
    stm32wb0x-hal-driver/Src/stm32wb0x_ll_system.c
    stm32wb0x-hal-driver/Src/stm32wb0x_ll_tim.c
)

target_include_directories(mbed-stm32wb0cube-fw INTERFACE
    .
    cmsis-device-wb0/Include
    stm32wb0x-hal-driver/Inc
    stm32wb0x-hal-driver/Inc/Legacy
)

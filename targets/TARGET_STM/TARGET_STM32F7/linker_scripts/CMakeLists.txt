# Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    mbed_set_linker_script(mbed-stm32f722xe STM32F7_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f746xg STM32F7_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f756xg STM32F7_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f767xi STM32F7_COMMON.ld)
    mbed_set_linker_script(mbed-stm32f769xi STM32F7_COMMON.ld)
endif()
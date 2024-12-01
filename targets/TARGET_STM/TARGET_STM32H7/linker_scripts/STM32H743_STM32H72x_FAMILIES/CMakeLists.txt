# Copyright (c) 2024 Jamie Smith
# SPDX-License-Identifier: Apache-2.0
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    mbed_set_linker_script(mbed-stm32h743xi STM32H743_H72x.ld)
    mbed_set_linker_script(mbed-stm32h723xg STM32H743_H72x.ld)
    mbed_set_linker_script(mbed-stm32h725xe STM32H743_H72x.ld)
    mbed_set_linker_script(mbed-stm32h735xg STM32H743_H72x.ld)
    mbed_set_linker_script(mbed-stm32h750xb STM32H743_H72x.ld)
    mbed_set_linker_script(mbed-stm32h753xi STM32H743_H72x.ld)
endif()

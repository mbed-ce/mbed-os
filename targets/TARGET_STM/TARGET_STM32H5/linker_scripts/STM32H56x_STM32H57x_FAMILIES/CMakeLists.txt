# Copyright (c) 2024 Jamie Smith
# SPDX-License-Identifier: Apache-2.0
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    mbed_set_linker_script(mbed-stm32h563xi STM32H56x_STM32H57x.ld)
    mbed_set_linker_script(mbed-stm32h573xi STM32H56x_STM32H57x.ld)
endif()

# Copyright (c) 2024 Jamie Smith
# SPDX-License-Identifier: Apache-2.0
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
	mbed_set_linker_script(mbed-stm32h7a3xiq STM32H7Ax.ld)
	mbed_set_linker_script(mbed-stm32h7b3xiq STM32H7Ax.ld)
endif()

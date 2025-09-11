# Copyright (c) 2024 Jamie Smith
# SPDX-License-Identifier: Apache-2.0
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
	mbed_set_linker_script(mbed-stm32h503xb STM32H503xB.ld)
endif()

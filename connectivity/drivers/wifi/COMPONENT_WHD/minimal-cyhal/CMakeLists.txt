# Copyright (c) 2024 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# In order to use the Cypress WHD driver, we need some level of Cypress HAL functionality available.
# However, for non-Cypress chips, these functions aren't available from the target code.
# This directory contains a minimal implementation of Cypress HAL designed to work on generic chips.
# Note that currently some of this has to be implemented on a per-target level, as Mbed
# does not have an SDIO HAL API.  Hopefully this can be fixed someday...

add_library(mbed-minimal-cyhal STATIC
	cy_hal.c
	cyhal_spi.c
	cyhal_system.c
	cyhal_gpio.cpp
	wiced_filesystem.cpp)
target_include_directories(mbed-minimal-cyhal PUBLIC .)
target_link_libraries(mbed-minimal-cyhal PUBLIC
	mbed-core-flags
	mbed-rtos-flags)

# This is needed in order to make cybsp_wifi.h include cycfg.h
target_compile_definitions(mbed-minimal-cyhal PUBLIC COMPONENT_CUSTOM_DESIGN_MODUS)

# This is needed in order to make cyabs_rtos_internal.h include cyhal.h
target_compile_definitions(mbed-minimal-cyhal PUBLIC CY_USING_HAL)

if(STM32H7 IN_LIST MBED_TARGET_LABELS)
	target_sources(mbed-minimal-cyhal PRIVATE
		TARGET_STM32H7/cyhal_sdio.c)
	target_include_directories(mbed-minimal-cyhal PUBLIC TARGET_STM32H7)

	if(STM32H747_ARDUINO IN_LIST MBED_TARGET_LABELS)
		target_include_directories(mbed-minimal-cyhal PUBLIC TARGET_STM32H7/TARGET_STM32H747_ARDUINO)
		target_sources(mbed-minimal-cyhal PRIVATE
			TARGET_STM32H7/TARGET_STM32H747_ARDUINO/wiced_filesystem_setup.cpp)
		target_link_libraries(mbed-minimal-cyhal PRIVATE
			mbed-storage-qspif
			mbed-storage-blockdevice
			mbed-storage-fat)
	endif()
endif()
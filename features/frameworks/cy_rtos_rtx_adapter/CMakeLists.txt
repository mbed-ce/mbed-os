# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CY_RTOS_RTX_ADAPTER_SOURCES
    source/COMPONENT_RTX/cyabs_rtos_rtxv5.c
	source/cy_worker_thread.c)

set(CY_RTOS_RTX_ADAPTER_INCLUDE_DIRS
	include
	include/COMPONENT_RTX)

if("Cypress" IN_LIST MBED_TARGET_LABELS)
	# Set up the RTOS abstraction so it's included as part of the base Mbed RTOS.
	# We pretty much have to add the sources like this because the HAL needs to use this
	# code, so there's a circular dependency.
	target_sources(mbed-rtos-sources INTERFACE ${CY_RTOS_RTX_ADAPTER_SOURCES})
	target_include_directories(mbed-rtos-flags INTERFACE ${CY_RTOS_RTX_ADAPTER_INCLUDE_DIRS})
else()
	# Build as part of the minimal CyHAL if it's enabled
	if(TARGET mbed-minimal-cyhal)
		target_sources(mbed-minimal-cyhal PRIVATE ${CY_RTOS_RTX_ADAPTER_SOURCES})
		target_include_directories(mbed-minimal-cyhal PUBLIC ${CY_RTOS_RTX_ADAPTER_INCLUDE_DIRS})
	endif()
endif()

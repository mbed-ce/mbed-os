# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cy-rtos-abstraction STATIC)
target_link_libraries(mbed-cy-rtos-abstraction PUBLIC mbed-core-flags mbed-rtos-flags)
target_include_directories(mbed-cy-rtos-abstraction
    PUBLIC
		include
        include/COMPONENT_RTX
)
target_sources(mbed-cy-rtos-abstraction
    PRIVATE
        source/COMPONENT_RTX/cyabs_rtos_rtxv5.c
		source/cy_worker_thread.c
)

if(NOT "Cypress" IN_LIST MBED_TARGET_LABELS)
	target_link_libraries(mbed-cy-rtos-abstraction PUBLIC mbed-minimal-cyhal)
	target_compile_definitions(mbed-cy-rtos-abstraction PUBLIC "CY_USING_HAL")
endif()

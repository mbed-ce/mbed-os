# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# If not running on a Cypress chip, we need to build the CyHAL support library.
if(NOT "Cypress" IN_LIST MBED_TARGET_LABELS)
	add_subdirectory(minimal-cyhal)
endif()

add_subdirectory(whd-bsp-integration)
add_subdirectory(wifi-host-driver)
add_subdirectory(whd_mac)
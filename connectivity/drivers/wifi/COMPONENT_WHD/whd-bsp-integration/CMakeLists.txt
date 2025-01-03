# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-wifi
    PUBLIC
        .
)

target_sources(mbed-wifi
    PRIVATE
        cy_network_buffer.c
        cybsp_wifi.c
)

target_link_libraries(mbed-wifi
    PUBLIC
        mbed-lwipstack
		mbed-core-flags
)

if("DEVICE_EMAC=1" IN_LIST MBED_TARGET_DEFINITIONS)
    target_link_libraries(mbed-wifi
	    PUBLIC
	        mbed-emac
	)
endif()

if(NOT "Cypress" IN_LIST MBED_TARGET_LABELS)
	target_link_libraries(mbed-wifi PUBLIC mbed-minimal-cyhal)
endif()
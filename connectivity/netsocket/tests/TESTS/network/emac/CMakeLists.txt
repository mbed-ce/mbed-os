# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=WIFI" IN_LIST MBED_CONFIG_DEFINITIONS AND
	NOT "MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=ETHERNET" IN_LIST MBED_CONFIG_DEFINITIONS)
	set(TEST_SKIPPED "Requires wi-fi or ethernet to be the default network interface")
endif()

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            emac_test_broadcast.cpp
            emac_test_initialize.cpp
            emac_test_memory.cpp
            emac_test_multicast_filter.cpp
            emac_test_unicast.cpp
            emac_test_unicast_burst.cpp
            emac_test_unicast_frame_len.cpp
            emac_test_unicast_long.cpp
)

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-network-emac
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
		mbed-netsocket
        mbed-emac-test-utils
    TEST_SKIPPED
        ${TEST_SKIPPED}
)

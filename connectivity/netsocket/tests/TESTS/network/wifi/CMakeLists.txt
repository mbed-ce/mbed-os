# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# TODO Currently, on boards with multiple interfaces, this test may not run, as it is looking
# for the default wifi interface.  As far as I can tell, there is no config definition that
# allows detecting if a board has wifi support other than
# MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE, so we are stuck with this limitation.
if(NOT "MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=WIFI" IN_LIST MBED_CONFIG_DEFINITIONS)
	set(TEST_SKIPPED "Requires wi-fi to be the default network interface")
endif()

# Options specific to this test
set(MBED_GREENTEA_WIFI_MAX_SCAN_SIZE 30 CACHE STRING "How many networks may appear in Wifi scan result")
set(MBED_GREENTEA_WIFI_SECURE_CHANNEL 1 CACHE STRING "Channel number that the wifi network is on.  Used for channel-specific connection tests, if supported by the wifi driver.")

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            wifi-constructor.cpp
            wifi_connect_disconnect_nonblock.cpp
            wifi_connect_disconnect_repeat.cpp
            wifi_connect_nocredentials.cpp
            wifi_connect_params_channel.cpp
            wifi_connect_params_channel_fail.cpp
            wifi_connect_params_null.cpp
            wifi_connect_params_valid_secure.cpp
            wifi_connect_secure_fail.cpp
            wifi_get_rssi.cpp
            wifi_scan.cpp
            wifi_scan_null.cpp
            wifi_set_channel.cpp
            wifi_set_credential.cpp
)

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-network-wifi
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
    TEST_SKIPPED
        ${TEST_SKIPPED}
)

if(TARGET test-mbed-connectivity-network-wifi)
	target_compile_definitions(test-mbed-connectivity-network-wifi PRIVATE
		MBED_GREENTEA_WIFI_MAX_SCAN_SIZE=${MBED_GREENTEA_WIFI_MAX_SCAN_SIZE}
		MBED_GREENTEA_WIFI_SECURE_CHANNEL=${MBED_GREENTEA_WIFI_SECURE_CHANNEL})
endif()
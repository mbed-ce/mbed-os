# Copyright (c) 2025 Jamie Smith.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-netsocket-tests-common-inc INTERFACE)
target_include_directories(mbed-netsocket-tests-common-inc INTERFACE .)
target_link_libraries(mbed-netsocket-tests-common-inc INTERFACE mbed-netsocket)

# Set up variables for wi-fi SSID and password
if("MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=WIFI" IN_LIST MBED_CONFIG_DEFINITIONS)
    set(MBED_GREENTEA_WIFI_SECURE_SSID "" CACHE STRING "SSID of a secured wi-fi network with internet access, for greentea tests which need to connect to wifi")
    set(MBED_GREENTEA_WIFI_SECURE_PASSWORD "" CACHE STRING "Password to the network given by MBED_GREENTEA_WIFI_SECURE_SSID")
    set(MBED_GREENTEA_WIFI_SECURE_PROTOCOL "WPA2" CACHE STRING "WiFi security protocol, valid values are WEP, WPA, WPA2, WPA_WPA2, WPA3, WPA3_WPA2, NONE")

    if("${MBED_GREENTEA_WIFI_SECURE_SSID}" STREQUAL "" OR "${MBED_GREENTEA_WIFI_SECURE_PASSWORD}" STREQUAL "")
        message(WARNING "MBED_GREENTEA_WIFI_SECURE_SSID and MBED_GREENTEA_WIFI_SECURE_PASSWORD must be set in order for wi-fi greentea tests to pass")
    else()
        target_compile_definitions(mbed-netsocket-tests-common-inc INTERFACE
                "MBED_GREENTEA_WIFI_SECURE_SSID=\"${MBED_GREENTEA_WIFI_SECURE_SSID}\""
                "MBED_GREENTEA_WIFI_SECURE_PASSWORD=\"${MBED_GREENTEA_WIFI_SECURE_PASSWORD}\""
                MBED_GREENTEA_WIFI_SECURE_PROTOCOL=${MBED_GREENTEA_WIFI_SECURE_PROTOCOL})
    endif()
endif()

# Tests can link one of the below two libraries to use lwipstack or nanostack
add_library(mbed-netsocket-tests-use-lwipstack STATIC EXCLUDE_FROM_ALL greentea_network_stack_lwipstack.cpp)
target_link_libraries(mbed-netsocket-tests-use-lwipstack PUBLIC mbed-lwipstack)

add_library(mbed-netsocket-tests-use-nanostack STATIC EXCLUDE_FROM_ALL greentea_network_stack_nanostack.cpp)
target_link_libraries(mbed-netsocket-tests-use-nanostack PUBLIC mbed-nanostack)
# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-wifi
    PRIVATE
        ESP8266Interface.cpp
        ESP8266/ESP8266.cpp
)

target_include_directories(mbed-wifi
    PUBLIC
        .
        ./ESP8266
)

# Add sources which contain override symbols
if("MBED_CONF_ESP8266_PROVIDE_DEFAULT=1" IN_LIST MBED_CONFIG_DEFINITIONS)
    mbed_wifi_add_override_sources(
        ESP8266Interface.cpp
    )
endif()

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

# Link override object file from static library
if("MBED_CONF_ESP8266_PROVIDE_DEFAULT=1" IN_LIST MBED_CONFIG_DEFINITIONS)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        target_link_options(mbed-wifi
            INTERFACE
                LINKER:--undefined=LINK_ESP8266INTERFACE_CPP
        )
    endif()
endif()

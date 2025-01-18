# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-wifi
    PRIVATE
        ./ESP32Interface.cpp
        ./ESP32InterfaceAP.cpp
        ./ESP32Stack.cpp
        ./ESP32/ESP32.cpp
)

target_include_directories(mbed-wifi
    PUBLIC
        ./
        ./ESP32/
)

# Link override object file coming from static library anyway
#
# NOTE: This linker option is to pretend undefined symbol and won't cause
#       undefined symbol error even though the override object file actually
#       doesn't provide such symbol definition.
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    target_link_options(mbed-wifi
            INTERFACE
            LINKER:--undefined=LINK_ESP32INTERFACE_CPP
    )
endif()

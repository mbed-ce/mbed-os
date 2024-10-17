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

# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


add_library(mbed-arduino-portenta-h7-m4 INTERFACE)

target_sources(mbed-arduino-portenta-h7-m4
    INTERFACE
        portenta_power.cpp
)

target_include_directories(mbed-arduino-portenta-h7-m4
    INTERFACE
        .
)

target_link_libraries(mbed-arduino-portenta-h7-m4 INTERFACE mbed-stm32h747xi-cm4)


add_library(mbed-arduino-portenta-h7-m7 INTERFACE)

target_sources(mbed-arduino-portenta-h7-m7
    INTERFACE
        portenta_power.cpp
)

target_include_directories(mbed-arduino-portenta-h7-m7
    INTERFACE
        .
)

target_link_libraries(mbed-arduino-portenta-h7-m7 INTERFACE mbed-stm32h747-arduino)

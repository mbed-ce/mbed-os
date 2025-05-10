# Copyright (c) 2024 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Target for common code between the two arduinos
add_library(mbed-stm32h747-arduino INTERFACE)
target_link_libraries(mbed-stm32h747-arduino INTERFACE mbed-stm32h747xi-cm7)

target_sources(mbed-stm32h747-arduino INTERFACE
	PeripheralPins.c)

add_subdirectory(TARGET_ARDUINO_PORTENTA_H7)
add_subdirectory(TARGET_ARDUINO_GIGA)

# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("COMPONENT_MULTITECH_DRAGONFLY_NANO_CELLULAR=1" IN_LIST MBED_TARGET_DEFINITIONS)
	add_subdirectory(DragonflyNano)
endif()
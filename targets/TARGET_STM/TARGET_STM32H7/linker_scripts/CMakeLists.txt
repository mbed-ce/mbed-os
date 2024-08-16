# Copyright (c) 2024 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
	add_subdirectory(TOOLCHAIN_GCC_ARM)
endif()
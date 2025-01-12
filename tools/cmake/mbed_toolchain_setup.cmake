# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This CMake script shall be included first:
# - before project() (or enable_languages()) is called by the application
# - before any other Mbed CMake scripts are included and before the mbed-os subdir is added
# - before any upload method config variables are set
# Its goal is to configure CMake to execute the ARM compilers.
# The only thing that should be done before including this script is to set the MBED_APP_JSON_PATH and
# MBED_CUSTOM_TARGETS_JSON_PATH variables.

# Add our CMake list files to CMake default module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

find_program(CCACHE "ccache")
if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Figure out path to Mbed source
get_filename_component(MBED_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../.. REALPATH)

# Find Python (needed to generate configurations)
include(mbed_python_interpreter)

include(mbed_generate_config_header)
include(mbed_target_functions)
include(mbed_create_distro)

# Load toolchain and mbed configuration, generating it if needed
include(mbed_generate_configuration)

# Load toolchain file
if(NOT CMAKE_TOOLCHAIN_FILE OR MBED_TOOLCHAIN_FILE_USED)
    set(MBED_TOOLCHAIN_FILE_USED TRUE CACHE INTERNAL "")
    # We want to bring CMP0123 we set in mbed_toolchain.cmake
    # to the whole Mbed OS.
    include(mbed_toolchain NO_POLICY_SCOPE)
endif()

# Specify available build profiles and add options for the selected build profile
include(mbed_profile)

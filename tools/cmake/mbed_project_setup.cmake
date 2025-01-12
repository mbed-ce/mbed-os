# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This script should be included after the top-level script calls project().
# It sets up some global variables for project and upload method configuration.


# Enable languages used by Mbed, in case the top level script didn't enable them
enable_language(C CXX ASM)

# set executable suffix (has to be done after enabling languages)
# Note: This is nice in general, but is also required because STM32Cube will only work on files with a .elf extension
set(CMAKE_EXECUTABLE_SUFFIX .elf)

# Load upload method configuration defaults for this target.
# Loading the settings here makes sure they are set at global scope, and also makes sure that
# the user can override them by changing variable values after including app.cmake.
#
# default expected paths
set(EXPECTED_CUSTOM_UPLOAD_CFG_FILE_PATH ${CMAKE_SOURCE_DIR}/custom_targets/upload_method_cfg/${MBED_TARGET}.cmake)
set(EXPECTED_MBED_UPLOAD_CFG_FILE_PATH ${MBED_SOURCE_DIR}/targets/upload_method_cfg/${MBED_TARGET}.cmake)

# check if a custom upload config path is defined in top lvl cmake
if((DEFINED CUSTOM_UPLOAD_CFG_PATH))
    # Make path absolute, as required by EXISTS
    get_filename_component(CUSTOM_UPLOAD_CFG_PATH "${CUSTOM_UPLOAD_CFG_PATH}" ABSOLUTE BASE_DIR ${CMAKE_SOURCE_DIR})
    if(EXISTS ${CUSTOM_UPLOAD_CFG_PATH})
        include(${CUSTOM_UPLOAD_CFG_PATH})
        message(STATUS "Mbed: Custom upload config included from ${CUSTOM_UPLOAD_CFG_PATH}")
    else()
        message(FATAL_ERROR "Mbed: Custom upload config is defined but files was not found here - ${CUSTOM_UPLOAD_CFG_PATH}")
    endif()

    # check if a custom upload config is present in custom_targets/YOUR_TARGET folder
elseif(EXISTS ${EXPECTED_CUSTOM_UPLOAD_CFG_FILE_PATH})
    include(${EXPECTED_CUSTOM_UPLOAD_CFG_FILE_PATH})
    message(STATUS "Mbed: Custom upload config included from ${EXPECTED_CUSTOM_UPLOAD_CFG_FILE_PATH}")

    # check for build in upload config
elseif(EXISTS ${EXPECTED_MBED_UPLOAD_CFG_FILE_PATH})
    include(${EXPECTED_MBED_UPLOAD_CFG_FILE_PATH})
    message(STATUS "Mbed: Loading default upload method configuration from ${EXPECTED_MBED_UPLOAD_CFG_FILE_PATH}")
else()
    message(STATUS "Mbed: Target does not have any upload method configuration.  'make flash-' commands will not be available unless configured by the upper-level project.")
    set(UPLOAD_METHOD_DEFAULT "NONE")
endif()

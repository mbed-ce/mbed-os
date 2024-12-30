# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Script responsible for generating the Mbed OS CMake configuration file (mbed_config.cmake) automatically
# when needed

# Input variables:
#  MBED_TARGET - Mbed OS target name that you want to build for
#  MBED_APP_JSON_PATH - Path to mbed_app.json.  If relative, will be interpreted relative to CMAKE_SOURCE_DIR.
#  CUSTOM_TARGETS_JSON_PATH - Full path to custom_targets.json, may be set to empty/undefined if not using it.  If relative, will be interpreted relative to CMAKE_SOURCE_DIR.

set(MBED_NEED_TO_RECONFIGURE FALSE)

# First, verify that MBED_TARGET has not changed
if(DEFINED MBED_INTERNAL_LAST_MBED_TARGET)
    if(NOT "${MBED_INTERNAL_LAST_MBED_TARGET}" STREQUAL "${MBED_TARGET}")
        message(FATAL_ERROR "MBED_TARGET has changed since this directory was originally configured. (previously was: ${MBED_INTERNAL_LAST_MBED_TARGET}).  Due to CMake limitations, this build dir must be deleted and recreated from scratch (or use cmake --fresh [CMake 3.24 or newer]).")
    endif()
else()
    if("${MBED_TARGET}" STREQUAL "")
        message(FATAL_ERROR "You must define MBED_TARGET to the Mbed target you wish to build for!")
    endif()

    # First run of CMake
    message(STATUS "Mbed: First CMake run detected, generating configs...")
    set(MBED_NEED_TO_RECONFIGURE TRUE)
endif()
set(MBED_INTERNAL_LAST_MBED_TARGET "${MBED_TARGET}" CACHE INTERNAL "Previous mbed target this dir was configured with" FORCE)

# Check if the include file is missing (e.g. because a previous attempt to generate it failed)
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/mbed_config.cmake)
    if(NOT MBED_NEED_TO_RECONFIGURE)
        message(STATUS "Mbed: mbed_config.cmake not found, regenerating configs...")
        set(MBED_NEED_TO_RECONFIGURE TRUE)
    endif()
else()
    # Include the old version of mbed_config.cmake to get the MBED_CONFIG_JSON_SOURCE_FILES variable used below
    include(${CMAKE_CURRENT_BINARY_DIR}/mbed_config.cmake)
endif()

# Check timestamps on all JSON files used to generate the Mbed configuration
if(NOT MBED_NEED_TO_RECONFIGURE)
    file(TIMESTAMP ${CMAKE_CURRENT_BINARY_DIR}/mbed_config.cmake MBED_CONFIG_CMAKE_TIMESTAMP "%s" UTC)

    foreach(CONFIG_JSON ${MBED_CONFIG_JSON_SOURCE_FILES})
        get_filename_component(CONFIG_JSON_ABSPATH ${CONFIG_JSON} ABSOLUTE)

        if(NOT EXISTS ${CONFIG_JSON_ABSPATH})
            message(STATUS "Mbed: ${CONFIG_JSON} deleted or renamed, regenerating configs...")
            set(MBED_NEED_TO_RECONFIGURE TRUE)
            break()
        endif()

        file(TIMESTAMP ${CONFIG_JSON_ABSPATH} CONFIG_JSON_TIMESTAMP "%s" UTC)
        if(${CONFIG_JSON_TIMESTAMP} GREATER ${MBED_CONFIG_CMAKE_TIMESTAMP})
            message(STATUS "Mbed: ${CONFIG_JSON} modified, regenerating configs...")
            set(MBED_NEED_TO_RECONFIGURE TRUE)
            break()
        endif()
    endforeach()
endif()

# Convert all relative paths to absolute paths, rooted at CMAKE_SOURCE_DIR.
# This makes sure that they are interpreted the same way everywhere.
get_filename_component(MBED_APP_JSON_PATH "${MBED_APP_JSON_PATH}" ABSOLUTE BASE_DIR ${CMAKE_SOURCE_DIR})
get_filename_component(CUSTOM_TARGETS_JSON_PATH "${CUSTOM_TARGETS_JSON_PATH}" ABSOLUTE BASE_DIR ${CMAKE_SOURCE_DIR})

if(MBED_NEED_TO_RECONFIGURE)
    # Generate mbed_config.cmake for this target
    if(EXISTS "${MBED_APP_JSON_PATH}" AND (NOT IS_DIRECTORY "${MBED_APP_JSON_PATH}"))
        set(APP_CONFIG_ARGUMENT --app-config "${MBED_APP_JSON_PATH}")
    else()
        set(APP_CONFIG_ARGUMENT "")
    endif()

    if(EXISTS "${CUSTOM_TARGETS_JSON_PATH}" AND (NOT IS_DIRECTORY "${CUSTOM_TARGETS_JSON_PATH}"))
        set(CUSTOM_TARGET_ARGUMENT --custom-targets-json "${CUSTOM_TARGETS_JSON_PATH}")
    else()
        set(CUSTOM_TARGET_ARGUMENT "")
    endif()

    # Make sure an old config file doesn't stick around
    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/mbed_config.cmake)

    set(MBEDTOOLS_CONFIGURE_COMMAND ${Python3_EXECUTABLE}
        -m mbed_tools.cli.main
        -v -v # without at least -v, warnings (e.g. "you have tried to override a nonexistent parameter") do not get printed
        configure
        -t GCC_ARM # GCC_ARM is currently the only supported toolchain
        -m "${MBED_TARGET}"
        --mbed-os-path ${CMAKE_CURRENT_LIST_DIR}/../..
        --output-dir ${CMAKE_CURRENT_BINARY_DIR}
        --program-path ${CMAKE_SOURCE_DIR}
        ${APP_CONFIG_ARGUMENT}
        ${CUSTOM_TARGET_ARGUMENT})

    execute_process(
        COMMAND ${MBEDTOOLS_CONFIGURE_COMMAND}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../python
        RESULT_VARIABLE MBEDTOOLS_CONFIGURE_RESULT
        OUTPUT_VARIABLE MBEDTOOLS_CONFIGURE_OUTPUT
        ERROR_VARIABLE MBEDTOOLS_CONFIGURE_ERROR_OUTPUT
        ECHO_OUTPUT_VARIABLE
        ECHO_ERROR_VARIABLE
    )

    if((NOT MBEDTOOLS_CONFIGURE_RESULT EQUAL 0) OR (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/mbed_config.cmake))
        string(JOIN " " MBEDTOOLS_COMMAND_SPC_SEP ${MBEDTOOLS_CONFIGURE_COMMAND})
        message(FATAL_ERROR "mbedtools configure failed!  Cannot build this project.  Command was cd ${CMAKE_CURRENT_LIST_DIR}/../python && ${MBEDTOOLS_COMMAND_SPC_SEP}")
    endif()

endif()

# Include the generated config file
include(${CMAKE_CURRENT_BINARY_DIR}/mbed_config.cmake)

# Make it so that if any config JSON files are modified, CMake is rerun.
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${MBED_CONFIG_JSON_SOURCE_FILES})
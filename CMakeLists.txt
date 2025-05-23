# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This is the boilerplate for Mbed OS

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)
cmake_policy(VERSION 3.19...3.22)

# Setup build type (target type, tests/unit tests/real build) ----------------------------------------------------------------------------------
# This block sets up the following variables for all subdirs to use:
# - MBED_IS_STANDALONE: True if Mbed OS is the top-level project.  False if Mbed is being built as part of an application.
# - MBED_IS_NATIVE_BUILD: True if we are building for the host machine.  False if we are building for a microcontroller
# - MBED_ENABLE_OS_INTERNAL_TESTS: True if we are building *any* internal Mbed OS tests at all.  Enabled by -DMBED_ENABLE_TESTING=TRUE (which is enabled by default when standalone).
# - MBED_BUILD_GREENTEA_TESTS: True to build greentea on-target tests.  False to build host UNITTESTS.  Defaults to false when standalone.


if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # We are the top level project, so tests or unittests are being built.
    set(MBED_IS_STANDALONE TRUE)
else()
    # Not the top level project
    set(MBED_IS_STANDALONE FALSE)
endif()

# Set up options for testing
option(MBED_ENABLE_TESTING "Whether to enable CTest tests in this project" ${MBED_IS_STANDALONE}) # This option is also created by include(CTest) but we need it here earlier on.
if(MBED_IS_STANDALONE AND MBED_ENABLE_TESTING)
    set(MBED_ENABLE_OS_INTERNAL_TESTS TRUE)
    option(MBED_BUILD_GREENTEA_TESTS "Build greentea tests instead of unit tests" FALSE)
endif()

# Figure out if this is a native build
if(MBED_IS_STANDALONE)

    # Standalone build, use MBED_BUILD_GREENTEA_TESTS to determine if we are building for native or not (user can select)
    if(MBED_BUILD_GREENTEA_TESTS)
        set(MBED_IS_NATIVE_BUILD FALSE)
    else()
        set(MBED_IS_NATIVE_BUILD TRUE)
    endif()

else()

    # Building as a subdir.  This means that the top-level project will already have called project(), so we can
    # rely on CMake's platform detection.
    if(CMAKE_CROSSCOMPILING)
        set(MBED_IS_NATIVE_BUILD FALSE)
    else()
        set(MBED_IS_NATIVE_BUILD TRUE)
    endif()

endif()

if(MBED_IS_STANDALONE AND NOT MBED_IS_NATIVE_BUILD)
    # For standalone builds, default to looking for mbed-config.cmake in the binary dir
    set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "")

    # Initialize Mbed build system
    include(${CMAKE_CURRENT_LIST_DIR}/tools/cmake/mbed_toolchain_setup.cmake)
endif()

# Create project and find compilers (if not already found)
project(mbed-os LANGUAGES C CXX ASM)

if(MBED_IS_STANDALONE AND NOT MBED_IS_NATIVE_BUILD)
    include(mbed_project_setup)
endif()

if(MBED_IS_NATIVE_BUILD)
    # Pick up some include files that are needed later
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/tools/cmake)
    include(mbed_create_distro)
else()
    include(mbed_set_linker_script)
endif()

# Print build type
if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        message(STATUS "Mbed: Compiling Greentea on-target tests for ${MBED_TARGET}")
    else()
        message(STATUS "Mbed: Compiling host UNITTESTS for native execution")
    endif()
else()
    message(STATUS "Mbed: Not building any Mbed OS tests.")
endif()

# If this is Mbed CI and we have an upload method file, include it
if(DEFINED MBED_OS_CI_UPLOAD_METHOD_CONFIG)
    include(${MBED_OS_CI_UPLOAD_METHOD_CONFIG})
endif()

# Create core Mbed OS targets and set up build flags  ----------------------------------------------------------------------------------

# Add all paths to the module files within Mbed OS
list(APPEND CMAKE_MODULE_PATH
    ${mbed-os_SOURCE_DIR}/platform/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_TFM/TARGET_TFM_LATEST/scripts
    ${mbed-os_SOURCE_DIR}/targets/TARGET_Cypress/scripts
    ${mbed-os_SOURCE_DIR}/targets/TARGET_NXP/scripts
    ${mbed-os_SOURCE_DIR}/targets/TARGET_NUVOTON/scripts/
    ${mbed-os_SOURCE_DIR}/tools/cmake/upload_methods
)

add_subdirectory(extern)

if(MBED_IS_STANDALONE)
    include(CTest)

    if((NOT MBED_BUILD_GREENTEA_TESTS) AND MBED_ENABLE_TESTING)
        # Building unit tests only.
        add_definitions(-DUNITTEST)
        add_subdirectory(UNITTESTS)
    endif()
endif()

if(MBED_ENABLE_TESTING)
    include(mbed_greentea)
endif()

# These targets are made visible here so their source files which
# are spread in different directories can be referenced and can be linked against
# by libraries that depend on them.
add_library(mbed-rtos-flags INTERFACE) # Collects flags that are in mbed-os but not mbed-baremetal
add_library(mbed-rtos-sources INTERFACE) # Collects source files that are in mbed-os but not mbed-baremetal
add_library(mbed-core-flags INTERFACE) # Collects flags that are in mbed-os or mbed-baremetal
                                       # depending on target.application-profile setting
add_library(mbed-core-sources INTERFACE) # Collects source files that are in mbed-os or mbed-baremetal
                                         # depending on target.application-profile setting

if("MBED_CONF_TARGET_APPLICATION_PROFILE=full" IN_LIST MBED_CONFIG_DEFINITIONS OR MBED_IS_NATIVE_BUILD)
    set(APPLICATION_PROFILE_CONFIG_FULL TRUE)
elseif("MBED_CONF_TARGET_APPLICATION_PROFILE=bare-metal" IN_LIST MBED_CONFIG_DEFINITIONS)
    set(APPLICATION_PROFILE_CONFIG_BAREMETAL TRUE)
else()
    message(FATAL_ERROR "target.application-profile must be set to either 'full' or 'bare-metal'! If you are getting this after upgrading mbed-os then you likely need to clear your CMake cache.")
endif()

# mbed-core-flags/mbed-core-sources link mbed-rtos-flags/mbed-rtos-sources or not depending on
# target.application-profile setting
if(APPLICATION_PROFILE_CONFIG_FULL)
    target_link_libraries(mbed-core-flags INTERFACE mbed-rtos-flags)
    target_link_libraries(mbed-core-sources INTERFACE mbed-rtos-sources)
endif()

# Validate selected C library type
# The C library type selected has to match the library that the target can support
if(NOT MBED_IS_NATIVE_BUILD)
    if(${MBED_C_LIB} STREQUAL "small")
        if(NOT "small" IN_LIST MBED_TARGET_SUPPORTED_C_LIBS)
            if("std" IN_LIST MBED_TARGET_SUPPORTED_C_LIBS)
                message(WARNING
                    "We noticed that target.c_lib is set to `${MBED_C_LIB}`."
                    " As the ${MBED_TARGET} target does not support a small C library for the ${MBED_TOOLCHAIN} toolchain,"
                    " we are using the standard C library instead."
                )
                set(MBED_C_LIB "std" CACHE STRING "")
            endif()
        endif()
    elseif(NOT ${MBED_C_LIB} IN_LIST MBED_TARGET_SUPPORTED_C_LIBS)
        message(FATAL_ERROR
            "Invalid `target.c_lib` ('${MBED_C_LIB}') for '${MBED_TARGET}' target."
            "\nPossible value(s): ${MBED_TARGET_SUPPORTED_C_LIBS}"
        )
    endif()

    # Validate selected printf library
    set(MBED_PRINTF_LIB_TYPES std minimal-printf)
    if(NOT ${MBED_PRINTF_LIB} IN_LIST MBED_PRINTF_LIB_TYPES)
        message(FATAL_ERROR
            "Invalid printf library type '${MBED_PRINTF_LIB}'. Possible values:\n ${MBED_PRINTF_LIB_TYPES}"
        )
    endif()

    mbed_set_cpu_core_definitions(mbed-core-flags)
    if(${MBED_TOOLCHAIN_FILE_USED})
        mbed_set_profile_options(mbed-core-flags ${MBED_TOOLCHAIN})
        mbed_set_c_lib(mbed-core-flags ${MBED_C_LIB})
        mbed_set_printf_lib(mbed-core-flags ${MBED_PRINTF_LIB})

        target_compile_features(mbed-core-flags
            INTERFACE
                c_std_11
                cxx_std_14
        )

    endif()

    target_compile_definitions(mbed-core-flags
        INTERFACE
            TARGET_NAME=${MBED_TARGET}
    )

    # Add MBED_TEST_MODE for backward compatibility with Greentea tests written for use with Mbed CLI 1
    if(MBED_ENABLE_OS_INTERNAL_TESTS)
        if(NOT MBED_BUILD_GREENTEA_TESTS)
            target_compile_definitions(${PROJECT_NAME}
                INTERFACE
                    MBED_TEST_MODE
            )
        endif()
    endif()

    # We need to generate a "response file" to pass to the C preprocessor when we preprocess the linker
    # script, because of path length limitations on Windows. We set the response file and bind the path
    # to a global property here. The MBED_TARGET being built queries this global property when it sets
    # the linker script.
    #
    # We must set this global property before the targets subdirectory is added to the project. This is
    # required because the MBED_TARGET depends on the response file. If the path to the response file
    # is not defined when the target requests it the config definitions will not be passed to CPP.
    mbed_generate_options_for_linker(mbed-core-flags RESPONSE_FILE_PATH)
    set_property(GLOBAL PROPERTY LINKER_SCRIPT_PREPROCESS_FLAGS_RESPONSE_FILE ${RESPONSE_FILE_PATH})

    # Add compile definitions for backward compatibility with the toolchain
    # supported. New source files should instead check for __GNUC__ and __clang__
    # for the GCC_ARM and ARM toolchains respectively.
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        target_compile_definitions(mbed-core-flags
            INTERFACE
                TOOLCHAIN_GCC_ARM
                TOOLCHAIN_GCC
        )
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        target_compile_definitions(mbed-core-flags
            INTERFACE
                TOOLCHAIN_ARM
        )
    endif()


    # Ensure the words that make up the Mbed target name are separated with a hyphen, lowercase, and with the `mbed-` prefix.
    string(TOLOWER ${MBED_TARGET} MBED_TARGET_CMAKE_NAME)
    string(REPLACE "_" "-" MBED_TARGET_CMAKE_NAME ${MBED_TARGET_CMAKE_NAME})
    string(PREPEND MBED_TARGET_CMAKE_NAME "mbed-")

    # Load upload method if one is set up
    include(UploadMethodManager)

    # Load debug config generator for IDEs
    include(mbed_ide_debug_cfg_generator)
endif()

if(MBED_IS_NATIVE_BUILD)
    # Fix issue on Windows with object files hitting a limit for number of sections
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        add_compile_options(-Wa,-mbig-obj)
    endif()
endif()

# Generate target config header and include it in all files
if(NOT MBED_IS_NATIVE_BUILD)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated-headers)
    mbed_write_target_config_header(${CMAKE_CURRENT_BINARY_DIR}/generated-headers/mbed-target-config.h MBED_TARGET_DEFINITIONS MBED_CONFIG_DEFINITIONS)
    target_compile_options(mbed-core-flags INTERFACE -include ${CMAKE_CURRENT_BINARY_DIR}/generated-headers/mbed-target-config.h)
    target_include_directories(mbed-core-flags INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/generated-headers)
endif()

# Include mbed.h and config from generate folder
target_include_directories(mbed-core-flags
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Recurse to subdirs  ----------------------------------------------------------------------------------

# Include targets/ first, because any post-build hook needs to be defined
# before other parts of Mbed OS can add greentea tests which require
# mbed_set_post_build().
add_subdirectory(targets)

if((NOT MBED_IS_NATIVE_BUILD) AND (NOT TARGET ${MBED_TARGET_CMAKE_NAME}))
    message(FATAL_ERROR "CMake target ${MBED_TARGET_CMAKE_NAME} was not found after scanning the mbed-os/targets \
directory.  If this is a custom target, you need to define a target called ${MBED_TARGET_CMAKE_NAME} before doing: \
add_subdirectory(mbed-os)")
endif()

if(NOT MBED_IS_NATIVE_BUILD)
    # Disable any requested files from the targets/ directory.
    mbed_apply_mcu_target_file_disables()
endif()

add_subdirectory(cmsis)
add_subdirectory(drivers)
add_subdirectory(hal)
add_subdirectory(platform)
add_subdirectory(rtos)
add_subdirectory(storage)
add_subdirectory(events)
add_subdirectory(connectivity)

# The directories below contain optional target libraries
add_subdirectory(drivers/device_key EXCLUDE_FROM_ALL)
add_subdirectory(features EXCLUDE_FROM_ALL)
add_subdirectory(cmsis/CMSIS_5/CMSIS/RTOS2 EXCLUDE_FROM_ALL)
add_subdirectory(cmsis/device/rtos EXCLUDE_FROM_ALL)

# Create top-level targets  ----------------------------------------------------------------------------------

if(NOT MBED_IS_NATIVE_BUILD)
    # Core Mbed OS library
    if(APPLICATION_PROFILE_CONFIG_FULL)
        set(MBED_OS_CORE_LIB_NAME mbed-os)
    else()
        set(MBED_OS_CORE_LIB_NAME mbed-baremetal)
    endif()
    mbed_create_distro(${MBED_OS_CORE_LIB_NAME} ${MBED_TARGET_CMAKE_NAME} mbed-core-flags mbed-core-sources)

    # Set up the linker script and hook it up to the top-level OS target
    mbed_setup_linker_script(${MBED_OS_CORE_LIB_NAME} ${CMAKE_CURRENT_BINARY_DIR}/generated-headers/mbed-target-config.h)

    # Make sure that things linking mbed-core-flags can also get the target-specific include dirs and flags.
    mbed_extract_flags(${MBED_TARGET_CMAKE_NAME}-flags ${MBED_TARGET_CMAKE_NAME})
    target_link_libraries(mbed-core-flags INTERFACE ${MBED_TARGET_CMAKE_NAME}-flags)
endif()

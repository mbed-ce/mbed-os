# Copyright (c) 2025 MbedCE Community Contributors (Jan Kamidra)
# SPDX-License-Identifier: Apache-2.0

# Get the active STM32 family label from MBED_TARGET_LABELS
#
# Usage:
#   get_stm32_family(<out_var>)
#
function(get_stm32_family out_var)
    set(temp_stm32 "")

    foreach(_label IN LISTS MBED_TARGET_LABELS)
        if(_label MATCHES "^STM32([A-Z][0-9]|WB|WL)$")
            set(temp_stm32 "${_label}")
            break()
        endif()
    endforeach()

    if(temp_stm32 STREQUAL "")
        message(FATAL_ERROR "No STM32 family label found in MBED_TARGET_LABELS. Expected a label in the format STM32<family>, e.g. STM32F4.")
    endif()

    set(${out_var} "${temp_stm32}" PARENT_SCOPE)
endfunction()

# Get system clock file path from MBED_TARGET_LABELS.
#
# Usage:
#   get_system_clock_file(<out_var> <label_prefix>)
#
# Label format accepted:
#   STM32F4_180MHZ -> clock_cfg/TARGET_STM32F4_180MHZ/system_clock.c
function(get_system_clock_file out_var label_prefix)
    set(temp_system_clock_file "")

    foreach(_label IN LISTS MBED_TARGET_LABELS)
        if(_label MATCHES "^${label_prefix}_[0-9_]+MHZ$")
            set(_system_clock_candidate "clock_cfg/TARGET_${_label}/system_clock.c")

            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_system_clock_candidate}")
                set(temp_system_clock_file "${_system_clock_candidate}")
                break()
            else()
                message(FATAL_ERROR "System clock file was not found ${_system_clock_candidate}")
            endif()
        endif()
    endforeach()

    if(temp_system_clock_file STREQUAL "")
        message(FATAL_ERROR "No system clock label was found in MBED_TARGET_LABELS. Expected a label in the format ${label_prefix}_<freq>, e.g. ${label_prefix}_180MHZ")
    endif()

    set(${out_var} "${temp_system_clock_file}" PARENT_SCOPE)
endfunction()

# Get startup assembly file path from MBED_TARGET_LABELS.
#
# Usage:
#   get_startup_file(<out_var> <startup_templates_dir>)
#
# Label format accepted:
#   STARTUP_STM32F429xx -> startup_stm32f429xx.s
function(get_startup_file out_var startup_templates_dir)
    set(temp_startup_file "")
    set(temp_startup_file_name "")

    foreach(_label IN LISTS MBED_TARGET_LABELS)
        if(_label MATCHES "^STARTUP_[0-9A-Za-z_]+$")
            string(REGEX REPLACE "^STARTUP_" "" _startup_suffix "${_label}")
            string(TOLOWER "${_startup_suffix}" _startup_suffix_lc)
            set(temp_startup_file_name "startup_${_startup_suffix_lc}.s")
            break()
        endif()
    endforeach()

    if(NOT temp_startup_file_name STREQUAL "")
        set(_startup_candidate "${startup_templates_dir}/${temp_startup_file_name}")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_startup_candidate}")
            set(temp_startup_file "${_startup_candidate}")
        else()
            message(FATAL_ERROR "Startup file was not found ${_startup_candidate}")
        endif()
    else()
        message(FATAL_ERROR "No startup label was found in MBED_TARGET_LABELS. Expected a label in the format STARTUP_<suffix>, e.g. STARTUP_STM32F429xx")
    endif()
    
    set(${out_var} "${temp_startup_file}" PARENT_SCOPE)
endfunction()

# Read, patch and write a generated startup file into the current binary directory.
#
# Usage:
#   patch_startup_file(<out_var> <startup_file>)
#
# Cache options:
#   STM_PATCH_STARTUP_ENTRY_FLOW  : enable default entry-flow patch (default ON)
#   STM_STARTUP_PATCH_REGEX       : semicolon-separated regex list
#   STM_STARTUP_PATCH_REPLACE     : semicolon-separated replacement list
function(patch_startup_file out_var startup_file)
    if(startup_file STREQUAL "")
        message(FATAL_ERROR "patch_startup_file() received an empty startup file path")
    endif()

    set(STM_PATCH_STARTUP_ENTRY_FLOW ON CACHE BOOL "Patch STM32 startup entry flow for Mbed startup")
    set(STM_STARTUP_PATCH_REGEX "" CACHE STRING "Regex patterns applied to generated STM32 startup file")
    set(STM_STARTUP_PATCH_REPLACE "" CACHE STRING "Replacement strings for STM_STARTUP_PATCH_REGEX")

    set(_startup_source "${CMAKE_CURRENT_SOURCE_DIR}/${startup_file}")
    if(NOT EXISTS "${_startup_source}")
        message(FATAL_ERROR "Startup file not found: ${_startup_source}")
    endif()

    get_filename_component(_startup_file_name "${startup_file}" NAME)
    set(_startup_file_gen "${CMAKE_CURRENT_BINARY_DIR}/${_startup_file_name}")

    file(READ "${_startup_source}" _startup_content)

    # Normalize line endings so line-based regex replacements behave consistently.
    string(REPLACE "\r\n" "\n" _startup_content "${_startup_content}")
    string(REPLACE "\r" "\n" _startup_content "${_startup_content}")

    if(STM_PATCH_STARTUP_ENTRY_FLOW)
        string(REGEX REPLACE "\n[ \t]*bl[ \t]+SystemInit[ \t]*\n" "\n" _startup_content "${_startup_content}")
        string(REGEX REPLACE "\n[ \t]*bl[ \t]+__libc_init_array[ \t]*\n" "\n" _startup_content "${_startup_content}")
        string(REGEX REPLACE "\n[ \t]*bl[ \t]+main[ \t]*\n" "\n  bl  SystemInit   \n  bl _start\n" _startup_content "${_startup_content}")
    endif()

    if(NOT STM_STARTUP_PATCH_REGEX STREQUAL "")
        set(_startup_patch_regex_list ${STM_STARTUP_PATCH_REGEX})
        set(_startup_patch_replace_list ${STM_STARTUP_PATCH_REPLACE})
        list(LENGTH _startup_patch_regex_list _startup_patch_regex_count)
        list(LENGTH _startup_patch_replace_list _startup_patch_replace_count)

        if(NOT _startup_patch_regex_count EQUAL _startup_patch_replace_count)
            message(FATAL_ERROR "STM_STARTUP_PATCH_REGEX and STM_STARTUP_PATCH_REPLACE must have the same number of entries")
        endif()

        if(_startup_patch_regex_count GREATER 0)
            math(EXPR _startup_patch_last "${_startup_patch_regex_count} - 1")
            foreach(_idx RANGE 0 ${_startup_patch_last})
                list(GET _startup_patch_regex_list ${_idx} _startup_patch_regex)
                list(GET _startup_patch_replace_list ${_idx} _startup_patch_replace)
                string(REGEX REPLACE "${_startup_patch_regex}" "${_startup_patch_replace}" _startup_content "${_startup_content}")
            endforeach()
        endif()
    endif()

    file(WRITE "${_startup_file_gen}" "${_startup_content}")

    set(${out_var} "${_startup_file_gen}" PARENT_SCOPE)
endfunction()

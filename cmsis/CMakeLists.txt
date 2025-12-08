# Copyright (c) 2025 MbedCE Community Contributors (Jan Kamidra)
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Created by: Jan Kamidra with GitHub Copilot

# ============================================================================
# Mbed-specific boot code, RTOS configuration
# ============================================================================
add_subdirectory(device)

# CMSIS Integration for MbedCE
# ============================================================================
# CMSIS-Core
# ============================================================================
#
# This file integrates CMSIS Core from external submodules CMSIS_6 without
# modifying the submodule contents. 
# Provides Core headers for Cortex-M and Cortex-A processors which are 
# needed for all builds (bare-metal and RTOS)
#
# Submodule: CMSIS 6 (https://github.com/ARM-software/CMSIS_6)
# Components used: CMSIS-Core 6.1.1
# Path: CMSIS_6/CMSIS/Core

# This include is just for github tests 
include(${CMAKE_CURRENT_LIST_DIR}/../tools/cmake/mbed_submodule_management.cmake)

# Manage CMSIS_6 submodule
mbed_setup_submodule(CMSIS_6 CHECK_FILE CMSIS/Core/Include/cmsis_gcc.h)

# CMSIS_6 base path
set(CMSIS_CORE_BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/CMSIS_6/CMSIS/Core)

# Add CMSIS-Core base include directories
target_include_directories(mbed-core-flags
    INTERFACE
        ${CMSIS_CORE_BASE_DIR}/Include 
)

if("CORTEX_A" IN_LIST MBED_TARGET_LABELS)
    # Cortex-A startup
    add_library(mbed-cmsis-cortex-a INTERFACE)

    target_include_directories(mbed-cmsis-cortex-a
        INTERFACE
             ${CMSIS_CORE_BASE_DIR}/Include/a-profile
    )
    
    target_sources(mbed-cmsis-cortex-a
        INTERFACE
             ${CMSIS_CORE_BASE_DIR}/Source/irq_ctrl_gic.c
    )

elseif("CORTEX_M" IN_LIST MBED_TARGET_LABELS)
    # Cortex-M startup
    add_library(mbed-cmsis-cortex-m INTERFACE)

    target_include_directories(mbed-cmsis-cortex-m
        INTERFACE
             ${CMSIS_CORE_BASE_DIR}/Include/m-profile 
    )
    
    target_sources(mbed-cmsis-cortex-m
        INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/device/rtos/source/mbed_tz_context.c
    )
endif()

# ============================================================================
# CMSIS-RTOS2 API
# ============================================================================
#
# CMSIS-RTOS2 API Real-time OS API provides RTOS API headers
# The API is backend-agnostic - works with RTX, FreeRTOS, or other RTOS
# These headers are needed even for bare-metal builds (for type definitions)
#
# Submodule: CMSIS 6 (https://github.com/ARM-software/CMSIS_6)
# Components used: CMSIS-RTOS2 API 2.3.0
# Path: /CMSIS_6/CMSIS/RTOS2

# CMSIS_6_RTOS2 base path
set(CMSIS_RTOS2_BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/CMSIS_6/CMSIS/RTOS2)

target_include_directories(mbed-core-flags
    INTERFACE
        ${CMSIS_RTOS2_BASE_DIR}/Include
)

# OS Tick source files
if("CORTEX_A" IN_LIST MBED_TARGET_LABELS)
    # Cortex-A startup
    target_sources(mbed-rtos-sources INTERFACE
        ${CMSIS_RTOS2_BASE_DIR}/Source/os_tick_ptim.c
    )
elseif("CORTEX_M" IN_LIST MBED_TARGET_LABELS)
    # Cortex-M startup
    target_sources(mbed-rtos-sources INTERFACE
        ${CMSIS_RTOS2_BASE_DIR}/Source/os_systick.c
    )
endif()

# ============================================================================
# RTOS Backend Selection
# Note: this is preparation for future support of multiple RTOS backends
# ============================================================================

# Determine RTOS backend from config/macros (default: RTX)
set(MBED_RTOS_BACKEND "RTX")

# Prefer library config: MBED_CONF_RTOS_BACKEND
foreach(def ${MBED_CONFIG_DEFINITIONS})
    if(def MATCHES "^MBED_CONF_RTOS_BACKEND=(.+)$")
        string(REGEX REPLACE "^MBED_CONF_RTOS_BACKEND=(.+)$" "\\1" _backend "${def}")
        set(MBED_RTOS_BACKEND "${_backend}")
    endif()
endforeach()

# Fallback: user macro MBED_RTOS_BACKEND
if(MBED_RTOS_BACKEND STREQUAL "RTX")
    foreach(def ${MBED_CONFIG_DEFINITIONS})
        if(def MATCHES "^MBED_RTOS_BACKEND=(.+)$")
            string(REGEX REPLACE "^MBED_RTOS_BACKEND=(.+)$" "\\1" _backend "${def}")
            set(MBED_RTOS_BACKEND "${_backend}")
        endif()
    endforeach()
endif()

if(MBED_RTOS_BACKEND STREQUAL "RTX")
    # ============================================================================
    # CMSIS-RTX (RTOS Backend)
    # ============================================================================
    #
    # CMSIS-RTX provides the RTX5 RTOS kernel implementation for CMSIS-RTOS2 API
    #
    # Submodule: CMSIS-RTX (https://github.com/ARM-software/CMSIS-RTX)
    # Components used: CMSIS-RTX 5.9.0
    # Path: /CMSIS-RTX

    # Manage CMSIS-RTX submodule
    mbed_setup_submodule(CMSIS-RTX CHECK_FILE Include/rtx_os.h)

    # CMSIS-RTX base path
    set(CMSIS_RTX_BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/CMSIS-RTX)

    # Add toolchain-specific interrupt handlers based on core type
    if(${CMAKE_CROSSCOMPILING})
        set(RTX_IRQ_FILE_PATH ${CMSIS_RTX_BASE_DIR}/Source/GCC)
        set(_cm0_variants M0 M0P)
        set(_cmx_variants M3 M4 M7)
        set(_cmxx_variants M33 M35 M35P M55 M85)

        foreach(key ${MBED_TARGET_LABELS})
            if(${key} STREQUAL CORTEX_A)
                # Cortex-A uses ARMv7-A
                target_sources(mbed-rtos-sources INTERFACE ${CMSIS_RTX_BASE_DIR}/Config/handlers.c)
                set(RTX_IRQ_FILE "irq_armv7a.S")
            elseif(${key} STREQUAL M23)
                # Cortex-M23 uses ARMv8-M
                set(RTX_IRQ_FILE "irq_armv8mbl.S")
            elseif(${key} IN_LIST _cm0_variants)
                # Cortex-M0/M0+ use ARMv6-M
                set(RTX_IRQ_FILE "irq_armv6m.S")
            elseif(${key} IN_LIST _cmx_variants)
                # Cortex-M3/M4/M7 use ARMv7-M
                set(RTX_IRQ_FILE "irq_armv7m.S")
            elseif(${key} IN_LIST _cmxx_variants)
                # Cortex-M33/M35/M55/M85 use ARMv8-M
                set(RTX_IRQ_FILE "irq_armv8mml.S")
            endif()
        endforeach()
        
        # Add the IRQ handler file if determined
        if(EXISTS ${RTX_IRQ_FILE_PATH}/${RTX_IRQ_FILE})
            target_sources(mbed-rtos-sources INTERFACE ${RTX_IRQ_FILE_PATH}/${RTX_IRQ_FILE})
        else()
            message(FATAL_ERROR "CMSIS-RTX: No IRQ handler found for this target (labels: ${MBED_TARGET_LABELS} )")
        endif()
    endif()

    # Make RTX headers and Mbed's device RTOS wrapper headers visible to ALL profiles
    # (needed for typedefs like mbed_rtos_storage_* used by headers even in bare-metal)
    target_include_directories(mbed-core-flags
        BEFORE
        INTERFACE
            # Do not touch this order - overrides must come first
            ${CMAKE_CURRENT_LIST_DIR}/device/rtos/include/RTX/RTX_overrides
            ${CMAKE_CURRENT_LIST_DIR}/device/rtos/include/RTX
            ${CMSIS_RTX_BASE_DIR}/Include
            ${CMSIS_RTX_BASE_DIR}/Source
    )

    # RTX source files (only compiled when building with RTOS)
    target_sources(mbed-rtos-sources
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/device/rtos/source/RTX/mbed_rtos_rtx.c
        ${CMAKE_CURRENT_LIST_DIR}/device/rtos/source/RTX/mbed_rtx_handlers.c
        ${CMAKE_CURRENT_LIST_DIR}/device/rtos/source/RTX/mbed_rtx_idle.cpp
        # Old CMSIS-RTOS v1 compatibility layer
        ${CMAKE_CURRENT_LIST_DIR}/device/rtos/source/cmsis_os1.c     
        # Configuration
        ${CMSIS_RTX_BASE_DIR}/Config/RTX_Config.c      
        # RTX kernel sources
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_delay.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_evflags.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_evr.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_kernel.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_lib.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_memory.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_mempool.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_msgqueue.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_mutex.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_semaphore.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_system.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_thread.c
        ${CMSIS_RTX_BASE_DIR}/Source/rtx_timer.c
    )
else()
    message(FATAL_ERROR "This RTOS backend ${MBED_RTOS_BACKEND} is not supported!")
endif()

# ============================================================================
# Testing Support
# ============================================================================
if(MBED_ENABLE_OS_INTERNAL_TESTS)
    add_subdirectory(tests/UNITTESTS/doubles)
endif()
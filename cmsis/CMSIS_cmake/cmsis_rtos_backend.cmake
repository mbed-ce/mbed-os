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
# RTOS Backend Selection
# Note: this is preparation for future support of multiple RTOS backends
# ============================================================================

# TODO:
# 1. Check for Submodules are present (CMSIS-RTX, CMSIS-FreeRTOS, etc.)
# 2. Support additional RTOS backends (e.g., FreeRTOS) in future

# Determine RTOS backend from config/macros (default: RTX)
set(MBED_RTOS_BACKEND "RTX")

# Prefer library config: MBED_CONF_RTOS_BACKEND
foreach(def ${MBED_CONFIG_DEFINITIONS})
    if(def STREQUAL "MBED_CONF_RTOS_BACKEND=RTX")
        set(MBED_RTOS_BACKEND "RTX")
    endif()
endforeach()

# Fallback: user macro MBED_RTOS_BACKEND
if(MBED_RTOS_BACKEND STREQUAL "RTX")
    foreach(def ${MBED_CONFIG_DEFINITIONS})
        if(def STREQUAL "MBED_RTOS_BACKEND=RTX")
            set(MBED_RTOS_BACKEND "RTX")
        endif()
    endforeach()
endif()

if(MBED_RTOS_BACKEND STREQUAL "RTX")
    # CMSIS-RTOS2 RTX Backend
    include(${CMAKE_CURRENT_LIST_DIR}/cmsis_rtos_rtx.cmake)
else()
    message(FATAL_ERROR "This RTOS backend ${MBED_RTOS_BACKEND} is not supported!")
endif()


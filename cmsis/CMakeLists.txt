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
# CMSIS-Core 6.1.1: Core headers for Cortex-M/A processors
# ============================================================================
include(${CMAKE_CURRENT_LIST_DIR}/CMSIS_cmake/cmsis_core.cmake)

# ============================================================================
# Mbed-specific boot code, RTOS configuration
# ============================================================================
add_subdirectory(device)

# ============================================================================
# Testing Support
# ============================================================================
if(MBED_ENABLE_OS_INTERNAL_TESTS)
    add_subdirectory(tests/UNITTESTS/doubles)
endif()
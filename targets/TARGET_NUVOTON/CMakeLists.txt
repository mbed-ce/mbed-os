# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nuvoton INTERFACE)

add_subdirectory(TARGET_M251 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_M261 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_M451 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_M460 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_M480 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NANO100 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUC472 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_M2354 EXCLUDE_FROM_ALL)

target_include_directories(mbed-nuvoton
    INTERFACE
        .
)

target_sources(mbed-nuvoton
    INTERFACE
        nu_miscutil.c
        nu_modutil.c
        nu_timer.c
        USBEndpoints_Nuvoton.cpp
)

target_link_libraries(mbed-nuvoton INTERFACE mbed-cmsis-cortex-m)

# PyOCD debug restart is failed without this somehow. It appears the device
# under debug must be halted after MBED_UPLOAD_RESTART_COMMANDS, but this is
# broken. Change to "monitor reset halt" to fix it.
function(mbed_upload_nuvoton_adjust_restart_command_for_pyocd target)
    if(DEFINED MBED_UPLOAD_RESTART_COMMANDS AND
       "${UPLOAD_METHOD}" STREQUAL "PYOCD")
        set(MBED_UPLOAD_RESTART_COMMANDS "monitor reset halt" CACHE INTERNAL "" FORCE)
    endif()
endfunction()

function(mbed_adjust_upload_debug_commands target)
    mbed_upload_nuvoton_adjust_restart_command_for_pyocd(${target})
endfunction()

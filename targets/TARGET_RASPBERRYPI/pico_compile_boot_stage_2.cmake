# Copyright (c) 2026 Jamie Smith All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Note: Based on boot_stage2/CMakeLists.txt in RPi Pico SDK

# Create rules to compile the boot stage 2 assembly and then include it as a binary in the build
function(pico_compile_boot_stage2 MCU_TARGET MCU_SOURCES_DIR)
    # Create executable target
    set(BS2_TARGET_NAME ${MCU_TARGET}-boot-stage-2)
    add_executable(${BS2_TARGET_NAME}
        EXCLUDE_FROM_ALL
        ${MCU_SOURCES_DIR}/boot_stage2/compile_time_choice.S
    )

    if(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
        target_link_options(${BS2_TARGET_NAME} PRIVATE "-nostdlib")
    else()
        target_link_options(${BS2_TARGET_NAME} PRIVATE "-nostartfiles")
    endif ()

    # Set up include paths
    set(PICO_SDK_SRC_BASE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/pico-sdk/src/)
    target_include_directories(${BS2_TARGET_NAME} PRIVATE
        ${MCU_SOURCES_DIR}/boot_stage2/asminclude
        ${MCU_SOURCES_DIR}/boot_stage2/include
        ${MCU_SOURCES_DIR}/hardware_regs/include
        ${MCU_SOURCES_DIR}/pico_platform/include
        ${PICO_SDK_SRC_BASE}/boards/include
        ${PICO_SDK_SRC_BASE}/common/pico_base_headers/include
        ${PICO_SDK_SRC_BASE}/rp2_common/pico_platform_common/include
        ${PICO_SDK_SRC_BASE}/rp2_common/pico_platform_compiler/include
        ${PICO_SDK_SRC_BASE}/rp2_common/pico_platform_panic/include
        ${PICO_SDK_SRC_BASE}/rp2_common/pico_platform_sections/include
        ${PICO_GENERATED_HEADERS_DIR})

    # Set up linker script
    target_link_options(${BS2_TARGET_NAME}
        PRIVATE
            "-T" "${MCU_SOURCES_DIR}/boot_stage2/boot_stage2.ld"
    )
    set_property(TARGET ${BS2_TARGET_NAME} PROPERTY LINK_DEPENDS ${MCU_SOURCES_DIR}/boot_stage2/boot_stage2.ld)

    # Overwrite MBED_OUTPUT_EXT temporarily to make sure we get a bin file
    set(MBED_OUTPUT_EXT "bin")
    mbed_generate_bin_hex(${BS2_TARGET_NAME})

    # Also print memory map
    mbed_configure_memory_map(${BS2_TARGET_NAME} "${CMAKE_CURRENT_BINARY_DIR}/${BS2_TARGET_NAME}${CMAKE_EXECUTABLE_SUFFIX}.map")
    mbed_show_memap_after_build(${BS2_TARGET_NAME})

    # Set up a post-build command that builds a padded, checksummed assembler file using the boot stage 2 executable
    set(ORIGINAL_BIN ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_BASE_NAME:${BS2_TARGET_NAME}>.bin)
    set(PADDED_CHECKSUMMED_ASM ${CMAKE_CURRENT_BINARY_DIR}/${BS2_TARGET_NAME}_padded_checksummed.S)
    add_custom_command(
        TARGET ${BS2_TARGET_NAME} POST_BUILD
        BYPRODUCTS ${PADDED_CHECKSUMMED_ASM}
        COMMAND
            ${Python3_EXECUTABLE}
            ${MCU_SOURCES_DIR}/boot_stage2/pad_checksum
            -s 0xffffffff
            ${ORIGINAL_BIN}
            ${PADDED_CHECKSUMMED_ASM}
        VERBATIM)

    # Include the .S as a source file
    target_sources(${MCU_TARGET} INTERFACE ${PADDED_CHECKSUMMED_ASM})
    add_dependencies(${MCU_TARGET} ${BS2_TARGET_NAME})
endfunction()
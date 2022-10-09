# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# File containing various functions for operating on library and executable targets.

#
# Converts output file of `target` to binary file and to Intel HEX file.
#
function(mbed_generate_bin_hex target)
    get_property(elf_to_bin GLOBAL PROPERTY ELF2BIN)

    set(artifact_name $<TARGET_FILE_BASE_NAME:${target}>)

    if (MBED_TOOLCHAIN STREQUAL "GCC_ARM")
        # The first condition is quoted in case MBED_OUTPUT_EXT is unset
        if ("${MBED_OUTPUT_EXT}" STREQUAL "" OR MBED_OUTPUT_EXT STREQUAL "bin")
            list(APPEND CMAKE_POST_BUILD_COMMAND
                COMMAND ${elf_to_bin} -O binary $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.bin
                COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.bin"
            )
        endif()
        if ("${MBED_OUTPUT_EXT}" STREQUAL "" OR MBED_OUTPUT_EXT STREQUAL "hex")
            list(APPEND CMAKE_POST_BUILD_COMMAND
                COMMAND ${elf_to_bin} -O ihex $<TARGET_FILE:${target}> ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.hex
                COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.hex"
            )
        endif()
    elseif(MBED_TOOLCHAIN STREQUAL "ARM")
        get_property(mbed_studio_arm_compiler GLOBAL PROPERTY MBED_STUDIO_ARM_COMPILER)
        if ("${MBED_OUTPUT_EXT}" STREQUAL "" OR MBED_OUTPUT_EXT STREQUAL "bin")
            list(APPEND CMAKE_POST_BUILD_COMMAND
                COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --bin  -o ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.bin $<TARGET_FILE:${target}>
                COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.bin"
            )
        endif()
        if ("${MBED_OUTPUT_EXT}" STREQUAL "" OR MBED_OUTPUT_EXT STREQUAL "hex")
            list(APPEND CMAKE_POST_BUILD_COMMAND
            COMMAND ${elf_to_bin} ${mbed_studio_arm_compiler} --i32combined  -o ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.hex $<TARGET_FILE:${target}>
            COMMAND ${CMAKE_COMMAND} -E echo "-- built: ${CMAKE_CURRENT_BINARY_DIR}/${artifact_name}.hex"
            )
        endif()
    endif()
    add_custom_command(
        TARGET
            ${target}
        POST_BUILD
        COMMAND
            ${CMAKE_POST_BUILD_COMMAND}
        COMMENT
            "executable:"
        VERBATIM
    )
endfunction()

#
# Parse toolchain generated map file of `target` and display a readable table format.
#
function(mbed_generate_map_file target)
     add_custom_command(
         TARGET
             ${target}
         POST_BUILD
         COMMAND ${Python3_EXECUTABLE} ${mbed-os_SOURCE_DIR}/tools/memap.py -t ${MBED_TOOLCHAIN} ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map
         WORKING_DIRECTORY
             ${CMAKE_CURRENT_BINARY_DIR}
)
endfunction()

#
# Create a library that will be used to store part of the Mbed code.
# An object library is created, plus a special workaround to help link dependencies work correctly.
# (static libraries cannot be used since they don't resolve weak symbols correctly)
#
# To create a library that is not compiled unless it's used, pass EXCLUDE_FROM_ALL as the second argument.
#
# Note: If you want to create an INTERFACE or ALIAS library, use the regular add_library() instead.
#
function(mbed_add_library NAME) # ARGN...
    add_library(${NAME} OBJECT ${ARGN})

    set_property(TARGET ${NAME} PROPERTY MBED_TARGET_OWN_NAME ${NAME})

    # Workaround to ensure that everything that links to this library receives its objects, including other object libraries
    # from here: https://gitlab.kitware.com/cmake/cmake/-/issues/18090#note_1041608
    #target_sources(${NAME} INTERFACE $<TARGET_OBJECTS:${NAME}>)
    target_link_libraries(${NAME} INTERFACE $<$<NOT:$<OR:$<IN_LIST:${NAME},$<TARGET_PROPERTY:LINK_LIBRARIES>>,$<STREQUAL:${NAME},$<TARGET_PROPERTY:MBED_TARGET_OWN_NAME>>>>:$<TARGET_OBJECTS:${NAME}>>)

    # Broken apart, this looks like:
    # $<
    #   $<NOT:
    #         $<OR:
    #              $<IN_LIST:${NAME},$<TARGET_PROPERTY:LINK_LIBRARIES>>,
    #              $<STREQUAL:${NAME},$<TARGET_PROPERTY:MBED_TARGET_OWN_NAME>>
    #          >
    #   >:
    #   $<TARGET_OBJECTS:${NAME}>
    #  >


endfunction()

#
# Validate selected application profile.
#
function(mbed_validate_application_profile target)
    get_target_property(app_link_libraries ${target} LINK_LIBRARIES)
    string(FIND "${app_link_libraries}" "mbed-baremetal" string_found_position)
    if(${string_found_position} GREATER_EQUAL 0)
        if(NOT "bare-metal" IN_LIST MBED_TARGET_SUPPORTED_APPLICATION_PROFILES)
            message(FATAL_ERROR
                "Use full profile as baremetal profile is not supported for this Mbed target")
        endif()
    elseif(NOT "full" IN_LIST MBED_TARGET_SUPPORTED_APPLICATION_PROFILES)
        message(FATAL_ERROR
            "The full profile is not supported for this Mbed target")
    endif()
endfunction()

#
# Set post build operations
#
function(mbed_set_post_build target)
    # The mapfile name includes the top-level target name and the
    # executable suffix for all toolchains as CMake hardcodes the name of the
    # diagnostic output file for some toolchains.

    # copy mapfile .map to .map.old for ram/rom statistics diff in memap.py
    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map)
        add_custom_command(
            TARGET
                ${target}
            PRE_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E rename "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map" "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map.old"
        )
    endif()
    
    mbed_configure_memory_map(${target} "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}.map")
    mbed_validate_application_profile(${target})
    mbed_generate_bin_hex(${target})

    if(COMMAND mbed_post_build_function)
        mbed_post_build_function(${target})
    endif()

    if(HAVE_MEMAP_DEPS)
        mbed_generate_map_file(${target})
    endif()

    mbed_generate_upload_debug_targets(${target})
    mbed_generate_ide_debug_configuration(${target})
endfunction()

#
# Call this at the very end of the build script.  Does some final cleanup tasks such as
# writing out debug configurations.
#
function(mbed_finalize_build)
    mbed_finalize_ide_debug_configurations()
endfunction(mbed_finalize_build)
# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# The 'mbed-wifi' target is lazily created because there might not be any subdirectories needing it at all.
macro(create_mbed_wifi_target)
	if(NOT TARGET mbed-wifi)
		add_library(mbed-wifi STATIC EXCLUDE_FROM_ALL)

		target_link_libraries(mbed-wifi
		    PUBLIC
		        mbed-rtos-flags
		        mbed-netsocket-api
		)
	endif()
endmacro()

# Collect sources which need to extract from static library to override
# weak symbols. Their paths are required to relative to this directory
# ,so that they match with entries in static library and their corresponding
# object files there can be found and extracted.
set(MBED_WIFI_OVERRIDE_SOURCES "" CACHE INTERNAL "" FORCE)

# Add override sources by subdirectories
#
# The input sources are required to relative to their subdirectories
# and will get converted to relative to this directory herein.
function(mbed_wifi_add_override_sources)
    set(override_sources ${MBED_WIFI_OVERRIDE_SOURCES})

    foreach(override_source ${ARGN})
        # Convert to relative to this directory
        file(RELATIVE_PATH
            override_source_fix
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/${override_source}
        )

        set(override_sources
            ${override_sources}
            ${override_source_fix}
        )
    endforeach()

    set(MBED_WIFI_OVERRIDE_SOURCES ${override_sources} CACHE INTERNAL "" FORCE)
endfunction()

# The WICED subdirectory is for wifi drivers developed using Infineon WICED framework.
# https://community.infineon.com/t5/Knowledge-Base-Articles/WICED-Wi-Fi-FAQ/ta-p/247356
if("WICED" IN_LIST MBED_TARGET_LABELS)
	create_mbed_wifi_target()
    add_subdirectory(TARGET_WICED EXCLUDE_FROM_ALL)
endif()

if("STM" IN_LIST MBED_TARGET_LABELS)
    if("EMW3080B" IN_LIST MBED_TARGET_LABELS)
        create_mbed_wifi_target()
    endif()
    add_subdirectory(TARGET_STM EXCLUDE_FROM_ALL)
endif()

if("WHD" IN_LIST MBED_TARGET_LABELS)
	create_mbed_wifi_target()
    add_subdirectory(COMPONENT_WHD EXCLUDE_FROM_ALL)
endif()

if("COMPONENT_ESPRESSIF_ESP8266=1" IN_LIST MBED_TARGET_DEFINITIONS)
	create_mbed_wifi_target()
    add_subdirectory(COMPONENT_ESPRESSIF_ESP8266)
endif()

# Extract override objects from static library and pass them to
# link options to actively participate in linking and override weak
# symbols.
if(TARGET mbed-wifi AND NOT "${MBED_WIFI_OVERRIDE_SOURCES}" STREQUAL "")
    set(override_sources ${MBED_WIFI_OVERRIDE_SOURCES})
    list(TRANSFORM override_sources APPEND ${CMAKE_CXX_OUTPUT_EXTENSION} OUTPUT_VARIABLE override_objects)
    list(TRANSFORM override_objects PREPEND ${CMAKE_CURRENT_BINARY_DIR}/ OUTPUT_VARIABLE override_objects_fullpath)

    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        # Override objects passed via target_link_options are finally
        # de-duplicated, so we can avoid multiple definition issue caused
        # due to cyclic target link via target_link_libraries.
        #
        # https://cmake.org/cmake/help/latest/command/target_link_options.html#option-de-duplication
        target_link_options(mbed-wifi
            INTERFACE
                ${override_objects_fullpath}
        )

        # Commands for extracting override objects
        foreach(override_object ${override_objects})
            get_filename_component(override_object_dirname ${override_object} DIRECTORY)

            # Extract object files from static library
            #
            # NOTE: The support has restrictions on input static library:
            # 1. No full path match modifier ('P') is specified on creating
            #    the static library.
            # 2. Object file names must be different for just one-level entry
            #    names in the static library.
            # https://sourceware.org/binutils/docs/binutils/ar-cmdline.html
            set(extract_commands
                ${extract_commands}
                COMMAND
                    ${CMAKE_COMMAND} -E make_directory ${override_object_dirname}
                COMMAND
                    ${CMAKE_AR} x --output ${override_object_dirname} $<TARGET_FILE_NAME:mbed-wifi> ${override_object}
            )
        endforeach()

        add_custom_command(
            TARGET
                mbed-wifi
            POST_BUILD
            ${extract_commands}
            WORKING_DIRECTORY
                ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM
        )
    endif()
endif()

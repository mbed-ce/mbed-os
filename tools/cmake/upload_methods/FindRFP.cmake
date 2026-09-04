# Copyright (c) 2026 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for the Renesas Flash Programmer command line tool (rfp-cli)
#
# This module defines:
# rfp_cli_PATH - full path to the rfp-cli executable
# RFP_FOUND - whether or not rfp-cli was found
# RFP_VERSION - if found, this is set to the RFP version

# try to figure out where the Renesas Flash Programmer may be installed.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")

    # On Windows, RFP installs into a versioned subfolder of
    # "C:\Program Files (x86)\Renesas Electronics\Programming Tools"
    file(GLOB RFP_HINTS "C:/Program Files*/Renesas Electronics/Programming Tools/Renesas Flash Programmer V*")

    # if we found multiple paths, check the one with the highest version number first
    list(SORT RFP_HINTS)
    list(REVERSE RFP_HINTS)

else()
    # On Linux and macOS, RFP is extracted wherever the user put it, so just search the PATH.
    # If it is not on your PATH, set rfp_cli_PATH to the full path of the rfp-cli executable.
    set(RFP_HINTS "")
endif()

find_program(rfp_cli_PATH
	NAMES rfp-cli
	DOC "Path to the rfp-cli executable"
	HINTS ${RFP_HINTS})

if(EXISTS "${rfp_cli_PATH}")
	# Detect version.  The output looks like this:
	# Renesas Flash Programmer CLI V1.14
	# Module Version: V3.21.00.000
	execute_process(COMMAND ${rfp_cli_PATH} -version
			OUTPUT_VARIABLE RFP_VERSION_OUTPUT
			ERROR_VARIABLE RFP_VERSION_OUTPUT_ERR
			OUTPUT_STRIP_TRAILING_WHITESPACE)

	set(RFP_VERSION_OUTPUT "${RFP_VERSION_OUTPUT} ${RFP_VERSION_OUTPUT_ERR}")
	if(RFP_VERSION_OUTPUT MATCHES "Module Version: V([0-9]+\\.[0-9]+)")
		set(RFP_VERSION ${CMAKE_MATCH_1})
	endif()
endif()

find_package_handle_standard_args(RFP VERSION_VAR RFP_VERSION REQUIRED_VARS rfp_cli_PATH)

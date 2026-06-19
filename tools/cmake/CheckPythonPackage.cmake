# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# CMake functions for checking for Python packages
# Requires PYTHON_EXECUTABLE to be defined.  Call FindPython first!

#
# Checks a python package in the current active Python interpreter.
#
# Usage: check_python_package(<package name> <result variable> [VERSION x.y.z])
#
# <result variable> will be set to TRUE iff the package is found in the current active python
#     interpreter and is at least the specified version.
#
# The following variables are also created:
#  - PY_INTERP_FOR_<result variable>: Set to the path to the python interpreter used to run this check.
#  - <result variable>_VERSION: Set to the version of the package from the most recent run of this check.
#
function(check_python_package PACKAGENAME OUTPUT_VAR)
    cmake_parse_arguments(ARG "" "VERSION" "" ${ARGN})

    # can't have Python packages without Python!
    if(NOT Python3_FOUND)
        set(${OUTPUT_VAR} FALSE PARENT_SCOPE)
        return()
    endif()

    set(NEED_TO_RUN_CHECK TRUE)

    if(DEFINED ${OUTPUT_VAR})
        if(${OUTPUT_VAR})
            # if the python interpreter changed, we need to recheck
            if("${PY_INTERP_FOR_${OUTPUT_VAR}}" STREQUAL "${Python3_EXECUTABLE}")
                if((NOT DEFINED ARG_VERSION) OR ("${${OUTPUT_VAR}_VERSION}" VERSION_GREATER_EQUAL "${ARG_VERSION}"))
                    set(NEED_TO_RUN_CHECK FALSE)
                endif()
            endif()
        endif()
    endif()

    if(NEED_TO_RUN_CHECK)
        set(PY_INTERP_FOR_${OUTPUT_VAR} ${Python3_EXECUTABLE} CACHE INTERNAL "The python interpreter used to run the ${OUTPUT_VAR} check" FORCE)

        execute_process(
            COMMAND ${Python3_EXECUTABLE} -c "import ${PACKAGENAME}"
            RESULT_VARIABLE PACKAGECHECK_RESULT
            ERROR_QUIET
        )

        if(${PACKAGECHECK_RESULT} EQUAL 0)
            set(HAVE_PACKAGE TRUE)
        else()
            set(HAVE_PACKAGE FALSE)
        endif()

        # Also check the version. The way to do this is different in new and old python versions.
        if(HAVE_PACKAGE)
            if(${Python3_VERSION} VERSION_GREATER_EQUAL 3.8)
                execute_process(
                    COMMAND ${Python3_EXECUTABLE} -c "from importlib.metadata import version; print(version(\"${PACKAGENAME}\"))"
                    OUTPUT_VARIABLE PACKAGECHECK_VERSION
                    COMMAND_ERROR_IS_FATAL ANY
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                )
            else()
                execute_process(
                    COMMAND ${Python3_EXECUTABLE} -c "import pkg_resources; print(pkg_resources.get_distribution(\"${PACKAGENAME}\").version)"
                    OUTPUT_VARIABLE PACKAGECHECK_VERSION
                    COMMAND_ERROR_IS_FATAL ANY
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                )
            endif()
            set(${OUTPUT_VAR}_VERSION ${PACKAGECHECK_VERSION} CACHE INTERNAL "Detected version of the Python package ${PACKAGENAME}" FORCE)
        endif()

        if(HAVE_PACKAGE)
            if((NOT DEFINED ARG_VERSION) OR ("${PACKAGECHECK_VERSION}" VERSION_GREATER_EQUAL "${ARG_VERSION}"))
                message(STATUS "Checking for Python package ${PACKAGENAME} -- found (version ${PACKAGECHECK_VERSION})")
            else()
                message(STATUS "Checking for Python package ${PACKAGENAME} -- found but too old (version ${PACKAGECHECK_VERSION}, need >=${ARG_VERSION})")
                set(HAVE_PACKAGE FALSE)
            endif()
        else()
            message(STATUS "Checking for Python package ${PACKAGENAME} -- not found")
        endif()

        set(${OUTPUT_VAR} ${HAVE_PACKAGE} CACHE BOOL "Whether the Python package ${PACKAGENAME} was found" FORCE)
        mark_as_advanced(${OUTPUT_VAR})
    endif()
endfunction(check_python_package)

# check that PACKAGENAME can be imported, and print an error if not
function(verify_python_package PACKAGENAME)

    # we can just generate our own variable name
    string(TOUPPER "HAVE_${PACKAGENAME}" HAVE_VAR_NAME)

    check_python_package(${PACKAGENAME} ${HAVE_VAR_NAME})

    if(NOT ${HAVE_VAR_NAME})
        message(FATAL_ERROR "The required Python package ${PACKAGENAME} was not found in ${Python3_EXECUTABLE}.  Please install it.")
    endif()

endfunction(verify_python_package)

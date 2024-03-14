# Copyright (c) 2024 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# CMake script to find the Python interpreter and either install or find
# Mbed's dependencies.

find_package(Python3 REQUIRED COMPONENTS Interpreter)
include(CheckPythonPackage)

option(MBED_CREATE_PYTHON_VENV "If true, Mbed OS will create its own virtual environment (venv) and install its Python packages there.  This removes the need to manually install Python packages." TRUE)

if(MBED_CREATE_PYTHON_VENV)
    # Create venv.
    # Note: venv is stored in the source directory as it can be shared between all the build directories
    # (not target specific)
    set(MBED_VENV_LOCATION ${CMAKE_SOURCE_DIR}/mbed-os/venv)
    set(VENV_STAMP_FILE ${MBED_VENV_LOCATION}/mbed-venv.stamp)
    set(MBED_REQUIREMENTS_TXT_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../requirements.txt")

    set(NEED_TO_CREATE_VENV FALSE)
    set(NEED_TO_INSTALL_PACKAGES FALSE)
    if(NOT EXISTS "${VENV_STAMP_FILE}")
        set(NEED_TO_CREATE_VENV TRUE)
        set(NEED_TO_INSTALL_PACKAGES TRUE)
    elseif("${MBED_REQUIREMENTS_TXT_LOCATION}" IS_NEWER_THAN "${VENV_STAMP_FILE}")
        set(NEED_TO_INSTALL_PACKAGES TRUE)
    endif()

    if(NEED_TO_CREATE_VENV)
        check_python_package(venv HAVE_PYTHON_VENV)
        if(NOT HAVE_PYTHON_VENV)
            message(FATAL_ERROR "Need the venv python package installed in interpreter at ${Python3_EXECUTABLE} to create a venv.
To disable venv creation, set MBED_CREATE_PYTHON_VENV to false.")
        endif()

        # Using approach from here: https://discourse.cmake.org/t/possible-to-create-a-python-virtual-env-from-cmake-and-then-find-it-with-findpython3/1132/2
        message(STATUS "Mbed: Creating virtual environment with Python interpreter ${Python3_EXECUTABLE}")
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -m venv ${MBED_VENV_LOCATION}
            COMMAND_ERROR_IS_FATAL ANY
        )

        ## update the environment with VIRTUAL_ENV variable (mimic the activate script)
        set (ENV{VIRTUAL_ENV} ${MBED_VENV_LOCATION})
        ## change the context of the search
        set (Python3_FIND_VIRTUALENV ONLY)
        ## Reset FindPython3 cache variables so it will run again
        unset(Python3_EXECUTABLE)
        unset(_Python3_EXECUTABLE CACHE)
        unset(_Python3_INTERPRETER_PROPERTIES CACHE)
        unset(_Python3_INTERPRETER_SIGNATURE CACHE)
        ## Launch a new search for Python3
        find_package (Python3 REQUIRED COMPONENTS Interpreter)
    endif()

    if(NEED_TO_INSTALL_PACKAGES)
        message(STATUS "Mbed: Installing Python requirements for Mbed into venv")
        # Upgrade pip first in case it needs an upgrade, to prevent a warning being printed
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -m pip install --upgrade pip
            COMMAND_ERROR_IS_FATAL ANY
        )
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -m pip install -r ${MBED_REQUIREMENTS_TXT_LOCATION}
            COMMAND_ERROR_IS_FATAL ANY
        )

        message("Mbed: venv created successfully")
        file(TOUCH ${VENV_STAMP_FILE})
    endif()

    # We always have the memap deps with the venv
    set(HAVE_MEMAP_DEPS TRUE)

else()

    # Check python packages
    set(PYTHON_PACKAGES_TO_CHECK intelhex prettytable future jinja2)
    foreach(PACKAGE_NAME ${PYTHON_PACKAGES_TO_CHECK})
        string(TOUPPER ${PACKAGE_NAME} PACKAGE_NAME_UCASE) # Ucase name needed for CMake variable
        string(TOLOWER ${PACKAGE_NAME} PACKAGE_NAME_LCASE) # Lcase name needed for import statement

        check_python_package(${PACKAGE_NAME_LCASE} HAVE_PYTHON_${PACKAGE_NAME_UCASE})
        if(NOT HAVE_PYTHON_${PACKAGE_NAME_UCASE})
            message(WARNING "Missing Python dependency ${PACKAGE_NAME}")
        endif()
    endforeach()

    # Check deps for memap
    if(Python3_FOUND AND HAVE_PYTHON_INTELHEX AND HAVE_PYTHON_PRETTYTABLE)
        set(HAVE_MEMAP_DEPS TRUE)
    else()
        set(HAVE_MEMAP_DEPS FALSE)
        message(STATUS "Missing Python dependencies (at least one of: python3, intelhex, prettytable) so the memory map cannot be printed")
    endif()

endif()

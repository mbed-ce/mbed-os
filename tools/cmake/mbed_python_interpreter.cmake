# Copyright (c) 2024 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# CMake script to find the Python interpreter and either install or find
# Mbed's dependencies.

option(MBED_CREATE_PYTHON_VENV "If true, Mbed OS will create its own virtual environment (venv) and install its Python packages there.  This removes the need to manually install Python packages." TRUE)

get_filename_component(MBED_CE_TOOLS_BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

if(MBED_CREATE_PYTHON_VENV)
    # Use the venv.

    # Note: venv is stored in the source directory as it can be shared between all the build directories
    # (not target specific)
    set(MBED_VENV_LOCATION ${MBED_SOURCE_DIR}/venv)
    set(VENV_STAMP_FILE ${MBED_VENV_LOCATION}/mbed-venv.stamp)
    set(MBED_PYPROJECT_TOML_LOCATION "${MBED_CE_TOOLS_BASE_DIR}/pyproject.toml")

    # Make it so modifying pyproject.toml will trigger a reconfigure
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${MBED_PYPROJECT_TOML_LOCATION})

    # Find Python3 (this will get the one in the venv if we already found it)
    set(ENV{VIRTUAL_ENV} ${MBED_VENV_LOCATION})
    set(Python3_FIND_VIRTUALENV FIRST)
    find_package(Python3 COMPONENTS Interpreter)
    include(CheckPythonPackage)

    set(NEED_TO_CREATE_VENV FALSE)
    set(NEED_TO_INSTALL_PACKAGES FALSE)

    # Special situation: if we have a cached interpreter location in the venv dir, but Python could be found,
    # this means that the venv was deleted or symlinks to a missing python install location.
    # So, use the system python and recreate it.
    if("${Python3_EXECUTABLE}" MATCHES "${MBED_VENV_LOCATION}" AND NOT Python3_FOUND)
        message(STATUS "Python venv deleted or unusable. Recreating using system Python...")

        # Launch a new search for Python3
        unset(Python3_EXECUTABLE)
        unset(_Python3_EXECUTABLE CACHE)
        unset(_Python3_INTERPRETER_PROPERTIES CACHE)
        unset(_Python3_INTERPRETER_SIGNATURE CACHE)
        set (Python3_FIND_VIRTUALENV STANDARD)
        find_package(Python3 REQUIRED COMPONENTS Interpreter)

        set(NEED_TO_CREATE_VENV TRUE)
        set(NEED_TO_INSTALL_PACKAGES TRUE)
    elseif(NOT EXISTS "${VENV_STAMP_FILE}")
        set(NEED_TO_CREATE_VENV TRUE)
        set(NEED_TO_INSTALL_PACKAGES TRUE)
    elseif(NOT ("${Python3_EXECUTABLE}" MATCHES "${MBED_VENV_LOCATION}"))
        # Alternately if we think we have the venv but FindPython didn't use it, that likely means it's
        # missing or corrupted and we need to recreate it
        set(NEED_TO_CREATE_VENV TRUE)
        set(NEED_TO_INSTALL_PACKAGES TRUE)
    elseif("${MBED_PYPROJECT_TOML_LOCATION}" IS_NEWER_THAN "${VENV_STAMP_FILE}")
        set(NEED_TO_INSTALL_PACKAGES TRUE)
    endif()

    if(NEED_TO_CREATE_VENV)
        # Create venv.
        # Using approach from here: https://discourse.cmake.org/t/possible-to-create-a-python-virtual-env-from-cmake-and-then-find-it-with-findpython3/1132/2
        message(STATUS "Mbed: Creating virtual environment with Python interpreter ${Python3_EXECUTABLE}")
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -m venv ${MBED_VENV_LOCATION}
            COMMAND_ERROR_IS_FATAL ANY
        )
        
        ## Reset FindPython3 cache variables so it will run again
        unset(Python3_EXECUTABLE)
        unset(_Python3_EXECUTABLE CACHE)
        unset(_Python3_INTERPRETER_PROPERTIES CACHE)
        unset(_Python3_INTERPRETER_SIGNATURE CACHE)

        ## Launch a new search for Python3 in the venv
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
            COMMAND ${Python3_EXECUTABLE} -m pip install ${MBED_CE_TOOLS_BASE_DIR}
            COMMAND_ERROR_IS_FATAL ANY
        )

        message(STATUS "Mbed: venv created successfully")
        file(TOUCH ${VENV_STAMP_FILE})
    endif()

    # We always have the memap deps with the venv
    set(HAVE_MEMAP_DEPS TRUE)

else()

    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    include(CheckPythonPackage)

    # The cmsis_mcu_descr module was written from scratch by Mbed CE.
    # So, this check will ensure that the user has installed the Mbed CE version of mbed_tools
    # and not the PyPI version (which we cannot do anything with because it's owned by ARM)
    check_python_package(mbed_tools.cli.cmsis_mcu_descr HAVE_MBED_CE_TOOLS)

    if(NOT HAVE_MBED_CE_TOOLS)
        message(FATAL_ERROR "Did not detect the Mbed CE Python tools installed into the python interpreter ${Python3_EXECUTABLE}. Install them with a command like: ${Python3_EXECUTABLE} -m pip install ${MBED_CE_TOOLS_BASE_DIR}")
    endif()
endif()

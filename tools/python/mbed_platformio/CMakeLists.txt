#
# This file is used as the CMakeLists.txt when building a PlatformIO project.
#

cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19...3.22)

set(MBED_APP_JSON_PATH ${PLATFORMIO_PROJECT_PATH}/mbed_app.json5)

include(${PLATFORMIO_MBED_OS_PATH}/tools/cmake/mbed_toolchain_setup.cmake)
project(PlatformIOMbedProject
        LANGUAGES C CXX ASM)
include(mbed_project_setup)

add_subdirectory(${PLATFORMIO_MBED_OS_PATH} mbed-os)

# Dummy executable. Not built, but needed to get the compile and linker flags via the CMake file API
add_executable(PIODummyExecutable
    dummy_source_file.c
    dummy_source_file.cpp
    dummy_source_file.S)
target_link_libraries(PIODummyExecutable mbed-os)
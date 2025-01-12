# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Find the cross compiler.  Use cache variables so that VS Code can detect the compiler from the cache.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    file(GLOB ARM_NONE_EABI_TOOLCHAIN_HINTS "C:/Program Files*/Arm GNU Toolchain arm-none-eabi/*/bin")
else()
    set(ARM_NONE_EABI_TOOLCHAIN_HINTS "")
endif()

find_program(CMAKE_C_COMPILER NAMES arm-none-eabi-gcc
    DOC "C Compiler"
    HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
    REQUIRED)

# Now that we have the C compiler location, also use it to find the ASM compiler and objcopy
get_filename_component(ARM_NONE_EABI_TOOLCHAIN_HINTS ${CMAKE_C_COMPILER} DIRECTORY)
find_program(CMAKE_CXX_COMPILER NAMES arm-none-eabi-g++
        DOC "CXX Compiler"
        HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
        REQUIRED)
find_program(CMAKE_ASM_COMPILER NAMES arm-none-eabi-gcc
        DOC "ASM Compiler"
        HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
        REQUIRED)
find_program(CMAKE_OBJCOPY NAMES arm-none-eabi-objcopy
        DOC "Elf to bin/hex conversion program"
        HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
        REQUIRED)
find_program(MBED_GDB
        NAMES arm-none-eabi-gdb gdb-multiarch
        HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
        DOC "Path to the GDB client program to use when debugging.")

set_property(GLOBAL PROPERTY ELF2BIN ${CMAKE_OBJCOPY})

# build toolchain flags that get passed to everything (including CMake compiler checks)
list(APPEND link_options
    "-Wl,--start-group"
        "-lstdc++"
        "-lsupc++"
        "-lm"
        "-lc"
        "-lgcc"
        "-lnosys"
    "-Wl,--end-group"
    "-specs=nosys.specs"
    "-Wl,--cref"
)

# Add linking time preprocessor macro for TFM targets
if("TFM" IN_LIST MBED_TARGET_LABELS)
    list(APPEND link_options
        "-DDOMAIN_NS=1"
    )
endif()

list(APPEND common_options
    "-Wall"
    "-Wextra"
    "-Wno-unused-parameter"
    "-Wno-missing-field-initializers"
    "-Wno-psabi" # Disable "parameter passing changed in GCC 7.1" warning
    "-fmessage-length=0"
    "-fno-exceptions"
    "-ffunction-sections"
    "-fdata-sections"
    "-funsigned-char"
    "-fomit-frame-pointer"
    "-g3"
)

list(APPEND cxx_compile_options
    "-Wno-register"
)

# Configure the toolchain to select the selected C library
function(mbed_set_c_lib target lib_type)
    if (${lib_type} STREQUAL "small")
        target_compile_definitions(${target}
            INTERFACE
                MBED_RTOS_SINGLE_THREAD
                __NEWLIB_NANO
        )

        target_link_options(${target}
            INTERFACE
                "--specs=nano.specs"
        )
    endif()

endfunction()

# Configure the toolchain to select the selected printf library
function(mbed_set_printf_lib target lib_type)
    if (${lib_type} STREQUAL "minimal-printf")
        target_compile_definitions(${target}
            INTERFACE
                MBED_MINIMAL_PRINTF
        )

        set(printf_link_options "")
        list(APPEND printf_link_options
            "-Wl,--wrap,printf"
            "-Wl,--wrap,sprintf"
            "-Wl,--wrap,snprintf"
            "-Wl,--wrap,vprintf"
            "-Wl,--wrap,vsprintf"
            "-Wl,--wrap,vsnprintf"
            "-Wl,--wrap,fprintf"
            "-Wl,--wrap,vfprintf"
        )
        target_link_options(${target}
            INTERFACE
                ${printf_link_options}
        )
    endif()
endfunction()

# Add linker flags to generate a mapfile with a given name
function(mbed_configure_memory_map target mapfile)
    target_link_options(${target}
        PRIVATE
            "-Wl,-Map=${mapfile}"
    )
endfunction()

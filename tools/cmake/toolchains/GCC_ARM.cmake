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

# Detect if the selected C compiler is actually an LLVM/clang based toolchain, such as the
# Arm Toolchain for Embedded (ATfE).  Such toolchains are command line compatible enough with
# arm-none-eabi-gcc to be driven through the GCC_ARM Mbed toolchain, but a few flags need to be
# adapted (see the MBED_ATFE conditionals below) so that clang, ld.lld and the LLVM runtime
# libraries are used correctly.  MBED_TOOLCHAIN stays GCC_ARM in this case.
set(MBED_ATFE FALSE)
execute_process(
    COMMAND ${CMAKE_C_COMPILER} --version
    RESULT_VARIABLE MBED_C_COMPILER_VERSION_RESULT
    OUTPUT_VARIABLE MBED_C_COMPILER_VERSION_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(NOT MBED_C_COMPILER_VERSION_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to run '${CMAKE_C_COMPILER} --version'")
endif()
if(MBED_C_COMPILER_VERSION_OUTPUT MATCHES "clang version")
    set(MBED_ATFE TRUE)
    message(STATUS "Mbed: LLVM/clang based toolchain (ATfE) detected, adapting GCC_ARM toolchain flags")
endif()

# Now that we have the C compiler location, also use it to find the ASM compiler and objcopy
get_filename_component(ARM_NONE_EABI_TOOLCHAIN_HINTS ${CMAKE_C_COMPILER} DIRECTORY)
find_program(CMAKE_CXX_COMPILER NAMES arm-none-eabi-g++
        DOC "CXX Compiler"
        HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
        REQUIRED)
if(MBED_ATFE)
    # Use the clang driver for assembling as well.  It understands the same option set as the
    # C/C++ compilers (including --target/--config/-mcpu), and its integrated assembler accepts
    # the GNU assembly syntax used by Mbed and CMSIS startup files.
    set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}" CACHE FILEPATH "ASM Compiler" FORCE)
    find_program(CMAKE_OBJCOPY NAMES llvm-objcopy
            DOC "Elf to bin/hex conversion program"
            HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
            REQUIRED)
    # Never link test executables during compiler detection: the ATfE sysroot only provides
    # semihosting/rdimon style startup objects which we do not want in the CMake checks.
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
else()
    find_program(CMAKE_ASM_COMPILER NAMES arm-none-eabi-gcc
            DOC "ASM Compiler"
            HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
            REQUIRED)
    find_program(CMAKE_OBJCOPY NAMES arm-none-eabi-objcopy
            DOC "Elf to bin/hex conversion program"
            HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
            REQUIRED)
endif()
find_program(MBED_GDB
        NAMES arm-none-eabi-gdb gdb-multiarch
        HINTS ${ARM_NONE_EABI_TOOLCHAIN_HINTS}
        DOC "Path to the GDB client program to use when debugging.")

set_property(GLOBAL PROPERTY ELF2BIN ${CMAKE_OBJCOPY})

# build toolchain flags that get passed to everything (including CMake compiler checks)
if(MBED_ATFE)
    # Flags required by the clang driver to generate code for the bare-metal ARM target.
    # These are passed to C, C++ and ASM compilations as well as all link invocations.
    list(APPEND common_options
        "--target=arm-none-eabi"
    )

    # Select the C library runtime through the ATfE driver --config mechanism
    # instead of the GCC .specs files.  The newlib-nano overlay is considered
    # part of the toolchain install and is used for ALL c_lib settings, so
    # that the runtime matches the newlib(-nano) based GCC_ARM toolchain.
    # MBED_ATFE_CONFIG can be set to the name of any other driver config file
    # (e.g. llvmlibc.cfg) to override this.
    set(MBED_ATFE_CONFIG "" CACHE STRING "ATfE/clang driver --config file used to select the C library runtime")
    if(MBED_ATFE_CONFIG)
        list(APPEND common_options "--config=${MBED_ATFE_CONFIG}")
    elseif(EXISTS "${ARM_NONE_EABI_TOOLCHAIN_HINTS}/newlib-nano.cfg")
        list(APPEND common_options "--config=newlib-nano.cfg")
    endif()

    # Do not pass any explicit C/C++ runtime libraries here: the clang driver appends the
    # multilib selected runtime set (libc++, libclang_rt.builtins, libc, ...) after all user
    # inputs, which gives the desired archive extraction order (Mbed's own syscall stubs and
    # __cxa_atexit/__malloc_lock implementations win over the ones from the runtime libs).
    list(APPEND link_options
        "-Wl,--cref"
        "-Wl,--print-memory-usage"
    )
else()
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
endif()

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
    "-Werror=return-type" # Turn a missing return statement into a fatal error
    "-fmessage-length=0"
    "-fno-exceptions"
    "-ffunction-sections"
    "-fdata-sections"
    "-funsigned-char"
    "-fomit-frame-pointer"
    "-g3"
)
if(NOT MBED_ATFE)
    list(APPEND common_options
        "-Wno-psabi" # Disable "parameter passing changed in GCC 7.1" warning
        "-Wno-packed-bitfield-compat" # Disable "offset of packed bitfield changed in GCC 4.4" warning
    )
else()
    # GCC silently allows macros that were defined on the command line (e.g. the
    # -D__FPU_PRESENT=1U we pass) to be redefined by device/CMSIS headers, clang
    # warns about it (-Wmacro-redefined) unless told otherwise.
    list(APPEND common_options
        "-Wno-macro-redefined"
    )
endif()

list(APPEND cxx_compile_options
    "-Wno-register"
)
if(MBED_ATFE)
    list(APPEND cxx_compile_options
        "-D_LIBCPP_HAS_THREADS=1"
        "-D_LIBCPP_HAS_THREAD_API_EXTERNAL"
        # mbed's mstd_* headers and libc++ declare some templates with a struct/class
        # mismatch, which is harmless under the Itanium ABI used on ARM
        "-Wno-mismatched-tags"
    )
endif()

# Configure the toolchain to select the selected C library
function(mbed_set_c_lib target lib_type)
    if (${lib_type} STREQUAL "small")
        target_compile_definitions(${target}
            INTERFACE
                MBED_RTOS_SINGLE_THREAD
                __NEWLIB_NANO
        )

        if(NOT MBED_ATFE)
            # With ATfE the nano variant is selected through the --config driver flag
            # (see MBED_ATFE_CONFIG above) instead of GCC's nano.specs.
            target_link_options(${target}
                INTERFACE
                    "--specs=nano.specs"
            )
        endif()
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

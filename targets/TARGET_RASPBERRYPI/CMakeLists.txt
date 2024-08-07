# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-raspberrypi INTERFACE)

# The Pico SDK relies on a couple of generated files in order to work.
# We generate those files here.

# Version header -- needs version information from CMake.
# Helpfully, that info lives in its own file, so we can just include it.
include(pico-sdk/pico_sdk_version.cmake)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pico-sdk-generated/pico)
configure_file(pico-sdk/src/common/pico_base/include/pico/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/pico-sdk-generated/pico/version.h)

# Autogen config header.
# The below settings discovered by analyzing the build system.
set(PICO_CONFIG_HEADER_FILES boards/pico.h)

# The following code copied from generate_config_header.cmake
macro(add_header_content_from_var VAR)
	set(header_content "${header_content}\n\n// based on ${VAR}:\n")
	foreach(var IN LISTS ${VAR})
		set(header_content "${header_content}\n#include \"${var}\"")
	endforeach()
endmacro()
set(header_content "// AUTOGENERATED FROM PICO_CONFIG_HEADER_FILES and then PICO_<PLATFORM>_CONFIG_HEADER_FILES\n// DO NOT EDIT!\n")
add_header_content_from_var(PICO_CONFIG_HEADER_FILES)
file(GENERATE
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pico-sdk-generated/pico/config_autogen.h
	CONTENT "${header_content}"
)

# add a link option to wrap the given function name; i.e. -Wl:wrap=FUNCNAME for gcc
function(pico_wrap_function TARGET FUNCNAME)
    target_link_options(${TARGET} INTERFACE "LINKER:--wrap=${FUNCNAME}")
endfunction()

# Following is copied from src/rp2_common/pico_float/CMakeLists.txt
function(wrap_float_functions TARGET)
	pico_wrap_function(${TARGET} __aeabi_fadd)
	pico_wrap_function(${TARGET} __aeabi_fdiv)
	pico_wrap_function(${TARGET} __aeabi_fmul)
	pico_wrap_function(${TARGET} __aeabi_frsub)
	pico_wrap_function(${TARGET} __aeabi_fsub)
	pico_wrap_function(${TARGET} __aeabi_cfcmpeq)
	pico_wrap_function(${TARGET} __aeabi_cfrcmple)
	pico_wrap_function(${TARGET} __aeabi_cfcmple)
	pico_wrap_function(${TARGET} __aeabi_fcmpeq)
	pico_wrap_function(${TARGET} __aeabi_fcmplt)
	pico_wrap_function(${TARGET} __aeabi_fcmple)
	pico_wrap_function(${TARGET} __aeabi_fcmpge)
	pico_wrap_function(${TARGET} __aeabi_fcmpgt)
	pico_wrap_function(${TARGET} __aeabi_fcmpun)
	pico_wrap_function(${TARGET} __aeabi_i2f)
	pico_wrap_function(${TARGET} __aeabi_l2f)
	pico_wrap_function(${TARGET} __aeabi_ui2f)
	pico_wrap_function(${TARGET} __aeabi_ul2f)
	pico_wrap_function(${TARGET} __aeabi_f2iz)
	pico_wrap_function(${TARGET} __aeabi_f2lz)
	pico_wrap_function(${TARGET} __aeabi_f2uiz)
	pico_wrap_function(${TARGET} __aeabi_f2ulz)
	pico_wrap_function(${TARGET} __aeabi_f2d)
	pico_wrap_function(${TARGET} sqrtf)
	pico_wrap_function(${TARGET} cosf)
	pico_wrap_function(${TARGET} sinf)
	pico_wrap_function(${TARGET} tanf)
	pico_wrap_function(${TARGET} atan2f)
	pico_wrap_function(${TARGET} expf)
	pico_wrap_function(${TARGET} logf)

	pico_wrap_function(${TARGET} ldexpf)
	pico_wrap_function(${TARGET} copysignf)
	pico_wrap_function(${TARGET} truncf)
	pico_wrap_function(${TARGET} floorf)
	pico_wrap_function(${TARGET} ceilf)
	pico_wrap_function(${TARGET} roundf)
	pico_wrap_function(${TARGET} sincosf) # gnu
	pico_wrap_function(${TARGET} asinf)
	pico_wrap_function(${TARGET} acosf)
	pico_wrap_function(${TARGET} atanf)
	pico_wrap_function(${TARGET} sinhf)
	pico_wrap_function(${TARGET} coshf)
	pico_wrap_function(${TARGET} tanhf)
	pico_wrap_function(${TARGET} asinhf)
	pico_wrap_function(${TARGET} acoshf)
	pico_wrap_function(${TARGET} atanhf)
	pico_wrap_function(${TARGET} exp2f)
	pico_wrap_function(${TARGET} log2f)
	pico_wrap_function(${TARGET} exp10f)
	pico_wrap_function(${TARGET} log10f)
	pico_wrap_function(${TARGET} powf)
	pico_wrap_function(${TARGET} powintf) #gnu
	pico_wrap_function(${TARGET} hypotf)
	pico_wrap_function(${TARGET} cbrtf)
	pico_wrap_function(${TARGET} fmodf)
	pico_wrap_function(${TARGET} dremf)
	pico_wrap_function(${TARGET} remainderf)
	pico_wrap_function(${TARGET} remquof)
	pico_wrap_function(${TARGET} expm1f)
	pico_wrap_function(${TARGET} log1pf)
	pico_wrap_function(${TARGET} fmaf)
endfunction()

# Now, add includes and headers from the Pico SDK
target_include_directories(mbed-raspberrypi
    INTERFACE
        .
		pico-sdk/src/rp2_common/hardware_adc/include
		pico-sdk/src/rp2_common/hardware_divider/include
		pico-sdk/src/rp2_common/hardware_gpio/include
		pico-sdk/src/rp2_common/hardware_resets/include
		pico-sdk/src/rp2_common/hardware_pwm/include
		pico-sdk/src/rp2_common/hardware_base/include
		pico-sdk/src/rp2_common/hardware_uart/include
		pico-sdk/src/rp2_common/hardware_spi/include
		pico-sdk/src/rp2_common/hardware_i2c/include
		pico-sdk/src/rp2_common/hardware_irq/include
		pico-sdk/src/rp2_common/hardware_flash/include
		pico-sdk/src/rp2_common/hardware_clocks/include
		pico-sdk/src/rp2_common/hardware_rtc/include
		pico-sdk/src/rp2_common/hardware_watchdog/include
		pico-sdk/src/rp2_common/hardware_timer/include
		pico-sdk/src/rp2_common/hardware_pll/include
		pico-sdk/src/rp2_common/hardware_sync/include
		pico-sdk/src/rp2_common/hardware_xosc/include
		pico-sdk/src/rp2_common/hardware_pio/include
		pico-sdk/src/rp2_common/hardware_dma/include
		pico-sdk/src/rp2_common/pico_platform/include
		pico-sdk/src/rp2_common/pico_fix/rp2040_usb_device_enumeration/include/
		pico-sdk/src/rp2_common/pico_bootrom/include
		pico-sdk/src/rp2_common/pico_float/include
		pico-sdk/src/rp2_common/hardware_claim/include
		pico-sdk/src/common/pico_sync/include
		pico-sdk/src/common/pico_time/include
		pico-sdk/src/common/pico_base/include
		pico-sdk/src/common/pico_binary_info/include
		pico-sdk/src/common/pico_util/include
		pico-sdk/src/boards/include
		pico-sdk/src/generated
		pico-sdk/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Include
		pico-sdk/src/rp2_common/cmsis/include
	    ${CMAKE_CURRENT_BINARY_DIR}/pico-sdk-generated/
)

target_sources(mbed-raspberrypi
    INTERFACE
		pico-sdk/src/rp2_common/hardware_adc/adc.c
		pico-sdk/src/rp2_common/hardware_flash/flash.c
		pico-sdk/src/rp2_common/hardware_uart/uart.c
		pico-sdk/src/rp2_common/hardware_spi/spi.c
		pico-sdk/src/rp2_common/hardware_i2c/i2c.c
		pico-sdk/src/rp2_common/hardware_gpio/gpio.c
		pico-sdk/src/rp2_common/hardware_xosc/xosc.c
		pico-sdk/src/rp2_common/hardware_irq/irq.c
		pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S
		pico-sdk/src/rp2_common/hardware_pll/pll.c
		pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c
		pico-sdk/src/rp2_common/hardware_clocks/clocks.c
		pico-sdk/src/rp2_common/hardware_claim/claim.c
		pico-sdk/src/rp2_common/hardware_timer/timer.c
		pico-sdk/src/rp2_common/hardware_sync/sync.c
		pico-sdk/src/rp2_common/hardware_rtc/rtc.c
		pico-sdk/src/rp2_common/hardware_pio/pio.c
		pico-sdk/src/rp2_common/hardware_dma/dma.c
		pico-sdk/src/rp2_common/pico_bootrom/bootrom.c
		pico-sdk/src/rp2_common/pico_platform/platform.c
		pico-sdk/src/common/pico_time/time.c
		pico-sdk/src/common/pico_sync/lock_core.c
		pico-sdk/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Source/system_RP2040.c
		pico-sdk/src/rp2_common/pico_float/float_aeabi.S
		pico-sdk/src/rp2_common/pico_float/float_init_rom.c
		pico-sdk/src/rp2_common/pico_float/float_math.c
		pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S
		pico-sdk/src/rp2_common/hardware_divider/divider.S
)

target_compile_definitions(mbed-raspberrypi
	INTERFACE
		LIB_CMSIS_CORE # Enables renaming interrupt vectors to their CMSIS names
)

target_link_libraries(mbed-raspberrypi INTERFACE mbed-cmsis-cortex-m)

# Create RP2040 target (will have more sources added in subdir)
add_library(mbed-rp2040 INTERFACE)
target_include_directories(mbed-rp2040
	INTERFACE
		pico-sdk/src/rp2040/hardware_structs/include
		pico-sdk/src/rp2040/hardware_regs/include
)
target_sources(mbed-rp2040
	INTERFACE
		pico-sdk/src/rp2_common/pico_fix/rp2040_usb_device_enumeration/rp2040_usb_device_enumeration.c
)

# Enable usage of the RPi Pico optimized floating point routines
wrap_float_functions(mbed-rp2040)

add_subdirectory(TARGET_RP2040 EXCLUDE_FROM_ALL)

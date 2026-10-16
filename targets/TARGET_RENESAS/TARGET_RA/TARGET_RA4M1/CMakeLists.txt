# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RA4M1
add_library(mbed-ra4m1 INTERFACE)

target_include_directories(mbed-ra4m1
    INTERFACE
        ../ra/fsp/src/r_sce
        ../ra/fsp/src/r_sce/common
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/private/inc
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/public/inc
        .
)

target_sources(mbed-ra4m1
    INTERFACE
        gpio_irq_device.c
        uart_device.c
        can_device.c
        bsp_linker.c
        USBPhy_RA4M1.cpp
		../ra/fsp/src/r_adc/r_adc.c
		../ra/fsp/src/r_agt/r_agt.c
		../ra/fsp/src/r_cac/r_cac.c
		../ra/fsp/src/r_can/r_can.c
		../ra/fsp/src/r_cgc/r_cgc.c
		../ra/fsp/src/r_crc/r_crc.c
		../ra/fsp/src/r_dac/r_dac.c
		../ra/fsp/src/r_dmac/r_dmac.c
		../ra/fsp/src/r_doc/r_doc.c
		../ra/fsp/src/r_dtc/r_dtc.c
		../ra/fsp/src/r_elc/r_elc.c
		../ra/fsp/src/r_flash_lp/r_flash_lp.c
		../ra/fsp/src/r_gpt/r_gpt.c
		../ra/fsp/src/r_gpt_three_phase/r_gpt_three_phase.c
		../ra/fsp/src/r_icu/r_icu.c
		../ra/fsp/src/r_iic_master/r_iic_master.c
		../ra/fsp/src/r_ioport/r_ioport.c
		../ra/fsp/src/r_iwdt/r_iwdt.c
		../ra/fsp/src/r_lpm/r_lpm.c
		../ra/fsp/src/r_lvd/r_lvd.c
		../ra/fsp/src/r_rtc/r_rtc.c
		../ra/fsp/src/r_sci_i2c/r_sci_i2c.c
		../ra/fsp/src/r_sci_lin/r_sci_lin.c
		../ra/fsp/src/r_sci_smci/r_sci_smci.c
		../ra/fsp/src/r_sci_uart/r_sci_uart.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/adaptors/r_sce_adapt.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_func001.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_func002.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_func003.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_func205.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_func206.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_func207.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p00.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p01.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p02.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p06.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p07.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p09.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p16.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p17.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p22.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p23.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p30.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p31.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p32.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p33.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p34.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p35.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p36.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p38.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p39.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p40.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p41.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p42.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p43.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p44.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p45.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p46.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p47.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p50.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p51.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p52.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p53.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p54.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p55.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p60.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p61.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p62.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p63.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p64.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p65.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p70.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p71.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p72.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p73.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p74.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p75.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p76.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p77.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p78.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p79.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p80.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p81.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p84.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p85.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p86.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p87.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p88.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p91.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_p92.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pb3f.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pb3i.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pb3u.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pb6f.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pb6i.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pb6u.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pb9f.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pb9i.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pb9u.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pc2f.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pc2i.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_pc2u.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_subprc01.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/hw_sce_subprc02.c
		../ra/fsp/src/r_sce/crypto_procedures/src/sce5/plainkey/primitive/s_flash.c
		../ra/fsp/src/r_spi/r_spi.c
		../ra/fsp/src/r_wdt/r_wdt.c
)

target_link_libraries(mbed-ra4m1 INTERFACE mbed-renesas-ra)

# NOTE: The RA4M1_DIRECT_SVC workaround (replacing SVC calls with direct
# function calls) is no longer needed. The original SVC failure was caused
# by PRIMASK=1 from the Arduino bootloader blocking all exceptions.
# With __enable_irq() in mbed_sdk_init(), the native SVC mechanism works.

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
	mbed_set_linker_script(mbed-ra4m1 RA4M1.ld)
endif()

add_subdirectory(TARGET_ARDUINO_UNO_R4_MINIMA EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_ARDUINO_UNO_R4_WIFI EXCLUDE_FROM_ALL)

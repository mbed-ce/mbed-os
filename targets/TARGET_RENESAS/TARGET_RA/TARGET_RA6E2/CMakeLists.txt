# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# RA6E2
add_library(mbed-ra6e2 INTERFACE)

target_include_directories(mbed-ra6e2
    INTERFACE
        ../ra/fsp/src/bsp/mcu/ra6e2
        .
)

target_sources(mbed-ra6e2
    INTERFACE
        gpio_irq_device.c
        uart_device.c
        can_device.c
        ../ra/fsp/src/bsp/mcu/ra6e2/bsp_linker.c
		../ra/fsp/src/r_adc/r_adc.c
		../ra/fsp/src/r_agt/r_agt.c
		../ra/fsp/src/r_cac/r_cac.c
		../ra/fsp/src/r_canfd/r_canfd.c
		../ra/fsp/src/r_cgc/r_cgc.c
		../ra/fsp/src/r_crc/r_crc.c
		../ra/fsp/src/r_dac/r_dac.c
		../ra/fsp/src/r_dmac/r_dmac.c
		../ra/fsp/src/r_doc/r_doc.c
		../ra/fsp/src/r_dtc/r_dtc.c
		../ra/fsp/src/r_elc/r_elc.c
		../ra/fsp/src/r_flash_hp/r_flash_hp.c
		../ra/fsp/src/r_gpt/r_gpt.c
		../ra/fsp/src/r_gpt_three_phase/r_gpt_three_phase.c
		../ra/fsp/src/r_icu/r_icu.c
		../ra/fsp/src/r_iic_b_master/r_iic_b_master.c
		../ra/fsp/src/r_ioport/r_ioport.c
		../ra/fsp/src/r_iwdt/r_iwdt.c
		../ra/fsp/src/r_lpm/r_lpm.c
		../ra/fsp/src/r_lvd/r_lvd.c
		../ra/fsp/src/r_poeg/r_poeg.c
		../ra/fsp/src/r_qspi/r_qspi.c
		../ra/fsp/src/r_rtc/r_rtc.c
		../ra/fsp/src/r_sci_i2c/r_sci_i2c.c
		../ra/fsp/src/r_sci_lin/r_sci_lin.c
		../ra/fsp/src/r_sci_smci/r_sci_smci.c
		../ra/fsp/src/r_sci_spi/r_sci_spi.c
		../ra/fsp/src/r_sci_uart/r_sci_uart.c
		../ra/fsp/src/r_spi/r_spi.c
		../ra/fsp/src/r_wdt/r_wdt.c
)

target_link_libraries(mbed-ra6e2 INTERFACE mbed-renesas-ra)
if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
	mbed_set_linker_script(mbed-ra6e2 ../ra/fsp/src/bsp/mcu/ra6e2/RA6E2.ld)
endif()

add_subdirectory(TARGET_FPB_RA6E2 EXCLUDE_FROM_ALL)

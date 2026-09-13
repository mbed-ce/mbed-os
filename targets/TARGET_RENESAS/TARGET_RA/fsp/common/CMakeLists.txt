# Common code
target_include_directories(mbed-renesas-ra
    INTERFACE
        inc
        inc/api
        inc/instances
        src/bsp/mcu/all
        src/bsp/cmsis/Device/RENESAS/Include
        src/r_usb_basic/src/driver/inc
        src/r_usb_basic/src/hw/inc
        src/r_rsip_protected/crypto_procedures_protected/src/rsip/common/inc/api
        src/r_rsip_protected/crypto_procedures_protected/src/rsip/ra/inc/instances
)

target_sources(mbed-renesas-ra
    INTERFACE
        src/bsp/cmsis/Device/RENESAS/Source/system.c
        src/bsp/cmsis/Device/RENESAS/Source/startup.c
        src/bsp/mcu/all/bsp_clocks.c
        src/bsp/mcu/all/bsp_common.c
        src/bsp/mcu/all/bsp_delay.c
        src/bsp/mcu/all/bsp_group_irq.c
        src/bsp/mcu/all/bsp_guard.c
        src/bsp/mcu/all/bsp_io.c
        src/bsp/mcu/all/bsp_ipc.c
        src/bsp/mcu/all/bsp_irq.c
        src/bsp/mcu/all/bsp_macl.c
        src/bsp/mcu/all/bsp_ospi_b.c
        src/bsp/mcu/all/bsp_register_protection.c
        src/bsp/mcu/all/bsp_sdram.c
        src/bsp/mcu/all/bsp_security.c
        src/r_adc/r_adc.c
        src/r_agt/r_agt.c
        src/r_cac/r_cac.c
        src/r_canfd/r_canfd.c
        src/r_cgc/r_cgc.c
        src/r_crc/r_crc.c
        src/r_dac/r_dac.c
        src/r_dmac/r_dmac.c
        src/r_doc/r_doc.c
        src/r_dtc/r_dtc.c
        src/r_elc/r_elc.c
        src/r_flash_hp/r_flash_hp.c
        src/r_gpt/r_gpt.c
        src/r_gpt_three_phase/r_gpt_three_phase.c
        src/r_icu/r_icu.c
        src/r_iic_master/r_iic_master.c
        src/r_iic_slave/r_iic_slave.c
        src/r_ioport/r_ioport.c
        src/r_iwdt/r_iwdt.c
        src/r_lpm/r_lpm.c
        src/r_lvd/r_lvd.c
        src/r_poeg/r_poeg.c
        src/r_rtc/r_rtc.c
        src/r_sci_i2c/r_sci_i2c.c
        src/r_sci_lin/r_sci_lin.c
        src/r_sci_smci/r_sci_smci.c
        src/r_sci_spi/r_sci_spi.c
        src/r_sci_b_uart/r_sci_b_uart.c
        src/r_spi_b/r_spi_b.c
        src/r_wdt/r_wdt.c
)

# RA8E1 specific code
target_include_directories(mbed-ra8e1 INTERFACE src/bsp/mcu/ra8e1)
target_sources(mbed-ra8e1 INTERFACE src/bsp/mcu/ra8e1/bsp_linker.c)
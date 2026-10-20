# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mcu-kwx1 INTERFACE)

add_library(mbed-mcu-kw41z INTERFACE)
target_link_libraries(mbed-mcu-kw41z INTERFACE mbed-mcu-kwx1)

# Link appropriate SDK drivers for KL4x
target_sources(mbed-mcu-kwx1
    INTERFACE
        mbed-mcux-sdk/drivers/common/fsl_common.c
        mbed-mcux-sdk/drivers/common/fsl_common_arm.c
        mbed-mcux-sdk/drivers/adc16/fsl_adc16.c
        mbed-mcux-sdk/drivers/i2c/fsl_i2c.c
        mbed-mcux-sdk/drivers/i2c/fsl_i2c_edma.c
        mbed-mcux-sdk/drivers/dspi/fsl_dspi.c
        mbed-mcux-sdk/drivers/dspi/fsl_dspi_edma.c
        mbed-mcux-sdk/drivers/lpuart/fsl_lpuart.c
        mbed-mcux-sdk/drivers/lpuart/fsl_lpuart_edma.c
        mbed-mcux-sdk/drivers/dmamux/fsl_dmamux.c
        mbed-mcux-sdk/drivers/edma/fsl_edma.c
        mbed-mcux-sdk/drivers/cop/fsl_cop.c
        mbed-mcux-sdk/drivers/dac/fsl_dac.c
        mbed-mcux-sdk/drivers/cmp/fsl_cmp.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_flash.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_cache.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_controller.c
        mbed-mcux-sdk/drivers/gpio/fsl_gpio.c
        mbed-mcux-sdk/drivers/llwu/fsl_llwu.c
        mbed-mcux-sdk/drivers/lptmr/fsl_lptmr.c
        mbed-mcux-sdk/drivers/pit/fsl_pit.c
        mbed-mcux-sdk/drivers/pmc/fsl_pmc.c
        mbed-mcux-sdk/drivers/rcm/fsl_rcm.c
        mbed-mcux-sdk/drivers/sim/fsl_sim.c
        mbed-mcux-sdk/drivers/smc/fsl_smc.c
        mbed-mcux-sdk/drivers/tpm/fsl_tpm.c
        mbed-mcux-sdk/drivers/trng/fsl_trng.c
        mbed-mcux-sdk/drivers/vref/fsl_vref.c
        mbed-mcux-sdk/drivers/dcdc/fsl_dcdc.c
        mbed-mcux-sdk/drivers/ltc/fsl_ltc.c
        mbed-mcux-sdk/drivers/ltc/fsl_ltc_edma.c
        mbed-mcux-sdk/drivers/rtc/fsl_rtc.c
)

target_include_directories(mbed-mcu-kwx1
    INTERFACE
        mbed-mcux-sdk/drivers/common
        mbed-mcux-sdk/drivers/adc16
        mbed-mcux-sdk/drivers/i2c
        mbed-mcux-sdk/drivers/dspi
        mbed-mcux-sdk/drivers/lpuart
        mbed-mcux-sdk/drivers/dmamux
        mbed-mcux-sdk/drivers/edma
        mbed-mcux-sdk/drivers/cop
        mbed-mcux-sdk/drivers/dac
        mbed-mcux-sdk/drivers/cmp
        mbed-mcux-sdk/drivers/flash
        mbed-mcux-sdk/drivers/gpio
        mbed-mcux-sdk/drivers/port
        mbed-mcux-sdk/drivers/llwu
        mbed-mcux-sdk/drivers/lptmr
        mbed-mcux-sdk/drivers/pit
        mbed-mcux-sdk/drivers/pmc
        mbed-mcux-sdk/drivers/rcm
        mbed-mcux-sdk/drivers/sim
        mbed-mcux-sdk/drivers/smc
        mbed-mcux-sdk/drivers/tpm
        mbed-mcux-sdk/drivers/trng
        mbed-mcux-sdk/drivers/vref
        mbed-mcux-sdk/drivers/dcdc
        mbed-mcux-sdk/drivers/ltc
        mbed-mcux-sdk/drivers/rtc
)

# Flag to MCUX SDK that we have a ram vector table
target_compile_definitions(mbed-mcu-kwx1 INTERFACE ENABLE_RAM_VECTOR_TABLE)

# Link appropriate Mbed OS HALs that interface with this MCU
target_include_directories(mbed-mcu-kwx1
    INTERFACE
        mbed-hal-drivers/kinetis-common-headers
        mbed-hal-drivers/fsl-os-abstraction
        mbed-hal-drivers/serial-via-LPUART
        mbed-hal-drivers/us-ticker-via-TPM-PIT
)
target_sources(mbed-mcu-kwx1
    INTERFACE
        mbed-hal-drivers/fsl-os-abstraction/fsl_os_abstraction_mbed.c
        mbed-hal-drivers/gpio-via-GPIO/gpio_api.c
        mbed-hal-drivers/serial-via-LPUART/serial_api.c
        mbed-hal-drivers/us-ticker-via-TPM-PIT/us_ticker.c
        mbed-hal-drivers/pwmout-via-TPM/pwmout_api.c
        mbed-hal-drivers/spi-via-DSPI/spi_api.c
        mbed-hal-drivers/trng-via-TRNG/trng_api.c
        mbed-hal-drivers/lpticker-via-LPTMR/lp_ticker.c
        mbed-hal-drivers/rtc-via-RTC/rtc_api.c
        mbed-hal-drivers/analogin-via-ADC16/analogin_api.c
        mbed-hal-drivers/analogout-via-DAC/analogout_api.c
        mbed-hal-drivers/i2c-via-I2C/i2c_api.c
        mbed-hal-drivers/interruptin-via-GPIO-PORT/gpio_irq_api.c
        mbed-hal-drivers/port-via-PORT/port_api.c
        mbed-hal-drivers/sleep-via-SMC/sleep.c
        mbed-hal-drivers/flash-via-FLASH/flash_api.c
        mbed-hal-drivers/pinmode-via-PORT/pinmap.c
        mbed-hal-drivers/dma-via-DMAMUX/dma_api.c
)

# Link appropriate headers and startup files
target_sources(mbed-mcu-kw41z INTERFACE
        mbed-mcux-sdk/devices/kinetis/MKW41Z4/system_MKW41Z4.c
        mbed-mcux-sdk/devices/kinetis/MKW41Z4/drivers/fsl_clock.c
)
target_include_directories(mbed-mcu-kw41z INTERFACE
        mbed-mcux-sdk/devices/kinetis/MKW41Z4/
        mbed-mcux-sdk/devices/kinetis/MKW41Z4/drivers
)
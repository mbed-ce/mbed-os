
# Link appropriate SDK drivers for KL4x
target_sources(mbed-mcu-kl4x INTERFACE
    mbed-mcux-sdk/drivers/common/fsl_common.c
    mbed-mcux-sdk/drivers/common/fsl_common_arm.c
    mbed-mcux-sdk/drivers/adc16/fsl_adc16.c
    mbed-mcux-sdk/drivers/i2c/fsl_i2c.c
    mbed-mcux-sdk/drivers/i2c/fsl_i2c_dma.c
    mbed-mcux-sdk/drivers/spi/fsl_spi.c
    mbed-mcux-sdk/drivers/spi/fsl_spi_dma.c
    mbed-mcux-sdk/drivers/uart/fsl_uart.c
    mbed-mcux-sdk/drivers/uart/fsl_uart_dma.c
    mbed-mcux-sdk/drivers/lpuart/fsl_lpuart.c
    mbed-mcux-sdk/drivers/lpuart/fsl_lpuart_dma.c
    mbed-mcux-sdk/drivers/flexio/fsl_flexio.c
    mbed-mcux-sdk/drivers/cmp/fsl_cmp.c
    mbed-mcux-sdk/drivers/dac/fsl_dac.c
    mbed-mcux-sdk/drivers/vref/fsl_vref.c
    mbed-mcux-sdk/drivers/pmc/fsl_pmc.c
    mbed-mcux-sdk/drivers/gpio/fsl_gpio.c
    mbed-mcux-sdk/drivers/llwu/fsl_llwu.c
    mbed-mcux-sdk/drivers/pit/fsl_pit.c
    mbed-mcux-sdk/drivers/lptmr/fsl_lptmr.c
    mbed-mcux-sdk/drivers/rtc/fsl_rtc.c
    mbed-mcux-sdk/drivers/tpm/fsl_tpm.c
    mbed-mcux-sdk/drivers/cop/fsl_cop.c
    mbed-mcux-sdk/drivers/dsc_flash/fsl_ftfx_flash.c
    mbed-mcux-sdk/drivers/dsc_flash/fsl_ftfx_controller.c
    mbed-mcux-sdk/drivers/dma/fsl_dma.c
    mbed-mcux-sdk/drivers/dmamux/fsl_dmamux.c
    mbed-mcux-sdk/drivers/smc/fsl_smc.c
)

target_include_directories(mbed-mcu-kl4x
    INTERFACE
        mbed-mcux-sdk/drivers/common
        mbed-mcux-sdk/drivers/adc16
        mbed-mcux-sdk/drivers/i2c
        mbed-mcux-sdk/drivers/spi
        mbed-mcux-sdk/drivers/uart
        mbed-mcux-sdk/drivers/lpuart/
        mbed-mcux-sdk/drivers/flexio
        mbed-mcux-sdk/drivers/cmp
        mbed-mcux-sdk/drivers/dac
        mbed-mcux-sdk/drivers/vref
        mbed-mcux-sdk/drivers/pmc
        mbed-mcux-sdk/drivers/gpio
        mbed-mcux-sdk/drivers/llwu
        mbed-mcux-sdk/drivers/pit
        mbed-mcux-sdk/drivers/lptmr
        mbed-mcux-sdk/drivers/rtc
        mbed-mcux-sdk/drivers/tpm
        mbed-mcux-sdk/drivers/cop
        mbed-mcux-sdk/drivers/dsc_flash
        mbed-mcux-sdk/drivers/dma
        mbed-mcux-sdk/drivers/dmamux
        mbed-mcux-sdk/drivers/port
        mbed-mcux-sdk/drivers/smc
)

# Link appropriate Mbed OS HAL that interface with Kinetis
target_include_directories(mbed-mcu-kl4x
    INTERFACE
        mbed-hal-drivers/kinetis-common-headers
        mbed-hal-drivers/serial-via-LPUART
        mbed-hal-drivers/us-ticker-via-TPM-PIT
)
target_sources(mbed-mcu-kl4x
    INTERFACE
        mbed-hal-drivers/lpticker-via-lptmr/lp_ticker.c
        mbed-hal-drivers/gpio-via-GPIO/gpio_api.c
        mbed-hal-drivers/interruptin-via-GPIO-PORT/gpio_irq_api.c
        mbed-hal-drivers/pinmode-via-PORT/pinmap.c
        mbed-hal-drivers/sleep-via-SMC/sleep.c
        mbed-hal-drivers/analogin-via-ADC16/analogin_api.c
        mbed-hal-drivers/serial-via-LPUART/serial_api.c
        mbed-hal-drivers/us-ticker-via-TPM-PIT/us_ticker.c
        mbed-hal-drivers/spi-via-SPI/spi_api.c
        mbed-hal-drivers/i2c-via-I2C/i2c_api.c
        mbed-hal-drivers/analogout-via-DAC/analogout_api.c
        mbed-hal-drivers/port-via-PORT/port_api.c
        mbed-hal-drivers/flash-via-FLASH/flash_api.c
)

# Link appropriate startup files for KL43
target_sources(mbed-mcu-kl43z INTERFACE
    mbed-mcux-sdk/devices_manual/MKL43Z4/system_MKL43Z4.c
    mbed-mcux-sdk/devices_manual/MKL43Z4/drivers/fsl_clock.c
)
target_include_directories(mbed-mcu-kl43z INTERFACE
    mbed-mcux-sdk/devices_manual/MKL43Z4
    mbed-mcux-sdk/devices_manual/MKL43Z4/drivers/
)

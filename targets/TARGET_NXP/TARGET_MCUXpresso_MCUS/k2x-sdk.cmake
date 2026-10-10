add_library(mbed-mcu-k2x INTERFACE)


# Link appropriate SDK drivers for K6x
target_sources(mbed-mcu-k2x INTERFACE
        mbed-mcux-sdk/drivers/common/fsl_common.c
        mbed-mcux-sdk/drivers/common/fsl_common_arm.c
        mbed-mcux-sdk/drivers/adc16/fsl_adc16.c
        mbed-mcux-sdk/drivers/aipstz/fsl_aipstz.c
        mbed-mcux-sdk/drivers/cmp/fsl_cmp.c
        mbed-mcux-sdk/drivers/crc/fsl_crc.c
        mbed-mcux-sdk/drivers/dac/fsl_dac.c
        mbed-mcux-sdk/drivers/dmamux/fsl_dmamux.c
        mbed-mcux-sdk/drivers/dspi/fsl_dspi.c
        mbed-mcux-sdk/drivers/dspi/fsl_dspi_edma.c
        mbed-mcux-sdk/drivers/edma/fsl_edma.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_cache.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_controller.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_flash.c
        mbed-mcux-sdk/drivers/flash/fsl_ftfx_flexnvm.c
        mbed-mcux-sdk/drivers/i2c/fsl_i2c.c
        mbed-mcux-sdk/drivers/i2c/fsl_i2c_edma.c
        mbed-mcux-sdk/drivers/llwu/fsl_llwu.c
        mbed-mcux-sdk/drivers/lptmr/fsl_lptmr.c
        mbed-mcux-sdk/drivers/lpuart/fsl_lpuart.c
        mbed-mcux-sdk/drivers/lpuart/fsl_lpuart_edma.c
        mbed-mcux-sdk/drivers/pit/fsl_pit.c
        mbed-mcux-sdk/drivers/ftm/fsl_ftm.c
        mbed-mcux-sdk/drivers/rcm/fsl_rcm.c
        mbed-mcux-sdk/drivers/sai/fsl_sai.c
        mbed-mcux-sdk/drivers/rnga/fsl_rnga.c
        mbed-mcux-sdk/drivers/sim/fsl_sim.c
        mbed-mcux-sdk/drivers/smc/fsl_smc.c
        mbed-mcux-sdk/drivers/rtc/fsl_rtc.c
        mbed-mcux-sdk/drivers/vref/fsl_vref.c
        mbed-mcux-sdk/drivers/wdog/fsl_wdog.c
)

target_include_directories(mbed-mcu-k2x
    INTERFACE
        mbed-mcux-sdk/drivers/common
        mbed-mcux-sdk/drivers/aipstz
        mbed-mcux-sdk/drivers/crc
        mbed-mcux-sdk/drivers/dac
        mbed-mcux-sdk/drivers/flash
        mbed-mcux-sdk/drivers/llwu
        mbed-mcux-sdk/drivers/lptmr
        mbed-mcux-sdk/drivers/pit
        mbed-mcux-sdk/drivers/mcm
        mbed-mcux-sdk/drivers/ftm
        mbed-mcux-sdk/drivers/adc16
        mbed-mcux-sdk/drivers/cmp
        mbed-mcux-sdk/drivers/edma
        mbed-mcux-sdk/drivers/dmamux
        mbed-mcux-sdk/drivers/dspi
        mbed-mcux-sdk/drivers/uart
        mbed-mcux-sdk/drivers/lpuart
        mbed-mcux-sdk/drivers/i2c
        mbed-mcux-sdk/drivers/port
        mbed-mcux-sdk/drivers/rcm
        mbed-mcux-sdk/drivers/rnga
        mbed-mcux-sdk/drivers/sai
        mbed-mcux-sdk/drivers/sim
        mbed-mcux-sdk/drivers/smc
        mbed-mcux-sdk/drivers/rtc
        mbed-mcux-sdk/drivers/vref
        mbed-mcux-sdk/drivers/wdog
)

# Link appropriate Mbed OS HALs that interface with this MCU
target_include_directories(mbed-mcu-k2x
    INTERFACE
        mbed-hal-drivers/kinetis-common-headers
        mbed-hal-drivers/us-ticker-via-PIT
)
target_sources(mbed-mcu-k2x
    INTERFACE
        mbed-hal-drivers/us-ticker-via-PIT/us_ticker.c
        mbed-hal-drivers/i2c-via-I2C/i2c_api.c
        mbed-hal-drivers/serial-via-UART/serial_api.c
        mbed-hal-drivers/lpticker-via-LPTMR/lp_ticker.c
        mbed-hal-drivers/gpio-via-GPIO/gpio_api.c
        mbed-hal-drivers/interruptin-via-GPIO-PORT/gpio_irq_api.c
        mbed-hal-drivers/flash-via-FLASH/flash_api.c
        mbed-hal-drivers/port-via-PORT/port_api.c
        mbed-hal-drivers/qspi-via-QSPI/qspi_api.c
        mbed-hal-drivers/rtc-via-RTC/rtc_api.c
        mbed-hal-drivers/analogin-via-ADC16/analogin_api.c
        mbed-hal-drivers/analogout-via-DAC/analogout_api.c
        mbed-hal-drivers/sleep-via-SMC/sleep.c
        mbed-hal-drivers/reset-reason-via-RCM/reset_reason.c
)

# Link appropriate headers and startup files

target_compile_definitions(mbed-mcu-k2x INTERFACE __PERFORMANCE_IMPLEMENTATION) # for startup ASM


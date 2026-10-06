
# Link appropriate SDK drivers for MIMXRT
target_sources(mbed-mcu-mimxrt105x_6x INTERFACE
    mbed-mcux-sdk/drivers/common/fsl_common.c
    mbed-mcux-sdk/drivers/common/fsl_common_arm.c
    mbed-mcux-sdk/drivers/adc_12b1msps_sar/fsl_adc.c
    mbed-mcux-sdk/drivers/adc_etc/fsl_adc_etc.c
    mbed-mcux-sdk/drivers/lpi2c/fsl_lpi2c.c
    mbed-mcux-sdk/drivers/lpi2c/fsl_lpi2c_edma.c
    mbed-mcux-sdk/drivers/lpspi/fsl_lpspi.c
    mbed-mcux-sdk/drivers/lpspi/fsl_lpspi_edma.c
    mbed-mcux-sdk/drivers/lpuart/fsl_lpuart.c
    mbed-mcux-sdk/drivers/lpuart/fsl_lpuart_edma.c
    mbed-mcux-sdk/drivers/flexio/fsl_flexio.c
    mbed-mcux-sdk/drivers/dcdc_1/fsl_dcdc.c
    mbed-mcux-sdk/drivers/gpc_1/fsl_gpc.c
    mbed-mcux-sdk/drivers/igpio/fsl_gpio.c
    mbed-mcux-sdk/drivers/pit/fsl_pit.c
    mbed-mcux-sdk/drivers/qtmr_1/fsl_qtmr.c
    mbed-mcux-sdk/drivers/gpt/fsl_gpt.c
    mbed-mcux-sdk/drivers/pwm/fsl_pwm.c
    mbed-mcux-sdk/drivers/edma/fsl_edma.c
    mbed-mcux-sdk/drivers/dmamux/fsl_dmamux.c
    mbed-mcux-sdk/drivers/enet/fsl_enet.c
    mbed-mcux-sdk/drivers/flexspi/fsl_flexspi.c
    mbed-mcux-sdk/drivers/flexspi/fsl_flexspi_edma.c
    mbed-mcux-sdk/drivers/wdog01/fsl_wdog.c
    mbed-mcux-sdk/drivers/rtwdog/fsl_rtwdog.c
    mbed-mcux-sdk/drivers/xbara/fsl_xbara.c
    mbed-mcux-sdk/drivers/trng/fsl_trng.c
    mbed-mcux-sdk/drivers/snvs_lp/fsl_snvs_lp.c
)

target_include_directories(mbed-mcu-mimxrt105x_6x
    INTERFACE
        mbed-mcux-sdk/drivers/common
        mbed-mcux-sdk/drivers/adc_12b1msps_sar
        mbed-mcux-sdk/drivers/lpi2c
        mbed-mcux-sdk/drivers/lpspi
        mbed-mcux-sdk/drivers/lpuart
        mbed-mcux-sdk/drivers/flexio
        mbed-mcux-sdk/drivers/dcdc_1
        mbed-mcux-sdk/drivers/gpc_1
        mbed-mcux-sdk/drivers/igpio
        mbed-mcux-sdk/drivers/vref
        mbed-mcux-sdk/drivers/pit
        mbed-mcux-sdk/drivers/qtmr_1
        mbed-mcux-sdk/drivers/gpt
        mbed-mcux-sdk/drivers/pit
        mbed-mcux-sdk/drivers/pwm
        mbed-mcux-sdk/drivers/edma
        mbed-mcux-sdk/drivers/dmamux
        mbed-mcux-sdk/drivers/enet
        mbed-mcux-sdk/drivers/flexspi
        mbed-mcux-sdk/drivers/xbara
        mbed-mcux-sdk/drivers/wdog01
        mbed-mcux-sdk/drivers/rtwdog
        mbed-mcux-sdk/drivers/trng
        mbed-mcux-sdk/drivers/snvs_lp
)

# We use the MCUX-based USB driver stack
target_link_libraries(mbed-mcu-mimxrt105x_6x INTERFACE mbed-nxp-usb)

# Link appropriate Mbed OS HALs that interface with this MCU
target_include_directories(mbed-mcu-mimxrt105x_6x
    INTERFACE
        mbed-hal-drivers/mimxrt-common-headers
        mbed-hal-drivers/fsl-os-abstraction
        mbed-hal-drivers/serial-via-LPUART
        mbed-hal-drivers/usb-low-level-via-imxrt-USB
        mbed-hal-drivers/us-ticker-via-PIT
        mbed-hal-drivers/flash-via-FLEXSPI
)
target_sources(mbed-mcu-mimxrt105x_6x
    INTERFACE
        mbed-hal-drivers/fsl-os-abstraction/fsl_os_abstraction_mbed.c
        mbed-hal-drivers/serial-via-LPUART/serial_api.c
        mbed-hal-drivers/usb-low-level-via-imxrt-USB/usb_device_ch9.c
        mbed-hal-drivers/usb-low-level-via-imxrt-USB/usb_device_class.c
        mbed-hal-drivers/us-ticker-via-PIT/us_ticker.c
        mbed-hal-drivers/pinmode-via-IOMUXC/pinmap.c
        mbed-hal-drivers/flash-via-FLEXSPI/flash_api.c
        mbed-hal-drivers/i2c-via-LPI2C/i2c_api.c
        mbed-hal-drivers/lpticker-via-GPT/lp_ticker.c
        mbed-hal-drivers/gpio-and-irq-and-port-via-IGPIO/port_api.c
        mbed-hal-drivers/gpio-and-irq-and-port-via-IGPIO/gpio_irq_api.c
        mbed-hal-drivers/gpio-and-irq-and-port-via-IGPIO/gpio_api.c
        mbed-hal-drivers/analogin-via-RT105x-ADC/analogin_api.c
        mbed-hal-drivers/trng-via-TRNG/trng_api.c
        mbed-hal-drivers/spi-via-LPSPI/spi_api.c
        mbed-hal-drivers/rtc-via-SNVS-LP-SRTC/rtc_api.c
        mbed-hal-drivers/watchdog-via-RTWDOG/watchdog_api.c
        mbed-hal-drivers/pwmout-via-FlexPWM/pwmout_api.c
)

# Link appropriate startup files for RT105x
target_sources(mbed-mcu-mimxrt105x INTERFACE
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1052/system_MIMXRT1052.c
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1052/drivers/fsl_clock.c
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1052/drivers/romapi/fsl_romapi.c
)
target_include_directories(mbed-mcu-mimxrt105x INTERFACE
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1052/
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1052/periph
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1052/drivers/
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1052/drivers/nic301
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1052/drivers/romapi
)

# Link appropriate startup files for RT106x
target_sources(mbed-mcu-mimxrt106x INTERFACE
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1062/system_MIMXRT1062.c
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1062/drivers/fsl_clock.c
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1062/drivers/romapi/fsl_romapi.c
)
target_include_directories(mbed-mcu-mimxrt106x INTERFACE
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1062/
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1062/periph
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1062/drivers/
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1062/drivers/nic301
    mbed-mcux-sdk/devices/mimxrt/MIMXRT1062/drivers/romapi
)

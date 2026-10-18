# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mcu-mimxrt116x_7x INTERFACE)

add_library(mbed-mcu-mimxrt117x INTERFACE)
target_link_libraries(mbed-mcu-mimxrt117x INTERFACE mbed-mcu-mimxrt116x_7x)

# Link appropriate SDK drivers for MIMXRT116x/7x
target_sources(mbed-mcu-mimxrt116x_7x INTERFACE
        mbed-mcux-sdk/drivers/common/fsl_common.c
        mbed-mcux-sdk/drivers/common/fsl_common_arm.c
        mbed-mcux-sdk/drivers/adc_etc/fsl_adc_etc.c
        mbed-mcux-sdk/drivers/lpi2c/fsl_lpi2c.c
        mbed-mcux-sdk/drivers/lpi2c/fsl_lpi2c_edma.c
        mbed-mcux-sdk/drivers/lpspi/fsl_lpspi.c
        mbed-mcux-sdk/drivers/lpspi/fsl_lpspi_edma.c
        mbed-mcux-sdk/drivers/lpuart/fsl_lpuart.c
        mbed-mcux-sdk/drivers/lpuart/fsl_lpuart_edma.c
        mbed-mcux-sdk/drivers/flexio/fsl_flexio.c
        mbed-mcux-sdk/drivers/flexcan/fsl_flexcan.c
        mbed-mcux-sdk/drivers/flexcan/fsl_flexcan_edma.c
        mbed-mcux-sdk/drivers/usdhc/fsl_usdhc.c
        mbed-mcux-sdk/drivers/lpadc/fsl_lpadc.c
        mbed-mcux-sdk/drivers/dac12/fsl_dac12.c
        mbed-mcux-sdk/drivers/tempsensor/fsl_tempsensor.c
        mbed-mcux-sdk/drivers/snvs_lp/fsl_snvs_lp.c
        mbed-mcux-sdk/drivers/mecc/fsl_mecc.c
        mbed-mcux-sdk/drivers/xecc/fsl_xecc.c
        mbed-mcux-sdk/drivers/edma/fsl_edma.c
        mbed-mcux-sdk/drivers/igpio/fsl_gpio.c
        mbed-mcux-sdk/drivers/qtmr_1/fsl_qtmr.c
        mbed-mcux-sdk/drivers/pwm/fsl_pwm.c
        mbed-mcux-sdk/drivers/gpt/fsl_gpt.c
        mbed-mcux-sdk/drivers/pit/fsl_pit.c
        mbed-mcux-sdk/drivers/rtwdog/fsl_rtwdog.c
        mbed-mcux-sdk/drivers/wdog01/fsl_wdog.c
        mbed-mcux-sdk/drivers/flexspi/fsl_flexspi.c
        mbed-mcux-sdk/drivers/flexspi/fsl_flexspi_edma.c
        mbed-mcux-sdk/drivers/ocotp/fsl_ocotp.c
        mbed-mcux-sdk/drivers/xbara/fsl_xbara.c
        mbed-mcux-sdk/drivers/caam/fsl_caam.c
        mbed-mcux-sdk/drivers/key_manager/fsl_key_manager.c
        mbed-mcux-sdk/drivers/puf/fsl_puf.c
        mbed-mcux-sdk/drivers/mu/fsl_mu.c
        mbed-mcux-sdk/drivers/rdc/fsl_rdc.c
        mbed-mcux-sdk/drivers/rdc_sema42/fsl_rdc_sema42.c
        mbed-mcux-sdk/drivers/xrdc2/fsl_xrdc2.c
        mbed-mcux-sdk/drivers/sema4/fsl_sema4.c
)

target_include_directories(mbed-mcu-mimxrt116x_7x INTERFACE
        mbed-mcux-sdk/drivers/common
        mbed-mcux-sdk/drivers/adc_etc
        mbed-mcux-sdk/drivers/lpi2c
        mbed-mcux-sdk/drivers/lpspi
        mbed-mcux-sdk/drivers/lpuart
        mbed-mcux-sdk/drivers/flexio
        mbed-mcux-sdk/drivers/flexcan
        mbed-mcux-sdk/drivers/usdhc
        mbed-mcux-sdk/drivers/lpadc
        mbed-mcux-sdk/drivers/dac12
        mbed-mcux-sdk/drivers/tempsensor
        mbed-mcux-sdk/drivers/snvs_lp
        mbed-mcux-sdk/drivers/mecc
        mbed-mcux-sdk/drivers/xecc
        mbed-mcux-sdk/drivers/edma
        mbed-mcux-sdk/drivers/dmamux
        mbed-mcux-sdk/drivers/igpio
        mbed-mcux-sdk/drivers/qtmr_1
        mbed-mcux-sdk/drivers/pwm
        mbed-mcux-sdk/drivers/gpt
        mbed-mcux-sdk/drivers/pit
        mbed-mcux-sdk/drivers/rtwdog
        mbed-mcux-sdk/drivers/wdog01
        mbed-mcux-sdk/drivers/flexspi
        mbed-mcux-sdk/drivers/ocotp
        mbed-mcux-sdk/drivers/xbara
        mbed-mcux-sdk/drivers/caam
        mbed-mcux-sdk/drivers/key_manager
        mbed-mcux-sdk/drivers/puf
        mbed-mcux-sdk/drivers/mu
        mbed-mcux-sdk/drivers/rdc
        mbed-mcux-sdk/drivers/rdc_sema42
        mbed-mcux-sdk/drivers/xrdc2
        mbed-mcux-sdk/drivers/mcm
)

# Link appropriate Mbed OS HALs that interface with this MCU
target_include_directories(mbed-mcu-mimxrt116x_7x
    INTERFACE
        mbed-hal-drivers/mimxrt-common-headers
        mbed-hal-drivers/fsl-os-abstraction
        mbed-hal-drivers/serial-via-LPUART
        mbed-hal-drivers/usb-low-level-via-imxrt-USB
        mbed-hal-drivers/us-ticker-via-PIT
        mbed-hal-drivers/flash-via-FLEXSPI
)
target_sources(mbed-mcu-mimxrt116x_7x
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
        mbed-hal-drivers/spi-via-LPSPI/spi_api.c
        mbed-hal-drivers/watchdog-via-RTWDOG/watchdog_api.c
        mbed-hal-drivers/pwmout-via-FlexPWM/pwmout_api.c
)

# We use the MCUX-based USB driver stack
target_link_libraries(mbed-mcu-mimxrt116x_7x INTERFACE mbed-nxp-usb)

# Link appropriate startup files for RT117x
target_sources(mbed-mcu-mimxrt117x INTERFACE
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/system_MIMXRT1176_cm7.c
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/gcc/startup_MIMXRT1176_cm7.S
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/fsl_clock.c
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/anatop_ai/fsl_anatop_ai.c
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/romapi/fsl_romapi.c
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/power/fsl_gpc.c
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/power/fsl_dcdc.c
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/power/fsl_pgmc.c
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/power/fsl_pmu.c
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/power/fsl_soc_src.c
)
target_include_directories(mbed-mcu-mimxrt117x INTERFACE
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/periph
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/nic301
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/romapi
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/anatop_ai
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/memory
        mbed-mcux-sdk/devices/mimxrt/MIMXRT1176/drivers/power
)

# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

# LPC5xxxx family
add_library(mbed-mcu-lpc54xxx INTERFACE)
add_library(mbed-mcu-lpc546xx INTERFACE)
target_link_libraries(mbed-mcu-lpc546xx INTERFACE mbed-mcu-lpc54xxx)

# Link common SDK drivers
target_sources(mbed-mcu-lpc54xxx
    INTERFACE
        mbed-mcux-sdk/drivers/common/fsl_common_arm.c
        mbed-mcux-sdk/drivers/flexcomm/fsl_flexcomm.c
        mbed-mcux-sdk/drivers/flexcomm/usart/fsl_usart.c
        mbed-mcux-sdk/drivers/flexcomm/usart/fsl_usart_dma.c
        mbed-mcux-sdk/drivers/flexcomm/spi/fsl_spi.c
        mbed-mcux-sdk/drivers/flexcomm/spi/fsl_spi_dma.c
        mbed-mcux-sdk/drivers/flexcomm/i2c/fsl_i2c.c
        mbed-mcux-sdk/drivers/flexcomm/i2c/fsl_i2c_dma.c
        mbed-mcux-sdk/drivers/lpc_enet/fsl_enet.c
        mbed-mcux-sdk/drivers/mcan/fsl_mcan.c
        mbed-mcux-sdk/drivers/spifi/fsl_spifi.c
        mbed-mcux-sdk/drivers/spifi/fsl_spifi_dma.c
        mbed-mcux-sdk/drivers/lpc_adc/fsl_adc.c
        mbed-mcux-sdk/drivers/lpc_dma/fsl_dma.c
        mbed-mcux-sdk/drivers/gint/fsl_gint.c
        mbed-mcux-sdk/drivers/lpc_gpio/fsl_gpio.c
        mbed-mcux-sdk/drivers/inputmux/fsl_inputmux.c
        mbed-mcux-sdk/drivers/pint/fsl_pint.c
        mbed-mcux-sdk/drivers/fmeas/fsl_fmeas.c
        mbed-mcux-sdk/drivers/ctimer/fsl_ctimer.c
        mbed-mcux-sdk/drivers/mrt/fsl_mrt.c
        mbed-mcux-sdk/drivers/lpc_rit/fsl_rit.c
        mbed-mcux-sdk/drivers/lpc_rtc/fsl_rtc.c
        mbed-mcux-sdk/drivers/sctimer/fsl_sctimer.c
        mbed-mcux-sdk/drivers/wwdt/fsl_wwdt.c
        mbed-mcux-sdk/drivers/utick/fsl_utick.c
        mbed-mcux-sdk/drivers/flashiap/fsl_flashiap.c
        mbed-mcux-sdk/drivers/fmc/fsl_fmc.c
        mbed-mcux-sdk/drivers/rng/fsl_rng.c
        mbed-mcux-sdk/drivers/sha/fsl_sha.c
        mbed-mcux-sdk/drivers/emc/fsl_emc.c
)

target_include_directories(mbed-mcu-lpc54xxx
    INTERFACE
        mbed-mcux-sdk/drivers/common
        mbed-mcux-sdk/drivers/flexcomm
        mbed-mcux-sdk/drivers/flexcomm/usart
        mbed-mcux-sdk/drivers/flexcomm/spi
        mbed-mcux-sdk/drivers/flexcomm/i2c
        mbed-mcux-sdk/drivers/lpc_enet
        mbed-mcux-sdk/drivers/mcan
        mbed-mcux-sdk/drivers/spifi
        mbed-mcux-sdk/drivers/lpc_adc
        mbed-mcux-sdk/drivers/lpc_dma
        mbed-mcux-sdk/drivers/gint
        mbed-mcux-sdk/drivers/lpc_gpio
        mbed-mcux-sdk/drivers/inputmux
        mbed-mcux-sdk/drivers/lpc_iocon
        mbed-mcux-sdk/drivers/pint
        mbed-mcux-sdk/drivers/fmeas
        mbed-mcux-sdk/drivers/ctimer
        mbed-mcux-sdk/drivers/mrt
        mbed-mcux-sdk/drivers/lpc_rit
        mbed-mcux-sdk/drivers/lpc_rtc
        mbed-mcux-sdk/drivers/sctimer
        mbed-mcux-sdk/drivers/wwdt
        mbed-mcux-sdk/drivers/utick
        mbed-mcux-sdk/drivers/flashiap
        mbed-mcux-sdk/drivers/fmc
        mbed-mcux-sdk/drivers/otp
        mbed-mcux-sdk/drivers/rng
        mbed-mcux-sdk/drivers/sha
        mbed-mcux-sdk/drivers/emc
)

# Link Mbed HAL drivers
target_sources(mbed-mcu-lpc54xxx
    INTERFACE
        mbed-hal-drivers/serial-via-FLEXCOMM/serial_api.c
        mbed-hal-drivers/gpio-via-LPC-GPIO/gpio_api.c
        mbed-hal-drivers/interruptin-via-PINT/gpio_irq_api.c
        mbed-hal-drivers/trng-via-RNG/trng_api.c
        mbed-hal-drivers/analogin-via-LPC-ADC/analogin_api.c
        mbed-hal-drivers/us-ticker-via-CTIMER/us_ticker.c
        mbed-hal-drivers/port-via-LPC-GPIO/port_api.c
        mbed-hal-drivers/i2c-via-FLEXCOMM/i2c_api.c
        mbed-hal-drivers/rtc-via-LPC-RTC/rtc_api.c
        mbed-hal-drivers/spi-via-FLEXCOMM/spi_api.c
        mbed-hal-drivers/sleep-via-LPC-SYSCON-POWER/sleep.c
        mbed-hal-drivers/pinmode-via-IOCON/pinmap.c
        mbed-hal-drivers/flash-via-FLASHIAP/flash_api.c
        mbed-hal-drivers/qspi-via-SPIFI/qspi_api.c
)

target_include_directories(mbed-mcu-lpc54xxx
    INTERFACE
        mbed-hal-drivers/us-ticker-via-CTIMER
)

# Link appropriate headers and startup files
target_sources(mbed-mcu-lpc546xx INTERFACE
        mbed-mcux-sdk/devices/lpc/LPC54628/system_LPC54628.c
        mbed-mcux-sdk/devices/lpc/LPC54628/gcc/startup_LPC54628.S
        mbed-mcux-sdk/devices/lpc/LPC54628/drivers/fsl_clock.c
        mbed-mcux-sdk/devices/lpc/LPC54628/drivers/fsl_power.c
        mbed-mcux-sdk/devices/lpc/LPC54628/drivers/fsl_reset.c
)
target_include_directories(mbed-mcu-lpc546xx INTERFACE
        mbed-mcux-sdk/devices/lpc/LPC54628/
        mbed-mcux-sdk/devices/lpc/LPC54628/drivers
        mbed-mcux-sdk/devices/lpc/LPC54628/periph
)

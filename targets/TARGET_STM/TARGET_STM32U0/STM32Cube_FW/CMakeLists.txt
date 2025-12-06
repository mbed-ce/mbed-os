# Copyright (c) 2025 Jan Kamidra
# SPDX-License-Identifier: Apache-2.0

mbed_setup_submodule(cmsis-device-u0 CHECK_FILE Source/Templates/system_stm32u0xx.c IF_LABEL MCU_STM32U0)
mbed_setup_submodule(stm32u0xx-hal-driver CHECK_FILE Src/stm32u0xx_hal.c IF_LABEL MCU_STM32U0)

add_library(mbed-stm32u0cube-fw INTERFACE)

target_sources(mbed-stm32u0cube-fw
    INTERFACE
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_lptim.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_lpuart.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_opamp.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_pwr.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_rng.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_rtc.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_spi.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_tim.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_usart.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_usb.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_utils.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_adc.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_adc_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_comp.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_cortex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_crc.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_crc_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_cryp.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_cryp_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_dac.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_dac_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_dma.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_dma_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_exti.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_flash.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_flash_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_gpio.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_i2c.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_i2c_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_irda.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_iwdg.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_lcd.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_lptim.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_opamp.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_opamp_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_pcd.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_pcd_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_pwr.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_pwr_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_rcc.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_rcc_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_rng.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_rng_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_rtc.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_rtc_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_smartcard.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_smartcard_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_spi.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_spi_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_tim.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_tim_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_tsc.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_uart.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_uart_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_usart.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_usart_ex.c
        stm32u0xx-hal-driver/Src/stm32u0xx_hal_wwdg.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_adc.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_comp.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_crc.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_crs.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_dac.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_dma.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_exti.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_gpio.c
        stm32u0xx-hal-driver/Src/stm32u0xx_ll_i2c.c

        # For now we use this file unmodified, so we can get it directly from the submodule
        cmsis-device-u0/Source/Templates/system_stm32u0xx.c
)

target_include_directories(mbed-stm32u0cube-fw
    INTERFACE
        .
        cmsis-device-u0/Include
        stm32u0xx-hal-driver/Inc
        stm32u0xx-hal-driver/Inc/Legacy
)

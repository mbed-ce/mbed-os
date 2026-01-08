# Copyright (c) 2025 MbedCE Community Contributors (Jan Kamidra)
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Created by: Jan Kamidra with GitHub Copilot

mbed_setup_submodule(cmsis-device-l1      CHECK_FILE Source/Templates/system_stm32l1xx.c    IF_LABEL MCU_STM32L1)
mbed_setup_submodule(stm32l1xx-hal-driver CHECK_FILE Src/stm32l1xx_hal.c                    IF_LABEL MCU_STM32L1)

add_library(mbed-stm32l1cube-fw INTERFACE)

target_sources(mbed-stm32l1cube-fw
    INTERFACE
        stm32l1xx-hal-driver/Src/stm32l1xx_hal.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_adc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_adc_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_comp.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_cortex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_crc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_cryp.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_cryp_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_dac.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_dac_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_dma.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_exti.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_flash.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_flash_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_flash_ramfunc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_gpio.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_i2c.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_i2s.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_irda.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_iwdg.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_lcd.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_nor.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_opamp.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_opamp_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_pcd.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_pcd_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_pwr.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_pwr_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_rcc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_rcc_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_rtc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_rtc_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_sd.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_smartcard.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_spi.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_sram.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_tim.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_tim_ex.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_uart.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_usart.c
        stm32l1xx-hal-driver/Src/stm32l1xx_hal_wwdg.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_adc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_comp.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_crc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_dac.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_dma.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_exti.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_fsmc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_gpio.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_i2c.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_opamp.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_pwr.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_rcc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_rtc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_sdmmc.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_spi.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_tim.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_usart.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_usb.c
        stm32l1xx-hal-driver/Src/stm32l1xx_ll_utils.c
        system_stm32l1xx.c
)

target_include_directories(mbed-stm32l1cube-fw
    INTERFACE
        .
        cmsis-device-l1/Include
        stm32l1xx-hal-driver/Inc
        stm32l1xx-hal-driver/Inc/Legacy
)

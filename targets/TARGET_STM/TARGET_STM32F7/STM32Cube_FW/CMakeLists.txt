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

mbed_setup_submodule(cmsis-device-f7      CHECK_FILE Source/Templates/system_stm32f7xx.c    IF_LABEL MCU_STM32F7)
mbed_setup_submodule(stm32f7xx-hal-driver CHECK_FILE Src/stm32f7xx_hal.c                    IF_LABEL MCU_STM32F7)

add_library(mbed-stm32f7cube-fw INTERFACE)

target_sources(mbed-stm32f7cube-fw
    INTERFACE
        system_stm32f7xx.c
        stm32f7xx-hal-driver/Src/Legacy/stm32f7xx_hal_can.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_adc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_adc_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_can.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_cec.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_cortex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_crc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_crc_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_cryp.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_cryp_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_dac.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_dac_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_dcmi.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_dcmi_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_dfsdm.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_dma2d.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_dma_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_dsi.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_exti.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_flash.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_flash_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_gpio.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_hash.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_hash_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_hcd.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_i2c.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_i2c_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_i2s.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_irda.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_iwdg.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_jpeg.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_lptim.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_ltdc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_ltdc_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_mdios.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_mmc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_nand.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_nor.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_pcd.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_pcd_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_pwr.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_pwr_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_qspi.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_rcc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_rcc_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_rng.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_rtc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_rtc_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_sai.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_sai_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_sd.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_sdram.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_smartcard.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_smartcard_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_smbus.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_spdifrx.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_spi.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_spi_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_sram.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_tim.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_tim_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_uart.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_uart_ex.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_usart.c
        stm32f7xx-hal-driver/Src/stm32f7xx_hal_wwdg.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_adc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_crc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_dac.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_dma.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_dma2d.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_exti.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_fmc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_gpio.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_i2c.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_lptim.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_pwr.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_rcc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_rng.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_rtc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_sdmmc.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_spi.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_tim.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_usart.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_usb.c
        stm32f7xx-hal-driver/Src/stm32f7xx_ll_utils.c
        Mbed_HAL_overrides/stm32f7xx_hal_dma.c
)

target_include_directories(mbed-stm32f7cube-fw
    INTERFACE
        .
        cmsis-device-f7/Include
        stm32f7xx-hal-driver/Inc
        stm32f7xx-hal-driver/Inc/Legacy
)

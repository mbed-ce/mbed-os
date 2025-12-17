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

mbed_setup_submodule(cmsis-device-g4      CHECK_FILE Source/Templates/system_stm32g4xx.c    IF_LABEL MCU_STM32G4)
mbed_setup_submodule(stm32g4xx-hal-driver CHECK_FILE Src/stm32g4xx_hal.c                    IF_LABEL MCU_STM32G4)

add_library(mbed-stm32g4cube-fw INTERFACE)

target_sources(mbed-stm32g4cube-fw
    INTERFACE
       stm32g4xx-hal-driver/Src/stm32g4xx_hal.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_adc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_adc_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_comp.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_cordic.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_cortex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_crc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_crc_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_cryp.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_cryp_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_dac.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_dac_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_dma.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_dma_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_exti.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_fdcan.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_flash.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_flash_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_flash_ramfunc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_fmac.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_gpio.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_hrtim.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_i2c.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_i2c_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_i2s.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_irda.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_iwdg.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_lptim.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_nand.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_nor.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_opamp.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_opamp_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_pcd.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_pcd_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_pwr.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_pwr_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_qspi.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_rcc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_rcc_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_rng.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_rtc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_rtc_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_sai.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_sai_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_smartcard.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_smartcard_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_smbus.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_smbus_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_spi.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_spi_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_sram.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_tim.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_tim_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_uart.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_uart_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_usart.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_usart_ex.c
       stm32g4xx-hal-driver/Src/stm32g4xx_hal_wwdg.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_adc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_comp.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_cordic.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_crc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_crs.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_dac.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_dma.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_exti.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_fmac.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_fmc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_gpio.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_hrtim.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_i2c.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_lptim.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_lpuart.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_opamp.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_pwr.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_rcc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_rng.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_rtc.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_spi.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_tim.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_ucpd.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_usart.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_usb.c
       stm32g4xx-hal-driver/Src/stm32g4xx_ll_utils.c
        system_stm32g4xx.c
)

target_include_directories(mbed-stm32g4cube-fw
    INTERFACE
        .
        cmsis-device-g4/Include
        stm32g4xx-hal-driver/Inc
        stm32g4xx-hal-driver/Inc/Legacy
)

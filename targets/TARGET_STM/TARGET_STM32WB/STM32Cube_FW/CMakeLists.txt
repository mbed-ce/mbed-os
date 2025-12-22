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

mbed_setup_submodule(cmsis-device-wb      CHECK_FILE Source/Templates/system_stm32wbxx.c    IF_LABEL MCU_STM32WB)
mbed_setup_submodule(stm32wbxx-hal-driver CHECK_FILE Src/stm32wbxx_hal.c                    IF_LABEL MCU_STM32WB)

add_library(mbed-stm32wbcube-fw INTERFACE)

target_sources(mbed-stm32wbcube-fw
    INTERFACE
        stm32wbxx-hal-driver/Src/stm32wbxx_hal.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_adc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_adc_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_comp.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_cortex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_crc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_crc_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_cryp.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_cryp_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_dma.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_dma_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_exti.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_flash.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_flash_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_gpio.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_hsem.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_i2c.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_i2c_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_ipcc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_irda.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_iwdg.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_lcd.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_lptim.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_pcd.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_pcd_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_pka.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_pwr.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_pwr_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_qspi.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_rcc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_rcc_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_rng.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_rtc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_rtc_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_sai.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_sai_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_smartcard.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_smartcard_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_smbus.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_spi.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_spi_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_tim.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_tim_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_tsc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_uart.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_uart_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_usart.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_usart_ex.c
        stm32wbxx-hal-driver/Src/stm32wbxx_hal_wwdg.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_adc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_comp.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_crc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_crs.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_dma.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_exti.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_gpio.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_i2c.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_lptim.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_lpuart.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_pka.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_pwr.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_rcc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_rng.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_rtc.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_spi.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_tim.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_usart.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_usb.c
        stm32wbxx-hal-driver/Src/stm32wbxx_ll_utils.c
        system_stm32wbxx.c
        otp.c
        stm32_lpm_if.c
)

target_include_directories(mbed-stm32wbcube-fw
    INTERFACE
        .
        cmsis-device-wb/Include
        stm32wbxx-hal-driver/Inc
        stm32wbxx-hal-driver/Inc/Legacy
)

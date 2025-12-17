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

mbed_setup_submodule(cmsis-device-wl      CHECK_FILE Source/Templates/system_stm32wlxx.c    IF_LABEL MCU_STM32WL)
mbed_setup_submodule(stm32wlxx-hal-driver CHECK_FILE Src/stm32wlxx_hal.c                    IF_LABEL MCU_STM32WL)

add_library(mbed-stm32wlcube-fw INTERFACE)

target_sources(mbed-stm32wlcube-fw
    INTERFACE
         stm32wlxx-hal-driver/Src/stm32wlxx_hal.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_adc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_adc_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_comp.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_cortex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_crc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_crc_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_cryp.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_cryp_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_dac.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_dac_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_dma.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_dma_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_exti.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_flash.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_flash_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_gpio.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_gtzc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_hsem.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_i2c.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_i2c_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_i2s.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_ipcc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_irda.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_iwdg.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_lptim.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_pka.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_pwr.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_pwr_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_rcc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_rcc_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_rng.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_rng_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_rtc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_rtc_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_smartcard.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_smartcard_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_smbus.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_smbus_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_spi.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_spi_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_subghz.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_tim.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_tim_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_uart.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_uart_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_usart.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_usart_ex.c
         stm32wlxx-hal-driver/Src/stm32wlxx_hal_wwdg.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_adc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_comp.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_crc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_dac.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_dma.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_exti.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_gpio.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_i2c.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_lptim.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_lpuart.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_pka.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_pwr.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_rcc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_rng.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_rtc.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_spi.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_tim.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_usart.c
         stm32wlxx-hal-driver/Src/stm32wlxx_ll_utils.c
        system_stm32wlxx.c
)

target_include_directories(mbed-stm32wlcube-fw
    INTERFACE
        .
        cmsis-device-wl/Include
        stm32wlxx-hal-driver/Inc
        stm32wlxx-hal-driver/Inc/Legacy
)

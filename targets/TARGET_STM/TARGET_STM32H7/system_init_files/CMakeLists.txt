# Copyright (c) 2024 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

# Add correct system init file for each MCU.
target_sources(mbed-stm32h7a3xiq INTERFACE system_stm32h7xx_singlecore.c)
target_sources(mbed-stm32h7b3xiq INTERFACE system_stm32h7xx_singlecore.c)
target_sources(mbed-stm32h723xg INTERFACE system_stm32h7xx_singlecore.c)
target_sources(mbed-stm32h725xe INTERFACE system_stm32h7xx_singlecore.c)
target_sources(mbed-stm32h735xg INTERFACE system_stm32h7xx_singlecore.c)
target_sources(mbed-stm32h743xi INTERFACE system_stm32h7xx_singlecore.c)
target_sources(mbed-stm32h750xb INTERFACE system_stm32h7xx_singlecore.c)
target_sources(mbed-stm32h753xi INTERFACE system_stm32h7xx_singlecore.c)

target_sources(mbed-stm32h745xi INTERFACE system_stm32h7xx_dualcore_boot_cm4_cm7.c)
target_sources(mbed-stm32h747xi INTERFACE system_stm32h7xx_dualcore_boot_cm4_cm7.c)
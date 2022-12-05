# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mimxrt1050-evk INTERFACE)

target_include_directories(mbed-mimxrt1050-evk
    INTERFACE
        .
        xip
)

target_sources(mbed-mimxrt1050-evk
    INTERFACE
        flash_api.c
        fsl_phy.c
		fsl_flexspi_nor_boot.c

        xip/evkbimxrt1050_flexspi_nor_config.c
        xip/evkbimxrt1050_sdram_ini_dcd.c
)

target_link_libraries(mbed-mimxrt1050-evk
	INTERFACE
		mbed-mimxrt105x
)
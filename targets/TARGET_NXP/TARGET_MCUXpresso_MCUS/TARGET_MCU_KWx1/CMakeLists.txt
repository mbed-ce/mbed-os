# Copyright (c) 2026 Jamie Smith
# SPDX-License-Identifier: Apache-2.0

mbed_set_linker_script(mbed-mcu-kwx1 ${CMAKE_CURRENT_SOURCE_DIR}/MKWx1.ld)

target_include_directories(mbed-mcu-kwx1 INTERFACE . XCVR)

target_sources(mbed-mcu-kwx1 INTERFACE
    PeripheralPins.c
    mbed_overrides.c
    lpuart_device.c
    XCVR/dbg_ram_capture.c
    XCVR/fsl_xcvr.c
    XCVR/fsl_xcvr_trim.c
    XCVR/ifr_radio.c

    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_ant_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_ble_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_common_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p3_h_0p5_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p5_h_0p32_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p5_h_0p5_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p5_h_0p7_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p5_h_1p0_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p7_h_0p5_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_mode_datarate_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_msk_config.c
    XCVR/cfgs_kw4x_3x_2x/fsl_xcvr_zgbe_config.c
)

add_subdirectory(TARGET_MCU_KW41Z)
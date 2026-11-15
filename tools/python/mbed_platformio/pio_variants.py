"""
Maps PIO variant name to Mbed target name.

This is for the situation where the Mbed target name is different
from the uppercased version of the variant name

Copyright (c) 2025 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

PIO_VARIANT_TO_MBED_TARGET = {
    "seeedArchPro": "ARCH_PRO",
    "frdm_kl25z": "KL25Z",
    "frdm_kl46z": "KL46Z",
    "IBMEthernetKit": "FRDM_K64F",
    "cloud_jam": "NUCLEO_F401RE",
    "cloud_jam_l4": "NUCLEO_L476RG",
    "nucleo_h743zi": "NUCLEO_H743ZI2",
    "genericSTM32F103RB": "NUCLEO_F103RB",
    "disco_h747xi": "DISCO_H747I",
    "rpipico": "RASPBERRY_PI_PICO",
    "rpipicow": "RASPBERRY_PI_PICO_W",
    "sparkfun_thingplusrp2040": "SFE_THING_PLUS_RP2040",
    "rpipico2": "RASPBERRY_PI_PICO_2",
    "olimex_pico2xl": "OLIMEX_RP2350_PICO2_XL",
    "teensy40": "TEENSY_40",
    "teensy41": "TEENSY_41",
}

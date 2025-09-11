"""
Maps PIO variant name to Mbed target name, in the situation where the Mbed target name is different
from the uppercased version of the variant name

Copyright (c) 2025 Jamie Smith
SPDX-License-Identifier: Apache-2.0
"""

PIO_VARIANT_TO_MBED_TARGET = {
    "seeedArchPro": "ARCH_PRO",
    "seeedArchMax": "ARCH_MAX",
    "frdm_kl25z": "KL25Z",
    "frdm_kl43z": "KL43Z",
    "frdm_kl46z": "KL46Z",
    "frdm_k64f": "K64F",
    "frdm_k82f": "K82F",
    "IBMEthernetKit": "K64F",
    "frdm_k66f": "K66F",
    "frdm_k22f": "K22F",
    "frdm_kw41z": "KW41Z",
    "cloud_jam": "NUCLEO_F401RE",
    "cloud_jam_l4": "NUCLEO_L476RG",
    "nucleo_h743zi": "NUCLEO_H743ZI2",
    "genericSTM32F103RB": "NUCLEO_F103RB",
    "disco_h747xi": "DISCO_H747I"
}

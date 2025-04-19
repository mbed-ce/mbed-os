/* mbed Microcontroller Library
 * Copyright (c) 2025 Jamie Smith
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed_boot.h"
#include "clock_config.h"
#include "fsl_flashiap.h"
#include "fsl_power.h"

#include "MbedCRC.h"

// called before main
void mbed_sdk_init()
{
    if (SYSCON->DEVICE_ID0 == 0xFFF54628) {
        BOARD_BootClockFROHF96M(); /* Boot up FROHF96M for SPIFI to use*/
        /* LPC54628 runs at a higher core speed */
        BOARD_BootClockPLL220M();
    } else {
        BOARD_BootClockFROHF96M(); /* Boot up FROHF96M for SPIFI to use*/
        BOARD_BootClockPLL180M();
    }
}

// Get the QSPI clock frequency
extern "C" uint32_t qspi_get_freq(void)
{
    CLOCK_AttachClk(kFRO_HF_to_SPIFI_CLK);

    return CLOCK_GetFroHfFreq();
}

uint32_t FLASHIAP_ReadUid(uint32_t *addr)
{
    uint32_t command[5], result[5];

    command[0] = kIapCmd_FLASHIAP_ReadUid;
    iap_entry(command, result);

    memcpy(addr, &result[1], (sizeof(uint32_t) * 4));

    return result[0];
}

// Provide ethernet devices with a semi-unique MAC address from the UUID
void mbed_mac_address(char *mac)
{
    uint32_t UID[4];

    // get UID via ISP commands
    FLASHIAP_ReadUid(UID);

    // generate two 32-bit words using two slices of the 16-byte UID
    mbed::MbedCRC<POLY_32BIT_ANSI, 32, mbed::CrcMode::BITWISE> crcGenerator;
    uint32_t word1, word2;
    crcGenerator.compute(UID, 8, &word1);
    crcGenerator.compute(&UID[2], 8, &word2);

    // Copy the words into the MAC address
    memcpy(mac, &word1, 4);
    memcpy(mac + 4, &word2, 2);

    // We want to force bits [1:0] of the most significant byte [0]
    // to be "10"
    // http://en.wikipedia.org/wiki/MAC_address
    mac[0] |= 0x02; // force bit 1 to a "1" = "Locally Administered"
    mac[0] &= 0xFE; // force bit 0 to a "0" = Unicast
}

// Enable the RTC oscillator if available on the board
extern "C" void rtc_setup_oscillator(void)
{
    /* Enable the RTC 32K Oscillator */
    SYSCON->RTCOSCCTRL |= SYSCON_RTCOSCCTRL_EN_MASK;
}

extern "C" uint32_t us_ticker_get_clock()
{
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

extern "C" void ADC_ClockPower_Configuration(void)
{
    /* SYSCON power. */
    POWER_DisablePD(kPDRUNCFG_PD_VDDA);    /* Power on VDDA. */
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);    /* Power on the ADC converter. */
    POWER_DisablePD(kPDRUNCFG_PD_VD2_ANA); /* Power on the analog power supply. */
    POWER_DisablePD(kPDRUNCFG_PD_VREFP);   /* Power on the reference voltage source. */
    POWER_DisablePD(kPDRUNCFG_PD_TS);      /* Power on the temperature sensor. */


    /* CLOCK_AttachClk(kMAIN_CLK_to_ADC_CLK); */
    /* Sync clock source is not used. Using sync clock source and would be divided by 2.
     * The divider would be set when configuring the converter.
     */
    CLOCK_EnableClock(kCLOCK_Adc0); /* SYSCON->AHBCLKCTRL[0] |= SYSCON_AHBCLKCTRL_ADC0_MASK; */
    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);
}
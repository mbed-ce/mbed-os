/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
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

#include "analogin_api.h"

void mbed_sdk_init(void)
{
    // NOTE: Support singleton semantics to be called from other init functions
    static int inited = 0;
    if (inited) {
        return;
    }
    inited = 1;
    
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC clock (Internal RC 22.1184MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
#if MBED_CONF_TARGET_HXT_PRESENT
    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
#else
    /* Disable HXT clock (external XTAL 12MHz) */
    CLK_DisableXtalRC(CLK_PWRCTL_HXTEN_Msk);
#endif
    /* Enable LIRC */
    CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk);
#if MBED_CONF_TARGET_LXT_PRESENT
    /* Enable LXT */
    CLK_EnableXtalRC(CLK_PWRCTL_LXTEN_Msk);
#else
    /* Disable LXT */
    CLK_DisableXtalRC(CLK_PWRCTL_LXTEN_Msk);
#endif

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
#if MBED_CONF_TARGET_HXT_PRESENT
    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
#endif
    /* Wait for LIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);
#if MBED_CONF_TARGET_LXT_PRESENT
    /* Wait for LXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_LXTSTB_Msk);
#endif

    /* Select HCLK clock source as HIRC and HCLK clock divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    
    /* Set core clock as 192000000 from PLL */
    CLK_SetCoreClock(192000000);
    
    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_PCLK0DIV2 | CLK_PCLKDIV_PCLK1DIV2); // PCLK divider set 2

#if DEVICE_ANALOGIN
    /* Vref connect to internal */
    SYS->VREFCTL = (SYS->VREFCTL & ~SYS_VREFCTL_VREFCTL_Msk) | SYS_VREFCTL_VREF_3_0V;
#endif
    
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();

    /* Get around h/w limit with WDT reset from PD */
    if (SYS_IS_WDT_RST()) {
        /* Re-unlock protected clock setting */
        SYS_UnlockReg();

        /* Set up DPD power down mode */
        CLK->PMUSTS |= CLK_PMUSTS_CLRWK_Msk;
        CLK->PMUSTS |= CLK_PMUSTS_TMRWK_Msk;
        CLK_SetPowerDownMode(CLK_PMUCTL_PDMSEL_DPD);

        CLK_SET_WKTMR_INTERVAL(CLK_PMUCTL_WKTMRIS_256);
        CLK_ENABLE_WKTMR();

        CLK_PowerDown();

        /* Lock protected registers */
        SYS_LockReg();
    }
}

// Override mbed_mac_address of mbed_interface.c to provide ethernet devices with a semi-unique MAC address
void mbed_mac_address(char *mac)
{
    uint32_t uID1;
    // Fetch word 0
    uint32_t word0 = *(uint32_t *)0x7F804; // 2KB Data Flash at 0x7F800
    // Fetch word 1
    // we only want bottom 16 bits of word1 (MAC bits 32-47)
    // and bit 9 forced to 1, bit 8 forced to 0
    // Locally administered MAC, reduced conflicts
    // http://en.wikipedia.org/wiki/MAC_address
    uint32_t word1 = *(uint32_t *)0x7F800; // 2KB Data Flash at 0x7F800

    if (word0 == 0xFFFFFFFF) {       // Not burn any mac address at 1st 2 words of Data Flash
        // with a semi-unique MAC address from the UUID
        /* Enable FMC ISP function */
        SYS_UnlockReg();
        FMC_Open();
        // = FMC_ReadUID(0);
        uID1 = FMC_ReadUID(1);
        word1 = (uID1 & 0x003FFFFF) | ((uID1 & 0x030000) << 6) >> 8;
        word0 = ((FMC_ReadUID(0) >> 4) << 20) | ((uID1 & 0xFF) << 12) | (FMC_ReadUID(2) & 0xFFF);
        /* Disable FMC ISP function */
        FMC_Close();
        /* Lock protected registers */
        SYS_LockReg();
    }

    word1 |= 0x00000200;
    word1 &= 0x0000FEFF;

    mac[0] = (word1 & 0x0000ff00) >> 8;
    mac[1] = (word1 & 0x000000ff);
    mac[2] = (word0 & 0xff000000) >> 24;
    mac[3] = (word0 & 0x00ff0000) >> 16;
    mac[4] = (word0 & 0x0000ff00) >> 8;
    mac[5] = (word0 & 0x000000ff);
}

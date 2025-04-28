/*
 * Copyright (c) 2018 Nuvoton Technology Corp.
 * Copyright (c) 2018 ARM Limited
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
 *
 * Description:   M480 EMAC driver header file
 */

#include "m480_eth_pins.h"

#include <cmsis.h>
#include <m480_sys.h>
#include <m480_clk.h>
#include <system_M480.h>

void nu_eth_clk_and_pin_init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable IP clock */
    CLK_EnableModuleClock(EMAC_MODULE);

    // Configure MDC clock rate to HCLK / (127 + 1) = 1.25 MHz if system is running at 160 MHz
    CLK_SetModuleClock(EMAC_MODULE, 0, CLK_CLKDIV3_EMAC(127));

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    // Configure RMII pins
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA6MFP_Msk | SYS_GPA_MFPL_PA7MFP_Msk);
    SYS->GPA_MFPL |= SYS_GPA_MFPL_PA6MFP_EMAC_RMII_RXERR | SYS_GPA_MFPL_PA7MFP_EMAC_RMII_CRSDV;
    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC6MFP_Msk | SYS_GPC_MFPL_PC7MFP_Msk);
    SYS->GPC_MFPL |= SYS_GPC_MFPL_PC6MFP_EMAC_RMII_RXD1 | SYS_GPC_MFPL_PC7MFP_EMAC_RMII_RXD0;
    SYS->GPC_MFPH &= ~SYS_GPC_MFPH_PC8MFP_Msk;
    SYS->GPC_MFPH |= SYS_GPC_MFPH_PC8MFP_EMAC_RMII_REFCLK;
    SYS->GPE_MFPH &= ~(SYS_GPE_MFPH_PE8MFP_Msk | SYS_GPE_MFPH_PE9MFP_Msk | SYS_GPE_MFPH_PE10MFP_Msk |
                       SYS_GPE_MFPH_PE11MFP_Msk | SYS_GPE_MFPH_PE12MFP_Msk);
    SYS->GPE_MFPH |= SYS_GPE_MFPH_PE8MFP_EMAC_RMII_MDC |
                     SYS_GPE_MFPH_PE9MFP_EMAC_RMII_MDIO |
                     SYS_GPE_MFPH_PE10MFP_EMAC_RMII_TXD0 |
                     SYS_GPE_MFPH_PE11MFP_EMAC_RMII_TXD1 |
                     SYS_GPE_MFPH_PE12MFP_EMAC_RMII_TXEN;

    // Enable high slew rate on all RMII TX output pins
    PE->SLEWCTL = (GPIO_SLEWCTL_HIGH << GPIO_SLEWCTL_HSREN10_Pos) |
                  (GPIO_SLEWCTL_HIGH << GPIO_SLEWCTL_HSREN11_Pos) |
                  (GPIO_SLEWCTL_HIGH << GPIO_SLEWCTL_HSREN12_Pos);


    /* Lock protected registers */
    SYS_LockReg();
}

void nu_eth_clk_and_pin_deinit(void) {
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Disable IP clock */
    CLK_DisableModuleClock(EMAC_MODULE);

    // Reset all functions back to GPIO
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA6MFP_Msk | SYS_GPA_MFPL_PA7MFP_Msk);
    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC6MFP_Msk | SYS_GPC_MFPL_PC7MFP_Msk);
    SYS->GPC_MFPH &= ~SYS_GPC_MFPH_PC8MFP_Msk;
    SYS->GPE_MFPH &= ~(SYS_GPE_MFPH_PE8MFP_Msk | SYS_GPE_MFPH_PE9MFP_Msk | SYS_GPE_MFPH_PE10MFP_Msk |
                       SYS_GPE_MFPH_PE11MFP_Msk | SYS_GPE_MFPH_PE12MFP_Msk);

    /* Lock protected registers */
    SYS_LockReg();
}

PinName nu_eth_get_phy_reset_pin(void) {
    return NC;
}

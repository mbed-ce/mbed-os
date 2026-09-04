/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* RTL8201F PHY target driver for RT-Thread RA6M3 HMI Board.
* Realtek RTL8201F: single-port 10/100 Fast Ethernet transceiver, RMII interface.
* PHY address = 0 (strapped on board), 25MHz crystal on XI pin.
*/

#include "bsp_api.h"
#include "r_ether_phy.h"

#if (ETHER_PHY_CFG_USE_CUSTOM_PHY_LSI_ENABLE)

#define ETHER_PHY_REG_BMCR       (0x00)
#define ETHER_PHY_REG_BMSR       (0x01)
#define ETHER_PHY_REG_ANAR       (0x04)
#define ETHER_PHY_REG_ANLPAR     (0x05)
#define ETHER_PHY_REG_ANER       (0x06)
#define ETHER_PHY_REG_PHYCTRL    (0x16)
#define ETHER_PHY_REG_INTCR      (0x1A)

/* BMCR bits */
#define BMCR_RESET               (0x8000)
#define BMCR_AN_ENABLE           (0x1000)
#define BMCR_AN_RESTART          (0x0200)
#define BMCR_POWER_DOWN          (0x0800)
#define BMCR_ISOLATE             (0x0400)

/* BMSR bits */
#define BMSR_AN_COMPLETE         (0x0020)
#define BMSR_LINK_STATUS         (0x0004)
#define BMSR_AUTONEG_ABILITY     (0x0008)

/* PHYCTRL (reg 0x16) bits for RTL8201F */
#define PHYCTRL_RMII_MODE       (0x0100)

/***********************************************************************************************************************
 * Exported global functions
 **********************************************************************************************************************/
void ether_phy_target_rtl8201f_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
bool ether_phy_target_rtl8201f_is_support_link_partner_ability(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                                uint32_t line_speed_duplex);

/*******************************************************************************************************************//**
 * Initialize RTL8201F PHY for RMII operation.
 **********************************************************************************************************************/
void ether_phy_target_rtl8201f_initialize (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    uint32_t reg;

    /* Soft reset PHY */
    R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_BMCR, BMCR_RESET);

    /* Wait for reset to complete (poll until self-cleared) */
    uint32_t timeout = 100000;
    do
    {
        R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_BMCR, &reg);
        timeout--;
    }
    while((reg & BMCR_RESET) && (timeout > 0));

    /* Configure RMII mode (reg 0x16, bit 8 = RMII) */
    R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_PHYCTRL, &reg);
    reg |= PHYCTRL_RMII_MODE;
    R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_PHYCTRL, reg);

    /* Set advertisement: 10/100, Full/Half, pause, IEEE 802.3 */
    R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_ANAR,
                      (0x01 | 0x0040 | 0x0020 | 0x0010 | 0x0008 | 0x0004 | 0x0002 | 0x0001));
}

/*******************************************************************************************************************//**
 * Check if the link partner supports the given speed/duplex.
 **********************************************************************************************************************/
bool ether_phy_target_rtl8201f_is_support_link_partner_ability (ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                                 uint32_t line_speed_duplex)
{
    uint32_t lpar = 0;
    bool ret = false;

    /* Read link partner ability */
    R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_ANLPAR, &lpar);

    switch(line_speed_duplex)
    {
        case 0x00000006: /* 100M full */
            ret = (lpar & 0x0100) != 0; /* bit 8: 100FDX */
            break;
        case 0x00000004: /* 100M half */
            ret = (lpar & 0x0080) != 0; /* bit 7: 100HDX */
            break;
        case 0x00000002: /* 10M full */
            ret = (lpar & 0x0040) != 0; /* bit 6: 10FDX */
            break;
        case 0x00000000: /* 10M half */
            ret = (lpar & 0x0020) != 0; /* bit 5: 10HDX */
            break;
        default:
            break;
    }

    return ret;
}

#endif /* ETHER_PHY_CFG_USE_CUSTOM_PHY_LSI_ENABLE */

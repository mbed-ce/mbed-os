/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_phy.h"
#include "dp83825_regs.h"

#include <stdio.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the timeout macro. */
#define PHY_TIMEOUT_COUNT 100000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the ENET instance from peripheral base address.
 *
 * @param base ENET peripheral base address.
 * @return ENET instance.
 */
extern uint32_t ENET_GetInstance(ENET_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to enet clocks for each instance. */
extern clock_ip_name_t s_enetClock[FSL_FEATURE_SOC_ENET_COUNT];
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t PHY_Init(ENET_Type *base, uint32_t phyAddr, uint32_t srcClock_Hz)
{
    uint32_t counter = PHY_TIMEOUT_COUNT;
    uint32_t idReg = 0;
    status_t result = kStatus_Success;
    uint32_t instance = ENET_GetInstance(base);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Set SMI first. */
    CLOCK_EnableClock(s_enetClock[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
    ENET_SetSMI(base, srcClock_Hz, false);

    /* Initialization after PHY stars to work. */
    while ((idReg != PHY_CONTROL_ID1) && (counter != 0))
    {
        PHY_Read(base, phyAddr, PHY_ID1_REG, &idReg);
        printf("PHY_ID1_REG = %u\n", idReg);
        counter--;
    }

    if (!counter)
    {
        return kStatus_Fail;
    }

    // Set up LED control for the teensy hardware.
    // Reference here: https://github.com/ssilverman/QNEthernet/blob/master/src/lwip_t41.c#L265
    PHY_Write(base, phyAddr, PHY_LEDCR_REG, PHY_LEDCR_POLARITY_Msk | // LED to active high
            (1 << PHY_LEDCR_BLINK_RATE_Pos)); // Blink period to 10Hz

    // Set up clock select register
    PHY_Write(base, phyAddr, PHY_RCSR_REG, PHY_RCSR_RMII_CLK_SEL_Msk | // Select 50MHz RMII clock
            (1 << PHY_RCSR_RX_ELASTICITY_Pos)); // Rx elasticity to 2 bits

    return result;
}

status_t PHY_AutoNegotiation(ENET_Type *base, uint32_t phyAddr)
{
    status_t result = kStatus_Success;
    uint32_t bssReg;
    uint32_t counter = PHY_TIMEOUT_COUNT;

    /* Set the negotiation. */
    result = PHY_Write(base, phyAddr, PHY_AUTONEG_ADVERTISE_REG,
                       (PHY_100BASETX_FULLDUPLEX_MASK | PHY_100BASETX_HALFDUPLEX_MASK |
                        PHY_10BASETX_FULLDUPLEX_MASK | PHY_10BASETX_HALFDUPLEX_MASK | 0x1U));
    if (result == kStatus_Success)
    {
        result = PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG,
                           (PHY_BCTL_AUTONEG_MASK | PHY_BCTL_RESTART_AUTONEG_MASK));
        if (result == kStatus_Success)
        {
            /* Check auto negotiation complete. */
            while (counter--)
            {
                result = PHY_Read(base, phyAddr, PHY_BASICSTATUS_REG, &bssReg);
                if (result == kStatus_Success)
                {
                    if (((bssReg & PHY_BSTATUS_AUTONEGCOMP_MASK) != 0))
                    {
                        /* Wait a moment for Phy status stable. */
                        return kStatus_Success;
                    }
                }

                if (!counter)
                {
                    return kStatus_PHY_AutoNegotiateFail;
                }
            }
        }
    }

    return result;
}

status_t PHY_Write(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t data)
{
    uint32_t counter;

    /* Clear the SMI interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI write command. */
    ENET_StartSMIWrite(base, phyAddr, phyReg, kENET_MiiWriteValidFrame, data);

    /* Wait for SMI complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_Read(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t *dataPtr)
{
    assert(dataPtr);

    uint32_t counter;

    /* Clear the MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI read command operation. */
    ENET_StartSMIRead(base, phyAddr, phyReg, kENET_MiiReadValidFrame);

    /* Wait for MII complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Get data from MII register. */
    *dataPtr = ENET_ReadSMIData(base);

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_EnableLoopback(ENET_Type *base, uint32_t phyAddr, phy_loop_t mode, phy_speed_t speed, bool enable)
{
    uint32_t data = 0;

    /* Set the loop mode. */
    if (enable)
    {
        if (mode == kPHY_LocalLoop)
        {
            if (speed == kPHY_Speed100M)
            {
                data = PHY_BCTL_SPEED_100M_MASK | PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
            else
            {
                data = PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
            PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, data);

            // Per datasheet, also need to set BISCR
            uint32_t biscrVal;
            PHY_Read(base, phyAddr, PHY_BISCR_REG, &biscrVal);
            biscrVal &= ~PHY_BISCR_LOOPBACK_Msk;
            biscrVal |= (speed == kPHY_Speed100M ? PHY_BISCR_LOOPBACK_DIGITAL_LOOPBACK_100M_Val : PHY_BISCR_LOOPBACK_DIGITAL_LOOPBACK_10M_Val);
            PHY_Write(base, phyAddr, PHY_BISCR_REG, biscrVal);
        }
        else
        {
            // Set requested speed manually
            if (speed == kPHY_Speed100M)
            {
                data = PHY_BCTL_SPEED_100M_MASK | PHY_BCTL_DUPLEX_MASK;
            }
            else
            {
                data = PHY_BCTL_DUPLEX_MASK;
            }
            PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, data);

            // Enable analog loopback in BISCR
            uint32_t biscrVal;
            PHY_Read(base, phyAddr, PHY_BISCR_REG, &biscrVal);
            biscrVal &= ~PHY_BISCR_LOOPBACK_Msk;
            biscrVal |= PHY_BISCR_LOOPBACK_ANALOG_LOOPBACK_Val;
            PHY_Write(base, phyAddr, PHY_BISCR_REG, biscrVal);
        }
    }
    else
    {
        /* Disable the loop mode. */
        if (mode == kPHY_LocalLoop)
        {
            // Reenable autonegotiation
            PHY_Read(base, phyAddr, PHY_BASICCONTROL_REG, &data);
            data |= PHY_BCTL_RESTART_AUTONEG_MASK | PHY_BCTL_AUTONEG_MASK;
            PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, (data | PHY_BCTL_RESTART_AUTONEG_MASK));

            // Also, disable loopback in BISCR
            uint32_t biscrVal;
            PHY_Read(base, phyAddr, PHY_BISCR_REG, &biscrVal);
            biscrVal &= ~PHY_BISCR_LOOPBACK_Msk;
            PHY_Write(base, phyAddr, PHY_BISCR_REG, biscrVal);
        }
        else
        {
            // Reenable autonegotiation
            PHY_Read(base, phyAddr, PHY_BASICCONTROL_REG, &data);
            data |= PHY_BCTL_RESTART_AUTONEG_MASK | PHY_BCTL_AUTONEG_MASK;
            PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, (data | PHY_BCTL_RESTART_AUTONEG_MASK));

            // Disable loopback in BISCR
            uint32_t biscrVal;
            PHY_Read(base, phyAddr, PHY_BISCR_REG, &biscrVal);
            biscrVal &= ~PHY_BISCR_LOOPBACK_Msk;
            PHY_Write(base, phyAddr, PHY_BISCR_REG, biscrVal);
        }
    }
    return kStatus_Success;
}

status_t PHY_GetLinkStatus(ENET_Type *base, uint32_t phyAddr, bool *status)
{
    assert(status);

    status_t result = kStatus_Success;
    uint32_t data;

    /* Read the basic status register. */
    result = PHY_Read(base, phyAddr, PHY_BASICSTATUS_REG, &data);
    if (result == kStatus_Success)
    {
        if (!(PHY_BSTATUS_LINKSTATUS_MASK & data))
        {
            /* link down. */
            *status = false;
        }
        else
        {
            /* link up. */
            *status = true;
        }
    }
    return result;
}

status_t PHY_GetLinkSpeedDuplex(ENET_Type *base, uint32_t phyAddr, phy_speed_t *speed, phy_duplex_t *duplex)
{
    assert(duplex);

    status_t result = kStatus_Success;
    uint32_t stsReg;

    /* Read status register. */
    result = PHY_Read(base, phyAddr, PHY_PHYSTS_REG, &stsReg);
    if (result == kStatus_Success)
    {
        if(stsReg & PHY_PHYSTS_DUPLEX_Msk)
        {
            *duplex = kPHY_FullDuplex;
        }
        else
        {
            *duplex = kPHY_HalfDuplex;
        }

        if(stsReg & PHY_PHYSTS_SPEED_Msk)
        {
            *speed = kPHY_Speed10M;
        }
        else
        {
            *speed = kPHY_Speed100M;
        }
    }

    return result;
}

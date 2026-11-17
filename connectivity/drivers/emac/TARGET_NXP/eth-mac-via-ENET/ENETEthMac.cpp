/*
 * Mbed OS zero-copy driver for the ENET peripheral
 * used on NXP Kinetis and iMXRT MCUs.
 *
 * Copyright (c) 2026 Jamie Smith
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ENETEthMac.h"

#include <cmath>

#include "mbed_math_helpers.h"

extern "C" void kinetis_init_eth_hardware(); // Defined in hardware_init.c for your target

namespace mbed
{

// Address & reset definitions
// Copied from MCUX SDK.

/*! @brief Pointers to enet bases for each instance. */
static ENET_Type *const s_enetBases[] = ENET_BASE_PTRS;

const clock_ip_name_t s_enetClock[] = ENET_CLOCKS;
#if defined(FSL_FEATURE_ENET_HAS_EXTRA_CLOCK_GATE) && FSL_FEATURE_ENET_HAS_EXTRA_CLOCK_GATE
const clock_ip_name_t s_enetExtraClock[] = ENET_EXTRA_CLOCKS;
#endif

#if defined(FSL_FEATURE_ENET_HAS_RSTCTL) && (FSL_FEATURE_ENET_HAS_RSTCTL > 0)
static const reset_ip_name_t s_enetResets[] = ENET_RESETS_ARRAY;
#endif

/*! @brief Pointers to enet transmit IRQ number for each instance. */
static const IRQn_Type s_enetTxIrqId[] = ENET_Transmit_IRQS;
/*! @brief Pointers to enet receive IRQ number for each instance. */
static const IRQn_Type s_enetRxIrqId[] = ENET_Receive_IRQS;
/*! @brief Pointers to enet error IRQ number for each instance. */
static const IRQn_Type s_enetErrIrqId[] = ENET_Error_IRQS;

ENETEthMac * ENETEthMac::instances[FSL_FEATURE_SOC_ENET_COUNT + 1]{};

// IRQ handlers for each Ethernet
void ENET0_IRQHandler() {
    ENETEthMac::instances[0]->enetIrqHandler();
}

#ifdef ENET2
void ENET2_IRQHandler() {
    ENETEthMac::instances[2]->enetIrqHandler();
}
#endif

static auto * const ENETIRQHandlers[FSL_FEATURE_SOC_ENET_COUNT + 1] = {
    &ENET0_IRQHandler,
#ifdef ENET2
    nullptr,
    &ENET2_IRQHandler
#endif
};

const uint32_t ALL_USED_INTERRUPTS =
    ENET_EIR_TXB_MASK | // Buffer transmitted
    ENET_EIR_RXF_MASK | // Frame received
    ENET_EIR_EBERR_MASK | // Bus error when DMA tried to access data (fatal)
    ENET_EIR_BABR_MASK | // Babbling receive error
    ENET_EIR_PLR_MASK;// Receive length error


void ENETEthMac::MACDriver::setupMDIOClocking(uint32_t sourceClock) {
    // Calculate clock divider. 2*this divider must divide the clock down to 2.5MHz or less (but closer is better).
    // That is the same as saying this divider must divide the clock down to 5MHz or less.
    const uint8_t mdioClkDivider = mbed_integer_div_round_up<uint32_t>(sourceClock, (2500000 * 2));
    MBED_ASSERT(mdioClkDivider > 0 && mdioClkDivider < 64); // 6-bit field -> max value 63

    // Calculate hold time. Need to determine the number of clock cycles needed to obtain
    // 10ns or greater hold time.
    const float nsPerSourceClock = 1e9f / static_cast<float>(sourceClock);
    const uint8_t holdTimeSourceClkCycles = std::ceil(10 / nsPerSourceClock);

    // Register fields are off by 1 so subtract 1
    base->MSCR = ENET_MSCR_MII_SPEED_MASK(mdioClkDivider - 1) | ENET_MSCR_HOLDTIME_MASK(holdTimeSourceClkCycles - 1);

}

ENETEthMac::MACDriver::MACDriver(size_t enetNumber):
enetNumber(enetNumber),
base(s_enetBases[enetNumber])
{
}

CompositeEMAC::ErrCode ENETEthMac::MACDriver::init() {
    CLOCK_EnableClock(s_enetClock[enetNumber]);
#if defined(FSL_FEATURE_ENET_HAS_EXTRA_CLOCK_GATE) && FSL_FEATURE_ENET_HAS_EXTRA_CLOCK_GATE
    /* Ungate ENET extra clock. */
    (void)CLOCK_EnableClock(s_enetExtraClock[instance]);
#endif

    // Mux pins
    kinetis_init_eth_hardware();

    // Configure ECR
    base->ECR |=
        ENET_ECR_DBSWP_MASK | // Descriptor byte-swap for little-endian devices
        ENET_ECR_EN1588_MASK; // Use enhanced buffer descriptors with option for PTP timestamp

    // Configure RCR
    base->RCR =
        ENET_RCR_MII_MODE_MASK | // Should always be set
        ENET_RCR_RMII_MODE_MASK | // Use RMII mode always (for now)
        ENET_RCR_PADEN_MASK | // Remove padding on Rx
        ENET_RCR_MAX_FL(1518); // Max frame length (including CRC)

    // Configure max frame limit again to ensure frames over the limit are truncated in addition
    // to being considered an error.
    base->FTRL = 1518;

    // Configure TCR
    base->TCR = 0; // No options needed for now

    // Configure accelerators. Note that Mbed currently doesn't generally support checksum offloading at this time.
    base->TACC = 0;
    base->RACC =
        ENET_RACC_PADREM_MASK | // Remove padding from short IP frames
        ENET_RACC_IPDIS_MASK | // Discard frames with an invalid IP checksum
        ENET_RACC_PRODIS_MASK | // Discard frames with an invalid protocol checksums
        ENET_RACC_LINEDIS_MASK; // Discard frames with Rx errors

    // FIFO thresholds for Rx cut-through operation.
    // The datasheet doesn't give a lot of info about the best values for these so making some guesses

    // Receive Almost-Empty threshold. Rx DMA pauses when only this many 64-bit words are left in the FIFO from the
    // MAC. One part of the datasheet says set to at least 6 but reg description says at least 4.
    base->RAEM = 6; // = 24 bytes

    // Rx Section FIFO Level. DMA begins transferring bytes from the FIFO when there are at least this many 64-bit words
    // (or end-of-frame) in the FIFO. Must be greater than RAEM.
    base->RSFL = 16; // = 128 bytes

    // For Tx use store and forward, for now.
    // IMO, using cut-through on Tx is riskier because if there is a lot of contention on the MCU's
    // internal bus, the Eth DMA might not be able to grab the entire packet in time, which would likely
    // result in a garbled transmission.
    // Much better to have it wait until it has transferred the entire packet into the Tx FIFO
    // before beginning transmission.
    base->TFWR = ENET_TFWR_STRFWD_MASK;

    // Set up the MDIO clocking
    const uint32_t mdioSourceClock = CLOCK_GetFreq(kCLOCK_CoreSysClk); // AKA internal module clock / peripheral bus clock / IPG clock (classic NXP)
    setupMDIOClocking(mdioSourceClock);

    // For now do not enable interrupt coalescing. In the future, we could turn this on to
    // prevent the CPU core from being spammed with interrupts from every Rxed/Txed frame and instead
    // only trigger the interrupt after a certain number of frames and/or timeout.

    // Enable interrupts (and clear if pending)
    base->EIR = ALL_USED_INTERRUPTS;
    base->EIMR = ALL_USED_INTERRUPTS;

    // Enable IRQs.
    // Note that on Kinetis these are different IRQs but on MIMXRT they have been combined
    // into one. So, we just point all of them to the same handler.
    for (auto irq : {s_enetTxIrqId[enetNumber], s_enetRxIrqId[enetNumber], s_enetErrIrqId[enetNumber]}) {
        NVIC_SetVector(irq, reinterpret_cast<uint32_t>(ENETIRQHandlers[enetNumber]));
        NVIC_SetPriority(irq, ((0x01 << 3) | 0x01));
        NVIC_ClearPendingIRQ(irq); // in case pending from previous time the MAC was used
        NVIC_EnableIRQ(irq);
    }

    return ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode ENETEthMac::MACDriver::deinit() {
    // Put the ENET under reset, clearing all settings.
    base->ECR |= ENET_ECR_RESET_MASK;

    return ErrCode::SUCCESS;
}

CompositeEMAC::ErrCode ENETEthMac::MACDriver::enable(LinkSpeed speed, Duplex duplex) {

    // Set speed
    if (speed == LinkSpeed::LINK_10MBIT) {
        base->RCR |= ENET_RCR_RMII_10T_MASK;
    }
    else {
        base->RCR &= ~ENET_RCR_RMII_10T_MASK;
    }

    if (base->ECR & ENET_ECR_ETHEREN_MASK) {
        // Not the first initialization, not allowed to change duplex settings. Just bring out of sleep.
        // The quirks configuration will ensure that if the duplex is changed the MAC will be re-initialized.
        base->ECR &= ~ENET_ECR_SLEEP_MASK;
    }
    else {
        // First initialization, configure the duplex and enable the Ethernet peripheral
        if (duplex == Duplex::HALF) {
            base->RCR |= ENET_RCR_DRT_MASK;
            base->TCR &= ~ENET_TCR_FDEN_MASK;
        }
        else {
            base->RCR &= ~ENET_RCR_DRT_MASK;
            base->TCR |= ENET_TCR_FDEN_MASK;
        }

        base->ECR = (base->ECR & ~ENET_ECR_SLEEP_MASK) | ENET_ECR_ETHEREN_MASK;
    }

    return ErrCode::SUCCESS;
}

}

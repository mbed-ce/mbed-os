/*
 * Mbed OS zero-copy driver for the ENET peripheral
 * used on NXP Kinetis and iMXRT MCUs.
 *
 * Copyright (c) 2026 Jamie Smith
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "NXP_ENET_DMADescriptors.h"
#include "GenericEthDMA.h"
#include "CompositeEMAC.h"

namespace mbed {

class ENETEthMac : public CompositeEMAC {

    static constexpr Quirks quirks = {
        .txDMA = {
            .extraTxDescsToLeave = 0,
            .supportsDescChaining = true,
            .manuallyAppendCRC = false,
            .manuallyPadShortFrames = false
        },
        .rxDMA = {
            .supportsFirstDescFlag = false,
            .canReturnAllDescriptors = true,
            .packetEndDiscardSize = sizeof(uint32_t),
            .rxBufferSizeMultiple = 64, // Per the datasheet Rx buffer sizes should be a multiple of 64 for best performance
        },
        .duplexChangeRequiresReinit = true
    };

    class MACDriver : public CompositeEMAC::MACDriver
    {
        size_t const enetNumber;
        ENET_Type * const base;

        // When passingAllMcasts = false,
        bool passingAllMcasts = false;
        uint32_t mcastMacHashTable[2];

        /// Set up timings for MDIO clock generation
        void setupMDIOClocking(uint32_t sourceClock);

        /// Computes the 6-bit CRC of a MAC address, used for the unicast and multicast
        /// hash filter. Adapted from ENET_AddMulticastGroup().
        static uint8_t getMacAddrCRC(MACAddress const & address);

        /// Write the mcastMacHashTable value to memory.
        void writeMcastHashTable();

    public:
        explicit MACDriver(size_t enetNumber);

        ErrCode init() override;

        ErrCode deinit() override;

        ErrCode enable(LinkSpeed speed, Duplex duplex) override;

        ErrCode disable() override;

        void setOwnMACAddr(const MACAddress &ownAddress) override;

        ErrCode mdioRead(uint8_t devAddr, uint8_t regAddr, uint16_t &result) override;

        ErrCode mdioWrite(uint8_t devAddr, uint8_t regAddr, uint16_t data) override;

        PinName getPhyResetPin() override;

        ErrCode addMcastMAC(MACAddress mac) override;

        ErrCode clearMcastFilter() override;

        void setPassAllMcast(bool pass) override;

        void setPromiscuous(bool enable) override;
    };

    class TxDMA : public GenericTxDMARing
    {
    protected:
        ENET_Type * const base; // Base address of Ethernet peripheral

        // Tx descriptor array. Per the datasheet, for optimal performance this should be 64-byte aligned.
        alignas(64) NXP_ENET::EthTxDescriptor txDescs[TX_NUM_DESCS];

        void startDMA() override;

        void stopDMA() override;

        bool descOwnedByDMA(size_t descIdx) override;

        bool isDMAReadableBuffer(uint8_t const * start, size_t size) const override;

        void giveToDMA(size_t descIdx, uint8_t const * buffer, size_t len, bool firstDesc, bool lastDesc) override;
    public:
        explicit TxDMA(ENET_Type * const base):
        GenericTxDMARing(quirks),
        base(base)
        {}
    };

    class RxDMA : public GenericRxDMARing {
    protected:
        ENET_Type * const base; // Base address of Ethernet peripheral

        void startDMA() override;

        void stopDMA() override;

        bool descOwnedByDMA(size_t descIdx) override;

        bool isFirstDesc(size_t descIdx) override;

        bool isLastDesc(size_t descIdx) override;

        bool isErrorDesc(size_t descIdx) override;

        void returnDescriptor(size_t descIdx, uint8_t * buffer) override;

        size_t getTotalLen(size_t firstDescIdx, size_t lastDescIdx) override;
    public:

        explicit RxDMA(ENET_Type * const base):
        GenericRxDMARing(quirks),
        base(base)
        {}
    };

    ENET_Type * const base; // Base address of Ethernet peripheral

    // Components of the ethernet MAC
    TxDMA txDMA;
    RxDMA rxDMA;
    MACDriver macDriver;

    // Instance pointers for the interrupts.
    // Note that on MIMXRT106x the ethernets are numbered 0 and 2 so we need an extra entry here.
    static ENETEthMac * instances[FSL_FEATURE_SOC_ENET_COUNT + 1];

    friend void ENET0_IRQHandler();

public:
    explicit ENETEthMac(size_t instanceNumber);

    ~ENETEthMac();

    // Interrupt callbacks
    void enetIrqHandler();
};

}
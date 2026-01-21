/**********************************************************************
* $Id$        lpc17xx_emac.h                2010-05-21
*//**
* @file        lpc17xx_emac.h
* @brief    Contains all macro definitions and function prototypes
*             support for Ethernet MAC firmware library on LPC17xx
* @version    2.0
* @date        21. May. 2010
* @author    NXP MCU SW Application Team
*
* Copyright(C) 2010, NXP Semiconductor
* All rights reserved.
* SPDX-License-Identifier: Apache-2.0
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#pragma once

/* EMAC Memory Buffer configuration for 16K Ethernet RAM */
#define EMAC_NUM_RX_FRAG         4           /**< Num.of RX Fragments 4*1536= 6.0kB */
#define EMAC_NUM_TX_FRAG         3           /**< Num.of TX Fragments 3*1536= 4.6kB */
#define EMAC_ETH_MAX_FLEN        1536        /**< Max. Ethernet Frame Size          */
#define EMAC_TX_FRAME_TOUT       0x00100000  /**< Frame Transmit timeout count      */

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for MAC Configuration Register 1
 **********************************************************************/
#define EMAC_MAC1_REC_EN         0x00000001  /**< Receive Enable                    */
#define EMAC_MAC1_PASS_ALL       0x00000002  /**< Pass All Receive Frames           */
#define EMAC_MAC1_RX_FLOWC       0x00000004  /**< RX Flow Control                   */
#define EMAC_MAC1_TX_FLOWC       0x00000008  /**< TX Flow Control                   */
#define EMAC_MAC1_LOOPB          0x00000010  /**< Loop Back Mode                    */
#define EMAC_MAC1_RES_TX         0x00000100  /**< Reset TX Logic                    */
#define EMAC_MAC1_RES_MCS_TX     0x00000200  /**< Reset MAC TX Control Sublayer     */
#define EMAC_MAC1_RES_RX         0x00000400  /**< Reset RX Logic                    */
#define EMAC_MAC1_RES_MCS_RX     0x00000800  /**< Reset MAC RX Control Sublayer     */
#define EMAC_MAC1_SIM_RES        0x00004000  /**< Simulation Reset                  */
#define EMAC_MAC1_SOFT_RES       0x00008000  /**< Soft Reset MAC                    */

/*********************************************************************//**
 * Macro defines for MAC Configuration Register 2
 **********************************************************************/
#define EMAC_MAC2_FULL_DUP       0x00000001  /**< Full-Duplex Mode                  */
#define EMAC_MAC2_FRM_LEN_CHK    0x00000002  /**< Frame Length Checking             */
#define EMAC_MAC2_HUGE_FRM_EN    0x00000004  /**< Huge Frame Enable                 */
#define EMAC_MAC2_DLY_CRC        0x00000008  /**< Delayed CRC Mode                  */
#define EMAC_MAC2_CRC_EN         0x00000010  /**< Append CRC to every Frame         */
#define EMAC_MAC2_PAD_EN         0x00000020  /**< Pad all Short Frames              */
#define EMAC_MAC2_VLAN_PAD_EN    0x00000040  /**< VLAN Pad Enable                   */
#define EMAC_MAC2_ADET_PAD_EN    0x00000080  /**< Auto Detect Pad Enable            */
#define EMAC_MAC2_PPREAM_ENF     0x00000100  /**< Pure Preamble Enforcement         */
#define EMAC_MAC2_LPREAM_ENF     0x00000200  /**< Long Preamble Enforcement         */
#define EMAC_MAC2_NO_BACKOFF     0x00001000  /**< No Backoff Algorithm              */
#define EMAC_MAC2_BACK_PRESSURE  0x00002000  /**< Backoff Presurre / No Backoff     */
#define EMAC_MAC2_EXCESS_DEF     0x00004000  /**< Excess Defer                      */

/*********************************************************************//**
 * Macro defines for Back-to-Back Inter-Packet-Gap Register
 **********************************************************************/
/** Programmable field representing the nibble time offset of the minimum possible period
 * between the end of any transmitted packet to the beginning of the next */
#define EMAC_IPGT_BBIPG(n)        (n&0x7F)
/** Recommended value for Full Duplex of Programmable field representing the nibble time
 * offset of the minimum possible period between the end of any transmitted packet to the
 * beginning of the next */
#define EMAC_IPGT_FULL_DUP        (EMAC_IPGT_BBIPG(0x15))
/** Recommended value for Half Duplex of Programmable field representing the nibble time
 * offset of the minimum possible period between the end of any transmitted packet to the
 * beginning of the next */
#define EMAC_IPGT_HALF_DUP      (EMAC_IPGT_BBIPG(0x12))

/*********************************************************************//**
 * Macro defines for Non Back-to-Back Inter-Packet-Gap Register
 **********************************************************************/
/** Programmable field representing the Non-Back-to-Back Inter-Packet-Gap */
#define EMAC_IPGR_NBBIPG_P2(n)    (n&0x7F)
/** Recommended value for Programmable field representing the Non-Back-to-Back Inter-Packet-Gap Part 1 */
#define EMAC_IPGR_P2_DEF        (EMAC_IPGR_NBBIPG_P2(0x12))
/** Programmable field representing the optional carrierSense window referenced in
 * IEEE 802.3/4.2.3.2.1 'Carrier Deference' */
#define EMAC_IPGR_NBBIPG_P1(n)    ((n&0x7F)<<8)
/** Recommended value for Programmable field representing the Non-Back-to-Back Inter-Packet-Gap Part 2 */
#define EMAC_IPGR_P1_DEF        EMAC_IPGR_NBBIPG_P1(0x0C)

/*********************************************************************//**
 * Macro defines for Collision Window/Retry Register
 **********************************************************************/
/** Programmable field specifying the number of retransmission attempts following a collision before
 * aborting the packet due to excessive collisions */
#define EMAC_CLRT_MAX_RETX(n)    (n&0x0F)
/** Programmable field representing the slot time or collision window during which collisions occur
 * in properly configured networks */
#define EMAC_CLRT_COLL(n)        ((n&0x3F)<<8)
/** Default value for Collision Window / Retry register */
#define EMAC_CLRT_DEF           ((EMAC_CLRT_MAX_RETX(0x0F))|(EMAC_CLRT_COLL(0x37)))

/*********************************************************************//**
 * Macro defines for Maximum Frame Register
 **********************************************************************/
/** Represents a maximum receive frame of 1536 octets */
#define EMAC_MAXF_MAXFRMLEN(n)    (n&0xFFFF)

/*********************************************************************//**
 * Macro defines for PHY Support Register
 **********************************************************************/
#define EMAC_SUPP_SPEED            0x00000100      /**< Reduced MII Logic Current Speed   */
#define EMAC_SUPP_RES_RMII      0x00000800      /**< Reset Reduced MII Logic           */

/*********************************************************************//**
 * Macro defines for Test Register
 **********************************************************************/
#define EMAC_TEST_SHCUT_PQUANTA  0x00000001      /**< Shortcut Pause Quanta             */
#define EMAC_TEST_TST_PAUSE      0x00000002      /**< Test Pause                        */
#define EMAC_TEST_TST_BACKP      0x00000004      /**< Test Back Pressure                */

/*********************************************************************//**
 * Macro defines for MII Management Configuration Register
 **********************************************************************/
#define EMAC_MCFG_SCAN_INC       0x00000001      /**< Scan Increment PHY Address        */
#define EMAC_MCFG_SUPP_PREAM     0x00000002      /**< Suppress Preamble                 */
#define EMAC_MCFG_CLK_SEL(n)     ((n&0x0F)<<2)  /**< Clock Select Field                 */
#define EMAC_MCFG_RES_MII        0x00008000      /**< Reset MII Management Hardware     */
#define EMAC_MCFG_MII_MAXCLK     2500000UL        /**< MII Clock max */

/*********************************************************************//**
 * Macro defines for MII Management Command Register
 **********************************************************************/
#define EMAC_MCMD_READ           0x00000001      /**< MII Read                          */
#define EMAC_MCMD_SCAN           0x00000002      /**< MII Scan continuously             */

#define EMAC_MII_WR_TOUT         0x00050000      /**< MII Write timeout count           */
#define EMAC_MII_RD_TOUT         0x00050000      /**< MII Read timeout count            */

/*********************************************************************//**
 * Macro defines for MII Management Address Register
 **********************************************************************/
#define EMAC_MADR_REG_ADR(n)     (n&0x1F)          /**< MII Register Address field         */
#define EMAC_MADR_PHY_ADR(n)     ((n&0x1F)<<8)  /**< PHY Address Field                  */

/*********************************************************************//**
 * Macro defines for MII Management Write Data Register
 **********************************************************************/
#define EMAC_MWTD_DATA(n)        (n&0xFFFF)        /**< Data field for MMI Management Write Data register */

/*********************************************************************//**
 * Macro defines for MII Management Read Data Register
 **********************************************************************/
#define EMAC_MRDD_DATA(n)        (n&0xFFFF)        /**< Data field for MMI Management Read Data register */

/*********************************************************************//**
 * Macro defines for MII Management Indicators Register
 **********************************************************************/
#define EMAC_MIND_BUSY           0x00000001      /**< MII is Busy                       */
#define EMAC_MIND_SCAN           0x00000002      /**< MII Scanning in Progress          */
#define EMAC_MIND_NOT_VAL        0x00000004      /**< MII Read Data not valid           */
#define EMAC_MIND_MII_LINK_FAIL  0x00000008      /**< MII Link Failed                   */

/* Station Address 0 Register */
/* Station Address 1 Register */
/* Station Address 2 Register */


/* Control register definitions --------------------------------------------------------------------------- */
/*********************************************************************//**
 * Macro defines for Command Register
 **********************************************************************/
#define EMAC_Command_RX_EN            0x00000001      /**< Enable Receive                    */
#define EMAC_Command_TX_EN            0x00000002      /**< Enable Transmit                   */
#define EMAC_Command_REG_RES          0x00000008      /**< Reset Host Registers              */
#define EMAC_Command_TX_RES           0x00000010      /**< Reset Transmit Datapath           */
#define EMAC_Command_RX_RES           0x00000020      /**< Reset Receive Datapath            */
#define EMAC_Command_PASS_RUNT_FRM    0x00000040      /**< Pass Runt Frames                  */
#define EMAC_Command_PASS_RX_FILT     0x00000080      /**< Pass RX Filter                    */
#define EMAC_Command_TX_FLOW_CTRL     0x00000100      /**< TX Flow Control                   */
#define EMAC_Command_RMII             0x00000200      /**< Reduced MII Interface             */
#define EMAC_Command_FULL_DUP         0x00000400      /**< Full Duplex                       */

/*********************************************************************//**
 * Macro defines for Status Register
 **********************************************************************/
#define EMAC_SR_RX_EN            0x00000001      /**< Enable Receive                    */
#define EMAC_SR_TX_EN            0x00000002      /**< Enable Transmit                   */

/*********************************************************************//**
 * Macro defines for Transmit Status Vector 0 Register
 **********************************************************************/
#define EMAC_TSV0_CRC_ERR        0x00000001  /**< CRC error                         */
#define EMAC_TSV0_LEN_CHKERR     0x00000002  /**< Length Check Error                */
#define EMAC_TSV0_LEN_OUTRNG     0x00000004  /**< Length Out of Range               */
#define EMAC_TSV0_DONE           0x00000008  /**< Tramsmission Completed            */
#define EMAC_TSV0_MCAST          0x00000010  /**< Multicast Destination             */
#define EMAC_TSV0_BCAST          0x00000020  /**< Broadcast Destination             */
#define EMAC_TSV0_PKT_DEFER      0x00000040  /**< Packet Deferred                   */
#define EMAC_TSV0_EXC_DEFER      0x00000080  /**< Excessive Packet Deferral         */
#define EMAC_TSV0_EXC_COLL       0x00000100  /**< Excessive Collision               */
#define EMAC_TSV0_LATE_COLL      0x00000200  /**< Late Collision Occured            */
#define EMAC_TSV0_GIANT          0x00000400  /**< Giant Frame                       */
#define EMAC_TSV0_UNDERRUN       0x00000800  /**< Buffer Underrun                   */
#define EMAC_TSV0_BYTES          0x0FFFF000  /**< Total Bytes Transferred           */
#define EMAC_TSV0_CTRL_FRAME     0x10000000  /**< Control Frame                     */
#define EMAC_TSV0_PAUSE          0x20000000  /**< Pause Frame                       */
#define EMAC_TSV0_BACK_PRESS     0x40000000  /**< Backpressure Method Applied       */
#define EMAC_TSV0_VLAN           0x80000000  /**< VLAN Frame                        */

/*********************************************************************//**
 * Macro defines for Transmit Status Vector 1 Register
 **********************************************************************/
#define EMAC_TSV1_BYTE_CNT       0x0000FFFF  /**< Transmit Byte Count               */
#define EMAC_TSV1_COLL_CNT       0x000F0000  /**< Transmit Collision Count          */

/*********************************************************************//**
 * Macro defines for Receive Status Vector Register
 **********************************************************************/
#define EMAC_RSV_BYTE_CNT        0x0000FFFF  /**< Receive Byte Count                */
#define EMAC_RSV_PKT_IGNORED     0x00010000  /**< Packet Previously Ignored         */
#define EMAC_RSV_RXDV_SEEN       0x00020000  /**< RXDV Event Previously Seen        */
#define EMAC_RSV_CARR_SEEN       0x00040000  /**< Carrier Event Previously Seen     */
#define EMAC_RSV_REC_CODEV       0x00080000  /**< Receive Code Violation            */
#define EMAC_RSV_CRC_ERR         0x00100000  /**< CRC Error                         */
#define EMAC_RSV_LEN_CHKERR      0x00200000  /**< Length Check Error                */
#define EMAC_RSV_LEN_OUTRNG      0x00400000  /**< Length Out of Range               */
#define EMAC_RSV_REC_OK          0x00800000  /**< Frame Received OK                 */
#define EMAC_RSV_MCAST           0x01000000  /**< Multicast Frame                   */
#define EMAC_RSV_BCAST           0x02000000  /**< Broadcast Frame                   */
#define EMAC_RSV_DRIB_NIBB       0x04000000  /**< Dribble Nibble                    */
#define EMAC_RSV_CTRL_FRAME      0x08000000  /**< Control Frame                     */
#define EMAC_RSV_PAUSE           0x10000000  /**< Pause Frame                       */
#define EMAC_RSV_UNSUPP_OPC      0x20000000  /**< Unsupported Opcode                */
#define EMAC_RSV_VLAN            0x40000000  /**< VLAN Frame                        */

/*********************************************************************//**
 * Macro defines for Flow Control Counter Register
 **********************************************************************/
#define EMAC_FCC_MIRR_CNT(n)            (n&0xFFFF)          /**< Mirror Counter                    */
#define EMAC_FCC_PAUSE_TIM(n)           ((n&0xFFFF)<<16)      /**< Pause Timer                       */

/*********************************************************************//**
 * Macro defines for Flow Control Status Register
 **********************************************************************/
#define EMAC_FCS_MIRR_CNT(n)            (n&0xFFFF)          /**< Mirror Counter Current            */


/* Receive filter register definitions -------------------------------------------------------- */
/*********************************************************************//**
 * Macro defines for Receive Filter Control Register
 **********************************************************************/
#define EMAC_RxFilterCtrl_UCAST_EN        0x00000001  /**< Accept Unicast Frames Enable      */
#define EMAC_RxFilterCtrl_BCAST_EN        0x00000002  /**< Accept Broadcast Frames Enable    */
#define EMAC_RxFilterCtrl_MCAST_EN        0x00000004  /**< Accept Multicast Frames Enable    */
#define EMAC_RxFilterCtrl_UCAST_HASH_EN   0x00000008  /**< Accept Unicast Hash Filter Frames */
#define EMAC_RxFilterCtrl_MCAST_HASH_EN   0x00000010  /**< Accept Multicast Hash Filter Fram.*/
#define EMAC_RxFilterCtrl_PERFECT_EN      0x00000020  /**< Accept Perfect Match Enable       */
#define EMAC_RxFilterCtrl_MAGP_WOL_EN     0x00001000  /**< Magic Packet Filter WoL Enable    */
#define EMAC_RxFilterCtrl_PFILT_WOL_EN    0x00002000  /**< Perfect Filter WoL Enable         */

/*********************************************************************//**
 * Macro defines for Receive Filter WoL Status/Clear Registers
 **********************************************************************/
#define EMAC_WOL_UCAST           0x00000001  /**< Unicast Frame caused WoL          */
#define EMAC_WOL_BCAST           0x00000002  /**< Broadcast Frame caused WoL        */
#define EMAC_WOL_MCAST           0x00000004  /**< Multicast Frame caused WoL        */
#define EMAC_WOL_UCAST_HASH      0x00000008  /**< Unicast Hash Filter Frame WoL     */
#define EMAC_WOL_MCAST_HASH      0x00000010  /**< Multicast Hash Filter Frame WoL   */
#define EMAC_WOL_PERFECT         0x00000020  /**< Perfect Filter WoL                */
#define EMAC_WOL_RX_FILTER       0x00000080  /**< RX Filter caused WoL              */
#define EMAC_WOL_MAG_PACKET      0x00000100  /**< Magic Packet Filter caused WoL    */
#define EMAC_WOL_BITMASK         0x01BF        /**< Receive Filter WoL Status/Clear bitmasl value */


/* Module control register definitions ---------------------------------------------------- */
/*********************************************************************//**
 * Macro defines for Interrupt Status/Enable/Clear/Set Registers
 **********************************************************************/
#define EMAC_Int_RX_OVERRUN      0x00000001  /**< Overrun Error in RX Queue         */
#define EMAC_Int_RX_ERR          0x00000002  /**< Receive Error                     */
#define EMAC_Int_RX_FIN          0x00000004  /**< RX Finished Process Descriptors   */
#define EMAC_Int_RX_DONE         0x00000008  /**< Receive Done                      */
#define EMAC_Int_TX_UNDERRUN     0x00000010  /**< Transmit Underrun                 */
#define EMAC_Int_TX_ERR          0x00000020  /**< Transmit Error                    */
#define EMAC_Int_TX_FIN          0x00000040  /**< TX Finished Process Descriptors   */
#define EMAC_Int_TX_DONE         0x00000080  /**< Transmit Done                     */
#define EMAC_Int_SOFT_INT        0x00001000  /**< Software Triggered Interrupt      */
#define EMAC_Int_WAKEUP          0x00002000  /**< Wakeup Event Interrupt            */

/*********************************************************************//**
 * Macro defines for Power Down Register
 **********************************************************************/
#define EMAC_PD_POWER_DOWN       0x80000000  /**< Power Down MAC                    */
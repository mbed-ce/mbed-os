/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL0_CLK_HZ                         24000000U  /*!< Board xtal0 frequency in Hz */

#define BOARD_XTAL32K_CLK_HZ                          32768U  /*!< Board xtal32k frequency in Hz */
/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************* Configuration BOARD_ClockFullSpeed **********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_ClockFullSpeed configuration
 ******************************************************************************/
#define BOARD_CLOCKFULLSPEED_CORE_CLOCK           528000000U  /*!< Core clock frequency: 528000000Hz */

/* Clock outputs (values are in Hz): */
#define BOARD_CLOCKFULLSPEED_AHB_CLK_ROOT             528000000UL    /* Clock consumers of AHB_CLK_ROOT output : AIPSTZ1, AIPSTZ2, AIPSTZ3, AIPSTZ4, ARM, FLEXSPI */
#define BOARD_CLOCKFULLSPEED_CAN_CLK_ROOT             40000000UL     /* Clock consumers of CAN_CLK_ROOT output : CAN1, CAN2 */
#define BOARD_CLOCKFULLSPEED_CKIL_SYNC_CLK_ROOT       32768UL        /* Clock consumers of CKIL_SYNC_CLK_ROOT output : CSU, EWM, GPT1, GPT2, KPP, PIT, RTWDOG, SNVS, SPDIF, TEMPMON, TSC, USB1, USB2, WDOG1, WDOG2 */
#define BOARD_CLOCKFULLSPEED_CLKO1_CLK                0UL            /* Clock consumers of CLKO1_CLK output : N/A */
#define BOARD_CLOCKFULLSPEED_CLKO2_CLK                0UL            /* Clock consumers of CLKO2_CLK output : N/A */
#define BOARD_CLOCKFULLSPEED_CLK_1M                   1000000UL      /* Clock consumers of CLK_1M output : EWM, RTWDOG */
#define BOARD_CLOCKFULLSPEED_CLK_24M                  24000000UL     /* Clock consumers of CLK_24M output : GPT1, GPT2 */
#define BOARD_CLOCKFULLSPEED_CSI_CLK_ROOT             12000000UL     /* Clock consumers of CSI_CLK_ROOT output : CSI */
#define BOARD_CLOCKFULLSPEED_ENET_125M_CLK            2400000UL      /* Clock consumers of ENET_125M_CLK output : N/A */
#define BOARD_CLOCKFULLSPEED_ENET_25M_REF_CLK         1200000UL      /* Clock consumers of ENET_25M_REF_CLK output : ENET */
#define BOARD_CLOCKFULLSPEED_ENET_REF_CLK             0UL            /* Clock consumers of ENET_REF_CLK output : ENET */
#define BOARD_CLOCKFULLSPEED_ENET_TX_CLK              0UL            /* Clock consumers of ENET_TX_CLK output : ENET */
#define BOARD_CLOCKFULLSPEED_FLEXIO1_CLK_ROOT         30000000UL     /* Clock consumers of FLEXIO1_CLK_ROOT output : FLEXIO1 */
#define BOARD_CLOCKFULLSPEED_FLEXIO2_CLK_ROOT         30000000UL     /* Clock consumers of FLEXIO2_CLK_ROOT output : FLEXIO2 */
#define BOARD_CLOCKFULLSPEED_FLEXSPI_CLK_ROOT         160000000UL    /* Clock consumers of FLEXSPI_CLK_ROOT output : FLEXSPI */
#define BOARD_CLOCKFULLSPEED_GPT1_IPG_CLK_HIGHFREQ    24000000UL     /* Clock consumers of GPT1_ipg_clk_highfreq output : GPT1 */
#define BOARD_CLOCKFULLSPEED_GPT2_IPG_CLK_HIGHFREQ    24000000UL     /* Clock consumers of GPT2_ipg_clk_highfreq output : GPT2 */
#define BOARD_CLOCKFULLSPEED_IPG_CLK_ROOT             132000000UL    /* Clock consumers of IPG_CLK_ROOT output : ADC1, ADC2, ADC_ETC, AOI1, AOI2, ARM, BEE, CAN1, CAN2, CCM, CMP1, CMP2, CMP3, CMP4, CSI, CSU, DCDC, DCP, DMA0, DMAMUX, ENC1, ENC2, ENC3, ENC4, ENET, EWM, FLEXIO1, FLEXIO2, FLEXRAM, FLEXSPI, GPC, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, IOMUXC, KPP, LCDIF, LPI2C1, LPI2C2, LPI2C3, LPI2C4, LPSPI1, LPSPI2, LPSPI3, LPSPI4, LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8, OCOTP, PMU, PWM1, PWM2, PWM3, PWM4, PXP, ROMC, RTWDOG, SAI1, SAI2, SAI3, SNVS, SPDIF, SRC, TEMPMON, TMR1, TMR2, TMR3, TMR4, TRNG, TSC, USB1, USB2, USDHC1, USDHC2, WDOG1, WDOG2, XBARA1, XBARB2, XBARB3 */
#define BOARD_CLOCKFULLSPEED_LCDIF_CLK_ROOT           67500000UL     /* Clock consumers of LCDIF_CLK_ROOT output : LCDIF */
#define BOARD_CLOCKFULLSPEED_LPI2C_CLK_ROOT           10000000UL     /* Clock consumers of LPI2C_CLK_ROOT output : LPI2C1, LPI2C2, LPI2C3, LPI2C4 */
#define BOARD_CLOCKFULLSPEED_LPSPI_CLK_ROOT           105600000UL    /* Clock consumers of LPSPI_CLK_ROOT output : LPSPI1, LPSPI2, LPSPI3, LPSPI4 */
#define BOARD_CLOCKFULLSPEED_LVDS1_CLK                1056000000UL   /* Clock consumers of LVDS1_CLK output : N/A */
#define BOARD_CLOCKFULLSPEED_MQS_MCLK                 63529411UL     /* Clock consumers of MQS_MCLK output : N/A */
#define BOARD_CLOCKFULLSPEED_PERCLK_CLK_ROOT          24000000UL     /* Clock consumers of PERCLK_CLK_ROOT output : GPT1, GPT2, PIT */
#define BOARD_CLOCKFULLSPEED_PLL7_MAIN_CLK            24000000UL     /* Clock consumers of PLL7_MAIN_CLK output : N/A */
#define BOARD_CLOCKFULLSPEED_SAI1_CLK_ROOT            63529411UL     /* Clock consumers of SAI1_CLK_ROOT output : N/A */
#define BOARD_CLOCKFULLSPEED_SAI1_MCLK1               63529411UL     /* Clock consumers of SAI1_MCLK1 output : SAI1 */
#define BOARD_CLOCKFULLSPEED_SAI1_MCLK2               63529411UL     /* Clock consumers of SAI1_MCLK2 output : SAI1 */
#define BOARD_CLOCKFULLSPEED_SAI1_MCLK3               30000000UL     /* Clock consumers of SAI1_MCLK3 output : SAI1 */
#define BOARD_CLOCKFULLSPEED_SAI2_CLK_ROOT            63529411UL     /* Clock consumers of SAI2_CLK_ROOT output : N/A */
#define BOARD_CLOCKFULLSPEED_SAI2_MCLK1               63529411UL     /* Clock consumers of SAI2_MCLK1 output : SAI2 */
#define BOARD_CLOCKFULLSPEED_SAI2_MCLK2               0UL            /* Clock consumers of SAI2_MCLK2 output : SAI2 */
#define BOARD_CLOCKFULLSPEED_SAI2_MCLK3               30000000UL     /* Clock consumers of SAI2_MCLK3 output : SAI2 */
#define BOARD_CLOCKFULLSPEED_SAI3_CLK_ROOT            63529411UL     /* Clock consumers of SAI3_CLK_ROOT output : N/A */
#define BOARD_CLOCKFULLSPEED_SAI3_MCLK1               63529411UL     /* Clock consumers of SAI3_MCLK1 output : SAI3 */
#define BOARD_CLOCKFULLSPEED_SAI3_MCLK2               0UL            /* Clock consumers of SAI3_MCLK2 output : SAI3 */
#define BOARD_CLOCKFULLSPEED_SAI3_MCLK3               30000000UL     /* Clock consumers of SAI3_MCLK3 output : SAI3 */
#define BOARD_CLOCKFULLSPEED_SEMC_CLK_ROOT            66000000UL     /* Clock consumers of SEMC_CLK_ROOT output : SEMC */
#define BOARD_CLOCKFULLSPEED_SPDIF0_CLK_ROOT          30000000UL     /* Clock consumers of SPDIF0_CLK_ROOT output : SPDIF */
#define BOARD_CLOCKFULLSPEED_SPDIF0_EXTCLK_OUT        0UL            /* Clock consumers of SPDIF0_EXTCLK_OUT output : SPDIF */
#define BOARD_CLOCKFULLSPEED_TRACE_CLK_ROOT           117333333UL    /* Clock consumers of TRACE_CLK_ROOT output : ARM */
#define BOARD_CLOCKFULLSPEED_UART_CLK_ROOT            24000000UL     /* Clock consumers of UART_CLK_ROOT output : LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8 */
#define BOARD_CLOCKFULLSPEED_USBPHY1_CLK              0UL            /* Clock consumers of USBPHY1_CLK output : TEMPMON, USB1 */
#define BOARD_CLOCKFULLSPEED_USBPHY2_CLK              0UL            /* Clock consumers of USBPHY2_CLK output : USB2 */
#define BOARD_CLOCKFULLSPEED_USDHC1_CLK_ROOT          176000000UL    /* Clock consumers of USDHC1_CLK_ROOT output : USDHC1 */
#define BOARD_CLOCKFULLSPEED_USDHC2_CLK_ROOT          176000000UL    /* Clock consumers of USDHC2_CLK_ROOT output : USDHC2 */

/*! @brief Arm PLL set for BOARD_ClockFullSpeed configuration.
 */
extern const clock_arm_pll_config_t armPllConfig_BOARD_ClockFullSpeed;
/*! @brief Usb1 PLL set for BOARD_ClockFullSpeed configuration.
 */
extern const clock_usb_pll_config_t usb1PllConfig_BOARD_ClockFullSpeed;
/*! @brief Sys PLL for BOARD_ClockFullSpeed configuration.
 */
extern const clock_sys_pll_config_t sysPllConfig_BOARD_ClockFullSpeed;

/*******************************************************************************
 * API for BOARD_ClockFullSpeed configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_ClockFullSpeed(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************* Configuration BOARD_ClockOverdrive **********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_ClockOverdrive configuration
 ******************************************************************************/
#define BOARD_CLOCKOVERDRIVE_CORE_CLOCK           600000000U  /*!< Core clock frequency: 600000000Hz */

/* Clock outputs (values are in Hz): */
#define BOARD_CLOCKOVERDRIVE_AHB_CLK_ROOT             600000000UL    /* Clock consumers of AHB_CLK_ROOT output : AIPSTZ1, AIPSTZ2, AIPSTZ3, AIPSTZ4, ARM, FLEXSPI */
#define BOARD_CLOCKOVERDRIVE_CAN_CLK_ROOT             40000000UL     /* Clock consumers of CAN_CLK_ROOT output : CAN1, CAN2 */
#define BOARD_CLOCKOVERDRIVE_CKIL_SYNC_CLK_ROOT       32768UL        /* Clock consumers of CKIL_SYNC_CLK_ROOT output : CSU, EWM, GPT1, GPT2, KPP, PIT, RTWDOG, SNVS, SPDIF, TEMPMON, TSC, USB1, USB2, WDOG1, WDOG2 */
#define BOARD_CLOCKOVERDRIVE_CLKO1_CLK                0UL            /* Clock consumers of CLKO1_CLK output : N/A */
#define BOARD_CLOCKOVERDRIVE_CLKO2_CLK                0UL            /* Clock consumers of CLKO2_CLK output : N/A */
#define BOARD_CLOCKOVERDRIVE_CLK_1M                   1000000UL      /* Clock consumers of CLK_1M output : EWM, RTWDOG */
#define BOARD_CLOCKOVERDRIVE_CLK_24M                  24000000UL     /* Clock consumers of CLK_24M output : GPT1, GPT2 */
#define BOARD_CLOCKOVERDRIVE_CSI_CLK_ROOT             12000000UL     /* Clock consumers of CSI_CLK_ROOT output : CSI */
#define BOARD_CLOCKOVERDRIVE_ENET_125M_CLK            2400000UL      /* Clock consumers of ENET_125M_CLK output : N/A */
#define BOARD_CLOCKOVERDRIVE_ENET_25M_REF_CLK         1200000UL      /* Clock consumers of ENET_25M_REF_CLK output : ENET */
#define BOARD_CLOCKOVERDRIVE_ENET_REF_CLK             0UL            /* Clock consumers of ENET_REF_CLK output : ENET */
#define BOARD_CLOCKOVERDRIVE_ENET_TX_CLK              0UL            /* Clock consumers of ENET_TX_CLK output : ENET */
#define BOARD_CLOCKOVERDRIVE_FLEXIO1_CLK_ROOT         30000000UL     /* Clock consumers of FLEXIO1_CLK_ROOT output : FLEXIO1 */
#define BOARD_CLOCKOVERDRIVE_FLEXIO2_CLK_ROOT         30000000UL     /* Clock consumers of FLEXIO2_CLK_ROOT output : FLEXIO2 */
#define BOARD_CLOCKOVERDRIVE_FLEXSPI_CLK_ROOT         160000000UL    /* Clock consumers of FLEXSPI_CLK_ROOT output : FLEXSPI */
#define BOARD_CLOCKOVERDRIVE_GPT1_IPG_CLK_HIGHFREQ    24000000UL     /* Clock consumers of GPT1_ipg_clk_highfreq output : GPT1 */
#define BOARD_CLOCKOVERDRIVE_GPT2_IPG_CLK_HIGHFREQ    24000000UL     /* Clock consumers of GPT2_ipg_clk_highfreq output : GPT2 */
#define BOARD_CLOCKOVERDRIVE_IPG_CLK_ROOT             150000000UL    /* Clock consumers of IPG_CLK_ROOT output : ADC1, ADC2, ADC_ETC, AOI1, AOI2, ARM, BEE, CAN1, CAN2, CCM, CMP1, CMP2, CMP3, CMP4, CSI, CSU, DCDC, DCP, DMA0, DMAMUX, ENC1, ENC2, ENC3, ENC4, ENET, EWM, FLEXIO1, FLEXIO2, FLEXRAM, FLEXSPI, GPC, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, IOMUXC, KPP, LCDIF, LPI2C1, LPI2C2, LPI2C3, LPI2C4, LPSPI1, LPSPI2, LPSPI3, LPSPI4, LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8, OCOTP, PMU, PWM1, PWM2, PWM3, PWM4, PXP, ROMC, RTWDOG, SAI1, SAI2, SAI3, SNVS, SPDIF, SRC, TEMPMON, TMR1, TMR2, TMR3, TMR4, TRNG, TSC, USB1, USB2, USDHC1, USDHC2, WDOG1, WDOG2, XBARA1, XBARB2, XBARB3 */
#define BOARD_CLOCKOVERDRIVE_LCDIF_CLK_ROOT           67500000UL     /* Clock consumers of LCDIF_CLK_ROOT output : LCDIF */
#define BOARD_CLOCKOVERDRIVE_LPI2C_CLK_ROOT           10000000UL     /* Clock consumers of LPI2C_CLK_ROOT output : LPI2C1, LPI2C2, LPI2C3, LPI2C4 */
#define BOARD_CLOCKOVERDRIVE_LPSPI_CLK_ROOT           105600000UL    /* Clock consumers of LPSPI_CLK_ROOT output : LPSPI1, LPSPI2, LPSPI3, LPSPI4 */
#define BOARD_CLOCKOVERDRIVE_LVDS1_CLK                1200000000UL   /* Clock consumers of LVDS1_CLK output : N/A */
#define BOARD_CLOCKOVERDRIVE_MQS_MCLK                 63529411UL     /* Clock consumers of MQS_MCLK output : N/A */
#define BOARD_CLOCKOVERDRIVE_PERCLK_CLK_ROOT          24000000UL     /* Clock consumers of PERCLK_CLK_ROOT output : GPT1, GPT2, PIT */
#define BOARD_CLOCKOVERDRIVE_PLL7_MAIN_CLK            24000000UL     /* Clock consumers of PLL7_MAIN_CLK output : N/A */
#define BOARD_CLOCKOVERDRIVE_SAI1_CLK_ROOT            63529411UL     /* Clock consumers of SAI1_CLK_ROOT output : N/A */
#define BOARD_CLOCKOVERDRIVE_SAI1_MCLK1               63529411UL     /* Clock consumers of SAI1_MCLK1 output : SAI1 */
#define BOARD_CLOCKOVERDRIVE_SAI1_MCLK2               63529411UL     /* Clock consumers of SAI1_MCLK2 output : SAI1 */
#define BOARD_CLOCKOVERDRIVE_SAI1_MCLK3               30000000UL     /* Clock consumers of SAI1_MCLK3 output : SAI1 */
#define BOARD_CLOCKOVERDRIVE_SAI2_CLK_ROOT            63529411UL     /* Clock consumers of SAI2_CLK_ROOT output : N/A */
#define BOARD_CLOCKOVERDRIVE_SAI2_MCLK1               63529411UL     /* Clock consumers of SAI2_MCLK1 output : SAI2 */
#define BOARD_CLOCKOVERDRIVE_SAI2_MCLK2               0UL            /* Clock consumers of SAI2_MCLK2 output : SAI2 */
#define BOARD_CLOCKOVERDRIVE_SAI2_MCLK3               30000000UL     /* Clock consumers of SAI2_MCLK3 output : SAI2 */
#define BOARD_CLOCKOVERDRIVE_SAI3_CLK_ROOT            63529411UL     /* Clock consumers of SAI3_CLK_ROOT output : N/A */
#define BOARD_CLOCKOVERDRIVE_SAI3_MCLK1               63529411UL     /* Clock consumers of SAI3_MCLK1 output : SAI3 */
#define BOARD_CLOCKOVERDRIVE_SAI3_MCLK2               0UL            /* Clock consumers of SAI3_MCLK2 output : SAI3 */
#define BOARD_CLOCKOVERDRIVE_SAI3_MCLK3               30000000UL     /* Clock consumers of SAI3_MCLK3 output : SAI3 */
#define BOARD_CLOCKOVERDRIVE_SEMC_CLK_ROOT            75000000UL     /* Clock consumers of SEMC_CLK_ROOT output : SEMC */
#define BOARD_CLOCKOVERDRIVE_SPDIF0_CLK_ROOT          30000000UL     /* Clock consumers of SPDIF0_CLK_ROOT output : SPDIF */
#define BOARD_CLOCKOVERDRIVE_SPDIF0_EXTCLK_OUT        0UL            /* Clock consumers of SPDIF0_EXTCLK_OUT output : SPDIF */
#define BOARD_CLOCKOVERDRIVE_TRACE_CLK_ROOT           117333333UL    /* Clock consumers of TRACE_CLK_ROOT output : ARM */
#define BOARD_CLOCKOVERDRIVE_UART_CLK_ROOT            24000000UL     /* Clock consumers of UART_CLK_ROOT output : LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8 */
#define BOARD_CLOCKOVERDRIVE_USBPHY1_CLK              0UL            /* Clock consumers of USBPHY1_CLK output : TEMPMON, USB1 */
#define BOARD_CLOCKOVERDRIVE_USBPHY2_CLK              0UL            /* Clock consumers of USBPHY2_CLK output : USB2 */
#define BOARD_CLOCKOVERDRIVE_USDHC1_CLK_ROOT          198000000UL    /* Clock consumers of USDHC1_CLK_ROOT output : USDHC1 */
#define BOARD_CLOCKOVERDRIVE_USDHC2_CLK_ROOT          198000000UL    /* Clock consumers of USDHC2_CLK_ROOT output : USDHC2 */

/*! @brief Arm PLL set for BOARD_ClockOverdrive configuration.
 */
extern const clock_arm_pll_config_t armPllConfig_BOARD_ClockOverdrive;
/*! @brief Usb1 PLL set for BOARD_ClockOverdrive configuration.
 */
extern const clock_usb_pll_config_t usb1PllConfig_BOARD_ClockOverdrive;
/*! @brief Sys PLL for BOARD_ClockOverdrive configuration.
 */
extern const clock_sys_pll_config_t sysPllConfig_BOARD_ClockOverdrive;

/*******************************************************************************
 * API for BOARD_ClockOverdrive configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_ClockOverdrive(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************* Configuration BOARD_ClockLowPower ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_ClockLowPower configuration
 ******************************************************************************/
#define BOARD_CLOCKLOWPOWER_CORE_CLOCK             24000000U  /*!< Core clock frequency: 24000000Hz */

/* Clock outputs (values are in Hz): */
#define BOARD_CLOCKLOWPOWER_AHB_CLK_ROOT              24000000UL     /* Clock consumers of AHB_CLK_ROOT output : AIPSTZ1, AIPSTZ2, AIPSTZ3, AIPSTZ4, ARM, FLEXSPI */
#define BOARD_CLOCKLOWPOWER_CAN_CLK_ROOT              2000000UL      /* Clock consumers of CAN_CLK_ROOT output : CAN1, CAN2 */
#define BOARD_CLOCKLOWPOWER_CKIL_SYNC_CLK_ROOT        0UL            /* Clock consumers of CKIL_SYNC_CLK_ROOT output : CSU, EWM, GPT1, GPT2, KPP, PIT, RTWDOG, SNVS, SPDIF, TEMPMON, TSC, USB1, USB2, WDOG1, WDOG2 */
#define BOARD_CLOCKLOWPOWER_CLKO1_CLK                 0UL            /* Clock consumers of CLKO1_CLK output : N/A */
#define BOARD_CLOCKLOWPOWER_CLKO2_CLK                 0UL            /* Clock consumers of CLKO2_CLK output : N/A */
#define BOARD_CLOCKLOWPOWER_CLK_1M                    1000000UL      /* Clock consumers of CLK_1M output : EWM, RTWDOG */
#define BOARD_CLOCKLOWPOWER_CLK_24M                   24000000UL     /* Clock consumers of CLK_24M output : GPT1, GPT2 */
#define BOARD_CLOCKLOWPOWER_CSI_CLK_ROOT              12000000UL     /* Clock consumers of CSI_CLK_ROOT output : CSI */
#define BOARD_CLOCKLOWPOWER_ENET_125M_CLK             2400000UL      /* Clock consumers of ENET_125M_CLK output : N/A */
#define BOARD_CLOCKLOWPOWER_ENET_25M_REF_CLK          1200000UL      /* Clock consumers of ENET_25M_REF_CLK output : ENET */
#define BOARD_CLOCKLOWPOWER_ENET_REF_CLK              0UL            /* Clock consumers of ENET_REF_CLK output : ENET */
#define BOARD_CLOCKLOWPOWER_ENET_TX_CLK               0UL            /* Clock consumers of ENET_TX_CLK output : ENET */
#define BOARD_CLOCKLOWPOWER_FLEXIO1_CLK_ROOT          1500000UL      /* Clock consumers of FLEXIO1_CLK_ROOT output : FLEXIO1 */
#define BOARD_CLOCKLOWPOWER_FLEXIO2_CLK_ROOT          1500000UL      /* Clock consumers of FLEXIO2_CLK_ROOT output : FLEXIO2 */
#define BOARD_CLOCKLOWPOWER_FLEXSPI_CLK_ROOT          24000000UL     /* Clock consumers of FLEXSPI_CLK_ROOT output : FLEXSPI */
#define BOARD_CLOCKLOWPOWER_GPT1_IPG_CLK_HIGHFREQ     24000000UL     /* Clock consumers of GPT1_ipg_clk_highfreq output : GPT1 */
#define BOARD_CLOCKLOWPOWER_GPT2_IPG_CLK_HIGHFREQ     24000000UL     /* Clock consumers of GPT2_ipg_clk_highfreq output : GPT2 */
#define BOARD_CLOCKLOWPOWER_IPG_CLK_ROOT              12000000UL     /* Clock consumers of IPG_CLK_ROOT output : ADC1, ADC2, ADC_ETC, AOI1, AOI2, ARM, BEE, CAN1, CAN2, CCM, CMP1, CMP2, CMP3, CMP4, CSI, CSU, DCDC, DCP, DMA0, DMAMUX, ENC1, ENC2, ENC3, ENC4, ENET, EWM, FLEXIO1, FLEXIO2, FLEXRAM, FLEXSPI, GPC, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, IOMUXC, KPP, LCDIF, LPI2C1, LPI2C2, LPI2C3, LPI2C4, LPSPI1, LPSPI2, LPSPI3, LPSPI4, LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8, OCOTP, PMU, PWM1, PWM2, PWM3, PWM4, PXP, ROMC, RTWDOG, SAI1, SAI2, SAI3, SNVS, SPDIF, SRC, TEMPMON, TMR1, TMR2, TMR3, TMR4, TRNG, TSC, USB1, USB2, USDHC1, USDHC2, WDOG1, WDOG2, XBARA1, XBARB2, XBARB3 */
#define BOARD_CLOCKLOWPOWER_LCDIF_CLK_ROOT            3000000UL      /* Clock consumers of LCDIF_CLK_ROOT output : LCDIF */
#define BOARD_CLOCKLOWPOWER_LPI2C_CLK_ROOT            3000000UL      /* Clock consumers of LPI2C_CLK_ROOT output : LPI2C1, LPI2C2, LPI2C3, LPI2C4 */
#define BOARD_CLOCKLOWPOWER_LPSPI_CLK_ROOT            12000000UL     /* Clock consumers of LPSPI_CLK_ROOT output : LPSPI1, LPSPI2, LPSPI3, LPSPI4 */
#define BOARD_CLOCKLOWPOWER_LVDS1_CLK                 24000000UL     /* Clock consumers of LVDS1_CLK output : N/A */
#define BOARD_CLOCKLOWPOWER_MQS_MCLK                  3000000UL      /* Clock consumers of MQS_MCLK output : N/A */
#define BOARD_CLOCKLOWPOWER_PERCLK_CLK_ROOT           24000000UL     /* Clock consumers of PERCLK_CLK_ROOT output : GPT1, GPT2, PIT */
#define BOARD_CLOCKLOWPOWER_PLL7_MAIN_CLK             24000000UL     /* Clock consumers of PLL7_MAIN_CLK output : N/A */
#define BOARD_CLOCKLOWPOWER_SAI1_CLK_ROOT             3000000UL      /* Clock consumers of SAI1_CLK_ROOT output : N/A */
#define BOARD_CLOCKLOWPOWER_SAI1_MCLK1                3000000UL      /* Clock consumers of SAI1_MCLK1 output : SAI1 */
#define BOARD_CLOCKLOWPOWER_SAI1_MCLK2                3000000UL      /* Clock consumers of SAI1_MCLK2 output : SAI1 */
#define BOARD_CLOCKLOWPOWER_SAI1_MCLK3                1500000UL      /* Clock consumers of SAI1_MCLK3 output : SAI1 */
#define BOARD_CLOCKLOWPOWER_SAI2_CLK_ROOT             3000000UL      /* Clock consumers of SAI2_CLK_ROOT output : N/A */
#define BOARD_CLOCKLOWPOWER_SAI2_MCLK1                3000000UL      /* Clock consumers of SAI2_MCLK1 output : SAI2 */
#define BOARD_CLOCKLOWPOWER_SAI2_MCLK2                0UL            /* Clock consumers of SAI2_MCLK2 output : SAI2 */
#define BOARD_CLOCKLOWPOWER_SAI2_MCLK3                1500000UL      /* Clock consumers of SAI2_MCLK3 output : SAI2 */
#define BOARD_CLOCKLOWPOWER_SAI3_CLK_ROOT             3000000UL      /* Clock consumers of SAI3_CLK_ROOT output : N/A */
#define BOARD_CLOCKLOWPOWER_SAI3_MCLK1                3000000UL      /* Clock consumers of SAI3_MCLK1 output : SAI3 */
#define BOARD_CLOCKLOWPOWER_SAI3_MCLK2                0UL            /* Clock consumers of SAI3_MCLK2 output : SAI3 */
#define BOARD_CLOCKLOWPOWER_SAI3_MCLK3                1500000UL      /* Clock consumers of SAI3_MCLK3 output : SAI3 */
#define BOARD_CLOCKLOWPOWER_SEMC_CLK_ROOT             24000000UL     /* Clock consumers of SEMC_CLK_ROOT output : SEMC */
#define BOARD_CLOCKLOWPOWER_SPDIF0_CLK_ROOT           1500000UL      /* Clock consumers of SPDIF0_CLK_ROOT output : SPDIF */
#define BOARD_CLOCKLOWPOWER_SPDIF0_EXTCLK_OUT         0UL            /* Clock consumers of SPDIF0_EXTCLK_OUT output : SPDIF */
#define BOARD_CLOCKLOWPOWER_TRACE_CLK_ROOT            6000000UL      /* Clock consumers of TRACE_CLK_ROOT output : ARM */
#define BOARD_CLOCKLOWPOWER_UART_CLK_ROOT             24000000UL     /* Clock consumers of UART_CLK_ROOT output : LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8 */
#define BOARD_CLOCKLOWPOWER_USBPHY1_CLK               0UL            /* Clock consumers of USBPHY1_CLK output : TEMPMON, USB1 */
#define BOARD_CLOCKLOWPOWER_USBPHY2_CLK               0UL            /* Clock consumers of USBPHY2_CLK output : USB2 */
#define BOARD_CLOCKLOWPOWER_USDHC1_CLK_ROOT           12000000UL     /* Clock consumers of USDHC1_CLK_ROOT output : USDHC1 */
#define BOARD_CLOCKLOWPOWER_USDHC2_CLK_ROOT           12000000UL     /* Clock consumers of USDHC2_CLK_ROOT output : USDHC2 */


/*******************************************************************************
 * API for BOARD_ClockLowPower configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_ClockLowPower(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */


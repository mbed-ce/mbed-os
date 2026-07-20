/* Copyright (c) 2024 Renesas Electronics Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* generated vector source file - do not edit */
        #include "bsp_api.h"
        /* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
        #if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = rtc_carry_isr, /* RTC CARRY (Carry interrupt) */
            [1] = gpt_counter_overflow_isr, /* GPT1 COUNTER OVERFLOW (Overflow) */
            [2] = gpt_capture_compare_a_isr, /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
            [3] = r_icu_isr, /* ICU IRQ0 (External pin interrupt 0) */
            [4] = iic_master_rxi_isr, /* IIC0 RXI (Receive data full) */
            [5] = iic_master_txi_isr, /* IIC0 TXI (Transmit data empty) */
            [6] = iic_master_tei_isr, /* IIC0 TEI (Transmit end) */
            [7] = iic_master_eri_isr, /* IIC0 ERI (Transfer error) */
            [8] = r_icu_isr, /* ICU IRQ1 (External pin interrupt 1) */
            [9] = r_icu_isr, /* ICU IRQ2 (External pin interrupt 2) */
            [10] = r_icu_isr, /* ICU IRQ3 (External pin interrupt 3) */
            [11] = r_icu_isr, /* ICU IRQ4 (External pin interrupt 4) */
            [12] = r_icu_isr, /* ICU IRQ5 (External pin interrupt 5) */
            [13] = r_icu_isr, /* ICU IRQ6 (External pin interrupt 6) */
            [14] = r_icu_isr, /* ICU IRQ7 (External pin interrupt 7) */
            [15] = r_icu_isr, /* ICU IRQ8 (External pin interrupt 8) */
            [16] = r_icu_isr, /* ICU IRQ9 (External pin interrupt 9) */
            [17] = r_icu_isr, /* ICU IRQ13 (External pin interrupt 13) */
            [18] = spi_b_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [19] = spi_b_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [20] = spi_b_eri_isr, /* SPI0 ERI (Error) */
            [21] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [22] = r_icu_isr, /* ICU IRQ10 (External pin interrupt 10) */
            [23] = r_icu_isr, /* ICU IRQ11 (External pin interrupt 11) */
            [24] = r_icu_isr, /* ICU IRQ12 (External pin interrupt 12) */
            [25] = r_icu_isr, /* ICU IRQ14 (External pin interrupt 14) */
            [26] = r_icu_isr, /* ICU IRQ15 (External pin interrupt 15) */
            [27] = sci_b_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
            [28] = sci_b_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [29] = sci_b_uart_tei_isr, /* SCI0 TEI (Transmit end) */
            [30] = sci_b_uart_eri_isr, /* SCI0 ERI (Receive error) */
            [31] = sci_b_uart_rxi_isr, /* SCI1 RXI (Receive data full) */
            [32] = sci_b_uart_txi_isr, /* SCI1 TXI (Transmit data empty) */
            [33] = sci_b_uart_tei_isr, /* SCI1 TEI (Transmit end) */
            [34] = sci_b_uart_eri_isr, /* SCI1 ERI (Receive error) */
            [35] = sci_b_uart_rxi_isr, /* SCI2 RXI (Receive data full) */
            [36] = sci_b_uart_txi_isr, /* SCI2 TXI (Transmit data empty) */
            [37] = sci_b_uart_tei_isr, /* SCI2 TEI (Transmit end) */
            [38] = sci_b_uart_eri_isr, /* SCI2 ERI (Receive error) */
            [39] = sci_b_uart_rxi_isr, /* SCI3 RXI (Receive data full) */
            [40] = sci_b_uart_txi_isr, /* SCI3 TXI (Transmit data empty) */
            [41] = sci_b_uart_tei_isr, /* SCI3 TEI (Transmit end) */
            [42] = sci_b_uart_eri_isr, /* SCI3 ERI (Receive error) */
            [43] = sci_b_uart_rxi_isr, /* SCI4 RXI (Receive data full) */
            [44] = sci_b_uart_txi_isr, /* SCI4 TXI (Transmit data empty) */
            [45] = sci_b_uart_tei_isr, /* SCI4 TEI (Transmit end) */
            [46] = sci_b_uart_eri_isr, /* SCI4 ERI (Receive error) */
            [47] = sci_b_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [48] = sci_b_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [49] = sci_b_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [50] = sci_b_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [51] = spi_b_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [52] = spi_b_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [53] = spi_b_eri_isr, /* SPI1 ERI (Error) */
            [54] = dmac_int_isr, /* DMAC1 INT (DMAC1 transfer end) */
            [55] = iic_master_rxi_isr, /* IIC1 RXI (Receive data full) */
            [56] = iic_master_txi_isr, /* IIC1 TXI (Transmit data empty) */
            [57] = iic_master_tei_isr, /* IIC1 TEI (Transmit end) */
            [58] = iic_master_eri_isr, /* IIC1 ERI (Transfer error) */
            [59] = canfd_error_isr, /* CAN0 CHERR (Channel  error) */
            [60] = canfd_channel_tx_isr, /* CAN0 TX (Transmit interrupt) */
            [61] = canfd_common_fifo_rx_isr, /* CAN0 COMFRX (Common FIFO receive interrupt) */
            [62] = canfd_error_isr, /* CAN GLERR (Global error) */
            [63] = canfd_rx_fifo_isr, /* CAN RXF (Global receive FIFO interrupt) */
            [64] = canfd_error_isr, /* CAN1 CHERR (Channel  error) */
            [65] = canfd_channel_tx_isr, /* CAN1 TX (Transmit interrupt) */
            [66] = canfd_common_fifo_rx_isr, /* CAN1 COMFRX (Common FIFO receive interrupt) */
            [67] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [68] = fcu_fiferr_isr, /* FCU FIFERR (Flash access error interrupt) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_RTC_CARRY,GROUP0), /* RTC CARRY (Carry interrupt) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_GPT1_COUNTER_OVERFLOW,GROUP1), /* GPT1 COUNTER OVERFLOW (Overflow) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_GPT1_CAPTURE_COMPARE_A,GROUP2), /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ0,GROUP3), /* ICU IRQ0 (External pin interrupt 0) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_IIC0_RXI,GROUP4), /* IIC0 RXI (Receive data full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TXI,GROUP5), /* IIC0 TXI (Transmit data empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TEI,GROUP6), /* IIC0 TEI (Transmit end) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_IIC0_ERI,GROUP7), /* IIC0 ERI (Transfer error) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ1,GROUP0), /* ICU IRQ1 (External pin interrupt 1) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ2,GROUP1), /* ICU IRQ2 (External pin interrupt 2) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ3,GROUP2), /* ICU IRQ3 (External pin interrupt 3) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ4,GROUP3), /* ICU IRQ4 (External pin interrupt 4) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ5,GROUP4), /* ICU IRQ5 (External pin interrupt 5) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ6,GROUP5), /* ICU IRQ6 (External pin interrupt 6) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ7,GROUP6), /* ICU IRQ7 (External pin interrupt 7) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ8,GROUP7), /* ICU IRQ8 (External pin interrupt 8) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ9,GROUP0), /* ICU IRQ9 (External pin interrupt 9) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ13,GROUP1), /* ICU IRQ13 (External pin interrupt 13) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP2), /* SPI0 RXI (Receive buffer full) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP3), /* SPI0 TEI (Transmission complete event) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP4), /* SPI0 ERI (Error) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP5), /* DMAC0 INT (DMAC0 transfer end) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ10,GROUP6), /* ICU IRQ10 (External pin interrupt 10) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ11,GROUP7), /* ICU IRQ11 (External pin interrupt 11) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ12,GROUP0), /* ICU IRQ12 (External pin interrupt 12) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ14,GROUP1), /* ICU IRQ14 (External pin interrupt 14) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ15,GROUP2), /* ICU IRQ15 (External pin interrupt 15) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_SCI0_RXI,GROUP3), /* SCI0 RXI (Receive data full) */
            [28] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,GROUP4), /* SCI0 TXI (Transmit data empty) */
            [29] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,GROUP5), /* SCI0 TEI (Transmit end) */
            [30] = BSP_PRV_VECT_ENUM(EVENT_SCI0_ERI,GROUP6), /* SCI0 ERI (Receive error) */
            [31] = BSP_PRV_VECT_ENUM(EVENT_SCI1_RXI,GROUP7), /* SCI1 RXI (Receive data full) */
            [32] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TXI,FIXED), /* SCI1 TXI (Transmit data empty) */
            [33] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TEI,FIXED), /* SCI1 TEI (Transmit end) */
            [34] = BSP_PRV_VECT_ENUM(EVENT_SCI1_ERI,FIXED), /* SCI1 ERI (Receive error) */
            [35] = BSP_PRV_VECT_ENUM(EVENT_SCI2_RXI,FIXED), /* SCI2 RXI (Receive data full) */
            [36] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TXI,FIXED), /* SCI2 TXI (Transmit data empty) */
            [37] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TEI,FIXED), /* SCI2 TEI (Transmit end) */
            [38] = BSP_PRV_VECT_ENUM(EVENT_SCI2_ERI,FIXED), /* SCI2 ERI (Receive error) */
            [39] = BSP_PRV_VECT_ENUM(EVENT_SCI3_RXI,FIXED), /* SCI3 RXI (Receive data full) */
            [40] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TXI,FIXED), /* SCI3 TXI (Transmit data empty) */
            [41] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TEI,FIXED), /* SCI3 TEI (Transmit end) */
            [42] = BSP_PRV_VECT_ENUM(EVENT_SCI3_ERI,FIXED), /* SCI3 ERI (Receive error) */
            [43] = BSP_PRV_VECT_ENUM(EVENT_SCI4_RXI,FIXED), /* SCI4 RXI (Receive data full) */
            [44] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TXI,FIXED), /* SCI4 TXI (Transmit data empty) */
            [45] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TEI,FIXED), /* SCI4 TEI (Transmit end) */
            [46] = BSP_PRV_VECT_ENUM(EVENT_SCI4_ERI,FIXED), /* SCI4 ERI (Receive error) */
            [47] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,FIXED), /* SCI9 RXI (Receive data full) */
            [48] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,FIXED), /* SCI9 TXI (Transmit data empty) */
            [49] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,FIXED), /* SCI9 TEI (Transmit end) */
            [50] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,FIXED), /* SCI9 ERI (Receive error) */
            [51] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,FIXED), /* SPI1 RXI (Receive buffer full) */
            [52] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,FIXED), /* SPI1 TEI (Transmission complete event) */
            [53] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,FIXED), /* SPI1 ERI (Error) */
            [54] = BSP_PRV_VECT_ENUM(EVENT_DMAC1_INT,FIXED), /* DMAC1 INT (DMAC1 transfer end) */
            [55] = BSP_PRV_VECT_ENUM(EVENT_IIC1_RXI,FIXED), /* IIC1 RXI (Receive data full) */
            [56] = BSP_PRV_VECT_ENUM(EVENT_IIC1_TXI,FIXED), /* IIC1 TXI (Transmit data empty) */
            [57] = BSP_PRV_VECT_ENUM(EVENT_IIC1_TEI,FIXED), /* IIC1 TEI (Transmit end) */
            [58] = BSP_PRV_VECT_ENUM(EVENT_IIC1_ERI,FIXED), /* IIC1 ERI (Transfer error) */
            [59] = BSP_PRV_VECT_ENUM(EVENT_CAN0_CHERR,FIXED), /* CAN0 CHERR (Channel  error) */
            [60] = BSP_PRV_VECT_ENUM(EVENT_CAN0_TX,FIXED), /* CAN0 TX (Transmit interrupt) */
            [61] = BSP_PRV_VECT_ENUM(EVENT_CAN0_COMFRX,FIXED), /* CAN0 COMFRX (Common FIFO receive interrupt) */
            [62] = BSP_PRV_VECT_ENUM(EVENT_CAN_GLERR,FIXED), /* CAN GLERR (Global error) */
            [63] = BSP_PRV_VECT_ENUM(EVENT_CAN_RXF,FIXED), /* CAN RXF (Global receive FIFO interrupt) */
            [64] = BSP_PRV_VECT_ENUM(EVENT_CAN1_CHERR,FIXED), /* CAN1 CHERR (Channel  error) */
            [65] = BSP_PRV_VECT_ENUM(EVENT_CAN1_TX,FIXED), /* CAN1 TX (Transmit interrupt) */
            [66] = BSP_PRV_VECT_ENUM(EVENT_CAN1_COMFRX,FIXED), /* CAN1 COMFRX (Common FIFO receive interrupt) */
            [67] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,FIXED), /* FCU FRDYI (Flash ready interrupt) */
            [68] = BSP_PRV_VECT_ENUM(EVENT_FCU_FIFERR,FIXED), /* FCU FIFERR (Flash access error interrupt) */
        };
        #endif
        #endif
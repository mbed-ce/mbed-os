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
            [8] = spi_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [9] = spi_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [10] = spi_eri_isr, /* SPI0 ERI (Error) */
            [11] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [12] = sci_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
            [13] = sci_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [14] = sci_uart_tei_isr, /* SCI0 TEI (Transmit end) */
            [15] = sci_uart_eri_isr, /* SCI0 ERI (Receive error) */
            [16] = r_icu_isr, /* ICU IRQ1 (External pin interrupt 1) */
            [17] = r_icu_isr, /* ICU IRQ2 (External pin interrupt 2) */
            [18] = r_icu_isr, /* ICU IRQ3 (External pin interrupt 3) */
            [19] = r_icu_isr, /* ICU IRQ4 (External pin interrupt 4) */
            [20] = r_icu_isr, /* ICU IRQ5 (External pin interrupt 5) */
            [21] = r_icu_isr, /* ICU IRQ6 (External pin interrupt 6) */
            [22] = r_icu_isr, /* ICU IRQ7 (External pin interrupt 7) */
            [23] = r_icu_isr, /* ICU IRQ8 (External pin interrupt 8) */
            [24] = r_icu_isr, /* ICU IRQ9 (External pin interrupt 9) */
            [25] = r_icu_isr, /* ICU IRQ13 (External pin interrupt 13) */
            [26] = sci_uart_rxi_isr, /* SCI3 RXI (Receive data full) */
            [27] = sci_uart_txi_isr, /* SCI3 TXI (Transmit data empty) */
            [28] = sci_uart_tei_isr, /* SCI3 TEI (Transmit end) */
            [29] = sci_uart_eri_isr, /* SCI3 ERI (Receive error) */
            [30] = sci_uart_rxi_isr, /* SCI4 RXI (Receive data full) */
            [31] = sci_uart_txi_isr, /* SCI4 TXI (Transmit data empty) */
            [32] = sci_uart_tei_isr, /* SCI4 TEI (Transmit end) */
            [33] = sci_uart_eri_isr, /* SCI4 ERI (Receive error) */
            [34] = sci_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [35] = sci_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [36] = sci_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [37] = sci_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [38] = can_error_isr, /* CAN0 ERROR (Error interrupt) */
            [39] = can_rx_isr, /* CAN0 MAILBOX RX (Reception complete interrupt) */
            [40] = can_tx_isr, /* CAN0 MAILBOX TX (Transmission complete interrupt) */
            [41] = can_rx_isr, /* CAN0 FIFO RX (Receive FIFO interrupt) */
            [42] = can_tx_isr, /* CAN0 FIFO TX (Transmit FIFO interrupt) */
            [43] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [44] = fcu_fiferr_isr, /* FCU FIFERR (Flash access error interrupt) */
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
            [8] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP0), /* SPI0 RXI (Receive buffer full) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP1), /* SPI0 TEI (Transmission complete event) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP2), /* SPI0 ERI (Error) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP3), /* DMAC0 INT (DMAC0 transfer end) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_SCI0_RXI,GROUP4), /* SCI0 RXI (Receive data full) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,GROUP5), /* SCI0 TXI (Transmit data empty) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,GROUP6), /* SCI0 TEI (Transmit end) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_SCI0_ERI,GROUP7), /* SCI0 ERI (Receive error) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ1,GROUP0), /* ICU IRQ1 (External pin interrupt 1) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ2,GROUP1), /* ICU IRQ2 (External pin interrupt 2) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ3,GROUP2), /* ICU IRQ3 (External pin interrupt 3) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ4,GROUP3), /* ICU IRQ4 (External pin interrupt 4) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ5,GROUP4), /* ICU IRQ5 (External pin interrupt 5) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ6,GROUP5), /* ICU IRQ6 (External pin interrupt 6) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ7,GROUP6), /* ICU IRQ7 (External pin interrupt 7) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ8,GROUP7), /* ICU IRQ8 (External pin interrupt 8) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ9,GROUP0), /* ICU IRQ9 (External pin interrupt 9) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ13,GROUP1), /* ICU IRQ13 (External pin interrupt 13) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_SCI3_RXI,GROUP2), /* SCI3 RXI (Receive data full) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TXI,GROUP3), /* SCI3 TXI (Transmit data empty) */
            [28] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TEI,GROUP4), /* SCI3 TEI (Transmit end) */
            [29] = BSP_PRV_VECT_ENUM(EVENT_SCI3_ERI,GROUP5), /* SCI3 ERI (Receive error) */
            [30] = BSP_PRV_VECT_ENUM(EVENT_SCI4_RXI,GROUP6), /* SCI4 RXI (Receive data full) */
            [31] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TXI,GROUP7), /* SCI4 TXI (Transmit data empty) */
            [32] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TEI,FIXED), /* SCI4 TEI (Transmit end) */
            [33] = BSP_PRV_VECT_ENUM(EVENT_SCI4_ERI,FIXED), /* SCI4 ERI (Receive error) */
            [34] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,FIXED), /* SCI9 RXI (Receive data full) */
            [35] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,FIXED), /* SCI9 TXI (Transmit data empty) */
            [36] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,FIXED), /* SCI9 TEI (Transmit end) */
            [37] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,FIXED), /* SCI9 ERI (Receive error) */
            [38] = BSP_PRV_VECT_ENUM(EVENT_CAN0_ERROR,FIXED), /* CAN0 ERROR (Error interrupt) */
            [39] = BSP_PRV_VECT_ENUM(EVENT_CAN0_MAILBOX_RX,FIXED), /* CAN0 MAILBOX RX (Reception complete interrupt) */
            [40] = BSP_PRV_VECT_ENUM(EVENT_CAN0_MAILBOX_TX,FIXED), /* CAN0 MAILBOX TX (Transmission complete interrupt) */
            [41] = BSP_PRV_VECT_ENUM(EVENT_CAN0_FIFO_RX,FIXED), /* CAN0 FIFO RX (Receive FIFO interrupt) */
            [42] = BSP_PRV_VECT_ENUM(EVENT_CAN0_FIFO_TX,FIXED), /* CAN0 FIFO TX (Transmit FIFO interrupt) */
            [43] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,FIXED), /* FCU FRDYI (Flash ready interrupt) */
            [44] = BSP_PRV_VECT_ENUM(EVENT_FCU_FIFERR,FIXED), /* FCU FIFERR (Flash access error interrupt) */
        };
        #endif
        #endif
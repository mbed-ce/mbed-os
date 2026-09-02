/* generated vector source file - do not edit */
        #include "bsp_api.h"
        /* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
        #if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = gpt_counter_overflow_isr, /* GPT1 COUNTER OVERFLOW (Overflow) */
            [1] = gpt_capture_compare_a_isr, /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
            [2] = r_icu_isr, /* ICU IRQ0 (External pin interrupt 0) */
            [3] = spi_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [4] = spi_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [5] = spi_eri_isr, /* SPI0 ERI (Error) */
            [6] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [7] = sci_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
            [8] = sci_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [9] = sci_uart_tei_isr, /* SCI0 TEI (Transmit end) */
            [10] = sci_uart_eri_isr, /* SCI0 ERI (Receive error) */
            [11] = r_icu_isr, /* ICU IRQ1 (External pin interrupt 1) */
            [12] = r_icu_isr, /* ICU IRQ2 (External pin interrupt 2) */
            [13] = r_icu_isr, /* ICU IRQ3 (External pin interrupt 3) */
            [14] = r_icu_isr, /* ICU IRQ4 (External pin interrupt 4) */
            [15] = r_icu_isr, /* ICU IRQ5 (External pin interrupt 5) */
            [16] = r_icu_isr, /* ICU IRQ6 (External pin interrupt 6) */
            [17] = r_icu_isr, /* ICU IRQ7 (External pin interrupt 7) */
            [18] = r_icu_isr, /* ICU IRQ8 (External pin interrupt 8) */
            [19] = r_icu_isr, /* ICU IRQ9 (External pin interrupt 9) */
            [20] = r_icu_isr, /* ICU IRQ10 (External pin interrupt 10) */
            [21] = iic_b_master_rxi_isr, /* IICB0 RXI (Receive) */
            [22] = iic_b_master_txi_isr, /* IICB0 TXI (Transmit) */
            [23] = iic_b_master_tei_isr, /* IICB0 TEI (Transmit end) */
            [24] = iic_b_master_eri_isr, /* IICB0 ERI (Error) */
            [25] = r_icu_isr, /* ICU IRQ11 (External pin interrupt 11) */
            [26] = r_icu_isr, /* ICU IRQ12 (External pin interrupt 12) */
            [27] = r_icu_isr, /* ICU IRQ13 (External pin interrupt 13) */
            [28] = r_icu_isr, /* ICU IRQ14 (External pin interrupt 14) */
            [29] = sci_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [30] = sci_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [31] = sci_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [32] = sci_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [33] = spi_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [34] = spi_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [35] = spi_eri_isr, /* SPI1 ERI (Error) */
            [36] = dmac_int_isr, /* DMAC1 INT (DMAC1 transfer end) */
            [37] = canfd_error_isr, /* CAN0 CHERR (Channel  error) */
            [38] = canfd_channel_tx_isr, /* CAN0 TX (Transmit interrupt) */
            [39] = canfd_common_fifo_rx_isr, /* CAN0 COMFRX (Common FIFO receive interrupt) */
            [40] = canfd_error_isr, /* CAN GLERR (Global error) */
            [41] = canfd_rx_fifo_isr, /* CAN RXF (Global receive FIFO interrupt) */
            [42] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [43] = fcu_fiferr_isr, /* FCU FIFERR (Flash access error interrupt) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_GPT1_COUNTER_OVERFLOW,GROUP0), /* GPT1 COUNTER OVERFLOW (Overflow) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_GPT1_CAPTURE_COMPARE_A,GROUP1), /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ0,GROUP2), /* ICU IRQ0 (External pin interrupt 0) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP3), /* SPI0 RXI (Receive buffer full) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP4), /* SPI0 TEI (Transmission complete event) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP5), /* SPI0 ERI (Error) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP6), /* DMAC0 INT (DMAC0 transfer end) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_SCI0_RXI,GROUP7), /* SCI0 RXI (Receive data full) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,GROUP0), /* SCI0 TXI (Transmit data empty) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,GROUP1), /* SCI0 TEI (Transmit end) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SCI0_ERI,GROUP2), /* SCI0 ERI (Receive error) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ1,GROUP3), /* ICU IRQ1 (External pin interrupt 1) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ2,GROUP4), /* ICU IRQ2 (External pin interrupt 2) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ3,GROUP5), /* ICU IRQ3 (External pin interrupt 3) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ4,GROUP6), /* ICU IRQ4 (External pin interrupt 4) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ5,GROUP7), /* ICU IRQ5 (External pin interrupt 5) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ6,GROUP0), /* ICU IRQ6 (External pin interrupt 6) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ7,GROUP1), /* ICU IRQ7 (External pin interrupt 7) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ8,GROUP2), /* ICU IRQ8 (External pin interrupt 8) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ9,GROUP3), /* ICU IRQ9 (External pin interrupt 9) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ10,GROUP4), /* ICU IRQ10 (External pin interrupt 10) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_IICB0_RXI,GROUP5), /* IICB0 RXI (Receive) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TXI,GROUP6), /* IICB0 TXI (Transmit) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TEI,GROUP7), /* IICB0 TEI (Transmit end) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_IICB0_ERI,GROUP0), /* IICB0 ERI (Error) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ11,GROUP1), /* ICU IRQ11 (External pin interrupt 11) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ12,GROUP2), /* ICU IRQ12 (External pin interrupt 12) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ13,GROUP3), /* ICU IRQ13 (External pin interrupt 13) */
            [28] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ14,GROUP4), /* ICU IRQ14 (External pin interrupt 14) */
            [29] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP5), /* SCI9 RXI (Receive data full) */
            [30] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP6), /* SCI9 TXI (Transmit data empty) */
            [31] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP7), /* SCI9 TEI (Transmit end) */
            [32] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,FIXED), /* SCI9 ERI (Receive error) */
            [33] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,FIXED), /* SPI1 RXI (Receive buffer full) */
            [34] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,FIXED), /* SPI1 TEI (Transmission complete event) */
            [35] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,FIXED), /* SPI1 ERI (Error) */
            [36] = BSP_PRV_VECT_ENUM(EVENT_DMAC1_INT,FIXED), /* DMAC1 INT (DMAC1 transfer end) */
            [37] = BSP_PRV_VECT_ENUM(EVENT_CAN0_CHERR,FIXED), /* CAN0 CHERR (Channel  error) */
            [38] = BSP_PRV_VECT_ENUM(EVENT_CAN0_TX,FIXED), /* CAN0 TX (Transmit interrupt) */
            [39] = BSP_PRV_VECT_ENUM(EVENT_CAN0_COMFRX,FIXED), /* CAN0 COMFRX (Common FIFO receive interrupt) */
            [40] = BSP_PRV_VECT_ENUM(EVENT_CAN_GLERR,FIXED), /* CAN GLERR (Global error) */
            [41] = BSP_PRV_VECT_ENUM(EVENT_CAN_RXF,FIXED), /* CAN RXF (Global receive FIFO interrupt) */
            [42] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,FIXED), /* FCU FRDYI (Flash ready interrupt) */
            [43] = BSP_PRV_VECT_ENUM(EVENT_FCU_FIFERR,FIXED), /* FCU FIFERR (Flash access error interrupt) */
        };
        #endif
        #endif
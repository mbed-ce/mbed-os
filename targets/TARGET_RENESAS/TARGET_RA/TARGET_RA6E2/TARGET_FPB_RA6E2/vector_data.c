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
            [4] = spi_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [5] = spi_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [6] = spi_eri_isr, /* SPI0 ERI (Error) */
            [7] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [8] = sci_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
            [9] = sci_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [10] = sci_uart_tei_isr, /* SCI0 TEI (Transmit end) */
            [11] = sci_uart_eri_isr, /* SCI0 ERI (Receive error) */
            [12] = r_icu_isr, /* ICU IRQ1 (External pin interrupt 1) */
            [13] = r_icu_isr, /* ICU IRQ2 (External pin interrupt 2) */
            [14] = r_icu_isr, /* ICU IRQ3 (External pin interrupt 3) */
            [15] = r_icu_isr, /* ICU IRQ4 (External pin interrupt 4) */
            [16] = r_icu_isr, /* ICU IRQ5 (External pin interrupt 5) */
            [17] = r_icu_isr, /* ICU IRQ6 (External pin interrupt 6) */
            [18] = r_icu_isr, /* ICU IRQ7 (External pin interrupt 7) */
            [19] = r_icu_isr, /* ICU IRQ8 (External pin interrupt 8) */
            [20] = r_icu_isr, /* ICU IRQ9 (External pin interrupt 9) */
            [21] = r_icu_isr, /* ICU IRQ10 (External pin interrupt 10) */
            [22] = iic_b_master_rxi_isr, /* IICB0 RXI (Receive) */
            [23] = iic_b_master_txi_isr, /* IICB0 TXI (Transmit) */
            [24] = iic_b_master_tei_isr, /* IICB0 TEI (Transmit end) */
            [25] = iic_b_master_eri_isr, /* IICB0 ERI (Error) */
            [26] = r_icu_isr, /* ICU IRQ11 (External pin interrupt 11) */
            [27] = r_icu_isr, /* ICU IRQ12 (External pin interrupt 12) */
            [28] = r_icu_isr, /* ICU IRQ13 (External pin interrupt 13) */
            [29] = r_icu_isr, /* ICU IRQ14 (External pin interrupt 14) */
            [30] = sci_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [31] = sci_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [32] = sci_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [33] = sci_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [34] = spi_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [35] = spi_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [36] = spi_eri_isr, /* SPI1 ERI (Error) */
            [37] = dmac_int_isr, /* DMAC1 INT (DMAC1 transfer end) */
            [38] = canfd_error_isr, /* CAN0 CHERR (Channel  error) */
            [39] = canfd_channel_tx_isr, /* CAN0 TX (Transmit interrupt) */
            [40] = canfd_common_fifo_rx_isr, /* CAN0 COMFRX (Common FIFO receive interrupt) */
            [41] = canfd_error_isr, /* CAN GLERR (Global error) */
            [42] = canfd_rx_fifo_isr, /* CAN RXF (Global receive FIFO interrupt) */
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
            [4] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP4), /* SPI0 RXI (Receive buffer full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP5), /* SPI0 TEI (Transmission complete event) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP6), /* SPI0 ERI (Error) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP7), /* DMAC0 INT (DMAC0 transfer end) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SCI0_RXI,GROUP0), /* SCI0 RXI (Receive data full) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,GROUP1), /* SCI0 TXI (Transmit data empty) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,GROUP2), /* SCI0 TEI (Transmit end) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_SCI0_ERI,GROUP3), /* SCI0 ERI (Receive error) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ1,GROUP4), /* ICU IRQ1 (External pin interrupt 1) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ2,GROUP5), /* ICU IRQ2 (External pin interrupt 2) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ3,GROUP6), /* ICU IRQ3 (External pin interrupt 3) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ4,GROUP7), /* ICU IRQ4 (External pin interrupt 4) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ5,GROUP0), /* ICU IRQ5 (External pin interrupt 5) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ6,GROUP1), /* ICU IRQ6 (External pin interrupt 6) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ7,GROUP2), /* ICU IRQ7 (External pin interrupt 7) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ8,GROUP3), /* ICU IRQ8 (External pin interrupt 8) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ9,GROUP4), /* ICU IRQ9 (External pin interrupt 9) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ10,GROUP5), /* ICU IRQ10 (External pin interrupt 10) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_IICB0_RXI,GROUP6), /* IICB0 RXI (Receive) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TXI,GROUP7), /* IICB0 TXI (Transmit) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TEI,GROUP0), /* IICB0 TEI (Transmit end) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_IICB0_ERI,GROUP1), /* IICB0 ERI (Error) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ11,GROUP2), /* ICU IRQ11 (External pin interrupt 11) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ12,GROUP3), /* ICU IRQ12 (External pin interrupt 12) */
            [28] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ13,GROUP4), /* ICU IRQ13 (External pin interrupt 13) */
            [29] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ14,GROUP5), /* ICU IRQ14 (External pin interrupt 14) */
            [30] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP6), /* SCI9 RXI (Receive data full) */
            [31] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP7), /* SCI9 TXI (Transmit data empty) */
            [32] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,FIXED), /* SCI9 TEI (Transmit end) */
            [33] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,FIXED), /* SCI9 ERI (Receive error) */
            [34] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,FIXED), /* SPI1 RXI (Receive buffer full) */
            [35] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,FIXED), /* SPI1 TEI (Transmission complete event) */
            [36] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,FIXED), /* SPI1 ERI (Error) */
            [37] = BSP_PRV_VECT_ENUM(EVENT_DMAC1_INT,FIXED), /* DMAC1 INT (DMAC1 transfer end) */
            [38] = BSP_PRV_VECT_ENUM(EVENT_CAN0_CHERR,FIXED), /* CAN0 CHERR (Channel  error) */
            [39] = BSP_PRV_VECT_ENUM(EVENT_CAN0_TX,FIXED), /* CAN0 TX (Transmit interrupt) */
            [40] = BSP_PRV_VECT_ENUM(EVENT_CAN0_COMFRX,FIXED), /* CAN0 COMFRX (Common FIFO receive interrupt) */
            [41] = BSP_PRV_VECT_ENUM(EVENT_CAN_GLERR,FIXED), /* CAN GLERR (Global error) */
            [42] = BSP_PRV_VECT_ENUM(EVENT_CAN_RXF,FIXED), /* CAN RXF (Global receive FIFO interrupt) */
            [43] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,FIXED), /* FCU FRDYI (Flash ready interrupt) */
            [44] = BSP_PRV_VECT_ENUM(EVENT_FCU_FIFERR,FIXED), /* FCU FIFERR (Flash access error interrupt) */
        };
        #endif
        #endif
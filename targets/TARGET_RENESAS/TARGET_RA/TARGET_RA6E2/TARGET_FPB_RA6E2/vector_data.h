/* Copyright (c) 2024 Renesas Electronics Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #ifdef __cplusplus
        extern "C" {
        #endif
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (45)
        #endif
        /* ISR prototypes */
        void rtc_carry_isr(void);
        void gpt_counter_overflow_isr(void);
        void gpt_capture_compare_a_isr(void);
        void r_icu_isr(void);
        void spi_rxi_isr(void);
        void spi_tei_isr(void);
        void spi_eri_isr(void);
        void dmac_int_isr(void);
        void sci_uart_rxi_isr(void);
        void sci_uart_txi_isr(void);
        void sci_uart_tei_isr(void);
        void sci_uart_eri_isr(void);
        void iic_b_master_rxi_isr(void);
        void iic_b_master_txi_isr(void);
        void iic_b_master_tei_isr(void);
        void iic_b_master_eri_isr(void);
        void canfd_error_isr(void);
        void canfd_channel_tx_isr(void);
        void canfd_common_fifo_rx_isr(void);
        void canfd_rx_fifo_isr(void);
        void fcu_frdyi_isr(void);
        void fcu_fiferr_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_RTC_CARRY ((IRQn_Type) 0) /* RTC CARRY (Carry interrupt) */
        #define RTC_CARRY_IRQn          ((IRQn_Type) 0) /* RTC CARRY (Carry interrupt) */
        #define VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW ((IRQn_Type) 1) /* GPT1 COUNTER OVERFLOW (Overflow) */
        #define GPT1_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 1) /* GPT1 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_A ((IRQn_Type) 2) /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
        #define GPT1_CAPTURE_COMPARE_A_IRQn          ((IRQn_Type) 2) /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
        #define VECTOR_NUMBER_ICU_IRQ0 ((IRQn_Type) 3) /* ICU IRQ0 (External pin interrupt 0) */
        #define ICU_IRQ0_IRQn          ((IRQn_Type) 3) /* ICU IRQ0 (External pin interrupt 0) */
        #define VECTOR_NUMBER_SPI0_RXI ((IRQn_Type) 4) /* SPI0 RXI (Receive buffer full) */
        #define SPI0_RXI_IRQn          ((IRQn_Type) 4) /* SPI0 RXI (Receive buffer full) */
        #define VECTOR_NUMBER_SPI0_TEI ((IRQn_Type) 5) /* SPI0 TEI (Transmission complete event) */
        #define SPI0_TEI_IRQn          ((IRQn_Type) 5) /* SPI0 TEI (Transmission complete event) */
        #define VECTOR_NUMBER_SPI0_ERI ((IRQn_Type) 6) /* SPI0 ERI (Error) */
        #define SPI0_ERI_IRQn          ((IRQn_Type) 6) /* SPI0 ERI (Error) */
        #define VECTOR_NUMBER_DMAC0_INT ((IRQn_Type) 7) /* DMAC0 INT (DMAC0 transfer end) */
        #define DMAC0_INT_IRQn          ((IRQn_Type) 7) /* DMAC0 INT (DMAC0 transfer end) */
        #define VECTOR_NUMBER_SCI0_RXI ((IRQn_Type) 8) /* SCI0 RXI (Receive data full) */
        #define SCI0_RXI_IRQn          ((IRQn_Type) 8) /* SCI0 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI0_TXI ((IRQn_Type) 9) /* SCI0 TXI (Transmit data empty) */
        #define SCI0_TXI_IRQn          ((IRQn_Type) 9) /* SCI0 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI0_TEI ((IRQn_Type) 10) /* SCI0 TEI (Transmit end) */
        #define SCI0_TEI_IRQn          ((IRQn_Type) 10) /* SCI0 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI0_ERI ((IRQn_Type) 11) /* SCI0 ERI (Receive error) */
        #define SCI0_ERI_IRQn          ((IRQn_Type) 11) /* SCI0 ERI (Receive error) */
        #define VECTOR_NUMBER_ICU_IRQ1 ((IRQn_Type) 12) /* ICU IRQ1 (External pin interrupt 1) */
        #define ICU_IRQ1_IRQn          ((IRQn_Type) 12) /* ICU IRQ1 (External pin interrupt 1) */
        #define VECTOR_NUMBER_ICU_IRQ2 ((IRQn_Type) 13) /* ICU IRQ2 (External pin interrupt 2) */
        #define ICU_IRQ2_IRQn          ((IRQn_Type) 13) /* ICU IRQ2 (External pin interrupt 2) */
        #define VECTOR_NUMBER_ICU_IRQ3 ((IRQn_Type) 14) /* ICU IRQ3 (External pin interrupt 3) */
        #define ICU_IRQ3_IRQn          ((IRQn_Type) 14) /* ICU IRQ3 (External pin interrupt 3) */
        #define VECTOR_NUMBER_ICU_IRQ4 ((IRQn_Type) 15) /* ICU IRQ4 (External pin interrupt 4) */
        #define ICU_IRQ4_IRQn          ((IRQn_Type) 15) /* ICU IRQ4 (External pin interrupt 4) */
        #define VECTOR_NUMBER_ICU_IRQ5 ((IRQn_Type) 16) /* ICU IRQ5 (External pin interrupt 5) */
        #define ICU_IRQ5_IRQn          ((IRQn_Type) 16) /* ICU IRQ5 (External pin interrupt 5) */
        #define VECTOR_NUMBER_ICU_IRQ6 ((IRQn_Type) 17) /* ICU IRQ6 (External pin interrupt 6) */
        #define ICU_IRQ6_IRQn          ((IRQn_Type) 17) /* ICU IRQ6 (External pin interrupt 6) */
        #define VECTOR_NUMBER_ICU_IRQ7 ((IRQn_Type) 18) /* ICU IRQ7 (External pin interrupt 7) */
        #define ICU_IRQ7_IRQn          ((IRQn_Type) 18) /* ICU IRQ7 (External pin interrupt 7) */
        #define VECTOR_NUMBER_ICU_IRQ8 ((IRQn_Type) 19) /* ICU IRQ8 (External pin interrupt 8) */
        #define ICU_IRQ8_IRQn          ((IRQn_Type) 19) /* ICU IRQ8 (External pin interrupt 8) */
        #define VECTOR_NUMBER_ICU_IRQ9 ((IRQn_Type) 20) /* ICU IRQ9 (External pin interrupt 9) */
        #define ICU_IRQ9_IRQn          ((IRQn_Type) 20) /* ICU IRQ9 (External pin interrupt 9) */
        #define VECTOR_NUMBER_ICU_IRQ10 ((IRQn_Type) 21) /* ICU IRQ10 (External pin interrupt 10) */
        #define ICU_IRQ10_IRQn          ((IRQn_Type) 21) /* ICU IRQ10 (External pin interrupt 10) */
        #define VECTOR_NUMBER_IICB0_RXI ((IRQn_Type) 22) /* IICB0 RXI (Receive) */
        #define IICB0_RXI_IRQn          ((IRQn_Type) 22) /* IICB0 RXI (Receive) */
        #define VECTOR_NUMBER_IICB0_TXI ((IRQn_Type) 23) /* IICB0 TXI (Transmit) */
        #define IICB0_TXI_IRQn          ((IRQn_Type) 23) /* IICB0 TXI (Transmit) */
        #define VECTOR_NUMBER_IICB0_TEI ((IRQn_Type) 24) /* IICB0 TEI (Transmit end) */
        #define IICB0_TEI_IRQn          ((IRQn_Type) 24) /* IICB0 TEI (Transmit end) */
        #define VECTOR_NUMBER_IICB0_ERI ((IRQn_Type) 25) /* IICB0 ERI (Error) */
        #define IICB0_ERI_IRQn          ((IRQn_Type) 25) /* IICB0 ERI (Error) */
        #define VECTOR_NUMBER_ICU_IRQ11 ((IRQn_Type) 26) /* ICU IRQ11 (External pin interrupt 11) */
        #define ICU_IRQ11_IRQn          ((IRQn_Type) 26) /* ICU IRQ11 (External pin interrupt 11) */
        #define VECTOR_NUMBER_ICU_IRQ12 ((IRQn_Type) 27) /* ICU IRQ12 (External pin interrupt 12) */
        #define ICU_IRQ12_IRQn          ((IRQn_Type) 27) /* ICU IRQ12 (External pin interrupt 12) */
        #define VECTOR_NUMBER_ICU_IRQ13 ((IRQn_Type) 28) /* ICU IRQ13 (External pin interrupt 13) */
        #define ICU_IRQ13_IRQn          ((IRQn_Type) 28) /* ICU IRQ13 (External pin interrupt 13) */
        #define VECTOR_NUMBER_ICU_IRQ14 ((IRQn_Type) 29) /* ICU IRQ14 (External pin interrupt 14) */
        #define ICU_IRQ14_IRQn          ((IRQn_Type) 29) /* ICU IRQ14 (External pin interrupt 14) */
        #define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 30) /* SCI9 RXI (Receive data full) */
        #define SCI9_RXI_IRQn          ((IRQn_Type) 30) /* SCI9 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 31) /* SCI9 TXI (Transmit data empty) */
        #define SCI9_TXI_IRQn          ((IRQn_Type) 31) /* SCI9 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 32) /* SCI9 TEI (Transmit end) */
        #define SCI9_TEI_IRQn          ((IRQn_Type) 32) /* SCI9 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 33) /* SCI9 ERI (Receive error) */
        #define SCI9_ERI_IRQn          ((IRQn_Type) 33) /* SCI9 ERI (Receive error) */
        #define VECTOR_NUMBER_SPI1_RXI ((IRQn_Type) 34) /* SPI1 RXI (Receive buffer full) */
        #define SPI1_RXI_IRQn          ((IRQn_Type) 34) /* SPI1 RXI (Receive buffer full) */
        #define VECTOR_NUMBER_SPI1_TEI ((IRQn_Type) 35) /* SPI1 TEI (Transmission complete event) */
        #define SPI1_TEI_IRQn          ((IRQn_Type) 35) /* SPI1 TEI (Transmission complete event) */
        #define VECTOR_NUMBER_SPI1_ERI ((IRQn_Type) 36) /* SPI1 ERI (Error) */
        #define SPI1_ERI_IRQn          ((IRQn_Type) 36) /* SPI1 ERI (Error) */
        #define VECTOR_NUMBER_DMAC1_INT ((IRQn_Type) 37) /* DMAC1 INT (DMAC1 transfer end) */
        #define DMAC1_INT_IRQn          ((IRQn_Type) 37) /* DMAC1 INT (DMAC1 transfer end) */
        #define VECTOR_NUMBER_CAN0_CHERR ((IRQn_Type) 38) /* CAN0 CHERR (Channel  error) */
        #define CAN0_CHERR_IRQn          ((IRQn_Type) 38) /* CAN0 CHERR (Channel  error) */
        #define VECTOR_NUMBER_CAN0_TX ((IRQn_Type) 39) /* CAN0 TX (Transmit interrupt) */
        #define CAN0_TX_IRQn          ((IRQn_Type) 39) /* CAN0 TX (Transmit interrupt) */
        #define VECTOR_NUMBER_CAN0_COMFRX ((IRQn_Type) 40) /* CAN0 COMFRX (Common FIFO receive interrupt) */
        #define CAN0_COMFRX_IRQn          ((IRQn_Type) 40) /* CAN0 COMFRX (Common FIFO receive interrupt) */
        #define VECTOR_NUMBER_CAN_GLERR ((IRQn_Type) 41) /* CAN GLERR (Global error) */
        #define CAN_GLERR_IRQn          ((IRQn_Type) 41) /* CAN GLERR (Global error) */
        #define VECTOR_NUMBER_CAN_RXF ((IRQn_Type) 42) /* CAN RXF (Global receive FIFO interrupt) */
        #define CAN_RXF_IRQn          ((IRQn_Type) 42) /* CAN RXF (Global receive FIFO interrupt) */
        #define VECTOR_NUMBER_FCU_FRDYI ((IRQn_Type) 43) /* FCU FRDYI (Flash ready interrupt) */
        #define FCU_FRDYI_IRQn          ((IRQn_Type) 43) /* FCU FRDYI (Flash ready interrupt) */
        #define VECTOR_NUMBER_FCU_FIFERR ((IRQn_Type) 44) /* FCU FIFERR (Flash access error interrupt) */
        #define FCU_FIFERR_IRQn          ((IRQn_Type) 44) /* FCU FIFERR (Flash access error interrupt) */
        /* The number of entries required for the ICU vector table. */
        #define BSP_ICU_VECTOR_NUM_ENTRIES (45)

        #ifdef __cplusplus
        }
        #endif
        #endif /* VECTOR_DATA_H */
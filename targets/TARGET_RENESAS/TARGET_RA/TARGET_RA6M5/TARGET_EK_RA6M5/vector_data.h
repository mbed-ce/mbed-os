/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #ifdef __cplusplus
        extern "C" {
        #endif
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (90)
        #endif
        /* ISR prototypes */
        void gpt_counter_overflow_isr(void);
        void gpt_capture_compare_a_isr(void);
        void r_icu_isr(void);
        void rtc_carry_isr(void);
        void iic_master_rxi_isr(void);
        void iic_master_txi_isr(void);
        void iic_master_tei_isr(void);
        void iic_master_eri_isr(void);
        void spi_rxi_isr(void);
        void spi_tei_isr(void);
        void spi_eri_isr(void);
        void dmac_int_isr(void);
        void sci_uart_rxi_isr(void);
        void sci_uart_txi_isr(void);
        void sci_uart_tei_isr(void);
        void sci_uart_eri_isr(void);
        void fcu_frdyi_isr(void);
        void fcu_fiferr_isr(void);
        void canfd_error_isr(void);
        void canfd_channel_tx_isr(void);
        void canfd_common_fifo_rx_isr(void);
        void canfd_rx_fifo_isr(void);
        void ether_eint_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW ((IRQn_Type) 0) /* GPT1 COUNTER OVERFLOW (Overflow) */
        #define GPT1_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 0) /* GPT1 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_A ((IRQn_Type) 1) /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
        #define GPT1_CAPTURE_COMPARE_A_IRQn          ((IRQn_Type) 1) /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
        #define VECTOR_NUMBER_ICU_IRQ0 ((IRQn_Type) 2) /* ICU IRQ0 (External pin interrupt 0) */
        #define ICU_IRQ0_IRQn          ((IRQn_Type) 2) /* ICU IRQ0 (External pin interrupt 0) */
        #define VECTOR_NUMBER_ICU_IRQ1 ((IRQn_Type) 3) /* ICU IRQ1 (External pin interrupt 1) */
        #define ICU_IRQ1_IRQn          ((IRQn_Type) 3) /* ICU IRQ1 (External pin interrupt 1) */
        #define VECTOR_NUMBER_ICU_IRQ2 ((IRQn_Type) 4) /* ICU IRQ2 (External pin interrupt 2) */
        #define ICU_IRQ2_IRQn          ((IRQn_Type) 4) /* ICU IRQ2 (External pin interrupt 2) */
        #define VECTOR_NUMBER_ICU_IRQ3 ((IRQn_Type) 5) /* ICU IRQ3 (External pin interrupt 3) */
        #define ICU_IRQ3_IRQn          ((IRQn_Type) 5) /* ICU IRQ3 (External pin interrupt 3) */
        #define VECTOR_NUMBER_ICU_IRQ4 ((IRQn_Type) 6) /* ICU IRQ4 (External pin interrupt 4) */
        #define ICU_IRQ4_IRQn          ((IRQn_Type) 6) /* ICU IRQ4 (External pin interrupt 4) */
        #define VECTOR_NUMBER_ICU_IRQ5 ((IRQn_Type) 7) /* ICU IRQ5 (External pin interrupt 5) */
        #define ICU_IRQ5_IRQn          ((IRQn_Type) 7) /* ICU IRQ5 (External pin interrupt 5) */
        #define VECTOR_NUMBER_ICU_IRQ6 ((IRQn_Type) 8) /* ICU IRQ6 (External pin interrupt 6) */
        #define ICU_IRQ6_IRQn          ((IRQn_Type) 8) /* ICU IRQ6 (External pin interrupt 6) */
        #define VECTOR_NUMBER_ICU_IRQ7 ((IRQn_Type) 9) /* ICU IRQ7 (External pin interrupt 7) */
        #define ICU_IRQ7_IRQn          ((IRQn_Type) 9) /* ICU IRQ7 (External pin interrupt 7) */
        #define VECTOR_NUMBER_ICU_IRQ8 ((IRQn_Type) 10) /* ICU IRQ8 (External pin interrupt 8) */
        #define ICU_IRQ8_IRQn          ((IRQn_Type) 10) /* ICU IRQ8 (External pin interrupt 8) */
        #define VECTOR_NUMBER_ICU_IRQ9 ((IRQn_Type) 11) /* ICU IRQ9 (External pin interrupt 9) */
        #define ICU_IRQ9_IRQn          ((IRQn_Type) 11) /* ICU IRQ9 (External pin interrupt 9) */
        #define VECTOR_NUMBER_ICU_IRQ10 ((IRQn_Type) 12) /* ICU IRQ10 (External pin interrupt 10) */
        #define ICU_IRQ10_IRQn          ((IRQn_Type) 12) /* ICU IRQ10 (External pin interrupt 10) */
        #define VECTOR_NUMBER_ICU_IRQ11 ((IRQn_Type) 13) /* ICU IRQ11 (External pin interrupt 11) */
        #define ICU_IRQ11_IRQn          ((IRQn_Type) 13) /* ICU IRQ11 (External pin interrupt 11) */
        #define VECTOR_NUMBER_ICU_IRQ12 ((IRQn_Type) 14) /* ICU IRQ12 (External pin interrupt 12) */
        #define ICU_IRQ12_IRQn          ((IRQn_Type) 14) /* ICU IRQ12 (External pin interrupt 12) */
        #define VECTOR_NUMBER_ICU_IRQ13 ((IRQn_Type) 15) /* ICU IRQ13 (External pin interrupt 13) */
        #define ICU_IRQ13_IRQn          ((IRQn_Type) 15) /* ICU IRQ13 (External pin interrupt 13) */
        #define VECTOR_NUMBER_ICU_IRQ14 ((IRQn_Type) 16) /* ICU IRQ14 (External pin interrupt 14) */
        #define ICU_IRQ14_IRQn          ((IRQn_Type) 16) /* ICU IRQ14 (External pin interrupt 14) */
        #define VECTOR_NUMBER_ICU_IRQ15 ((IRQn_Type) 17) /* ICU IRQ15 (External pin interrupt 15) */
        #define ICU_IRQ15_IRQn          ((IRQn_Type) 17) /* ICU IRQ15 (External pin interrupt 15) */
        #define VECTOR_NUMBER_RTC_CARRY ((IRQn_Type) 18) /* RTC CARRY (Carry interrupt) */
        #define RTC_CARRY_IRQn          ((IRQn_Type) 18) /* RTC CARRY (Carry interrupt) */
        #define VECTOR_NUMBER_IIC0_RXI ((IRQn_Type) 19) /* IIC0 RXI (Receive data full) */
        #define IIC0_RXI_IRQn          ((IRQn_Type) 19) /* IIC0 RXI (Receive data full) */
        #define VECTOR_NUMBER_IIC0_TXI ((IRQn_Type) 20) /* IIC0 TXI (Transmit data empty) */
        #define IIC0_TXI_IRQn          ((IRQn_Type) 20) /* IIC0 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_IIC0_TEI ((IRQn_Type) 21) /* IIC0 TEI (Transmit end) */
        #define IIC0_TEI_IRQn          ((IRQn_Type) 21) /* IIC0 TEI (Transmit end) */
        #define VECTOR_NUMBER_IIC0_ERI ((IRQn_Type) 22) /* IIC0 ERI (Transfer error) */
        #define IIC0_ERI_IRQn          ((IRQn_Type) 22) /* IIC0 ERI (Transfer error) */
        #define VECTOR_NUMBER_IIC1_RXI ((IRQn_Type) 23) /* IIC1 RXI (Receive data full) */
        #define IIC1_RXI_IRQn          ((IRQn_Type) 23) /* IIC1 RXI (Receive data full) */
        #define VECTOR_NUMBER_IIC1_TXI ((IRQn_Type) 24) /* IIC1 TXI (Transmit data empty) */
        #define IIC1_TXI_IRQn          ((IRQn_Type) 24) /* IIC1 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_IIC1_TEI ((IRQn_Type) 25) /* IIC1 TEI (Transmit end) */
        #define IIC1_TEI_IRQn          ((IRQn_Type) 25) /* IIC1 TEI (Transmit end) */
        #define VECTOR_NUMBER_IIC1_ERI ((IRQn_Type) 26) /* IIC1 ERI (Transfer error) */
        #define IIC1_ERI_IRQn          ((IRQn_Type) 26) /* IIC1 ERI (Transfer error) */
        #define VECTOR_NUMBER_IIC2_RXI ((IRQn_Type) 27) /* IIC2 RXI (Receive data full) */
        #define IIC2_RXI_IRQn          ((IRQn_Type) 27) /* IIC2 RXI (Receive data full) */
        #define VECTOR_NUMBER_IIC2_TXI ((IRQn_Type) 28) /* IIC2 TXI (Transmit data empty) */
        #define IIC2_TXI_IRQn          ((IRQn_Type) 28) /* IIC2 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_IIC2_TEI ((IRQn_Type) 29) /* IIC2 TEI (Transmit end) */
        #define IIC2_TEI_IRQn          ((IRQn_Type) 29) /* IIC2 TEI (Transmit end) */
        #define VECTOR_NUMBER_IIC2_ERI ((IRQn_Type) 30) /* IIC2 ERI (Transfer error) */
        #define IIC2_ERI_IRQn          ((IRQn_Type) 30) /* IIC2 ERI (Transfer error) */
        #define VECTOR_NUMBER_SPI0_RXI ((IRQn_Type) 31) /* SPI0 RXI (Receive buffer full) */
        #define SPI0_RXI_IRQn          ((IRQn_Type) 31) /* SPI0 RXI (Receive buffer full) */
        #define VECTOR_NUMBER_SPI0_TEI ((IRQn_Type) 32) /* SPI0 TEI (Transmission complete event) */
        #define SPI0_TEI_IRQn          ((IRQn_Type) 32) /* SPI0 TEI (Transmission complete event) */
        #define VECTOR_NUMBER_SPI0_ERI ((IRQn_Type) 33) /* SPI0 ERI (Error) */
        #define SPI0_ERI_IRQn          ((IRQn_Type) 33) /* SPI0 ERI (Error) */
        #define VECTOR_NUMBER_DMAC0_INT ((IRQn_Type) 34) /* DMAC0 INT (DMAC0 transfer end) */
        #define DMAC0_INT_IRQn          ((IRQn_Type) 34) /* DMAC0 INT (DMAC0 transfer end) */
        #define VECTOR_NUMBER_SPI1_RXI ((IRQn_Type) 35) /* SPI1 RXI (Receive buffer full) */
        #define SPI1_RXI_IRQn          ((IRQn_Type) 35) /* SPI1 RXI (Receive buffer full) */
        #define VECTOR_NUMBER_SPI1_TEI ((IRQn_Type) 36) /* SPI1 TEI (Transmission complete event) */
        #define SPI1_TEI_IRQn          ((IRQn_Type) 36) /* SPI1 TEI (Transmission complete event) */
        #define VECTOR_NUMBER_SPI1_ERI ((IRQn_Type) 37) /* SPI1 ERI (Error) */
        #define SPI1_ERI_IRQn          ((IRQn_Type) 37) /* SPI1 ERI (Error) */
        #define VECTOR_NUMBER_DMAC1_INT ((IRQn_Type) 38) /* DMAC1 INT (DMAC1 transfer end) */
        #define DMAC1_INT_IRQn          ((IRQn_Type) 38) /* DMAC1 INT (DMAC1 transfer end) */
        #define VECTOR_NUMBER_SCI0_RXI ((IRQn_Type) 39) /* SCI0 RXI (Receive data full) */
        #define SCI0_RXI_IRQn          ((IRQn_Type) 39) /* SCI0 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI0_TXI ((IRQn_Type) 40) /* SCI0 TXI (Transmit data empty) */
        #define SCI0_TXI_IRQn          ((IRQn_Type) 40) /* SCI0 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI0_TEI ((IRQn_Type) 41) /* SCI0 TEI (Transmit end) */
        #define SCI0_TEI_IRQn          ((IRQn_Type) 41) /* SCI0 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI0_ERI ((IRQn_Type) 42) /* SCI0 ERI (Receive error) */
        #define SCI0_ERI_IRQn          ((IRQn_Type) 42) /* SCI0 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI1_RXI ((IRQn_Type) 43) /* SCI1 RXI (Receive data full) */
        #define SCI1_RXI_IRQn          ((IRQn_Type) 43) /* SCI1 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI1_TXI ((IRQn_Type) 44) /* SCI1 TXI (Transmit data empty) */
        #define SCI1_TXI_IRQn          ((IRQn_Type) 44) /* SCI1 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI1_TEI ((IRQn_Type) 45) /* SCI1 TEI (Transmit end) */
        #define SCI1_TEI_IRQn          ((IRQn_Type) 45) /* SCI1 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI1_ERI ((IRQn_Type) 46) /* SCI1 ERI (Receive error) */
        #define SCI1_ERI_IRQn          ((IRQn_Type) 46) /* SCI1 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI2_RXI ((IRQn_Type) 47) /* SCI2 RXI (Receive data full) */
        #define SCI2_RXI_IRQn          ((IRQn_Type) 47) /* SCI2 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI2_TXI ((IRQn_Type) 48) /* SCI2 TXI (Transmit data empty) */
        #define SCI2_TXI_IRQn          ((IRQn_Type) 48) /* SCI2 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI2_TEI ((IRQn_Type) 49) /* SCI2 TEI (Transmit end) */
        #define SCI2_TEI_IRQn          ((IRQn_Type) 49) /* SCI2 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI2_ERI ((IRQn_Type) 50) /* SCI2 ERI (Receive error) */
        #define SCI2_ERI_IRQn          ((IRQn_Type) 50) /* SCI2 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI3_RXI ((IRQn_Type) 51) /* SCI3 RXI (Receive data full) */
        #define SCI3_RXI_IRQn          ((IRQn_Type) 51) /* SCI3 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI3_TXI ((IRQn_Type) 52) /* SCI3 TXI (Transmit data empty) */
        #define SCI3_TXI_IRQn          ((IRQn_Type) 52) /* SCI3 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI3_TEI ((IRQn_Type) 53) /* SCI3 TEI (Transmit end) */
        #define SCI3_TEI_IRQn          ((IRQn_Type) 53) /* SCI3 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI3_ERI ((IRQn_Type) 54) /* SCI3 ERI (Receive error) */
        #define SCI3_ERI_IRQn          ((IRQn_Type) 54) /* SCI3 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI4_RXI ((IRQn_Type) 55) /* SCI4 RXI (Receive data full) */
        #define SCI4_RXI_IRQn          ((IRQn_Type) 55) /* SCI4 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI4_TXI ((IRQn_Type) 56) /* SCI4 TXI (Transmit data empty) */
        #define SCI4_TXI_IRQn          ((IRQn_Type) 56) /* SCI4 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI4_TEI ((IRQn_Type) 57) /* SCI4 TEI (Transmit end) */
        #define SCI4_TEI_IRQn          ((IRQn_Type) 57) /* SCI4 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI4_ERI ((IRQn_Type) 58) /* SCI4 ERI (Receive error) */
        #define SCI4_ERI_IRQn          ((IRQn_Type) 58) /* SCI4 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI5_RXI ((IRQn_Type) 59) /* SCI5 RXI (Receive data full) */
        #define SCI5_RXI_IRQn          ((IRQn_Type) 59) /* SCI5 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI5_TXI ((IRQn_Type) 60) /* SCI5 TXI (Transmit data empty) */
        #define SCI5_TXI_IRQn          ((IRQn_Type) 60) /* SCI5 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI5_TEI ((IRQn_Type) 61) /* SCI5 TEI (Transmit end) */
        #define SCI5_TEI_IRQn          ((IRQn_Type) 61) /* SCI5 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI5_ERI ((IRQn_Type) 62) /* SCI5 ERI (Receive error) */
        #define SCI5_ERI_IRQn          ((IRQn_Type) 62) /* SCI5 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI6_RXI ((IRQn_Type) 63) /* SCI6 RXI (Receive data full) */
        #define SCI6_RXI_IRQn          ((IRQn_Type) 63) /* SCI6 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI6_TXI ((IRQn_Type) 64) /* SCI6 TXI (Transmit data empty) */
        #define SCI6_TXI_IRQn          ((IRQn_Type) 64) /* SCI6 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI6_TEI ((IRQn_Type) 65) /* SCI6 TEI (Transmit end) */
        #define SCI6_TEI_IRQn          ((IRQn_Type) 65) /* SCI6 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI6_ERI ((IRQn_Type) 66) /* SCI6 ERI (Receive error) */
        #define SCI6_ERI_IRQn          ((IRQn_Type) 66) /* SCI6 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI7_RXI ((IRQn_Type) 67) /* SCI7 RXI (Receive data full) */
        #define SCI7_RXI_IRQn          ((IRQn_Type) 67) /* SCI7 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI7_TXI ((IRQn_Type) 68) /* SCI7 TXI (Transmit data empty) */
        #define SCI7_TXI_IRQn          ((IRQn_Type) 68) /* SCI7 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI7_TEI ((IRQn_Type) 69) /* SCI7 TEI (Transmit end) */
        #define SCI7_TEI_IRQn          ((IRQn_Type) 69) /* SCI7 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI7_ERI ((IRQn_Type) 70) /* SCI7 ERI (Receive error) */
        #define SCI7_ERI_IRQn          ((IRQn_Type) 70) /* SCI7 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI8_RXI ((IRQn_Type) 71) /* SCI8 RXI (Receive data full) */
        #define SCI8_RXI_IRQn          ((IRQn_Type) 71) /* SCI8 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI8_TXI ((IRQn_Type) 72) /* SCI8 TXI (Transmit data empty) */
        #define SCI8_TXI_IRQn          ((IRQn_Type) 72) /* SCI8 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI8_TEI ((IRQn_Type) 73) /* SCI8 TEI (Transmit end) */
        #define SCI8_TEI_IRQn          ((IRQn_Type) 73) /* SCI8 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI8_ERI ((IRQn_Type) 74) /* SCI8 ERI (Receive error) */
        #define SCI8_ERI_IRQn          ((IRQn_Type) 74) /* SCI8 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 75) /* SCI9 RXI (Receive data full) */
        #define SCI9_RXI_IRQn          ((IRQn_Type) 75) /* SCI9 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 76) /* SCI9 TXI (Transmit data empty) */
        #define SCI9_TXI_IRQn          ((IRQn_Type) 76) /* SCI9 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 77) /* SCI9 TEI (Transmit end) */
        #define SCI9_TEI_IRQn          ((IRQn_Type) 77) /* SCI9 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 78) /* SCI9 ERI (Receive error) */
        #define SCI9_ERI_IRQn          ((IRQn_Type) 78) /* SCI9 ERI (Receive error) */
        #define VECTOR_NUMBER_FCU_FRDYI ((IRQn_Type) 79) /* FCU FRDYI (Flash ready interrupt) */
        #define FCU_FRDYI_IRQn          ((IRQn_Type) 79) /* FCU FRDYI (Flash ready interrupt) */
        #define VECTOR_NUMBER_FCU_FIFERR ((IRQn_Type) 80) /* FCU FIFERR (Flash access error interrupt) */
        #define FCU_FIFERR_IRQn          ((IRQn_Type) 80) /* FCU FIFERR (Flash access error interrupt) */
        #define VECTOR_NUMBER_CAN0_CHERR ((IRQn_Type) 81) /* CAN0 CHERR (Channel  error) */
        #define CAN0_CHERR_IRQn          ((IRQn_Type) 81) /* CAN0 CHERR (Channel  error) */
        #define VECTOR_NUMBER_CAN0_TX ((IRQn_Type) 82) /* CAN0 TX (Transmit interrupt) */
        #define CAN0_TX_IRQn          ((IRQn_Type) 82) /* CAN0 TX (Transmit interrupt) */
        #define VECTOR_NUMBER_CAN0_COMFRX ((IRQn_Type) 83) /* CAN0 COMFRX (Common FIFO receive interrupt) */
        #define CAN0_COMFRX_IRQn          ((IRQn_Type) 83) /* CAN0 COMFRX (Common FIFO receive interrupt) */
        #define VECTOR_NUMBER_CAN_GLERR ((IRQn_Type) 84) /* CAN GLERR (Global error) */
        #define CAN_GLERR_IRQn          ((IRQn_Type) 84) /* CAN GLERR (Global error) */
        #define VECTOR_NUMBER_CAN_RXF ((IRQn_Type) 85) /* CAN RXF (Global receive FIFO interrupt) */
        #define CAN_RXF_IRQn          ((IRQn_Type) 85) /* CAN RXF (Global receive FIFO interrupt) */
        #define VECTOR_NUMBER_CAN1_CHERR ((IRQn_Type) 86) /* CAN1 CHERR (Channel  error) */
        #define CAN1_CHERR_IRQn          ((IRQn_Type) 86) /* CAN1 CHERR (Channel  error) */
        #define VECTOR_NUMBER_CAN1_TX ((IRQn_Type) 87) /* CAN1 TX (Transmit interrupt) */
        #define CAN1_TX_IRQn          ((IRQn_Type) 87) /* CAN1 TX (Transmit interrupt) */
        #define VECTOR_NUMBER_CAN1_COMFRX ((IRQn_Type) 88) /* CAN1 COMFRX (Common FIFO receive interrupt) */
        #define CAN1_COMFRX_IRQn          ((IRQn_Type) 88) /* CAN1 COMFRX (Common FIFO receive interrupt) */
        #define VECTOR_NUMBER_EDMAC0_EINT ((IRQn_Type) 89) /* EDMAC0 EINT (EDMAC 0 interrupt) */
        #define EDMAC0_EINT_IRQn          ((IRQn_Type) 89) /* EDMAC0 EINT (EDMAC 0 interrupt) */
        /* The number of entries required for the ICU vector table. */
        #define BSP_ICU_VECTOR_NUM_ENTRIES (90)

        #ifdef __cplusplus
        }
        #endif
        #endif /* VECTOR_DATA_H */
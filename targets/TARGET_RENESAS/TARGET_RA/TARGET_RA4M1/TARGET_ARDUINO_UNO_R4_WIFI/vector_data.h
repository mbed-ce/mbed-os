/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #ifdef __cplusplus
        extern "C" {
        #endif
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (32)
        #endif
        /* ISR prototypes */
        void rtc_carry_isr(void);
        void gpt_counter_overflow_isr(void);
        void gpt_capture_compare_a_isr(void);
        void r_icu_isr(void);
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
        void usbfs_int_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_RTC_CARRY ((IRQn_Type) 0) /* RTC CARRY (Carry interrupt) */
        #define RTC_CARRY_IRQn          ((IRQn_Type) 0) /* RTC CARRY (Carry interrupt) */
        #define VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW ((IRQn_Type) 1) /* GPT1 COUNTER OVERFLOW (Overflow) */
        #define GPT1_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 1) /* GPT1 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_A ((IRQn_Type) 2) /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
        #define GPT1_CAPTURE_COMPARE_A_IRQn          ((IRQn_Type) 2) /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
        #define VECTOR_NUMBER_ICU_IRQ0 ((IRQn_Type) 3) /* ICU IRQ0 (External pin interrupt 0) */
        #define ICU_IRQ0_IRQn          ((IRQn_Type) 3) /* ICU IRQ0 (External pin interrupt 0) */
        #define VECTOR_NUMBER_IIC1_RXI ((IRQn_Type) 4) /* IIC1 RXI (Receive data full) */
        #define IIC1_RXI_IRQn          ((IRQn_Type) 4) /* IIC1 RXI (Receive data full) */
        #define VECTOR_NUMBER_IIC1_TXI ((IRQn_Type) 5) /* IIC1 TXI (Transmit data empty) */
        #define IIC1_TXI_IRQn          ((IRQn_Type) 5) /* IIC1 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_IIC1_TEI ((IRQn_Type) 6) /* IIC1 TEI (Transmit end) */
        #define IIC1_TEI_IRQn          ((IRQn_Type) 6) /* IIC1 TEI (Transmit end) */
        #define VECTOR_NUMBER_IIC1_ERI ((IRQn_Type) 7) /* IIC1 ERI (Transfer error) */
        #define IIC1_ERI_IRQn          ((IRQn_Type) 7) /* IIC1 ERI (Transfer error) */
        #define VECTOR_NUMBER_SPI0_RXI ((IRQn_Type) 8) /* SPI0 RXI (Receive buffer full) */
        #define SPI0_RXI_IRQn          ((IRQn_Type) 8) /* SPI0 RXI (Receive buffer full) */
        #define VECTOR_NUMBER_SPI0_TEI ((IRQn_Type) 9) /* SPI0 TEI (Transmission complete event) */
        #define SPI0_TEI_IRQn          ((IRQn_Type) 9) /* SPI0 TEI (Transmission complete event) */
        #define VECTOR_NUMBER_SPI0_ERI ((IRQn_Type) 10) /* SPI0 ERI (Error) */
        #define SPI0_ERI_IRQn          ((IRQn_Type) 10) /* SPI0 ERI (Error) */
        #define VECTOR_NUMBER_DMAC0_INT ((IRQn_Type) 11) /* DMAC0 INT (DMAC0 transfer end) */
        #define DMAC0_INT_IRQn          ((IRQn_Type) 11) /* DMAC0 INT (DMAC0 transfer end) */
        #define VECTOR_NUMBER_SCI1_RXI ((IRQn_Type) 12) /* SCI1 RXI (Receive data full) */
        #define SCI1_RXI_IRQn          ((IRQn_Type) 12) /* SCI1 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI1_TXI ((IRQn_Type) 13) /* SCI1 TXI (Transmit data empty) */
        #define SCI1_TXI_IRQn          ((IRQn_Type) 13) /* SCI1 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI1_TEI ((IRQn_Type) 14) /* SCI1 TEI (Transmit end) */
        #define SCI1_TEI_IRQn          ((IRQn_Type) 14) /* SCI1 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI1_ERI ((IRQn_Type) 15) /* SCI1 ERI (Receive error) */
        #define SCI1_ERI_IRQn          ((IRQn_Type) 15) /* SCI1 ERI (Receive error) */
        #define VECTOR_NUMBER_ICU_IRQ1 ((IRQn_Type) 16) /* ICU IRQ1 (External pin interrupt 1) */
        #define ICU_IRQ1_IRQn          ((IRQn_Type) 16) /* ICU IRQ1 (External pin interrupt 1) */
        #define VECTOR_NUMBER_ICU_IRQ2 ((IRQn_Type) 17) /* ICU IRQ2 (External pin interrupt 2) */
        #define ICU_IRQ2_IRQn          ((IRQn_Type) 17) /* ICU IRQ2 (External pin interrupt 2) */
        #define VECTOR_NUMBER_ICU_IRQ3 ((IRQn_Type) 18) /* ICU IRQ3 (External pin interrupt 3) */
        #define ICU_IRQ3_IRQn          ((IRQn_Type) 18) /* ICU IRQ3 (External pin interrupt 3) */
        #define VECTOR_NUMBER_SCI2_RXI ((IRQn_Type) 19) /* SCI2 RXI (Receive data full) */
        #define SCI2_RXI_IRQn          ((IRQn_Type) 19) /* SCI2 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI2_TXI ((IRQn_Type) 20) /* SCI2 TXI (Transmit data empty) */
        #define SCI2_TXI_IRQn          ((IRQn_Type) 20) /* SCI2 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI2_TEI ((IRQn_Type) 21) /* SCI2 TEI (Transmit end) */
        #define SCI2_TEI_IRQn          ((IRQn_Type) 21) /* SCI2 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI2_ERI ((IRQn_Type) 22) /* SCI2 ERI (Receive error) */
        #define SCI2_ERI_IRQn          ((IRQn_Type) 22) /* SCI2 ERI (Receive error) */
        #define VECTOR_NUMBER_USBFS_INT ((IRQn_Type) 23) /* USBFS INT (USBFS interrupt) */
        #define USBFS_INT_IRQn          ((IRQn_Type) 23) /* USBFS INT (USBFS interrupt) */
        /* SCI9 = UART to ESP32-S3 USB bridge (P109/P110). CAN0 vectors were
         * removed to make room: the CAN0 pins (P109/P110) are wired to the
         * ESP32-S3 on this board, so CAN is not usable anyway. */
        #define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 24) /* SCI9 RXI (Receive data full) */
        #define SCI9_RXI_IRQn          ((IRQn_Type) 24) /* SCI9 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 25) /* SCI9 TXI (Transmit data empty) */
        #define SCI9_TXI_IRQn          ((IRQn_Type) 25) /* SCI9 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 26) /* SCI9 TEI (Transmit end) */
        #define SCI9_TEI_IRQn          ((IRQn_Type) 26) /* SCI9 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 27) /* SCI9 ERI (Receive error) */
        #define SCI9_ERI_IRQn          ((IRQn_Type) 27) /* SCI9 ERI (Receive error) */
        /* The number of entries required for the ICU vector table. */
        #define BSP_ICU_VECTOR_NUM_ENTRIES (32)

        #ifdef __cplusplus
        }
        #endif
        #endif /* VECTOR_DATA_H */
        /* generated vector source file - do not edit */
        #include "bsp_api.h"
        #include "r_ssi.h"
        extern void ssi_txi_isr(void);
        extern void ssi_int_isr(void);
        /* r_ipc.c (FSP IPC driver, used by rpmsg-lite for the CPU0<->CPU1 doorbell) */
        extern void ipc_isr(void);
        /* The ICU supports 32 external IRQ channels but only 12 vector table
         * entries are reserved for them. The entries below form the external
         * IRQ slot pool: each slot is linked to an ICU IRQ channel at runtime
         * through IELSR by the allocator in TARGET_RA8P1/gpio_irq_device.c,
         * so their g_interrupt_event_link_select entries stay 0 (unlinked).
         * The pool slots are: 3, 8, 9, 10, 11, 12, 13, 14, 17, 22, 23, 24. */
        /* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
        #if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = rtc_carry_isr, /* RTC CARRY (Carry interrupt) */
            [1] = gpt_counter_overflow_isr, /* GPT1 COUNTER OVERFLOW (Overflow) */
            [2] = gpt_capture_compare_a_isr, /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
            [3] = r_icu_isr, /* External IRQ slot 0 (dynamically linked to any ICU IRQ channel) */
            [4] = iic_master_rxi_isr, /* IIC0 RXI (Receive data full) */
            [5] = iic_master_txi_isr, /* IIC0 TXI (Transmit data empty) */
            [6] = iic_master_tei_isr, /* IIC0 TEI (Transmit end) */
            [7] = iic_master_eri_isr, /* IIC0 ERI (Transfer error) */
            [8] = r_icu_isr, /* External IRQ slot 1 (dynamically linked to any ICU IRQ channel) */
            [9] = r_icu_isr, /* External IRQ slot 2 (dynamically linked to any ICU IRQ channel) */
            [10] = r_icu_isr, /* External IRQ slot 3 (dynamically linked to any ICU IRQ channel) */
            [11] = r_icu_isr, /* External IRQ slot 4 (dynamically linked to any ICU IRQ channel) */
            [12] = r_icu_isr, /* External IRQ slot 5 (dynamically linked to any ICU IRQ channel) */
            [13] = r_icu_isr, /* External IRQ slot 6 (dynamically linked to any ICU IRQ channel) */
            [14] = r_icu_isr, /* External IRQ slot 7 (dynamically linked to any ICU IRQ channel) */
            [15] = ssi_txi_isr, /* SSI0 TXI (Transmit data empty) */
            [16] = ssi_int_isr, /* SSI0 INT (Idle/error) */
            [17] = r_icu_isr, /* External IRQ slot 8 (dynamically linked to any ICU IRQ channel) */
            [18] = spi_b_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [19] = spi_b_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [20] = spi_b_eri_isr, /* SPI0 ERI (Error) */
            [21] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [25] = ipc_isr, /* IPC IRQ1 (CPU mutual interrupt 1: doorbell from CPU1, used by rpmsg-lite) */
            [22] = r_icu_isr, /* External IRQ slot 9 (dynamically linked to any ICU IRQ channel) */
            [23] = r_icu_isr, /* External IRQ slot 10 (dynamically linked to any ICU IRQ channel) */
            [24] = r_icu_isr, /* External IRQ slot 11 (dynamically linked to any ICU IRQ channel) */
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
            [67] = adc_b_calend0_isr, /* ADC CALEND0 (End of calibration of A/D converter unit 0) */
            [68] = adc_b_calend1_isr, /* ADC CALEND1 (End of calibration of A/D converter unit 1) */
            [69] = sci_b_uart_rxi_isr, /* SCI5 RXI (Receive data full) */
            [70] = sci_b_uart_txi_isr, /* SCI5 TXI (Transmit data empty) */
            [71] = sci_b_uart_tei_isr, /* SCI5 TEI (Transmit end) */
            [72] = sci_b_uart_eri_isr, /* SCI5 ERI (Receive error) */
            [73] = sci_b_uart_rxi_isr, /* SCI6 RXI (Receive data full) */
            [74] = sci_b_uart_txi_isr, /* SCI6 TXI (Transmit data empty) */
            [75] = sci_b_uart_tei_isr, /* SCI6 TEI (Transmit end) */
            [76] = sci_b_uart_eri_isr, /* SCI6 ERI (Receive error) */
            [77] = sci_b_uart_rxi_isr, /* SCI7 RXI (Receive data full) */
            [78] = sci_b_uart_txi_isr, /* SCI7 TXI (Transmit data empty) */
            [79] = sci_b_uart_tei_isr, /* SCI7 TEI (Transmit end) */
            [80] = sci_b_uart_eri_isr, /* SCI7 ERI (Receive error) */
            [81] = sci_b_uart_rxi_isr, /* SCI8 RXI (Receive data full) */
            [82] = sci_b_uart_txi_isr, /* SCI8 TXI (Transmit data empty) */
            [83] = sci_b_uart_tei_isr, /* SCI8 TEI (Transmit end) */
            [84] = sci_b_uart_eri_isr, /* SCI8 ERI (Receive error) */
            [86] = iic_master_rxi_isr, /* IIC2 RXI (Receive data full) */
            [87] = iic_master_txi_isr, /* IIC2 TXI (Transmit data empty) */
            [88] = iic_master_tei_isr, /* IIC2 TEI (Transmit end) */
            [89] = iic_master_eri_isr, /* IIC2 ERI (Transfer error) */
            [90] = sdhimmc_accs_isr, /* SDHIMMC0 ACCS (Card access) */
            [91] = sdhimmc_card_isr, /* SDHIMMC0 CARD (Card detect) */
            [92] = dmac_int_isr, /* DMAC3 INT (DMAC3 transfer end) */
            [93] = layer3_switch_gwdi_isr, /* ETHER GWDI0 (GWCA Data Interrupt 0) */
            [94] = layer3_switch_eaei_isr, /* ETHER EAEI0 (ETHA0 Error Interrupt) */
            [95] = layer3_switch_eaei_isr, /* ETHER EAEI1 (ETHA1 Error Interrupt) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_RTC_CARRY,GROUP0), /* RTC CARRY (Carry interrupt) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_GPT1_COUNTER_OVERFLOW,GROUP1), /* GPT1 COUNTER OVERFLOW (Overflow) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_GPT1_CAPTURE_COMPARE_A,GROUP2), /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
            /* [3],[8]-[14],[17],[22]-[24]: external IRQ slot pool, linked at
             * runtime by gpio_irq_device.c - left unlinked (0) here. */
            [4] = BSP_PRV_VECT_ENUM(EVENT_IIC0_RXI,GROUP4), /* IIC0 RXI (Receive data full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TXI,GROUP5), /* IIC0 TXI (Transmit data empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TEI,GROUP6), /* IIC0 TEI (Transmit end) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_IIC0_ERI,GROUP7), /* IIC0 ERI (Transfer error) */
 [15] = BSP_PRV_VECT_ENUM(EVENT_SSI0_TXI,GROUP7), /* SSI0 TXI (Transmit data empty) */
 [16] = BSP_PRV_VECT_ENUM(EVENT_SSI0_INT,GROUP0), /* SSI0 INT (Idle/error) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP2), /* SPI0 RXI (Receive buffer full) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP3), /* SPI0 TEI (Transmission complete event) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP4), /* SPI0 ERI (Error) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP5), /* DMAC0 INT (DMAC0 transfer end) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_IPC_IRQ1,GROUP25), /* IPC IRQ1 (CPU mutual interrupt 1, doorbell from CPU1) */
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
            [67] = BSP_PRV_VECT_ENUM(EVENT_ADC_CALEND0,FIXED), /* ADC CALEND0 (End of calibration of A/D converter unit 0) */
            [68] = BSP_PRV_VECT_ENUM(EVENT_ADC_CALEND1,FIXED), /* ADC CALEND1 (End of calibration of A/D converter unit 1) */
            [69] = BSP_PRV_VECT_ENUM(EVENT_SCI5_RXI,FIXED), /* SCI5 RXI (Receive data full) */
            [70] = BSP_PRV_VECT_ENUM(EVENT_SCI5_TXI,FIXED), /* SCI5 TXI (Transmit data empty) */
            [71] = BSP_PRV_VECT_ENUM(EVENT_SCI5_TEI,FIXED), /* SCI5 TEI (Transmit end) */
            [72] = BSP_PRV_VECT_ENUM(EVENT_SCI5_ERI,FIXED), /* SCI5 ERI (Receive error) */
            [73] = BSP_PRV_VECT_ENUM(EVENT_SCI6_RXI,FIXED), /* SCI6 RXI (Receive data full) */
            [74] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TXI,FIXED), /* SCI6 TXI (Transmit data empty) */
            [75] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TEI,FIXED), /* SCI6 TEI (Transmit end) */
            [76] = BSP_PRV_VECT_ENUM(EVENT_SCI6_ERI,FIXED), /* SCI6 ERI (Receive error) */
            [77] = BSP_PRV_VECT_ENUM(EVENT_SCI7_RXI,FIXED), /* SCI7 RXI (Receive data full) */
            [78] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TXI,FIXED), /* SCI7 TXI (Transmit data empty) */
            [79] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TEI,FIXED), /* SCI7 TEI (Transmit end) */
            [80] = BSP_PRV_VECT_ENUM(EVENT_SCI7_ERI,FIXED), /* SCI7 ERI (Receive error) */
            [81] = BSP_PRV_VECT_ENUM(EVENT_SCI8_RXI,FIXED), /* SCI8 RXI (Receive data full) */
            [82] = BSP_PRV_VECT_ENUM(EVENT_SCI8_TXI,FIXED), /* SCI8 TXI (Transmit data empty) */
            [83] = BSP_PRV_VECT_ENUM(EVENT_SCI8_TEI,FIXED), /* SCI8 TEI (Transmit end) */
            [84] = BSP_PRV_VECT_ENUM(EVENT_SCI8_ERI,FIXED), /* SCI8 ERI (Receive error) */
            [86] = BSP_PRV_VECT_ENUM(EVENT_IIC2_RXI,GROUP3), /* IIC2 RXI (Receive data full) */
            [87] = BSP_PRV_VECT_ENUM(EVENT_IIC2_TXI,GROUP4), /* IIC2 TXI (Transmit data empty) */
            [88] = BSP_PRV_VECT_ENUM(EVENT_IIC2_TEI,GROUP5), /* IIC2 TEI (Transmit end) */
            [89] = BSP_PRV_VECT_ENUM(EVENT_IIC2_ERI,GROUP6), /* IIC2 ERI (Transfer error) */
            [90] = BSP_PRV_VECT_ENUM(EVENT_SDHIMMC0_ACCS,GROUP3), /* SDHIMMC0 ACCS (Card access) */
            [91] = BSP_PRV_VECT_ENUM(EVENT_SDHIMMC0_CARD,GROUP4), /* SDHIMMC0 CARD (Card detect) */
            [92] = BSP_PRV_VECT_ENUM(EVENT_DMAC3_INT,GROUP5), /* DMAC3 INT (DMAC3 transfer end) */
            [93] = BSP_PRV_VECT_ENUM(EVENT_ETHER_GWDI0,GROUP6), /* ETHER GWDI0 (GWCA Data Interrupt 0) */
            [94] = BSP_PRV_VECT_ENUM(EVENT_ETHER_EAEI0,GROUP7), /* ETHER EAEI0 (ETHA0 Error Interrupt) */
            [95] = BSP_PRV_VECT_ENUM(EVENT_ETHER_EAEI1,GROUP0), /* ETHER EAEI1 (ETHA1 Error Interrupt) */
        };
        #endif
        #endif
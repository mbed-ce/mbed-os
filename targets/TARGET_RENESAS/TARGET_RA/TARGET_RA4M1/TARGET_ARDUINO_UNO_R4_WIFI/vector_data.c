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
            [4] = iic_master_rxi_isr, /* IIC1 RXI (Receive data full) */
            [5] = iic_master_txi_isr, /* IIC1 TXI (Transmit data empty) */
            [6] = iic_master_tei_isr, /* IIC1 TEI (Transmit end) */
            [7] = iic_master_eri_isr, /* IIC1 ERI (Transfer error) */
            [8] = spi_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [9] = spi_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [10] = spi_eri_isr, /* SPI0 ERI (Error) */
            [11] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [12] = sci_uart_rxi_isr, /* SCI1 RXI (Receive data full) */
            [13] = sci_uart_txi_isr, /* SCI1 TXI (Transmit data empty) */
            [14] = sci_uart_tei_isr, /* SCI1 TEI (Transmit end) */
            [15] = sci_uart_eri_isr, /* SCI1 ERI (Receive error) */
            [16] = r_icu_isr, /* ICU IRQ1 (External pin interrupt 1) */
            [17] = r_icu_isr, /* ICU IRQ2 (External pin interrupt 2) */
            [18] = r_icu_isr, /* ICU IRQ3 (External pin interrupt 3) */
            [19] = sci_uart_rxi_isr, /* SCI2 RXI (Receive data full) */
            [20] = sci_uart_txi_isr, /* SCI2 TXI (Transmit data empty) */
            [21] = sci_uart_tei_isr, /* SCI2 TEI (Transmit end) */
            [22] = sci_uart_eri_isr, /* SCI2 ERI (Receive error) */
            [23] = usbfs_int_isr, /* USBFS INT (USBFS interrupt) */
            [24] = sci_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [25] = sci_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [26] = sci_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [27] = sci_uart_eri_isr, /* SCI9 ERI (Receive error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_RTC_CARRY,GROUP0), /* RTC CARRY (Carry interrupt) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_GPT1_COUNTER_OVERFLOW,GROUP1), /* GPT1 COUNTER OVERFLOW (Overflow) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_GPT1_CAPTURE_COMPARE_A,GROUP2), /* GPT1 CAPTURE COMPARE A (Capture/Compare match A) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ0,GROUP3), /* ICU IRQ0 (External pin interrupt 0) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_IIC1_RXI,GROUP4), /* IIC1 RXI (Receive data full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_IIC1_TXI,GROUP5), /* IIC1 TXI (Transmit data empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_IIC1_TEI,GROUP6), /* IIC1 TEI (Transmit end) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_IIC1_ERI,GROUP7), /* IIC1 ERI (Transfer error) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP0), /* SPI0 RXI (Receive buffer full) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP1), /* SPI0 TEI (Transmission complete event) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP2), /* SPI0 ERI (Error) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP3), /* DMAC0 INT (DMAC0 transfer end) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_SCI1_RXI,GROUP4), /* SCI1 RXI (Receive data full) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TXI,GROUP5), /* SCI1 TXI (Transmit data empty) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TEI,GROUP6), /* SCI1 TEI (Transmit end) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_SCI1_ERI,GROUP7), /* SCI1 ERI (Receive error) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ1,GROUP0), /* ICU IRQ1 (External pin interrupt 1) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ2,GROUP1), /* ICU IRQ2 (External pin interrupt 2) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ3,GROUP2), /* ICU IRQ3 (External pin interrupt 3) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_SCI2_RXI,GROUP3), /* SCI2 RXI (Receive data full) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TXI,GROUP4), /* SCI2 TXI (Transmit data empty) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TEI,GROUP5), /* SCI2 TEI (Transmit end) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_SCI2_ERI,GROUP6), /* SCI2 ERI (Receive error) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_USBFS_INT,GROUP7), /* USBFS INT (USBFS interrupt) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP0), /* SCI9 RXI (Receive data full) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP1), /* SCI9 TXI (Transmit data empty) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP2), /* SCI9 TEI (Transmit end) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP3), /* SCI9 ERI (Receive error) */
        };
        #endif
        #endif
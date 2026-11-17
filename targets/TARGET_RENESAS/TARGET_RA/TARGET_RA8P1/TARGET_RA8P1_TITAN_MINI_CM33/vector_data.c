/* Cortex-M33 (CPU1) ICU vector table of the RA8P1 Titan Mini.
 * See vector_data.h in this folder for the layout description. */
/* generated vector source file - do not edit */
#include "bsp_api.h"

#if VECTOR_DATA_IRQ_COUNT > 0
BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
{
    [0] = ipc_isr, /* IPC IRQ0 (CPU mutual interrupt 0: doorbell from CPU0, used by rpmsg-lite) */
    [1] = gpt_counter_overflow_isr, /* GPT12 COUNTER OVERFLOW (us_ticker) */
    [2] = gpt_capture_compare_a_isr, /* GPT12 CAPTURE COMPARE A (us_ticker) */
    [3] = r_icu_isr, /* External IRQ slot 0 (dynamically linked to any ICU IRQ channel) */
    [4] = r_icu_isr, /* External IRQ slot 1 (dynamically linked to any ICU IRQ channel) */
    [5] = r_icu_isr, /* External IRQ slot 2 (dynamically linked to any ICU IRQ channel) */
    [6] = r_icu_isr, /* External IRQ slot 3 (dynamically linked to any ICU IRQ channel) */
    [7] = r_icu_isr, /* External IRQ slot 4 (dynamically linked to any ICU IRQ channel) */
    [8] = r_icu_isr, /* External IRQ slot 5 (dynamically linked to any ICU IRQ channel) */
    [9] = r_icu_isr, /* External IRQ slot 6 (dynamically linked to any ICU IRQ channel) */
    [10] = r_icu_isr, /* External IRQ slot 7 (dynamically linked to any ICU IRQ channel) */
    [11] = sci_b_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
    [12] = sci_b_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
    [13] = sci_b_uart_tei_isr, /* SCI0 TEI (Transmit end) */
    [14] = sci_b_uart_eri_isr, /* SCI0 ERI (Receive error) */
    [15] = sci_b_uart_rxi_isr, /* SCI1 RXI (Receive data full) */
    [16] = sci_b_uart_txi_isr, /* SCI1 TXI (Transmit data empty) */
    [17] = sci_b_uart_tei_isr, /* SCI1 TEI (Transmit end) */
    [18] = sci_b_uart_eri_isr, /* SCI1 ERI (Receive error) */
    [19] = sci_b_uart_rxi_isr, /* SCI2 RXI (Receive data full) */
    [20] = sci_b_uart_txi_isr, /* SCI2 TXI (Transmit data empty) */
    [21] = sci_b_uart_tei_isr, /* SCI2 TEI (Transmit end) */
    [22] = sci_b_uart_eri_isr, /* SCI2 ERI (Receive error) */
    [23] = sci_b_uart_rxi_isr, /* SCI3 RXI (Receive data full) */
    [24] = sci_b_uart_txi_isr, /* SCI3 TXI (Transmit data empty) */
    [25] = sci_b_uart_tei_isr, /* SCI3 TEI (Transmit end) */
    [26] = sci_b_uart_eri_isr, /* SCI3 ERI (Receive error) */
    [27] = sci_b_uart_rxi_isr, /* SCI4 RXI (Receive data full) */
    [28] = sci_b_uart_txi_isr, /* SCI4 TXI (Transmit data empty) */
    [29] = sci_b_uart_tei_isr, /* SCI4 TEI (Transmit end) */
    [30] = sci_b_uart_eri_isr, /* SCI4 ERI (Receive error) */
    [31] = sci_b_uart_rxi_isr, /* SCI5 RXI (Receive data full) */
    [32] = sci_b_uart_txi_isr, /* SCI5 TXI (Transmit data empty) */
    [33] = sci_b_uart_tei_isr, /* SCI5 TEI (Transmit end) */
    [34] = sci_b_uart_eri_isr, /* SCI5 ERI (Receive error) */
    [35] = sci_b_uart_rxi_isr, /* SCI6 RXI (Receive data full) */
    [36] = sci_b_uart_txi_isr, /* SCI6 TXI (Transmit data empty) */
    [37] = sci_b_uart_tei_isr, /* SCI6 TEI (Transmit end) */
    [38] = sci_b_uart_eri_isr, /* SCI6 ERI (Receive error) */
    [39] = sci_b_uart_rxi_isr, /* SCI7 RXI (Receive data full) */
    [40] = sci_b_uart_txi_isr, /* SCI7 TXI (Transmit data empty) */
    [41] = sci_b_uart_tei_isr, /* SCI7 TEI (Transmit end) */
    [42] = sci_b_uart_eri_isr, /* SCI7 ERI (Receive error) */
    [43] = sci_b_uart_rxi_isr, /* SCI8 RXI (Receive data full) */
    [44] = sci_b_uart_txi_isr, /* SCI8 TXI (Transmit data empty) */
    [45] = sci_b_uart_tei_isr, /* SCI8 TEI (Transmit end) */
    [46] = sci_b_uart_eri_isr, /* SCI8 ERI (Receive error) */
    [47] = sci_b_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
    [48] = sci_b_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
    [49] = sci_b_uart_tei_isr, /* SCI9 TEI (Transmit end) */
    [50] = sci_b_uart_eri_isr, /* SCI9 ERI (Receive error) */
    [51] = iic_master_rxi_isr, /* IIC0 RXI (Receive data full) */
    [52] = iic_master_txi_isr, /* IIC0 TXI (Transmit data empty) */
    [53] = iic_master_tei_isr, /* IIC0 TEI (Transmit end) */
    [54] = iic_master_eri_isr, /* IIC0 ERI (Transfer error) */
    [55] = iic_master_rxi_isr, /* IIC1 RXI (Receive data full) */
    [56] = iic_master_txi_isr, /* IIC1 TXI (Transmit data empty) */
    [57] = iic_master_tei_isr, /* IIC1 TEI (Transmit end) */
    [58] = iic_master_eri_isr, /* IIC1 ERI (Transfer error) */
    [59] = iic_master_rxi_isr, /* IIC2 RXI (Receive data full) */
    [60] = iic_master_txi_isr, /* IIC2 TXI (Transmit data empty) */
    [61] = iic_master_tei_isr, /* IIC2 TEI (Transmit end) */
    [62] = iic_master_eri_isr, /* IIC2 ERI (Transfer error) */
    [63] = spi_b_rxi_isr, /* SPI0 RXI (Receive buffer full) */
    [64] = spi_b_tei_isr, /* SPI0 TEI (Transmission complete event) */
    [65] = spi_b_eri_isr, /* SPI0 ERI (Error) */
    [66] = spi_b_rxi_isr, /* SPI1 RXI (Receive buffer full) */
    [67] = spi_b_tei_isr, /* SPI1 TEI (Transmission complete event) */
    [68] = spi_b_eri_isr, /* SPI1 ERI (Error) */
};
#if BSP_FEATURE_ICU_HAS_IELSR
const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
{
    [0] = BSP_PRV_VECT_ENUM(EVENT_IPC_IRQ0,GROUP0), /* IPC IRQ0 (CPU mutual interrupt 0) */
    [1] = BSP_PRV_VECT_ENUM(EVENT_GPT12_COUNTER_OVERFLOW,GROUP1), /* GPT12 COUNTER OVERFLOW (Overflow) */
    [2] = BSP_PRV_VECT_ENUM(EVENT_GPT12_CAPTURE_COMPARE_A,GROUP2), /* GPT12 CAPTURE COMPARE A (Capture/Compare match A) */
    /* [3]-[10]: external IRQ slot pool, linked at runtime by
     * gpio_irq_device.c - left unlinked (0) here. */
    [11] = BSP_PRV_VECT_ENUM(EVENT_SCI0_RXI,GROUP3), /* SCI0 RXI (Receive data full) */
    [12] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,GROUP4), /* SCI0 TXI (Transmit data empty) */
    [13] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,GROUP5), /* SCI0 TEI (Transmit end) */
    [14] = BSP_PRV_VECT_ENUM(EVENT_SCI0_ERI,GROUP6), /* SCI0 ERI (Receive error) */
    [15] = BSP_PRV_VECT_ENUM(EVENT_SCI1_RXI,GROUP7), /* SCI1 RXI (Receive data full) */
    [16] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TXI,GROUP0), /* SCI1 TXI (Transmit data empty) */
    [17] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TEI,GROUP1), /* SCI1 TEI (Transmit end) */
    [18] = BSP_PRV_VECT_ENUM(EVENT_SCI1_ERI,GROUP2), /* SCI1 ERI (Receive error) */
    [19] = BSP_PRV_VECT_ENUM(EVENT_SCI2_RXI,GROUP3), /* SCI2 RXI (Receive data full) */
    [20] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TXI,GROUP4), /* SCI2 TXI (Transmit data empty) */
    [21] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TEI,GROUP5), /* SCI2 TEI (Transmit end) */
    [22] = BSP_PRV_VECT_ENUM(EVENT_SCI2_ERI,GROUP6), /* SCI2 ERI (Receive error) */
    [23] = BSP_PRV_VECT_ENUM(EVENT_SCI3_RXI,GROUP7), /* SCI3 RXI (Receive data full) */
    [24] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TXI,GROUP0), /* SCI3 TXI (Transmit data empty) */
    [25] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TEI,GROUP1), /* SCI3 TEI (Transmit end) */
    [26] = BSP_PRV_VECT_ENUM(EVENT_SCI3_ERI,GROUP2), /* SCI3 ERI (Receive error) */
    [27] = BSP_PRV_VECT_ENUM(EVENT_SCI4_RXI,GROUP3), /* SCI4 RXI (Receive data full) */
    [28] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TXI,GROUP4), /* SCI4 TXI (Transmit data empty) */
    [29] = BSP_PRV_VECT_ENUM(EVENT_SCI4_TEI,GROUP5), /* SCI4 TEI (Transmit end) */
    [30] = BSP_PRV_VECT_ENUM(EVENT_SCI4_ERI,GROUP6), /* SCI4 ERI (Receive error) */
    [31] = BSP_PRV_VECT_ENUM(EVENT_SCI5_RXI,GROUP7), /* SCI5 RXI (Receive data full) */
    [32] = BSP_PRV_VECT_ENUM(EVENT_SCI5_TXI,GROUP0), /* SCI5 TXI (Transmit data empty) */
    [33] = BSP_PRV_VECT_ENUM(EVENT_SCI5_TEI,GROUP1), /* SCI5 TEI (Transmit end) */
    [34] = BSP_PRV_VECT_ENUM(EVENT_SCI5_ERI,GROUP2), /* SCI5 ERI (Receive error) */
    [35] = BSP_PRV_VECT_ENUM(EVENT_SCI6_RXI,GROUP3), /* SCI6 RXI (Receive data full) */
    [36] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TXI,GROUP4), /* SCI6 TXI (Transmit data empty) */
    [37] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TEI,GROUP5), /* SCI6 TEI (Transmit end) */
    [38] = BSP_PRV_VECT_ENUM(EVENT_SCI6_ERI,GROUP6), /* SCI6 ERI (Receive error) */
    [39] = BSP_PRV_VECT_ENUM(EVENT_SCI7_RXI,GROUP7), /* SCI7 RXI (Receive data full) */
    [40] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TXI,GROUP0), /* SCI7 TXI (Transmit data empty) */
    [41] = BSP_PRV_VECT_ENUM(EVENT_SCI7_TEI,GROUP1), /* SCI7 TEI (Transmit end) */
    [42] = BSP_PRV_VECT_ENUM(EVENT_SCI7_ERI,GROUP2), /* SCI7 ERI (Receive error) */
    [43] = BSP_PRV_VECT_ENUM(EVENT_SCI8_RXI,GROUP3), /* SCI8 RXI (Receive data full) */
    [44] = BSP_PRV_VECT_ENUM(EVENT_SCI8_TXI,GROUP4), /* SCI8 TXI (Transmit data empty) */
    [45] = BSP_PRV_VECT_ENUM(EVENT_SCI8_TEI,GROUP5), /* SCI8 TEI (Transmit end) */
    [46] = BSP_PRV_VECT_ENUM(EVENT_SCI8_ERI,GROUP6), /* SCI8 ERI (Receive error) */
    [47] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP7), /* SCI9 RXI (Receive data full) */
    [48] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP0), /* SCI9 TXI (Transmit data empty) */
    [49] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP1), /* SCI9 TEI (Transmit end) */
    [50] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP2), /* SCI9 ERI (Receive error) */
    [51] = BSP_PRV_VECT_ENUM(EVENT_IIC0_RXI,GROUP3), /* IIC0 RXI (Receive data full) */
    [52] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TXI,GROUP4), /* IIC0 TXI (Transmit data empty) */
    [53] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TEI,GROUP5), /* IIC0 TEI (Transmit end) */
    [54] = BSP_PRV_VECT_ENUM(EVENT_IIC0_ERI,GROUP6), /* IIC0 ERI (Transfer error) */
    [55] = BSP_PRV_VECT_ENUM(EVENT_IIC1_RXI,GROUP7), /* IIC1 RXI (Receive data full) */
    [56] = BSP_PRV_VECT_ENUM(EVENT_IIC1_TXI,GROUP0), /* IIC1 TXI (Transmit data empty) */
    [57] = BSP_PRV_VECT_ENUM(EVENT_IIC1_TEI,GROUP1), /* IIC1 TEI (Transmit end) */
    [58] = BSP_PRV_VECT_ENUM(EVENT_IIC1_ERI,GROUP2), /* IIC1 ERI (Transfer error) */
    [59] = BSP_PRV_VECT_ENUM(EVENT_IIC2_RXI,GROUP3), /* IIC2 RXI (Receive data full) */
    [60] = BSP_PRV_VECT_ENUM(EVENT_IIC2_TXI,GROUP4), /* IIC2 TXI (Transmit data empty) */
    [61] = BSP_PRV_VECT_ENUM(EVENT_IIC2_TEI,GROUP5), /* IIC2 TEI (Transmit end) */
    [62] = BSP_PRV_VECT_ENUM(EVENT_IIC2_ERI,GROUP6), /* IIC2 ERI (Transfer error) */
    [63] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP7), /* SPI0 RXI (Receive buffer full) */
    [64] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP0), /* SPI0 TEI (Transmission complete event) */
    [65] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP1), /* SPI0 ERI (Error) */
    [66] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,GROUP2), /* SPI1 RXI (Receive buffer full) */
    [67] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,GROUP3), /* SPI1 TEI (Transmission complete event) */
    [68] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,GROUP4), /* SPI1 ERI (Error) */
};
#endif
#endif

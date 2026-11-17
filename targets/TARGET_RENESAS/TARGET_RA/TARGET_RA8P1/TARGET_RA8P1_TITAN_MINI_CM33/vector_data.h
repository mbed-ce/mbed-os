/* Cortex-M33 (CPU1) ICU vector table of the RA8P1 Titan Mini.
 *
 * The RA8P1 gives each core its own ICU (aliased at the same local address
 * 0x40006000): vector numbers are therefore CPU1-local. The table below
 * allocates:
 *
 *   [0]      IPC_IRQ0  - CPU mutual interrupt 0 (doorbell from CPU0; the
 *                        FSP r_ipc driver reads the FIFO message and invokes
 *                        the callback registered by the rpmsg-lite platform
 *                        port)
 *   [1..2]   GPT12     - us_ticker of the CM33 target (see
 *                        TARGET_RA8P1/us_ticker_data.h)
 *   [3..10]  external IRQ slot pool - r_icu_isr entries whose IELSR links
 *                        are written at runtime by gpio_irq_device.c, so up
 *                        to 8 InterruptIn objects can be used on CPU1
 *   [11..50] SCI0..SCI9 - RXI/TXI/TEI/ERI for all ten UARTs (SERIAL)
 *   [51..62] IIC0..IIC2 - RXI/TXI/TEI/ERI for the three I2C masters
 *   [63..68] SPI0/SPI1  - RXI/TEI/ERI for both SPI masters
 *
 * hal_data.c of the M85 board resolves every interrupt number through the
 * VECTOR_NUMBER_* macros defined here, so the exact same FSP configuration
 * file builds a working CPU1 image.
 */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
extern "C" {
#endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (69)
#endif
/* ISR prototypes */
void gpt_counter_overflow_isr(void);
void gpt_capture_compare_a_isr(void);
void r_icu_isr(void);
void iic_master_rxi_isr(void);
void iic_master_txi_isr(void);
void iic_master_tei_isr(void);
void iic_master_eri_isr(void);
void spi_b_rxi_isr(void);
void spi_b_tei_isr(void);
void spi_b_eri_isr(void);
void sci_b_uart_rxi_isr(void);
void sci_b_uart_txi_isr(void);
void sci_b_uart_tei_isr(void);
void sci_b_uart_eri_isr(void);
/* r_ipc.c (FSP IPC driver, used by rpmsg-lite for the CPU0<->CPU1 doorbell) */
extern void ipc_isr(void);

/* IPC IRQ0: messages written by CPU0 into the IPC1 FIFO (CPU0 -> CPU1) */
#define VECTOR_NUMBER_IPC_IRQ0 ((IRQn_Type) 0)
#define IPC_IRQ0_IRQn          ((IRQn_Type) 0)

/* GPT12: us_ticker of the Cortex-M33 build */
#define VECTOR_NUMBER_GPT12_COUNTER_OVERFLOW ((IRQn_Type) 1)
#define GPT12_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 1)
#define VECTOR_NUMBER_GPT12_CAPTURE_COMPARE_A ((IRQn_Type) 2)
#define GPT12_CAPTURE_COMPARE_A_IRQn          ((IRQn_Type) 2)

/* External IRQ slot pool (indices 3..10): the IELSR links are established
 * at runtime by TARGET_RA8P1/gpio_irq_device.c, matching the pool declared
 * in PeripheralNames.h of this folder. */
#define VECTOR_NUMBER_ICU_IRQ ((IRQn_Type) 3) /* external IRQ slot 0 */

/* SCI0 */
#define VECTOR_NUMBER_SCI0_RXI ((IRQn_Type) 11)
#define SCI0_RXI_IRQn          ((IRQn_Type) 11)
#define VECTOR_NUMBER_SCI0_TXI ((IRQn_Type) 12)
#define SCI0_TXI_IRQn          ((IRQn_Type) 12)
#define VECTOR_NUMBER_SCI0_TEI ((IRQn_Type) 13)
#define SCI0_TEI_IRQn          ((IRQn_Type) 13)
#define VECTOR_NUMBER_SCI0_ERI ((IRQn_Type) 14)
#define SCI0_ERI_IRQn          ((IRQn_Type) 14)
/* SCI1 */
#define VECTOR_NUMBER_SCI1_RXI ((IRQn_Type) 15)
#define SCI1_RXI_IRQn          ((IRQn_Type) 15)
#define VECTOR_NUMBER_SCI1_TXI ((IRQn_Type) 16)
#define SCI1_TXI_IRQn          ((IRQn_Type) 16)
#define VECTOR_NUMBER_SCI1_TEI ((IRQn_Type) 17)
#define SCI1_TEI_IRQn          ((IRQn_Type) 17)
#define VECTOR_NUMBER_SCI1_ERI ((IRQn_Type) 18)
#define SCI1_ERI_IRQn          ((IRQn_Type) 18)
/* SCI2 */
#define VECTOR_NUMBER_SCI2_RXI ((IRQn_Type) 19)
#define SCI2_RXI_IRQn          ((IRQn_Type) 19)
#define VECTOR_NUMBER_SCI2_TXI ((IRQn_Type) 20)
#define SCI2_TXI_IRQn          ((IRQn_Type) 20)
#define VECTOR_NUMBER_SCI2_TEI ((IRQn_Type) 21)
#define SCI2_TEI_IRQn          ((IRQn_Type) 21)
#define VECTOR_NUMBER_SCI2_ERI ((IRQn_Type) 22)
#define SCI2_ERI_IRQn          ((IRQn_Type) 22)
/* SCI3 */
#define VECTOR_NUMBER_SCI3_RXI ((IRQn_Type) 23)
#define SCI3_RXI_IRQn          ((IRQn_Type) 23)
#define VECTOR_NUMBER_SCI3_TXI ((IRQn_Type) 24)
#define SCI3_TXI_IRQn          ((IRQn_Type) 24)
#define VECTOR_NUMBER_SCI3_TEI ((IRQn_Type) 25)
#define SCI3_TEI_IRQn          ((IRQn_Type) 25)
#define VECTOR_NUMBER_SCI3_ERI ((IRQn_Type) 26)
#define SCI3_ERI_IRQn          ((IRQn_Type) 26)
/* SCI4 */
#define VECTOR_NUMBER_SCI4_RXI ((IRQn_Type) 27)
#define SCI4_RXI_IRQn          ((IRQn_Type) 27)
#define VECTOR_NUMBER_SCI4_TXI ((IRQn_Type) 28)
#define SCI4_TXI_IRQn          ((IRQn_Type) 28)
#define VECTOR_NUMBER_SCI4_TEI ((IRQn_Type) 29)
#define SCI4_TEI_IRQn          ((IRQn_Type) 29)
#define VECTOR_NUMBER_SCI4_ERI ((IRQn_Type) 30)
#define SCI4_ERI_IRQn          ((IRQn_Type) 30)
/* SCI5 */
#define VECTOR_NUMBER_SCI5_RXI ((IRQn_Type) 31)
#define SCI5_RXI_IRQn          ((IRQn_Type) 31)
#define VECTOR_NUMBER_SCI5_TXI ((IRQn_Type) 32)
#define SCI5_TXI_IRQn          ((IRQn_Type) 32)
#define VECTOR_NUMBER_SCI5_TEI ((IRQn_Type) 33)
#define SCI5_TEI_IRQn          ((IRQn_Type) 33)
#define VECTOR_NUMBER_SCI5_ERI ((IRQn_Type) 34)
#define SCI5_ERI_IRQn          ((IRQn_Type) 34)
/* SCI6 */
#define VECTOR_NUMBER_SCI6_RXI ((IRQn_Type) 35)
#define SCI6_RXI_IRQn          ((IRQn_Type) 35)
#define VECTOR_NUMBER_SCI6_TXI ((IRQn_Type) 36)
#define SCI6_TXI_IRQn          ((IRQn_Type) 36)
#define VECTOR_NUMBER_SCI6_TEI ((IRQn_Type) 37)
#define SCI6_TEI_IRQn          ((IRQn_Type) 37)
#define VECTOR_NUMBER_SCI6_ERI ((IRQn_Type) 38)
#define SCI6_ERI_IRQn          ((IRQn_Type) 38)
/* SCI7 */
#define VECTOR_NUMBER_SCI7_RXI ((IRQn_Type) 39)
#define SCI7_RXI_IRQn          ((IRQn_Type) 39)
#define VECTOR_NUMBER_SCI7_TXI ((IRQn_Type) 40)
#define SCI7_TXI_IRQn          ((IRQn_Type) 40)
#define VECTOR_NUMBER_SCI7_TEI ((IRQn_Type) 41)
#define SCI7_TEI_IRQn          ((IRQn_Type) 41)
#define VECTOR_NUMBER_SCI7_ERI ((IRQn_Type) 42)
#define SCI7_ERI_IRQn          ((IRQn_Type) 42)
/* SCI8 */
#define VECTOR_NUMBER_SCI8_RXI ((IRQn_Type) 43)
#define SCI8_RXI_IRQn          ((IRQn_Type) 43)
#define VECTOR_NUMBER_SCI8_TXI ((IRQn_Type) 44)
#define SCI8_TXI_IRQn          ((IRQn_Type) 44)
#define VECTOR_NUMBER_SCI8_TEI ((IRQn_Type) 45)
#define SCI8_TEI_IRQn          ((IRQn_Type) 45)
#define VECTOR_NUMBER_SCI8_ERI ((IRQn_Type) 46)
#define SCI8_ERI_IRQn          ((IRQn_Type) 46)
/* SCI9 */
#define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 47)
#define SCI9_RXI_IRQn          ((IRQn_Type) 47)
#define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 48)
#define SCI9_TXI_IRQn          ((IRQn_Type) 48)
#define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 49)
#define SCI9_TEI_IRQn          ((IRQn_Type) 49)
#define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 50)
#define SCI9_ERI_IRQn          ((IRQn_Type) 50)
/* IIC0 */
#define VECTOR_NUMBER_IIC0_RXI ((IRQn_Type) 51)
#define IIC0_RXI_IRQn          ((IRQn_Type) 51)
#define VECTOR_NUMBER_IIC0_TXI ((IRQn_Type) 52)
#define IIC0_TXI_IRQn          ((IRQn_Type) 52)
#define VECTOR_NUMBER_IIC0_TEI ((IRQn_Type) 53)
#define IIC0_TEI_IRQn          ((IRQn_Type) 53)
#define VECTOR_NUMBER_IIC0_ERI ((IRQn_Type) 54)
#define IIC0_ERI_IRQn          ((IRQn_Type) 54)
/* IIC1 */
#define VECTOR_NUMBER_IIC1_RXI ((IRQn_Type) 55)
#define IIC1_RXI_IRQn          ((IRQn_Type) 55)
#define VECTOR_NUMBER_IIC1_TXI ((IRQn_Type) 56)
#define IIC1_TXI_IRQn          ((IRQn_Type) 56)
#define VECTOR_NUMBER_IIC1_TEI ((IRQn_Type) 57)
#define IIC1_TEI_IRQn          ((IRQn_Type) 57)
#define VECTOR_NUMBER_IIC1_ERI ((IRQn_Type) 58)
#define IIC1_ERI_IRQn          ((IRQn_Type) 58)
/* IIC2 */
#define VECTOR_NUMBER_IIC2_RXI ((IRQn_Type) 59)
#define IIC2_RXI_IRQn          ((IRQn_Type) 59)
#define VECTOR_NUMBER_IIC2_TXI ((IRQn_Type) 60)
#define IIC2_TXI_IRQn          ((IRQn_Type) 60)
#define VECTOR_NUMBER_IIC2_TEI ((IRQn_Type) 61)
#define IIC2_TEI_IRQn          ((IRQn_Type) 61)
#define VECTOR_NUMBER_IIC2_ERI ((IRQn_Type) 62)
#define IIC2_ERI_IRQn          ((IRQn_Type) 62)
/* SPI0 */
#define VECTOR_NUMBER_SPI0_RXI ((IRQn_Type) 63)
#define SPI0_RXI_IRQn          ((IRQn_Type) 63)
#define VECTOR_NUMBER_SPI0_TEI ((IRQn_Type) 64)
#define SPI0_TEI_IRQn          ((IRQn_Type) 64)
#define VECTOR_NUMBER_SPI0_ERI ((IRQn_Type) 65)
#define SPI0_ERI_IRQn          ((IRQn_Type) 65)
/* SPI1 */
#define VECTOR_NUMBER_SPI1_RXI ((IRQn_Type) 66)
#define SPI1_RXI_IRQn          ((IRQn_Type) 66)
#define VECTOR_NUMBER_SPI1_TEI ((IRQn_Type) 67)
#define SPI1_TEI_IRQn          ((IRQn_Type) 67)
#define VECTOR_NUMBER_SPI1_ERI ((IRQn_Type) 68)
#define SPI1_ERI_IRQn          ((IRQn_Type) 68)

/* The number of entries required for the ICU vector table. */
#define BSP_ICU_VECTOR_NUM_ENTRIES (69)

#ifdef __cplusplus
}
#endif
#endif /* VECTOR_DATA_H */

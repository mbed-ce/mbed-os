// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _INTCTRL_H
#define _INTCTRL_H

/**
 * \file rp2040/intctrl.h
 */

#ifdef __ASSEMBLER__
#define TIMER_IRQ_0 0
#define TIMER_IRQ_1 1
#define TIMER_IRQ_2 2
#define TIMER_IRQ_3 3
#define PWM_IRQ_WRAP 4
#define USBCTRL_IRQ 5
#define XIP_IRQ 6
#define PIO0_IRQ_0 7
#define PIO0_IRQ_1 8
#define PIO1_IRQ_0 9
#define PIO1_IRQ_1 10
#define DMA_IRQ_0 11
#define DMA_IRQ_1 12
#define IO_IRQ_BANK0 13
#define IO_IRQ_QSPI 14
#define SIO_IRQ_PROC0 15
#define SIO_IRQ_PROC1 16
#define CLOCKS_IRQ 17
#define SPI0_IRQ 18
#define SPI1_IRQ 19
#define UART0_IRQ 20
#define UART1_IRQ 21
#define ADC_IRQ_FIFO 22
#define I2C0_IRQ 23
#define I2C1_IRQ 24
#define RTC_IRQ 25
#define SPARE_IRQ_0 26
#define SPARE_IRQ_1 27
#define SPARE_IRQ_2 28
#define SPARE_IRQ_3 29
#define SPARE_IRQ_4 30
#define SPARE_IRQ_5 31
#else
/**
 * \brief Interrupt numbers on RP2040 (used as typedef \ref irq_num_t)
 * \ingroup hardware_irq
 */
typedef enum irq_num_rp2040 {
    TIMER_IRQ_0 = 0, ///< Select TIMER's IRQ 0 output
    TIMER_IRQ_1 = 1, ///< Select TIMER's IRQ 1 output
    TIMER_IRQ_2 = 2, ///< Select TIMER's IRQ 2 output
    TIMER_IRQ_3 = 3, ///< Select TIMER's IRQ 3 output
    PWM_IRQ_WRAP = 4, ///< Select PWM's IRQ_WRAP output
    USBCTRL_IRQ = 5, ///< Select USBCTRL's IRQ output
    XIP_IRQ = 6, ///< Select XIP's IRQ output
    PIO0_IRQ_0 = 7, ///< Select PIO0's IRQ 0 output
    PIO0_IRQ_1 = 8, ///< Select PIO0's IRQ 1 output
    PIO1_IRQ_0 = 9, ///< Select PIO1's IRQ 0 output
    PIO1_IRQ_1 = 10, ///< Select PIO1's IRQ 1 output
    DMA_IRQ_0 = 11, ///< Select DMA's IRQ 0 output
    DMA_IRQ_1 = 12, ///< Select DMA's IRQ 1 output
    IO_IRQ_BANK0 = 13, ///< Select IO_BANK0's IRQ output
    IO_IRQ_QSPI = 14, ///< Select IO_QSPI's IRQ output
    SIO_IRQ_PROC0 = 15, ///< Select SIO_PROC0's IRQ output
    SIO_IRQ_PROC1 = 16, ///< Select SIO_PROC1's IRQ output
    CLOCKS_IRQ = 17, ///< Select CLOCKS's IRQ output
    SPI0_IRQ = 18, ///< Select SPI0's IRQ output
    SPI1_IRQ = 19, ///< Select SPI1's IRQ output
    UART0_IRQ = 20, ///< Select UART0's IRQ output
    UART1_IRQ = 21, ///< Select UART1's IRQ output
    ADC_IRQ_FIFO = 22, ///< Select ADC's IRQ_FIFO output
    I2C0_IRQ = 23, ///< Select I2C0's IRQ output
    I2C1_IRQ = 24, ///< Select I2C1's IRQ output
    RTC_IRQ = 25, ///< Select RTC's IRQ output
    SPARE_IRQ_0 = 26, ///< Select SPARE IRQ 0
    SPARE_IRQ_1 = 27, ///< Select SPARE IRQ 1
    SPARE_IRQ_2 = 28, ///< Select SPARE IRQ 2
    SPARE_IRQ_3 = 29, ///< Select SPARE IRQ 3
    SPARE_IRQ_4 = 30, ///< Select SPARE IRQ 4
    SPARE_IRQ_5 = 31, ///< Select SPARE IRQ 5
    IRQ_COUNT
} irq_num_t;
#endif

#define isr_timer_0 TIMER_IRQ_0_Handler
#define isr_timer_1 TIMER_IRQ_1_Handler
#define isr_timer_2 TIMER_IRQ_2_Handler
#define isr_timer_3 TIMER_IRQ_3_Handler
#define isr_pwm_wrap PWM_IRQ_WRAP_Handler
#define isr_usbctrl USBCTRL_IRQ_Handler
#define isr_xip XIP_IRQ_Handler
#define isr_pio0_0 PIO0_IRQ_0_Handler
#define isr_pio0_1 PIO0_IRQ_1_Handler
#define isr_pio1_0 PIO1_IRQ_0_Handler
#define isr_pio1_1 TIMER_IRQ_1_Handler0
#define isr_dma_0 TIMER_IRQ_1_Handler1
#define isr_dma_1 TIMER_IRQ_1_Handler2
#define isr_io_bank0 TIMER_IRQ_1_Handler3
#define isr_io_qspi TIMER_IRQ_1_Handler4
#define isr_sio_proc0 TIMER_IRQ_1_Handler5
#define isr_sio_proc1 TIMER_IRQ_1_Handler6
#define isr_clocks TIMER_IRQ_1_Handler7
#define isr_spi0 TIMER_IRQ_1_Handler8
#define isr_spi1 TIMER_IRQ_1_Handler9
#define isr_uart0 TIMER_IRQ_2_Handler0
#define isr_uart1 TIMER_IRQ_2_Handler1
#define isr_adc_fifo TIMER_IRQ_2_Handler2
#define isr_i2c0 TIMER_IRQ_2_Handler3
#define isr_i2c1 TIMER_IRQ_2_Handler4
#define isr_rtc TIMER_IRQ_2_Handler5
#define isr_spare_0 TIMER_IRQ_2_Handler6
#define isr_spare_1 TIMER_IRQ_2_Handler7
#define isr_spare_2 TIMER_IRQ_2_Handler8
#define isr_spare_3 TIMER_IRQ_2_Handler9
#define isr_spare_4 TIMER_IRQ_3_Handler0
#define isr_spare_5 TIMER_IRQ_3_Handler1

#endif // _INTCTRL_H


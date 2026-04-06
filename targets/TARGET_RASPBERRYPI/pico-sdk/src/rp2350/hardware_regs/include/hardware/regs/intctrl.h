// THIS HEADER FILE IS AUTOMATICALLY GENERATED -- DO NOT EDIT

/**
 * Copyright (c) 2024 Raspberry Pi Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _INTCTRL_H
#define _INTCTRL_H

/**
 * \file rp2350/intctrl.h
 */

#ifdef __ASSEMBLER__
#define TIMER0_IRQ_0 0
#define TIMER0_IRQ_1 1
#define TIMER0_IRQ_2 2
#define TIMER0_IRQ_3 3
#define TIMER1_IRQ_0 4
#define TIMER1_IRQ_1 5
#define TIMER1_IRQ_2 6
#define TIMER1_IRQ_3 7
#define PWM_IRQ_WRAP_0 8
#define PWM_IRQ_WRAP_1 9
#define DMA_IRQ_0 10
#define DMA_IRQ_1 11
#define DMA_IRQ_2 12
#define DMA_IRQ_3 13
#define USBCTRL_IRQ 14
#define PIO0_IRQ_0 15
#define PIO0_IRQ_1 16
#define PIO1_IRQ_0 17
#define PIO1_IRQ_1 18
#define PIO2_IRQ_0 19
#define PIO2_IRQ_1 20
#define IO_IRQ_BANK0 21
#define IO_IRQ_BANK0_NS 22
#define IO_IRQ_QSPI 23
#define IO_IRQ_QSPI_NS 24
#define SIO_IRQ_FIFO 25
#define SIO_IRQ_BELL 26
#define SIO_IRQ_FIFO_NS 27
#define SIO_IRQ_BELL_NS 28
#define SIO_IRQ_MTIMECMP 29
#define CLOCKS_IRQ 30
#define SPI0_IRQ 31
#define SPI1_IRQ 32
#define UART0_IRQ 33
#define UART1_IRQ 34
#define ADC_IRQ_FIFO 35
#define I2C0_IRQ 36
#define I2C1_IRQ 37
#define OTP_IRQ 38
#define TRNG_IRQ 39
#define PROC0_IRQ_CTI 40
#define PROC1_IRQ_CTI 41
#define PLL_SYS_IRQ 42
#define PLL_USB_IRQ 43
#define POWMAN_IRQ_POW 44
#define POWMAN_IRQ_TIMER 45
#define SPARE_IRQ_0 46
#define SPARE_IRQ_1 47
#define SPARE_IRQ_2 48
#define SPARE_IRQ_3 49
#define SPARE_IRQ_4 50
#define SPARE_IRQ_5 51
#else
/**
 * \brief Interrupt numbers on RP2350 (used as typedef \ref irq_num_t)
 * \ingroup hardware_irq
 */
typedef enum irq_num_rp2350 {
    TIMER0_IRQ_0 = 0, ///< Select TIMER0's IRQ 0 output
    TIMER0_IRQ_1 = 1, ///< Select TIMER0's IRQ 1 output
    TIMER0_IRQ_2 = 2, ///< Select TIMER0's IRQ 2 output
    TIMER0_IRQ_3 = 3, ///< Select TIMER0's IRQ 3 output
    TIMER1_IRQ_0 = 4, ///< Select TIMER1's IRQ 0 output
    TIMER1_IRQ_1 = 5, ///< Select TIMER1's IRQ 1 output
    TIMER1_IRQ_2 = 6, ///< Select TIMER1's IRQ 2 output
    TIMER1_IRQ_3 = 7, ///< Select TIMER1's IRQ 3 output
    PWM_IRQ_WRAP_0 = 8, ///< Select PWM's WRAP_0 IRQ output
    PWM_IRQ_WRAP_1 = 9, ///< Select PWM's WRAP_1 IRQ output
    DMA_IRQ_0 = 10, ///< Select DMA's IRQ 0 output
    DMA_IRQ_1 = 11, ///< Select DMA's IRQ 1 output
    DMA_IRQ_2 = 12, ///< Select DMA's IRQ 2 output
    DMA_IRQ_3 = 13, ///< Select DMA's IRQ 3 output
    USBCTRL_IRQ = 14, ///< Select USBCTRL's IRQ output
    PIO0_IRQ_0 = 15, ///< Select PIO0's IRQ 0 output
    PIO0_IRQ_1 = 16, ///< Select PIO0's IRQ 1 output
    PIO1_IRQ_0 = 17, ///< Select PIO1's IRQ 0 output
    PIO1_IRQ_1 = 18, ///< Select PIO1's IRQ 1 output
    PIO2_IRQ_0 = 19, ///< Select PIO2's IRQ 0 output
    PIO2_IRQ_1 = 20, ///< Select PIO2's IRQ 1 output
    IO_IRQ_BANK0 = 21, ///< Select IO_BANK0's IRQ output
    IO_IRQ_BANK0_NS = 22, ///< Select IO_BANK0_NS's IRQ output
    IO_IRQ_QSPI = 23, ///< Select IO_QSPI's IRQ output
    IO_IRQ_QSPI_NS = 24, ///< Select IO_QSPI_NS's IRQ output
    SIO_IRQ_FIFO = 25, ///< Select SIO's FIFO IRQ output
    SIO_IRQ_BELL = 26, ///< Select SIO's BELL IRQ output
    SIO_IRQ_FIFO_NS = 27, ///< Select SIO_NS's FIFO IRQ output
    SIO_IRQ_BELL_NS = 28, ///< Select SIO_NS's BELL IRQ output
    SIO_IRQ_MTIMECMP = 29, ///< Select SIO's MTIMECMP IRQ output
    CLOCKS_IRQ = 30, ///< Select CLOCKS's IRQ output
    SPI0_IRQ = 31, ///< Select SPI0's IRQ output
    SPI1_IRQ = 32, ///< Select SPI1's IRQ output
    UART0_IRQ = 33, ///< Select UART0's IRQ output
    UART1_IRQ = 34, ///< Select UART1's IRQ output
    ADC_IRQ_FIFO = 35, ///< Select ADC's FIFO IRQ output
    I2C0_IRQ = 36, ///< Select I2C0's IRQ output
    I2C1_IRQ = 37, ///< Select I2C1's IRQ output
    OTP_IRQ = 38, ///< Select OTP's IRQ output
    TRNG_IRQ = 39, ///< Select TRNG's IRQ output
    PROC0_IRQ_CTI = 40, ///< Select PROC0's CTI IRQ output
    PROC1_IRQ_CTI = 41, ///< Select PROC1's CTI IRQ output
    PLL_SYS_IRQ = 42, ///< Select PLL_SYS's IRQ output
    PLL_USB_IRQ = 43, ///< Select PLL_USB's IRQ output
    POWMAN_IRQ_POW = 44, ///< Select POWMAN's POW IRQ output
    POWMAN_IRQ_TIMER = 45, ///< Select POWMAN's TIMER IRQ output
    SPARE_IRQ_0 = 46, ///< Select SPARE IRQ 0
    SPARE_IRQ_1 = 47, ///< Select SPARE IRQ 1
    SPARE_IRQ_2 = 48, ///< Select SPARE IRQ 2
    SPARE_IRQ_3 = 49, ///< Select SPARE IRQ 3
    SPARE_IRQ_4 = 50, ///< Select SPARE IRQ 4
    SPARE_IRQ_5 = 51, ///< Select SPARE IRQ 5
    IRQ_COUNT
} irq_num_t;
#endif

#define isr_timer0_0 TIMER_IRQ_0_Handler
#define isr_timer0_1 TIMER_IRQ_1_Handler
#define isr_timer0_2 TIMER_IRQ_2_Handler
#define isr_timer0_3 TIMER_IRQ_3_Handler
#define isr_timer1_0 PWM_IRQ_WRAP_Handler
#define isr_timer1_1 USBCTRL_IRQ_Handler
#define isr_timer1_2 XIP_IRQ_Handler
#define isr_timer1_3 PIO0_IRQ_0_Handler
#define isr_pwm_wrap_0 PIO0_IRQ_1_Handler
#define isr_pwm_wrap_1 PIO1_IRQ_0_Handler
#define isr_dma_0 TIMER_IRQ_1_Handler0
#define isr_dma_1 TIMER_IRQ_1_Handler1
#define isr_dma_2 TIMER_IRQ_1_Handler2
#define isr_dma_3 TIMER_IRQ_1_Handler3
#define isr_usbctrl TIMER_IRQ_1_Handler4
#define isr_pio0_0 TIMER_IRQ_1_Handler5
#define isr_pio0_1 TIMER_IRQ_1_Handler6
#define isr_pio1_0 TIMER_IRQ_1_Handler7
#define isr_pio1_1 TIMER_IRQ_1_Handler8
#define isr_pio2_0 TIMER_IRQ_1_Handler9
#define isr_pio2_1 TIMER_IRQ_2_Handler0
#define isr_io_bank0 TIMER_IRQ_2_Handler1
#define isr_io_bank0_ns TIMER_IRQ_2_Handler2
#define isr_io_qspi TIMER_IRQ_2_Handler3
#define isr_io_qspi_ns TIMER_IRQ_2_Handler4
#define isr_sio_fifo TIMER_IRQ_2_Handler5
#define isr_sio_bell TIMER_IRQ_2_Handler6
#define isr_sio_fifo_ns TIMER_IRQ_2_Handler7
#define isr_sio_bell_ns TIMER_IRQ_2_Handler8
#define isr_sio_mtimecmp TIMER_IRQ_2_Handler9
#define isr_clocks TIMER_IRQ_3_Handler0
#define isr_spi0 TIMER_IRQ_3_Handler1
#define isr_spi1 TIMER_IRQ_3_Handler2
#define isr_uart0 TIMER_IRQ_3_Handler3
#define isr_uart1 TIMER_IRQ_3_Handler4
#define isr_adc_fifo TIMER_IRQ_3_Handler5
#define isr_i2c0 TIMER_IRQ_3_Handler6
#define isr_i2c1 TIMER_IRQ_3_Handler7
#define isr_otp TIMER_IRQ_3_Handler8
#define isr_trng TIMER_IRQ_3_Handler9
#define isr_proc0_cti PWM_IRQ_WRAP_Handler0
#define isr_proc1_cti PWM_IRQ_WRAP_Handler1
#define isr_pll_sys PWM_IRQ_WRAP_Handler2
#define isr_pll_usb PWM_IRQ_WRAP_Handler3
#define isr_powman_pow PWM_IRQ_WRAP_Handler4
#define isr_powman_timer PWM_IRQ_WRAP_Handler5
#define isr_spare_0 PWM_IRQ_WRAP_Handler6
#define isr_spare_1 PWM_IRQ_WRAP_Handler7
#define isr_spare_2 PWM_IRQ_WRAP_Handler8
#define isr_spare_3 PWM_IRQ_WRAP_Handler9
#define isr_spare_4 USBCTRL_IRQ_Handler0
#define isr_spare_5 USBCTRL_IRQ_Handler1

#endif // _INTCTRL_H


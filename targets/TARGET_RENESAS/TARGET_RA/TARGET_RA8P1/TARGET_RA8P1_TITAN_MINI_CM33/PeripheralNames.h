/* mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Peripheral names for the Cortex-M33 (CPU1) view of the RA8P1 Titan Mini.
 * Same pin/peripheral mapping as the M85 target (see
 * ../TARGET_RA8P1_TITAN_MINI/PeripheralNames.h), but:
 *
 *  - the external-IRQ vector slot pool points at the CPU1-local ICU vector
 *    table entries reserved in vector_data.h of this folder (indices 3..10),
 *  - STDIO_UART stays UART_2 (SCI2) but the default console of this target
 *    is RTT, because the debug UART pins are owned by the M85 image.
 */
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = 0,
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    UART_7,
    UART_8,
    UART_9 = 9
} UARTName;

typedef enum {
    PWM_GPT0 = 0x00,
    PWM_GPT1,
    PWM_GPT2,
    PWM_GPT3,
    PWM_GPT4,
    PWM_GPT5,
    PWM_GPT6,
    PWM_GPT7,
    PWM_GPT8,
    PWM_GPT9,
    PWM_GPT10,
    PWM_GPT11,
    /* GPT12 is the us_ticker timer on this core and cannot be used for PWM
     * (PwmOut on a GPT12 pin fails at runtime); the entry only exists so
     * the shared PeripheralPins.c compiles. */
    PWM_GPT12,
    PWM_GPT13,
    PWM_AGT0,
    PWM_AGT1,
} PWMName;

typedef enum {
    PORT_0= BSP_IO_PORT_00,
    PORT_1= BSP_IO_PORT_01,
    PORT_2= BSP_IO_PORT_02,
    PORT_3= BSP_IO_PORT_03,
    PORT_4= BSP_IO_PORT_04,
    PORT_5= BSP_IO_PORT_05,
    PORT_6= BSP_IO_PORT_06,
    PORT_7= BSP_IO_PORT_07,
    PORT_8= BSP_IO_PORT_08,
    PORT_9= BSP_IO_PORT_09,
    PORT_10= BSP_IO_PORT_10,
    PORT_11= BSP_IO_PORT_11,
    PORT_12= BSP_IO_PORT_12,
    PORT_13= BSP_IO_PORT_13,
} PortName;

typedef enum {
    ADC_0= 0,
    ADC_1,
} ADCName;

typedef enum {
    DAC_0= 0,
    DAC_1,
} DACName;

typedef enum {
    IRQ_0= 0,
    IRQ_1,
    IRQ_2,
    IRQ_3,
    IRQ_4,
    IRQ_5,
    IRQ_6,
    IRQ_7,
    IRQ_8,
    IRQ_9,
    IRQ_10,
    IRQ_11,
    IRQ_12,
    IRQ_13,
    IRQ_14,
    IRQ_15,
    IRQ_16,
    IRQ_17,
    IRQ_18,
    IRQ_19,
    IRQ_20,
    IRQ_21,
    IRQ_22,
    IRQ_23,
    IRQ_24,
    IRQ_25,
    IRQ_26,
    IRQ_27,
    IRQ_28,
    IRQ_29,
    IRQ_30,
    IRQ_31,
} IRQName;

typedef enum {
    SPI_0 = 0,
    SPI_1,
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
    I2C_2,
} I2CName;

typedef enum {
    CAN_0 = 0,
    CAN_1,
} CANName;

typedef enum {
    QSPI_0 = 0,
} QSPIName;


#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
#define STDIO_UART_TX MBED_CONF_TARGET_STDIO_UART_TX
#else
#define STDIO_UART_TX CONSOLE_TX
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
#define STDIO_UART_RX MBED_CONF_TARGET_STDIO_UART_RX
#else
#define STDIO_UART_RX CONSOLE_RX
#endif
#define STDIO_UART UART_2

/* Number of ICU external IRQ channels usable with an InterruptIn. The RA8P1
 * gives each core its own ICU with 32 external IRQ channels; the CPU1 vector
 * table reserves 8 entries for them (see vector_data.h). */
#define IRQ_CHANNELS_COUNT (32)
#define UART_COUNT (10)
/* CAN is not part of device_has on CPU1 (can_api.c compiles to an empty unit
 * through its #ifdef DEVICE_CAN guard and the r_canfd driver is excluded from
 * the CM33 build), but the shared can_device.c glue still references the
 * count. */
#define CAN_COUNT (2)

/* External-IRQ vector slot pool of the CPU1-local ICU (consumed by
 * TARGET_RA8P1/gpio_irq_device.c). Must match the r_icu_isr entries of
 * g_vector_table in vector_data.c. */
#define EXT_IRQ_SLOT_COUNT (8)
#define EXT_IRQ_VECTOR_SLOTS { 3, 4, 5, 6, 7, 8, 9, 10 }

#ifdef __cplusplus
}
#endif

#endif

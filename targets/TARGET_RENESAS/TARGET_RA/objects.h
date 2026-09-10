/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "gpio_object.h"
#include "PeripheralNames.h"

#if MBED_CONF_RTOS_PRESENT
#include "cmsis_os.h"
#include "cmsis_os2.h"
#endif

struct port_s {
    PortName port;
    uint32_t mask;
    PinDirection direction;
};

#if DEVICE_ANALOGIN
struct analogin_s {
    uint32_t peripheral;
    uint32_t channel;
    PinName pin;
};
#endif

#if DEVICE_TRNG
struct trng_s {
    void *handle;
};
#endif

#if DEVICE_ANALOGOUT
#include "r_dac.h"
struct dac_s {
    uint8_t channel;
    dac_instance_ctrl_t *ctrl;
    dac_cfg_t cfg;
    dac_extended_cfg_t ext_cfg;
    uint16_t last_value;
};
#endif

#if DEVICE_PWMOUT
#include "r_timer_api.h"
#include "r_gpt.h"
#include "r_agt.h"

typedef enum {
    PWM_OUTPUT_A = 0,
    PWM_OUTPUT_B = 1,
} pwm_output_t;

typedef enum {
    PWM_TIMER_TYPE_GPT = 0,
    PWM_TIMER_TYPE_AGT
} pwm_timer_type_t;

struct pwmout_s {
    PinName pin;
    const timer_instance_t *p_timer;
    pwm_output_t pwm_unit;
    uint32_t period_counts;
    uint32_t duty_counts;
    uint8_t initialized;

    pwm_timer_type_t timer_type;
    bool is_16bit;
    timer_cfg_t cfg;
    union {
        gpt_extended_cfg_t gpt_ext;
        agt_extended_cfg_t agt_ext;
    } ext_cfg;
};
#endif

#if DEVICE_INTERRUPTIN
struct gpio_irq_s {
    uint32_t channel;
    PinName pin;
    uintptr_t context;
};
#endif

#if DEVICE_FLASH
struct flash_s {
    /*  nothing to be stored for now */
    uint32_t dummy;
};
#endif

#if DEVICE_SERIAL
/* The SCI_B peripheral is register incompatible with SCI. All differences are
 * adapted here so that serial_api.c can use the original SCI names everywhere. */
#if BSP_PERIPHERAL_SCI_B_PRESENT
    #include "r_sci_b_uart.h"
    typedef sci_b_uart_instance_ctrl_t sci_uart_instance_ctrl_t;
    typedef sci_b_uart_extended_cfg_t sci_uart_extended_cfg_t;
    typedef sci_b_baud_setting_t baud_setting_t;
    #define R_SCI_UART_BaudCalculate R_SCI_B_UART_BaudCalculate
    #define SSR_b CSR_b
#else
    #include "r_sci_uart.h"
#endif
struct serial_s {
    uart_cfg_t cfg;                     /* Local copy of configuration */
    uart_api_t const *p_api;            /* Pointer to R_SCI_UART API */
    sci_uart_instance_ctrl_t *p_ctrl;   /* Pointer to R_SCI_UART control block (instance-specific) */
    sci_uart_extended_cfg_t  ext;       /* Local copy of extended configuration */

    int instance_index;
    PinName tx;
    PinName rx;
};
#endif

#if DEVICE_SPI
/* The SPI_B peripheral has its own driver and register layout. All accesses go
 * through the generic spi_api_t interface (p_api) selected at init, so no FSP
 * function names need to be remapped; only types and enum values are aliased. */
#if BSP_PERIPHERAL_SPI_B_PRESENT
    #include <r_spi_b.h>
    typedef spi_b_instance_ctrl_t spi_instance_ctrl_t;
    typedef spi_b_extended_cfg_t  spi_extended_cfg_t;
    #define SPI_COMMUNICATION_FULL_DUPLEX   SPI_B_COMMUNICATION_FULL_DUPLEX
    #define SPI_COMMUNICATION_TRANSMIT_ONLY SPI_B_COMMUNICATION_TRANSMIT_ONLY
#else
    #include <r_spi.h>
#endif

/* A SCI channel can also be used in simple SPI mode (r_sci_spi driver). */
#include <r_sci_spi.h>

struct spi_s {
    spi_cfg_t           cfg;       /* Local copy of configuration */
    union {
        spi_extended_cfg_t     spi; /* R_SPI / R_SPI_B extended configuration */
        sci_spi_extended_cfg_t sci; /* R_SCI_SPI extended configuration */
    } ext;                         /* Local copy of extended configuration (interpreted per driver) */
    spi_instance_ctrl_t *p_ctrl;   /* R_SPI control block (instance-specific) */
    spi_api_t const     *p_api;    /* Pointer to R_SPI API (R_SPI, R_SPI_B or R_SCI_SPI driver) */
    bool                is_sci;    /* True if this instance uses the R_SCI_SPI driver */
    SPIName             channel;   /* SPI_0 / SPI_1 / SPI_SCI0 */
    uint32_t            hz;        /* Current frequency */
    uint8_t             bits;      /* Bits per frame (usually 8) */
    uint8_t             mode;      /* SPI mode 0..3 */
    bool                has_mosi;  /* Has MOSI pin */
    bool                has_miso;  /* Has MISO pin */
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreId_t     semaphoreId;
    osRtxSemaphore_t    semaphoreMem;
#else
    volatile bool       xfer_done;       /* Set by callback when the current transfer completes or aborted */
#endif
};
#endif

#if DEVICE_I2C
/* Same pattern as SPI: the IIC_B driver is reached through the generic
 * i2c_master_api_t interface (p_api), so only types are aliased here. */
#if BSP_PERIPHERAL_IIC_B_PRESENT
    #include "r_iic_b_master.h"
    typedef iic_b_master_extended_cfg_t   i2c_extended_cfg_t;
    typedef iic_b_master_instance_ctrl_t  i2c_instance_ctrl_t;
    typedef iic_b_master_clock_settings_t iic_master_clock_settings_t;
#else
    #include "r_iic_master.h"
    typedef iic_master_extended_cfg_t     i2c_extended_cfg_t;
    typedef iic_master_instance_ctrl_t    i2c_instance_ctrl_t;
#endif

struct i2c_s {
    i2c_master_cfg_t      cfg;             /* Local copy of the FSP configuration */
    i2c_extended_cfg_t    ext;             /* Local copy of the extended configuration */
    i2c_instance_ctrl_t  *p_ctrl;          /* Pointer to the FSP control block */
    i2c_master_api_t const *p_api;         /* Pointer to IIC master API (R_IIC_MASTER or R_IIC_B_MASTER driver) */
    I2CName               i2c;             /* I2C peripheral identifier (I2C_0 / I2C_1 / ...) */
    int                   hz;              /* Current bus frequency in Hz */

    /* ---- Mbed HAL transaction state ---- */
    volatile bool         xfer_done;       /* Set by callback when the current transfer completes */
    volatile bool         xfer_aborted;    /* Set by callback when the current transfer is aborted (NACK, timeout, etc.) */

    /* ---- Single-byte API buffering (required for Mbed I2CEEBlockDevice) ---- */
    /* The FSP driver can only do complete transfers, so address and data bytes passed to
     * i2c_byte_write() are accumulated here and sent as one transfer when the sequence
     * ends (see i2c_flush_tx_buf() in i2c_api.c). */
    bool                  tx_active;       /* True after i2c_start(), enabling raw byte accumulation */
    uint8_t               tx_buf[256];     /* Accumulated raw bytes to be sent in a single FSP transfer */
    size_t                tx_len;          /* Number of bytes currently stored in tx_buf */

    /* ---- Last device address used (required for i2c_byte_read) ---- */
    int                   last_address;    /* Cached 8-bit I2C address (write form, LSB = 0) used for the last operation */
};

#endif /* DEVICE_I2C */

#ifdef DEVICE_CAN

#ifdef DEVICE_CAN_FD
#include "r_canfd.h"
#else
#include "r_can.h"
#endif

#define MBED_CAN_RX_QUEUE_SIZE   16

struct can_s {
    const can_instance_t *instance;
    can_cfg_t             cfg_copy;

    uintptr_t             irq_context;

    /* RX ring buffer */
    can_frame_t           rx_queue[MBED_CAN_RX_QUEUE_SIZE];
    volatile uint8_t      rx_head;
    volatile uint8_t      rx_tail;

    int                   hz;
#ifdef DEVICE_CAN_FD
    int                   data_hz;
#endif
};


#endif /* DEVICE_CAN */


#define HAL_CRC_IS_SUPPORTED(polynomial, width) ((polynomial) == POLY_8BIT_CCITT || (polynomial) == POLY_16BIT_CCITT || (polynomial) == POLY_16BIT_IBM || (polynomial) == POLY_32BIT_ANSI)

#endif

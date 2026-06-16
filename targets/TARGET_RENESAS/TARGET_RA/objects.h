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

typedef enum {
    PWM_OUTPUT_A = 0,
    PWM_OUTPUT_B = 1,
} pwm_output_t;

struct pwmout_s {
    PinName pin;
    const timer_instance_t *p_timer;
    pwm_output_t pwm_unit;
    uint32_t period_counts;
    uint32_t duty_counts;
    uint8_t initialized;
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
#if BSP_PERIPHERAL_SCI_B_PRESENT
    #include "r_sci_b_uart.h"
    typedef sci_b_uart_instance_ctrl_t sci_uart_instance_ctrl_t;
    typedef sci_b_uart_extended_cfg_t sci_uart_extended_cfg_t;
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
#if BSP_PERIPHERAL_SPI_B_PRESENT
    #include <r_spi_b.h>
    typedef spi_b_extended_cfg_t spi_extended_cfg_t;
    typedef spi_b_instance_ctrl_t spi_instance_ctrl_t;
    #define R_SPI_Open          R_SPI_B_Open
    #define R_SPI_Close         R_SPI_B_Close
    #define R_SPI_WriteRead     R_SPI_B_WriteRead
    #define R_SPI_Write         R_SPI_B_Write
    #define R_SPI_Read          R_SPI_B_Read
    #define SPI_COMMUNICATION_FULL_DUPLEX SPI_B_COMMUNICATION_FULL_DUPLEX
    #define SPI_COMMUNICATION_TRANSMIT_ONLY SPI_B_COMMUNICATION_TRANSMIT_ONLY
#else
    #include <r_spi.h>
#endif

struct spi_s {
    spi_cfg_t           cfg;       /* Local copy of configuration */
    spi_extended_cfg_t  ext;       /* Local copy of extended configuration */
    spi_instance_ctrl_t *p_ctrl;   /* R_SPI control block (instance-specific) */
    SPIName             channel;   /* SPI_0 / SPI_1 */
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
#if BSP_PERIPHERAL_IIC_B_PRESENT
    #include "r_iic_b_master.h"
    typedef iic_b_master_extended_cfg_t   i2c_extended_cfg_t;
    typedef iic_b_master_instance_ctrl_t  i2c_instance_ctrl_t;
    typedef iic_b_master_clock_settings_t iic_master_clock_settings_t;
    #define R_IIC_MASTER_Open             R_IIC_B_MASTER_Open
    #define R_IIC_MASTER_Close            R_IIC_B_MASTER_Close
    #define R_IIC_MASTER_Read             R_IIC_B_MASTER_Read
    #define R_IIC_MASTER_Write            R_IIC_B_MASTER_Write
    #define R_IIC_MASTER_Abort            R_IIC_B_MASTER_Abort
    #define R_IIC_MASTER_SlaveAddressSet  R_IIC_B_MASTER_SlaveAddressSet
#else
    #include "r_iic_master.h"
    typedef iic_master_extended_cfg_t     i2c_extended_cfg_t;
    typedef iic_master_instance_ctrl_t    i2c_instance_ctrl_t;
#endif

struct i2c_s {
    i2c_master_cfg_t      cfg;             /* Local copy of the FSP configuration */
    i2c_extended_cfg_t    ext;             /* Local copy of the extended configuration */
    i2c_instance_ctrl_t  *p_ctrl;          /* Pointer to the FSP control block */
    I2CName               i2c;             /* I2C peripheral identifier (I2C_0 / I2C_1 / ...) */
    int                   hz;              /* Current bus frequency in Hz */

    /* ---- Mbed HAL transaction state ---- */
    bool                  pending_restart; /* Indicates whether the next transfer should use a repeated START */
    volatile bool         xfer_done;       /* Set by callback when the current transfer completes */
    volatile bool         xfer_aborted;    /* Set by callback when the current transfer is aborted (NACK, timeout, etc.) */

    /* ---- Raw?byte write mode (required for Mbed I2CEEBlockDevice) ---- */
    bool                  tx_active;       /* True after i2c_start(), enabling raw byte accumulation */
    uint8_t               tx_buf[256];     /* Accumulated raw bytes to be sent in a single FSP transfer */
    size_t                tx_len;          /* Number of bytes currently stored in tx_buf */

    /* ---- Last device address used (required for i2c_byte_read/write) ---- */
    int                   last_address;    /* Cached 8?bit I2C address used for the last read/write operation */
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


#define HAL_CRC_IS_SUPPORTED(polynomial, width) ((width) == 8 || (width) == 16 || (width) == 32)

#endif

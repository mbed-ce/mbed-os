/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed_assert.h"
#include "mbed_error.h"
#include "serial_api.h"
#include "pinmap.h"
#include "hal_data.h"

#define RX_BUF_SIZE 16

#if BSP_PERIPHERAL_SCI_B_PRESENT
#define baud_setting_t sci_b_baud_setting_t
#define R_SCI_UART_BaudCalculate R_SCI_B_UART_BaudCalculate
#define sci_uart_instance_ctrl_t sci_b_uart_instance_ctrl_t
#define R_SCI0_Type R_SCI_B0_Type
#define SSR_b CSR_b
#endif

#if MBED_CONF_TARGET_CONSOLE_UART
int stdio_uart_inited = 0; // used in platform/mbed_board.c and platform/mbed_retarget.cpp
serial_t stdio_uart;
#endif

extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];

extern uart_instance_t* const g_uart_instances[];

static uart_irq_handler g_irq_handler = NULL;
static uint32_t g_irq_id[UART_COUNT];
static uint8_t g_rx_irq_enabled[UART_COUNT];
static uint8_t g_tx_irq_enabled[UART_COUNT];
static volatile uint8_t g_rx_buf[UART_COUNT][RX_BUF_SIZE];
static volatile uint8_t g_rx_head[UART_COUNT];
static volatile uint8_t g_rx_tail[UART_COUNT];

static inline int rxbuf_is_full(int idx)
{
    return ((g_rx_head[idx] + 1) & (RX_BUF_SIZE - 1)) == g_rx_tail[idx];
}

static inline int rxbuf_is_empty(int idx)
{
    return g_rx_head[idx] == g_rx_tail[idx];
}

static inline void rxbuf_push(int idx, uint8_t b)
{
    g_rx_buf[idx][g_rx_head[idx]] = b;
    g_rx_head[idx] = (g_rx_head[idx] + 1) & (RX_BUF_SIZE - 1);
}

static inline uint8_t rxbuf_pop(int idx)
{
    uint8_t b = g_rx_buf[idx][g_rx_tail[idx]];
    g_rx_tail[idx] = (g_rx_tail[idx] + 1) & (RX_BUF_SIZE - 1);
    return b;
}


/* ---------------- PinMap UART channel ---------------- */

static int channel_from_pin(PinName tx, PinName rx)
{
    int tx_peri = pinmap_peripheral(tx, PinMap_UART_TX);
    int rx_peri = pinmap_peripheral(rx, PinMap_UART_RX);
    int merged  = pinmap_merge(tx_peri, rx_peri);
    if (merged == NC) {
        return -1;
    }
    return merged; /* UART_0 / UART_1 / ... */
}

int instance_index_from_channel(int channel)
{
    for(int instane_index = 0; instane_index < UART_COUNT; instane_index++) {
        if(g_uart_instances[instane_index]->p_cfg->channel == channel) {
            return instane_index;
        }
    }
    return -1;
}

/* ---------------- Mbed API: init/free ---------------- */

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(tx != NC && rx != NC);

    uint32_t uart_tx = pinmap_peripheral(tx, PinMap_UART_TX);
    uint32_t uart_rx = pinmap_peripheral(rx, PinMap_UART_RX);
    uint32_t peripheral = (int)pinmap_merge(uart_tx, uart_rx);

    int channel = channel_from_pin(tx, rx);

    uint32_t tx_function = pinmap_function(tx, PinMap_UART_TX);
    uint32_t rx_function = pinmap_function(rx, PinMap_UART_RX);
    pin_function(tx, tx_function);
    pin_function(rx, rx_function);

    obj->instance_index = instance_index_from_channel(channel);
    MBED_ASSERT(obj->instance_index >= 0 && obj->instance_index < UART_COUNT);

    uart_instance_t* const uart = g_uart_instances[obj->instance_index];

    obj->p_api = uart->p_api;
    obj->p_ctrl = (sci_uart_instance_ctrl_t *) uart->p_ctrl;
    const uart_cfg_t *cfg_src = uart->p_cfg;
    const sci_uart_extended_cfg_t *ext_src = (const sci_uart_extended_cfg_t *)cfg_src->p_extend;

    obj->cfg = *cfg_src;
    if (ext_src) {
        obj->ext = *ext_src;
        obj->cfg.p_extend = &obj->ext;
    } else {
        obj->cfg.p_extend = NULL;
    }
    obj->cfg.p_context = obj;
    obj->tx = tx;
    obj->rx = rx;

    uint8_t stdio_config = false;
#if defined(MBED_CONF_TARGET_CONSOLE_UART)
    if ((tx == CONSOLE_TX) || (rx == CONSOLE_RX)) {
        stdio_config = true;
    } else {
        if (peripheral == pinmap_peripheral(CONSOLE_TX, PinMap_UART_TX)) {
            error("Error: new serial object is using same UART as STDIO");
        }
    }
#endif

    if (stdio_config) {
#if MBED_CONF_PLATFORM_STDIO_BAUD_RATE
        // baudrate takes value from platform/mbed_lib.json
        R_SCI_UART_BaudCalculate(MBED_CONF_PLATFORM_STDIO_BAUD_RATE, true, 500, obj->ext.p_baud_setting);
#endif /* MBED_CONF_PLATFORM_STDIO_BAUD_RATE */
    } else {
#if MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
        // baudrate takes value from platform/mbed_lib.json
        R_SCI_UART_BaudCalculate(MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE, true, 500, obj->ext.p_baud_setting);
#endif /* MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE */
    }

    g_rx_head[obj->instance_index] = 0;
    g_rx_tail[obj->instance_index] = 0;

    fsp_err_t err = obj->p_api->open(obj->p_ctrl, &obj->cfg);
    MBED_ASSERT(err == FSP_SUCCESS);

#if MBED_CONF_TARGET_CONSOLE_UART
    // For stdio management in platform/mbed_board.c and platform/mbed_retarget.cpp
    if (stdio_config) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
#endif
}

void serial_free(serial_t *obj)
{
    MBED_ASSERT(obj);

    obj->p_api->callbackSet(obj->p_ctrl, NULL, NULL, NULL);
    g_rx_irq_enabled[obj->instance_index] = 0;
    g_tx_irq_enabled[obj->instance_index] = 0;
}

/* ---------------- Mbed API: baud/format ---------------- */

void serial_baud(serial_t *obj, int baudrate)
{
    MBED_ASSERT(obj);

    baud_setting_t setting;
    fsp_err_t err = R_SCI_UART_BaudCalculate(baudrate, true, 500, &setting);
    if (err == FSP_SUCCESS) {
        obj->p_api->baudSet(obj->p_ctrl, &setting);
    }
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    MBED_ASSERT(obj);

    uart_cfg_t *cfg = &obj->cfg;

    switch (data_bits) {
        case 7: cfg->data_bits = UART_DATA_BITS_7; break;
        case 8:
        default: cfg->data_bits = UART_DATA_BITS_8; break;
    }

    switch (parity) {
        case ParityOdd:  cfg->parity = UART_PARITY_ODD;  break;
        case ParityEven: cfg->parity = UART_PARITY_EVEN; break;
        default:         cfg->parity = UART_PARITY_OFF;  break;
    }

    cfg->stop_bits = (stop_bits == 2) ? UART_STOP_BITS_2 : UART_STOP_BITS_1;

    obj->p_api->close(obj->p_ctrl);
    obj->p_api->open(obj->p_ctrl, cfg);

    void uartCallback(uart_callback_args_t *p_args);
    obj->p_api->callbackSet(obj->p_ctrl, &uartCallback, NULL, NULL);
}

/* ---------------- Mbed API: blocking getc/putc ---------------- */

int serial_getc(serial_t *obj)
{
    int idx = obj->instance_index;

    while (rxbuf_is_empty(idx)) {
        __NOP();
    }

    return rxbuf_pop(idx);
}

void serial_putc(serial_t *obj, int c)
{
    sci_uart_instance_ctrl_t *ctrl = obj->p_ctrl;
    R_SCI0_Type *reg = ctrl->p_reg;
    if(ctrl->fifo_depth > 0)
    {
#if BSP_PERIPHERAL_SCI_B_PRESENT
        while(reg->FTSR_b.T == ctrl->fifo_depth);
        reg->TDR_BY = (uint8_t)c;
#else
        while(reg->FDR_b.T == ctrl->fifo_depth);
        reg->FTDRHL = (uint16_t)c;
#endif
    }
    else
    {
        reg->TDR = (uint8_t) c;
        while (reg->SSR_b.TEND == 0);
    }
}



int serial_readable(serial_t *obj)
{
    return !rxbuf_is_empty(obj->instance_index);
}

int serial_writable(serial_t *obj)
{
    (void) obj;
    return 1;
}

/* ---------------- Mbed API: IRQ ---------------- */

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    (void)obj;
    g_irq_handler = handler;

    if (obj) {
        g_irq_id[obj->instance_index] = id;
    }
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    MBED_ASSERT(obj);

    if (irq == RxIrq) g_rx_irq_enabled[obj->instance_index] = enable;
    if (irq == TxIrq) g_tx_irq_enabled[obj->instance_index] = enable;
}

void serial_break_set(serial_t *obj)
{
    (void)obj;
}

void serial_break_clear(serial_t *obj)
{
    (void)obj;
}

/* ---------------- UART callback ---------------- */

void uart_callback(uart_callback_args_t *p_args)
{
    serial_t *obj = (serial_t *) p_args->p_context;
    int idx = obj->instance_index;

    switch (p_args->event)
    {
        case UART_EVENT_RX_CHAR:
        {
            rxbuf_push(idx, (uint8_t)p_args->data);

            sci_uart_instance_ctrl_t *p_ctrl = obj->p_ctrl;
            if (p_ctrl->fifo_depth > 0U) {
#if BSP_PERIPHERAL_SCI_B_PRESENT
                while (p_ctrl->p_reg->FRSR_b.R > 0U) {
                    uint8_t data = p_ctrl->p_reg->RDR_BY & 0xFF;
#else
                while (p_ctrl->p_reg->FDR_b.R > 0U) {
                    uint8_t data = p_ctrl->p_reg->FRDRHL & 0xFF;
#endif
                    rxbuf_push(idx, data);
                    if (rxbuf_is_full(idx)) {
                        break;
                    }
                }
            }

            if (g_rx_irq_enabled[idx]) {
                g_irq_handler(g_irq_id[idx], RxIrq);
            }
            break;
        }

        case UART_EVENT_RX_COMPLETE:
            if (g_rx_irq_enabled[idx])
                g_irq_handler(g_irq_id[idx], RxIrq);
            break;

        case UART_EVENT_TX_DATA_EMPTY:
        case UART_EVENT_TX_COMPLETE:
            if (g_tx_irq_enabled[idx])
                g_irq_handler(g_irq_id[idx], TxIrq);
            break;

        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
        case UART_EVENT_BREAK_DETECT:
            if (g_rx_irq_enabled[idx])
                g_irq_handler(g_irq_id[idx], RxIrq);
            break;
    }
}

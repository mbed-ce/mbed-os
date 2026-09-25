/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed_assert.h"
#include "mbed_error.h"
#include "serial_api.h"
#include "pinmap.h"
#include "hal_data.h"
#include "mbed_atomic.h"
#include "mbed_critical.h"

#define RX_BUF_SIZE 16

#if MBED_CONF_TARGET_CONSOLE_UART
int stdio_uart_inited = 0; // used in platform/mbed_board.c and platform/mbed_retarget.cpp
serial_t stdio_uart;
#endif

extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];

extern uart_instance_t* const g_uart_instances[];

static uart_irq_handler g_irq_handler = NULL;
static uint32_t g_irq_id[UART_COUNT];

static volatile bool g_rx_irq_enabled[UART_COUNT] = {};
static volatile bool g_tx_irq_enabled[UART_COUNT] = {};

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
    for(int instance_index = 0; instance_index < UART_COUNT; instance_index++) {
        if(g_uart_instances[instance_index]->p_cfg->channel == channel) {
            return instance_index;
        }
    }
    return -1;
}

/* ---------------- Mbed API: init/free ---------------- */

static void sci_configure_tx_interrupts(serial_t * const obj) {
#if BSP_PERIPHERAL_SCI_B_PRESENT
    obj->p_ctrl->p_reg->CCR0 |= R_SCI_B0_CCR0_TIE_Msk;
    obj->p_ctrl->p_reg->CCR0 &= ~R_SCI_B0_CCR0_TEIE_Msk;

    // Set Tx data empty interrupt threshold to 1/4 of the way full so that, e.g.,
    // we interrupt the core when it can write 12 bytes out of a 16 byte FIFO. The default is to interrupt the core
    // only when the FIFO is completely empty, which could lead to a gap in transmission if the core
    // does not respond quickly.
    obj->p_ctrl->p_reg->FCR_b.TTRG = obj->p_ctrl->fifo_depth / 4;

    // Clear TDRE flag in case it got set from the BSP enabling TEI interrupt
    obj->p_ctrl->p_reg->CFCLR = R_SCI_B0_CFCLR_TDREC_Msk;
#else
    obj->p_ctrl->p_reg->SCR |= R_SCI0_SCR_TIE_Msk | SCI_SCR_TE_MASK;
#endif
}

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
    obj->has_rx_char_from_callback = false;

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

    g_rx_irq_enabled[obj->instance_index] = false;
    g_tx_irq_enabled[obj->instance_index] = false;

    fsp_err_t err = obj->p_api->open(obj->p_ctrl, &obj->cfg);
    MBED_ASSERT(err == FSP_SUCCESS);

    // Disable interrupts for now, until explicitly enabled.
    R_BSP_IrqDisable(obj->p_ctrl->p_cfg->rxi_irq);
    R_BSP_IrqDisable(obj->p_ctrl->p_cfg->eri_irq);
    R_BSP_IrqDisable(obj->p_ctrl->p_cfg->txi_irq);
    R_BSP_IrqDisable(obj->p_ctrl->p_cfg->tei_irq);

    // Renesas BSP does not turn on Tx interrupts until you call the write() function.
    // We operate at a lower level so we need them on (though the interrupt cannot actually fire
    // yet due to above)
    sci_configure_tx_interrupts(obj);

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
    obj->p_api->close(obj->p_ctrl);
}

/* ---------------- Mbed API: baud/format ---------------- */

void serial_baud(serial_t *obj, int baudrate)
{
    MBED_ASSERT(obj);

    fsp_err_t err = R_SCI_UART_BaudCalculate(baudrate, true, 500, obj->ext.p_baud_setting);
    if (err == FSP_SUCCESS) {
        obj->p_api->baudSet(obj->p_ctrl, obj->ext.p_baud_setting);
    }

    // For some reason changing the baudrate causes the BSP to disable transmit interrupts
    sci_configure_tx_interrupts(obj);
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

    // Note: Baudrate from earlier will be remembered via `obj->ext.p_baud_setting`
    obj->p_api->open(obj->p_ctrl, cfg);

    // Restore interrupt state cleared by `open`
    obj->p_api->callbackSet(obj->p_ctrl, &uart_callback, obj, NULL);
    serial_irq_set(obj, RxIrq, g_rx_irq_enabled[obj->instance_index]);
    serial_irq_set(obj, TxIrq, g_tx_irq_enabled[obj->instance_index]);
    sci_configure_tx_interrupts(obj);
}

/* ---------------- Mbed API: blocking getc/putc ---------------- */

int serial_getc(serial_t *obj)
{
    while (!serial_readable(obj)) {
        __NOP();
    }

    if(obj->has_rx_char_from_callback) {
        obj->has_rx_char_from_callback = false;
        const char result = obj->rx_char_from_callback;
        core_util_critical_section_exit();
        return result;
    }

#if BSP_PERIPHERAL_SCI_B_PRESENT
    return obj->p_ctrl->p_reg->RDR_BY & 0xFF;
#else
    return obj->p_ctrl->p_reg->FRDRHL & 0xFF;
#endif

}

void serial_putc(serial_t *obj, int c)
{
    sci_uart_instance_ctrl_t *ctrl = obj->p_ctrl;

    while(!serial_writable(obj)) {}

#if BSP_PERIPHERAL_SCI_B_PRESENT
    ctrl->p_reg->TDR_BY = (uint8_t)c;

    // Clear TDRE flag in case it was set. Seems to be undocumented, but if we don't do this, the TXI
    // interrupt is never triggered again after triggering once. I only figured this out by reading BSP code...
    obj->p_ctrl->p_reg->CFCLR = R_SCI_B0_CFCLR_TDREC_Msk;
#else
    ctrl->p_reg->FTDRHL = (uint16_t)c;
#endif
}

int serial_readable(serial_t *obj)
{
    if(obj->has_rx_char_from_callback) {
        return true;
    }

    // Check if there is at least 1 byte in the Rx FIFO
#if BSP_PERIPHERAL_SCI_B_PRESENT
    return obj->p_ctrl->p_reg->FRSR_b.R > 0U;
#else
    return obj->p_ctrl->p_reg->FDR_b.R > 0U;
#endif
}

int serial_writable(serial_t *obj)
{
    // Check if the Tx FIFO is not full
#if BSP_PERIPHERAL_SCI_B_PRESENT
    return obj->p_ctrl->p_reg->FTSR_b.T < obj->p_ctrl->fifo_depth;
#else
    return obj->p_ctrl->p_reg->FDR_b.T < obj->p_ctrl->fifo_depth;
#endif
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

    if (irq == RxIrq) {
        if(enable) {
            R_BSP_IrqEnableNoClear(obj->p_ctrl->p_cfg->rxi_irq); // Rx
            R_BSP_IrqEnableNoClear(obj->p_ctrl->p_cfg->eri_irq); // Rx error
        }
        else {
            R_BSP_IrqDisable(obj->p_ctrl->p_cfg->rxi_irq);
            R_BSP_IrqDisable(obj->p_ctrl->p_cfg->eri_irq);
        }
        g_rx_irq_enabled[obj->instance_index] = enable;
    }
    else if (irq == TxIrq) {
        if(enable) {
            R_BSP_IrqEnableNoClear(obj->p_ctrl->p_cfg->txi_irq); // Transmit empty
        }
        else {
            R_BSP_IrqDisable(obj->p_ctrl->p_cfg->txi_irq);
        }
        g_tx_irq_enabled[obj->instance_index] = enable;
    }
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

void mbed_sci_txi_isr() {
    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Clear pending IRQ to make sure it doesn't fire again after exiting */
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    sci_b_uart_instance_ctrl_t * const p_ctrl = (sci_b_uart_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    serial_t * const obj = p_ctrl->p_context;

    // Call Mbed OS IRQ handler
    if (g_tx_irq_enabled[obj->instance_index])
        g_irq_handler(g_irq_id[obj->instance_index], TxIrq);
}

void uart_callback(uart_callback_args_t *p_args)
{
    serial_t *obj = (serial_t *) p_args->p_context;
    MBED_ASSERT(obj != NULL);

    int idx = obj->instance_index;

    switch (p_args->event)
    {
        case UART_EVENT_RX_CHAR:
        {
            // This event means that the Renesas BSP has already read the first char
            // and stored it in the event args.
            obj->has_rx_char_from_callback = true;
            obj->rx_char_from_callback = (char)p_args->data;

            if (g_rx_irq_enabled[idx]) {
                g_irq_handler(g_irq_id[idx], RxIrq);
            }
            break;
        }

        case UART_EVENT_RX_COMPLETE:
            if (g_rx_irq_enabled[idx])
                g_irq_handler(g_irq_id[idx], RxIrq);
            break;

        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
            if (g_rx_irq_enabled[idx])
                g_irq_handler(g_irq_id[idx], RxIrq);
            break;
        default:
            break;
    }
}

const PinMap *serial_tx_pinmap()
{
    return PinMap_UART_TX;
}

const PinMap *serial_rx_pinmap()
{
    return PinMap_UART_RX;
}


/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if DEVICE_SERIAL

#include "serial_api_hal.h"

#if defined (STM32H503xx) 
#define UART_NUM (3)
#else
#define UART_NUM (11)
#endif

uint32_t serial_irq_ids[UART_NUM] = {0};
UART_HandleTypeDef uart_handlers[UART_NUM];

static uart_irq_handler irq_handler;

// Defined in serial_api.c
extern int8_t get_uart_index(UARTName uart_name);

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

static void uart_irq(UARTName uart_name)
{
    int8_t id = get_uart_index(uart_name);

    if (id >= 0) {
        UART_HandleTypeDef *huart = &uart_handlers[id];
        if (serial_irq_ids[id] != 0) {
            if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) != RESET) {
                if (__HAL_UART_GET_IT(huart, UART_IT_TXE) != RESET && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE)) {
                    irq_handler(serial_irq_ids[id], TxIrq);
                }
            }
            if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET) {
                if (__HAL_UART_GET_IT(huart, UART_IT_RXNE) != RESET && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE)) {
                    irq_handler(serial_irq_ids[id], RxIrq);
                    /* Flag has been cleared when reading the content */
                }
            }
            if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
                if (__HAL_UART_GET_IT(huart, UART_IT_ORE) != RESET) {
                    __HAL_UART_CLEAR_IT(huart, UART_CLEAR_OREF);
                }
            }
        }
    }
}

#if defined(USART1_BASE)
static void uart1_irq(void)
{
    uart_irq(UART_1);
}
#endif

#if defined(USART2_BASE)
static void uart2_irq(void)
{
    uart_irq(UART_2);
}
#endif

#if defined(USART3_BASE)
static void uart3_irq(void)
{
    uart_irq(UART_3);
}
#endif

#if defined(UART4_BASE)
static void uart4_irq(void)
{
    uart_irq(UART_4);
}
#endif

#if defined(UART5_BASE)
static void uart5_irq(void)
{
    uart_irq(UART_5);
}
#endif

#if defined(USART6_BASE)
static void uart6_irq(void)
{
    uart_irq(UART_6);
}
#endif

#if defined(UART7_BASE)
static void uart7_irq(void)
{
    uart_irq(UART_7);
}
#endif

#if defined(UART8_BASE)
static void uart8_irq(void)
{
    uart_irq(UART_8);
}
#endif

#if defined(UART9_BASE)
static void uart9_irq(void)
{
    uart_irq(UART_9);
}
#endif

#if defined(USART10_BASE)
static void uart10_irq(void)
{
    uart_irq(UART_10);
}
#endif

#if defined(USART11_BASE)
static void uart11_irq(void)
{
    uart_irq(UART_11);
}
#endif

#if defined(UART12_BASE)
static void uart12_irq(void)
{
    uart_irq(UART_12);
}
#endif


#if defined(LPUART1_BASE)
static void lpuart1_irq(void)
{
    uart_irq(LPUART_1);
}
#endif

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    struct serial_s *obj_s = SERIAL_S(obj);

    irq_handler = handler;
    serial_irq_ids[obj_s->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;

    switch (obj_s->uart) {
#if defined(USART1_BASE)
        case UART_1:
            irq_n = USART1_IRQn;
            vector = (uint32_t)&uart1_irq;
            break;
#endif
#if defined(USART2_BASE)
        case UART_2:
            irq_n = USART2_IRQn;
            vector = (uint32_t)&uart2_irq;
            break;
#endif
#if defined(USART3_BASE)
        case UART_3:
            irq_n = USART3_IRQn;
            vector = (uint32_t)&uart3_irq;
            break;
#endif
#if defined(UART4_BASE)
        case UART_4:
            irq_n = UART4_IRQn;
            vector = (uint32_t)&uart4_irq;
            break;
#endif
#if defined(UART5_BASE)
        case UART_5:
            irq_n = UART5_IRQn;
            vector = (uint32_t)&uart5_irq;
            break;
#endif
#if defined(USART6_BASE)
        case UART_6:
            irq_n = USART6_IRQn;
            vector = (uint32_t)&uart6_irq;
            break;
#endif
#if defined(UART7_BASE)
        case UART_7:
            irq_n = UART7_IRQn;
            vector = (uint32_t)&uart7_irq;
            break;
#endif
#if defined(UART8_BASE)
        case UART_8:
            irq_n = UART8_IRQn;
            vector = (uint32_t)&uart8_irq;
            break;
#endif
#if defined(UART9_BASE)
        case UART_9:
            irq_n = UART9_IRQn;
            vector = (uint32_t)&uart9_irq;
            break;
#endif
#if defined(USART10_BASE)
        case UART_10:
            irq_n = USART10_IRQn;
            vector = (uint32_t)&uart10_irq;
            break;
#endif
#if defined(USART11_BASE)
        case UART_11:
            irq_n = USART11_IRQn;
            vector = (uint32_t)&uart11_irq;
            break;
#endif
#if defined(UART12_BASE)
        case UART_12:
            irq_n = UART12_IRQn;
            vector = (uint32_t)&uart12_irq;
            break;
#endif
#if defined(LPUART1_BASE)
        case LPUART_1:
            irq_n = LPUART1_IRQn;
            vector = (uint32_t)&lpuart1_irq;
            break;
#endif
    }

    if (enable) {
        if (irq == RxIrq) {
            __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        } else { // TxIrq
            __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);
        }
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);
    } else { // disable
        int all_disabled = 0;
        if (irq == RxIrq) {
            __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
            // Check if TxIrq is disabled too
            if ((huart->Instance->CR1 & USART_CR1_TXEIE) == 0) {
                all_disabled = 1;
            }
        } else { // TxIrq
            __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
            // Check if RxIrq is disabled too
            if ((huart->Instance->CR1 & USART_CR1_RXNEIE) == 0) {
                all_disabled = 1;
            }
        }

        if (all_disabled) {
            NVIC_DisableIRQ(irq_n);
        }
    }
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/

int serial_getc(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    /* Computation of UART mask to apply to RDR register */
    UART_MASK_COMPUTATION(huart);
    uint16_t uhMask = huart->Mask;

    while (!serial_readable(obj));
    /* When receiving with the parity enabled, the value read in the MSB bit
     * is the received parity bit.
     */
    return (int)(huart->Instance->RDR & uhMask);
}

void serial_putc(serial_t *obj, int c)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    while (!serial_writable(obj));
    /* When transmitting with the parity enabled (PCE bit set to 1 in the
     * USART_CR1 register), the value written in the MSB (bit 7 or bit 8
     * depending on the data length) has no effect because it is replaced
     * by the parity.
     */
    huart->Instance->TDR = (uint16_t)(c & 0x1FFU);
}

void serial_clear(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    /* Clear RXNE and error flags */
    volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR;
    HAL_UART_ErrorCallback(huart);
}

void serial_break_set(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    HAL_LIN_SendBreak(huart);
}

#if DEVICE_SERIAL_ASYNCH

/******************************************************************************
 * LOCAL HELPER FUNCTIONS
 ******************************************************************************/

/**
 * Configure the TX buffer for an asynchronous write serial transaction
 *
 * @param obj       The serial object.
 * @param tx        The buffer for sending.
 * @param tx_length The number of words to transmit.
 */
static void serial_tx_buffer_set(serial_t *obj, void *tx, int tx_length, uint8_t width)
{
    (void)width;

    // Exit if a transmit is already on-going
    if (serial_tx_active(obj)) {
        return;
    }

    obj->tx_buff.buffer = tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
}

/**
 * Configure the RX buffer for an asynchronous write serial transaction
 *
 * @param obj       The serial object.
 * @param tx        The buffer for sending.
 * @param tx_length The number of words to transmit.
 */
static void serial_rx_buffer_set(serial_t *obj, void *rx, int rx_length, uint8_t width)
{
    (void)width;

    // Exit if a reception is already on-going
    if (serial_rx_active(obj)) {
        return;
    }

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
}

/**
 * Configure events
 *
 * @param obj    The serial object
 * @param event  The logical OR of the events to configure
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
static void serial_enable_event(serial_t *obj, int event, uint8_t enable)
{
    struct serial_s *obj_s = SERIAL_S(obj);

    // Shouldn't have to enable interrupt here, just need to keep track of the requested events.
    if (enable) {
        obj_s->events |= event;
    } else {
        obj_s->events &= ~event;
    }
}


/**
* Get index of serial object TX IRQ, relating it to the physical peripheral.
*
* @param uart_name i.e. UART_1, UART_2, ...
* @return internal NVIC TX IRQ index of U(S)ART peripheral
*/
static IRQn_Type serial_get_irq_n(UARTName uart_name)
{
    IRQn_Type irq_n;

    switch (uart_name) {
#if defined(USART1_BASE)
        case UART_1:
            irq_n = USART1_IRQn;
            break;
#endif
#if defined(USART2_BASE)
        case UART_2:
            irq_n = USART2_IRQn;
            break;
#endif
#if defined(USART3_BASE)
        case UART_3:
            irq_n = USART3_IRQn;
            break;
#endif
#if defined(UART4_BASE)
        case UART_4:
            irq_n = UART4_IRQn;
            break;
#endif
#if defined(UART5_BASE)
        case UART_5:
            irq_n = UART5_IRQn;
            break;
#endif
#if defined(USART6_BASE)
        case UART_6:
            irq_n = USART6_IRQn;
            break;
#endif
#if defined(UART7_BASE)
        case UART_7:
            irq_n = UART7_IRQn;
            break;
#endif
#if defined(UART8_BASE)
        case UART_8:
            irq_n = UART8_IRQn;
            break;
#endif
#if defined(UART9_BASE)
        case UART_9:
            irq_n = UART9_IRQn;
            break;
#endif
#if defined(USART10_BASE)
        case UART_10:
            irq_n = USART10_IRQn;
            break;
#endif
        default:
            irq_n = (IRQn_Type)0;
    }

    return irq_n;
}

/******************************************************************************
 * MBED API FUNCTIONS
 ******************************************************************************/

/**
 * Begin asynchronous TX transfer. The used buffer is specified in the serial
 * object, tx_buff
 *
 * @param obj       The serial object
 * @param tx        The buffer for sending
 * @param tx_length The number of words to transmit
 * @param tx_width  The bit width of buffer word
 * @param handler   The serial handler
 * @param event     The logical OR of events to be registered
 * @param hint      A suggestion for how to use DMA with this transfer
 * @return Returns number of data transfered, or 0 otherwise
 */
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    // TODO: DMA usage is currently ignored
    (void) hint;

    // Check buffer is ok
    MBED_ASSERT(tx != (void *)0);
    MBED_ASSERT(tx_width == 8); // support only 8b width

    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    if (tx_length == 0) {
        return 0;
    }

    // Set up buffer
    serial_tx_buffer_set(obj, (void *)tx, tx_length, tx_width);

    // Set up events
    serial_enable_event(obj, SERIAL_EVENT_TX_ALL, 0); // Clear all events
    serial_enable_event(obj, event, 1); // Set only the wanted events

    // Enable interrupt
    IRQn_Type irq_n = serial_get_irq_n(obj_s->uart);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);
    NVIC_SetPriority(irq_n, 1);
    NVIC_SetVector(irq_n, (uint32_t)handler);
    NVIC_EnableIRQ(irq_n);

    // the following function will enable UART_IT_TXE and error interrupts
    if (HAL_UART_Transmit_IT(huart, (uint8_t *)tx, tx_length) != HAL_OK) {
        return 0;
    }

    return tx_length;
}

/**
 * Begin asynchronous RX transfer (enable interrupt for data collecting)
 * The used buffer is specified in the serial object, rx_buff
 *
 * @param obj        The serial object
 * @param rx         The buffer for sending
 * @param rx_length  The number of words to transmit
 * @param rx_width   The bit width of buffer word
 * @param handler    The serial handler
 * @param event      The logical OR of events to be registered
 * @param handler    The serial handler
 * @param char_match A character in range 0-254 to be matched
 * @param hint       A suggestion for how to use DMA with this transfer
 */
void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    // TODO: DMA usage is currently ignored
    (void) hint;

    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void *)0);
    MBED_ASSERT(rx_width == 8); // support only 8b width

    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    serial_enable_event(obj, SERIAL_EVENT_RX_ALL, 0);
    serial_enable_event(obj, event, 1);

    // set CharMatch
    obj->char_match = char_match;

    serial_rx_buffer_set(obj, rx, rx_length, rx_width);

    IRQn_Type irq_n = serial_get_irq_n(obj_s->uart);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);
    NVIC_SetPriority(irq_n, 0);
    NVIC_SetVector(irq_n, (uint32_t)handler);
    NVIC_EnableIRQ(irq_n);

    // following HAL function will enable the RXNE interrupt + error interrupts
    HAL_UART_Receive_IT(huart, (uint8_t *)rx, rx_length);
}

/**
 * Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the TX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
    MBED_ASSERT(obj);

    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    return (((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX) ? 1 : 0);
}

/**
 * Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
    MBED_ASSERT(obj);

    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    return (((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX) ? 1 : 0);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET) {
        __HAL_UART_CLEAR_IT(huart, UART_CLEAR_PEF);
    }
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
        __HAL_UART_CLEAR_IT(huart, UART_CLEAR_FEF);
    }
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) != RESET) {
        __HAL_UART_CLEAR_IT(huart, UART_CLEAR_NEF);
    }
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
        __HAL_UART_CLEAR_IT(huart, UART_CLEAR_OREF);
    }
}

/**
 * The asynchronous TX and RX handler.
 *
 * @param obj The serial object
 * @return Returns event flags if a TX/RX transfer termination condition was met or 0 otherwise
 */
int serial_irq_handler_asynch(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    volatile int return_event = 0;
    uint8_t *buf = (uint8_t *)(obj->rx_buff.buffer);
    size_t i = 0;

    // TX PART:
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) != RESET) {
        if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC) != RESET) {
            // Return event SERIAL_EVENT_TX_COMPLETE if requested
            if ((obj_s->events & SERIAL_EVENT_TX_COMPLETE) != 0) {
                return_event |= (SERIAL_EVENT_TX_COMPLETE & obj_s->events);
            }
        }
    }

    // Handle error events
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET) {
        if (__HAL_UART_GET_IT(huart, UART_IT_PE) != RESET) {
            return_event |= (SERIAL_EVENT_RX_PARITY_ERROR & obj_s->events);
        }
    }

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
        if (__HAL_UART_GET_IT(huart, UART_IT_FE) != RESET) {
            return_event |= (SERIAL_EVENT_RX_FRAMING_ERROR & obj_s->events);
        }
    }

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
        if (__HAL_UART_GET_IT(huart, UART_IT_ORE) != RESET) {
            return_event |= (SERIAL_EVENT_RX_OVERRUN_ERROR & obj_s->events);
        }
    }

    HAL_UART_IRQHandler(huart);

    // Abort if an error occurs
    if ((return_event & SERIAL_EVENT_RX_PARITY_ERROR) ||
            (return_event & SERIAL_EVENT_RX_FRAMING_ERROR) ||
            (return_event & SERIAL_EVENT_RX_OVERRUN_ERROR)) {
        return return_event;
    }

    //RX PART
    if (huart->RxXferSize != 0) {
        obj->rx_buff.pos = huart->RxXferSize - huart->RxXferCount;
    }
    if ((huart->RxXferCount == 0) && (obj->rx_buff.pos >= (obj->rx_buff.length - 1))) {
        return_event |= (SERIAL_EVENT_RX_COMPLETE & obj_s->events);
    }

    // Check if char_match is present
    if (obj_s->events & SERIAL_EVENT_RX_CHARACTER_MATCH) {
        if (buf != NULL) {
            for (i = 0; i < obj->rx_buff.pos; i++) {
                if (buf[i] == obj->char_match) {
                    obj->rx_buff.pos = i;
                    return_event |= (SERIAL_EVENT_RX_CHARACTER_MATCH & obj_s->events);
                    serial_rx_abort_asynch(obj);
                    break;
                }
            }
        }
    }

    return return_event;
}

/**
 * Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 * flush TX hardware buffer if TX FIFO is used
 *
 * @param obj The serial object
 */
void serial_tx_abort_asynch(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    __HAL_UART_DISABLE_IT(huart, UART_IT_TC);
    __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

    // reset states
    huart->TxXferCount = 0;
    // update handle state
    if (huart->gState == HAL_UART_STATE_BUSY_TX_RX) {
        huart->gState = HAL_UART_STATE_BUSY_RX;
    } else {
        huart->gState = HAL_UART_STATE_READY;
    }
}

/**
 * Abort the ongoing RX transaction It disables the enabled interrupt for RX and
 * flush RX hardware buffer if RX FIFO is used
 *
 * @param obj The serial object
 */
void serial_rx_abort_asynch(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    // disable interrupts
    __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(huart, UART_IT_PE);
    __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);

    // clear flags
    volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR; // Clear RXNE
    __HAL_UART_CLEAR_IT(huart, UART_CLEAR_PEF);
    __HAL_UART_CLEAR_IT(huart, UART_CLEAR_FEF);
    __HAL_UART_CLEAR_IT(huart, UART_CLEAR_NEF);
    __HAL_UART_CLEAR_IT(huart, UART_CLEAR_OREF);

    // reset states
    huart->RxXferCount = 0;
    // update handle state
    if (huart->RxState == HAL_UART_STATE_BUSY_TX_RX) {
        huart->RxState = HAL_UART_STATE_BUSY_TX;
    } else {
        huart->RxState = HAL_UART_STATE_READY;
    }
}

#endif /* DEVICE_SERIAL_ASYNCH */

#if DEVICE_SERIAL_FC

/**
 * Set HW Control Flow
 * @param obj    The serial object
 * @param type   The Control Flow type (FlowControlNone, FlowControlRTS, FlowControlCTS, FlowControlRTSCTS)
 * @param rxflow Pin for the rxflow
 * @param txflow Pin for the txflow
 */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    struct serial_s *obj_s = SERIAL_S(obj);

    // Checked used UART name (UART_1, UART_2, ...)
    UARTName uart_rts = (UARTName)pinmap_peripheral(rxflow, PinMap_UART_RTS);
    UARTName uart_cts = (UARTName)pinmap_peripheral(txflow, PinMap_UART_CTS);
    if (((UARTName)pinmap_merge(uart_rts, obj_s->uart) == (UARTName)NC) || ((UARTName)pinmap_merge(uart_cts, obj_s->uart) == (UARTName)NC)) {
        MBED_ASSERT(0);
        return;
    }

    if (type == FlowControlNone) {
        // Disable hardware flow control
        obj_s->hw_flow_ctl = UART_HWCONTROL_NONE;
    }
    if (type == FlowControlRTS) {
        // Enable RTS
        MBED_ASSERT(uart_rts != (UARTName)NC);
        obj_s->hw_flow_ctl = UART_HWCONTROL_RTS;
        obj_s->pin_rts = rxflow;
        // Enable the pin for RTS function
        pinmap_pinout(rxflow, PinMap_UART_RTS);
    }
    if (type == FlowControlCTS) {
        // Enable CTS
        MBED_ASSERT(uart_cts != (UARTName)NC);
        obj_s->hw_flow_ctl = UART_HWCONTROL_CTS;
        obj_s->pin_cts = txflow;
        // Enable the pin for CTS function
        pinmap_pinout(txflow, PinMap_UART_CTS);
    }
    if (type == FlowControlRTSCTS) {
        // Enable CTS & RTS
        MBED_ASSERT(uart_rts != (UARTName)NC);
        MBED_ASSERT(uart_cts != (UARTName)NC);
        obj_s->hw_flow_ctl = UART_HWCONTROL_RTS_CTS;
        obj_s->pin_rts = rxflow;
        obj_s->pin_cts = txflow;
        // Enable the pin for CTS function
        pinmap_pinout(txflow, PinMap_UART_CTS);
        // Enable the pin for RTS function
        pinmap_pinout(rxflow, PinMap_UART_RTS);
    }

    init_uart(obj);
}

#endif /* DEVICE_SERIAL_FC */

#endif /* DEVICE_SERIAL */

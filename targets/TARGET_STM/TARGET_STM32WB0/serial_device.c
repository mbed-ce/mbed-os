/* Copyright (c) 2019 STMicroelectronics
 * Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#if DEVICE_SERIAL

#include "serial_api_hal.h"

#define UART_NUM 2

uint32_t serial_irq_ids[UART_NUM] = {0};
UART_HandleTypeDef uart_handlers[UART_NUM];

static uart_irq_handler irq_handler;

extern int8_t get_uart_index(UARTName uart_name);
extern int stdio_uart_inited;
extern serial_t stdio_uart;

void serial_restore_stdio(void)
{
    if (stdio_uart_inited) {
        struct serial_s *obj_s = SERIAL_S(&stdio_uart);
        serial_init(&stdio_uart, obj_s->pin_tx, obj_s->pin_rx);
    }
}

static void uart_irq(UARTName uart_name)
{
    int8_t id = get_uart_index(uart_name);

    if ((id < 0) || (serial_irq_ids[id] == 0)) {
        return;
    }

    UART_HandleTypeDef *huart = &uart_handlers[id];

    if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) != RESET) &&
            (__HAL_UART_GET_IT(huart, UART_IT_TXE) != RESET) &&
            __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE)) {
        irq_handler(serial_irq_ids[id], TxIrq);
    }

    if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET) &&
            (__HAL_UART_GET_IT(huart, UART_IT_RXNE) != RESET) &&
            __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE)) {
        irq_handler(serial_irq_ids[id], RxIrq);
    }

    if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) &&
            (__HAL_UART_GET_IT(huart, UART_IT_ORE) != RESET)) {
        __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF);
    }
}

static void uart1_irq(void)
{
    uart_irq(UART_1);
}

static void lpuart1_irq(void)
{
    uart_irq(LPUART_1);
}

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
    IRQn_Type irq_n;
    uint32_t vector;

    if (obj_s->uart == UART_1) {
        irq_n = USART1_IRQn;
        vector = (uint32_t)&uart1_irq;
    } else {
        irq_n = LPUART1_IRQn;
        vector = (uint32_t)&lpuart1_irq;
    }

    if (enable) {
        if (irq == RxIrq) {
            __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        } else {
            __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);
        }
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);
    } else {
        if (irq == RxIrq) {
            __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
        } else {
            __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
        }

        if (!__HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE) &&
                !__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE)) {
            NVIC_DisableIRQ(irq_n);
        }
    }
}

int serial_getc(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    UART_MASK_COMPUTATION(huart);
    while (!serial_readable(obj)) {
    }
    return (int)(huart->Instance->RDR & huart->Mask);
}

void serial_putc(serial_t *obj, int c)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    while (!serial_writable(obj)) {
    }
    huart->Instance->TDR = (uint16_t)(c & 0x1FFU);
}

void serial_clear(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    volatile uint32_t unused = huart->Instance->RDR;

    (void)unused;
    HAL_UART_ErrorCallback(huart);
}

void serial_break_set(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    __HAL_UART_SEND_REQ(huart, UART_SENDBREAK_REQUEST);
}

#endif /* DEVICE_SERIAL */

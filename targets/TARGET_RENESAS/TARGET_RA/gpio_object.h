/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MBED_GPIO_OBJECT_H
#define MBED_GPIO_OBJECT_H

#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName pin;
    uint32_t port;
    uint32_t pin_mask;
} gpio_t;

void gpio_init(gpio_t *obj, PinName pin);
void gpio_mode(gpio_t *obj, PinMode mode);
void gpio_dir(gpio_t *obj, PinDirection direction);
void gpio_write(gpio_t *obj, int value);
int  gpio_read(gpio_t *obj);

#ifdef __cplusplus
}
#endif

#endif

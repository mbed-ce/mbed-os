
/*
 * Custom reset reason HAL implementation for STM32H7-based Arduinos.
 * On these devices, the bootloader reads the Mbed reset reason and stores it in RTC backup register 8.
 * See here: https://forum.arduino.cc/t/last-reset-reason-is-always-unknown/1318139/6
 *
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) 2026 Jamie Smith
 */

#include "reset_reason_api.h"
#include "stm32h7xx.h"

reset_reason_t hal_reset_reason_get(void) {
    HAL_PWR_EnableBkUpAccess();
    return RTC->BKP8R;
}

uint32_t hal_reset_reason_get_raw(void) {
    return hal_reset_reason_get();
}

void hal_reset_reason_clear(void) {
    RTC->BKP8R = RESET_REASON_UNKNOWN;
}
/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "PeripheralNames.h"
#include "hal_data.h"
#include "device.h"

const can_instance_t * const g_can_instances[CAN_COUNT] = {
    &g_canfd0,
    &g_canfd1,
};

canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH0_RULE_NUM];
canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH1_RULE_NUM];

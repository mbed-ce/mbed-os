/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "PeripheralNames.h"
#include "common_data.h"
#include "device.h"
#include "mbed_critical.h"

/* The RA8P1 ICU provides 32 external IRQ channels (IRQ0-IRQ31), but the ICU
 * vector table only has 96 NVIC entries which must be shared with all other
 * peripherals, so a fixed one-vector-per-channel mapping is not affordable.
 *
 * Instead, the vector table reserves the 12 entries listed in
 * g_ext_irq_vector_slots (the "IRQ allocation table") for external IRQs.
 * When gpio_irq_init() needs a channel, one of these slots is allocated and
 * the channel's IRQ event is linked to it by writing the slot's IELSR
 * register (event link), exactly like the BSP does at startup for the
 * statically mapped peripherals (see bsp_irq_cfg()). The link is removed
 * again in gpio_irq_free(). Up to 12 external pin interrupts can therefore
 * be active at the same time, regardless of which IRQ channels the pins
 * belong to. */

/* Number of vector table entries reserved for external IRQs. Must match the
 * r_icu_isr entries of g_vector_table in vector_data.c. Boards/cores can
 * override both the count and the slot list through PeripheralNames.h (the
 * Cortex-M33 build, for example, uses the CPU1-local ICU with a different
 * vector table layout). */
#ifndef EXT_IRQ_SLOT_COUNT
#define EXT_IRQ_SLOT_COUNT        (12)
#endif

/* Interrupt priority used for all dynamically allocated external IRQs. */
#define EXT_IRQ_IPL               (12)

/* IRQ allocation table: NVIC vector indices reserved for external IRQs.
 * These are the only g_vector_table entries that hold r_icu_isr; their
 * IELSR registers stay 0 (unlinked) until a channel is allocated. */
#ifndef EXT_IRQ_VECTOR_SLOTS
#define EXT_IRQ_VECTOR_SLOTS      { 3, 8, 9, 10, 11, 12, 13, 14, 17, 22, 23, 24 }
#endif

static const IRQn_Type g_ext_irq_vector_slots[EXT_IRQ_SLOT_COUNT] =
    EXT_IRQ_VECTOR_SLOTS;

/* Bitmask of allocated slots (bit n == g_ext_irq_vector_slots[n] in use). */
static uint16_t g_ext_irq_slot_used;

/* Allocation state per ICU channel: index into g_ext_irq_vector_slots plus
 * one, 0 means the channel has no vector slot. */
static uint8_t g_ext_irq_channel_slot[IRQ_CHANNELS_COUNT];

/* Control blocks and configuration copies for all 32 IRQ channels. The
 * configurations live in RAM because their .irq member (the vector slot)
 * is only known when the channel is allocated. */
static icu_instance_ctrl_t  g_ext_irq_ctrl[IRQ_CHANNELS_COUNT];
static external_irq_cfg_t   g_ext_irq_cfg_ram[IRQ_CHANNELS_COUNT];

static const icu_extended_cfg_t g_ext_irq_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

icu_instance_ctrl_t * const g_icu_ctrl[IRQ_CHANNELS_COUNT] = {
    &g_ext_irq_ctrl[0],  &g_ext_irq_ctrl[1],  &g_ext_irq_ctrl[2],  &g_ext_irq_ctrl[3],
    &g_ext_irq_ctrl[4],  &g_ext_irq_ctrl[5],  &g_ext_irq_ctrl[6],  &g_ext_irq_ctrl[7],
    &g_ext_irq_ctrl[8],  &g_ext_irq_ctrl[9],  &g_ext_irq_ctrl[10], &g_ext_irq_ctrl[11],
    &g_ext_irq_ctrl[12], &g_ext_irq_ctrl[13], &g_ext_irq_ctrl[14], &g_ext_irq_ctrl[15],
    &g_ext_irq_ctrl[16], &g_ext_irq_ctrl[17], &g_ext_irq_ctrl[18], &g_ext_irq_ctrl[19],
    &g_ext_irq_ctrl[20], &g_ext_irq_ctrl[21], &g_ext_irq_ctrl[22], &g_ext_irq_ctrl[23],
    &g_ext_irq_ctrl[24], &g_ext_irq_ctrl[25], &g_ext_irq_ctrl[26], &g_ext_irq_ctrl[27],
    &g_ext_irq_ctrl[28], &g_ext_irq_ctrl[29], &g_ext_irq_ctrl[30], &g_ext_irq_ctrl[31],
};

const external_irq_cfg_t * const g_icu_cfg[IRQ_CHANNELS_COUNT] = {
    &g_ext_irq_cfg_ram[0],  &g_ext_irq_cfg_ram[1],  &g_ext_irq_cfg_ram[2],  &g_ext_irq_cfg_ram[3],
    &g_ext_irq_cfg_ram[4],  &g_ext_irq_cfg_ram[5],  &g_ext_irq_cfg_ram[6],  &g_ext_irq_cfg_ram[7],
    &g_ext_irq_cfg_ram[8],  &g_ext_irq_cfg_ram[9],  &g_ext_irq_cfg_ram[10], &g_ext_irq_cfg_ram[11],
    &g_ext_irq_cfg_ram[12], &g_ext_irq_cfg_ram[13], &g_ext_irq_cfg_ram[14], &g_ext_irq_cfg_ram[15],
    &g_ext_irq_cfg_ram[16], &g_ext_irq_cfg_ram[17], &g_ext_irq_cfg_ram[18], &g_ext_irq_cfg_ram[19],
    &g_ext_irq_cfg_ram[20], &g_ext_irq_cfg_ram[21], &g_ext_irq_cfg_ram[22], &g_ext_irq_cfg_ram[23],
    &g_ext_irq_cfg_ram[24], &g_ext_irq_cfg_ram[25], &g_ext_irq_cfg_ram[26], &g_ext_irq_cfg_ram[27],
    &g_ext_irq_cfg_ram[28], &g_ext_irq_cfg_ram[29], &g_ext_irq_cfg_ram[30], &g_ext_irq_cfg_ram[31],
};

/* Allocate a vector table slot for an ICU IRQ channel and link the channel's
 * event to it. Called from gpio_irq_init() before the channel is opened.
 * Returns 0 on success, -1 if no slot is free or the channel is invalid.
 * Acquiring an already mapped channel again succeeds without allocating a
 * second slot. */
int ra_gpio_irq_vector_acquire(uint32_t channel)
{
    int err = -1;

    if (channel >= IRQ_CHANNELS_COUNT)
        return -1;

    core_util_critical_section_enter();

    if (g_ext_irq_channel_slot[channel] != 0)
    {
        /* Channel already has a slot. */
        err = 0;
    }
    else
    {
        for (uint32_t idx = 0; idx < EXT_IRQ_SLOT_COUNT; idx++)
        {
            if (g_ext_irq_slot_used & (1U << idx))
                continue;

            IRQn_Type slot = g_ext_irq_vector_slots[idx];

            g_ext_irq_slot_used |= (uint16_t) (1U << idx);
            g_ext_irq_channel_slot[channel] = (uint8_t) (idx + 1U);

            /* Link the IRQ event of the channel to the vector slot.
             * Writing IELSR also clears the IR flag of the slot. */
            R_ICU->IELSR[slot] = (uint32_t) ELC_EVENT_ICU_IRQ0 + channel;

            external_irq_cfg_t *cfg = &g_ext_irq_cfg_ram[channel];
            cfg->channel         = (uint8_t) channel;
            cfg->trigger         = EXTERNAL_IRQ_TRIG_BOTH_EDGE;
            cfg->filter_enable   = true;
            cfg->clock_source_div = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64;
            cfg->p_callback      = external_irq_callback;
            cfg->p_context       = NULL;
            cfg->p_extend        = &g_ext_irq_ext_cfg;
            cfg->ipl             = EXT_IRQ_IPL;
            cfg->irq             = slot;

            err = 0;
            break;
        }
    }

    core_util_critical_section_exit();

    return err;
}

/* Release the vector table slot of an ICU IRQ channel: unlink the event,
 * drop a possibly pending interrupt and return the slot to the pool. */
void ra_gpio_irq_vector_release(uint32_t channel)
{
    if (channel >= IRQ_CHANNELS_COUNT)
        return;

    core_util_critical_section_enter();

    uint32_t idx_plus_1 = g_ext_irq_channel_slot[channel];
    if (idx_plus_1 != 0)
    {
        IRQn_Type slot = g_ext_irq_vector_slots[idx_plus_1 - 1U];

        R_BSP_IrqDisable(slot);
        R_ICU->IELSR[slot] = 0;       /* unlink event, clears IR */
        R_BSP_IrqClearPending(slot);

        g_ext_irq_slot_used &= (uint16_t) ~(1U << (idx_plus_1 - 1U));
        g_ext_irq_channel_slot[channel] = 0;
    }

    core_util_critical_section_exit();
}

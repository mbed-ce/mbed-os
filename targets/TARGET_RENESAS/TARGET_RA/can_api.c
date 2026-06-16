/* mbed Microcontroller Library
 * Copyright (c) 2024 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed_assert.h"
#include "mbed_error.h"
#include "can_api.h"
#include "pinmap.h"

#ifdef DEVICE_CAN

#ifdef DEVICE_CAN_FD
#include "r_canfd.h"
#else
#include "r_can.h"
#endif

static can_irq_handler irq_handler;

extern const can_instance_t *const g_can_instances[];

#define MBED_CAN_MAILBOX_TX 0

extern const PinMap PinMap_CAN_RD[];
extern const PinMap PinMap_CAN_TD[];

const PinMap *can_rd_pinmap(void)
{
    return PinMap_CAN_RD;
}

const PinMap *can_td_pinmap(void)
{
    return PinMap_CAN_TD;
}

static const can_instance_t *can_instance_from_pins(PinName rd, PinName td, int *peripheral_index)
{
    int per_rd = (int) pinmap_peripheral(rd, PinMap_CAN_RD);
    int per_td = (int) pinmap_peripheral(td, PinMap_CAN_TD);
    int per    = (int) pinmap_merge(per_rd, per_td);

    if (per == (int) NC) {
        return NULL;
    }

    if (peripheral_index) {
        *peripheral_index = per;
    }

    return g_can_instances[per];
}

static void can_configure_pins_from_pins(PinName rd, PinName td)
{
    int func_rd = (int) pinmap_function(rd, PinMap_CAN_RD);
    int func_td = (int) pinmap_function(td, PinMap_CAN_TD);

    pin_function(rd, func_rd);
    pin_function(td, func_td);
}

#ifdef DEVICE_CAN_FD
static void canfd_default_accept_all(const canfd_extended_cfg_t *ext)
{
    canfd_afl_entry_t *afl = (canfd_afl_entry_t *) ext->p_afl;

    afl[0].id.id = 0;
    afl[0].id.id_mode = CAN_ID_MODE_STANDARD;
    afl[0].id.frame_type = CAN_FRAME_TYPE_DATA;

    afl[0].mask.mask_id = 0;
    afl[0].mask.mask_id_mode = 0;
    afl[0].mask.mask_frame_type = 0;

    afl[0].destination.minimum_dlc = 0;
    afl[0].destination.fifo_select_flags = CANFD_RX_FIFO_0;
}

static uint32_t canfd_get_clock_div(uint32_t div_macro_value)
{
    switch(div_macro_value)
    {
    case BSP_CLOCKS_CANFD_CLOCK_DIV_1:
        return 1;
    case BSP_CLOCKS_CANFD_CLOCK_DIV_2:
        return 2;
    case BSP_CLOCKS_CANFD_CLOCK_DIV_3:
        return 3;
    case BSP_CLOCKS_CANFD_CLOCK_DIV_4:
        return 4;
    case BSP_CLOCKS_CANFD_CLOCK_DIV_5:
        return 5;
    case BSP_CLOCKS_CANFD_CLOCK_DIV_6:
        return 6;
    case BSP_CLOCKS_CANFD_CLOCK_DIV_8:
        return 8;
    case BSP_CLOCKS_CANFD_CLOCK_DIV_10:
        return 10;
    case BSP_CLOCKS_CANFD_CLOCK_DIV_16:
        return 16;
    case BSP_CLOCKS_CANFD_CLOCK_DIV_32:
        return 32;
    }
    return 1;
}
#endif

/* ---------------- Mbed API ---------------- */

void can_init(can_t *obj, PinName rd, PinName td)
{
    can_init_freq(obj, rd, td, 100000
#ifdef DEVICE_CAN_FD
                  , 0
#endif
                 );
}

void can_init_direct(can_t *obj, const can_pinmap_t *pinmap)
{
    can_init_freq_direct(obj, pinmap, 100000
#ifdef DEVICE_CAN_FD
                         , 0
#endif
                        );
}

void can_init_freq(can_t *obj, PinName rd, PinName td, int hz
#ifdef DEVICE_CAN_FD
                   , int data_hz
#endif
                  )
{
    MBED_ASSERT(obj);

    int per_index = -1;
    const can_instance_t *inst = can_instance_from_pins(rd, td, &per_index);
    MBED_ASSERT(inst != NULL);

    can_configure_pins_from_pins(rd, td);

    obj->instance     = inst;
    obj->irq_context  = 0;
    obj->hz           = hz;
#ifdef DEVICE_CAN_FD
    obj->data_hz      = data_hz;
    canfd_default_accept_all((const canfd_extended_cfg_t*)inst->p_cfg->p_extend);
#endif

    const can_cfg_t *cfg_rom = (const can_cfg_t *) inst->p_cfg;
    obj->cfg_copy            = *cfg_rom;
    obj->cfg_copy.p_context = obj;

    fsp_err_t err = obj->instance->p_api->open(obj->instance->p_ctrl, &obj->cfg_copy);
    MBED_ASSERT(FSP_SUCCESS == err);

    (void) can_frequency(obj, hz
#ifdef DEVICE_CAN_FD
                         , data_hz
#endif
                        );
}

void can_init_freq_direct(can_t *obj, const can_pinmap_t *pinmap, int hz
#ifdef DEVICE_CAN_FD
                          , int data_hz
#endif
                         )
{
    MBED_ASSERT(obj);
    MBED_ASSERT(pinmap);

    int per = pinmap->peripheral;

    pin_function(pinmap->rd_pin, pinmap->rd_function);
    pin_function(pinmap->td_pin, pinmap->td_function);

    const can_instance_t *inst = g_can_instances[per];

    obj->instance     = inst;
    obj->irq_context  = 0;
    obj->hz           = hz;
#ifdef DEVICE_CAN_FD
    obj->data_hz      = data_hz;
    canfd_default_accept_all((const canfd_extended_cfg_t*)inst->p_cfg->p_extend);
#endif

    const can_cfg_t *cfg_rom = (const can_cfg_t *) inst->p_cfg;
    obj->cfg_copy            = *cfg_rom;

    fsp_err_t err = obj->instance->p_api->open(obj->instance->p_ctrl, &obj->cfg_copy);
    MBED_ASSERT(FSP_SUCCESS == err);

    (void) can_frequency(obj, hz
#ifdef DEVICE_CAN_FD
                         , data_hz
#endif
                        );
}

void can_free(can_t *obj)
{
    if (!obj || !obj->instance) {
        return;
    }

    (void) obj->instance->p_api->close(obj->instance->p_ctrl);
    obj->instance    = NULL;
    obj->irq_context = 0;
}

static int calc_bit_timing(uint32_t canclk, uint32_t bitrate,
                           can_bit_timing_cfg_t *out,
                           float sample_point)
{
    /* sample_point = TSEG1 / (TSEG1 + TSEG2) */

    for (uint32_t prescaler = 1; prescaler <= 1024; prescaler++) {

        uint32_t tq = canclk / (prescaler * bitrate);
        if (tq < 8 || tq > 256) continue;

        uint32_t tseg1 = (uint32_t)(tq * sample_point);
        uint32_t tseg2 = tq - tseg1 - 1;

        if (tseg1 < 2 || tseg1 > 255) continue;
        if (tseg2 < 1 || tseg2 > 127) continue;

        out->baud_rate_prescaler = prescaler;
        out->time_segment_1 = tseg1;
        out->time_segment_2 = tseg2;
        out->synchronization_jump_width = (tseg2 > 4 ? 4 : tseg2);

        return 1;
    }

    return 0;
}

int can_frequency(can_t *obj, int hz
#ifdef DEVICE_CAN_FD
                  , int data_hz
#endif
                 )
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->instance);

    obj->instance->p_api->close(obj->instance->p_ctrl);

#ifdef DEVICE_CAN_FD
    // Use PLL/PLL1Q for CAN FD clock source
    uint32_t canclk = R_BSP_SourceClockHzGet(BSP_CFG_CANFDCLK_SOURCE) / canfd_get_clock_div(BSP_CFG_CANFDCLK_DIV) ;
#else
    // Use PCLKB for CAN clock source
    uint32_t canclk = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);
#endif

    if (!calc_bit_timing(canclk, hz, obj->cfg_copy.p_bit_timing, 0.75f)) {
        return 0;
    }

    obj->hz = hz;
#ifdef DEVICE_CAN_FD
    canfd_extended_cfg_t *ext_rom = (canfd_extended_cfg_t *) obj->cfg_copy.p_extend;

    obj->data_hz = data_hz;
    if (data_hz > 0) {
        if (!calc_bit_timing(canclk, data_hz, ext_rom->p_data_timing, 0.80f)) {
            return 0;
        }
    }
#endif

    fsp_err_t err = obj->instance->p_api->open(obj->instance->p_ctrl, &obj->cfg_copy);
    return (err == FSP_SUCCESS);
}

void can_irq_init(can_t *obj, can_irq_handler handler, uintptr_t context)
{
    MBED_ASSERT(obj);
    irq_handler = handler;
    obj->irq_context = context;
}

void can_irq_free(can_t *obj)
{
    if (!obj) {
        return;
    }
    obj->irq_context = 0;
}

void can_irq_set(can_t *obj, CanIrqType irq, uint32_t enable)
{
    MBED_ASSERT(obj);
    FSP_PARAMETER_NOT_USED(irq);
    FSP_PARAMETER_NOT_USED(enable);
}

static void mbed_to_can_frame(const CAN_Message *msg, can_frame_t *frame)
{
    frame->id = msg->id;
    frame->id_mode = (msg->format == CANExtended) ? CAN_ID_MODE_EXTENDED : CAN_ID_MODE_STANDARD;
    frame->type    = (msg->type == CANRemote) ? CAN_FRAME_TYPE_REMOTE : CAN_FRAME_TYPE_DATA;
    frame->data_length_code = msg->len;
    frame->options = 0;
    for (int i = 0; i < msg->len && i < 8; i++) {
        frame->data[i] = msg->data[i];
    }
}

static void can_frame_to_mbed(const can_frame_t *frame, CAN_Message *msg)
{
    msg->id     = frame->id;
    msg->format = (frame->id_mode == CAN_ID_MODE_EXTENDED) ? CANExtended : CANStandard;
    msg->type   = (frame->type == CAN_FRAME_TYPE_REMOTE) ? CANRemote : CANData;
    msg->len    = frame->data_length_code;
    for (int i = 0; i < msg->len && i < 8; i++) {
        msg->data[i] = frame->data[i];
    }
}

int can_write(can_t *obj, CAN_Message msg)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->instance);

    can_frame_t frame;
    mbed_to_can_frame(&msg, &frame);

    fsp_err_t err = obj->instance->p_api->write(obj->instance->p_ctrl, MBED_CAN_MAILBOX_TX, &frame);
    return (err == FSP_SUCCESS) ? 1 : 0;
}

int can_read(can_t *obj, CAN_Message *msg, int handle)
{
    FSP_PARAMETER_NOT_USED(handle);

    if (obj->rx_head == obj->rx_tail) {
        return 0;   // no data
    }

    can_frame_t *f = &obj->rx_queue[obj->rx_tail];
    obj->rx_tail = (obj->rx_tail + 1) % MBED_CAN_RX_QUEUE_SIZE;

    can_frame_to_mbed(f, msg);
    return 1;
}


int can_mode(can_t *obj, CanMode mode)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->instance);

    can_operation_mode_t op_mode = CAN_OPERATION_MODE_NORMAL;
    can_test_mode_t      test_mode = CAN_TEST_MODE_DISABLED;

    switch (mode) {

        case MODE_RESET:
            op_mode = CAN_OPERATION_MODE_RESET;
            test_mode = CAN_TEST_MODE_DISABLED;
            break;

        case MODE_NORMAL:
            op_mode = CAN_OPERATION_MODE_NORMAL;
            test_mode = CAN_TEST_MODE_DISABLED;
            break;

        case MODE_SILENT:
            op_mode = CAN_OPERATION_MODE_NORMAL;
            test_mode = CAN_TEST_MODE_LISTEN;
            break;

        case MODE_TEST_LOCAL:
            op_mode = CAN_OPERATION_MODE_NORMAL;
            test_mode = CAN_TEST_MODE_LOOPBACK_INTERNAL;
            break;

        case MODE_TEST_GLOBAL:
            op_mode = CAN_OPERATION_MODE_NORMAL;
            test_mode = CAN_TEST_MODE_LOOPBACK_EXTERNAL;
            break;

        case MODE_TEST_SILENT:
            op_mode = CAN_OPERATION_MODE_NORMAL;
            test_mode = CAN_TEST_MODE_LISTEN;
            break;

        default:
            return 0;
    }

    fsp_err_t err = obj->instance->p_api->modeTransition(
        obj->instance->p_ctrl,
        op_mode,
        test_mode
    );

    return (err == FSP_SUCCESS) ? 1 : 0;
}

#ifdef DEVICE_CAN_FD

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
    MBED_ASSERT(obj);

    obj->instance->p_api->close(obj->instance->p_ctrl);

    const can_instance_t *inst = obj->instance;
    const can_cfg_t *cfg = inst->p_cfg;
    canfd_extended_cfg_t *ext = (canfd_extended_cfg_t *) cfg->p_extend;

    canfd_afl_entry_t *afl = (canfd_afl_entry_t *) ext->p_afl;
    int32_t rule_num = CANFD_CFG_AFL_CH0_RULE_NUM;
    int32_t idx = (handle >= 0 && handle < rule_num) ? handle : 0;

    afl[idx].id.id = id;
    afl[idx].id.frame_type = CAN_FRAME_TYPE_DATA;
    afl[idx].id.id_mode = (format == CANStandard ? CAN_ID_MODE_STANDARD: CAN_ID_MODE_EXTENDED);
    afl[idx].mask.mask_id = mask;
    afl[idx].mask.mask_id_mode = (format == CANExtended) ? CAN_ID_MODE_EXTENDED : CAN_ID_MODE_STANDARD;
    afl[idx].mask.mask_frame_type = CAN_FRAME_TYPE_DATA;
    afl[idx].destination.minimum_dlc = 0;
    afl[idx].destination.fifo_select_flags = CANFD_RX_FIFO_0;

    fsp_err_t err = obj->instance->p_api->open(obj->instance->p_ctrl, &obj->cfg_copy);
    return (err == FSP_SUCCESS) ? idx : 0;
}

#else

int classic_can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
    MBED_ASSERT(obj);

    const can_instance_t *inst = obj->instance;
    const can_cfg_t *cfg = inst->p_cfg;
    can_extended_cfg_t *ext = (can_extended_cfg_t *) cfg->p_extend;

    /* 1. close CAN */
    inst->p_api->close(inst->p_ctrl);

    /* 2. mailbox index（mailbox 0 is usually used by TX，so starting from 1） */
    uint32_t mb_count = ext->mailbox_count;
    uint32_t mb = (handle >= 1 && handle < mb_count) ? handle : 1;

    /* 3. configure mailbox */
    can_mailbox_t *mbox = &ext->p_mailbox[mb];

    mbox->mailbox_id = id;
    mbox->id_mode = (format == CANStandard) ? CAN_ID_MODE_STANDARD : CAN_ID_MODE_EXTENDED;
    mbox->frame_type = CAN_FRAME_TYPE_DATA;
    mbox->mailbox_type = CAN_MAILBOX_RECEIVE;

    /* 4. set mask（each mask shared by 4 mailboxes） */
    uint32_t group = mb / 4;
    ext->p_mailbox_mask[group] = mask;

    /* 5. reopen CAN */
    fsp_err_t err = inst->p_api->open(inst->p_ctrl, &obj->cfg_copy);
    return (err == FSP_SUCCESS) ? mb : 0;
}

#endif

void can_reset(can_t *obj)
{
    (void) can_mode(obj, MODE_RESET);
    (void) can_mode(obj, MODE_NORMAL);
}

unsigned char can_rderror(can_t *obj)
{
    MBED_ASSERT(obj);
    can_info_t info;
    if (obj->instance->p_api->infoGet(obj->instance->p_ctrl, &info) != FSP_SUCCESS) {
        return 0;
    }
    return (unsigned char) info.error_count_receive;
}

unsigned char can_tderror(can_t *obj)
{
    MBED_ASSERT(obj);
    can_info_t info;
    if (obj->instance->p_api->infoGet(obj->instance->p_ctrl, &info) != FSP_SUCCESS) {
        return 0;
    }
    return (unsigned char) info.error_count_transmit;
}

void can_monitor(can_t *obj, int silent)
{
    if (silent) {
        (void) can_mode(obj, MODE_SILENT);
    } else {
        (void) can_mode(obj, MODE_NORMAL);
    }
}

#ifdef DEVICE_CAN_FD

static void mbed_to_canfd_frame(const CANFD_Message *msg, can_frame_t *frame, bool need_brs)
{
    frame->id = msg->id;
    frame->id_mode = (msg->format == CANExtended) ? CAN_ID_MODE_EXTENDED : CAN_ID_MODE_STANDARD;
    frame->type    = (msg->type == CANRemote) ? CAN_FRAME_TYPE_REMOTE : CAN_FRAME_TYPE_DATA;
    frame->data_length_code = msg->len;
    frame->options = CANFD_FRAME_OPTION_FD | (need_brs ? CANFD_FRAME_OPTION_BRS: 0);
    for (int i = 0; i < msg->len && i < 64; i++) {
        frame->data[i] = msg->data[i];
    }
}

static void canfd_frame_to_mbed(const can_frame_t *frame, CANFD_Message *msg)
{
    msg->id     = frame->id;
    msg->format = (frame->id_mode == CAN_ID_MODE_EXTENDED) ? CANExtended : CANStandard;
    msg->type   = (frame->type == CAN_FRAME_TYPE_REMOTE) ? CANRemote : CANData;
    msg->len    = frame->data_length_code;
    for (int i = 0; i < msg->len && i < 64; i++) {
        msg->data[i] = frame->data[i];
    }
}

int canfd_write(can_t *obj, CANFD_Message msg)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->instance);

    can_frame_t frame;
    mbed_to_canfd_frame(&msg, &frame, obj->hz != obj->data_hz);

    const can_api_t *fd_api = (const can_api_t *) obj->instance->p_api;
    fsp_err_t err = fd_api->write(obj->instance->p_ctrl, MBED_CAN_MAILBOX_TX, &frame);
    return (err == FSP_SUCCESS) ? 1 : 0;
}

int canfd_read(can_t *obj, CANFD_Message *msg, int handle)
{
    FSP_PARAMETER_NOT_USED(handle);

    if (obj->rx_head == obj->rx_tail) {
        return 0;
    }

    can_frame_t *f = &obj->rx_queue[obj->rx_tail];
    obj->rx_tail = (obj->rx_tail + 1) % MBED_CAN_RX_QUEUE_SIZE;

    canfd_frame_to_mbed(f, msg);
    return 1;
}

#endif /* DEVICE_CAN_FD */

void can_callback(can_callback_args_t *p_args)
{
    can_t *obj = (can_t *) p_args->p_context;

    if (!obj) return;

    switch (p_args->event) {

        case CAN_EVENT_RX_COMPLETE:
        {
            uint8_t next = (obj->rx_head + 1) % MBED_CAN_RX_QUEUE_SIZE;

            if (next == obj->rx_tail) {
                obj->rx_tail = (obj->rx_tail + 1) % MBED_CAN_RX_QUEUE_SIZE;
            }

            obj->rx_queue[obj->rx_head] = p_args->frame;
            obj->rx_head = next;

            if (irq_handler) {
                irq_handler(obj->irq_context, IRQ_RX);
            }
            break;
        }

        case CAN_EVENT_TX_COMPLETE:
            if (irq_handler) {
                irq_handler(obj->irq_context, IRQ_TX);
            }
            break;

        case CAN_EVENT_ERR_CHANNEL:
            /* Reset channel */
            obj->instance->p_api->modeTransition(obj->instance->p_ctrl,
                CAN_OPERATION_MODE_GLOBAL_RESET,
                CAN_TEST_MODE_DISABLED);
            /* Back to operation */
            obj->instance->p_api->modeTransition(obj->instance->p_ctrl,
                CAN_OPERATION_MODE_GLOBAL_OPERATION,
                CAN_TEST_MODE_DISABLED);
            if (irq_handler) {
                irq_handler(obj->irq_context, IRQ_ERROR);
            }
            break;
        case CAN_EVENT_BUS_RECOVERY:
        case CAN_EVENT_ERR_PASSIVE:
        case CAN_EVENT_ERR_WARNING:
        case CAN_EVENT_ERR_BUS_OFF:
            if (irq_handler) {
                irq_handler(obj->irq_context, IRQ_ERROR);
            }
            break;

        default:
            break;
    }
}

#endif

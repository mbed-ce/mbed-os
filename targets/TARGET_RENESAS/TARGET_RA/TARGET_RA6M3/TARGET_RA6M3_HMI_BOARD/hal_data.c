/* generated HAL source file - do not edit */
#include "hal_data.h"

crc_instance_ctrl_t g_crc0_ctrl;
const crc_cfg_t g_crc0_cfg =
{
    .polynomial      = CRC_POLYNOMIAL_CRC_32,
    .bit_order       = CRC_BIT_ORDER_LMS_LSB,
    .snoop_address   = 0x003,
    .p_extend        = NULL,
};

/* Instance structure to use this module. */
const crc_instance_t g_crc0 =
{
    .p_ctrl        = &g_crc0_ctrl,
    .p_cfg         = &g_crc0_cfg,
    .p_api         = &g_crc_on_crc
};
flash_hp_instance_ctrl_t g_flash0_ctrl;
const flash_cfg_t g_flash0_cfg =
{
    .data_flash_bgo      = false,
    .p_callback          = flash_callback,
    .p_context           = NULL,
#if defined(VECTOR_NUMBER_FCU_FRDYI)
    .irq                 = VECTOR_NUMBER_FCU_FRDYI,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_FCU_FIFERR)
    .err_irq             = VECTOR_NUMBER_FCU_FIFERR,
#else
    .err_irq             = FSP_INVALID_VECTOR,
#endif
    .err_ipl             = (7),
    .ipl                 = (7),
};
/* Instance structure to use this module. */
const flash_instance_t g_flash0 =
{
    .p_ctrl        = &g_flash0_ctrl,
    .p_cfg         = &g_flash0_cfg,
    .p_api         = &g_flash_on_flash_hp
};
#ifndef CAN1_BAUD_SETTINGS_OVERRIDE
#define CAN1_BAUD_SETTINGS_OVERRIDE  (0)
#endif
#if CAN1_BAUD_SETTINGS_OVERRIDE
can_bit_timing_cfg_t g_can1_bit_timing_cfg =
{
    .baud_rate_prescaler = 1,
    .time_segment_1 = 4,
    .time_segment_2 = 2,
    .synchronization_jump_width = 1
};
#else
can_bit_timing_cfg_t g_can1_bit_timing_cfg =
{
    /* Actual bitrate: 500000 Hz. Actual Bit Time Ratio: 75 %. */  .baud_rate_prescaler = 1 +2 /* Division value of baud rate prescaler */, .time_segment_1 = 11, .time_segment_2 = 4, .synchronization_jump_width = 4,
};
#endif

uint32_t g_can1_mailbox_mask[CAN_NO_OF_MAILBOXES_g_can1/4] =
{
0x00000000,
#if CAN_NO_OF_MAILBOXES_g_can1 > 4
0x00000000,
#endif
#if CAN_NO_OF_MAILBOXES_g_can1 > 8
0x00000000,
0x00000000,
#endif
#if CAN_NO_OF_MAILBOXES_g_can1 > 16
0x00000000,
0x00000000,
#endif
#if CAN_NO_OF_MAILBOXES_g_can1 > 24
0x00000000,
0x00000000,
#endif
};

can_mailbox_t g_can1_mailbox[CAN_NO_OF_MAILBOXES_g_can1] =
{
    {
        .mailbox_id              =  0,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_REMOTE
},
    {
        .mailbox_id              =  1,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  2,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA,
    },
    {
        .mailbox_id              =  3,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
#if CAN_NO_OF_MAILBOXES_g_can1 > 4
    {
        .mailbox_id              =  4,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  5,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  6,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  7,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
#endif
#if CAN_NO_OF_MAILBOXES_g_can1 > 8
    {
        .mailbox_id              =  8,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  9,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  10,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  11,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  12,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA,
    },
    {
        .mailbox_id              =  13,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  14,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  15,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
#endif
#if CAN_NO_OF_MAILBOXES_g_can1 > 16
    {
        .mailbox_id              =  16,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },

    {
        .mailbox_id              =  17,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  18,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  19,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  20,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  21,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  22,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA,
    },
    {
        .mailbox_id              =  23,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
#endif
#if CAN_NO_OF_MAILBOXES_g_can1 > 24
    {
        .mailbox_id              =  24,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  25,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  26,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  27,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  28,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  29,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  30,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  31,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    }
#endif
};

#if CAN_CFG_FIFO_SUPPORT
const can_fifo_interrupt_cfg_t g_can1_fifo_int_cfg =
{
    .fifo_int_mode           = (can_fifo_interrupt_mode_t) (CAN_FIFO_INTERRUPT_MODE_RX_EVERY_FRAME | CAN_FIFO_INTERRUPT_MODE_TX_EVERY_FRAME),
    .rx_fifo_irq             = VECTOR_NUMBER_CAN1_FIFO_RX,
    .tx_fifo_irq             = VECTOR_NUMBER_CAN1_FIFO_TX,
};

can_rx_fifo_cfg_t g_can1_rx_fifo_cfg =
{
    .rx_fifo_mask1 = 0x1FFFFFFF,
    .rx_fifo_mask2 = 0x1FFFFFFF,

    .rx_fifo_id1 =
    {
        .mailbox_id              =  0,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_REMOTE
    },

    .rx_fifo_id2 =
    {
        .mailbox_id              =  0,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_REMOTE
    },
};
#endif

const can_extended_cfg_t g_can1_extended_cfg =
{
    .clock_source           = CAN_CLOCK_SOURCE_CANMCLK,
    .p_mailbox_mask         = g_can1_mailbox_mask,
    .p_mailbox              = g_can1_mailbox,
    .global_id_mode         = CAN_GLOBAL_ID_MODE_STANDARD,
    .mailbox_count          = CAN_NO_OF_MAILBOXES_g_can1,
    .message_mode           = CAN_MESSAGE_MODE_OVERWRITE,
#if CAN_CFG_FIFO_SUPPORT
    .p_fifo_int_cfg         = &g_can1_fifo_int_cfg,
    .p_rx_fifo_cfg          = &g_can1_rx_fifo_cfg,
#else
    .p_fifo_int_cfg         = NULL,
    .p_rx_fifo_cfg          = NULL,
#endif
};

can_instance_ctrl_t g_can1_ctrl;
const can_cfg_t g_can1_cfg =
{
    .channel            = 1,
    .p_bit_timing       = &g_can1_bit_timing_cfg,
    .p_callback         = can_callback,
    .p_extend           = &g_can1_extended_cfg,
    .p_context          = NULL,
    .ipl                = (12),
#if defined(VECTOR_NUMBER_CAN1_MAILBOX_TX)
    .tx_irq             = VECTOR_NUMBER_CAN1_MAILBOX_TX,
#else
    .tx_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN1_MAILBOX_RX)
    .rx_irq             = VECTOR_NUMBER_CAN1_MAILBOX_RX,
#else
    .rx_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN1_ERROR)
    .error_irq             = VECTOR_NUMBER_CAN1_ERROR,
#else
    .error_irq             = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const can_instance_t g_can1 =
{
    .p_ctrl        = &g_can1_ctrl,
    .p_cfg         = &g_can1_cfg,
    .p_api         = &g_can_on_can
};
#ifndef CAN0_BAUD_SETTINGS_OVERRIDE
#define CAN0_BAUD_SETTINGS_OVERRIDE  (0)
#endif
#if CAN0_BAUD_SETTINGS_OVERRIDE
can_bit_timing_cfg_t g_can0_bit_timing_cfg =
{
    .baud_rate_prescaler = 1,
    .time_segment_1 = 4,
    .time_segment_2 = 2,
    .synchronization_jump_width = 1
};
#else
can_bit_timing_cfg_t g_can0_bit_timing_cfg =
{
    /* Actual bitrate: 500000 Hz. Actual Bit Time Ratio: 75 %. */  .baud_rate_prescaler = 1 +2 /* Division value of baud rate prescaler */, .time_segment_1 = 11, .time_segment_2 = 4, .synchronization_jump_width = 4,
};
#endif

uint32_t g_can0_mailbox_mask[CAN_NO_OF_MAILBOXES_g_can0/4] =
{
0x1FFFFFFF,
#if CAN_NO_OF_MAILBOXES_g_can0 > 4
0x00000000,
#endif
#if CAN_NO_OF_MAILBOXES_g_can0 > 8
0x00000000,
0x00000000,
#endif
#if CAN_NO_OF_MAILBOXES_g_can0 > 16
0x00000000,
0x00000000,
#endif
#if CAN_NO_OF_MAILBOXES_g_can0 > 24
0x00000000,
0x00000000,
#endif
};

can_mailbox_t g_can0_mailbox[CAN_NO_OF_MAILBOXES_g_can0] =
{
    {
        .mailbox_id              =  0,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_TRANSMIT,
        .frame_type              =  CAN_FRAME_TYPE_REMOTE
},
    {
        .mailbox_id              =  1,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  2,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA,
    },
    {
        .mailbox_id              =  3,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
#if CAN_NO_OF_MAILBOXES_g_can0 > 4
    {
        .mailbox_id              =  4,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  5,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  6,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  7,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
#endif
#if CAN_NO_OF_MAILBOXES_g_can0 > 8
    {
        .mailbox_id              =  8,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  9,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  10,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  11,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  12,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA,
    },
    {
        .mailbox_id              =  13,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  14,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  15,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
#endif
#if CAN_NO_OF_MAILBOXES_g_can0 > 16
    {
        .mailbox_id              =  16,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },

    {
        .mailbox_id              =  17,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  18,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  19,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  20,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  21,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  22,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA,
    },
    {
        .mailbox_id              =  23,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
#endif
#if CAN_NO_OF_MAILBOXES_g_can0 > 24
    {
        .mailbox_id              =  24,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  25,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  26,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  27,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  28,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  29,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  30,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    },
    {
        .mailbox_id              =  31,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_DATA
    }
#endif
};

#if CAN_CFG_FIFO_SUPPORT
const can_fifo_interrupt_cfg_t g_can0_fifo_int_cfg =
{
    .fifo_int_mode           = (can_fifo_interrupt_mode_t) (CAN_FIFO_INTERRUPT_MODE_RX_EVERY_FRAME | CAN_FIFO_INTERRUPT_MODE_TX_EVERY_FRAME),
    .rx_fifo_irq             = VECTOR_NUMBER_CAN0_FIFO_RX,
    .tx_fifo_irq             = VECTOR_NUMBER_CAN0_FIFO_TX,
};

can_rx_fifo_cfg_t g_can0_rx_fifo_cfg =
{
    .rx_fifo_mask1 = 0x1FFFFFFF,
    .rx_fifo_mask2 = 0x1FFFFFFF,

    .rx_fifo_id1 =
    {
        .mailbox_id              =  0,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_REMOTE
    },

    .rx_fifo_id2 =
    {
        .mailbox_id              =  0,
        .id_mode                 =  CAN_ID_MODE_STANDARD,
        .mailbox_type            =  CAN_MAILBOX_RECEIVE,
        .frame_type              =  CAN_FRAME_TYPE_REMOTE
    },
};
#endif

const can_extended_cfg_t g_can0_extended_cfg =
{
    .clock_source           = CAN_CLOCK_SOURCE_CANMCLK,
    .p_mailbox_mask         = g_can0_mailbox_mask,
    .p_mailbox              = g_can0_mailbox,
    .global_id_mode         = CAN_GLOBAL_ID_MODE_STANDARD,
    .mailbox_count          = CAN_NO_OF_MAILBOXES_g_can0,
    .message_mode           = CAN_MESSAGE_MODE_OVERWRITE,
#if CAN_CFG_FIFO_SUPPORT
    .p_fifo_int_cfg         = &g_can0_fifo_int_cfg,
    .p_rx_fifo_cfg          = &g_can0_rx_fifo_cfg,
#else
    .p_fifo_int_cfg         = NULL,
    .p_rx_fifo_cfg          = NULL,
#endif
};

can_instance_ctrl_t g_can0_ctrl;
const can_cfg_t g_can0_cfg =
{
    .channel            = 0,
    .p_bit_timing       = &g_can0_bit_timing_cfg,
    .p_callback         = can_callback,
    .p_extend           = &g_can0_extended_cfg,
    .p_context          = NULL,
    .ipl                = (12),
#if defined(VECTOR_NUMBER_CAN0_MAILBOX_TX)
    .tx_irq             = VECTOR_NUMBER_CAN0_MAILBOX_TX,
#else
    .tx_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN0_MAILBOX_RX)
    .rx_irq             = VECTOR_NUMBER_CAN0_MAILBOX_RX,
#else
    .rx_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN0_ERROR)
    .error_irq             = VECTOR_NUMBER_CAN0_ERROR,
#else
    .error_irq             = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const can_instance_t g_can0 =
{
    .p_ctrl        = &g_can0_ctrl,
    .p_cfg         = &g_can0_cfg,
    .p_api         = &g_can_on_can
};
sci_uart_instance_ctrl_t     g_uart9_ctrl;

            baud_setting_t               g_uart9_baud_setting =
            {
                /* Baud rate calculated with 0.009% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 43, .mddr = (uint8_t) 173, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart9_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart9_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
                .irda_setting = {
                    .ircr_bits_b.ire = 0,
                    .ircr_bits_b.irrxinv = 0,
                    .ircr_bits_b.irtxinv = 0,
                },
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart9_cfg =
            {
                .channel             = 9,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = uart_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart9_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI9_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI9_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI9_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI9_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI9_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart9 =
{
    .p_ctrl        = &g_uart9_ctrl,
    .p_cfg         = &g_uart9_cfg,
    .p_api         = &g_uart_on_sci
};
sci_uart_instance_ctrl_t     g_uart8_ctrl;

            baud_setting_t               g_uart8_baud_setting =
            {
                /* Baud rate calculated with 0.009% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 43, .mddr = (uint8_t) 173, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart8_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart8_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
                .irda_setting = {
                    .ircr_bits_b.ire = 0,
                    .ircr_bits_b.irrxinv = 0,
                    .ircr_bits_b.irtxinv = 0,
                },
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart8_cfg =
            {
                .channel             = 8,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = uart_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart8_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI8_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI8_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI8_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI8_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI8_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI8_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI8_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI8_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart8 =
{
    .p_ctrl        = &g_uart8_ctrl,
    .p_cfg         = &g_uart8_cfg,
    .p_api         = &g_uart_on_sci
};
sci_uart_instance_ctrl_t     g_uart7_ctrl;

            baud_setting_t               g_uart7_baud_setting =
            {
                /* Baud rate calculated with 0.009% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 43, .mddr = (uint8_t) 173, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart7_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart7_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
                .irda_setting = {
                    .ircr_bits_b.ire = 0,
                    .ircr_bits_b.irrxinv = 0,
                    .ircr_bits_b.irtxinv = 0,
                },
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart7_cfg =
            {
                .channel             = 7,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = uart_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart7_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI7_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI7_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI7_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI7_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI7_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart7 =
{
    .p_ctrl        = &g_uart7_ctrl,
    .p_cfg         = &g_uart7_cfg,
    .p_api         = &g_uart_on_sci
};
sci_uart_instance_ctrl_t     g_uart6_ctrl;

            baud_setting_t               g_uart6_baud_setting =
            {
                /* Baud rate calculated with 0.009% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 43, .mddr = (uint8_t) 173, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart6_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart6_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
                .irda_setting = {
                    .ircr_bits_b.ire = 0,
                    .ircr_bits_b.irrxinv = 0,
                    .ircr_bits_b.irtxinv = 0,
                },
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart6_cfg =
            {
                .channel             = 6,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = NULL,
                .p_context           = NULL,
                .p_extend            = &g_uart6_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI6_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI6_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI6_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI6_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI6_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart6 =
{
    .p_ctrl        = &g_uart6_ctrl,
    .p_cfg         = &g_uart6_cfg,
    .p_api         = &g_uart_on_sci
};
sci_uart_instance_ctrl_t     g_uart5_ctrl;

            baud_setting_t               g_uart5_baud_setting =
            {
                /* Baud rate calculated with 0.009% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 43, .mddr = (uint8_t) 173, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart5_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart5_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
                .irda_setting = {
                    .ircr_bits_b.ire = 0,
                    .ircr_bits_b.irrxinv = 0,
                    .ircr_bits_b.irtxinv = 0,
                },
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart5_cfg =
            {
                .channel             = 5,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = NULL,
                .p_context           = NULL,
                .p_extend            = &g_uart5_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI5_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI5_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI5_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI5_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI5_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart5 =
{
    .p_ctrl        = &g_uart5_ctrl,
    .p_cfg         = &g_uart5_cfg,
    .p_api         = &g_uart_on_sci
};
sci_uart_instance_ctrl_t     g_uart4_ctrl;

            baud_setting_t               g_uart4_baud_setting =
            {
                /* Baud rate calculated with 0.009% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 43, .mddr = (uint8_t) 173, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart4_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart4_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
                .irda_setting = {
                    .ircr_bits_b.ire = 0,
                    .ircr_bits_b.irrxinv = 0,
                    .ircr_bits_b.irtxinv = 0,
                },
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart4_cfg =
            {
                .channel             = 4,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = uart_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart4_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI4_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI4_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI4_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI4_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI4_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart4 =
{
    .p_ctrl        = &g_uart4_ctrl,
    .p_cfg         = &g_uart4_cfg,
    .p_api         = &g_uart_on_sci
};
/* SCI3 is used in simple SPI mode for the RW007 WiFi module, not as a UART. */
sci_spi_instance_ctrl_t g_spi_sci3_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi_sci3_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 7500000. */.cks = 0,
  .brr = 3, .mddr = 0, } };

/** SPI interface configuration */
const spi_cfg_t g_spi_sci3_cfg =
{ .channel = 3, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_EVEN, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
  .p_transfer_tx   = NULL,
  .p_transfer_rx   = NULL,
  .p_callback = spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI3_RXI, .txi_irq = VECTOR_NUMBER_SCI3_TXI, .tei_irq =
          VECTOR_NUMBER_SCI3_TEI,
  .eri_irq = VECTOR_NUMBER_SCI3_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi_sci3_cfg_extend, };

/* Instance structure to use this module. */
const spi_instance_t g_spi_sci3 =
{ .p_ctrl = &g_spi_sci3_ctrl, .p_cfg = &g_spi_sci3_cfg, .p_api = &g_spi_on_sci };
sci_uart_instance_ctrl_t     g_uart2_ctrl;

            baud_setting_t               g_uart2_baud_setting =
            {
                /* Baud rate calculated with 0.009% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 43, .mddr = (uint8_t) 173, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart2_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart2_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
                .irda_setting = {
                    .ircr_bits_b.ire = 0,
                    .ircr_bits_b.irrxinv = 0,
                    .ircr_bits_b.irtxinv = 0,
                },
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart2_cfg =
            {
                .channel             = 2,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = uart_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart2_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI2_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI2_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI2_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI2_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI2_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart2 =
{
    .p_ctrl        = &g_uart2_ctrl,
    .p_cfg         = &g_uart2_cfg,
    .p_api         = &g_uart_on_sci
};
sci_uart_instance_ctrl_t     g_uart1_ctrl;

            baud_setting_t               g_uart1_baud_setting =
            {
                /* Baud rate calculated with 0.009% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 43, .mddr = (uint8_t) 173, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart1_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart1_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
                .irda_setting = {
                    .ircr_bits_b.ire = 0,
                    .ircr_bits_b.irrxinv = 0,
                    .ircr_bits_b.irtxinv = 0,
                },
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart1_cfg =
            {
                .channel             = 1,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = uart_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart1_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI1_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI1_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI1_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI1_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI1_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart1 =
{
    .p_ctrl        = &g_uart1_ctrl,
    .p_cfg         = &g_uart1_cfg,
    .p_api         = &g_uart_on_sci
};
sci_uart_instance_ctrl_t     g_uart0_ctrl;

            baud_setting_t               g_uart0_baud_setting =
            {
                /* Baud rate calculated with 0.009% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 43, .mddr = (uint8_t) 173, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart0_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart0_baud_setting,
                .flow_control           = SCI_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting = {
                    .enable = SCI_UART_RS485_DISABLE,
                    .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
                #if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                    .de_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                },
                .irda_setting = {
                    .ircr_bits_b.ire = 0,
                    .ircr_bits_b.irrxinv = 0,
                    .ircr_bits_b.irtxinv = 0,
                },
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart0_cfg =
            {
                .channel             = 0,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = uart_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart0_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
                .rxi_ipl             = (12),
                .txi_ipl             = (12),
                .tei_ipl             = (12),
                .eri_ipl             = (12),
#if defined(VECTOR_NUMBER_SCI0_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI0_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI0_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI0_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI0_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{
    .p_ctrl        = &g_uart0_ctrl,
    .p_cfg         = &g_uart0_cfg,
    .p_api         = &g_uart_on_sci
};

dmac_instance_ctrl_t g_transfer1_ctrl;
transfer_info_t g_transfer1_info =
{
    .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
    .transfer_settings_word_b.repeat_area    = TRANSFER_REPEAT_AREA_SOURCE,
    .transfer_settings_word_b.irq            = TRANSFER_IRQ_END,
    .transfer_settings_word_b.chain_mode     = TRANSFER_CHAIN_MODE_DISABLED,
    .transfer_settings_word_b.src_addr_mode  = TRANSFER_ADDR_MODE_INCREMENTED,
    .transfer_settings_word_b.size           = TRANSFER_SIZE_1_BYTE,
    .transfer_settings_word_b.mode           = TRANSFER_MODE_NORMAL,
    .p_dest                                  = (void *) NULL,
    .p_src                                   = (void const *) NULL,
    .num_blocks                              = 0,
    .length                                  = 0,
};
const dmac_extended_cfg_t g_transfer1_extend =
{
    .offset              = 1,
    .src_buffer_size     = 1,
#if defined(VECTOR_NUMBER_DMAC1_INT)
    .irq                 = VECTOR_NUMBER_DMAC1_INT,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
    .ipl                 = (12),
    .channel             = 1,
    .p_callback          = g_spi1_tx_transfer_callback,
    .p_context           = NULL,
    .activation_source   = ELC_EVENT_SPI1_TXI,
};
const transfer_cfg_t g_transfer1_cfg =
{
    .p_info              = &g_transfer1_info,
    .p_extend            = &g_transfer1_extend,
};
/* Instance structure to use this module. */
const transfer_instance_t g_transfer1 =
{
    .p_ctrl        = &g_transfer1_ctrl,
    .p_cfg         = &g_transfer1_cfg,
    .p_api         = &g_transfer_on_dmac
};
#define RA_NOT_DEFINED (UINT32_MAX)
#if (RA_NOT_DEFINED) != (1)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void spi_tx_dmac_callback(spi_instance_ctrl_t const * const p_ctrl);

void g_spi1_tx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_tx_dmac_callback(&g_spi1_ctrl);
}
#endif

#if (RA_NOT_DEFINED) != (RA_NOT_DEFINED)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void spi_rx_dmac_callback(spi_instance_ctrl_t const * const p_ctrl);

void g_spi1_rx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_rx_dmac_callback(&g_spi1_ctrl);
}
#endif
#undef RA_NOT_DEFINED

spi_instance_ctrl_t g_spi1_ctrl;

/** SPI extended configuration for SPI HAL driver */
const spi_extended_cfg_t g_spi1_ext_cfg =
{
    .spi_clksyn         = SPI_SSL_MODE_CLK_SYN,
    .spi_comm           = SPI_COMMUNICATION_FULL_DUPLEX,
    .ssl_polarity        = SPI_SSLP_LOW,
    .ssl_select          = SPI_SSL_SELECT_SSL0,
    .mosi_idle           = SPI_MOSI_IDLE_VALUE_FIXING_DISABLE,
    .parity              = SPI_PARITY_MODE_DISABLE,
    .byte_swap           = SPI_BYTE_SWAP_DISABLE,
    .spck_div            = {
        /* Actual calculated bitrate: 15000000. */ .spbr = 3, .brdv = 0
    },
    .spck_delay          = SPI_DELAY_COUNT_1,
    .ssl_negation_delay  = SPI_DELAY_COUNT_1,
    .next_access_delay   = SPI_DELAY_COUNT_1,
    .burst_interframe_delay = SPI_BURST_TRANSFER_WITH_DELAY
 };

/** SPI configuration for SPI HAL driver */
const spi_cfg_t g_spi1_cfg =
{
    .channel             = 1,

#if defined(VECTOR_NUMBER_SPI1_RXI)
    .rxi_irq             = VECTOR_NUMBER_SPI1_RXI,
#else
    .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TXI)
    .txi_irq             = VECTOR_NUMBER_SPI1_TXI,
#else
    .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TEI)
    .tei_irq             = VECTOR_NUMBER_SPI1_TEI,
#else
    .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_ERI)
    .eri_irq             = VECTOR_NUMBER_SPI1_ERI,
#else
    .eri_irq             = FSP_INVALID_VECTOR,
#endif

    .rxi_ipl             = (10),
    .txi_ipl             = (BSP_IRQ_DISABLED),
    .tei_ipl             = (12),
    .eri_ipl             = (12),

    .operating_mode      = SPI_MODE_MASTER,

    .clk_phase           = SPI_CLK_PHASE_EDGE_ODD,
    .clk_polarity        = SPI_CLK_POLARITY_LOW,

    .mode_fault          = SPI_MODE_FAULT_ERROR_DISABLE,
    .bit_order           = SPI_BIT_ORDER_MSB_FIRST,
    .p_transfer_tx       = g_spi1_P_TRANSFER_TX,
    .p_transfer_rx       = g_spi1_P_TRANSFER_RX,
    .p_callback          = spi_callback,

    .p_context           = NULL,
    .p_extend            = (void *)&g_spi1_ext_cfg,
};

/* Instance structure to use this module. */
const spi_instance_t g_spi1 =
{
    .p_ctrl        = &g_spi1_ctrl,
    .p_cfg         = &g_spi1_cfg,
    .p_api         = &g_spi_on_spi
};

dmac_instance_ctrl_t g_transfer0_ctrl;
transfer_info_t g_transfer0_info =
{
    .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
    .transfer_settings_word_b.repeat_area    = TRANSFER_REPEAT_AREA_SOURCE,
    .transfer_settings_word_b.irq            = TRANSFER_IRQ_END,
    .transfer_settings_word_b.chain_mode     = TRANSFER_CHAIN_MODE_DISABLED,
    .transfer_settings_word_b.src_addr_mode  = TRANSFER_ADDR_MODE_INCREMENTED,
    .transfer_settings_word_b.size           = TRANSFER_SIZE_1_BYTE,
    .transfer_settings_word_b.mode           = TRANSFER_MODE_NORMAL,
    .p_dest                                  = (void *) NULL,
    .p_src                                   = (void const *) NULL,
    .num_blocks                              = 0,
    .length                                  = 0,
};
const dmac_extended_cfg_t g_transfer0_extend =
{
    .offset              = 1,
    .src_buffer_size     = 1,
#if defined(VECTOR_NUMBER_DMAC0_INT)
    .irq                 = VECTOR_NUMBER_DMAC0_INT,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
    .ipl                 = (12),
    .channel             = 0,
    .p_callback          = g_spi0_tx_transfer_callback,
    .p_context           = NULL,
    .activation_source   = ELC_EVENT_SPI0_TXI,
};
const transfer_cfg_t g_transfer0_cfg =
{
    .p_info              = &g_transfer0_info,
    .p_extend            = &g_transfer0_extend,
};
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{
    .p_ctrl        = &g_transfer0_ctrl,
    .p_cfg         = &g_transfer0_cfg,
    .p_api         = &g_transfer_on_dmac
};
#define RA_NOT_DEFINED (UINT32_MAX)
#if (RA_NOT_DEFINED) != (1)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void spi_tx_dmac_callback(spi_instance_ctrl_t const * const p_ctrl);

void g_spi0_tx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_tx_dmac_callback(&g_spi0_ctrl);
}
#endif

#if (RA_NOT_DEFINED) != (RA_NOT_DEFINED)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void spi_rx_dmac_callback(spi_instance_ctrl_t const * const p_ctrl);

void g_spi0_rx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_rx_dmac_callback(&g_spi0_ctrl);
}
#endif
#undef RA_NOT_DEFINED

spi_instance_ctrl_t g_spi0_ctrl;

/** SPI extended configuration for SPI HAL driver */
const spi_extended_cfg_t g_spi0_ext_cfg =
{
    .spi_clksyn         = SPI_SSL_MODE_CLK_SYN,
    .spi_comm           = SPI_COMMUNICATION_FULL_DUPLEX,
    .ssl_polarity        = SPI_SSLP_LOW,
    .ssl_select          = SPI_SSL_SELECT_SSL0,
    .mosi_idle           = SPI_MOSI_IDLE_VALUE_FIXING_DISABLE,
    .parity              = SPI_PARITY_MODE_DISABLE,
    .byte_swap           = SPI_BYTE_SWAP_DISABLE,
    .spck_div            = {
        /* Actual calculated bitrate: 15000000. */ .spbr = 3, .brdv = 0
    },
    .spck_delay          = SPI_DELAY_COUNT_1,
    .ssl_negation_delay  = SPI_DELAY_COUNT_1,
    .next_access_delay   = SPI_DELAY_COUNT_1,
    .burst_interframe_delay = SPI_BURST_TRANSFER_WITH_DELAY
 };

/** SPI configuration for SPI HAL driver */
const spi_cfg_t g_spi0_cfg =
{
    .channel             = 0,

#if defined(VECTOR_NUMBER_SPI0_RXI)
    .rxi_irq             = VECTOR_NUMBER_SPI0_RXI,
#else
    .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI0_TXI)
    .txi_irq             = VECTOR_NUMBER_SPI0_TXI,
#else
    .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI0_TEI)
    .tei_irq             = VECTOR_NUMBER_SPI0_TEI,
#else
    .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI0_ERI)
    .eri_irq             = VECTOR_NUMBER_SPI0_ERI,
#else
    .eri_irq             = FSP_INVALID_VECTOR,
#endif

    .rxi_ipl             = (10),
    .txi_ipl             = (BSP_IRQ_DISABLED),
    .tei_ipl             = (12),
    .eri_ipl             = (12),

    .operating_mode      = SPI_MODE_MASTER,

    .clk_phase           = SPI_CLK_PHASE_EDGE_ODD,
    .clk_polarity        = SPI_CLK_POLARITY_LOW,

    .mode_fault          = SPI_MODE_FAULT_ERROR_DISABLE,
    .bit_order           = SPI_BIT_ORDER_MSB_FIRST,
    .p_transfer_tx       = g_spi0_P_TRANSFER_TX,
    .p_transfer_rx       = g_spi0_P_TRANSFER_RX,
    .p_callback          = spi_callback,

    .p_context           = NULL,
    .p_extend            = (void *)&g_spi0_ext_cfg,
};

/* Instance structure to use this module. */
const spi_instance_t g_spi0 =
{
    .p_ctrl        = &g_spi0_ctrl,
    .p_cfg         = &g_spi0_cfg,
    .p_api         = &g_spi_on_spi
};
agt_instance_ctrl_t g_timer_agt1_ctrl;
const agt_extended_cfg_t g_timer_agt1_extend =
{
    .count_source            = AGT_CLOCK_PCLKB,
    .agto                    = AGT_PIN_CFG_DISABLED,
    .agtoab_settings_b.agtoa = AGT_PIN_CFG_DISABLED,
    .agtoab_settings_b.agtob = AGT_PIN_CFG_DISABLED,
    .measurement_mode        = AGT_MEASURE_DISABLED,
    .agtio_filter            = AGT_AGTIO_FILTER_NONE,
    .enable_pin              = AGT_ENABLE_PIN_NOT_USED,
    .trigger_edge            = AGT_TRIGGER_EDGE_RISING,
    .counter_bit_width       = AGT_COUNTER_BIT_WIDTH_16,
};
const timer_cfg_t g_timer_agt1_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0010922666666666667 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 1,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_agt1_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_AGT1_INT)
    .cycle_end_irq       = VECTOR_NUMBER_AGT1_INT,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_agt1 =
{
    .p_ctrl        = &g_timer_agt1_ctrl,
    .p_cfg         = &g_timer_agt1_cfg,
    .p_api         = &g_timer_on_agt
};
agt_instance_ctrl_t g_timer_agt0_ctrl;
const agt_extended_cfg_t g_timer_agt0_extend =
{
    .count_source            = AGT_CLOCK_PCLKB,
    .agto                    = AGT_PIN_CFG_DISABLED,
    .agtoab_settings_b.agtoa = AGT_PIN_CFG_DISABLED,
    .agtoab_settings_b.agtob = AGT_PIN_CFG_DISABLED,
    .measurement_mode        = AGT_MEASURE_DISABLED,
    .agtio_filter            = AGT_AGTIO_FILTER_NONE,
    .enable_pin              = AGT_ENABLE_PIN_NOT_USED,
    .trigger_edge            = AGT_TRIGGER_EDGE_RISING,
    .counter_bit_width       = AGT_COUNTER_BIT_WIDTH_16,
};
const timer_cfg_t g_timer_agt0_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0010922666666666667 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 0,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_agt0_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_AGT0_INT)
    .cycle_end_irq       = VECTOR_NUMBER_AGT0_INT,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_agt0 =
{
    .p_ctrl        = &g_timer_agt0_ctrl,
    .p_cfg         = &g_timer_agt0_cfg,
    .p_api         = &g_timer_on_agt
};
gpt_instance_ctrl_t g_timer_gpt13_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt13_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT13_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT13_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt13_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT13_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT13_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT13_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT13_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT13_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT13_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT13_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT13_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT13_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT13_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT13_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT13_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt13_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt13_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 13,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt13_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT13_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT13_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt13 =
{
    .p_ctrl        = &g_timer_gpt13_ctrl,
    .p_cfg         = &g_timer_gpt13_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt12_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt12_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT12_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT12_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt12_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT12_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT12_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT12_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT12_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT12_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT12_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT12_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT12_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT12_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT12_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT12_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT12_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt12_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt12_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 12,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt12_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT12_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT12_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt12 =
{
    .p_ctrl        = &g_timer_gpt12_ctrl,
    .p_cfg         = &g_timer_gpt12_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt11_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt11_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT11_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT11_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt11_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT11_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT11_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT11_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT11_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT11_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT11_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT11_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt11_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt11_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 11,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt11_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT11_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT11_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt11 =
{
    .p_ctrl        = &g_timer_gpt11_ctrl,
    .p_cfg         = &g_timer_gpt11_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt10_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt10_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT10_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT10_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt10_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT10_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT10_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT10_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT10_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT10_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT10_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT10_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT10_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT10_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT10_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT10_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT10_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt10_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt10_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 10,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt10_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT10_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT10_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt10 =
{
    .p_ctrl        = &g_timer_gpt10_ctrl,
    .p_cfg         = &g_timer_gpt10_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt9_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt9_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT9_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT9_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt9_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT9_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT9_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT9_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT9_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT9_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT9_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT9_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT9_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT9_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT9_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT9_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT9_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt9_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt9_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 9,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt9_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT9_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT9_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt9 =
{
    .p_ctrl        = &g_timer_gpt9_ctrl,
    .p_cfg         = &g_timer_gpt9_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt8_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt8_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT8_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT8_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt8_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT8_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT8_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT8_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT8_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT8_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT8_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT8_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT8_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT8_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT8_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT8_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT8_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt8_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt8_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 8,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt8_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT8_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT8_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt8 =
{
    .p_ctrl        = &g_timer_gpt8_ctrl,
    .p_cfg         = &g_timer_gpt8_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt7_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt7_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT7_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT7_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt7_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT7_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT7_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT7_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT7_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT7_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT7_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT7_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT7_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt7_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt7_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 7,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt7_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT7_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT7_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt7 =
{
    .p_ctrl        = &g_timer_gpt7_ctrl,
    .p_cfg         = &g_timer_gpt7_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt6_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt6_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT6_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT6_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt6_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT6_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT6_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT6_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT6_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT6_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT6_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT6_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT6_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt6_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt6_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 6,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt6_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT6_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT6_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt6 =
{
    .p_ctrl        = &g_timer_gpt6_ctrl,
    .p_cfg         = &g_timer_gpt6_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt5_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt5_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT5_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT5_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt5_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT5_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT5_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT5_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT5_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT5_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT5_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT5_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT5_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT5_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT5_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT5_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT5_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt5_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt5_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 5,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt5_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT5_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT5_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt5 =
{
    .p_ctrl        = &g_timer_gpt5_ctrl,
    .p_cfg         = &g_timer_gpt5_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt4_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt4_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT4_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT4_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt4_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT4_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT4_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT4_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT4_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT4_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT4_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT4_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT4_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT4_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT4_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT4_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT4_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt4_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt4_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 4,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt4_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT4_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT4_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt4 =
{
    .p_ctrl        = &g_timer_gpt4_ctrl,
    .p_cfg         = &g_timer_gpt4_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt3_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt3_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT3_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT3_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt3_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT3_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT3_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT3_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT3_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT3_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT3_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT3_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT3_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT3_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT3_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT3_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT3_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt3_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt3_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 3,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt3_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT3_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT3_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt3 =
{
    .p_ctrl        = &g_timer_gpt3_ctrl,
    .p_cfg         = &g_timer_gpt3_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt2_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt2_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT2_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT2_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt2_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT2_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT2_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT2_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT2_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT2_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT2_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT2_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT2_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT2_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT2_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT2_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT2_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt2_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt2_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 2,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt2_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT2_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT2_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt2 =
{
    .p_ctrl        = &g_timer_gpt2_ctrl,
    .p_cfg         = &g_timer_gpt2_cfg,
    .p_api         = &g_timer_on_gpt
};
gpt_instance_ctrl_t g_timer_gpt0_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt0_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT0_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt0_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (BSP_IRQ_DISABLED),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT0_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT0_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT0_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT0_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt0_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt0_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 0,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt0_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt0 =
{
    .p_ctrl        = &g_timer_gpt0_ctrl,
    .p_cfg         = &g_timer_gpt0_cfg,
    .p_api         = &g_timer_on_gpt
};
iic_master_instance_ctrl_t g_i2c_master2_ctrl;
const iic_master_extended_cfg_t g_i2c_master2_extend =
{
    .timeout_mode             = IIC_MASTER_TIMEOUT_MODE_SHORT,
    .timeout_scl_low          = IIC_MASTER_TIMEOUT_SCL_LOW_ENABLED,
    .smbus_operation         = 0,
    /* Actual calculated bitrate: 98945. Actual calculated duty cycle: 51%. */ .clock_settings.brl_value = 15, .clock_settings.brh_value = 16, .clock_settings.cks_value = 4, .clock_settings.sddl_value = 0, .clock_settings.dlcs_value = 0,
};
const i2c_master_cfg_t g_i2c_master2_cfg =
{
    .channel             = 2,
    .rate                = I2C_MASTER_RATE_STANDARD,
    .slave               = 0x00,
    .addr_mode           = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
    .p_callback          = i2c_callback,
    .p_context           = NULL,
#if defined(VECTOR_NUMBER_IIC2_RXI)
    .rxi_irq             = VECTOR_NUMBER_IIC2_RXI,
#else
    .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC2_TXI)
    .txi_irq             = VECTOR_NUMBER_IIC2_TXI,
#else
    .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC2_TEI)
    .tei_irq             = VECTOR_NUMBER_IIC2_TEI,
#else
    .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC2_ERI)
    .eri_irq             = VECTOR_NUMBER_IIC2_ERI,
#else
    .eri_irq             = FSP_INVALID_VECTOR,
#endif
    .ipl                 = (12),
    .p_extend            = &g_i2c_master2_extend,
};
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_master2 =
{
    .p_ctrl        = &g_i2c_master2_ctrl,
    .p_cfg         = &g_i2c_master2_cfg,
    .p_api         = &g_i2c_master_on_iic
};
iic_master_instance_ctrl_t g_i2c_master1_ctrl;
const iic_master_extended_cfg_t g_i2c_master1_extend =
{
    .timeout_mode             = IIC_MASTER_TIMEOUT_MODE_SHORT,
    .timeout_scl_low          = IIC_MASTER_TIMEOUT_SCL_LOW_ENABLED,
    .smbus_operation         = 0,
    /* Actual calculated bitrate: 98945. Actual calculated duty cycle: 51%. */ .clock_settings.brl_value = 15, .clock_settings.brh_value = 16, .clock_settings.cks_value = 4, .clock_settings.sddl_value = 0, .clock_settings.dlcs_value = 0,
};
const i2c_master_cfg_t g_i2c_master1_cfg =
{
    .channel             = 1,
    .rate                = I2C_MASTER_RATE_STANDARD,
    .slave               = 0x00,
    .addr_mode           = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
    .p_callback          = i2c_callback,
    .p_context           = NULL,
#if defined(VECTOR_NUMBER_IIC1_RXI)
    .rxi_irq             = VECTOR_NUMBER_IIC1_RXI,
#else
    .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC1_TXI)
    .txi_irq             = VECTOR_NUMBER_IIC1_TXI,
#else
    .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC1_TEI)
    .tei_irq             = VECTOR_NUMBER_IIC1_TEI,
#else
    .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC1_ERI)
    .eri_irq             = VECTOR_NUMBER_IIC1_ERI,
#else
    .eri_irq             = FSP_INVALID_VECTOR,
#endif
    .ipl                 = (12),
    .p_extend            = &g_i2c_master1_extend,
};
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_master1 =
{
    .p_ctrl        = &g_i2c_master1_ctrl,
    .p_cfg         = &g_i2c_master1_cfg,
    .p_api         = &g_i2c_master_on_iic
};
iic_master_instance_ctrl_t g_i2c_master0_ctrl;
const iic_master_extended_cfg_t g_i2c_master0_extend =
{
    .timeout_mode             = IIC_MASTER_TIMEOUT_MODE_SHORT,
    .timeout_scl_low          = IIC_MASTER_TIMEOUT_SCL_LOW_ENABLED,
    .smbus_operation         = 0,
    /* Actual calculated bitrate: 98945. Actual calculated duty cycle: 51%. */ .clock_settings.brl_value = 15, .clock_settings.brh_value = 16, .clock_settings.cks_value = 4, .clock_settings.sddl_value = 0, .clock_settings.dlcs_value = 0,
};
const i2c_master_cfg_t g_i2c_master0_cfg =
{
    .channel             = 0,
    .rate                = I2C_MASTER_RATE_STANDARD,
    .slave               = 0x00,
    .addr_mode           = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_tx       = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
                .p_transfer_rx       = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
    .p_callback          = i2c_callback,
    .p_context           = NULL,
#if defined(VECTOR_NUMBER_IIC0_RXI)
    .rxi_irq             = VECTOR_NUMBER_IIC0_RXI,
#else
    .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC0_TXI)
    .txi_irq             = VECTOR_NUMBER_IIC0_TXI,
#else
    .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC0_TEI)
    .tei_irq             = VECTOR_NUMBER_IIC0_TEI,
#else
    .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC0_ERI)
    .eri_irq             = VECTOR_NUMBER_IIC0_ERI,
#else
    .eri_irq             = FSP_INVALID_VECTOR,
#endif
    .ipl                 = (12),
    .p_extend            = &g_i2c_master0_extend,
};
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_master0 =
{
    .p_ctrl        = &g_i2c_master0_ctrl,
    .p_cfg         = &g_i2c_master0_cfg,
    .p_api         = &g_i2c_master_on_iic
};
wdt_instance_ctrl_t g_wdt0_ctrl;

const wdt_cfg_t g_wdt0_cfg =
{
    .timeout = WDT_TIMEOUT_16384,
    .clock_division = WDT_CLOCK_DIVISION_8192,
    .window_start = WDT_WINDOW_START_100,
    .window_end = WDT_WINDOW_END_0,
    .reset_control = WDT_RESET_CONTROL_RESET,
    .stop_control = WDT_STOP_CONTROL_ENABLE,
    .p_callback = NULL,
};

/* Instance structure to use this module. */
const wdt_instance_t g_wdt0 =
{
    .p_ctrl        = &g_wdt0_ctrl,
    .p_cfg         = &g_wdt0_cfg,
    .p_api         = &g_wdt_on_wdt
};
rtc_instance_ctrl_t g_rtc0_ctrl;
const rtc_error_adjustment_cfg_t g_rtc0_err_cfg =
{
    .adjustment_mode         = RTC_ERROR_ADJUSTMENT_MODE_AUTOMATIC,
    .adjustment_period       = RTC_ERROR_ADJUSTMENT_PERIOD_10_SECOND,
    .adjustment_type         = RTC_ERROR_ADJUSTMENT_NONE,
    .adjustment_value        = 0,
};
const rtc_cfg_t g_rtc0_cfg =
{
    .clock_source            = RTC_CLOCK_SOURCE_SUBCLK,
    .freq_compare_value = 255,
    .p_err_cfg               = &g_rtc0_err_cfg,
    .p_callback              = NULL,
    .p_context               = NULL,
    .p_extend                = NULL,
    .alarm_ipl               = (BSP_IRQ_DISABLED),
    .periodic_ipl            = (BSP_IRQ_DISABLED),
    .carry_ipl               = (12),
#if defined(VECTOR_NUMBER_RTC_ALARM)
    .alarm_irq               = VECTOR_NUMBER_RTC_ALARM,
#else
    .alarm_irq               = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_RTC_PERIOD)
    .periodic_irq            = VECTOR_NUMBER_RTC_PERIOD,
#else
    .periodic_irq            = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_RTC_CARRY)
    .carry_irq               = VECTOR_NUMBER_RTC_CARRY,
#else
    .carry_irq               = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const rtc_instance_t g_rtc0 =
{
    .p_ctrl        = &g_rtc0_ctrl,
    .p_cfg         = &g_rtc0_cfg,
    .p_api         = &g_rtc_on_rtc
};
dac_instance_ctrl_t g_dac1_ctrl;
const dac_extended_cfg_t g_dac1_ext_cfg =
{
    .enable_charge_pump       = 0,
    .data_format              = DAC_DATA_FORMAT_FLUSH_RIGHT,
    .output_amplifier_enabled = 0,
    .internal_output_enabled  = false,
    .ref_volt_sel             = (dac_ref_volt_sel_t) (0)
};
const dac_cfg_t g_dac1_cfg =
{
    .channel             = 1,
    .ad_da_synchronized  = false,
    .p_extend            = &g_dac1_ext_cfg
};
/* Instance structure to use this module. */
const dac_instance_t g_dac1 =
{
    .p_ctrl    = &g_dac1_ctrl,
    .p_cfg     = &g_dac1_cfg,
    .p_api     = &g_dac_on_dac
};
dac_instance_ctrl_t g_dac0_ctrl;
const dac_extended_cfg_t g_dac0_ext_cfg =
{
    .enable_charge_pump       = 0,
    .data_format              = DAC_DATA_FORMAT_FLUSH_RIGHT,
    .output_amplifier_enabled = 0,
    .internal_output_enabled  = false,
    .ref_volt_sel             = (dac_ref_volt_sel_t) (0)
};
const dac_cfg_t g_dac0_cfg =
{
    .channel             = 0,
    .ad_da_synchronized  = false,
    .p_extend            = &g_dac0_ext_cfg
};
/* Instance structure to use this module. */
const dac_instance_t g_dac0 =
{
    .p_ctrl    = &g_dac0_ctrl,
    .p_cfg     = &g_dac0_cfg,
    .p_api     = &g_dac_on_dac
};
adc_instance_ctrl_t g_adc1_ctrl;
const adc_extended_cfg_t g_adc1_cfg_extend =
{
    .add_average_count   = ADC_ADD_OFF,
    .clearing            = ADC_CLEAR_AFTER_READ_ON,
    .trigger             = ADC_START_SOURCE_DISABLED,
    .trigger_group_b     = ADC_START_SOURCE_DISABLED,
    .double_trigger_mode = ADC_DOUBLE_TRIGGER_DISABLED,
    .adc_vref_control    = ADC_VREF_CONTROL_VREFH,
    .enable_adbuf        = 0,
#if defined(VECTOR_NUMBER_ADC1_WINDOW_A)
    .window_a_irq        = VECTOR_NUMBER_ADC1_WINDOW_A,
#else
    .window_a_irq        = FSP_INVALID_VECTOR,
#endif
    .window_a_ipl        = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC1_WINDOW_B)
    .window_b_irq      = VECTOR_NUMBER_ADC1_WINDOW_B,
#else
    .window_b_irq      = FSP_INVALID_VECTOR,
#endif
    .window_b_ipl      = (BSP_IRQ_DISABLED),
};
const adc_cfg_t g_adc1_cfg =
{
    .unit                = 1,
    .mode                = ADC_MODE_SINGLE_SCAN,
    .resolution          = ADC_RESOLUTION_12_BIT,
    .alignment           = (adc_alignment_t) ADC_ALIGNMENT_RIGHT,
    .trigger             = (adc_trigger_t)0xF, // Not used
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_adc1_cfg_extend,
#if defined(VECTOR_NUMBER_ADC1_SCAN_END)
    .scan_end_irq        = VECTOR_NUMBER_ADC1_SCAN_END,
#else
    .scan_end_irq        = FSP_INVALID_VECTOR,
#endif
    .scan_end_ipl        = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC1_SCAN_END_B)
    .scan_end_b_irq      = VECTOR_NUMBER_ADC1_SCAN_END_B,
#else
    .scan_end_b_irq      = FSP_INVALID_VECTOR,
#endif
    .scan_end_b_ipl      = (BSP_IRQ_DISABLED),
};
#if ((0) | (0))
const adc_window_cfg_t g_adc1_window_cfg =
{
    .compare_mask        =  0,
    .compare_mode_mask   =  0,
    .compare_cfg         = (adc_compare_cfg_t) ((0) | (0) | (0) | (ADC_COMPARE_CFG_EVENT_OUTPUT_OR)),
    .compare_ref_low     = 0,
    .compare_ref_high    = 0,
    .compare_b_channel   = (ADC_WINDOW_B_CHANNEL_0),
    .compare_b_mode      = (ADC_WINDOW_B_MODE_LESS_THAN_OR_OUTSIDE),
    .compare_b_ref_low   = 0,
    .compare_b_ref_high  = 0,
};
#endif
const adc_channel_cfg_t g_adc1_channel_cfg =
{
    .scan_mask           =  0,
    .scan_mask_group_b   =  0,
    .priority_group_a    = ADC_GROUP_A_PRIORITY_OFF,
    .add_mask            =  0,
    .sample_hold_mask    =  0,
    .sample_hold_states  = 24,
#if ((0) | (0))
    .p_window_cfg        = (adc_window_cfg_t *) &g_adc1_window_cfg,
#else
    .p_window_cfg        = NULL,
#endif
};
/* Instance structure to use this module. */
const adc_instance_t g_adc1 =
{
    .p_ctrl    = &g_adc1_ctrl,
    .p_cfg     = &g_adc1_cfg,
    .p_channel_cfg = &g_adc1_channel_cfg,
    .p_api     = &g_adc_on_adc
};
adc_instance_ctrl_t g_adc0_ctrl;
const adc_extended_cfg_t g_adc0_cfg_extend =
{
    .add_average_count   = ADC_ADD_OFF,
    .clearing            = ADC_CLEAR_AFTER_READ_ON,
    .trigger             = ADC_START_SOURCE_DISABLED,
    .trigger_group_b     = ADC_START_SOURCE_DISABLED,
    .double_trigger_mode = ADC_DOUBLE_TRIGGER_DISABLED,
    .adc_vref_control    = ADC_VREF_CONTROL_VREFH,
    .enable_adbuf        = 0,
#if defined(VECTOR_NUMBER_ADC0_WINDOW_A)
    .window_a_irq        = VECTOR_NUMBER_ADC0_WINDOW_A,
#else
    .window_a_irq        = FSP_INVALID_VECTOR,
#endif
    .window_a_ipl        = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC0_WINDOW_B)
    .window_b_irq      = VECTOR_NUMBER_ADC0_WINDOW_B,
#else
    .window_b_irq      = FSP_INVALID_VECTOR,
#endif
    .window_b_ipl      = (BSP_IRQ_DISABLED),
};
const adc_cfg_t g_adc0_cfg =
{
    .unit                = 0,
    .mode                = ADC_MODE_SINGLE_SCAN,
    .resolution          = ADC_RESOLUTION_12_BIT,
    .alignment           = (adc_alignment_t) ADC_ALIGNMENT_RIGHT,
    .trigger             = (adc_trigger_t)0xF, // Not used
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_adc0_cfg_extend,
#if defined(VECTOR_NUMBER_ADC0_SCAN_END)
    .scan_end_irq        = VECTOR_NUMBER_ADC0_SCAN_END,
#else
    .scan_end_irq        = FSP_INVALID_VECTOR,
#endif
    .scan_end_ipl        = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC0_SCAN_END_B)
    .scan_end_b_irq      = VECTOR_NUMBER_ADC0_SCAN_END_B,
#else
    .scan_end_b_irq      = FSP_INVALID_VECTOR,
#endif
    .scan_end_b_ipl      = (BSP_IRQ_DISABLED),
};
#if ((0) | (0))
const adc_window_cfg_t g_adc0_window_cfg =
{
    .compare_mask        =  0,
    .compare_mode_mask   =  0,
    .compare_cfg         = (adc_compare_cfg_t) ((0) | (0) | (0) | (ADC_COMPARE_CFG_EVENT_OUTPUT_OR)),
    .compare_ref_low     = 0,
    .compare_ref_high    = 0,
    .compare_b_channel   = (ADC_WINDOW_B_CHANNEL_0),
    .compare_b_mode      = (ADC_WINDOW_B_MODE_LESS_THAN_OR_OUTSIDE),
    .compare_b_ref_low   = 0,
    .compare_b_ref_high  = 0,
};
#endif
const adc_channel_cfg_t g_adc0_channel_cfg =
{
    .scan_mask           =  0,
    .scan_mask_group_b   =  0,
    .priority_group_a    = ADC_GROUP_A_PRIORITY_OFF,
    .add_mask            =  0,
    .sample_hold_mask    =  0,
    .sample_hold_states  = 24,
#if ((0) | (0))
    .p_window_cfg        = (adc_window_cfg_t *) &g_adc0_window_cfg,
#else
    .p_window_cfg        = NULL,
#endif
};
/* Instance structure to use this module. */
const adc_instance_t g_adc0 =
{
    .p_ctrl    = &g_adc0_ctrl,
    .p_cfg     = &g_adc0_cfg,
    .p_channel_cfg = &g_adc0_channel_cfg,
    .p_api     = &g_adc_on_adc
};
#if JPEG_CFG_ENCODE_ENABLE

            /* Luminance quantization table */static const uint8_t quant_table_0[] ={16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99, };/* Chrominance quantization table */static const uint8_t quant_table_1[] ={17, 18, 24, 47, 99, 99, 99, 99, 18, 21, 26, 66, 99, 99, 99, 99, 24, 26, 56, 99, 99, 99, 99, 99, 47, 66, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, };

            /* Luminance DC coefficients */
            static const uint8_t huffm_dc_table_0[] =
            {
                /* Table K.3 ITU-T81 specification */
                0x00, 0x01, 0x05, 0x01,
                0x01, 0x01, 0x01, 0x01,
                0x01, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x01, 0x02, 0x03,
                0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B,
            };

            /* Chrominance DC coefficients */
            static const uint8_t huffm_dc_table_1[] =
            {
                /* Table K.4 ITU-T81 specification */
                0x00, 0x03, 0x01, 0x01,
                0x01, 0x01, 0x01, 0x01,
                0x01, 0x01, 0x01, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x01, 0x02, 0x03,
                0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B,
            };

            /* Luminance AC coefficients */
            static const uint8_t huffm_ac_table_0[] =
            {
                /* Table K.5 ITU-T81 specification */
                0x00, 0x02, 0x01, 0x03,
                0x03, 0x02, 0x04, 0x03,
                0x05, 0x05, 0x04, 0x04,
                0x00, 0x00, 0x01, 0x7D,
                0x01, 0x02, 0x03, 0x00,
                0x04, 0x11, 0x05, 0x12,
                0x21, 0x31, 0x41, 0x06,
                0x13, 0x51, 0x61, 0x07,
                0x22, 0x71, 0x14, 0x32,
                0x81, 0x91, 0xA1, 0x08,
                0x23, 0x42, 0xB1, 0xC1,
                0x15, 0x52, 0xD1, 0xF0,
                0x24, 0x33, 0x62, 0x72,
                0x82, 0x09, 0x0A, 0x16,
                0x17, 0x18, 0x19, 0x1A,
                0x25, 0x26, 0x27, 0x28,
                0x29, 0x2A, 0x34, 0x35,
                0x36, 0x37, 0x38, 0x39,
                0x3A, 0x43, 0x44, 0x45,
                0x46, 0x47, 0x48, 0x49,
                0x4A, 0x53, 0x54, 0x55,
                0x56, 0x57, 0x58, 0x59,
                0x5A, 0x63, 0x64, 0x65,
                0x66, 0x67, 0x68, 0x69,
                0x6A, 0x73, 0x74, 0x75,
                0x76, 0x77, 0x78, 0x79,
                0x7A, 0x83, 0x84, 0x85,
                0x86, 0x87, 0x88, 0x89,
                0x8A, 0x92, 0x93, 0x94,
                0x95, 0x96, 0x97, 0x98,
                0x99, 0x9A, 0xA2, 0xA3,
                0xA4, 0xA5, 0xA6, 0xA7,
                0xA8, 0xA9, 0xAA, 0xB2,
                0xB3, 0xB4, 0xB5, 0xB6,
                0xB7, 0xB8, 0xB9, 0xBA,
                0xC2, 0xC3, 0xC4, 0xC5,
                0xC6, 0xC7, 0xC8, 0xC9,
                0xCA, 0xD2, 0xD3, 0xD4,
                0xD5, 0xD6, 0xD7, 0xD8,
                0xD9, 0xDA, 0xE1, 0xE2,
                0xE3, 0xE4, 0xE5, 0xE6,
                0xE7, 0xE8, 0xE9, 0xEA,
                0xF1, 0xF2, 0xF3, 0xF4,
                0xF5, 0xF6, 0xF7, 0xF8,
                0xF9, 0xFA,
            };

            /* Chrominance AC coefficients */
            static const uint8_t huffm_ac_table_1[] =
            {
                /* Table K.6 ITU-T81 specification */
                0x00, 0x02, 0x01, 0x02,
                0x04, 0x04, 0x03, 0x04,
                0x07, 0x05, 0x04, 0x04,
                0x00, 0x01, 0x02, 0x77,
                0x00, 0x01, 0x02, 0x03,
                0x11, 0x04, 0x05, 0x21,
                0x31, 0x06, 0x12, 0x41,
                0x51, 0x07, 0x61, 0x71,
                0x13, 0x22, 0x32, 0x81,
                0x08, 0x14, 0x42, 0x91,
                0xA1, 0xB1, 0xC1, 0x09,
                0x23, 0x33, 0x52, 0xF0,
                0x15, 0x62, 0x72, 0xD1,
                0x0A, 0x16, 0x24, 0x34,
                0xE1, 0x25, 0xF1, 0x17,
                0x18, 0x19, 0x1A, 0x26,
                0x27, 0x28, 0x29, 0x2A,
                0x35, 0x36, 0x37, 0x38,
                0x39, 0x3A, 0x43, 0x44,
                0x45, 0x46, 0x47, 0x48,
                0x49, 0x4A, 0x53, 0x54,
                0x55, 0x56, 0x57, 0x58,
                0x59, 0x5A, 0x63, 0x64,
                0x65, 0x66, 0x67, 0x68,
                0x69, 0x6A, 0x73, 0x74,
                0x75, 0x76, 0x77, 0x78,
                0x79, 0x7A, 0x82, 0x83,
                0x84, 0x85, 0x86, 0x87,
                0x88, 0x89, 0x8A, 0x92,
                0x93, 0x94, 0x95, 0x96,
                0x97, 0x98, 0x99, 0x9A,
                0xA2, 0xA3, 0xA4, 0xA5,
                0xA6, 0xA7, 0xA8, 0xA9,
                0xAA, 0xB2, 0xB3, 0xB4,
                0xB5, 0xB6, 0xB7, 0xB8,
                0xB9, 0xBA, 0xC2, 0xC3,
                0xC4, 0xC5, 0xC6, 0xC7,
                0xC8, 0xC9, 0xCA, 0xD2,
                0xD3, 0xD4, 0xD5, 0xD6,
                0xD7, 0xD8, 0xD9, 0xDA,
                0xE2, 0xE3, 0xE4, 0xE5,
                0xE6, 0xE7, 0xE8, 0xE9,
                0xEA, 0xF2, 0xF3, 0xF4,
                0xF5, 0xF6, 0xF7, 0xF8,
                0xF9, 0xFA,
            };
            #endif

            jpeg_instance_ctrl_t g_jpeg0_ctrl;
            const jpeg_cfg_t  g_jpeg0_cfg =
            {
                .jedi_ipl           = (12),
                .jdti_ipl           = (12),
            #if defined(VECTOR_NUMBER_JPEG_JEDI)
                .jedi_irq           = VECTOR_NUMBER_JPEG_JEDI,
            #else
                .jedi_irq           = FSP_INVALID_VECTOR,
            #endif
                .jdti_irq           = VECTOR_NUMBER_JPEG_JDTI,

            #if JPEG_CFG_DECODE_ENABLE && JPEG_CFG_ENCODE_ENABLE
                .default_mode              = (JPEG_MODE_DECODE),
            #endif

            #if JPEG_CFG_DECODE_ENABLE
                .decode_input_data_order   = JPEG_DATA_ORDER_NORMAL,
                .decode_output_data_order  = JPEG_DATA_ORDER_NORMAL,
                .pixel_format              = JPEG_DECODE_PIXEL_FORMAT_RGB565,
                .alpha_value               = 255,
                .p_decode_callback         = NULL,
                .p_decode_context          = NULL,
            #endif

            #if JPEG_CFG_ENCODE_ENABLE
                .encode_input_data_order   = JPEG_DATA_ORDER_NORMAL,
                .encode_output_data_order  = JPEG_DATA_ORDER_NORMAL,
                .dri_marker                = 512,
                .horizontal_resolution     = 480,
                .vertical_resolution       = 272,
                .horizontal_stride_pixels  = 480,
                .p_quant_luma_table        = quant_table_0,
                .p_quant_chroma_table      = quant_table_1,
                .p_huffman_luma_ac_table   = huffm_ac_table_0,
                .p_huffman_luma_dc_table   = huffm_dc_table_0,
                .p_huffman_chroma_ac_table = huffm_ac_table_1,
                .p_huffman_chroma_dc_table = huffm_dc_table_1,
                .p_encode_callback         = NULL,
                .p_encode_context          = NULL,
            #endif
            };

            const jpeg_instance_t g_jpeg0 =
            {
                .p_api  = (jpeg_api_t const *)&g_jpeg_on_jpeg,
                .p_ctrl = &g_jpeg0_ctrl,
                .p_cfg  = &g_jpeg0_cfg
            };
gpt_instance_ctrl_t g_timer_gpt1_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_gpt1_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT1_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_gpt1_extend =
{
    .gtioca = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = false,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .start_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .stop_source         = (gpt_source_t) ( GPT_SOURCE_NONE),
    .clear_source        = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_up_source     = (gpt_source_t) ( GPT_SOURCE_NONE),
    .count_down_source   = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_b_source    = (gpt_source_t) ( GPT_SOURCE_NONE),
    .capture_a_ipl       = (10),
    .capture_b_ipl       = (BSP_IRQ_DISABLED),
    .compare_match_c_ipl = (BSP_IRQ_DISABLED),
    .compare_match_d_ipl = (BSP_IRQ_DISABLED),
    .compare_match_e_ipl = (BSP_IRQ_DISABLED),
    .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_A,
#else
    .capture_a_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_B,
#else
    .capture_b_irq         = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT1_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT1_COMPARE_C,
#else
    .compare_match_c_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT1_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT1_COMPARE_D,
#else
    .compare_match_d_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT1_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT1_COMPARE_E,
#else
    .compare_match_e_irq   = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT1_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT1_COMPARE_F,
#else
    .compare_match_f_irq   = FSP_INVALID_VECTOR,
#endif
     .compare_match_value = { (uint32_t)0x0, /* CMP_A */(uint32_t)0x0, /* CMP_B */(uint32_t)0x0, /* CMP_C */(uint32_t)0x0, /* CMP_D */(uint32_t)0x0, /* CMP_E */(uint32_t)0x0, /* CMP_F */ }, .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U),
    .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
    .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer_gpt1_pwm_extend,
#else
    .p_pwm_cfg             = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
    .gtior_setting.gtior = 0U,
#endif

    .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
    .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL,
};

const timer_cfg_t g_timer_gpt1_cfg =
{
    .mode                = TIMER_MODE_PERIODIC,
    /* Actual period: 286.331153 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x100000000, .duty_cycle_counts = 0x80000000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_8,
    .channel             = 1,
    .p_callback          = usticker_compare_match_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt1_extend,
    .cycle_end_ipl       = (15),
#if defined(VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_gpt1 =
{
    .p_ctrl        = &g_timer_gpt1_ctrl,
    .p_cfg         = &g_timer_gpt1_cfg,
    .p_api         = &g_timer_on_gpt
};
void g_hal_init(void) {
g_common_init();
}

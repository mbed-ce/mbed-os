/* Copyright (c) 2024 Renesas Electronics Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* generated HAL source file - do not edit */
#include "hal_data.h"

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

/* Nominal and Data bit timing configuration */

can_bit_timing_cfg_t g_canfd1_bit_timing_cfg =
{
    /* Actual bitrate: 500000 Hz. Actual sample point: 75 %. */
    .baud_rate_prescaler = 1,
    .time_segment_1 = 89,
    .time_segment_2 = 30,
    .synchronization_jump_width = 4
};

#if BSP_FEATURE_CANFD_FD_SUPPORT
can_bit_timing_cfg_t g_canfd1_data_timing_cfg =
{
    /* Actual bitrate: 2000000 Hz. Actual sample point: 77 %. */
    .baud_rate_prescaler = 1,
    .time_segment_1 = 22,
    .time_segment_2 = 7,
    .synchronization_jump_width = 1
};
#endif


extern const canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH1_RULE_NUM];


#define CANFD_CFG_COMMONFIFO0 (((0) << R_CANFD_CFDCFCC_CFE_Pos) | \
                                        ((0) << R_CANFD_CFDCFCC_CFRXIE_Pos) | \
                                        ((0) << R_CANFD_CFDCFCC_CFTXIE_Pos) | \
                                        ((7) << R_CANFD_CFDCFCC_CFPLS_Pos) | \
                                        ((1) << R_CANFD_CFDCFCC_CFM_Pos) | \
                                        ((0) << R_CANFD_CFDCFCC_CFITSS_Pos) | \
                                        ((0) << R_CANFD_CFDCFCC_CFITR_Pos) | \
                                        ((0)  << R_CANFD_CFDCFCC_CFIM_Pos) | \
                                        ((3U) << R_CANFD_CFDCFCC_CFIGCV_Pos) | \
                                        ((0) << R_CANFD_CFDCFCC_CFTML_Pos) | \
                                        ((1) << R_CANFD_CFDCFCC_CFDC_Pos) | \
                                        (0 << R_CANFD_CFDCFCC_CFITT_Pos))


/* Buffer RAM used: 608 bytes */
canfd_global_cfg_t g_canfd1_global_cfg =
{
    .global_interrupts = ( 0x3),
    .global_config = ((R_CANFD_CFDGCFG_TPRI_Msk) | (0) | (BSP_CFG_CANFDCLK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC ? R_CANFD_CFDGCFG_DCS_Msk : 0U) | (0) |
                      ((0) << R_CANFD_CFDGCFG_ITRCP_Pos)),
    .rx_mb_config = (0 | ((7) << R_CANFD_CFDRMNB_RMPLS_Pos)),
    .global_err_ipl = CANFD_CFG_GLOBAL_ERR_IPL,
    .rx_fifo_ipl = CANFD_CFG_RX_FIFO_IPL,
    .rx_fifo_config =
    {
        ((3U) << R_CANFD_CFDRFCC_RFIGCV_Pos) | ((2) << R_CANFD_CFDRFCC_RFDC_Pos) | ((7) << R_CANFD_CFDRFCC_RFPLS_Pos) | ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk)) | ((1)),
        ((3U) << R_CANFD_CFDRFCC_RFIGCV_Pos) | ((3) << R_CANFD_CFDRFCC_RFDC_Pos) | ((0) << R_CANFD_CFDRFCC_RFPLS_Pos) | ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk)) | ((0))
    },
    .common_fifo_config =
    {
        CANFD_CFG_COMMONFIFO0
    }
};

canfd_extended_cfg_t g_canfd1_extended_cfg =
{
    .p_afl              = p_canfd1_afl,
    .txmb_txi_enable    = ((1ULL << 0) |  0ULL),
    .error_interrupts   = (R_CANFD_CFDC_CTR_EWIE_Msk | R_CANFD_CFDC_CTR_EPIE_Msk | R_CANFD_CFDC_CTR_BOEIE_Msk | R_CANFD_CFDC_CTR_BORIE_Msk | R_CANFD_CFDC_CTR_OLIE_Msk |  0U),
#if BSP_FEATURE_CANFD_FD_SUPPORT
    .p_data_timing      = &g_canfd1_data_timing_cfg,
#else
    .p_data_timing      = NULL,
#endif
    .delay_compensation = (1),
    .p_global_cfg       = &g_canfd1_global_cfg,
};

canfd_instance_ctrl_t g_canfd1_ctrl;
const can_cfg_t g_canfd1_cfg =
{
    .channel                = 1,
    .p_bit_timing           = &g_canfd1_bit_timing_cfg,
    .p_callback             = can_callback,
    .p_extend               = &g_canfd1_extended_cfg,
    .p_context              = NULL,
    .ipl                    = (12),
#if defined(VECTOR_NUMBER_CAN1_COMFRX)
    .rx_irq             = VECTOR_NUMBER_CAN1_COMFRX,
#else
    .rx_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN1_TX)
    .tx_irq             = VECTOR_NUMBER_CAN1_TX,
#else
    .tx_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN1_CHERR)
    .error_irq             = VECTOR_NUMBER_CAN1_CHERR,
#else
    .error_irq             = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const can_instance_t g_canfd1 =
{
    .p_ctrl        = &g_canfd1_ctrl,
    .p_cfg         = &g_canfd1_cfg,
    .p_api         = &g_canfd_on_canfd
};
/* Nominal and Data bit timing configuration */

can_bit_timing_cfg_t g_canfd0_bit_timing_cfg =
{
    /* Actual bitrate: 500000 Hz. Actual sample point: 75 %. */
    .baud_rate_prescaler = 1,
    .time_segment_1 = 89,
    .time_segment_2 = 30,
    .synchronization_jump_width = 4
};

#if BSP_FEATURE_CANFD_FD_SUPPORT
can_bit_timing_cfg_t g_canfd0_data_timing_cfg =
{
    /* Actual bitrate: 2000000 Hz. Actual sample point: 77 %. */
    .baud_rate_prescaler = 1,
    .time_segment_1 = 22,
    .time_segment_2 = 7,
    .synchronization_jump_width = 1
};
#endif


extern const canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH0_RULE_NUM];

/* Buffer RAM used: 608 bytes */
canfd_global_cfg_t g_canfd0_global_cfg =
{
    .global_interrupts = ( 0x3),
    .global_config = ((R_CANFD_CFDGCFG_TPRI_Msk) | (0) | (BSP_CFG_CANFDCLK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC ? R_CANFD_CFDGCFG_DCS_Msk : 0U) | (0) |
                      ((0) << R_CANFD_CFDGCFG_ITRCP_Pos)),
    .rx_mb_config = (0 | ((7) << R_CANFD_CFDRMNB_RMPLS_Pos)),
    .global_err_ipl = CANFD_CFG_GLOBAL_ERR_IPL,
    .rx_fifo_ipl = CANFD_CFG_RX_FIFO_IPL,
    .rx_fifo_config =
    {
        ((3U) << R_CANFD_CFDRFCC_RFIGCV_Pos) | ((2) << R_CANFD_CFDRFCC_RFDC_Pos) | ((7) << R_CANFD_CFDRFCC_RFPLS_Pos) | ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk)) | ((1)),
        ((3U) << R_CANFD_CFDRFCC_RFIGCV_Pos) | ((3) << R_CANFD_CFDRFCC_RFDC_Pos) | ((0) << R_CANFD_CFDRFCC_RFPLS_Pos) | ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk)) | ((0))
    },
    .common_fifo_config =
    {
        CANFD_CFG_COMMONFIFO0
    }
};

canfd_extended_cfg_t g_canfd0_extended_cfg =
{
    .p_afl              = p_canfd0_afl,
    .txmb_txi_enable    = ((1ULL << 0) |  0ULL),
    .error_interrupts   = (R_CANFD_CFDC_CTR_EWIE_Msk | R_CANFD_CFDC_CTR_EPIE_Msk | R_CANFD_CFDC_CTR_BOEIE_Msk | R_CANFD_CFDC_CTR_BORIE_Msk | R_CANFD_CFDC_CTR_OLIE_Msk |  0U),
#if BSP_FEATURE_CANFD_FD_SUPPORT
    .p_data_timing      = &g_canfd0_data_timing_cfg,
#else
    .p_data_timing      = NULL,
#endif
    .delay_compensation = (1),
    .p_global_cfg       = &g_canfd0_global_cfg,
};

canfd_instance_ctrl_t g_canfd0_ctrl;
const can_cfg_t g_canfd0_cfg =
{
    .channel                = 0,
    .p_bit_timing           = &g_canfd0_bit_timing_cfg,
    .p_callback             = can_callback,
    .p_extend               = &g_canfd0_extended_cfg,
    .p_context              = NULL,
    .ipl                    = (12),
#if defined(VECTOR_NUMBER_CAN0_COMFRX)
    .rx_irq             = VECTOR_NUMBER_CAN0_COMFRX,
#else
    .rx_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN0_TX)
    .tx_irq             = VECTOR_NUMBER_CAN0_TX,
#else
    .tx_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN0_CHERR)
    .error_irq             = VECTOR_NUMBER_CAN0_CHERR,
#else
    .error_irq             = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const can_instance_t g_canfd0 =
{
    .p_ctrl        = &g_canfd0_ctrl,
    .p_cfg         = &g_canfd0_cfg,
    .p_api         = &g_canfd_on_canfd
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
extern void spi_b_tx_dmac_callback(spi_b_instance_ctrl_t const * const p_ctrl);

void g_spi1_tx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_b_tx_dmac_callback(&g_spi1_ctrl);
}
#endif

#if (RA_NOT_DEFINED) != (RA_NOT_DEFINED)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void spi_b_rx_dmac_callback(spi_b_instance_ctrl_t const * const p_ctrl);

void g_spi1_rx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_b_rx_dmac_callback(&g_spi1_ctrl);
}
#endif
#undef RA_NOT_DEFINED

spi_b_instance_ctrl_t g_spi1_ctrl;

/** SPI extended configuration for SPI HAL driver */
const spi_b_extended_cfg_t g_spi1_ext_cfg =
{
    .spi_clksyn         = SPI_B_SSL_MODE_SPI,
    .spi_comm           = SPI_B_COMMUNICATION_FULL_DUPLEX,
    .ssl_polarity        = SPI_B_SSLP_LOW,
    .ssl_select          = SPI_B_SSL_SELECT_SSL0,
    .mosi_idle           = SPI_B_MOSI_IDLE_VALUE_FIXING_DISABLE,
    .parity              = SPI_B_PARITY_MODE_DISABLE,
    .byte_swap           = SPI_B_BYTE_SWAP_DISABLE,
    .clock_source        = SPI_B_CLOCK_SOURCE_SCISPICLK,
    .spck_div            = {
        /* Actual calculated bitrate: 60000000. */ .spbr = 0, .brdv = 0
    },
    .spck_delay          = SPI_B_DELAY_COUNT_1,
    .ssl_negation_delay  = SPI_B_DELAY_COUNT_1,
    .next_access_delay   = SPI_B_DELAY_COUNT_1,
    .burst_interframe_delay = SPI_B_BURST_TRANSFER_WITH_DELAY

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
    .p_api         = &g_spi_on_spi_b
};
sci_b_uart_instance_ctrl_t     g_uart9_ctrl;

            sci_b_baud_setting_t               g_uart9_baud_setting =
            {
                /* Baud rate calculated with 0.160% error. */ .baudrate_bits_b.abcse = 0, .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 64, .baudrate_bits_b.mddr = (uint8_t) 256, .baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_b_uart_extended_cfg_t g_uart9_cfg_extend =
            {
                .clock                = SCI_B_UART_CLOCK_INT,
                .rx_edge_start          = SCI_B_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_B_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_B_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart9_baud_setting,
                .flow_control           = SCI_B_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting          = {
                    .enable = SCI_B_UART_RS485_DISABLE,
                    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
                    .assertion_time = 1,
                    .negation_time = 1,
                }
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
    .p_api         = &g_uart_on_sci_b
};
sci_b_uart_instance_ctrl_t     g_uart4_ctrl;

            sci_b_baud_setting_t               g_uart4_baud_setting =
            {
                /* Baud rate calculated with 0.160% error. */ .baudrate_bits_b.abcse = 0, .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 64, .baudrate_bits_b.mddr = (uint8_t) 256, .baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_b_uart_extended_cfg_t g_uart4_cfg_extend =
            {
                .clock                = SCI_B_UART_CLOCK_INT,
                .rx_edge_start          = SCI_B_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_B_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_B_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart4_baud_setting,
                .flow_control           = SCI_B_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting          = {
                    .enable = SCI_B_UART_RS485_DISABLE,
                    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
                    .assertion_time = 1,
                    .negation_time = 1,
                }
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
    .p_api         = &g_uart_on_sci_b
};
sci_b_uart_instance_ctrl_t     g_uart3_ctrl;

            sci_b_baud_setting_t               g_uart3_baud_setting =
            {
                /* Baud rate calculated with 0.160% error. */ .baudrate_bits_b.abcse = 0, .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 64, .baudrate_bits_b.mddr = (uint8_t) 256, .baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_b_uart_extended_cfg_t g_uart3_cfg_extend =
            {
                .clock                = SCI_B_UART_CLOCK_INT,
                .rx_edge_start          = SCI_B_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_B_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_B_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart3_baud_setting,
                .flow_control           = SCI_B_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting          = {
                    .enable = SCI_B_UART_RS485_DISABLE,
                    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
                    .assertion_time = 1,
                    .negation_time = 1,
                }
            };

            /** UART interface configuration */
            const uart_cfg_t g_uart3_cfg =
            {
                .channel             = 3,
                .data_bits           = UART_DATA_BITS_8,
                .parity              = UART_PARITY_OFF,
                .stop_bits           = UART_STOP_BITS_1,
                .p_callback          = uart_callback,
                .p_context           = NULL,
                .p_extend            = &g_uart3_cfg_extend,
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
#if defined(VECTOR_NUMBER_SCI3_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI3_RXI,
#else
                .rxi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI3_TXI,
#else
                .txi_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI3_TEI,
#else
                .tei_irq             = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI3_ERI,
#else
                .eri_irq             = FSP_INVALID_VECTOR,
#endif
            };

/* Instance structure to use this module. */
const uart_instance_t g_uart3 =
{
    .p_ctrl        = &g_uart3_ctrl,
    .p_cfg         = &g_uart3_cfg,
    .p_api         = &g_uart_on_sci_b
};
sci_b_uart_instance_ctrl_t     g_uart2_ctrl;

            sci_b_baud_setting_t               g_uart2_baud_setting =
            {
                /* Baud rate calculated with 0.160% error. */ .baudrate_bits_b.abcse = 0, .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 64, .baudrate_bits_b.mddr = (uint8_t) 256, .baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_b_uart_extended_cfg_t g_uart2_cfg_extend =
            {
                .clock                = SCI_B_UART_CLOCK_INT,
                .rx_edge_start          = SCI_B_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_B_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_B_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart2_baud_setting,
                .flow_control           = SCI_B_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting          = {
                    .enable = SCI_B_UART_RS485_DISABLE,
                    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
                    .assertion_time = 1,
                    .negation_time = 1,
                }
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
    .p_api         = &g_uart_on_sci_b
};
sci_b_uart_instance_ctrl_t     g_uart1_ctrl;

            sci_b_baud_setting_t               g_uart1_baud_setting =
            {
                /* Baud rate calculated with 0.160% error. */ .baudrate_bits_b.abcse = 0, .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 64, .baudrate_bits_b.mddr = (uint8_t) 256, .baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_b_uart_extended_cfg_t g_uart1_cfg_extend =
            {
                .clock                = SCI_B_UART_CLOCK_INT,
                .rx_edge_start          = SCI_B_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_B_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_B_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart1_baud_setting,
                .flow_control           = SCI_B_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting          = {
                    .enable = SCI_B_UART_RS485_DISABLE,
                    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
                    .assertion_time = 1,
                    .negation_time = 1,
                }
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
    .p_api         = &g_uart_on_sci_b
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
    .scan_mask           = ADC_MASK_CHANNEL_0 | ADC_MASK_CHANNEL_1 | ADC_MASK_CHANNEL_2 | ADC_MASK_CHANNEL_4 | ADC_MASK_CHANNEL_5 |  0,
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
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
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
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
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
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_4,
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
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
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
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
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
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_64,
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
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
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
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
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
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_64,
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
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
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
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
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
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_64,
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
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
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
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
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
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_64,
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
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
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
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
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
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_4,
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
sci_b_uart_instance_ctrl_t     g_uart0_ctrl;

            sci_b_baud_setting_t               g_uart0_baud_setting =
            {
                /* Baud rate calculated with 0.160% error. */ .baudrate_bits_b.abcse = 0, .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 64, .baudrate_bits_b.mddr = (uint8_t) 256, .baudrate_bits_b.brme = false
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_b_uart_extended_cfg_t g_uart0_cfg_extend =
            {
                .clock                = SCI_B_UART_CLOCK_INT,
                .rx_edge_start          = SCI_B_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_B_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_B_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart0_baud_setting,
                .flow_control           = SCI_B_UART_FLOW_CONTROL_RTS,
                #if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
                .flow_control_pin       = (bsp_io_port_pin_t) UINT16_MAX,
                #endif
                .rs485_setting          = {
                    .enable = SCI_B_UART_RS485_DISABLE,
                    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
                    .assertion_time = 1,
                    .negation_time = 1,
                }
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
    .p_api         = &g_uart_on_sci_b
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
extern void spi_b_tx_dmac_callback(spi_b_instance_ctrl_t const * const p_ctrl);

void g_spi0_tx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_b_tx_dmac_callback(&g_spi0_ctrl);
}
#endif

#if (RA_NOT_DEFINED) != (RA_NOT_DEFINED)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void spi_b_rx_dmac_callback(spi_b_instance_ctrl_t const * const p_ctrl);

void g_spi0_rx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_b_rx_dmac_callback(&g_spi0_ctrl);
}
#endif
#undef RA_NOT_DEFINED

spi_b_instance_ctrl_t g_spi0_ctrl;

/** SPI extended configuration for SPI HAL driver */
const spi_b_extended_cfg_t g_spi0_ext_cfg =
{
    .spi_clksyn         = SPI_B_SSL_MODE_SPI,
    .spi_comm           = SPI_B_COMMUNICATION_FULL_DUPLEX,
    .ssl_polarity        = SPI_B_SSLP_LOW,
    .ssl_select          = SPI_B_SSL_SELECT_SSL0,
    .mosi_idle           = SPI_B_MOSI_IDLE_VALUE_FIXING_DISABLE,
    .parity              = SPI_B_PARITY_MODE_DISABLE,
    .byte_swap           = SPI_B_BYTE_SWAP_DISABLE,
    .clock_source        = SPI_B_CLOCK_SOURCE_SCISPICLK,
    .spck_div            = {
        /* Actual calculated bitrate: 60000000. */ .spbr = 0, .brdv = 0
    },
    .spck_delay          = SPI_B_DELAY_COUNT_1,
    .ssl_negation_delay  = SPI_B_DELAY_COUNT_1,
    .next_access_delay   = SPI_B_DELAY_COUNT_1,
    .burst_interframe_delay = SPI_B_BURST_TRANSFER_WITH_DELAY

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
    .p_api         = &g_spi_on_spi_b
};
agt_instance_ctrl_t g_timer_agt1_ctrl;
const agt_extended_cfg_t g_timer_agt1_extend =
{
    .count_source            = AGT_CLOCK_PCLKB,
    .agto                    = AGT_PIN_CFG_DISABLED,
    .agtoab_settings_b.agtoa = AGT_PIN_CFG_START_LEVEL_LOW,
    .agtoab_settings_b.agtob = AGT_PIN_CFG_START_LEVEL_LOW,
    .measurement_mode        = AGT_MEASURE_DISABLED,
    .agtio_filter            = AGT_AGTIO_FILTER_NONE,
    .enable_pin              = AGT_ENABLE_PIN_NOT_USED,
    .trigger_edge            = AGT_TRIGGER_EDGE_RISING,
    .counter_bit_width       = AGT_COUNTER_BIT_WIDTH_16,
};
const timer_cfg_t g_timer_agt1_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0010922666666666667 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_8,
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
    .agtoab_settings_b.agtoa = AGT_PIN_CFG_START_LEVEL_LOW,
    .agtoab_settings_b.agtob = AGT_PIN_CFG_START_LEVEL_LOW,
    .measurement_mode        = AGT_MEASURE_DISABLED,
    .agtio_filter            = AGT_AGTIO_FILTER_NONE,
    .enable_pin              = AGT_ENABLE_PIN_NOT_USED,
    .trigger_edge            = AGT_TRIGGER_EDGE_RISING,
    .counter_bit_width       = AGT_COUNTER_BIT_WIDTH_16,
};
const timer_cfg_t g_timer_agt0_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.0010922666666666667 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_8,
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
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
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
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
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
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_4,
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
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
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
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
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
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_4,
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
    .gtioca = { .output_enabled = true,
                .stop_level     = GPT_PIN_LEVEL_LOW
              },
    .gtiocb = { .output_enabled = true,
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
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
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
    /* Actual period: 0.0005461333333333333 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_4,
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
    .p_callback          = NULL,
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
#if BSP_TZ_NONSECURE_BUILD
 #if defined(BSP_CFG_CLOCKS_SECURE) && BSP_CFG_CLOCKS_SECURE
  #error "The LPM module requires CGC registers to be non-secure when it is used in a TrustZone Non-secure project."
 #endif
#endif

lpm_instance_ctrl_t g_lpm0_ctrl;

const lpm_cfg_t g_lpm0_cfg =
{
    .low_power_mode     = LPM_MODE_SLEEP,
    .standby_wake_sources       =  (lpm_standby_wake_source_t) 0,
#if BSP_FEATURE_ICU_HAS_WUPEN2
    .standby_wake_sources_2     =  (lpm_standby_wake_source_2_t) 0,
#endif
#if BSP_FEATURE_LPM_HAS_SNOOZE
    .snooze_cancel_sources      = RA_NOT_DEFINED,
    .snooze_request_source      = (lpm_snooze_request_t) RA_NOT_DEFINED,
#if BSP_FEATURE_LPM_SNZEDCR_MASK > 0
    .snooze_end_sources         =  (lpm_snooze_end_t) 0,
#endif
    .dtc_state_in_snooze        = RA_NOT_DEFINED,
#endif
#if BSP_FEATURE_LPM_HAS_SBYCR_OPE
    .output_port_enable         = 0,
#endif
#if BSP_FEATURE_LPM_HAS_DEEP_STANDBY
    .io_port_state                = LPM_IO_PORT_NO_CHANGE,
    .power_supply_state           = LPM_POWER_SUPPLY_DEEP_STANDBY_MODE1,
    .deep_standby_cancel_source   =  (lpm_deep_standby_cancel_source_t) 0,
    .deep_standby_cancel_edge     =  (lpm_deep_standby_cancel_edge_t) 0,
#if BSP_FEATURE_LPM_HAS_DPSBYCR_DCSSMODE
    .deep_standby_soft_start_mode = LPM_DEEP_STANDBY_SOFT_START_MODE_0,
#endif
#endif
#if BSP_FEATURE_LPM_HAS_PDRAMSCR
    .ram_retention_cfg.ram_retention = (uint16_t) ( 0),
    .ram_retention_cfg.tcm_retention = false,
#endif
#if BSP_FEATURE_LPM_HAS_DPSBYCR_SRKEEP
    .ram_retention_cfg.standby_ram_retention = false,
#endif
#if BSP_FEATURE_LPM_HAS_LDO_SKEEP
    .ldo_standby_cfg.pll1_ldo = false,
    .ldo_standby_cfg.pll2_ldo = false,
    .ldo_standby_cfg.hoco_ldo = false,
#endif
#if BSP_FEATURE_LPM_HAS_FLASH_MODE_SELECT
    .lpm_flash_mode_select = false,
#endif
#if BSP_FEATURE_LPM_HAS_HOCO_STARTUP_SPEED_MODE
    .lpm_hoco_startup_speed = false,
#endif
#if BSP_FEATURE_LPM_HAS_STANDBY_SOSC_SELECT
    .lpm_standby_sosc = false,
#endif
    .p_extend           = NULL,
};

const lpm_instance_t g_lpm0 =
{
    .p_api = &g_lpm_on_lpm,
    .p_ctrl = &g_lpm0_ctrl,
    .p_cfg = &g_lpm0_cfg
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
    .capture_a_ipl       = (12),
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
    /* Actual period: 35.791394133333334 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x100000000, .duty_cycle_counts = 0x80000000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_8,
    .channel             = 1,
    .p_callback          = usticker_compare_match_callback,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_gpt1_extend,
    .cycle_end_ipl       = (10),
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
    .clock_source            = RTC_CLOCK_SOURCE_LOCO,
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
    .scan_mask           = ADC_MASK_CHANNEL_0 | ADC_MASK_CHANNEL_1 | ADC_MASK_CHANNEL_2 | ADC_MASK_CHANNEL_4 | ADC_MASK_CHANNEL_5 | ADC_MASK_CHANNEL_6 | ADC_MASK_CHANNEL_7 | ADC_MASK_CHANNEL_8 |  0,
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
void g_hal_init(void) {
g_common_init();
}

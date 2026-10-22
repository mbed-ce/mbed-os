/* generated HAL source file - do not edit */
#include "hal_data.h"

qspi_instance_ctrl_t g_qspi0_ctrl;

static const spi_flash_erase_command_t g_qspi0_erase_command_list[] =
{
#if 4096 > 0
    {.command = 0x20,     .size = 4096 },
#endif
#if 32768 > 0
    {.command = 0x52, .size = 32768 },
#endif
#if 65536 > 0
    {.command = 0xD8,      .size = 65536 },
#endif
#if 0xC7 > 0
    {.command = 0xC7,       .size  = SPI_FLASH_ERASE_SIZE_CHIP_ERASE         },
#endif
};
static const qspi_extended_cfg_t g_qspi0_extended_cfg =
{
    .min_qssl_deselect_cycles = QSPI_QSSL_MIN_HIGH_LEVEL_4_QSPCLK,
    .qspclk_div          = QSPI_QSPCLK_DIV_2,
};
const spi_flash_cfg_t g_qspi0_cfg =
{
    .spi_protocol        = SPI_FLASH_PROTOCOL_EXTENDED_SPI,
    .read_mode           = SPI_FLASH_READ_MODE_FAST_READ_QUAD_IO,
    .address_bytes       = SPI_FLASH_ADDRESS_BYTES_3,
    .dummy_clocks        = SPI_FLASH_DUMMY_CLOCKS_DEFAULT,
    .page_program_address_lines = SPI_FLASH_DATA_LINES_1,
    .page_size_bytes     = 256,
    .page_program_command = 0x02,
    .write_enable_command = 0x06,
    .status_command = 0x05,
    .write_status_bit    = 0,
    .xip_enter_command   = 0x20,
    .xip_exit_command    = 0xFF,
    .p_erase_command_list = &g_qspi0_erase_command_list[0],
    .erase_command_list_length = sizeof(g_qspi0_erase_command_list) / sizeof(g_qspi0_erase_command_list[0]),
    .p_extend            = &g_qspi0_extended_cfg,
};
/** This structure encompasses everything that is needed to use an instance of this interface. */
const spi_flash_instance_t g_qspi0 =
{
    .p_ctrl = &g_qspi0_ctrl,
    .p_cfg =  &g_qspi0_cfg,
    .p_api =  &g_qspi_on_spi_flash,
};
/* Nominal and Data bit timing configuration */

can_bit_timing_cfg_t g_canfd1_bit_timing_cfg =
{
    /* Actual bitrate: 500000 Hz. Actual sample point: 75 %. */
    .baud_rate_prescaler = 1,
    .time_segment_1 = 59,
    .time_segment_2 = 20,
    .synchronization_jump_width = 4
};

#if BSP_FEATURE_CANFD_FD_SUPPORT
can_bit_timing_cfg_t g_canfd1_data_timing_cfg =
{
    /* Actual bitrate: 2000000 Hz. Actual sample point: 75 %. */
    .baud_rate_prescaler = 1,
    .time_segment_1 = 14,
    .time_segment_2 = 5,
    .synchronization_jump_width = 1
};
#endif


extern const canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH1_RULE_NUM];
#ifndef CANFD_PRV_GLOBAL_CFG
#define CANFD_PRV_GLOBAL_CFG

#ifdef RA_NOT_DEFINED
#undef RA_NOT_DEFINED
#endif
#define RA_NOT_DEFINED (0)

/* Buffer RAM used: RA_NOT_DEFINED bytes */
canfd_global_cfg_t g_canfd_global_cfg =
{
    .global_interrupts = CANFD_CFG_GLOBAL_ERR_SOURCES,
    .global_config     = (CANFD_CFG_TX_PRIORITY | CANFD_CFG_DLC_CHECK | (BSP_CFG_CANFDCLK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC ? R_CANFD_CFDGCFG_DCS_Msk : 0U) | CANFD_CFG_FD_OVERFLOW |
                          ((RA_NOT_DEFINED) << R_CANFD_CFDGCFG_ITRCP_Pos)),
    .rx_mb_config      = (CANFD_CFG_RXMB_NUMBER | (CANFD_CFG_RXMB_SIZE << R_CANFD_CFDRMNB_RMPLS_Pos)),
    .global_err_ipl = CANFD_CFG_GLOBAL_ERR_IPL,
    .rx_fifo_ipl    = CANFD_CFG_RX_FIFO_IPL,
    .rx_fifo_config    =
    {
        ((CANFD_CFG_RXFIFO0_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO0_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO0_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO0_INT_MODE) | (CANFD_CFG_RXFIFO0_ENABLE)),
        ((CANFD_CFG_RXFIFO1_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO1_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO1_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO1_INT_MODE) | (CANFD_CFG_RXFIFO1_ENABLE)),
#if !BSP_FEATURE_CANFD_LITE
        ((CANFD_CFG_RXFIFO2_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO2_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO2_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO2_INT_MODE) | (CANFD_CFG_RXFIFO2_ENABLE)),
        ((CANFD_CFG_RXFIFO3_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO3_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO3_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO3_INT_MODE) | (CANFD_CFG_RXFIFO3_ENABLE)),
        ((CANFD_CFG_RXFIFO4_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO4_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO4_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO4_INT_MODE) | (CANFD_CFG_RXFIFO4_ENABLE)),
        ((CANFD_CFG_RXFIFO5_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO5_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO5_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO5_INT_MODE) | (CANFD_CFG_RXFIFO5_ENABLE)),
        ((CANFD_CFG_RXFIFO6_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO6_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO6_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO6_INT_MODE) | (CANFD_CFG_RXFIFO6_ENABLE)),
        ((CANFD_CFG_RXFIFO7_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO7_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO7_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO7_INT_MODE) | (CANFD_CFG_RXFIFO7_ENABLE)),
#endif
    },
    .common_fifo_config =
    {
        CANFD_CFG_COMMONFIFO0,
#if !BSP_FEATURE_CANFD_LITE
        CANFD_CFG_COMMONFIFO1,
        CANFD_CFG_COMMONFIFO2,
        CANFD_CFG_COMMONFIFO3,
        CANFD_CFG_COMMONFIFO4,
        CANFD_CFG_COMMONFIFO5,
#endif
    }
};
#undef RA_NOT_DEFINED

#endif

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
    .p_global_cfg       = &g_canfd_global_cfg,
};

canfd_instance_ctrl_t g_canfd1_ctrl;
const can_cfg_t g_canfd1_cfg =
{
    .channel                = 1,
    .p_bit_timing           = &g_canfd1_bit_timing_cfg,
    .p_callback             = canfd1_callback,
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
    .time_segment_1 = 59,
    .time_segment_2 = 20,
    .synchronization_jump_width = 4
};

#if BSP_FEATURE_CANFD_FD_SUPPORT
can_bit_timing_cfg_t g_canfd0_data_timing_cfg =
{
    /* Actual bitrate: 2000000 Hz. Actual sample point: 75 %. */
    .baud_rate_prescaler = 1,
    .time_segment_1 = 14,
    .time_segment_2 = 5,
    .synchronization_jump_width = 1
};
#endif


extern const canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH0_RULE_NUM];
#ifndef CANFD_PRV_GLOBAL_CFG
#define CANFD_PRV_GLOBAL_CFG

#ifdef RA_NOT_DEFINED
#undef RA_NOT_DEFINED
#endif
#define RA_NOT_DEFINED (0)

/* Buffer RAM used: RA_NOT_DEFINED bytes */
canfd_global_cfg_t g_canfd_global_cfg =
{
    .global_interrupts = CANFD_CFG_GLOBAL_ERR_SOURCES,
    .global_config     = (CANFD_CFG_TX_PRIORITY | CANFD_CFG_DLC_CHECK | (BSP_CFG_CANFDCLK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC ? R_CANFD_CFDGCFG_DCS_Msk : 0U) | CANFD_CFG_FD_OVERFLOW |
                          ((RA_NOT_DEFINED) << R_CANFD_CFDGCFG_ITRCP_Pos)),
    .rx_mb_config      = (CANFD_CFG_RXMB_NUMBER | (CANFD_CFG_RXMB_SIZE << R_CANFD_CFDRMNB_RMPLS_Pos)),
    .global_err_ipl = CANFD_CFG_GLOBAL_ERR_IPL,
    .rx_fifo_ipl    = CANFD_CFG_RX_FIFO_IPL,
    .rx_fifo_config    =
    {
        ((CANFD_CFG_RXFIFO0_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO0_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO0_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO0_INT_MODE) | (CANFD_CFG_RXFIFO0_ENABLE)),
        ((CANFD_CFG_RXFIFO1_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO1_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO1_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO1_INT_MODE) | (CANFD_CFG_RXFIFO1_ENABLE)),
#if !BSP_FEATURE_CANFD_LITE
        ((CANFD_CFG_RXFIFO2_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO2_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO2_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO2_INT_MODE) | (CANFD_CFG_RXFIFO2_ENABLE)),
        ((CANFD_CFG_RXFIFO3_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO3_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO3_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO3_INT_MODE) | (CANFD_CFG_RXFIFO3_ENABLE)),
        ((CANFD_CFG_RXFIFO4_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO4_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO4_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO4_INT_MODE) | (CANFD_CFG_RXFIFO4_ENABLE)),
        ((CANFD_CFG_RXFIFO5_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO5_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO5_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO5_INT_MODE) | (CANFD_CFG_RXFIFO5_ENABLE)),
        ((CANFD_CFG_RXFIFO6_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO6_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO6_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO6_INT_MODE) | (CANFD_CFG_RXFIFO6_ENABLE)),
        ((CANFD_CFG_RXFIFO7_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO7_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO7_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO7_INT_MODE) | (CANFD_CFG_RXFIFO7_ENABLE)),
#endif
    },
    .common_fifo_config =
    {
        CANFD_CFG_COMMONFIFO0,
#if !BSP_FEATURE_CANFD_LITE
        CANFD_CFG_COMMONFIFO1,
        CANFD_CFG_COMMONFIFO2,
        CANFD_CFG_COMMONFIFO3,
        CANFD_CFG_COMMONFIFO4,
        CANFD_CFG_COMMONFIFO5,
#endif
    }
};
#undef RA_NOT_DEFINED

#endif

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
    .p_global_cfg       = &g_canfd_global_cfg,
};

canfd_instance_ctrl_t g_canfd0_ctrl;
const can_cfg_t g_canfd0_cfg =
{
    .channel                = 0,
    .p_bit_timing           = &g_canfd0_bit_timing_cfg,
    .p_callback             = canfd0_callback,
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
agt_instance_ctrl_t g_timer_agt5_ctrl;
const agt_extended_cfg_t g_timer_agt5_extend =
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
const timer_cfg_t g_timer_agt5_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.00131072 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 5,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_agt5_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_AGT5_INT)
    .cycle_end_irq       = VECTOR_NUMBER_AGT5_INT,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_agt5 =
{
    .p_ctrl        = &g_timer_agt5_ctrl,
    .p_cfg         = &g_timer_agt5_cfg,
    .p_api         = &g_timer_on_agt
};
agt_instance_ctrl_t g_timer_agt4_ctrl;
const agt_extended_cfg_t g_timer_agt4_extend =
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
const timer_cfg_t g_timer_agt4_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.00131072 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 4,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_agt4_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_AGT4_INT)
    .cycle_end_irq       = VECTOR_NUMBER_AGT4_INT,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_agt4 =
{
    .p_ctrl        = &g_timer_agt4_ctrl,
    .p_cfg         = &g_timer_agt4_cfg,
    .p_api         = &g_timer_on_agt
};
agt_instance_ctrl_t g_timer_agt3_ctrl;
const agt_extended_cfg_t g_timer_agt3_extend =
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
const timer_cfg_t g_timer_agt3_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.00131072 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 3,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_agt3_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_AGT3_INT)
    .cycle_end_irq       = VECTOR_NUMBER_AGT3_INT,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_agt3 =
{
    .p_ctrl        = &g_timer_agt3_ctrl,
    .p_cfg         = &g_timer_agt3_cfg,
    .p_api         = &g_timer_on_agt
};
agt_instance_ctrl_t g_timer_agt2_ctrl;
const agt_extended_cfg_t g_timer_agt2_extend =
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
const timer_cfg_t g_timer_agt2_cfg =
{
    .mode                = TIMER_MODE_PWM,
    /* Actual period: 0.00131072 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
    .channel             = 2,
    .p_callback          = NULL,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = &g_timer_agt2_extend,
    .cycle_end_ipl       = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_AGT2_INT)
    .cycle_end_irq       = VECTOR_NUMBER_AGT2_INT,
#else
    .cycle_end_irq       = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const timer_instance_t g_timer_agt2 =
{
    .p_ctrl        = &g_timer_agt2_ctrl,
    .p_cfg         = &g_timer_agt2_cfg,
    .p_api         = &g_timer_on_agt
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
sci_uart_instance_ctrl_t     g_uart9_ctrl;

            baud_setting_t               g_uart9_baud_setting =
            {
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
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
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
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
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
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
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
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
                .p_callback          = uart_callback,
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
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
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
                .p_callback          = uart_callback,
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
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
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
sci_uart_instance_ctrl_t     g_uart3_ctrl;

            baud_setting_t               g_uart3_baud_setting =
            {
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
            };

            /** UART extended configuration for UARTonSCI HAL driver */
            const sci_uart_extended_cfg_t g_uart3_cfg_extend =
            {
                .clock                = SCI_UART_CLOCK_INT,
                .rx_edge_start          = SCI_UART_START_BIT_FALLING_EDGE,
                .noise_cancel         = SCI_UART_NOISE_CANCELLATION_DISABLE,
                .rx_fifo_trigger        = SCI_UART_RX_FIFO_TRIGGER_MAX,
                .p_baud_setting         = &g_uart3_baud_setting,
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
    .p_api         = &g_uart_on_sci
};
sci_uart_instance_ctrl_t     g_uart2_ctrl;

            baud_setting_t               g_uart2_baud_setting =
            {
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
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
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
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
                /* Baud rate calculated with 0.014% error. */ .semr_baudrate_bits_b.abcse = 0, .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme = true
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
        /* Actual calculated bitrate: 12500000. */ .spbr = 3, .brdv = 0
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
        /* Actual calculated bitrate: 12500000. */ .spbr = 3, .brdv = 0
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
    /* Actual period: 0.00131072 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00131072 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00065536 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00065536 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00065536 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00065536 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00065536 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00065536 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00065536 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00065536 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual period: 0.00065536 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x10000, .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t)0,
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
    /* Actual calculated bitrate: 98425. Actual calculated duty cycle: 50%. */ .clock_settings.brl_value = 28, .clock_settings.brh_value = 28, .clock_settings.cks_value = 3, .clock_settings.sddl_value = 0, .clock_settings.dlcs_value = 0,
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
    /* Actual calculated bitrate: 98425. Actual calculated duty cycle: 50%. */ .clock_settings.brl_value = 28, .clock_settings.brh_value = 28, .clock_settings.cks_value = 3, .clock_settings.sddl_value = 0, .clock_settings.dlcs_value = 0,
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
    /* Actual calculated bitrate: 98425. Actual calculated duty cycle: 50%. */ .clock_settings.brl_value = 28, .clock_settings.brh_value = 28, .clock_settings.cks_value = 3, .clock_settings.sddl_value = 0, .clock_settings.dlcs_value = 0,
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
    /* Actual period: 42.94967296 seconds. Actual duty: 50%. */ .period_counts = (uint32_t) 0x100000000, .duty_cycle_counts = 0x80000000, .source_div = (timer_source_div_t)TIMER_SOURCE_DIV_8,
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

void g_hal_init(void) {
g_common_init();
}

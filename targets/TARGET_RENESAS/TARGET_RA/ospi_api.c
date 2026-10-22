/* mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * OSPI HAL implementation for the RA6M5 legacy xSPI (OSPI) peripheral.
 *
 * The EK-RA6M5 memory mapping is assumed:
 *   - the OctaFlash sits on OSPI chip select 1 (memory-mapped window at
 *     0x70000000, see BSP_FEATURE_OSPI_DEVICE_1_START_ADDRESS),
 *   - chip select 0 is unused.
 *
 * The mbed OSPI HAL traffic (OSPIFBlockDevice driving a Macronix MX25LM octal
 * flash) falls into three frame shapes:
 *
 *   - 1S-1S-1S control frames with up to 4 data bytes (SFDP probing phase,
 *     status/config reads, WREN, erases, soft reset): executed with
 *     R_OSPI_DirectTransfer, chunked 4 bytes at a time.
 *   - OPI (SOPI/DOPI) frames with a 16-bit instruction and a 4-byte address.
 *     Control frames go through R_OSPI_DirectTransfer with the two-byte
 *     command encoding of the MX25LM (cmd | ~cmd << 8, e.g. WREN 0x06F9,
 *     RDSR 0x05FA). Register-class read commands in OPI mode always use the
 *     dummy count validated with the Renesas EK-RA6M5 OSPI example (4), the
 *     frame's dummy count is only honoured on the data-read fast path.
 *   - bulk data traffic (16-bit instruction + 4-byte address + data):
 *     served from the memory-mapped window. The OPI command set in the FSP
 *     configuration is mutable; before each read the memory-mapped read
 *     command and dummy length (MDLR) are rewritten from the requested frame
 *     and re-applied with R_OSPI_SpiProtocolSet, then the data is memcpy-ed
 *     from the window. Page programs go through R_OSPI_Write, which issues
 *     the configured program command (with WREN) via single continuous write
 *     mode and is split at page boundaries here.
 *
 * Dummy cycle units: the upper layer counts dummy cycles in transfer units
 * (bits-per-line, i.e. half clocks in DTR mode), while the FSP MDLR/DCSR
 * fields validated for this flash count them differently per mode. The
 * mapping below reproduces the Renesas-validated numbers for the MX25LM in
 * its default 20-dummy-cycle configuration (CR2 DC = 000):
 *   - memory-mapped DOPI read: MDLR = frame dummy / 2 (20 -> 10)
 *   - memory-mapped SOPI read: MDLR = frame dummy      (20 -> 20)
 *   - direct-mode register reads: fixed 4 dummy clocks (datasheet note 5)
 *
 * Only SPI mode 0 is supported. The bus frequency is fixed by the BSP
 * OCTACLK configuration (see the board's bsp_clock_cfg.h) and the frequency
 * argument is only recorded.
 */

#if DEVICE_OSPI

#include "ospi_api.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "objects.h"
#include "bsp_api.h"
#include "r_ospi.h"

#include <string.h>

/* MX25LM51245G on the EK-RA6M5: 64 MB OctaFlash on OSPI chip select 1. */
#define OSPI_PRV_CHANNEL         (OSPI_DEVICE_NUMBER_1)
#define OSPI_PRV_MEM_SIZE        (0x04000000U)
#define OSPI_PRV_PAGE_SIZE       (256U)
#define OSPI_PRV_XIP_BASE        (BSP_FEATURE_OSPI_DEVICE_1_START_ADDRESS)
#define OSPI_PRV_DIRECT_MAX      (4U)    /* CWDR/CRR are 32-bit            */

/* Renesas EK-RA6M5 OSPI example timing values (validated with the MX25LM). */
static const ospi_timing_setting_t ospi_timing_settings =
{
    .command_to_command_interval = OSPI_COMMAND_INTERVAL_CLOCKS_2,
    .cs_pullup_lag               = OSPI_COMMAND_CS_PULLUP_CLOCKS_7,
    .cs_pulldown_lead            = OSPI_COMMAND_CS_PULLDOWN_CLOCKS_3,
};
static const ospi_timing_setting_t ospi_write_settings =
{
    .command_to_command_interval = OSPI_COMMAND_INTERVAL_CLOCKS_2,
    .cs_pullup_lag               = OSPI_COMMAND_CS_PULLUP_CLOCKS_2,
    .cs_pulldown_lead            = OSPI_COMMAND_CS_PULLDOWN_CLOCKS_3,
};
static const ospi_timing_setting_t ospi_read_settings =
{
    .command_to_command_interval = OSPI_COMMAND_INTERVAL_CLOCKS_2,
    .cs_pullup_lag               = OSPI_COMMAND_CS_PULLUP_CLOCKS_7,
    .cs_pulldown_lead            = OSPI_COMMAND_CS_PULLDOWN_CLOCKS_3,
};

/* Mutable OPI command set. The memory-mapped read command (SOPI: read_command,
 * DOPI: dual_read_command) is rewritten before every window read so the
 * auto-issued transaction matches the frame the upper layer asked for. The
 * instruction expansion (value | ~value << 8) yields the MX25LM two-byte
 * command encoding of every one-byte opcode the OSPIF driver uses. */
static ospi_opi_command_set_t ospi_opi_commands =
{
    .read_command         = 0xEC13,   /* 8S-8S-8S fast read (SOPI)   */
    .dual_read_command    = 0xEE11,   /* 8D-8D-8D fast read (DOPI)   */
    .page_program_command = 0x12ED,
    .write_enable_command = 0x06F9,
    .status_command       = 0x05FA,
    .command_bytes        = 2,
};

static const spi_flash_erase_command_t ospi_erase_commands[] =
{
    { .command = 0x21DE, .size = 4096 },
    { .command = 0xDC23, .size = 65536 },
    { .command = 0xC738, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE },
};

static ospi_extended_cfg_t ospi_extended_cfg =
{
    .channel                                 = OSPI_PRV_CHANNEL,
    .memory_size                             = OSPI_PRV_MEM_SIZE,
    .memory_type                             = OSPI_DEVICE_FLASH,
    .data_latch_delay_clocks                 = 0x80,
    .om_dqs_enable_counter_dopi              = 6,
    .om_dqs_enable_counter_sopi              = 9,
    .single_continuous_mode_read_idle_time   = 100,
    .single_continuous_mode_write_idle_time  = 100,
    .p_mem_mapped_read_timing_settings       = &ospi_read_settings,
    .p_mem_mapped_write_timing_settings      = &ospi_write_settings,
    .p_timing_settings                       = &ospi_timing_settings,
    .p_opi_commands                          = &ospi_opi_commands,
    .opi_mem_read_dummy_cycles               = 10,    /* DOPI: 20/2 */
    .opi_mem_write_dummy_cycles              = 0,
    .opi_status_read_dummy_cycles            = 4,
    .p_autocalibration_preamble_pattern_addr = (uint8_t *)0,
    .dopi_byte_order                         = OSPI_DOPI_BYTE_ORDER_0123,
    .ram_chip_select_max_period_setting      = 0,
};

static spi_flash_cfg_t ospi_flash_cfg =
{
    .spi_protocol                = SPI_FLASH_PROTOCOL_EXTENDED_SPI,
    .read_mode                   = SPI_FLASH_READ_MODE_STANDARD,
    .address_bytes               = SPI_FLASH_ADDRESS_BYTES_4,
    .dummy_clocks                = SPI_FLASH_DUMMY_CLOCKS_DEFAULT,
    .page_program_address_lines  = (spi_flash_data_lines_t)0U,
    .write_status_bit            = 0,
    .write_enable_bit            = 1,
    .page_size_bytes             = OSPI_PRV_PAGE_SIZE,
    .page_program_command        = 0x12,   /* SPI-mode 4-byte page program */
    .write_enable_command        = 0x06,
    .status_command              = 0x05,
    .read_command                = 0x13,   /* SPI-mode 4-byte read        */
    .xip_enter_command           = 0U,
    .xip_exit_command            = 0U,
    .erase_command_list_length   = sizeof(ospi_erase_commands) / sizeof(ospi_erase_commands[0]),
    .p_erase_command_list        = ospi_erase_commands,
    .p_extend                    = &ospi_extended_cfg,
};

static ospi_instance_ctrl_t ospi_ctrl;
static bool ospi_open;
static uint32_t ospi_users;   /* active ospi_t objects sharing the controller */

/* ---------------------------------------------------------------- helpers */

/* MX25LM two-byte command encoding for one-byte opcodes; values already
 * carrying a second byte pass through unchanged. */
static uint16_t ospi_prv_expand(uint32_t instruction)
{
    if (instruction > 0xFFU) {
        return (uint16_t)instruction;
    }
    return (uint16_t)((instruction << 8) | ((~instruction) & 0xFFU));
}

/* Map the per-phase bus widths of a frame onto an FSP protocol. The OSPIF
 * driver only issues all-single or all-octal frames (STR or DTR). */
static ospi_status_t ospi_prv_map_protocol(const ospi_command_t *command, spi_flash_protocol_t *protocol)
{
    const ospi_bus_width_t inst  = command->instruction.disabled ?
                                   OSPI_CFG_BUS_SINGLE : command->instruction.bus_width;
    const ospi_bus_width_t addr  = command->address.disabled ?
                                   OSPI_CFG_BUS_SINGLE : command->address.bus_width;
    const ospi_bus_width_t data  = command->data.bus_width;

    if ((inst != OSPI_CFG_BUS_SINGLE) && (inst != OSPI_CFG_BUS_OCTA) && (inst != OSPI_CFG_BUS_OCTA_DTR)) {
        return OSPI_STATUS_INVALID_PARAMETER;
    }
    if ((addr != OSPI_CFG_BUS_SINGLE) && (addr != inst) && (addr != data)) {
        return OSPI_STATUS_INVALID_PARAMETER;
    }

    if ((inst == OSPI_CFG_BUS_OCTA_DTR) || (data == OSPI_CFG_BUS_OCTA_DTR)) {
        *protocol = SPI_FLASH_PROTOCOL_DOPI;
        return OSPI_STATUS_OK;
    }
    if ((inst == OSPI_CFG_BUS_OCTA) || (data == OSPI_CFG_BUS_OCTA)) {
        *protocol = SPI_FLASH_PROTOCOL_SOPI;
        return OSPI_STATUS_OK;
    }
    if ((inst == OSPI_CFG_BUS_SINGLE) && (addr == OSPI_CFG_BUS_SINGLE) && (data == OSPI_CFG_BUS_SINGLE)) {
        *protocol = SPI_FLASH_PROTOCOL_EXTENDED_SPI;
        return OSPI_STATUS_OK;
    }
    return OSPI_STATUS_INVALID_PARAMETER;
}

/* Frame of an OPI (SOPI/DOPI) protocol: 16-bit instruction encoding. */
static bool ospi_prv_is_opi(spi_flash_protocol_t protocol)
{
    return (protocol == SPI_FLASH_PROTOCOL_SOPI) || (protocol == SPI_FLASH_PROTOCOL_DOPI);
}

/* The OM_SCLK limit depends on the protocol: 50 MHz in extended SPI mode,
 * 100 MHz in SOPI/DOPI (see the Renesas EK-RA6M5 OSPI example). OM_SCLK is
 * OCTACLK/2, so the OCTACLK divider is switched together with the protocol:
 * the BSP default from bsp_clock_cfg.h (safe for SPI mode) for EXTENDED_SPI,
 * and the fast setting for OPI modes. */
static void ospi_prv_update_octaclk(spi_flash_protocol_t protocol)
{
    bsp_octaclk_settings_t octaclk = {
        .source_clock = (bsp_clocks_source_t)BSP_CFG_OCTACLK_SOURCE,
        .divider      = ospi_prv_is_opi(protocol) ? BSP_CLOCKS_OCTA_CLOCK_DIV_1 :
                        (bsp_clocks_octaclk_div_t)BSP_CFG_OCTACLK_DIV,
    };
    R_BSP_OctaclkUpdate(&octaclk);
}

static ospi_status_t ospi_prv_set_protocol(spi_flash_protocol_t protocol)
{
    ospi_prv_update_octaclk(protocol);
    if (FSP_SUCCESS != R_OSPI_SpiProtocolSet(&ospi_ctrl, protocol)) {
        return OSPI_STATUS_ERROR;
    }
    return OSPI_STATUS_OK;
}

/* Instruction and its byte length for a frame in the given protocol. Note
 * that the mbed OSPI driver class never sets command->instruction.size (it
 * stays OSPI_CFG_INST_SIZE_8 after the memset in _build_ospi_command), so
 * the protocol - not the size field - decides the command length. */
static uint32_t ospi_prv_frame_command(spi_flash_protocol_t protocol, uint32_t instruction, uint8_t *length)
{
    if (ospi_prv_is_opi(protocol)) {
        *length = 2U;
        return ospi_prv_expand(instruction);
    }
    *length = 1U;
    return instruction;
}

/* ospi_address_size_t is an enum (index), not a bit count. */
static uint8_t ospi_prv_addr_bytes(ospi_address_size_t size)
{
    switch (size) {
        case OSPI_CFG_ADDR_SIZE_8:
            return 1;
        case OSPI_CFG_ADDR_SIZE_16:
            return 2;
        case OSPI_CFG_ADDR_SIZE_24:
            return 3;
        default:
            return 4;
    }
}

/* One manual-command transaction with up to 4 data bytes (little-endian in
 * CWDR/CRR: the first byte on the wire occupies the least significant byte). */
static ospi_status_t ospi_prv_direct(spi_flash_protocol_t protocol, uint32_t command, uint8_t command_length,
                                     uint32_t address, uint8_t address_length, uint64_t *data,
                                     uint8_t data_length, uint8_t dummy_cycles,
                                     spi_flash_direct_transfer_dir_t dir)
{
    spi_flash_direct_transfer_t xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.command        = command;
    xfer.command_length = command_length;
    xfer.address        = address;
    xfer.address_length = address_length;
    xfer.data_length    = data_length;
    xfer.dummy_cycles   = dummy_cycles;
    if ((NULL != data) && (SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE == dir)) {
        xfer.data_u64 = *data;
    }

    /* The direct transfer inherits the protocol currently selected in the
     * control block; make sure it matches the frame. */
    if (ospi_ctrl.spi_protocol != protocol) {
        ospi_status_t status = ospi_prv_set_protocol(protocol);
        if (status != OSPI_STATUS_OK) {
            return status;
        }
    }

    if (FSP_SUCCESS != R_OSPI_DirectTransfer(&ospi_ctrl, &xfer, dir)) {
        return OSPI_STATUS_ERROR;
    }

    if ((NULL != data) && (SPI_FLASH_DIRECT_TRANSFER_DIR_READ == dir)) {
        *data = xfer.data_u64;
    }
    return OSPI_STATUS_OK;
}

/* Dummy cycles for direct-mode OPI register-class reads: the datasheet fixes
 * these at 4 clocks (status/configuration registers, both STR and DTR),
 * independent of the (transfer-unit based) frame count. */
#define OSPI_PRV_OPI_REG_DUMMY  (4U)

static uint8_t ospi_prv_direct_dummy(spi_flash_protocol_t protocol, uint8_t frame_dummy)
{
    if ((protocol == SPI_FLASH_PROTOCOL_SOPI) || (protocol == SPI_FLASH_PROTOCOL_DOPI)) {
        return (frame_dummy > 0U) ? OSPI_PRV_OPI_REG_DUMMY : 0U;
    }
    return frame_dummy;
}

static uint64_t ospi_prv_pack(const uint8_t *src, size_t length)
{
    uint64_t value = 0;
    for (size_t i = 0; i < length; i++) {
        value |= ((uint64_t)src[i]) << (8 * i);
    }
    return value;
}

static void ospi_prv_unpack(uint64_t value, uint8_t *dst, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        dst[i] = (uint8_t)(value >> (8 * i));
    }
}

/* Read the JEDEC ID over a manual 1S command. Returns the manufacturer byte
 * (0 on a short/failed read). */
static uint8_t ospi_prv_read_jedec(spi_flash_protocol_t protocol)
{
    uint64_t data = 0;
    if (OSPI_STATUS_OK != ospi_prv_direct(protocol,
                                          (protocol == SPI_FLASH_PROTOCOL_EXTENDED_SPI) ? 0x9F : ospi_prv_expand(0x9F),
                                          (protocol == SPI_FLASH_PROTOCOL_EXTENDED_SPI) ? 1U : 2U,
                                          0, 0, &data, 3, 0,
                                          SPI_FLASH_DIRECT_TRANSFER_DIR_READ)) {
        return 0;
    }
    return (uint8_t)(data & 0xFFU);
}

/* Deterministic flash state: RSTEN (0x66) + RST (0x99) in the given protocol.
 * The two-byte encodings of 0x66/0x99 (0x6699/0x9966) are the MX25LM OPI
 * reset commands, so the same code covers SPI, SOPI and DOPI. */
static void ospi_prv_soft_reset(spi_flash_protocol_t protocol)
{
    uint8_t cmd_len = (protocol == SPI_FLASH_PROTOCOL_EXTENDED_SPI) ? 1U : 2U;

    (void)ospi_prv_direct(protocol, ospi_prv_expand(0x66), cmd_len, 0, 0, NULL, 0, 0,
                          SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
    (void)ospi_prv_direct(protocol, ospi_prv_expand(0x99), cmd_len, 0, 0, NULL, 0, 0,
                          SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
    /* tRU (reset time) is 10-30 us; round up generously. */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
}

/* ------------------------------------------------------------------- init */

#if STATIC_PINMAP_READY
#define OSPI_INIT_DIRECT ospi_init_direct
ospi_status_t ospi_init_direct(ospi_t *obj, const ospi_pinmap_t *pinmap, uint32_t hz, uint8_t mode)
#else
#define OSPI_INIT_DIRECT _ospi_init_direct
static ospi_status_t _ospi_init_direct(ospi_t *obj, const ospi_pinmap_t *pinmap, uint32_t hz, uint8_t mode)
#endif
{
    if (mode != 0) {
        /* The OSPI controller only supports SPI mode 0. */
        return OSPI_STATUS_INVALID_PARAMETER;
    }

    /* Configure the OSPI pins (no pin_mode() here - on RA each pin_function
     * call rewrites the whole PFS register). */
    obj->io0 = pinmap->data0_pin;
    pin_function(pinmap->data0_pin, pinmap->data0_function);
    obj->io1 = pinmap->data1_pin;
    pin_function(pinmap->data1_pin, pinmap->data1_function);
    obj->io2 = pinmap->data2_pin;
    pin_function(pinmap->data2_pin, pinmap->data2_function);
    obj->io3 = pinmap->data3_pin;
    pin_function(pinmap->data3_pin, pinmap->data3_function);
    obj->io4 = pinmap->data4_pin;
    pin_function(pinmap->data4_pin, pinmap->data4_function);
    obj->io5 = pinmap->data5_pin;
    pin_function(pinmap->data5_pin, pinmap->data5_function);
    obj->io6 = pinmap->data6_pin;
    pin_function(pinmap->data6_pin, pinmap->data6_function);
    obj->io7 = pinmap->data7_pin;
    pin_function(pinmap->data7_pin, pinmap->data7_function);
    obj->sclk = pinmap->sclk_pin;
    pin_function(pinmap->sclk_pin, pinmap->sclk_function);
    obj->ssel = pinmap->ssel_pin;
    pin_function(pinmap->ssel_pin, pinmap->ssel_function);
    obj->dqs = pinmap->dqs_pin;
    pin_function(pinmap->dqs_pin, pinmap->dqs_function);

    obj->hz = (int)hz;

    /* The controller is shared by every ospi_t object (single flash device):
     * open it with the first user only. */
    ospi_users++;
    if (ospi_open) {
        return OSPI_STATUS_OK;
    }

    /* Restore the default (SPI-mode) command set the init flow expects. */
    ospi_opi_commands.read_command         = 0xEC13;
    ospi_opi_commands.dual_read_command    = 0xEE11;
    ospi_opi_commands.page_program_command = 0x12ED;
    ospi_extended_cfg.opi_mem_read_dummy_cycles = 10;

    ospi_flash_cfg.spi_protocol = SPI_FLASH_PROTOCOL_EXTENDED_SPI;
    fsp_err_t err = R_OSPI_Open(&ospi_ctrl, &ospi_flash_cfg);
    if (FSP_SUCCESS != err && FSP_ERR_ALREADY_OPEN != err) {
        ospi_users--;
        return OSPI_STATUS_ERROR;
    }
    ospi_open = true;

    /* Deterministic device state. After a power cycle the MX25LM boots in
     * extended SPI mode; after a warm MCU reset it may still be in OPI mode
     * from a previous run, in which case the SPI-mode reset is not decoded -
     * fall back to a DOPI-mode reset and retry. */
    ospi_prv_soft_reset(SPI_FLASH_PROTOCOL_EXTENDED_SPI);
    uint8_t jedec = ospi_prv_read_jedec(SPI_FLASH_PROTOCOL_EXTENDED_SPI);
    if (jedec != 0xC2U) {
        ospi_prv_soft_reset(SPI_FLASH_PROTOCOL_DOPI);
        (void)ospi_prv_set_protocol(SPI_FLASH_PROTOCOL_EXTENDED_SPI);
        jedec = ospi_prv_read_jedec(SPI_FLASH_PROTOCOL_EXTENDED_SPI);
        if (jedec != 0xC2U) {
            /* Not the expected Macronix octal flash; leave the controller
             * open anyway - the upper layer reports the failure. */
            return OSPI_STATUS_ERROR;
        }
    }

    return OSPI_STATUS_OK;
}

ospi_status_t ospi_free(ospi_t *obj)
{
    /* Only the last user tears the controller down. */
    if (ospi_users > 0) {
        ospi_users--;
    }

    if ((0 == ospi_users) && ospi_open) {
        /* Best-effort return to SPI mode before closing, so a warm reboot
         * starts from the same state as a power cycle. */
        if (ospi_ctrl.spi_protocol != SPI_FLASH_PROTOCOL_EXTENDED_SPI) {
            ospi_prv_soft_reset(ospi_ctrl.spi_protocol);
            (void)ospi_prv_set_protocol(SPI_FLASH_PROTOCOL_EXTENDED_SPI);
        }

        R_OSPI_Close(&ospi_ctrl);
        ospi_open = false;

        /* Release the pins (function 0 = analog/reset state). */
        pin_function(obj->io0, 0);
        pin_function(obj->io1, 0);
        pin_function(obj->io2, 0);
        pin_function(obj->io3, 0);
        pin_function(obj->io4, 0);
        pin_function(obj->io5, 0);
        pin_function(obj->io6, 0);
        pin_function(obj->io7, 0);
        pin_function(obj->sclk, 0);
        pin_function(obj->ssel, 0);
        pin_function(obj->dqs, 0);
    }

    return OSPI_STATUS_OK;
}

ospi_status_t ospi_frequency(ospi_t *obj, int hz)
{
    if (hz <= 0) {
        return OSPI_STATUS_INVALID_PARAMETER;
    }

    /* The OSPI clock is fixed by the BSP OCTACLK configuration; only record
     * the request. */
    obj->hz = hz;
    return OSPI_STATUS_OK;
}

/* ------------------------------------------------------------------ write */

ospi_status_t ospi_write(ospi_t *obj, const ospi_command_t *command, const void *data, size_t *length)
{
    (void)obj;

    if ((data == NULL) || (length == NULL) || (*length == 0)) {
        return OSPI_STATUS_INVALID_PARAMETER;
    }

    spi_flash_protocol_t protocol;
    ospi_status_t status = ospi_prv_map_protocol(command, &protocol);
    if (status != OSPI_STATUS_OK) {
        return status;
    }

    uint32_t addr = command->address.disabled ? 0 : command->address.value;
    uint8_t  addr_len = command->address.disabled ? 0 : ospi_prv_addr_bytes(command->address.size);

    /* Page program through the XIP window in OPI mode with the native
     * (4-byte) addressing the single-continuous-write engine expects:
     * R_OSPI_Write issues WREN plus the configured program command. */
    if (ospi_prv_is_opi(protocol) &&
            !command->instruction.disabled &&
            !command->address.disabled &&
            (command->address.size == OSPI_CFG_ADDR_SIZE_32)) {
        if (addr & 1U) {
            /* DTR mode requires an even start address (MX25LM datasheet). */
            return OSPI_STATUS_INVALID_PARAMETER;
        }

        ospi_opi_commands.page_program_command = ospi_prv_expand(command->instruction.value);
        status = ospi_prv_set_protocol(protocol);
        if (status != OSPI_STATUS_OK) {
            return status;
        }

        uint32_t written = 0;
        while (written < *length) {
            uint32_t chunk = OSPI_PRV_PAGE_SIZE - ((addr + written) % OSPI_PRV_PAGE_SIZE);
            if (chunk > (*length - written)) {
                chunk = (uint32_t)(*length - written);
            }
            if (FSP_SUCCESS != R_OSPI_Write(&ospi_ctrl,
                                            (const uint8_t *)data + written,
                                            (uint8_t *)(OSPI_PRV_XIP_BASE + addr + written),
                                            chunk)) {
                return OSPI_STATUS_ERROR;
            }
            written += chunk;
        }
        return OSPI_STATUS_OK;
    }

    /* Everything else (SFDP-era writes, register writes): chunked manual
     * commands, 4 data bytes per transaction. */
    uint8_t cmd_len;
    uint32_t cmd = ospi_prv_frame_command(protocol, command->instruction.value, &cmd_len);
    uint8_t dummy = ospi_prv_direct_dummy(protocol, command->dummy_count);

    size_t done = 0;
    while (done < *length) {
        size_t chunk = *length - done;
        if (chunk > OSPI_PRV_DIRECT_MAX) {
            chunk = OSPI_PRV_DIRECT_MAX;
        }
        uint64_t value = ospi_prv_pack((const uint8_t *)data + done, chunk);
        status = ospi_prv_direct(protocol, cmd, cmd_len, addr + done, addr_len, &value,
                                 (uint8_t)chunk, dummy,
                                 SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
        if (status != OSPI_STATUS_OK) {
            return status;
        }
        done += chunk;
    }
    return OSPI_STATUS_OK;
}

/* ------------------------------------------------------------------- read */

ospi_status_t ospi_read(ospi_t *obj, const ospi_command_t *command, void *data, size_t *length)
{
    (void)obj;

    if ((data == NULL) || (length == NULL) || (*length == 0)) {
        return OSPI_STATUS_INVALID_PARAMETER;
    }

    spi_flash_protocol_t protocol;
    ospi_status_t status = ospi_prv_map_protocol(command, &protocol);
    if (status != OSPI_STATUS_OK) {
        return status;
    }

    uint32_t addr = command->address.disabled ? 0 : command->address.value;
    uint8_t  addr_len = command->address.disabled ? 0 : ospi_prv_addr_bytes(command->address.size);

    /* Fast path: memory-mapped window read in OPI mode with the native
     * (4-byte) addressing. Rewrite the auto-issued read command and dummy
     * length so the window transaction matches the requested frame, then
     * memcpy from the window. */
    if (ospi_prv_is_opi(protocol) &&
            !command->instruction.disabled &&
            !command->address.disabled &&
            (command->address.size == OSPI_CFG_ADDR_SIZE_32)) {
        if ((addr & 1U) || (*length & 1U)) {
            /* DTR mode needs an even address and an even length. An odd
             * length with an even address is handled by window-reading the
             * even part and fetching the last byte through the manual path;
             * an odd address always falls back to the manual path. */
            if (addr & 1U) {
                return OSPI_STATUS_INVALID_PARAMETER;
            }
            if (*length < 2U) {
                return OSPI_STATUS_INVALID_PARAMETER;
            }
            size_t even = *length - 1U;
            status = ospi_read(obj, command, data, &even);
            if (status != OSPI_STATUS_OK) {
                return status;
            }
            uint8_t cmd_len2;
            uint32_t cmd2 = ospi_prv_frame_command(protocol, command->instruction.value, &cmd_len2);
            uint64_t value = 0;
            return ospi_prv_direct(protocol, cmd2, cmd_len2, addr + even,
                                   ospi_prv_addr_bytes(command->address.size), &value, 1,
                                   ospi_prv_direct_dummy(protocol, command->dummy_count),
                                   SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
        }

        if (protocol == SPI_FLASH_PROTOCOL_DOPI) {
            ospi_opi_commands.dual_read_command = ospi_prv_expand(command->instruction.value);
            ospi_extended_cfg.opi_mem_read_dummy_cycles = command->dummy_count / 2U;
        } else {
            ospi_opi_commands.read_command = ospi_prv_expand(command->instruction.value);
            ospi_extended_cfg.opi_mem_read_dummy_cycles = command->dummy_count;
        }
        if (ospi_extended_cfg.opi_mem_read_dummy_cycles == 0U) {
            /* A zero MDLR is not a valid dummy setting for OPI reads. */
            ospi_extended_cfg.opi_mem_read_dummy_cycles = (protocol == SPI_FLASH_PROTOCOL_DOPI) ? 10U : 20U;
        }

        status = ospi_prv_set_protocol(protocol);
        if (status != OSPI_STATUS_OK) {
            return status;
        }

        /* Memory-mapped DOPI/SOPI reads require single continuous read mode
         * on this controller (DRCSTR.CTRx); without it every window access
         * aborts with a bus error. Continuous read is only enabled around
         * the memcpy so direct commands keep working. */
        if (FSP_SUCCESS != R_OSPI_XipEnter(&ospi_ctrl)) {
            return OSPI_STATUS_ERROR;
        }

        memcpy(data, (const void *)(OSPI_PRV_XIP_BASE + addr), *length);

        R_OSPI_XipExit(&ospi_ctrl);
        return OSPI_STATUS_OK;
    }

    /* Manual path: SFDP reads (0x5A) and other addressed reads, chunked 4
     * bytes per transaction. */
    uint8_t cmd_len;
    uint32_t cmd = ospi_prv_frame_command(protocol, command->instruction.value, &cmd_len);
    uint8_t dummy = ospi_prv_direct_dummy(protocol, command->dummy_count);

    size_t done = 0;
    while (done < *length) {
        size_t chunk = *length - done;
        if (chunk > OSPI_PRV_DIRECT_MAX) {
            chunk = OSPI_PRV_DIRECT_MAX;
        }
        uint64_t value = 0;
        status = ospi_prv_direct(protocol, cmd, cmd_len, addr + done, addr_len, &value,
                                 (uint8_t)chunk, dummy,
                                 SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
        if (status != OSPI_STATUS_OK) {
            return status;
        }
        ospi_prv_unpack(value, (uint8_t *)data + done, chunk);
        done += chunk;
    }
    return OSPI_STATUS_OK;
}

/* -------------------------------------------------------- command transfer */

ospi_status_t ospi_command_transfer(ospi_t *obj, const ospi_command_t *command,
                                    const void *tx_data, size_t tx_size,
                                    void *rx_data, size_t rx_size)
{
    (void)obj;

    if (command->instruction.disabled) {
        return OSPI_STATUS_INVALID_PARAMETER;
    }
    if ((tx_size > OSPI_PRV_DIRECT_MAX) || (rx_size > OSPI_PRV_DIRECT_MAX)) {
        return OSPI_STATUS_INVALID_PARAMETER;
    }

    spi_flash_protocol_t protocol;
    ospi_status_t status = ospi_prv_map_protocol(command, &protocol);
    if (status != OSPI_STATUS_OK) {
        return status;
    }

    uint8_t  cmd_len;
    uint32_t cmd = ospi_prv_frame_command(protocol, command->instruction.value, &cmd_len);
    uint32_t addr = command->address.disabled ? 0 : command->address.value;
    uint8_t  addr_len = command->address.disabled ? 0 : ospi_prv_addr_bytes(command->address.size);
    uint8_t  dummy = ospi_prv_direct_dummy(protocol, command->dummy_count);

    /* Transmit first, then receive with the same command (two bus cycles),
     * matching the other implementations. Command-only frames (WREN, WRDI,
     * erase, reset sequences...) carry no data and must still be clocked
     * out. */
    if ((tx_data != NULL) && (tx_size > 0)) {
        uint64_t value = ospi_prv_pack((const uint8_t *)tx_data, tx_size);
        status = ospi_prv_direct(protocol, cmd, cmd_len, addr, addr_len, &value,
                                 (uint8_t)tx_size, dummy,
                                 SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
        if (status != OSPI_STATUS_OK) {
            return status;
        }
    } else if ((rx_data != NULL) && (rx_size > 0)) {
        uint64_t value = 0;
        status = ospi_prv_direct(protocol, cmd, cmd_len, addr, addr_len, &value,
                                 (uint8_t)rx_size, dummy,
                                 SPI_FLASH_DIRECT_TRANSFER_DIR_READ);
        if (status != OSPI_STATUS_OK) {
            return status;
        }
        ospi_prv_unpack(value, (uint8_t *)rx_data, rx_size);
    } else {
        /* Command-only transaction. */
        status = ospi_prv_direct(protocol, cmd, cmd_len, addr, addr_len, NULL,
                                 0, dummy,
                                 SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
        if (status != OSPI_STATUS_OK) {
            return status;
        }
    }

    return OSPI_STATUS_OK;
}

/* ------------------------------------------------------ pinmap-based init */

ospi_status_t ospi_init(ospi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3,
                        PinName io4, PinName io5, PinName io6, PinName io7,
                        PinName sclk, PinName ssel, PinName dqs, uint32_t hz, uint8_t mode)
{
    OSPIName ospi_data_first = (OSPIName)pinmap_peripheral(io0, PinMap_OSPI_DATA0);
    OSPIName ospi_data_second = (OSPIName)pinmap_peripheral(io4, PinMap_OSPI_DATA4);
    OSPIName ospi_data_third = (OSPIName)pinmap_peripheral(sclk, PinMap_OSPI_SCLK);
    OSPIName ospi_data_fourth = (OSPIName)pinmap_peripheral(ssel, PinMap_OSPI_SSEL);
    OSPIName ospi_data_fifth = (OSPIName)pinmap_peripheral(dqs, PinMap_OSPI_DQS);

    if (ospi_data_first != ospi_data_second || ospi_data_second != ospi_data_third ||
            ospi_data_third != ospi_data_fourth || ospi_data_fourth != ospi_data_fifth) {
        return OSPI_STATUS_INVALID_PARAMETER;
    }

    int peripheral = (int)ospi_data_first;
    int function_io0 = (int)pinmap_find_function(io0, PinMap_OSPI_DATA0);
    int function_io1 = (int)pinmap_find_function(io1, PinMap_OSPI_DATA1);
    int function_io2 = (int)pinmap_find_function(io2, PinMap_OSPI_DATA2);
    int function_io3 = (int)pinmap_find_function(io3, PinMap_OSPI_DATA3);
    int function_io4 = (int)pinmap_find_function(io4, PinMap_OSPI_DATA4);
    int function_io5 = (int)pinmap_find_function(io5, PinMap_OSPI_DATA5);
    int function_io6 = (int)pinmap_find_function(io6, PinMap_OSPI_DATA6);
    int function_io7 = (int)pinmap_find_function(io7, PinMap_OSPI_DATA7);
    int function_sclk = (int)pinmap_find_function(sclk, PinMap_OSPI_SCLK);
    int function_ssel = (int)pinmap_find_function(ssel, PinMap_OSPI_SSEL);
    int function_dqs = (int)pinmap_find_function(dqs, PinMap_OSPI_DQS);

    const ospi_pinmap_t static_pinmap = {
        peripheral,
        io0, function_io0, io1, function_io1, io2, function_io2, io3, function_io3,
        io4, function_io4, io5, function_io5, io6, function_io6, io7, function_io7,
        sclk, function_sclk, ssel, function_ssel, dqs, function_dqs
    };

    return OSPI_INIT_DIRECT(obj, &static_pinmap, hz, mode);
}

const PinMap *ospi_master_sclk_pinmap(void)
{
    return PinMap_OSPI_SCLK;
}

const PinMap *ospi_master_ssel_pinmap(void)
{
    return PinMap_OSPI_SSEL;
}

const PinMap *ospi_master_dqs_pinmap(void)
{
    return PinMap_OSPI_DQS;
}

const PinMap *ospi_master_data0_pinmap(void)
{
    return PinMap_OSPI_DATA0;
}

const PinMap *ospi_master_data1_pinmap(void)
{
    return PinMap_OSPI_DATA1;
}

const PinMap *ospi_master_data2_pinmap(void)
{
    return PinMap_OSPI_DATA2;
}

const PinMap *ospi_master_data3_pinmap(void)
{
    return PinMap_OSPI_DATA3;
}

const PinMap *ospi_master_data4_pinmap(void)
{
    return PinMap_OSPI_DATA4;
}

const PinMap *ospi_master_data5_pinmap(void)
{
    return PinMap_OSPI_DATA5;
}

const PinMap *ospi_master_data6_pinmap(void)
{
    return PinMap_OSPI_DATA6;
}

const PinMap *ospi_master_data7_pinmap(void)
{
    return PinMap_OSPI_DATA7;
}

#endif

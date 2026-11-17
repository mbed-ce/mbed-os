/*
 * mbed Microcontroller Library
 * Copyright (c) 2026 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Linker script for the Cortex-M33 (CPU1) image of the Renesas RA8P1
 * (R7KA8P1KF). Derived from RA8P1.ld; see that file for the general layout
 * comments. The differences are:
 *
 *  - FLASH/RAM come from the CPU1 partitions of the shared memories
 *    (memory_bank_config of the MCU_RA8P1_CM33 target): Code MRAM
 *    [0x020C0000, 0x02100000) and SRAM [0x22194000, 0x221D4000).
 *    0x020C0000 is the value CPU0 writes into CPSCU.CPU1INITVTOR when it
 *    releases CPU1 from reset, so the Cortex vector table (.fixed_vectors)
 *    must be the first section of the image.
 *  - The ITCM/DTCM MEMORY regions are mapped onto the Cortex-M33 local TCMs
 *    CTCM (0x00000000, 64 KB) and STCM (0x20000000, 64 KB). The region and
 *    section names of the CPU0 script are kept so that the FSP init tables
 *    in bsp_linker.c (__itcm_zero$$ / __dtcm_zero$$ ranges processed by
 *    SystemRuntimeInit on both cores) stay valid. The relocatable vector
 *    table therefore lives in the STCM.
 *  - bsp_linker.c emits no option-setting bytes for CPU1 builds (CPU0 owns
 *    them), so all OPTION_SETTING sections remain empty.
 *
 * The shared SRAM slice [0x22174000, 0x22194000) used for rpmsg-lite vrings
 * is deliberately NOT covered by any MEMORY region: it is accessed at
 * runtime through pointers (see BSP_PARTITION_SHARED_MEM_START/SIZE in
 * bsp_dualcore_partition.h).
 */

/* Space reserved for the relocatable vector table.
 * All RA MCUs have 16 system exceptions + 96 device IRQs = 112 vectors. */
__vector_table_size__ = 112 * 4;

M_CRASH_DATA_RAM_SIZE = 0x100;

MEMORY
{
    FLASH (rx)  : ORIGIN = MBED_CONFIGURED_ROM_BANK_Flash_START, LENGTH = MBED_CONFIGURED_ROM_BANK_Flash_SIZE
    RAM   (rwx) : ORIGIN = MBED_CONFIGURED_RAM_BANK_SRAM_START,  LENGTH = MBED_CONFIGURED_RAM_BANK_SRAM_SIZE
    /* ITCM/DTCM region names map onto the Cortex-M33 local TCMs (CTCM/STCM). */
    ITCM  (rwx) : ORIGIN = MBED_CONFIGURED_RAM_BANK_CTCM_START,  LENGTH = MBED_CONFIGURED_RAM_BANK_CTCM_SIZE
    DTCM  (rwx) : ORIGIN = MBED_CONFIGURED_RAM_BANK_STCM_START,  LENGTH = MBED_CONFIGURED_RAM_BANK_STCM_SIZE

    /* Option setting bytes (see bsp_linker.c; empty for CPU1 builds). */
    OPTION_SETTING_OFS0        (r) : ORIGIN = 0x02c9f040, LENGTH = 0x4
    OPTION_SETTING_OFS2        (r) : ORIGIN = 0x02c9f044, LENGTH = 0x4
    OPTION_SETTING_SAS         (r) : ORIGIN = 0x02c9f074, LENGTH = 0x4
    OPTION_SETTING_OFS1        (r) : ORIGIN = 0x12c9f4c0, LENGTH = 0x4
    OPTION_SETTING_OFS1_SEC    (r) : ORIGIN = 0x02c9f0c0, LENGTH = 0x4
    OPTION_SETTING_OFS1_SEL    (r) : ORIGIN = 0x02c9f120, LENGTH = 0x4
    OPTION_SETTING_OFS3        (r) : ORIGIN = 0x12c9f4c4, LENGTH = 0x4
    OPTION_SETTING_OFS3_SEC    (r) : ORIGIN = 0x02c9f0c4, LENGTH = 0x4
    OPTION_SETTING_OFS3_SEL    (r) : ORIGIN = 0x02c9f124, LENGTH = 0x4
    OPTION_SETTING_BPS         (r) : ORIGIN = 0x12c9f600, LENGTH = 0x80
    OPTION_SETTING_BPS_SEC     (r) : ORIGIN = 0x02c9f200, LENGTH = 0x80
    OPTION_SETTING_OTP_FSBLCTRL0 (r) : ORIGIN = 0x02e07600, LENGTH = 0x4
    OPTION_SETTING_OTP_FSBLCTRL1 (r) : ORIGIN = 0x02e07604, LENGTH = 0x4
    OPTION_SETTING_OTP_FSBLCTRL2 (r) : ORIGIN = 0x02e07608, LENGTH = 0x4
    OPTION_SETTING_OTP_SAMR    (r) : ORIGIN = 0x02e07614, LENGTH = 0x4
    OPTION_SETTING_OTP_SACC00  (r) : ORIGIN = 0x02e07620, LENGTH = 0x4
    OPTION_SETTING_OTP_SACC10  (r) : ORIGIN = 0x02e07630, LENGTH = 0x4
    OPTION_SETTING_OTP_SACC01  (r) : ORIGIN = 0x02e07640, LENGTH = 0x4
    OPTION_SETTING_OTP_SACC11  (r) : ORIGIN = 0x02e07650, LENGTH = 0x4
    OPTION_SETTING_OTP_SACC02  (r) : ORIGIN = 0x02e07660, LENGTH = 0x4
    OPTION_SETTING_OTP_SACC12  (r) : ORIGIN = 0x02e07670, LENGTH = 0x4
    OPTION_SETTING_OTP_SACC03  (r) : ORIGIN = 0x02e07680, LENGTH = 0x4
    OPTION_SETTING_OTP_SACC13  (r) : ORIGIN = 0x02e07690, LENGTH = 0x4
    OPTION_SETTING_OTP_PBPS_SEC (r) : ORIGIN = 0x02e17700, LENGTH = 0x80
    OPTION_SETTING_OTP_PBPS    (r) : ORIGIN = 0x12e17780, LENGTH = 0x80
    OPTION_SETTING_OTP_ZHUK    (r) : ORIGIN = 0x02e17920, LENGTH = 0x4
}

/* Linker script to place sections and symbols. It defines following symbols,
 * which code can use without definition:
 *   __etext, _sidata
 *   __data_start__, __data_end__, _sdata, _edata
 *   __preinit_array_start, __preinit_array_end
 *   __init_array_start, __init_array_end
 *   __fini_array_start, __fini_array_end
 *   __bss_start__, __bss_end__, _sbss, _ebss
 *   __StackTop, __StackLimit, __stack, _estack
 *   __HeapBase, __HeapLimit, __end__, end
 *   __exidx_start, __exidx_end
 *   __vector_ram_start__, __vector_ram_end__
 */
ENTRY(Reset_Handler)

SECTIONS
{
    /* ================================ FLASH ================================ */

    .text :
    {
        /* MCU vector table. Must be the first section: 0x020C0000 is the
         * CPU1 initial VTOR programmed by CPU0 (CPSCU.CPU1INITVTOR). */
        KEEP(*(.fixed_vectors))
        KEEP(*(.application_vectors))

        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)

        *(.got.plt)
        *(.got)

        KEEP(*(.mcuboot_sce9_key))
        KEEP(*(.version))
    } > FLASH

    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } > FLASH

    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN(__init_array_end = .);
    } > FLASH

    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > FLASH

    /* .ARM.extab sections contain exception unwinding information */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    /* .ARM.exidx sections contain index entries for section unwinding */
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    __etext = .;
    _sidata = LOADADDR(.data);

    /* ============================ DTCM (STCM) ============================== */

    /* Relocatable vector table used by the mbed NVIC API. Lives in the
     * Cortex-M33 STCM (0x20000000). */
    .vector_ram (NOLOAD) :
    {
        __vector_ram_start__ = .;
        . += __vector_table_size__;
        __vector_ram_end__ = .;
    } > DTCM

    /* Non-initialized dtcm */
    __dtcm_noinit$$ (NOLOAD) :
    {
        __dtcm_noinit$$Base = .;
        *(.dtcm_noinit)
        __dtcm_noinit$$Limit = .;
    } > DTCM

    /* Zeroed dtcm. Zeroed by Reset_Handler through the __dtcm_zero$$ range. */
    __dtcm_zero$$ (NOLOAD) :
    {
        __dtcm_zero$$Base = .;
        *(.dtcm)
        __dtcm_zero$$Limit = .;
    } > DTCM

    /* ============================ ITCM (CTCM) ============================== */

    /* Non-initialized itcm */
    __itcm_noinit$$ (NOLOAD) :
    {
        __itcm_noinit$$Base = .;
        *(.itcm_noinit)
        __itcm_noinit$$Limit = .;
    } > ITCM

    /* Zeroed itcm. Zeroed by Reset_Handler through the __itcm_zero$$ range. */
    __itcm_zero$$ (NOLOAD) :
    {
        __itcm_zero$$Base = .;
        *(.itcm)
        __itcm_zero$$Limit = .;
    } > ITCM

    /* ================================= RAM ================================= */

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
    /* Crash data ram, right after the vector table */
    .crash_data_ram (NOLOAD) :
    {
        . = ALIGN(8);
        __CRASH_DATA_RAM__ = .;
        __CRASH_DATA_RAM_START__ = .;
        KEEP(*(.keep.crash_data_ram))
        *(.m_crash_data_ram)
        . += M_CRASH_DATA_RAM_SIZE;
        . = ALIGN(8);
        __CRASH_DATA_RAM_END__ = .;
    } > RAM
#endif

    /* Initialized data. Copied from FLASH by Reset_Handler through the
     * __ram_from_flash$$* range in g_init_info. */
    .data :
    {
        __data_start__ = .;
        _sdata = .;
        __ram_from_flash$$Base = .;
        *(.ram_from_flash)
        *(.ram_code_from_flash)
        *(.data*)
        __ram_from_flash$$Limit = .;
        __data_end__ = .;
        _edata = .;
    } > RAM AT > FLASH
    __ram_from_flash$$Load = LOADADDR(.data);

    /* Non-cached RAM range hooks (no D-cache on the Cortex-M33, so this is
     * only kept for section compatibility). */
    __ram_noinit_nocache$$ (NOLOAD) : ALIGN(32)
    {
        __bss_start__ = .;
        _sbss = .;
        __ram_noinit_nocache$$Base = .;
        __noncached_start = .;
        *(.noncached)
        *(.ram_noinit_nocache)
        __ram_noinit_nocache$$Limit = .;
    } > RAM

    __ram_zero_nocache$$ (NOLOAD) :
    {
        __ram_zero_nocache$$Base = .;
        *(.ram_nocache)
        *(.fsp_dtc_vector_table)
        . = ALIGN(32);
        __noncached_end = .;
        __ram_zero_nocache$$Limit = .;
    } > RAM

    /* Main stack and no-init data. The main stack is entered before the C
     * runtime runs, so it must not be zeroed or copied. */
    .noinit (NOLOAD) :
    {
        __ram_noinit$$Base = .;
        __StackLimit = .;
        *(.bss.g_main_stack)
        __StackTop = .;
        _estack = __StackTop;
        PROVIDE(__stack = __StackTop);
        *(.ram_noinit)
        *(.noinit)
        __ram_noinit$$Limit = .;
    } > RAM

    /* Zeroed data. Zeroed by Reset_Handler through the __ram_zero$$ range. */
    .bss (NOLOAD) :
    {
        __ram_zero$$Base = .;
        *(.ram)
        *(.bss*)
        *(COMMON)
        . = ALIGN(32);
        __ram_zero$$Limit = .;
        __bss_end__ = .;
        _ebss = .;
    } > RAM

    /* Thread stacks placed with an explicit section attribute */
    __ram_thread_stack$$ (NOLOAD) : ALIGN(8)
    {
        __ram_thread_stack$$Base = .;
        KEEP(*(.stack?*))
        __ram_thread_stack$$Limit = .;
    } > RAM

    /* Heap, occupying the remainder of SRAM.  Note that __HeapLimit is set to the
     * end of RAM with a plain symbol assignment instead of advancing the location
     * counter, so the .heap section does not physically fill the region.  This way
     * the linker's memory usage statistics (--print-memory-usage) report the
     * statically allocated data only, not the reserved heap space. */
    .heap (NOLOAD) :
    {
        __HeapBase = .;
        __end__ = .;
        PROVIDE(end = .);
        *(.bss.g_heap)
        __HeapLimit = ORIGIN(RAM) + LENGTH(RAM);
    } > RAM

    ASSERT(__HeapBase <= ORIGIN(RAM) + LENGTH(RAM), "region RAM overflowed with stack, data and bss")

    /* ===================== EMPTY FSP INIT TABLE RANGES ===================== */

    /* Zero-length ranges referenced by g_init_info (bsp_linker.c). No Mbed
     * build copies data out of SDRAM, OSPI or data flash (the RA8P1 has no
     * data flash), so Base == Limit and the BSP init code skips them. */
    __sdram_zero_nocache$$Base           = ORIGIN(RAM);
    __sdram_zero_nocache$$Limit          = ORIGIN(RAM);
    __sdram_noinit_nocache$$Base         = ORIGIN(RAM);
    __sdram_zero$$Base                   = ORIGIN(RAM);
    __sdram_zero$$Limit                  = ORIGIN(RAM);
    __sdram_from_ospi0_cs1$$Base         = ORIGIN(RAM);
    __sdram_from_ospi0_cs1$$Limit        = ORIGIN(RAM);
    __sdram_from_ospi0_cs1$$Load         = ORIGIN(RAM);
    __sdram_from_ospi1_cs1$$Base         = ORIGIN(RAM);
    __sdram_from_ospi1_cs1$$Limit        = ORIGIN(RAM);
    __sdram_from_ospi1_cs1$$Load         = ORIGIN(RAM);
    __sdram_from_data_flash$$Base        = ORIGIN(RAM);
    __sdram_from_data_flash$$Limit       = ORIGIN(RAM);
    __sdram_from_data_flash$$Load        = ORIGIN(RAM);
    __sdram_from_flash$$Base             = ORIGIN(RAM);
    __sdram_from_flash$$Limit            = ORIGIN(RAM);
    __sdram_from_flash$$Load             = ORIGIN(RAM);
    __ospi0_cs0_zero_nocache$$Base        = ORIGIN(RAM);
    __ospi0_cs0_zero_nocache$$Limit       = ORIGIN(RAM);
    __ospi0_cs0_zero$$Base                = ORIGIN(RAM);
    __ospi0_cs0_zero$$Limit               = ORIGIN(RAM);
    __ospi0_cs0_noinit_nocache$$Base      = ORIGIN(RAM);
    __ospi0_cs0_noinit_nocache$$Limit     = ORIGIN(RAM);
    __ospi0_cs0_from_ospi0_cs1$$Base      = ORIGIN(RAM);
    __ospi0_cs0_from_ospi0_cs1$$Limit     = ORIGIN(RAM);
    __ospi0_cs0_from_ospi0_cs1$$Load      = ORIGIN(RAM);
    __ospi0_cs0_from_ospi1_cs1$$Base      = ORIGIN(RAM);
    __ospi0_cs0_from_ospi1_cs1$$Limit     = ORIGIN(RAM);
    __ospi0_cs0_from_ospi1_cs1$$Load      = ORIGIN(RAM);
    __ospi0_cs0_from_data_flash$$Base     = ORIGIN(RAM);
    __ospi0_cs0_from_data_flash$$Limit    = ORIGIN(RAM);
    __ospi0_cs0_from_data_flash$$Load     = ORIGIN(RAM);
    __ospi0_cs0_from_flash$$Base          = ORIGIN(RAM);
    __ospi0_cs0_from_flash$$Limit         = ORIGIN(RAM);
    __ospi0_cs0_from_flash$$Load          = ORIGIN(RAM);
    __ospi1_cs0_zero_nocache$$Base        = ORIGIN(RAM);
    __ospi1_cs0_zero_nocache$$Limit       = ORIGIN(RAM);
    __ospi1_cs0_zero$$Base                = ORIGIN(RAM);
    __ospi1_cs0_zero$$Limit               = ORIGIN(RAM);
    __ospi1_cs0_noinit_nocache$$Base      = ORIGIN(RAM);
    __ospi1_cs0_noinit_nocache$$Limit     = ORIGIN(RAM);
    __ospi1_cs0_from_ospi0_cs1$$Base      = ORIGIN(RAM);
    __ospi1_cs0_from_ospi0_cs1$$Limit     = ORIGIN(RAM);
    __ospi1_cs0_from_ospi0_cs1$$Load      = ORIGIN(RAM);
    __ospi1_cs0_from_ospi1_cs1$$Base      = ORIGIN(RAM);
    __ospi1_cs0_from_ospi1_cs1$$Limit     = ORIGIN(RAM);
    __ospi1_cs0_from_ospi1_cs1$$Load      = ORIGIN(RAM);
    __ospi1_cs0_from_data_flash$$Base     = ORIGIN(RAM);
    __ospi1_cs0_from_data_flash$$Limit    = ORIGIN(RAM);
    __ospi1_cs0_from_data_flash$$Load     = ORIGIN(RAM);
    __ospi1_cs0_from_flash$$Base          = ORIGIN(RAM);
    __ospi1_cs0_from_flash$$Limit         = ORIGIN(RAM);
    __ospi1_cs0_from_flash$$Load          = ORIGIN(RAM);
    __itcm_from_ospi0_cs1$$Base           = ORIGIN(ITCM);
    __itcm_from_ospi0_cs1$$Limit          = ORIGIN(ITCM);
    __itcm_from_ospi0_cs1$$Load           = ORIGIN(ITCM);
    __itcm_from_ospi1_cs1$$Base           = ORIGIN(ITCM);
    __itcm_from_ospi1_cs1$$Limit          = ORIGIN(ITCM);
    __itcm_from_ospi1_cs1$$Load           = ORIGIN(ITCM);
    __itcm_from_data_flash$$Base          = ORIGIN(ITCM);
    __itcm_from_data_flash$$Limit         = ORIGIN(ITCM);
    __itcm_from_data_flash$$Load          = ORIGIN(ITCM);
    __itcm_from_flash$$Base               = ORIGIN(ITCM);
    __itcm_from_flash$$Limit              = ORIGIN(ITCM);
    __itcm_from_flash$$Load               = ORIGIN(ITCM);
    __dtcm_from_ospi0_cs1$$Base           = ORIGIN(DTCM);
    __dtcm_from_ospi0_cs1$$Limit          = ORIGIN(DTCM);
    __dtcm_from_ospi0_cs1$$Load           = ORIGIN(DTCM);
    __dtcm_from_ospi1_cs1$$Base           = ORIGIN(DTCM);
    __dtcm_from_ospi1_cs1$$Limit          = ORIGIN(DTCM);
    __dtcm_from_ospi1_cs1$$Load           = ORIGIN(DTCM);
    __dtcm_from_data_flash$$Base          = ORIGIN(DTCM);
    __dtcm_from_data_flash$$Limit         = ORIGIN(DTCM);
    __dtcm_from_data_flash$$Load          = ORIGIN(DTCM);
    __dtcm_from_flash$$Base               = ORIGIN(DTCM);
    __dtcm_from_flash$$Limit              = ORIGIN(DTCM);
    __dtcm_from_flash$$Load               = ORIGIN(DTCM);
    __ram_from_ospi0_cs1$$Base            = ORIGIN(RAM);
    __ram_from_ospi0_cs1$$Limit           = ORIGIN(RAM);
    __ram_from_ospi0_cs1$$Load            = ORIGIN(RAM);
    __ram_from_ospi1_cs1$$Base            = ORIGIN(RAM);
    __ram_from_ospi1_cs1$$Limit           = ORIGIN(RAM);
    __ram_from_ospi1_cs1$$Load            = ORIGIN(RAM);
    __ram_from_data_flash$$Base           = ORIGIN(RAM);
    __ram_from_data_flash$$Limit          = ORIGIN(RAM);
    __ram_from_data_flash$$Load           = ORIGIN(RAM);

    /* ============================ OPTION SETTINGS =========================== */

    /* All option-setting sections stay empty for CPU1 builds: bsp_linker.c
     * only emits option bytes on the CPU0 (primary core) image. The regions
     * are kept so the section references in bsp_linker.c resolve. */

    .option_setting_ofs0 :
    {
        KEEP(*(.option_setting_ofs0))
    } > OPTION_SETTING_OFS0

    .option_setting_ofs2 :
    {
        KEEP(*(.option_setting_ofs2))
    } > OPTION_SETTING_OFS2

    .option_setting_sas :
    {
        KEEP(*(.option_setting_sas))
    } > OPTION_SETTING_SAS

    .option_setting_ofs1 :
    {
        KEEP(*(.option_setting_ofs1))
    } > OPTION_SETTING_OFS1

    .option_setting_ofs1_sec :
    {
        KEEP(*(.option_setting_ofs1_sec))
    } > OPTION_SETTING_OFS1_SEC

    .option_setting_ofs1_sel :
    {
        KEEP(*(.option_setting_ofs1_sel))
    } > OPTION_SETTING_OFS1_SEL

    .option_setting_ofs3 :
    {
        KEEP(*(.option_setting_ofs3))
    } > OPTION_SETTING_OFS3

    .option_setting_ofs3_sec :
    {
        KEEP(*(.option_setting_ofs3_sec))
    } > OPTION_SETTING_OFS3_SEC

    .option_setting_ofs3_sel :
    {
        KEEP(*(.option_setting_ofs3_sel))
    } > OPTION_SETTING_OFS3_SEL

    .option_setting_bps :
    {
        KEEP(*(.option_setting_bps))
    } > OPTION_SETTING_BPS

    .option_setting_bps_sec :
    {
        KEEP(*(.option_setting_bps_sec))
    } > OPTION_SETTING_BPS_SEC

    .option_setting_otp_fsblctrl0 :
    {
        KEEP(*(.option_setting_otp_fsblctrl0))
    } > OPTION_SETTING_OTP_FSBLCTRL0

    .option_setting_otp_fsblctrl1 :
    {
        KEEP(*(.option_setting_otp_fsblctrl1))
    } > OPTION_SETTING_OTP_FSBLCTRL1

    .option_setting_otp_fsblctrl2 :
    {
        KEEP(*(.option_setting_otp_fsblctrl2))
    } > OPTION_SETTING_OTP_FSBLCTRL2

    .option_setting_otp_samr :
    {
        KEEP(*(.option_setting_otp_samr))
    } > OPTION_SETTING_OTP_SAMR

    .option_setting_otp_sacc00 :
    {
        KEEP(*(.option_setting_otp_sacc00))
    } > OPTION_SETTING_OTP_SACC00

    .option_setting_otp_sacc10 :
    {
        KEEP(*(.option_setting_otp_sacc10))
    } > OPTION_SETTING_OTP_SACC10

    .option_setting_otp_sacc01 :
    {
        KEEP(*(.option_setting_otp_sacc01))
    } > OPTION_SETTING_OTP_SACC01

    .option_setting_otp_sacc11 :
    {
        KEEP(*(.option_setting_otp_sacc11))
    } > OPTION_SETTING_OTP_SACC11

    .option_setting_otp_sacc02 :
    {
        KEEP(*(.option_setting_otp_sacc02))
    } > OPTION_SETTING_OTP_SACC02

    .option_setting_otp_sacc12 :
    {
        KEEP(*(.option_setting_otp_sacc12))
    } > OPTION_SETTING_OTP_SACC12

    .option_setting_otp_sacc03 :
    {
        KEEP(*(.option_setting_otp_sacc03))
    } > OPTION_SETTING_OTP_SACC03

    .option_setting_otp_sacc13 :
    {
        KEEP(*(.option_setting_otp_sacc13))
    } > OPTION_SETTING_OTP_SACC13

    /* Permanent Block Protect Setting Register Secure (OTP) */
    .option_setting_otp_pbps_sec :
    {
        KEEP(*(.option_setting_otp_pbps_sec))
    } > OPTION_SETTING_OTP_PBPS_SEC

    /* Permanent Block Protect Setting Register (OTP) */
    .option_setting_otp_pbps :
    {
        KEEP(*(.option_setting_otp_pbps))
    } > OPTION_SETTING_OTP_PBPS

    /* Zerorization HUK Register (OTP) */
    .option_setting_otp_zhuk :
    {
        KEEP(*(.option_setting_otp_zhuk))
    } > OPTION_SETTING_OTP_ZHUK
}

/*
** ###################################################################
**     Processors:          MIMXRT1052CVJ5B
**                          MIMXRT1052CVL5B
**                          MIMXRT1052DVJ6B
**                          MIMXRT1052DVL6B
**
**     Compiler:            GNU C Compiler
**     Reference manual:    IMXRT1050RM Rev.1, 03/2018
**     Version:             rev. 1.0, 2018-09-21
**     Build:               b180921
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2018 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

__ram_vector_table__ = 1;

#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
    #define MBED_CONF_TARGET_BOOT_STACK_SIZE 0x400
#endif

__stack_size__ = MBED_CONF_TARGET_BOOT_STACK_SIZE;

STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x0400 : 0x0;

M_CRASH_DATA_RAM_SIZE = 0x100;

/* If we are not configured to execute out of the start of ROM, then a bootloader is
 * present.  This tells us whether we need to add the Flash Configuration Field at the start of flash. */
#if MBED_CONFIGURED_ROM_BANK_EXT_FLASH_START == MBED_ROM_BANK_EXT_FLASH_START
#define IS_BOOTLOADER_PRESENT 0
#else
#define IS_BOOTLOADER_PRESENT 1
#endif

/* Specify the ELF segments (program headers) */
PHDRS
{
  text PT_LOAD FLAGS(5); /* read + execute */
  ram_functions PT_LOAD FLAGS(5); /* read + execute */
  ram_vector_table PT_LOAD FLAGS(6); /* read + write */
  ram_noinit PT_LOAD FLAGS(6); /* read + write */
  ram_init PT_LOAD FLAGS(6); /* read + write */
  dtcm_stack PT_LOAD FLAGS(6); /* read + write */
  ram_ncache_noinit PT_LOAD FLAGS(6); /* read + write */
  ram_ncache_init PT_LOAD FLAGS(6); /* read + write */
  crash_data_ram PT_LOAD FLAGS(6); /* read + write */
}

/* Specify the memory areas */
MEMORY
{
  m_text                (RX)  : ORIGIN = MBED_CONFIGURED_ROM_BANK_EXT_FLASH_START, LENGTH = MBED_CONFIGURED_ROM_BANK_EXT_FLASH_SIZE

/* DTCM memory.
     Startup code configures size to 256k (stealing space from OCRAM). */
  m_dtcm                (RW)  : ORIGIN = MBED_RAM_BANK_SRAM_DTC_START, LENGTH = MBED_RAM_BANK_SRAM_DTC_SIZE

#ifdef MBED_RAM_BANK_SDRAM_SIZE
  /* Use the external RAM as main memory */
  m_sdram               (RW)  : ORIGIN = MBED_RAM_BANK_SDRAM_START, LENGTH = MBED_RAM_BANK_SDRAM_SIZE
#endif

  /* ITCM bank -- used for functions that need to execute from RAM
     (which is faster than having to load them from flash).
     Startup code configures size to 128k. */
  m_itcm               (RX)  : ORIGIN = MBED_RAM_BANK_SRAM_ITC_START, LENGTH = MBED_RAM_BANK_SRAM_ITC_SIZE

  /* OCRAM bank -- extra RAM, available for misc storage but slower to access.
     Startup code configures size to 128k.
     Note that address is different on the 105x and the 106x. */
  m_ocram              (RW)  : ORIGIN = MBED_RAM_BANK_SRAM_OC_START, LENGTH = MBED_RAM_BANK_SRAM_OC_SIZE

#ifdef MBED_RAM_BANK_SRAM_OC2_START
  /* OCRAM2 bank -- extra RAM, available on MIMXRT106x only. */
  m_ocram2             (RW)  : ORIGIN = MBED_RAM_BANK_SRAM_OC2_START, LENGTH = MBED_RAM_BANK_SRAM_OC2_SIZE
#endif
}

/* Define output sections */
SECTIONS
{
#if !IS_BOOTLOADER_PRESENT
  /* Flash config goes first, at the start of flash */
  .flash_config :
  {
    __FLASH_BASE = .;
    KEEP(* (.boot_hdr.conf))     /* flash config section */
  } > m_text :text

  /* Then IVT at offset 0x1000 */
  .ivt MBED_ROM_BANK_EXT_FLASH_START + 0x1000 :
  {
    KEEP(* (.boot_hdr.ivt))           /* ivt section */
    KEEP(* (.boot_hdr.boot_data))     /* boot section */
    KEEP(* (.boot_hdr.dcd_data))      /* dcd section */
  } > m_text :text
  /* Note that we do not want to use AT> above, because that would result in this section being
   * placed in flash immediately after .flash_config.  Instead we don't use AT> to set the LMA equal
   * to the VMA. */

  /* Interrupts go after that */
  #define INTERRUPT_TABLE_ADDR MBED_ROM_BANK_EXT_FLASH_START + 0x2000
#else
  /* We have a bootloader so we don't need the flash config or IVT */
  /DISCARD/ : {
    *(.boot_hdr.conf)
    *(.boot_hdr.ivt)
    *(.boot_hdr.boot_data)
    *(.boot_hdr.dcd_data)
  }

  /* Interrupts go at the start of configured flash area */
  #define INTERRUPT_TABLE_ADDR MBED_CONFIGURED_ROM_BANK_EXT_FLASH_START
#endif

  /* Now we have the ISR vector */
  .interrupts INTERRUPT_TABLE_ADDR :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(8);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(8);
  } > m_text :text

  /* The program code and other data goes into internal RAM */
  .text :
  {
    . = ALIGN(8);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(8);
  } > m_text :text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text :text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text :text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text :text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text :text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text :text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text :text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text :text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .interrupts_ram :
  {
    . = ALIGN(8);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(8);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_dtcm :ram_vector_table

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data :
  {
    . = ALIGN(8);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(m_usb_dma_init_data)
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(8);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_dtcm AT> m_text :ram_init
  __ram_function_flash_start = __DATA_ROM + (__data_end__ - __data_start__); /* Symbol is used by startup for TCM data initialization */

  .ram_function :
  {
    . = ALIGN(32);
    __ram_function_start__ = .;
    *(CodeQuickAccess)
    . = ALIGN(128);
    __ram_function_end__ = .;
  } > m_itcm AT> m_text :ram_functions

  __ram_function_size = SIZEOF(.ram_function);

  __NDATA_ROM = __ram_function_flash_start + SIZEOF(.ram_function);

  /* Always store noncacheable data (e.g. DMA descriptors) in DTCM, since this memory
     is always cache coherent. */
  .ncache.init :
  {
    __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
    *(NonCacheable.init)
    . = ALIGN(8);
    __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
  } > m_dtcm AT> m_text :ram_ncache_init
  . = __noncachedata_init_end__;
  .ncache :
  {
    *(NonCacheable)
    . = ALIGN(8);
    __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
  } > m_dtcm :ram_ncache_noinit

  __TEXT_CSF_ROM = __NDATA_ROM + (__noncachedata_init_end__ - __noncachedata_start__);

  .csf :
  {
    FILL(0xFF)
    . = ALIGN(1024);
    KEEP(*(.csf))
    __text_csf_end = .;
  } > m_text :text


  /* __USED_FLASH_END gives the end of all data written to the flash memory, including code and data
      initializers. */
  PROVIDE(__USED_FLASH_END = __TEXT_CSF_ROM + SIZEOF(.csf));

  /* __USED_FLASH_SIZE gives the size of all data written to the flash memory, including code and data
      initializers. */
  PROVIDE(__USED_FLASH_SIZE = __USED_FLASH_END - ORIGIN(m_text));

  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__USED_FLASH_END <= text_end, "Code and data initializers exceed flash size!")

  /* Uninitialized data section */
  .bss (NOLOAD):
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(8);
    __START_BSS = .;
    __bss_start__ = .;
    *(m_usb_dma_noninit_data)
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_dtcm :ram_noinit

  . = ALIGN(8);
  __end__ = .;
  PROVIDE(end = .);

  .heap_0 (NOLOAD):
  {
      PROVIDE(__mbed_sbrk_start_0 = .);
      . =  ORIGIN(m_dtcm) + LENGTH(m_dtcm) - STACK_SIZE;
      PROVIDE(__mbed_krbs_start_0 = .);
  } > m_dtcm

  /* Reserve space for stack (even though stack is always at the end of DTCM regardless
     of where this section is located) */
  .stack (NOLOAD):
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_dtcm :dtcm_stack

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_dtcm) + LENGTH(m_dtcm);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  /* Store crash data RAM at the end of OCRAM (which is otherwise unused).
     Note that the ROM bootloader clobbers the first part of OCRAM, so we have to put this at the end. */
  __CRASH_DATA_RAM_START__ = ORIGIN(m_ocram) + LENGTH(m_ocram) - M_CRASH_DATA_RAM_SIZE; /* Create a global symbol at data start */

  /* Second heap region. Use OCRAM2 if available, otherwise use OCRAM.
   * TODO https://github.com/mbed-ce/mbed-os/issues/413 Use SDRAM once this bug is fixed. */
#ifdef MBED_RAM_BANK_SRAM_OC2_START
  .heap (NOLOAD):
  {
      PROVIDE(__mbed_sbrk_start = .);
      . += (ORIGIN(m_ocram2) + LENGTH(m_ocram2));
      PROVIDE(__mbed_krbs_start = .);
  } > m_ocram2
#else
  .heap (NOLOAD):
  {
      PROVIDE(__mbed_sbrk_start = .);
      . = __CRASH_DATA_RAM_START__;
      PROVIDE(__mbed_krbs_start = .);
  } > m_ocram2
#endif

  .crash_data_ram __CRASH_DATA_RAM_START__ :
  {
    . = ALIGN(8);
    __CRASH_DATA_RAM__ = .;
    KEEP(*(.keep.crash_data_ram))
    *(.m_crash_data_ram)     /* This is a user defined section */
    . += M_CRASH_DATA_RAM_SIZE;
    . = ALIGN(8);
    __CRASH_DATA_RAM_END__ = .; /* Define a global symbol at data end */
  } > m_ocram :crash_data_ram

  .ARM.attributes 0 : { *(.ARM.attributes) }

  /* Teensy model identifier symbol (used by Teensy Loader) */
  /* See here https://forum.pjrc.com/threads/72107-Testers-Needed-Mbed-OS-Port-for-Teensy-4-0?p=320414&viewfull=1#post320414 */
#if defined(TARGET_TEENSY_40)
	_teensy_model_identifier = 0x24;
#elif defined(TARGET_TEENSY_41)
	_teensy_model_identifier = 0x25;
#endif
}


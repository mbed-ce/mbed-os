/*
** ###################################################################
**     Processors:          MK64FN1M0CAJ12
**                          MK64FN1M0VDC12
**                          MK64FN1M0VLL12
**                          MK64FN1M0VLQ12
**                          MK64FN1M0VMD12
**
**     Compiler:            GNU C Compiler
**     Reference manual:    K64P144M120SF5RM, Rev.2, January 2014
**     Version:             rev. 2.9, 2016-03-21
**     Build:               b160613
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright (c) 2016 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
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
FLASH_VTOR_TABLE_SIZE = 0x400;
FLASH_CONFIG_FIELD_SIZE = 0x10;

/* If we are not configured to execute out of the start of ROM, then a bootloader is
 * present.  This tells us whether we need to add the Flash Configuration Field at the start of flash. */
#if MBED_CONFIGURED_ROM_BANK_IROM1_START == 0
#define IS_BOOTLOADER_PRESENT 0
#else
#define IS_BOOTLOADER_PRESENT 1
#endif

#if IS_BOOTLOADER_PRESENT && MBED_CONFIGURED_ROM_BANK_IROM1_START < 0x410
#error MBED_CONFIGURED_ROM_BANK_IROM1_START too small and will overwrite interrupts and flash config
#endif

/* Specify the ELF segments (program headers) */
PHDRS
{
  text PT_LOAD FLAGS(5); /* read + execute */
  ram_vector_table PT_LOAD FLAGS(6); /* read + write */
  ram_noinit PT_LOAD FLAGS(6); /* read + write */
  ram_init PT_LOAD FLAGS(6); /* read + write */
  sram_l PT_LOAD FLAGS(6); /* read + write */
}

/* Specify the memory areas */
MEMORY
{
  m_text   (RX)  : ORIGIN = MBED_CONFIGURED_ROM_BANK_IROM1_START, LENGTH = MBED_CONFIGURED_ROM_BANK_IROM1_SIZE

  /*
   * Note: while these two SRAMs occupy a contiguous address space, we have to keep them as
   * separate memory banks because the MCU doesn't support accesses that bridge the two banks.
   */
  m_sram_l (RW)  : ORIGIN = MBED_RAM_BANK_SRAM_L_START, LENGTH = MBED_RAM_BANK_SRAM_L_SIZE
  m_sram_u (RW)  : ORIGIN = MBED_RAM_BANK_SRAM_U_START, LENGTH = MBED_RAM_BANK_SRAM_U_SIZE
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(8);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(8);
  } > m_text :text

  /* FCF to absolute address of 0x400, but only if bootloader is not present. */
#if !IS_BOOTLOADER_PRESENT
  .flash_config FLASH_VTOR_TABLE_SIZE :
  {
    KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
  } > m_text :text
#else
  /DISCARD/ : {
    *(.FlashConfig)
  }
#endif

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

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
  /* Stick the crash data ram at the start of sram_l */
  .crash_data_ram : ALIGN(8)
  {
    __CRASH_DATA_RAM__ = .;
    __CRASH_DATA_RAM_START__ = .; /* Create a global symbol at data start */
    KEEP(*(.keep.crash_data_ram))
    *(.m_crash_data_ram)     /* This is a user defined section */
    . += M_CRASH_DATA_RAM_SIZE;
    . = ALIGN(8);
    __CRASH_DATA_RAM_END__ = .; /* Define a global symbol at data end */
  } > m_sram_l :sram_l
#endif

  /* Fill the entire sram_l with the heap 0 section. */
  .heap_0 (NOLOAD): ALIGN(8)
  {
    __mbed_sbrk_start_0 = .;
    . += (ORIGIN(m_sram_l) + LENGTH(m_sram_l) - .);
    __mbed_krbs_start_0 = .;
  } > m_sram_l :sram_l

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .interrupts_ram :
  {
    . = ALIGN(8);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(8);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_sram_u :ram_vector_table

  .data : ALIGN(8)
  {
    PROVIDE(__etext = LOADADDR(.data));    /* Define a global symbol at end of code, */
    PROVIDE(__DATA_ROM = LOADADDR(.data)); /* Symbol is used by startup for data initialization. */
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(8);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_sram_u AT > m_text :ram_init

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")


  /* Uninitialized data section
   * This region is not initialized by the C/C++ library and can be used to
   * store state across soft reboots. */
  .uninitialized (NOLOAD):
  {
    . = ALIGN(32);
    __uninitialized_start = .;
    *(.uninitialized)
    KEEP(*(.keep.uninitialized))
    . = ALIGN(32);
    __uninitialized_end = .;
  } > m_sram_u AT> m_sram_u :ram_noinit

  USB_RAM_GAP = DEFINED(__usb_ram_size__) ? __usb_ram_size__ : 0x800;

  /* Uninitialized data section */
  .bss : ALIGN(8)
  {
    /* This is used by the startup in order to initialize the .bss section */
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    . = ALIGN(512);
    USB_RAM_START = .;
    . += USB_RAM_GAP;
    *(COMMON)
    . = ALIGN(8);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_sram_u AT> m_sram_u :ram_noinit

  .heap : ALIGN(8)
  {
    __end__ = .;
    PROVIDE(end = .);

    ASSERT((__end__ - ORIGIN(m_sram_u)) + STACK_SIZE <= LENGTH(m_sram_u), "SRAM_U is not large enough to contain globals and boot stack!");

    __mbed_sbrk_start = .;
    __HeapBase = .;
    . = ORIGIN(m_sram_u) + LENGTH(m_sram_u) - STACK_SIZE;
    __mbed_krbs_start = .;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_sram_u AT> m_sram_u :ram_noinit

  /* USB RAM sections.  These live inside a "gap" created in .bss. */
  m_usb_bdt USB_RAM_START (OVERLAY) :
  {
    *(m_usb_bdt)
    USB_RAM_BDT_END = .;
  } :ram_noinit

  m_usb_global USB_RAM_BDT_END (OVERLAY) :
  {
    *(m_usb_global)
  } :ram_noinit

  /* Initializes stack on the end of block */
  __StackTop = ORIGIN(m_sram_u) + LENGTH(m_sram_u);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  /DISCARD/ : {
    *(.ARM.attributes)
  }
}

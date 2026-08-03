/* SPDX-License-Identifier: Apache-2.0 */
#include "../cmsis_nvic.h"

M_CRASH_DATA_RAM_SIZE = 0x100;
#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
#define MBED_CONF_TARGET_BOOT_STACK_SIZE 0x400
#endif
STACK_SIZE = MBED_CONF_TARGET_BOOT_STACK_SIZE;

/* The WB0 ROM ABI owns 0x000-0x0ff. The RAM vector table follows it. */
#define WB0_RAM_PREAMBLE_SIZE 0x100
#define MBED_VECTTABLE_RAM_SIZE (((NVIC_NUM_VECTORS * 4) + 7) & 0xFFFFFFF8)

MEMORY
{
  FLASH (rx)  : ORIGIN = MBED_CONFIGURED_ROM_BANK_IROM1_START, LENGTH = MBED_CONFIGURED_ROM_BANK_IROM1_SIZE
  RAM   (xrw) : ORIGIN = MBED_RAM_BANK_SRAM1_START + WB0_RAM_PREAMBLE_SIZE + MBED_VECTTABLE_RAM_SIZE,
                LENGTH = MBED_RAM_BANK_SRAM1_SIZE - WB0_RAM_PREAMBLE_SIZE - MBED_VECTTABLE_RAM_SIZE
}

ENTRY(Reset_Handler)
PROVIDE(g_pfnVectors = __vector_table);

SECTIONS
{
  .intvec :
  {
    __vector_table_start__ = .;
    KEEP(*(.intvec))
    __vector_table_end__ = .;
  } > FLASH
  ASSERT((__vector_table_end__ - __vector_table_start__) == (NVIC_NUM_VECTORS * 4),
         "STM32WB09 vector count does not match cmsis_nvic.h")

  .text :
  {
    *(.text*)
    KEEP(*(.init))
    KEEP(*(.fini))
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)
    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)
    *(.rodata*)
    KEEP(*(.eh_frame*))
  } > FLASH

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > FLASH
  __exidx_start = .;
  .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } > FLASH
  __exidx_end = .;
  __etext = .;
  _sidata = .;

  /* Fixed locations required by ST's WB0 ROM/system implementation. */
  .ram_vr MBED_RAM_BANK_SRAM1_START + 0x4 (NOLOAD) : { KEEP(*(.ram_vr)) }
  .bss.blueRAM MBED_RAM_BANK_SRAM1_START + 0xC0 (NOLOAD) :
  {
    _sbssblue = .;
    KEEP(*(.bss.__blue_RAM))
    _ebssblue = .;
  }

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

  .data : AT (__etext)
  {
    __data_start__ = .;
    _sdata = .;
    *(vtable)
    *(.data*)
    . = ALIGN(8);
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN(__preinit_array_end = .);
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN(__init_array_end = .);
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN(__fini_array_end = .);
    KEEP(*(.jcr*))
    . = ALIGN(8);
    __data_end__ = .;
    _edata = .;
  } > RAM

  .bss (NOLOAD) :
  {
    . = ALIGN(8);
    __bss_start__ = .;
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
    __bss_end__ = .;
    _ebss = .;
  } > RAM

  .noinit (NOLOAD) : { *(.noinit*) } > RAM

  .heap (COPY) :
  {
    __end__ = .;
    PROVIDE(end = .);
    *(.heap*)
    . = ORIGIN(RAM) + LENGTH(RAM) - MBED_CONF_TARGET_BOOT_STACK_SIZE;
    __HeapLimit = .;
  } > RAM

  .stack_dummy (COPY) : { *(.stack*) } > RAM
  __StackTop = ORIGIN(RAM) + LENGTH(RAM);
  _estack = __StackTop;
  __StackLimit = __StackTop - MBED_CONF_TARGET_BOOT_STACK_SIZE;
  PROVIDE(__stack = __StackTop);
  ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}

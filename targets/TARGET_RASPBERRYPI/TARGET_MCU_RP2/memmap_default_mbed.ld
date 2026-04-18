/* Based on GCC ARM embedded samples.
   Defines the following symbols for use by code:
    __exidx_start
    __exidx_end
    __etext
    __data_start__
    __preinit_array_start
    __preinit_array_end
    __init_array_start
    __init_array_end
    __fini_array_start
    __fini_array_end
    __data_end__
    __bss_start__
    __bss_end__
    __end__
    end
    __HeapLimit
    __StackLimit
    __StackTop
    __StackBottom (== __StackLimit)
    __stack (== StackTop)
*/

#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
    /* This value is normally defined by the tools
       to 0x1000 for bare metal and 0x400 for RTOS */
    #define MBED_CONF_TARGET_BOOT_STACK_SIZE  0x400
#endif

MEMORY
{
    FLASH(rx) : ORIGIN = MBED_CONFIGURED_ROM_BANK_QSPI_FLASH_START, LENGTH = MBED_CONFIGURED_ROM_BANK_QSPI_FLASH_SIZE
    RAM(rwx) : ORIGIN =  MBED_CONFIGURED_RAM_BANK_IRAM1_START, LENGTH = MBED_CONFIGURED_RAM_BANK_IRAM1_SIZE

    /*
     * Scratch banks are commonly used for critical data and functions accessed only by one core (when only
     * one core is accessing the RAM bank, there is no opportunity for stalls).
     */
    SCRATCH_X(rwx) : ORIGIN = MBED_RAM_BANK_SCRATCH_X_START, LENGTH = MBED_RAM_BANK_SCRATCH_X_SIZE
    SCRATCH_Y(rwx) : ORIGIN = MBED_RAM_BANK_SCRATCH_Y_START, LENGTH = MBED_RAM_BANK_SCRATCH_Y_SIZE

/* If we are at the start of RAM, we are core 0.  Otherwise we are core 1. */
#if MBED_CONFIGURED_RAM_BANK_IRAM1_START == MBED_RAM_BANK_IRAM1_START
#define STACK_SCRATCH SCRATCH_X
#define STACK_SCRATCH_STATIC_END_SYMBOL __scratch_x_end__
#else
#define STACK_SCRATCH SCRATCH_Y
#define STACK_SCRATCH_STATIC_END_SYMBOL __scratch_y_end__
#endif
}

ENTRY(_entry_point)

SECTIONS
{
    .flash_begin : {
        __flash_binary_start = .;
    } > FLASH

    /*
     * On RP2040, boot2 must be at the start of the image as it is executed
     * as the entry point to configure XIP flash and start the user program.
     * On RP2350, boot2 can live anywhere and is called by the startup code using the
     * __boot2_entry_point symbol.
     */
    .boot2 : {
        __boot2_start__ = .;
        KEEP (*(.boot2))
        __boot2_end__ = .;
    } > FLASH

    ASSERT(__boot2_end__ - __boot2_start__ == 256,
        "ERROR: Pico second stage bootloader must be 256 bytes in size")

    /* The second stage will always enter the image at the start of .text.
       The debugger will use the ELF entry point, which is the _entry_point
       symbol if present, otherwise defaults to start of .text.
       This can be used to transfer control back to the bootrom on debugger
       launches only, to perform proper flash setup.
    */

    .text : {
        __logical_binary_start = .;
        KEEP (*(.vectors))
        KEEP (*(.binary_info_header))
        __binary_info_header_end = .;
        KEEP (*(.embedded_block))
        __embedded_block_end = .;
        KEEP (*(.reset))

        /* Note: The Pico SDK linker script has some extra stuff here that puts libgcc routines in RAM instead
         * of flash, but this just does not seem to work in Mbed. We get a hardfault from the division operator,
         * with no obvious cause. I don't really understand why but I suspect it's, again, something to do with
         * using a different toolchain (maybe that somehow doesn't support relocating libgcc routines to RAM?) */
        *(.text*)

        /* Note: This KEEP() is important -- without it, only the first part of the _init function from crti.S
         * is linked in, not the last part from crtn.S. This then causes a HardFault later because the registers
         * pushed to the stack were never popped! The linker script used in the Pico SDK does not have this
         * KEEP() and I don't know why it works for them -- maybe a toolchain / C library related difference? */
        KEEP (*(.init))
        KEEP (*(.fini))

        /* Pull all c'tors into .text */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        /* Followed by destructors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(SORT(.preinit_array.*)))
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        *(SORT(.fini_array.*))
        *(.fini_array)
        PROVIDE_HIDDEN (__fini_array_end = .);

        *(.eh_frame*)
        . = ALIGN(4);
    } > FLASH

    .rodata : {
        *(.rodata*)
        *(.srodata*)
        . = ALIGN(4);
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.flashdata*)))
        . = ALIGN(4);
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    /* Machine inspectable binary information */
    . = ALIGN(4);
    __binary_info_start = .;
    .binary_info :
    {
        KEEP(*(.binary_info.keep.*))
        *(.binary_info.*)
    } > FLASH
    __binary_info_end = .;
    . = ALIGN(4);

    /* End of .text-like segments */
    __etext = .;

   .ram_vector_table (NOLOAD): {
        *(.ram_vector_table)
    } > RAM

    .data : {
        __data_start__ = .;
        *(vtable)

        *(.time_critical*)

        /* remaining .text and .rodata; i.e. stuff we exclude above because we want it in RAM */
        *(.text*)
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);

        *(.data*)

        . = ALIGN(4);
        *(.after_data.*)
        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__mutex_array_start = .);
        KEEP(*(SORT(.mutex_array.*)))
        KEEP(*(.mutex_array))
        PROVIDE_HIDDEN (__mutex_array_end = .);

        . = ALIGN(4);
        *(.jcr)
        . = ALIGN(4);

        __data_end__ = .;
    } > RAM AT> FLASH

    .uninitialized_data (NOLOAD): {
        . = ALIGN(4);
        *(.uninitialized_data*)
    } > RAM

    /* Start and end symbols must be word-aligned */
    .scratch_x : {
        __scratch_x_start__ = .;
        *(.scratch_x.*)
        . = ALIGN(4);
        __scratch_x_end__ = .;
    } > SCRATCH_X AT > FLASH
    __scratch_x_source__ = LOADADDR(.scratch_x);

    .scratch_y : {
        __scratch_y_start__ = .;
        *(.scratch_y.*)
        . = ALIGN(4);
        __scratch_y_end__ = .;
    } > SCRATCH_Y AT > FLASH
    __scratch_y_source__ = LOADADDR(.scratch_y);

    .bss  : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.bss*)))
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    .heap (NOLOAD):
    {
        __end__ = .;
        PROVIDE(end = .);
        *(.heap*)
        . = ORIGIN(RAM) + LENGTH(RAM);
        __HeapLimit = .;
    } > RAM

    /* Check if data + heap exceeds RAM limit */
    ASSERT(__end__ < ORIGIN(RAM) + LENGTH(RAM), "region RAM overflowed")

    .flash_end : {
        KEEP(*(.embedded_end_block*))
        PROVIDE(__flash_binary_end = .);
    } > FLASH =0xaa

    __StackTop = ORIGIN(STACK_SCRATCH) + LENGTH(STACK_SCRATCH);
    __StackLimit = __StackTop - MBED_CONF_TARGET_BOOT_STACK_SIZE;
    __StackBottom = __StackLimit;
    ASSERT(__StackLimit >= STACK_SCRATCH_STATIC_END_SYMBOL, "stack scratch region overflowed")
    PROVIDE(__stack = __StackTop);


    ASSERT( __binary_info_header_end - __logical_binary_start <= 1024, "Binary info must be in first 1024 bytes of the binary")

    /* todo assert on extra code */
}
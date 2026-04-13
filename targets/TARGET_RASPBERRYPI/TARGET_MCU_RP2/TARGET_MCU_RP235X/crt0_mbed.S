/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico.h"
#include "pico/asm_helper.S"

#include "pico/platform/cpu_regs.h"

#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#include "pico/binary_info/defs.h"
#include "boot/picobin.h"
#include "pico/bootrom.h"

// PICO_CONFIG: PICO_CRT0_NEAR_CALLS, Whether calls from crt0 into the binary are near (<16M away) - ignored for PICO_COPY_TO_RAM, default=0, type=bool, group=pico_crt0
#ifndef PICO_CRT0_NEAR_CALLS
#define PICO_CRT0_NEAR_CALLS 0
#endif

#ifdef NDEBUG
#ifndef COLLAPSE_IRQS
#define COLLAPSE_IRQS
#endif
#endif

pico_default_asm_setup

// PICO_CONFIG: PICO_CRT0_NO_RESET_SECTION, Omit .reset section contents containing the startup code. This must be set if you want to replace the startup code while still keeping the rest of pico_crt0 as the reset section define here is not garbage collected, type=bool, default=0, advanced=true, group=pico_crt0
#ifndef PICO_CRT0_NO_RESET_SECTION
#define PICO_CRT0_NO_RESET_SECTION 0
#endif

#ifdef PICO_NO_STORED_VECTOR_TABLE
#warning PICO_NO_STORED_VECTOR_TABLE is no longer used. PICO_MINIMAL_STORED_VECTOR_TABLE is not identical but usually serves the same purpose
#endif

.section .vectors, "ax"
.align 2

.global __vectors, __VECTOR_TABLE, __vectors_end
__VECTOR_TABLE:
__vectors:
.word __StackTop
.word _reset_handler

#if PICO_MINIMAL_STORED_VECTOR_TABLE
.word isr_invalid   // NMI
.word isr_invalid   // HardFault
#else
.word NMI_Handler
.word HardFault_Handler
#if PICO_RP2040
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
#else
.word MemManage_Handler
.word BusFault_Handler
.word UsageFault_Handler
.word SecureFault_Handler
#endif
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word SVC_Handler
#if PICO_RP2040
.word isr_invalid // Reserved, should never fire
#else
.word DebugMon_Handler
#endif
.word isr_invalid // Reserved, should never fire
.word PendSV_Handler
.word SysTick_Handler

#if PICO_RP2040
.word TIMER_IRQ_0_Handler
.word TIMER_IRQ_1_Handler
.word TIMER_IRQ_2_Handler
.word TIMER_IRQ_3_Handler
.word PWM_IRQ_WRAP_Handler
.word USBCTRL_IRQ_Handler
.word XIP_IRQ_Handler
.word PIO0_IRQ_0_Handler
.word PIO0_IRQ_1_Handler
.word PIO1_IRQ_0_Handler
.word PIO1_IRQ_1_Handler
.word DMA_IRQ_0_Handler
.word DMA_IRQ_1_Handler
.word IO_IRQ_BANK0_Handler
.word IO_IRQ_QSPI_Handler
.word SIO_IRQ_PROC0_Handler
.word SIO_IRQ_PROC1_Handler
.word CLOCKS_IRQ_Handler
.word SPI0_IRQ_Handler
.word SPI1_IRQ_Handler
.word UART0_IRQ_Handler
.word UART1_IRQ_Handler
.word ADC_IRQ_FIFO_Handler
.word I2C0_IRQ_Handler
.word I2C1_IRQ_Handler
.word RTC_IRQ_Handler
#elif PICO_RP2350
.word TIMER0_IRQ_0_Handler
.word TIMER0_IRQ_1_Handler
.word TIMER0_IRQ_2_Handler
.word TIMER0_IRQ_3_Handler
.word TIMER1_IRQ_0_Handler
.word TIMER1_IRQ_1_Handler
.word TIMER1_IRQ_2_Handler
.word TIMER1_IRQ_3_Handler
.word PWM_IRQ_WRAP_0_Handler
.word PWM_IRQ_WRAP_1_Handler
.word DMA_IRQ_0_Handler
.word DMA_IRQ_1_Handler
.word DMA_IRQ_2_Handler
.word DMA_IRQ_3_Handler
.word USBCTRL_IRQ_Handler
.word PIO0_IRQ_0_Handler
.word PIO0_IRQ_1_Handler
.word PIO1_IRQ_0_Handler
.word PIO1_IRQ_1_Handler
.word PIO2_IRQ_0_Handler
.word PIO2_IRQ_1_Handler
.word IO_IRQ_BANK0_Handler
.word IO_IRQ_BANK0_NS_Handler
.word IO_IRQ_QSPI_Handler
.word IO_IRQ_QSPI_NS_Handler
.word SIO_IRQ_FIFO_Handler
.word SIO_IRQ_BELL_Handler
.word SIO_IRQ_FIFO_NS_Handler
.word SIO_IRQ_BELL_NS_Handler
.word SIO_IRQ_MTIMECMP_Handler
.word CLOCKS_IRQ_Handler
.word SPI0_IRQ_Handler
.word SPI1_IRQ_Handler
.word UART0_IRQ_Handler
.word UART1_IRQ_Handler
.word ADC_IRQ_FIFO_Handler
.word I2C0_IRQ_Handler
.word I2C1_IRQ_Handler
.word OTP_IRQ_Handler
.word TRNG_IRQ_Handler
.word PLL_SYS_IRQ_Handler
.word PLL_USB_IRQ_Handler
.word POWMAN_IRQ_POW_Handler
.word POWMAN_IRQ_TIMER_Handler
#else
#error Unknown processor, can't declare IRQs!
#endif

#endif  // #if !PICO_MINIMAL_STORED_VECTOR_TABLE
__vectors_end:

// all default exception handlers do nothing, and we can check for them being set to our
// default values by seeing if they point to somewhere between __defaults_isrs_start and __default_isrs_end
.global __default_isrs_start
__default_isrs_start:

// Declare a weak symbol for each ISR.
// By default, they will fall through to the undefined IRQ handler below (breakpoint),
// but can be overridden by C functions with correct name.

.macro decl_isr_bkpt name
.weak \name
.type \name,%function
.thumb_func
\name:
    bkpt #0
.endm

.altmacro
.macro decl_isr name
#if !PICO_MINIMAL_STORED_VECTOR_TABLE | PICO_NO_FLASH
// We declare a weak label, so user can override
.weak \name
#else
// We declare a strong global label, so user can't override (their version would not automatically be used)
.global \name
#endif
.type \name,%function
.thumb_func
\name:
.endm

#if PICO_RP2040
decl_isr TIMER_IRQ_0_Handler
decl_isr TIMER_IRQ_1_Handler
decl_isr TIMER_IRQ_2_Handler
decl_isr TIMER_IRQ_3_Handler
decl_isr PWM_IRQ_WRAP_Handler
decl_isr USBCTRL_IRQ_Handler
decl_isr XIP_IRQ_Handler
decl_isr PIO0_IRQ_0_Handler
decl_isr PIO0_IRQ_1_Handler
decl_isr PIO1_IRQ_0_Handler
decl_isr PIO1_IRQ_1_Handler
decl_isr DMA_IRQ_0_Handler
decl_isr DMA_IRQ_1_Handler
decl_isr IO_IRQ_BANK0_Handler
decl_isr IO_IRQ_QSPI_Handler
decl_isr SIO_IRQ_PROC0_Handler
decl_isr SIO_IRQ_PROC1_Handler
decl_isr CLOCKS_IRQ_Handler
decl_isr SPI0_IRQ_Handler
decl_isr SPI1_IRQ_Handler
decl_isr UART0_IRQ_Handler
decl_isr UART1_IRQ_Handler
decl_isr ADC_IRQ_FIFO_Handler
decl_isr I2C0_IRQ_Handler
decl_isr I2C1_IRQ_Handler
decl_isr RTC_IRQ_Handler
#elif PICO_RP2350
decl_isr TIMER0_IRQ_0_Handler
decl_isr TIMER0_IRQ_1_Handler
decl_isr TIMER0_IRQ_2_Handler
decl_isr TIMER0_IRQ_3_Handler
decl_isr TIMER1_IRQ_0_Handler
decl_isr TIMER1_IRQ_1_Handler
decl_isr TIMER1_IRQ_2_Handler
decl_isr TIMER1_IRQ_3_Handler
decl_isr PWM_IRQ_WRAP_0_Handler
decl_isr PWM_IRQ_WRAP_1_Handler
decl_isr DMA_IRQ_0_Handler
decl_isr DMA_IRQ_1_Handler
decl_isr DMA_IRQ_2_Handler
decl_isr DMA_IRQ_3_Handler
decl_isr USBCTRL_IRQ_Handler
decl_isr PIO0_IRQ_0_Handler
decl_isr PIO0_IRQ_1_Handler
decl_isr PIO1_IRQ_0_Handler
decl_isr PIO1_IRQ_1_Handler
decl_isr PIO2_IRQ_0_Handler
decl_isr PIO2_IRQ_1_Handler
decl_isr IO_IRQ_BANK0_Handler
decl_isr IO_IRQ_BANK0_NS_Handler
decl_isr IO_IRQ_QSPI_Handler
decl_isr IO_IRQ_QSPI_NS_Handler
decl_isr SIO_IRQ_FIFO_Handler
decl_isr SIO_IRQ_BELL_Handler
decl_isr SIO_IRQ_FIFO_NS_Handler
decl_isr SIO_IRQ_BELL_NS_Handler
decl_isr SIO_IRQ_MTIMECMP_Handler
decl_isr CLOCKS_IRQ_Handler
decl_isr SPI0_IRQ_Handler
decl_isr SPI1_IRQ_Handler
decl_isr UART0_IRQ_Handler
decl_isr UART1_IRQ_Handler
decl_isr ADC_IRQ_FIFO_Handler
decl_isr I2C0_IRQ_Handler
decl_isr I2C1_IRQ_Handler
decl_isr OTP_IRQ_Handler
decl_isr TRNG_IRQ_Handler
decl_isr PLL_SYS_IRQ_Handler
decl_isr PLL_USB_IRQ_Handler
decl_isr POWMAN_IRQ_POW_Handler
decl_isr POWMAN_IRQ_TIMER_Handler
#else
#error Unknown processor, can't declare IRQs!
#endif

#if !PICO_RP2040
// since these are disabled by default, map them all to __unhandled_user_irq (will have
// a negative number
decl_isr MemManage_Handler
decl_isr BusFault_Handler
decl_isr UsageFault_Handler
decl_isr SecureFault_Handler
decl_isr DebugMon_Handler
#endif

// All unhandled USER IRQs fall through to here.
// Additionally, if the Armv9-M MemManage/BusFault/UsageFault/SecureFault/DebugMonitor exceptions
// are enabled, but the handlers are not defined, then unhandled_user_irq_num_in_r0 will
// also be reached, but with a negative exception number (e.g. MemManage == -12)
.global __unhandled_user_irq
.thumb_func
__unhandled_user_irq:
// if we include the implementation if there could be a valid IRQ hanler in the vtable that uses it
#if !(PICO_NO_RAM_VECTOR_TABLE && PICO_MINIMAL_STORED_VECTOR_TABLE)
    mrs  r0, ipsr
    subs r0, #16
.global unhandled_user_irq_num_in_r0
unhandled_user_irq_num_in_r0:
#endif
    // note the next instruction is a breakpoint too, however we have a 2 byte alignment hole
    // and it is preferrable to have distinct labels, to inform the user what has happened in the debugger.
    bkpt #0

decl_isr_bkpt isr_invalid
// these are separated out for clarity
#if !PICO_MINIMAL_STORED_VECTOR_TABLE
// these are separated out into individual BKPT instructions with label for clarity
decl_isr_bkpt NMI_Handler
decl_isr_bkpt HardFault_Handler
decl_isr_bkpt SVC_Handler
decl_isr_bkpt PendSV_Handler
decl_isr_bkpt SysTick_Handler
#endif

.global __default_isrs_end
__default_isrs_end:

// ----------------------------------------------------------------------------

.section .binary_info_header, "a"

// Header must be in first 256 bytes of main image (i.e. excluding flash boot2).
// For flash builds we put it immediately after vector table; for NO_FLASH the
// vectors are at a +0x100 offset because the bootrom enters RAM images directly
// at their lowest address, so we put the header in the VTOR alignment hole.

#if !PICO_NO_BINARY_INFO
binary_info_header:
.word BINARY_INFO_MARKER_START
.word __binary_info_start
.word __binary_info_end
.word data_cpy_table // we may need to decode pointers that are in RAM at runtime.
.word BINARY_INFO_MARKER_END
#endif

#include "embedded_start_block.inc.S"

// ----------------------------------------------------------------------------

#if !PICO_CRT0_NO_RESET_SECTION
.section .reset, "ax"

// On flash builds, the vector table comes first in the image (conventional).
// On NO_FLASH builds, the reset handler section comes first, as the entry
// point is at offset 0 (fixed due to bootrom), and VTOR is highly-aligned.
// Image is entered in various ways:
//
// - NO_FLASH builds are entered from beginning by UF2 bootloader
//
// - Flash builds vector through the table into _reset_handler from boot2
//
// - Either type can be entered via _entry_point by the debugger, and flash builds
//   must then be sent back round the boot sequence to properly initialise flash

// ELF entry point:
.type _entry_point,%function
.thumb_func
.global _entry_point
_entry_point:

#if PICO_NO_FLASH
    // on the NO_FLASH case, we do not do a rest thru bootrom below, so the RCP may or may not have been initialized:
    //
    // in the normal (e.g. UF2 download etc. case) we will have passed thru bootrom initialization, but if
    // a NO_FLASH binary is loaded by the debugger, and run directly after a reset, then we won't have.
    //
    // we must therefore initialize the RCP if it hasn't already been

#if HAS_REDUNDANCY_COPROCESSOR
    // just enable the RCP which is fine if it already was (we assume no other co-processors are enabled at this point to save space)
    ldr r0, = PPB_BASE + M33_CPACR_OFFSET
    movs r1, #ARM_CPU_PREFIXED(CPACR_CP7_BITS)
    str r1, [r0]
    // only initialize canary seeds if they haven't been (as to do so twice is a fault)
    mrc p7, #1, apsr_nzcv, c0, c0, #0
    bmi 1f
    // i dont think it much matters what we initialized to, as to have gotten here we must have not
    // gone thru the bootrom (which a secure boot would have)
    mcrr p7, #8, r0, r0, c0
    mcrr p7, #8, r0, r0, c1
    sev
1:
#endif
#if !__ARM_ARCH_6M__
    // Make sure stack limit is 0 if we came in thru the debugger; we do not know what it should be
    movs r0, #0
    msr msplim, r0
#endif

    ldr r0, =__vectors
    // Vector through our own table (SP, VTOR will not have been set up at
    // this point). Same path for debugger entry and bootloader entry.
    ldr r0, =__vectors
#else
    // Debugger tried to run code after loading, so SSI is in 03h-only mode.
    // Go back through bootrom + boot2 to properly initialise flash.
    ldr r0, =BOOTROM_VTABLE_OFFSET
#endif

_enter_vtable_in_r0:
    ldr r1, =(PPB_BASE + ARM_CPU_PREFIXED(VTOR_OFFSET))
    str r0, [r1]
    ldmia r0!, {r1, r2}
    msr msp, r1
    bx r2

// Reset handler:
// - initialises .data
// - clears .bss
// - calls runtime_init
// - calls main
// - calls exit (which should eventually hang the processor via _exit)

.type _reset_handler,%function
.thumb_func
_reset_handler:
    // Note if we entered thru here on core 0, then we should have gone thru bootrom, so SP (and MSPLIM) on Armv8-M
    // should already be set

    // Only core 0 should run the C runtime startup code; core 1 is normally
    // sleeping in the bootrom at this point but check to be sure (e.g. if
    // debugger put core 1 at the ELF entry point for some reason)
    ldr r0, =(SIO_BASE + SIO_CPUID_OFFSET)
    ldr r0, [r0]
#if __ARM_ARCH_6M__
    cmp r0, #0
    beq 1f
#else
    cbz r0, 1f
#endif
hold_non_core0_in_bootrom:
    // Send back to the ROM to wait for core 0 to launch it.
    ldr r0, =BOOTROM_VTABLE_OFFSET
    b _enter_vtable_in_r0
1:

#if !PICO_RP2040 && PICO_EMBED_XIP_SETUP && !PICO_NO_FLASH
    // Execute boot2 on the core 0 stack (it also gets copied into BOOTRAM due
    // to inclusion in the data copy table below). Note the reference
    // to __boot2_entry_point here is what prevents the .boot2 section from
    // being garbage-collected.
_copy_xip_setup:
    ldr r1, =__boot2_entry_point
    mov r3, sp
    add sp, #-256
    mov r2, sp
    bl data_cpy
_call_xip_setup:
    mov r0, sp
    adds r0, #1
    blx r0
    add sp, #256
#endif

    // In a NO_FLASH binary, don't perform .data etc copy, since it's loaded
    // in-place by the SRAM load. Still need to clear .bss
#if !PICO_NO_FLASH
    adr r4, data_cpy_table

    // assume there is at least one entry
1:
    ldmia r4!, {r1-r3}
    cmp r1, #0
    beq 2f
    bl data_cpy
    b 1b
2:
#endif

    // Zero out the BSS
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__
    movs r0, #0
    b bss_fill_test
bss_fill_loop:
    stm r1!, {r0}
bss_fill_test:
    cmp r1, r2
    bne bss_fill_loop

platform_entry: // symbol for stack traces
    // Use 32-bit jumps, in case these symbols are moved out of branch range
    // (e.g. if main is in SRAM and crt0 in flash)
    // Mbed OS: Changed this to call _start() instead of runtime_init() and main()
    ldr r1, =_start
    blx r1
    ldr r1, =exit
    blx r1
    // exit should not return.  If it does, hang the core.
    // (fall thru into our hang _exit impl
1: // separate label because _exit can be moved out of branch range
    bkpt #0
    b 1b


#if !PICO_NO_FLASH
data_cpy_loop:
    ldm r1!, {r0}
    stm r2!, {r0}
data_cpy:
    cmp r2, r3
    blo data_cpy_loop
    bx lr
#endif

// Note the data copy table is still included for NO_FLASH builds, even though
// we skip the copy, because it is listed in binary info

.p2align 2
data_cpy_table:
#if PICO_RP2350 && PICO_EMBED_XIP_SETUP && !PICO_NO_FLASH
.word __boot2_start__
.word BOOTRAM_BASE
.word BOOTRAM_BASE + 256
#endif

#if PICO_COPY_TO_RAM
.word __ram_text_source__
.word __ram_text_start__
.word __ram_text_end__
#endif
.word __etext
.word __data_start__
.word __data_end__

.word __scratch_x_source__
.word __scratch_x_start__
.word __scratch_x_end__

.word __scratch_y_source__
.word __scratch_y_start__
.word __scratch_y_end__

.word 0 // null terminator

// ----------------------------------------------------------------------------
// Provide safe defaults for _exit and runtime_init
// Full implementations usually provided by platform.c

.weak runtime_init
.type runtime_init,%function
.thumb_func
runtime_init:
    bx lr

#endif // PICO_CRT0_NO_RESET_SECTION

#include "embedded_end_block.inc.S"

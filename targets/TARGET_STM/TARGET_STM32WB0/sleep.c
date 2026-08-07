/* Copyright (c) 2026 MbedCE Community Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

#if DEVICE_SLEEP

#include "sleep_api.h"
#include "mbed_critical.h"
#include "us_ticker_data.h"

extern void save_timer_ctx(void);
extern void restore_timer_ctx(void);
extern void init_16bit_timer(void);
extern int serial_is_tx_ongoing(void);
extern void serial_restore_stdio(void);
extern void SetSysClock(void);
extern int mbed_sdk_inited;

#define RETAINED_CONTEXT __attribute__((section(".noinit"), used))

static uint32_t saved_vtor RETAINED_CONTEXT;
static uint32_t saved_nvic_iser RETAINED_CONTEXT;
static uint32_t saved_nvic_ipr[8] RETAINED_CONTEXT;
static uint32_t saved_shpr3 RETAINED_CONTEXT;
static uint32_t saved_systick_ctrl RETAINED_CONTEXT;
static uint32_t saved_systick_load RETAINED_CONTEXT;
static uint32_t saved_ahbenr RETAINED_CONTEXT;
static uint32_t saved_apb1enr RETAINED_CONTEXT;

__attribute__((naked)) void CPUcontextSave(void)
{
    __asm volatile(
        "mrs r2, control\n"
        "mrs r1, psp\n"
        "movs r0, #0\n"
        "msr control, r0\n"
        "isb\n"
        "push {r4-r7, lr}\n"
        "mov r3, r8\n"
        "mov r4, r9\n"
        "mov r5, r10\n"
        "mov r6, r11\n"
        "mov r7, r12\n"
        "push {r3-r7}\n"
        "ldr r4, =RAM_VR\n"
        "mrs r3, msp\n"
        "str r3, [r4, #4]\n"
        "push {r1, r2}\n"
        "dsb\n"
        "wfi\n"
        "nop\n"
        "nop\n"
        "b CPUcontextRestore\n"
    );
}

__attribute__((naked)) void CPUcontextRestore(void)
{
    __asm volatile(
        "cpsid i\n"
        "ldr r4, =RAM_VR\n"
        "ldr r4, [r4, #4]\n"
        "msr msp, r4\n"
        "sub sp, #8\n"
        "pop {r0, r1}\n"
        "pop {r3-r7}\n"
        "mov r8, r3\n"
        "mov r9, r4\n"
        "mov r10, r5\n"
        "mov r11, r6\n"
        "mov r12, r7\n"
        "pop {r4-r7}\n"
        "pop {r2}\n"
        "msr psp, r0\n"
        "msr control, r1\n"
        "isb\n"
        "bx r2\n"
    );
}

static void save_device_context(void)
{
    saved_vtor = SCB->VTOR;
    saved_nvic_iser = NVIC->ISER[0];
    for (uint32_t index = 0; index < 8; index++) {
        saved_nvic_ipr[index] = NVIC->IPR[index];
    }
    saved_shpr3 = SCB->SHPR[1];
    saved_systick_ctrl = SysTick->CTRL;
    saved_systick_load = SysTick->LOAD;
    saved_ahbenr = RCC->AHBENR;
    saved_apb1enr = RCC->APB1ENR;
}

static void restore_device_context(void)
{
    SCB->VTOR = saved_vtor;
    NVIC->ISER[0] = saved_nvic_iser;
    for (uint32_t index = 0; index < 8; index++) {
        NVIC->IPR[index] = saved_nvic_ipr[index];
    }
    SCB->SHPR[1] = saved_shpr3;
    SysTick->LOAD = saved_systick_load;
    SysTick->VAL = 0;
    SysTick->CTRL = saved_systick_ctrl;
    RCC->AHBENR |= saved_ahbenr;
    RCC->APB1ENR |= saved_apb1enr;
    __DSB();
    __ISB();
}

void hal_sleep(void)
{
    core_util_critical_section_enter();
    HAL_PWR_EnterSLEEPMode();
    core_util_critical_section_exit();
}

void hal_deepsleep(void)
{
    if (serial_is_tx_ongoing()) {
        return;
    }

    core_util_critical_section_enter();

    save_timer_ctx();
    save_device_context();
    mbed_sdk_inited = 0;

    PWR_DEEPSTOPTypeDef config = {0};
    config.deepStopMode = PWR_DEEPSTOP_WITH_SLOW_CLOCK_ON;
    HAL_PWR_ConfigDEEPSTOP(&config);
    LL_PWR_EnableInternWU();
    LL_PWR_ClearWakeupSource(LL_PWR_WAKEUP_ALL);
    LL_PWR_SetPowerMode(LL_PWR_MODE_DEEPSTOP);
    SET_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
    CPUcontextSave();

    CLEAR_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
    SetSysClock();
    restore_device_context();
    serial_restore_stdio();
    init_16bit_timer();
    restore_timer_ctx();
#if defined(PWR_CR2_GPIORET)
    LL_PWR_DisableGPIORET();
#endif
    mbed_sdk_inited = 1;

    core_util_critical_section_exit();
}

#endif

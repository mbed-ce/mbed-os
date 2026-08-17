/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#if DEVICE_SLEEP

#include "sleep_api.h"
#include "gpio_low_power.h"
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
#define CSTACK_PREAMBLE_WORDS 20U
#define GPIO_PORT_COUNT       2U

/* Deepstop retains the SRAM banks in place. These variables hold copies of
 * CPU and peripheral registers needed to resume Mbed after the reset-style
 * wake. The .noinit section prevents startup from modifying those copies.
 */
static uint32_t saved_vtor RETAINED_CONTEXT;
static uint32_t saved_nvic_iser RETAINED_CONTEXT;
static uint32_t saved_nvic_ipr[8] RETAINED_CONTEXT;
static uint32_t saved_shpr3 RETAINED_CONTEXT;
static uint32_t saved_systick_ctrl RETAINED_CONTEXT;
static uint32_t saved_systick_load RETAINED_CONTEXT;
static uint32_t saved_ahbenr RETAINED_CONTEXT;
static uint32_t saved_apb1enr RETAINED_CONTEXT;

/* Keep the CPU frame outside both runtime stacks. In an RTOS build, thread
 * code runs on PSP while MSP points at the reset/exception stack. The
 * reset-style Deepstop wake uses that MSP stack before SystemInit() calls
 * CPUcontextRestore(), so a frame stored there would be overwritten.
 */
static uint32_t saved_cpu_context[12] RETAINED_CONTEXT;

/* The reset-style wake path temporarily uses the initial stack at the top of
 * SRAM. Preserve the part it uses so the suspended Mbed stack is not damaged.
 */
static uint32_t saved_cstack_preamble[CSTACK_PREAMBLE_WORDS] RETAINED_CONTEXT;

/* Keep an explicit copy of the Mbed GPIO configuration across the wake path. */
typedef struct {
    uint32_t moder;
    uint32_t otyper;
    uint32_t ospeedr;
    uint32_t pupdr;
    uint32_t odr;
    uint32_t afr[2];
} gpio_context_t;

static gpio_context_t saved_gpio[GPIO_PORT_COUNT] RETAINED_CONTEXT;
static uint32_t saved_syscfg_io_dtr RETAINED_CONTEXT;
static uint32_t saved_syscfg_io_iber RETAINED_CONTEXT;
static uint32_t saved_syscfg_io_ievr RETAINED_CONTEXT;
static uint32_t saved_syscfg_io_ier RETAINED_CONTEXT;

/** Return the GPIO register block for one of the two STM32WB0 GPIO ports. */
static GPIO_TypeDef *gpio_port(uint32_t port)
{
    return port == 0U ? GPIOA : GPIOB;
}

/** Back up the top of the initial C stack before entering Deepstop. */
static void save_cstack_preamble(void)
{
    const uint32_t *stack = (const uint32_t *)(*(const uint32_t *)SCB->VTOR);

    stack -= CSTACK_PREAMBLE_WORDS;
    for (uint32_t index = 0U; index < CSTACK_PREAMBLE_WORDS; index++) {
        saved_cstack_preamble[index] = stack[index];
    }
}

/** Restore stack data overwritten by the reset-style Deepstop wake path. */
static void restore_cstack_preamble(void)
{
    uint32_t *stack = (uint32_t *)(*(const uint32_t *)saved_vtor);

    stack -= CSTACK_PREAMBLE_WORDS;
    for (uint32_t index = 0U; index < CSTACK_PREAMBLE_WORDS; index++) {
        stack[index] = saved_cstack_preamble[index];
    }
}

/** Save the CPU state and enter Deepstop with WFI.
 *
 * Callee-saved registers, the return address, both stack pointers and CONTROL
 * are copied to dedicated retained storage. On wake, CPUcontextRestore() makes
 * this function return normally to its caller, as if execution had only
 * paused at WFI.
 */
__attribute__((naked)) void CPUcontextSave(void)
{
    __asm volatile(
        "ldr r0, =saved_cpu_context\n"
        "str r4, [r0, #0]\n"
        "str r5, [r0, #4]\n"
        "str r6, [r0, #8]\n"
        "str r7, [r0, #12]\n"
        "mov r1, r8\n"
        "str r1, [r0, #16]\n"
        "mov r1, r9\n"
        "str r1, [r0, #20]\n"
        "mov r1, r10\n"
        "str r1, [r0, #24]\n"
        "mov r1, r11\n"
        "str r1, [r0, #28]\n"
        "mov r1, lr\n"
        "str r1, [r0, #32]\n"
        "mrs r1, psp\n"
        "str r1, [r0, #36]\n"
        "mrs r1, control\n"
        "str r1, [r0, #40]\n"
        "mrs r1, msp\n"
        "str r1, [r0, #44]\n"
        "ldr r0, =RAM_VR\n"
        "str r1, [r0, #4]\n"
        "bl HAL_PWR_EnterDEEPSTOPMode\n"
        "b CPUcontextRestore\n"
    );
}

/** Resume the CPU context saved by CPUcontextSave().
 *
 * ST's SystemInit() calls this function when it detects a Deepstop wake. It
 * restores both stack pointers and the callee-saved registers, then branches
 * to the saved return address using the original MSP/PSP selection.
 */
__attribute__((naked)) void CPUcontextRestore(void)
{
    __asm volatile(
        "cpsid i\n"
        "ldr r0, =saved_cpu_context\n"
        "ldr r1, [r0, #44]\n"
        "msr msp, r1\n"
        "ldr r1, [r0, #36]\n"
        "msr psp, r1\n"
        "ldr r4, [r0, #0]\n"
        "ldr r5, [r0, #4]\n"
        "ldr r6, [r0, #8]\n"
        "ldr r7, [r0, #12]\n"
        "ldr r1, [r0, #16]\n"
        "mov r8, r1\n"
        "ldr r1, [r0, #20]\n"
        "mov r9, r1\n"
        "ldr r1, [r0, #24]\n"
        "mov r10, r1\n"
        "ldr r1, [r0, #28]\n"
        "mov r11, r1\n"
        "ldr r2, [r0, #32]\n"
        "ldr r1, [r0, #40]\n"
        "msr control, r1\n"
        "isb\n"
        "bx r2\n"
    );
}

/** Copy volatile MCU configuration registers into retained SRAM. */
static void save_device_context(void)
{
    saved_vtor = SCB->VTOR;
    save_cstack_preamble();
    saved_nvic_iser = NVIC->ISER[0];
    for (uint32_t index = 0U; index < 8U; index++) {
        saved_nvic_ipr[index] = NVIC->IPR[index];
    }
    saved_shpr3 = SCB->SHPR[1];
    saved_systick_ctrl = SysTick->CTRL;
    saved_systick_load = SysTick->LOAD;
    saved_ahbenr = RCC->AHBENR;
    saved_apb1enr = RCC->APB1ENR;

    for (uint32_t index = 0U; index < GPIO_PORT_COUNT; index++) {
        GPIO_TypeDef *gpio = gpio_port(index);
        saved_gpio[index].moder = gpio->MODER;
        saved_gpio[index].otyper = gpio->OTYPER;
        saved_gpio[index].ospeedr = gpio->OSPEEDR;
        saved_gpio[index].pupdr = gpio->PUPDR;
        saved_gpio[index].odr = gpio->ODR;
        saved_gpio[index].afr[0] = gpio->AFR[0];
        saved_gpio[index].afr[1] = gpio->AFR[1];
    }

    saved_syscfg_io_dtr = SYSCFG->IO_DTR;
    saved_syscfg_io_iber = SYSCFG->IO_IBER;
    saved_syscfg_io_ievr = SYSCFG->IO_IEVR;
    saved_syscfg_io_ier = SYSCFG->IO_IER;
}

/** Restore MCU configuration registers after clocks become available again. */
static void restore_device_context(void)
{
    /* Enable the peripheral clocks before accessing GPIO and SYSCFG. */
    RCC->AHBENR |= saved_ahbenr;
    RCC->APB1ENR |= saved_apb1enr;

    /* Restore output values before GPIO modes to avoid visible output glitches. */
    for (uint32_t index = 0U; index < GPIO_PORT_COUNT; index++) {
        GPIO_TypeDef *gpio = gpio_port(index);
        gpio->ODR = saved_gpio[index].odr;
        gpio->OTYPER = saved_gpio[index].otyper;
        gpio->OSPEEDR = saved_gpio[index].ospeedr;
        gpio->PUPDR = saved_gpio[index].pupdr;
        gpio->AFR[0] = saved_gpio[index].afr[0];
        gpio->AFR[1] = saved_gpio[index].afr[1];
        gpio->MODER = saved_gpio[index].moder;
    }

    /* Restore InterruptIn edge selection, clear stale events, then re-enable
     * the configured GPIO interrupt sources.
     */
    SYSCFG->IO_IER = 0;
    SYSCFG->IO_DTR = saved_syscfg_io_dtr;
    SYSCFG->IO_IBER = saved_syscfg_io_iber;
    SYSCFG->IO_IEVR = saved_syscfg_io_ievr;
    SYSCFG->IO_ISCR = UINT32_MAX;
    SYSCFG->IO_IER = saved_syscfg_io_ier;

    /* Restore the vector table, interrupt priorities, and system tick. */
    SCB->VTOR = saved_vtor;
    NVIC->ISER[0] = saved_nvic_iser;
    for (uint32_t index = 0U; index < 8U; index++) {
        NVIC->IPR[index] = saved_nvic_ipr[index];
    }
    SCB->SHPR[1] = saved_shpr3;
    SysTick->LOAD = saved_systick_load;
    SysTick->VAL = 0;
    SysTick->CTRL = saved_systick_ctrl;
    __DSB();
    __ISB();
}

/** Enter ordinary CPU sleep without switching off the main power domain. */
void hal_sleep(void)
{
    core_util_critical_section_enter();
    HAL_PWR_EnterSLEEPMode();
    core_util_critical_section_exit();
}

/** Enter STM32WB0 Deepstop and resume the suspended Mbed execution context. */
void hal_deepsleep(void)
{
    /* Do not stop the peripheral while a serial transmission is active. */
    if (serial_is_tx_ongoing()) {
        return;
    }

    core_util_critical_section_enter();

    /* Save Mbed timer state and MCU registers before their power domain is
     * switched off. SRAM contents themselves remain in their original banks.
     */
    save_timer_ctx();
    save_device_context();
    mbed_sdk_inited = 0;

    /* Apply GPIO pull settings required by the selected board in Deepstop. */
    gpio_deep_sleep_prepare();

    /* Keep the low-speed clock running for the Mbed low-power ticker. */
    PWR_DEEPSTOPTypeDef config = {0};
    config.deepStopMode = PWR_DEEPSTOP_WITH_SLOW_CLOCK_ON;

    /* Disable debug retention modes before entering low-power Deepstop. */
    LL_PWR_DisableDEEPSTOP2();
#if defined(PWR_CR2_DBGRET)
    LL_PWR_DisableDBGRET();
#endif
    /* Follow ST's PWR_DEEPSTOP_RTC entry sequence.  Deactivating the RTC
     * wake-up timer first clears RTC_ISR.WUTF and removes the internal wake
     * level.  The mirrored PWR flag can then be cleared before the timer is
     * re-enabled.  Clearing only the PWR flag while WUTF is still active does
     * not reliably re-arm the edge detector on STM32WB0.
     */
    RTC_HandleTypeDef rtc_handle = {0};
    uint32_t wake_counter = RTC->WUTR;
    uint32_t wake_clock = RTC->CR & RTC_CR_WUCKSEL;

    rtc_handle.Instance = RTC;
    HAL_RTCEx_DeactivateWakeUpTimer(&rtc_handle);
    __HAL_PWR_CLEAR_FLAG(PWR_WU_FLAG_ALL);
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN_RTC, PWR_WUP_RISIEDG);
    HAL_RTCEx_SetWakeUpTimer_IT(&rtc_handle, wake_counter, wake_clock);

    HAL_PWR_ConfigDEEPSTOP(&config);

    /* This call returns only after the reset handler enters SystemInit(),
     * SystemInit() calls CPUcontextRestore(), and the saved CPU frame is loaded.
     */
    CPUcontextSave();

    /* Repair stack data used temporarily by the reset startup code. */
    restore_cstack_preamble();

    /* Return the clocks and peripherals to their pre-Deepstop state. */
    CLEAR_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
    SetSysClock();
    restore_device_context();

    /* Recreate Mbed drivers whose hardware state is not covered by the generic
     * register snapshot, then restore the low-power ticker schedule.
     */
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

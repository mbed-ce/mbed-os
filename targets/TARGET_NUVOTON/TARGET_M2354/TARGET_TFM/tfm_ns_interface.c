/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <stdbool.h>

#include "cmsis.h"
#include "tfm_api.h"
#include "tfm_ns_interface.h"
#include "cmsis_os2.h"

#include "mbed_critical.h"
#include "mbed_assert.h"
#include "mbed_error.h"

/* Approach to serialization of NS secure calls required by TF-M secure world
 *
 * Default implementation of NS interface uses mutex to meet the requirement,
 * but it cannot support NS secure call in interrupt-disabled context. Instead,
 * in this override, NS secure call is guaranteed to be non-preemptive during
 * this period by locking kernel scheduler. Apparently, this approach has one
 * weakness: all other threads are also locked during this period. Until there's
 * a better approach coming out, we can just use this expedient one.
 *
 * For the 'lock kernel scheduler' approach to work thoroughly, we must also
 * address some side issues:
 *
 * - NOT SUPPORT NS secure call from ISR except SVC (rtos/baremetal)
 * - Support NS secure call in SVC context conditionally (rtos/baremetal)
 *   - For rtos profile, lock interrupt instead of kernel scheduler because
 *     svcRtxKernelLock()/svcRtxKernelRestoreLock(...) are inaccessible
 *     outside rtx_kernel.c. Currently, this is rare case and would cause
 *     little trouble (see known paths below).
 *   - For baremetal profile, NS secure call is guaranteed to be non-preemptive
 *     naturally.
 *   NOTE: However, per test, TF-M doesn't allow call in SVC context anymore.
 *   It will trap this as error and possibly reboot the system, except for 
 *   secure context calls like TZ_InitContextSystem_S and firends.
 * - Call into secure world straight in interrupt-disabled context (rtos/baremetal)
 *   NS secure call is guaranteed to be non-preemptive naturally.
 * - Call into secure world straight at pre-rtos stage (rtos)
 *   NS secure call is guaranteed to be non-preemptive naturally.
 * - osKernelLock() will error when kernel state is 'osKernelSuspended' (rtos).
 *   Address it separately. Known path of NS secure call when kernel state is
 *   'osKernelSuspended':
 *   default idle thread > osKernelSuspend() > lp_ticker_init > SYS_ResetModule_S/
 *   CLK_SetModuleClock_S/CLK_EnableModuleClock_S
 *
 * Known paths of NS secure call in interrupt-disabled context:
 * - mbed-os/platform/mbed_sleep_manager.c > sleep_manager_sleep_auto >
 *   hal_sleep/hal_deepsleep > nu_idle_s/nu_powerdown_s
 * - mbed-os/hal/LowPowerTickerWrapper.cpp > LowPowerTickerWrapper::init > 
 *   lp_ticker_init > SYS_ResetModule_S/CLK_SetModuleClock_S/CLK_EnableModuleClock_S
 * - mbed-os/platform/mbed_board.c > mbed_die > nu_pin_function_s
 * - mbed-os-tests-mbed_hal-rtc > rtc_write_read_test > rtc_write >
 *   CLK_IsRTCClockEnabled_S
 *
 * Known paths of NS secure call in SVC context:
 * - In MBED_TICKLESS mode, osKernelStart > svcRtxKernelStart > OS_Tick_Enable >
 *   us_ticker_init/lp_ticker_init > SYS_ResetModule_S/CLK_SetModuleClock_S/
 *   CLK_EnableModuleClock_S
 *   NOTE: Per above SVC test, this means MBED_TICKLESS mode is not supported.
 */

struct ns_interface_state
{
    bool        init;       // Initialized or not
};

static struct ns_interface_state ns_interface = {
    .init = false
};

/* Override tfm_ns_interface_dispatch(...) */
int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
    /* Prohibit NS secure call from ISR except SVC, so non-preemptive doesn't break */
    uint32_t ipsr = __get_IPSR();
    if (ipsr == 11U) {
        /* Support NS secure call in SVC context */
#if MBED_CONF_RTOS_PRESENT
        /*
         * Lock interrupt instead of kernel scheduler because svcRtxKernelLock()/
         * svcRtxKernelRestoreLock(...) are inaccessible outside rtx_kernel.c
         */
        core_util_critical_section_enter();
        int32_t result = fn(arg0, arg1, arg2, arg3);
        core_util_critical_section_exit();

        return result;
#else
        /*
         * Call into secure world straight for baremetal because NS secure
         * call is non-preemptive naturally
         */
        return fn(arg0, arg1, arg2, arg3);
#endif
    } else if (ipsr) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_PROHIBITED_IN_ISR_CONTEXT), "Prohibited in ISR context", (uintptr_t) fn);
    }

    /*
     * Call into secure world straight in interrupt-disabled context because
     * NS secure call is non-preemptive naturally
     */
    if (!core_util_are_interrupts_enabled()) {
        return fn(arg0, arg1, arg2, arg3);
    }

#if MBED_CONF_RTOS_PRESENT
    osKernelState_t kernel_state = osKernelGetState();

    /*
     * Call into secure world straight at pre-rtos stage because NS secure
     * call is non-preemptive naturally
     */
    if (kernel_state == osKernelInactive || kernel_state == osKernelReady) {
        return fn(arg0, arg1, arg2, arg3);
    }

    /*
     * osKernelLock() will error when kernel state is 'osKernelSuspended'. Address
     * it separately.
     */
    if (kernel_state == osKernelSuspended) {
        return fn(arg0, arg1, arg2, arg3);
    } else if (kernel_state == osKernelError) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_UNKNOWN), "RTX kernel state error", (uintptr_t) fn);
    }

    /* Lock kernel scheduler and save previous lock state for restore */
    int32_t lock_state = osKernelLock();
    if (lock_state == osError) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_UNKNOWN), "Unknown RTX error", (uintptr_t) fn);
    }
    MBED_ASSERT(lock_state >= 0);

    /* NS secure call is non-preemptive because kernel scheduler is locked */
    int32_t result = fn(arg0, arg1, arg2, arg3);

    /* Restore previous lock state */
    lock_state = osKernelRestoreLock(lock_state);
    if (lock_state == osError) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_UNKNOWN), "Unknown RTX error", (uintptr_t) fn);
    }
    MBED_ASSERT(lock_state >= 0);

    return result;
#else
    /*
     * Call into secure world straight for baremetal because NS secure
     * call is non-preemptive naturally
     */
    return fn(arg0, arg1, arg2, arg3);
#endif
}

/* Override tfm_ns_lock_init()
 *
 * Since Mbed OS 6, this function is called at the very start of main thread (mbed_start),
 * not before scheduler is started, so we cannot use this call to distinguish pre-rtos stage
 * and rtos stage in tfm_ns_interface_dispatch() as before. Instead, we use osKernelGetState()
 * to check if scheduler has started.
 */
enum tfm_status_e tfm_ns_interface_init(void)
{
    if (!ns_interface.init) {
        ns_interface.init = true;
    }

    return TFM_SUCCESS;
}

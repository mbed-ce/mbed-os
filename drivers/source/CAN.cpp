/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#include "drivers/CAN.h"

#if DEVICE_CAN

#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_error.h"

namespace mbed {

CAN::CAN(PinName rd, PinName td) : _can(), _irq()
{
    // No lock needed in constructor
    can_init(&_can, rd, td);
    can_irq_init(&_can, (&CAN::_irq_handler), reinterpret_cast<uintptr_t>(this));
}

CAN::CAN(PinName rd, PinName td, int hz, int data_hz) : _can(), _irq()
{
    // No lock needed in constructor
#if DEVICE_CAN_FD
    can_init_freq(&_can, rd, td, hz, data_hz);
#else
    MBED_ASSERT(data_hz == 0);
    can_init_freq(&_can, rd, td, hz);
#endif
    can_irq_init(&_can, (&CAN::_irq_handler), reinterpret_cast<uintptr_t>(this));
}

CAN::CAN(const can_pinmap_t &pinmap) : _can(), _irq()
{
    // No lock needed in constructor
    can_init_direct(&_can, &pinmap);
    can_irq_init(&_can, (&CAN::_irq_handler), reinterpret_cast<uintptr_t>(this));
}

CAN::CAN(const can_pinmap_t &pinmap, int hz, int data_hz) : _can(), _irq()
{
    // No lock needed in constructor
#if DEVICE_CAN_FD
    can_init_freq_direct(&_can, &pinmap, hz, data_hz);
#else
    MBED_ASSERT(data_hz == 0);
    can_init_freq_direct(&_can, &pinmap, hz);
#endif
    can_irq_init(&_can, (&CAN::_irq_handler), reinterpret_cast<uintptr_t>(this));
}

CAN::~CAN()
{
    // No lock needed in destructor

    // Detaching interrupts releases the sleep lock if it was locked
    for (int irq = 0; irq < IrqType::IrqCnt; irq++) {
        attach(nullptr, (IrqType)irq);
    }
    can_irq_free(&_can);
    can_free(&_can);
}

int CAN::frequency(int f, int data_f)
{
    lock();
#if DEVICE_CAN_FD
    int ret = can_frequency(&_can, f, data_f);
#else
    MBED_ASSERT(data_f == 0);
    int ret = can_frequency(&_can, f);
#endif
    unlock();
    return ret;
}

int CAN::write(CANMessage msg)
{
    lock();
    int ret = can_write(&_can, msg);
    unlock();
    return ret;
}

int CAN::read(CANMessage &msg, int handle)
{
    lock();
    int ret = can_read(&_can, &msg, handle);
    if (msg.len > 8) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_CAN, MBED_ERROR_CODE_READ_FAILED), "Read tried to write more than 8 bytes");
    }
    unlock();
    return ret;
}

#if DEVICE_CAN_FD

int CAN::write(CANFDMessage msg)
{
    lock();
    int ret = canfd_write(&_can, msg);
    unlock();
    return ret;
}

int CAN::read(CANFDMessage &msg, int handle)
{
    lock();
    int ret = canfd_read(&_can, &msg, handle);
    if (msg.len > 64) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_CAN, MBED_ERROR_CODE_READ_FAILED), "Read tried to write more than 64 bytes");
    }
    unlock();
    return ret;
}

#endif

void CAN::reset()
{
    lock();
    can_reset(&_can);
    unlock();
}

unsigned char CAN::rderror()
{
    lock();
    int ret = can_rderror(&_can);
    unlock();
    return ret;
}

unsigned char CAN::tderror()
{
    lock();
    int ret = can_tderror(&_can);
    unlock();
    return ret;
}

void CAN::monitor(bool silent)
{
    lock();
    can_monitor(&_can, (silent) ? 1 : 0);
    unlock();
}

int CAN::mode(Mode mode)
{
    lock();
    int ret = can_mode(&_can, (CanMode)mode);
    unlock();
    return ret;
}

int CAN::filter(unsigned int id, unsigned int mask, CANFormat format, int handle)
{
    lock();
    int ret = can_filter(&_can, id, mask, format, handle);
    unlock();
    return ret;
}

void CAN::attach(Callback<void()> func, IrqType type)
{
    CAN::lock();
    if (func) {
        // lock deep sleep only the first time
        if (!_irq[(CanIrqType)type]) {
            sleep_manager_lock_deep_sleep();
        }
        _irq[(CanIrqType)type] = func;
        can_irq_set(&_can, (CanIrqType)type, 1);
    } else {
        // unlock deep sleep only the first time
        if (_irq[(CanIrqType)type]) {
            sleep_manager_unlock_deep_sleep();
        }
        _irq[(CanIrqType)type] = nullptr;
        can_irq_set(&_can, (CanIrqType)type, 0);
    }
    CAN::unlock();
}

void CAN::_irq_handler(uintptr_t context, CanIrqType type)
{
    CAN *handler = reinterpret_cast<CAN *>(context);
    if (handler->_irq[type]) {
        handler->_irq[type].call();
    }
}

void CAN::lock()
{
    _mutex.lock();
}

void CAN::unlock()
{
    _mutex.unlock();
}

} // namespace mbed

#endif

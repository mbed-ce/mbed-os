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
#ifndef MBED_TIMER_H
#define MBED_TIMER_H

#include "platform/platform.h"
#include "drivers/TickerDataClock.h"
#include "platform/NonCopyable.h"

namespace mbed {

class CriticalSectionLock;

/**
 * \file
 * \defgroup drivers_Timer Timer classes
 * \ingroup drivers-public-api-ticker
 * @{
 */

/**
 * @brief Base class for Timer and LowPowerTimer
 *
 * @note Synchronization level: Interrupt safe
 */
class TimerBase {

public:
    /**
     * @brief Start the timer.
     *
     * Will lock deep sleep if this is a \c Timer instance
     */
    void start();

    /**
     * @brief Stop the timer
     *
     * Will release the deep sleep lock if this is a \c Timer instance
     */
    void stop();

    /**
     * @brief Reset the timer to 0.
     *
     * If it was already running, it will continue
     */
    void reset();

    /**
     * @brief Get the time passed in seconds
     *
     *  @returns    Time passed in floating-point seconds
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Floating point operators should normally be avoided for code size. If really needed, you can use `duration<float>{elapsed_time()}.count()`")
    float read() const;

    /**
     * @brief Get the time passed in milliseconds
     *
     *  @returns    Time passed in integer milliseconds
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Use the Chrono-based elapsed_time method.  If integer milliseconds are needed, you can use `duration_cast<milliseconds>(elapsed_time()).count()`")
    int read_ms() const;

    /**
     * @brief Get the time passed in microseconds
     *
     * @returns Time passed in integer microseconds
     *
     * \warning Limited to 31 bits -- will not return correct values for times larger than about 35 minutes.
     *     Use #read_high_resolution_us or #elapsed_time instead as they are not vulnerable to this problem.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Use the Chrono-based elapsed_time method.  If integer microseconds are needed, you can use `elapsed_time().count()`")
    int read_us() const;

    /**
     * @brief An operator shorthand for read()
     *
     * @returns Time passed in floating-point seconds
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Floating point operators should normally be avoided for code size. If really needed, you can use `duration<float>{elapsed_time()}.count()`")
    operator float() const;

    /**
     * @brief Get (in a high resolution type) the time passed in microseconds.
     *
     * @returns Time passed as 64-bit microseconds.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Use the Chrono-based elapsed_time method.  If integer microseconds are needed, you can use `elapsed_time().count()`")
    us_timestamp_t read_high_resolution_us() const;

    /**
     * @brief Get the time passed as chrono microseconds.
     *
     * @returns Time passed \c as std::chrono::microseconds
     */
    std::chrono::microseconds elapsed_time() const;

#if !defined(DOXYGEN_ONLY)
protected:
    TimerBase(const ticker_data_t *data);
    TimerBase(const ticker_data_t *data, bool lock_deepsleep);
    TimerBase(const TimerBase &t);
    TimerBase(TimerBase &&t);
    ~TimerBase();

    const TimerBase &operator=(const TimerBase &) = delete;

    std::chrono::microseconds slicetime() const;
    TickerDataClock::time_point _start{};   // the start time of the latest slice
    std::chrono::microseconds _time{};    // any accumulated time from previous slices
    TickerDataClock _ticker_data;
    bool _lock_deepsleep;    // flag that indicates if deep sleep should be disabled
    bool _running = false;   // whether the timer is running

private:
    // Copy storage while a lock is held
    TimerBase(const TimerBase &t, const CriticalSectionLock &) : TimerBase(t, false) {}
    // Copy storage only - used by delegating constructors
    TimerBase(const TimerBase &t, bool) : _start(t._start), _time(t._time), _ticker_data(t._ticker_data), _lock_deepsleep(t._lock_deepsleep), _running(t._running) {}
#endif
};

/**
 * @brief Timer implementation using the us ticker.
 *
 * Locks deep sleep while actively running.
 *
 * Example:
 * \code{.cpp}
 * // Count the time to toggle an LED
 *
 * #include "mbed.h"
 * #include <cinttypes>
 *
 * Timer timer;
 * DigitalOut led(LED1);
 *
 * int main() {
 *     timer.start();
 *     const auto begin = timer.elapsed_time();
 *     led = !led;
 *     const auto end = timer.elapsed_time();
 *     printf("Toggling the led takes %" PRIi64 " us", (end - begin).count());
 * }
 * \endcode
 *
 * @note When constructed, a Timer is stopped and reset to zero time.
 *
 * @note Synchronization level: Interrupt safe
 */
class Timer : public TimerBase {
public:
    Timer();
};
/** @}*/

} // namespace mbed

#endif

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
#ifndef MBED_US_TICKER_DEFINES_H_
#define MBED_US_TICKER_DEFINES_H_

#include "us_ticker_data.h"

#if US_TICKER_CLOCK_BITS == 16
#define US_TICKER_MASK          0xFFFF
#elif US_TICKER_CLOCK_BITS == 32
#define US_TICKER_MASK          0xFFFFFFFF
#else
#error "Bad US_TICKER_CLOCK_BITS"
#endif

/* Macro-optimised form of us_ticker_read */
#define us_ticker_read() (US_TICKER_INSTANCE->GTCNT)

#endif /* MBED_US_TICKER_DEFINES_H_ */

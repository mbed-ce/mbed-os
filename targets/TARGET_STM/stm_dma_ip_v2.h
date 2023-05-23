/* mbed Microcontroller Library
 * Copyright (c) 2016-2023 STMicroelectronics
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

/*
 * This header contains constants and defines specific to processors with the v2 DMA IP.
 *
 * The v2 DMA IP has DMA controllers with multiple channels, where each channel has a request source
 * that determines what triggers DMA transactions
 */

#ifndef MBED_OS_STM_DMA_IP_V2_H
#define MBED_OS_STM_DMA_IP_V2_H

#ifdef TARGET_MCU_STM32F0

// STM32F0 is weird and does its own thing.
// Only 5 channels usable, the other 2 lack interrupts
#define MAX_DMA_CHANNELS_PER_CONTROLLER 5

#elif defined(TARGET_MCU_STM32G0)

// STM32G0 is weird and does its own thing.
// Only 3 channels usable, the other 4 lack interrupts
#define MAX_DMA_CHANNELS_PER_CONTROLLER 3

#else

// Determine max channels per DMA controller.
// We have to do this by just counting the macros.
#if defined(DMA1_Channel1) && defined(DMA1_Channel2) && defined(DMA1_Channel3) && defined(DMA1_Channel4) && defined(DMA1_Channel5) && defined(DMA1_Channel6) && defined(DMA1_Channel7)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 7
#elif defined(DMA1_Channel1) && defined(DMA1_Channel2) && defined(DMA1_Channel3) && defined(DMA1_Channel4) && defined(DMA1_Channel5) && defined(DMA1_Channel6)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 6
#elif defined(DMA1_Channel1) && defined(DMA1_Channel2) && defined(DMA1_Channel3) && defined(DMA1_Channel4) && defined(DMA1_Channel5)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 5
#elif defined(DMA1_Channel1) && defined(DMA1_Channel2) && defined(DMA1_Channel3) && defined(DMA1_Channel4)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 4
#elif defined(DMA1_Channel1) && defined(DMA1_Channel2) && defined(DMA1_Channel3)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 3
#elif defined(DMA1_Channel1) && defined(DMA1_Channel2)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 2
#else
#define MAX_DMA_CHANNELS_PER_CONTROLLER 1
#endif

#endif

#endif //MBED_OS_STM_DMA_IP_V2_H

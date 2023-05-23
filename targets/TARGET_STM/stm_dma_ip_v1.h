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
 * This header contains constants and defines specific to processors with the v1 DMA IP.
 * The v1 IP has DMA controllers with multiple streams, where each "stream" has a "channel selection"
 * to determine what triggers DMA requests.
 */

#ifndef MBED_OS_STM_DMA_IP_V1_H
#define MBED_OS_STM_DMA_IP_V1_H

// Determine max channels per DMA controller.
// We have to do this by just counting the macros.
#if defined(DMA1_Stream0) && defined(DMA1_Stream1) && defined(DMA1_Stream2) && defined(DMA1_Stream3) && defined(DMA1_Stream4) && defined(DMA1_Stream5) && defined(DMA1_Stream6) && defined(DMA1_Stream7)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 8
#elif defined(DMA1_Stream0) && defined(DMA1_Stream1) && defined(DMA1_Stream2) && defined(DMA1_Stream3) && defined(DMA1_Stream4) && defined(DMA1_Stream5) && defined(DMA1_Stream6)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 7
#elif defined(DMA1_Stream0) && defined(DMA1_Stream1) && defined(DMA1_Stream2) && defined(DMA1_Stream3) && defined(DMA1_Stream4) && defined(DMA1_Stream5)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 6
#elif defined(DMA1_Stream0) && defined(DMA1_Stream1) && defined(DMA1_Stream2) && defined(DMA1_Stream3) && defined(DMA1_Stream4)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 5
#elif defined(DMA1_Stream0) && defined(DMA1_Stream1) && defined(DMA1_Stream2) && defined(DMA1_Stream3)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 4
#elif defined(DMA1_Stream0) && defined(DMA1_Stream1) && defined(DMA1_Stream2)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 3
#elif defined(DMA1_Stream0) && defined(DMA1_Stream1)
#define MAX_DMA_CHANNELS_PER_CONTROLLER 2
#else
#define MAX_DMA_CHANNELS_PER_CONTROLLER 1
#endif

// Provide an alias so that code can always use the v2 name for this structure
#define DMA_Channel_TypeDef DMA_Stream_TypeDef

#endif //MBED_OS_STM_DMA_IP_V1_H

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

#include "stm_dma_utils.h"

#include "mbed_error.h"

#include <stdbool.h>
#include <malloc.h>
#include <string.h>

// Array to store pointer to DMA handle for each DMA channel.
// Note: arrays are 0-indexed, so DMA1 Channel2 is at stmDMAHandles[0][1].
static DMA_HandleTypeDef * stmDMAHandles[NUM_DMA_CONTROLLERS][NUM_DMA_CHANNELS_PER_CONTROLLER];

DMA_Channel_TypeDef * stm_get_dma_channel(const DMALinkInfo *dmaLink)
{
    switch(dmaLink->dmaIdx)
    {
#ifdef DMA1
        case 1:
            switch(dmaLink->channelIdx)
            {
#ifdef DMA1_Channel1
                case 1:
                    return DMA1_Channel1;
#endif
#ifdef DMA1_Channel2
                case 2:
                    return DMA1_Channel2;
#endif
#ifdef DMA1_Channel3
                case 3:
                    return DMA1_Channel3;
#endif
#ifdef DMA1_Channel4
                case 4:
                    return DMA1_Channel4;
#endif
#ifdef DMA1_Channel5
                case 5:
                    return DMA1_Channel5;
#endif
#ifdef DMA1_Channel6
                case 6:
                    return DMA1_Channel6;
#endif
#ifdef DMA1_Channel7
                case 7:
                    return DMA1_Channel7;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif

#ifdef DMA2
        case 2:
            switch(dmaLink->channelIdx)
            {
#ifdef DMA2_Channel1
                case 1:
                    return DMA2_Channel1;
#endif
#ifdef DMA2_Channel2
                case 2:
                    return DMA2_Channel2;
#endif
#ifdef DMA2_Channel3
                case 3:
                    return DMA2_Channel3;
#endif
#ifdef DMA2_Channel4
                case 4:
                    return DMA2_Channel4;
#endif
#ifdef DMA2_Channel5
                case 5:
                    return DMA2_Channel5;
#endif
#ifdef DMA2_Channel6
                case 6:
                    return DMA2_Channel6;
#endif
#ifdef DMA2_Channel7
                case 7:
                    return DMA2_Channel7;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif
        default:
            mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA controller", dmaLink->dmaIdx, MBED_FILENAME, __LINE__);

    }
}

DMA_HandleTypeDef *stm_init_dma_link(const DMALinkInfo *dmaLink, uint32_t direction, bool periphInc, bool memInc,
                                     uint32_t periphDataAlignment, uint32_t memDataAlignment){
     // Enable DMA mux clock for devices with it
#ifdef DMAMUX1
    __HAL_RCC_DMAMUX1_CLK_ENABLE()
#endif

    // Turn on clock for the DMA module
    switch(dmaLink->dmaIdx)
    {
#ifdef DMA1
        case 1:
            __HAL_RCC_DMA1_CLK_ENABLE();
            break;
#endif
#ifdef DMA2
        case 2:
            __HAL_RCC_DMA2_CLK_ENABLE();
            break;
#endif
        default:
            mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA controller", dmaLink->dmaIdx, MBED_FILENAME, __LINE__);
    }

    // Allocate DMA handle.
    // Yes it's a little gross that we have to allocate on the heap, but this structure uses quite a lot of memory,
    // so we don't want to allocate DMA handles until they're needed.
    DMA_HandleTypeDef * dmaHandle = malloc(sizeof(DMA_HandleTypeDef));
    memset(dmaHandle, 0, sizeof(DMA_HandleTypeDef));
    stmDMAHandles[dmaLink->dmaIdx - 1][dmaLink->channelIdx - 1] = dmaHandle;

    // Configure handle
    dmaHandle->Instance = stm_get_dma_channel(dmaLink);
    dmaHandle->Init.Request = dmaLink->sourceNumber;
    dmaHandle->Init.Direction = direction;
    dmaHandle->Init.PeriphInc = periphInc ? DMA_PINC_ENABLE : DMA_PINC_DISABLE;
    dmaHandle->Init.MemInc = memInc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE;
    dmaHandle->Init.PeriphDataAlignment = periphDataAlignment;
    dmaHandle->Init.MemDataAlignment = memDataAlignment;
    dmaHandle->Init.Mode = DMA_NORMAL;
    dmaHandle->Init.Priority = DMA_PRIORITY_MEDIUM;

    HAL_DMA_Init(dmaHandle);

    // TODO set up interrupt

    return dmaHandle;
}

#ifdef DMA1_Channel1
void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][0]);
}
#endif

// STM32F0 has shared ISRs for Ch2-Ch3 and Ch4-Ch5
#ifdef TARGET_MCU_STM32F0

void DMA1_Channel2_3_IRQHandler(void)
{
    if(stmDMAHandles[0][1] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][1]);
    }
    if(stmDMAHandles[0][2] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][2]);
    }
}

void DMA1_Channel4_5_IRQHandler(void)
{
    if(stmDMAHandles[0][3] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
    }
    if(stmDMAHandles[0][4] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
    }
}

#else

#ifdef DMA1_Channel2
void DMA1_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][1]);
}
#endif

#ifdef DMA1_Channel3
void DMA1_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][2]);
}
#endif

#ifdef DMA1_Channel4
void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
}
#endif

#ifdef DMA1_Channel5
void DMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
}
#endif

#endif

#ifdef DMA1_Channel6
void DMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][5]);
}
#endif

#ifdef DMA1_Channel7
void DMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][6]);
}
#endif

#ifdef DMA2_Channel1
void DMA2_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][0]);
}
#endif

#ifdef DMA2_Channel2
void DMA2_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][1]);
}
#endif

#ifdef DMA2_Channel3
void DMA2_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][2]);
}
#endif

#ifdef DMA2_Channel4
void DMA2_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][3]);
}
#endif

#ifdef DMA2_Channel5
void DMA2_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][4]);
}
#endif

#ifdef DMA2_Channel6
void DMA2_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][5]);
}
#endif

#ifdef DMA2_Channel7
void DMA2_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][6]);
}
#endif
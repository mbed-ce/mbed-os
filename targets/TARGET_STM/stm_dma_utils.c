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
static DMAHandlePointer stmDMAHandles[NUM_DMA_CONTROLLERS][MAX_DMA_CHANNELS_PER_CONTROLLER];

DMAInstancePointer stm_get_dma_instance(DMALinkInfo const * dmaLink)
{
    DMAInstancePointer dma_instance;
    switch(dmaLink->dmaIdx)
    {
#ifdef DMA1
        case 1:
            dma_instance.dma = DMA1;
            break;
#endif
#ifdef DMA2
        case 2:
            dma_instance.dma = DMA2;
            break;
#endif
#ifdef GPDMA1
        case 1:
            dma_instance.dma = GPDMA1;
            break;
#endif
#ifdef BDMA
        case 3:
            dma_instance.bdma = BDMA;
            break;
#endif
#ifdef MDMA
        case 4:
            dma_instance.mdma = MDMA;
            break;
#endif
        default:
            mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA controller", dmaLink->dmaIdx, MBED_FILENAME, __LINE__);
    }
    return dma_instance;
}

DMAChannelPointer stm_get_dma_channel(const DMALinkInfo *dmaLink)
{
    DMAChannelPointer channel_pointer;
    switch(dmaLink->dmaIdx)
    {
#ifdef DMA1
        case 1:
            switch(dmaLink->channelIdx)
            {
#ifdef DMA1_Channel1
                case 1:
                    channel_pointer.channel = DMA1_Channel1;
                    break;
#endif
#ifdef DMA1_Channel2
                case 2:
                    channel_pointer.channel = DMA1_Channel2;
                    break;
#endif
#ifdef DMA1_Channel3
                case 3:
                    channel_pointer.channel = DMA1_Channel3;
                    break;
#endif
#ifdef DMA1_Channel4
                case 4:
                    channel_pointer.channel = DMA1_Channel4;
                    break;
#endif
#ifdef DMA1_Channel5
                case 5:
                    channel_pointer.channel = DMA1_Channel5;
                    break;
#endif
#ifdef DMA1_Channel6
                case 6:
                    channel_pointer.channel = DMA1_Channel6;
                    break;
#endif
#ifdef DMA1_Channel7
                case 7:
                    channel_pointer.channel = DMA1_Channel7;
                    break;
#endif
#ifdef DMA1_Stream0
                case 0:
                    channel_pointer.channel = DMA1_Stream0;
                    break;
#endif
#ifdef DMA1_Stream1
                case 1:
                    channel_pointer.channel = DMA1_Stream1;
                    break;
#endif
#ifdef DMA1_Stream2
                case 2:
                    channel_pointer.channel = DMA1_Stream2;
                    break;
#endif
#ifdef DMA1_Stream3
                case 3:
                    channel_pointer.channel = DMA1_Stream3;
                    break;
#endif
#ifdef DMA1_Stream4
                case 4:
                    channel_pointer.channel = DMA1_Stream4;
                    break;
#endif
#ifdef DMA1_Stream5
                case 5:
                    channel_pointer.channel = DMA1_Stream5;
                    break;
#endif
#ifdef DMA1_Stream6
                case 6:
                    channel_pointer.channel = DMA1_Stream6;
                    break;
#endif
#ifdef DMA1_Stream7
                case 7:
                    channel_pointer.channel = DMA1_Stream7;
                    break;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
            break;
#endif
#ifdef DMA2
        case 2:
            switch(dmaLink->channelIdx)
            {
#ifdef DMA2_Channel1
                case 1:
                    channel_pointer.channel = DMA2_Channel1;
                    break;
#endif
#ifdef DMA2_Channel2
                case 2:
                    channel_pointer.channel = DMA2_Channel2;
                    break;
#endif
#ifdef DMA2_Channel3
                case 3:
                    channel_pointer.channel = DMA2_Channel3;
                    break;
#endif
#ifdef DMA2_Channel4
                case 4:
                    channel_pointer.channel = DMA2_Channel4;
                    break;
#endif
#ifdef DMA2_Channel5
                case 5:
                    channel_pointer.channel = DMA2_Channel5;
                    break;
#endif
#ifdef DMA2_Channel6
                case 6:
                    channel_pointer.channel = DMA2_Channel6;
                    break;
#endif
#ifdef DMA2_Channel7
                case 7:
                    channel_pointer.channel = DMA2_Channel7;
                    break;
#endif
#ifdef DMA2_Stream0
                case 0:
                    channel_pointer.channel = DMA2_Stream0;
                    break;
#endif
#ifdef DMA2_Stream1
                case 1:
                    channel_pointer.channel = DMA2_Stream1;
                    break;
#endif
#ifdef DMA2_Stream2
                case 2:
                    channel_pointer.channel = DMA2_Stream2;
                    break;
#endif
#ifdef DMA2_Stream3
                case 3:
                    channel_pointer.channel = DMA2_Stream3;
                    break;
#endif
#ifdef DMA2_Stream4
                case 4:
                    channel_pointer.channel = DMA2_Stream4;
                    break;
#endif
#ifdef DMA2_Stream5
                case 5:
                    channel_pointer.channel = DMA2_Stream5;
                    break;
#endif
#ifdef DMA2_Stream6
                case 6:
                    channel_pointer.channel = DMA2_Stream6;
                    break;
#endif
#ifdef DMA2_Stream7
                case 7:
                    channel_pointer.channel = DMA2_Stream7;
                    break;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
            break;
#endif
#ifdef GPDMA1
        case 1:
            switch(dmaLink->channelIdx)
            {
#ifdef GPDMA1_Channel0
                case 0:
                    channel_pointer.channel = GPDMA1_Channel0;
                    break;
#endif
#ifdef GPDMA1_Channel1
                case 1:
                    channel_pointer.channel = GPDMA1_Channel1;
                    break;
#endif
#ifdef GPDMA1_Channel2
                case 2:
                    channel_pointer.channel = GPDMA1_Channel2;
                    break;
#endif
#ifdef GPDMA1_Channel3
                case 3:
                    channel_pointer.channel = GPDMA1_Channel3;
                    break;
#endif
#ifdef GPDMA1_Channel4
                case 4:
                    channel_pointer.channel = GPDMA1_Channel4;
                    break;
#endif
#ifdef GPDMA1_Channel5
                case 5:
                    channel_pointer.channel = GPDMA1_Channel5;
                    break;
#endif
#ifdef GPDMA1_Channel6
                case 6:
                    channel_pointer.channel = GPDMA1_Channel6;
                    break;
#endif
#ifdef GPDMA1_Channel7
                case 7:
                    channel_pointer.channel = GPDMA1_Channel7;
                    break;
#endif
#ifdef GPDMA1_Channel8
                case 8:
                    channel_pointer.channel = GPDMA1_Channel8;
                    break;
#endif
#ifdef GPDMA1_Channel9
                case 9:
                    channel_pointer.channel = GPDMA1_Channel9;
                    break;
#endif
#ifdef GPDMA1_Channel10
                case 10:
                    channel_pointer.channel = GPDMA1_Channel10;
                    break;
#endif
#ifdef GPDMA1_Channel11
                case 11:
                    channel_pointer.channel = GPDMA1_Channel11;
                    break;
#endif
#ifdef GPDMA1_Channel12
                case 12:
                    channel_pointer.channel = GPDMA1_Channel12;
                    break;
#endif
#ifdef GPDMA1_Channel13
                case 13:
                    channel_pointer.channel = GPDMA1_Channel13;
                    break;
#endif
#ifdef GPDMA1_Channel14
                case 14:
                    channel_pointer.channel = GPDMA1_Channel14;
                    break;
#endif
#ifdef GPDMA1_Channel15
                case 15:
                    channel_pointer.channel = GPDMA1_Channel15;
                    break;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
            break;
#endif
#ifdef BDMA
        case 3:
            switch(dmaLink->channelIdx)
            {
                case 0:
                    channel_pointer.bchannel = BDMA_Channel0;
                    break;
                case 1:
                    channel_pointer.bchannel = BDMA_Channel1;
                    break;
                case 2:
                    channel_pointer.bchannel = BDMA_Channel2;
                    break;
                case 3:
                    channel_pointer.bchannel = BDMA_Channel3;
                    break;
                case 4:
                    channel_pointer.bchannel = BDMA_Channel4;
                    break;
                case 5:
                    channel_pointer.bchannel = BDMA_Channel5;
                    break;
                case 6:
                    channel_pointer.bchannel = BDMA_Channel6;
                    break;
                case 7:
                    channel_pointer.bchannel = BDMA_Channel7;
                    break;
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid BDMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
            break;
#endif
#ifdef MDMA
        case 4:
            switch(dmaLink->channelIdx)
            {
                case 0:
                    channel_pointer.mchannel = MDMA_Channel0;
                    break;
                case 1:
                    channel_pointer.mchannel = MDMA_Channel1;
                    break;
                case 2:
                    channel_pointer.mchannel = MDMA_Channel2;
                    break;
                case 3:
                    channel_pointer.mchannel = MDMA_Channel3;
                    break;
                case 4:
                    channel_pointer.mchannel = MDMA_Channel4;
                    break;
                case 5:
                    channel_pointer.mchannel = MDMA_Channel5;
                    break;
                case 6:
                    channel_pointer.mchannel = MDMA_Channel6;
                    break;
                case 7:
                    channel_pointer.mchannel = MDMA_Channel7;
                    break;
                case 8:
                    channel_pointer.mchannel = MDMA_Channel8;
                    break;
                case 9:
                    channel_pointer.mchannel = MDMA_Channel9;
                    break;
                case 10:
                    channel_pointer.mchannel = MDMA_Channel10;
                    break;
                case 11:
                    channel_pointer.mchannel = MDMA_Channel11;
                    break;
                case 12:
                    channel_pointer.mchannel = MDMA_Channel12;
                    break;
                case 13:
                    channel_pointer.mchannel = MDMA_Channel13;
                    break;
                case 14:
                    channel_pointer.mchannel = MDMA_Channel14;
                    break;
                case 15:
                    channel_pointer.mchannel = MDMA_Channel15;
                    break;
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid MDMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
            break;
#endif
        default:
            mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA controller", dmaLink->dmaIdx, MBED_FILENAME, __LINE__);

    }
    return channel_pointer;
}

IRQn_Type stm_get_dma_irqn(const DMALinkInfo *dmaLink)
{
    switch(dmaLink->dmaIdx)
    {
#ifdef DMA1
        case 1:
            switch(dmaLink->channelIdx)
            {
#ifdef DMA1_Channel1
                case 1:
                    return DMA1_Channel1_IRQn;
#endif

// STM32F0 has shared ISRs for Ch2-Ch3 and Ch4-Ch5, and NO ISRs for channels 6 and 7
#ifdef TARGET_MCU_STM32F0
                case 2:
                case 3:
                    return DMA1_Channel2_3_IRQn;
                case 4:
                case 5:
                    return DMA1_Channel4_5_IRQn;

// STM32G0 has shared ISRs for Ch2-Ch3 and Ch4-Ch7 (and also all DMA2 channels on devices with DMA2)
#elif defined(TARGET_MCU_STM32G0)
                case 2:
                case 3:
                    return DMA1_Channel2_3_IRQn;

                // IRQ name for the remaining DMA channels depends on whether DMA2 exists or not
                case 4:
                case 5:
                case 6:
                case 7:
#ifdef DMA2
                    return DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQn;
#elif defined(DMA1_Channel7)
                    return DMA1_Ch4_7_DMAMUX1_OVR_IRQn;
#else
                    return DMA1_Ch4_5_DMAMUX1_OVR_IRQn;
#endif

// STM32L0 has shared ISRs for Ch2-Ch3 and Ch4-Ch7
#elif defined(TARGET_MCU_STM32L0)
                case 2:
                case 3:
                    return DMA1_Channel2_3_IRQn;

                case 4:
                case 5:
                case 6:
                case 7:
                    return DMA1_Channel4_5_6_7_IRQn;
// STM32U0 has shared ISRs for Ch2-Ch3 and Ch4-Ch7
#elif defined(TARGET_MCU_STM32U0)
                case 2:
                case 3:
                    return DMA1_Channel2_3_IRQn;

                case 4:
                case 5:
                case 6:
                case 7:
                    return DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX_OVR_IRQn;
#else
#ifdef DMA1_Channel2
                case 2:
                    return DMA1_Channel2_IRQn;
#endif
#ifdef DMA1_Channel3
                case 3:
                    return DMA1_Channel3_IRQn;
#endif
#ifdef DMA1_Channel4
                case 4:
                    return DMA1_Channel4_IRQn;
#endif
#ifdef DMA1_Channel5
                case 5:
                    return DMA1_Channel5_IRQn;
#endif
#ifdef DMA1_Channel6
                case 6:
                    return DMA1_Channel6_IRQn;
#endif
#ifdef DMA1_Channel7
                case 7:
                    return DMA1_Channel7_IRQn;
#endif
#endif

#ifdef DMA1_Stream0
                case 0:
                    return DMA1_Stream0_IRQn;
#endif
#ifdef DMA1_Stream1
                case 1:
                    return DMA1_Stream1_IRQn;
#endif
#ifdef DMA1_Stream2
                case 2:
                    return DMA1_Stream2_IRQn;
#endif
#ifdef DMA1_Stream3
                case 3:
                    return DMA1_Stream3_IRQn;
#endif
#ifdef DMA1_Stream4
                case 4:
                    return DMA1_Stream4_IRQn;
#endif
#ifdef DMA1_Stream5
                case 5:
                    return DMA1_Stream5_IRQn;
#endif
#ifdef DMA1_Stream6
                case 6:
                    return DMA1_Stream6_IRQn;
#endif
#ifdef DMA1_Stream7
                case 7:
                    return DMA1_Stream7_IRQn;
#endif

                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif

#ifdef DMA2
        case 2:
            switch(dmaLink->channelIdx)
            {
#if defined(TARGET_MCU_STM32G0)
                // STM32G0 does its own thing and has all DMA2 channels under 1 IRQ
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    return DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQn;
#elif defined(TARGET_MCU_STM32F0)
                // STM32F0 has a rather bespoke mapping
                case 1:
                case 2:
                    return DMA1_Ch2_3_DMA2_Ch1_2_IRQn;
                case 3:
                case 4:
                case 5:
                    return DMA1_Ch4_7_DMA2_Ch3_5_IRQn;
#elif defined(TARGET_MCU_STM32U0)
                // STM32U0 has a rather bespoke mapping
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    return DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX_OVR_IRQn;
#else

#ifdef DMA2_Channel1
                case 1:
                    return DMA2_Channel1_IRQn;
#endif
#ifdef DMA2_Channel2
                case 2:
                    return DMA2_Channel2_IRQn;
#endif
#ifdef DMA2_Channel3
                case 3:
                    return DMA2_Channel3_IRQn;
#endif
#ifdef DMA2_Channel4
                case 4:
                    return DMA2_Channel4_IRQn;
#endif
#ifdef DMA2_Channel5
                case 5:
                    return DMA2_Channel5_IRQn;
#endif
#ifdef DMA2_Channel6
                case 6:
                    return DMA2_Channel6_IRQn;
#endif
#ifdef DMA2_Channel7
                case 7:
                    return DMA2_Channel7_IRQn;
#endif

#ifdef DMA2_Stream0
                    case 0:
                    return DMA2_Stream0_IRQn;
#endif
#ifdef DMA2_Stream1
                    case 1:
                    return DMA2_Stream1_IRQn;
#endif
#ifdef DMA2_Stream2
                    case 2:
                    return DMA2_Stream2_IRQn;
#endif
#ifdef DMA2_Stream3
                    case 3:
                    return DMA2_Stream3_IRQn;
#endif
#ifdef DMA2_Stream4
                    case 4:
                    return DMA2_Stream4_IRQn;
#endif
#ifdef DMA2_Stream5
                    case 5:
                    return DMA2_Stream5_IRQn;
#endif
#ifdef DMA2_Stream6
                    case 6:
                    return DMA2_Stream6_IRQn;
#endif
#ifdef DMA2_Stream7
                    case 7:
                    return DMA2_Stream7_IRQn;
#endif
#endif

                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif

#ifdef GPDMA1
        case 1:
            switch(dmaLink->channelIdx)
            {
#ifdef GPDMA1_Channel0
                case 0:
                    return GPDMA1_Channel0_IRQn;
#endif
#ifdef GPDMA1_Channel1
                case 1:
                    return GPDMA1_Channel1_IRQn;
#endif
#ifdef GPDMA1_Channel2
                case 2:
                    return GPDMA1_Channel2_IRQn;
#endif
#ifdef GPDMA1_Channel3
                case 3:
                    return GPDMA1_Channel3_IRQn;
#endif
#ifdef GPDMA1_Channel4
                case 4:
                    return GPDMA1_Channel4_IRQn;
#endif
#ifdef GPDMA1_Channel5
                case 5:
                    return GPDMA1_Channel5_IRQn;
#endif
#ifdef GPDMA1_Channel6
                case 6:
                    return GPDMA1_Channel6_IRQn;
#endif
#ifdef GPDMA1_Channel7
                case 7:
                    return GPDMA1_Channel7_IRQn;
#endif
#ifdef GPDMA1_Channel8
                case 8:
                    return GPDMA1_Channel8_IRQn;
#endif
#ifdef GPDMA1_Channel9
                case 9:
                    return GPDMA1_Channel9_IRQn;
#endif
#ifdef GPDMA1_Channel10
                case 10:
                    return GPDMA1_Channel10_IRQn;
#endif
#ifdef GPDMA1_Channel11
                case 11:
                    return GPDMA1_Channel11_IRQn;
#endif
#ifdef GPDMA1_Channel12
                case 12:
                    return GPDMA1_Channel12_IRQn;
#endif
#ifdef GPDMA1_Channel13
                case 13:
                    return GPDMA1_Channel13_IRQn;
#endif
#ifdef GPDMA1_Channel14
                case 14:
                    return GPDMA1_Channel14_IRQn;
#endif
#ifdef GPDMA1_Channel15
                case 15:
                    return GPDMA1_Channel15_IRQn;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif

#ifdef MDMA
        case 4:
            return MDMA_IRQn;
#endif
        default:
            mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA controller", dmaLink->dmaIdx, MBED_FILENAME, __LINE__);

    }
}

bool stm_set_dma_handle_for_link(DMALinkInfo const * dmaLink, DMAHandlePointer handle)
{
#ifdef DMA_IP_VERSION_V2
    // Channels start from 1 in IP v2 only
    uint8_t channelIdx = dmaLink->channelIdx - 1;
#else
    uint8_t channelIdx = dmaLink->channelIdx;
#endif
    if(stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx].hdma != NULL && handle.hdma != NULL)
    {
        return false;
    }
    stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx] = handle;
    return true;
}

DMAHandlePointer stm_get_dma_handle_for_link(DMALinkInfo const * dmaLink)
{
#ifdef DMA_IP_VERSION_V2
    // Channels start from 1 in IP v2 only
    uint8_t channelIdx = dmaLink->channelIdx - 1;
#else
    uint8_t channelIdx = dmaLink->channelIdx;
#endif
    return stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx];
}

DMAHandlePointer stm_init_dma_link(const DMALinkInfo *dmaLink, uint32_t direction, bool periphInc, bool memInc,
                                     uint8_t periphDataAlignment, uint8_t memDataAlignment, uint32_t mode){

    DMAHandlePointer dmaHandlePointer;
    dmaHandlePointer.hdma = NULL;
#ifdef DMA_IP_VERSION_V2
    // Channels start from 1 in IP v2 only
    uint8_t channelIdx = dmaLink->channelIdx - 1;
#else
    uint8_t channelIdx = dmaLink->channelIdx;
#endif

    if(stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx].hdma != NULL)
    {
        // Channel already allocated (e.g. two SPI busses which use the same DMA request tried to be initialized)
        return dmaHandlePointer;
    }

     // Enable DMA mux clock for devices with it
#ifdef __HAL_RCC_DMAMUX1_CLK_ENABLE
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
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
#ifdef GPDMA1
        case 1:
            __HAL_RCC_GPDMA1_CLK_ENABLE();
            break;
#endif
#ifdef BDMA
        case 3:
            __HAL_RCC_BDMA_CLK_ENABLE();
            break;
#endif
#ifdef MDMA
        case 4:
            __HAL_RCC_MDMA_CLK_ENABLE();
            break;
#endif
        default:
            mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA controller", dmaLink->dmaIdx, MBED_FILENAME, __LINE__);
    }

    // Allocate DMA handle.
    // Yes it's a little gross that we have to allocate on the heap, but this structure uses quite a lot of memory,
    // so we don't want to allocate DMA handles until they're needed.
    if(dmaLink->dmaIdx < 4)
    {
        DMA_HandleTypeDef * dmaHandle = malloc(sizeof(DMA_HandleTypeDef));
        dmaHandlePointer.hdma = dmaHandle;
        memset(dmaHandle, 0, sizeof(DMA_HandleTypeDef));
        stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx] = dmaHandlePointer;

        // Configure handle
        dmaHandle->Instance = stm_get_dma_channel(dmaLink).channel;
    #if STM_DEVICE_HAS_DMA_SOURCE_SELECTION

        // Most devices with IP v1 call this member "Channel" and most with IP v2 call it "Request".
        // But not STM32H7!
    #if defined(DMA_IP_VERSION_V1) && !defined(TARGET_MCU_STM32H7)
        dmaHandle->Init.Channel = dmaLink->sourceNumber << DMA_SxCR_CHSEL_Pos;
    #else
        dmaHandle->Init.Request = dmaLink->sourceNumber;
    #endif

    #endif
        dmaHandle->Init.Direction = direction;

        // IP v3 uses different fields for... basically everything in this struct
    #ifdef DMA_IP_VERSION_V3
        if(direction == DMA_MEMORY_TO_PERIPH || direction == DMA_MEMORY_TO_MEMORY)
        {
            // Source is memory
            dmaHandle->Init.SrcInc = memInc ? DMA_SINC_INCREMENTED : DMA_SINC_FIXED;

            switch(memDataAlignment) {
                case 4:
                    dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
                    break;
                case 2:
                    dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
                    break;
                case 1:
                default:
                    dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
                    break;
            }
        }
        else {
            // Source is a peripheral
            dmaHandle->Init.SrcInc = periphInc ? DMA_SINC_INCREMENTED : DMA_SINC_FIXED;

            switch(periphDataAlignment) {
                case 4:
                    dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
                    break;
                case 2:
                    dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
                    break;
                case 1:
                default:
                    dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
                    break;

            }
        }

        if(direction == DMA_PERIPH_TO_MEMORY || direction == DMA_MEMORY_TO_MEMORY)
        {
            // Destination is memory
            dmaHandle->Init.DestInc = memInc ? DMA_DINC_INCREMENTED : DMA_DINC_FIXED;

            switch(memDataAlignment) {
                case 4:
                    dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
                    break;
                case 2:
                    dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
                    break;
                case 1:
                default:
                    dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
                    break;

            }
        }
        else {
            // Destination is a peripheral
            dmaHandle->Init.DestInc = periphInc ? DMA_DINC_INCREMENTED : DMA_DINC_FIXED;

            switch(periphDataAlignment) {
                case 4:
                    dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
                    break;
                case 2:
                    dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
                    break;
                case 1:
                default:
                    dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
                    break;

            }
        }

        dmaHandle->Init.SrcBurstLength = 1;
        dmaHandle->Init.DestBurstLength = 1;
        dmaHandle->Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
        dmaHandle->Init.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
        dmaHandle->Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1|DMA_DEST_ALLOCATED_PORT0;
        dmaHandle->Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;

    #else
        dmaHandle->Init.PeriphInc = periphInc ? DMA_PINC_ENABLE : DMA_PINC_DISABLE;
        dmaHandle->Init.MemInc = memInc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE;
        dmaHandle->Init.Priority = DMA_PRIORITY_MEDIUM;

        switch(periphDataAlignment) {
            case 4:
                dmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
                break;
            case 2:
                dmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
                break;
            case 1:
            default:
                dmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
                break;

        }

        switch(memDataAlignment) {
            case 4:
                dmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
                break;
            case 2:
                dmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
                break;
            case 1:
            default:
                dmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
                break;

        }

    #endif

        dmaHandle->Init.Mode = mode;

        HAL_DMA_Init(dmaHandle);
    }
#if defined(MDMA)
    else
    {
        MDMA_HandleTypeDef * dmaHandle = malloc(sizeof(MDMA_HandleTypeDef));
        dmaHandlePointer.hmdma = dmaHandle;
        memset(dmaHandle, 0, sizeof(MDMA_HandleTypeDef));
        stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx] = dmaHandlePointer;

        dmaHandle->Init.Request = dmaLink->sourceNumber;
        dmaHandle->Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
        dmaHandle->Init.Priority = MDMA_PRIORITY_HIGH;
        dmaHandle->Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;

        if(direction == DMA_MEMORY_TO_PERIPH || direction == DMA_MEMORY_TO_MEMORY)
        {
            // Source is memory
            dmaHandle->Init.SourceInc = memInc ? MDMA_SRC_INC_BYTE: MDMA_SRC_INC_DISABLE;

            switch(memDataAlignment) {
                case 8:
                    dmaHandle->Init.SourceDataSize = MDMA_SRC_DATASIZE_DOUBLEWORD;
                    break;
                case 4:
                    dmaHandle->Init.SourceDataSize = MDMA_SRC_DATASIZE_WORD;
                    break;
                case 2:
                    dmaHandle->Init.SourceDataSize = MDMA_SRC_DATASIZE_HALFWORD;
                    break;
                case 1:
                default:
                    dmaHandle->Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
                    break;

            }
        }
        else {
            // Source is a peripheral
        dmaHandle->Init.SourceInc = periphInc ? MDMA_SRC_INC_BYTE: MDMA_SRC_INC_DISABLE;

            switch(periphDataAlignment) {
        case 8:
            dmaHandle->Init.SourceDataSize = MDMA_SRC_DATASIZE_DOUBLEWORD;
            break;
        case 4:
            dmaHandle->Init.SourceDataSize = MDMA_SRC_DATASIZE_WORD;
            break;
        case 2:
            dmaHandle->Init.SourceDataSize = MDMA_SRC_DATASIZE_HALFWORD;
            break;
        case 1:
                default:
            dmaHandle->Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
            break;

        }
        }

        if(direction == DMA_PERIPH_TO_MEMORY || direction == DMA_MEMORY_TO_MEMORY)
        {
            // Destination is memory
            dmaHandle->Init.DestinationInc = memInc ? MDMA_DEST_INC_BYTE: MDMA_DEST_INC_DISABLE;

            switch(memDataAlignment) {
            case 8:
            dmaHandle->Init.DestDataSize = MDMA_DEST_DATASIZE_DOUBLEWORD;
            break;
        case 4:
            dmaHandle->Init.DestDataSize = MDMA_DEST_DATASIZE_WORD;
            break;
        case 2:
            dmaHandle->Init.DestDataSize = MDMA_DEST_DATASIZE_HALFWORD;
            break;
        case 1:
                default:
            dmaHandle->Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
            break;

            }
        }
        else {
            // Destination is a peripheral
            dmaHandle->Init.DestinationInc = periphInc ? MDMA_DEST_INC_BYTE: MDMA_DEST_INC_DISABLE;

            switch(periphDataAlignment) {
                case 8:
                    dmaHandle->Init.DestDataSize = MDMA_DEST_DATASIZE_DOUBLEWORD;
                    break;
                case 4:
                    dmaHandle->Init.DestDataSize = MDMA_DEST_DATASIZE_WORD;
                    break;
                case 2:
                    dmaHandle->Init.DestDataSize = MDMA_DEST_DATASIZE_HALFWORD;
                    break;
                case 1:
                default:
                    dmaHandle->Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
                    break;
            }
        }
        dmaHandle->Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
        dmaHandle->Init.BufferTransferLength = 64;
        dmaHandle->Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
        dmaHandle->Init.DestBurst = MDMA_DEST_BURST_SINGLE;

        dmaHandle->Init.SourceBlockAddressOffset = 0;
        dmaHandle->Init.DestBlockAddressOffset = 0;

        dmaHandle->Instance = stm_get_dma_channel(dmaLink).mchannel;

      HAL_MDMA_Init(dmaHandle);
    }
#endif
    // Set up interrupt
    IRQn_Type irqNum = stm_get_dma_irqn(dmaLink);
    NVIC_EnableIRQ(irqNum);
    NVIC_SetPriority(irqNum, 7);

    return dmaHandlePointer;
}

void stm_free_dma_link(const DMALinkInfo *dmaLink)
{
    // Note: we can't disable the interrupt here, in case one ISR is shared by multiple DMA channels
    // and another channel is still using the interrupt.

#ifdef DMA_IP_VERSION_V2
    // Channels start from 1 in IP v2 only
    uint8_t channelIdx = dmaLink->channelIdx - 1;
#else
    uint8_t channelIdx = dmaLink->channelIdx;
#endif

    if(stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx].hdma == NULL)
    {
        return;
    }

    // Deinit hardware channel
    switch(dmaLink->dmaIdx)
    {
        case 1:
        case 2:
        case 3:
            HAL_DMA_DeInit(stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx].hdma);
            break;
#ifdef MDMA
        case 4:
            HAL_MDMA_DeInit(stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx].hmdma);
        break;
#endif
    }
    IRQn_Type irqNum = stm_get_dma_irqn(dmaLink);
    NVIC_DisableIRQ(irqNum);
    free(stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx].hdma);
    stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx].hdma = NULL;
}

#ifdef DMA_IP_VERSION_V2

#ifdef DMA1_Channel1
void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][0].hdma);
}
#endif

// STM32F0 has shared ISRs for Ch2-Ch3 and Ch4-Ch5
#ifdef TARGET_MCU_STM32F0

void DMA1_Channel2_3_IRQHandler(void)
{
    if(stmDMAHandles[0][1].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][1].hdma);
    }
    if(stmDMAHandles[0][2].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][2].hdma);
    }
}

void DMA1_Channel4_5_IRQHandler(void)
{
    if(stmDMAHandles[0][3].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
    }
    if(stmDMAHandles[0][4].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
    }
}

#elif defined(TARGET_MCU_STM32G0)

void DMA1_Channel2_3_IRQHandler(void)
{
    if(stmDMAHandles[0][1].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][1].hdma);
    }
    if(stmDMAHandles[0][2].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][2].hdma);
    }
}

#ifdef DMA2
void DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQHandler(void)
{
    if(stmDMAHandles[0][3].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
    }
    if(stmDMAHandles[0][4].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
    }
    if(stmDMAHandles[0][5].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][5].hdma);
    }
    if(stmDMAHandles[0][6].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][6].hdma);
    }
    if(stmDMAHandles[1][0].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][0].hdma);
    }
    if(stmDMAHandles[1][1].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][1].hdma);
    }
    if(stmDMAHandles[1][2].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][2].hdma);
    }
    if(stmDMAHandles[1][3].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][3].hdma);
    }
    if(stmDMAHandles[1][4].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][4].hdma);
    }
}
#elif defined(DMA1_Channel7)
void DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler(void)
{
    if(stmDMAHandles[0][3].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
    }
    if(stmDMAHandles[0][4].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
    }
    if(stmDMAHandles[0][5].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][5].hdma);
    }
    if(stmDMAHandles[0][6].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][6].hdma);
    }
}
#else
void DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler(void)
{
    if(stmDMAHandles[0][3].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
    }
    if(stmDMAHandles[0][4].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
    }
}
#endif

#elif defined(TARGET_MCU_STM32U0)

void DMA1_Channel2_3_IRQHandler(void)
{
    if(stmDMAHandles[0][1].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][1].hdma);
    }
    if(stmDMAHandles[0][2].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][2].hdma);
    }
}

#ifdef DMA2
void DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX_OVR_IRQHandler(void)
{
    if(stmDMAHandles[0][3].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
    }
    if(stmDMAHandles[0][4].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
    }
    if(stmDMAHandles[0][5].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][5].hdma);
    }
    if(stmDMAHandles[0][6].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][6].hdma);
    }
    if(stmDMAHandles[1][0].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][0].hdma);
    }
    if(stmDMAHandles[1][1].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][1].hdma);
    }
    if(stmDMAHandles[1][2].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][2].hdma);
    }
    if(stmDMAHandles[1][3].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][3].hdma);
    }
    if(stmDMAHandles[1][4].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][4].hdma);
    }
}
#elif defined(DMA1_Channel7)
void DMA1_Ch4_7_DMAMUX_OVR_IRQHandler(void)
{
    if(stmDMAHandles[0][3] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
    }
    if(stmDMAHandles[0][4] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
    }
    if(stmDMAHandles[0][5] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][5].hdma);
    }
    if(stmDMAHandles[0][6] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][6].hdma);
    }
}
#else
void DMA1_Ch4_5_DMAMUX_OVR_IRQHandler(void)
{
    if(stmDMAHandles[0][3] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
    }
    if(stmDMAHandles[0][4] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
    }
}
#endif

#elif defined(TARGET_MCU_STM32L0)

void DMA1_Channel2_3_IRQHandler(void)
{
    if(stmDMAHandles[0][1].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][1].hdma);
    }
    if(stmDMAHandles[0][2].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][2].hdma);
    }
}

void DMA1_Channel4_5_6_7_IRQHandler(void)
{
    if(stmDMAHandles[0][3].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
    }
    if(stmDMAHandles[0][4].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
    }
    if(stmDMAHandles[0][5].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][5].hdma);
    }
    if(stmDMAHandles[0][6].hdma != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][6].hdma);
    }
}

#else

#ifdef DMA1_Channel2
void DMA1_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][1].hdma);
}
#endif

#ifdef DMA1_Channel3
void DMA1_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][2].hdma);
}
#endif

#ifdef DMA1_Channel4
void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
}
#endif

#ifdef DMA1_Channel5
void DMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
}
#endif

#ifdef DMA1_Channel6
void DMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][5].hdma);
}
#endif

#ifdef DMA1_Channel7
void DMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][6].hdma);
}
#endif
#endif

#ifdef DMA2_Channel1
void DMA2_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][0].hdma);
}
#endif

#ifdef DMA2_Channel2
void DMA2_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][1].hdma);
}
#endif

#ifdef DMA2_Channel3
void DMA2_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][2].hdma);
}
#endif

#ifdef DMA2_Channel4
void DMA2_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][3].hdma);
}
#endif

#ifdef DMA2_Channel5
void DMA2_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][4].hdma);
}
#endif

#ifdef DMA2_Channel6
void DMA2_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][5].hdma);
}
#endif

#ifdef DMA2_Channel7
void DMA2_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][6].hdma);
}
#endif
#endif // DMA_IP_VERSION_V2

#ifdef DMA_IP_VERSION_V1
#ifdef DMA1_Stream0
void DMA1_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][0].hdma);
}
#endif

#ifdef DMA1_Stream1
void DMA1_Stream1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][1].hdma);
}
#endif

#ifdef DMA1_Stream2
void DMA1_Stream2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][2].hdma);
}
#endif

#ifdef DMA1_Stream3
void DMA1_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
}
#endif

#ifdef DMA1_Stream4
void DMA1_Stream4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
}
#endif

#ifdef DMA1_Stream5
void DMA1_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][5].hdma);
}
#endif

#ifdef DMA1_Stream6
void DMA1_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][6].hdma);
}
#endif

#ifdef DMA1_Stream7
void DMA1_Stream7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][7].hdma);
}
#endif

#ifdef DMA2_Stream0
void DMA2_Stream0_IRQHandler(void)
{
    // Note: Unlike both IP v2 and IP v3, IP v1 channels are 0-indexed.
    HAL_DMA_IRQHandler(stmDMAHandles[1][0].hdma);
}
#endif

#ifdef DMA2_Stream1
void DMA2_Stream1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][1].hdma);
}
#endif

#ifdef DMA2_Stream2
void DMA2_Stream2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][2].hdma);
}
#endif

#ifdef DMA2_Stream3
void DMA2_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][3].hdma);
}
#endif

#ifdef DMA2_Stream4
void DMA2_Stream4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][4].hdma);
}
#endif

#ifdef DMA2_Stream5
void DMA2_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][5].hdma);
}
#endif

#ifdef DMA2_Stream6
void DMA2_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][6].hdma);
}
#endif

#ifdef DMA2_Stream7
void DMA2_Stream7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][7].hdma);
}
#endif
#endif // DMA_IP_VERSION_V1

#ifdef DMA_IP_VERSION_V3
#ifdef GPDMA1_Channel0
void GPDMA1_Channel0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][0].hdma);
}
#endif

#ifdef GPDMA1_Channel1
void GPDMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][1].hdma);
}
#endif

#ifdef GPDMA1_Channel2
void GPDMA1_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][2].hdma);
}
#endif

#ifdef GPDMA1_Channel3
void GPDMA1_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][3].hdma);
}
#endif

#ifdef GPDMA1_Channel4
void GPDMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][4].hdma);
}
#endif

#ifdef GPDMA1_Channel5
void GPDMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][5].hdma);
}
#endif

#ifdef GPDMA1_Channel6
void GPDMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][6].hdma);
}
#endif

#ifdef GPDMA1_Channel7
void GPDMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][7].hdma);
}
#endif

#ifdef GPDMA1_Channel8
void GPDMA1_Channel8_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][8].hdma);
}
#endif

#ifdef GPDMA1_Channel9
void GPDMA1_Channel9_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][9].hdma);
}
#endif

#ifdef GPDMA1_Channel10
void GPDMA1_Channel10_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][10].hdma);
}
#endif

#ifdef GPDMA1_Channel11
void GPDMA1_Channel11_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][11].hdma);
}
#endif

#ifdef GPDMA1_Channel12
void GPDMA1_Channel12_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][12].hdma);
}
#endif

#ifdef GPDMA1_Channel13
void GPDMA1_Channel13_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][13].hdma);
}
#endif

#ifdef GPDMA1_Channel14
void GPDMA1_Channel14_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][14].hdma);
}
#endif

#ifdef GPDMA1_Channel15
void GPDMA1_Channel15_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][15].hdma);
}
#endif

#endif // DMA_IP_VERSION_V3

#ifdef MDMA
void MDMA_IRQHandler(void)
{
    DMAHandlePointer *stmMDMAHandle = stmDMAHandles[3];
    for(size_t i = 0; i < MAX_MDMA_CHANNELS; i++)
    {
        if(stmMDMAHandle[i].hmdma != NULL)
        {
            HAL_MDMA_IRQHandler(stmMDMAHandle[i].hmdma);
        }
    }
}
#endif
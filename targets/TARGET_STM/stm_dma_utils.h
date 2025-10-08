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


#ifndef MBED_OS_STM_DMA_UTILS_H
#define MBED_OS_STM_DMA_UTILS_H

#include <inttypes.h>
#include <stdbool.h>

#include "cmsis.h"

// determine DMA IP version using the available constants in the chip header
#if defined(GPDMA1)
#define DMA_IP_VERSION_V3 1
#elif defined(DMA1_Channel1)
#define DMA_IP_VERSION_V2 1
#else
#define DMA_IP_VERSION_V1 1
#endif

// Include correct header for the IP version
#ifdef DMA_IP_VERSION_V3
#include "stm_dma_ip_v3.h"
#elif defined(DMA_IP_VERSION_V2)
#include "stm_dma_ip_v2.h"
#else
#include "stm_dma_ip_v1.h"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

// Any general-purpose DMA module (DMA1/DMA2 or GPDMA1/GPDMA2)
#define MBED_ANY_GPDMA_MODULE 255

// Any DMA channel
#define MBED_ANY_DMA_CHANNEL 255

/*
 * Structure containing info about a peripheral's link to the DMA controller.
 */
typedef struct DMALinkInfo {

    /// Index of the DMA module that the DMA link uses.
    /// Note: 1-indexed.
    uint8_t dmaIdx;

    /// Index of the channel on the DMA module.
    /// Note that some STMicro chips have a DMA mux allowing any DMA peripheral to be used with
    /// any channel, and others have a semi-fixed architecture with just some basic multiplexing.
    /// Note: May be 1 or 0 indexed depending on processor
    uint8_t channelIdx;

#if STM_DEVICE_HAS_DMA_SOURCE_SELECTION
    /// Request source number.  This is either a DMA mux input number, or a mux selection number
    /// on devices without a DMA mux.
    /// Note: 0-indexed.
    uint8_t sourceNumber;
#endif
} DMALinkInfo;

typedef union DMAInstancePointer {
    DMA_TypeDef *dma;
#ifdef BDMA
    BDMA_TypeDef *bdma;
#endif
#ifdef MDMA
    MDMA_TypeDef *mdma;
#endif
} DMAInstancePointer;

typedef struct {
    uint8_t dmaIdx; // Index of the DMA module
    uint8_t channelIdx; // Index of the channel
    union {
        DMA_HandleTypeDef *hdma; // General purpose DMA pointer
#ifdef MDMA
        MDMA_HandleTypeDef *hmdma; // STM32H7 MDMA pointer
#endif
    };
} DMAHandlePointer;

typedef union DMAChannelPointer {
    DMA_Channel_TypeDef *channel;
#ifdef BDMA
    BDMA_Channel_TypeDef *bchannel;
#endif
#ifdef MDMA
    MDMA_Channel_TypeDef *mchannel;
#endif
} DMAChannelPointer;

/**
 * @brief Initialize the mutex for DMA
 */
void stm_init_dma_mutex();

/**
 * @brief Lock the mutex for DMA
 */
void stm_lock_dma_mutex();

/**
 * @brief Unlock the mutex for DMA
 */
void stm_unlock_dma_mutex();

/**
 * @brief Find a free DMA channel for a DMA link with MBED_ANY_DMA_CHANNEL
 *
 * @param dmaLink DMA link instance with MBED_ANY_DMA_CHANNEL
 * @param freeDmaLink DMA link instance with the actual DMA instance and channel indexes
 *
 * @return true if a free DMA channel is found
 * @return false if a free DMA channel is not found
 */
bool stm_find_free_dma_channel(DMALinkInfo const * dmaLink, DMALinkInfo * freeDmaLink);


/**
 * @brief Get the DMA instance for a DMA link
 *
 * @param dmaLink DMA link instance
 */
DMAInstancePointer stm_get_dma_instance(DMALinkInfo const * dmaLink);

/**
 * @brief Get the DMA channel instance for a DMA link
 *
 * @param dmaLink DMA link instance
 */
DMAChannelPointer stm_get_dma_channel(DMALinkInfo const * dmaLink);

/**
 * @brief Get the interrupt number for a DMA link
 *
 * @param dmaLink DMA link instance
 */
IRQn_Type stm_get_dma_irqn(const DMALinkInfo *dmaLink);

/**
 * @brief Store a externaly initialized DMA handle to the DMA handles matrix
 *
 * @param dmaLink DMA link instance
 * @param handle DMA handle or NULL
 *
 * @return true if the handle is stored successfully
 * @return false if handle is not NULL and the DMA channel used by the link has already been used
 */
bool stm_set_dma_handle_for_link(DMALinkInfo const * dmaLink, DMAHandlePointer handle);

/**
 * @brief Get the handle of a DMA link
 *
 * @param dmaLink DMA link instance
 *
 * @return Pointer to DMA handle
 * @return NULL if the DMA channel used by the link is not allocated
 */
DMAHandlePointer stm_get_dma_handle_for_link(DMALinkInfo const * dmaLink);

/**
 * @brief Initialize a DMA link for use.
 *
 * This enables and sets up the interrupt, allocates a DMA handle, and returns the handle pointer.
 * Arguments are based on the parameters used for the DMA_InitTypeDef structure.
 *
 * @param dmaLink DMA link instance
 * @param direction \c DMA_PERIPH_TO_MEMORY, \c DMA_MEMORY_TO_PERIPH, or \c DMA_MEMORY_TO_MEMORY
 * @param periphInc Whether the Peripheral address register should be incremented or not.
 * @param memInc Whether the Memory address register should be incremented or not.
 * @param periphDataAlignment Alignment value of the peripheral data.  1, 2, or 4.
 * @param memDataAlignment Alignment value of the memory data.  1, 2, or 4.
 * @param mode Mode of the DMA transaction.  DMA_NORMAL, DMA_CIRCULAR, etc
 *
 * @return Pointer to DMA handle allocated by this module.
 * @return NULL if the DMA channel used by the link has already been allocated by something else.
 */
DMAHandlePointer stm_init_dma_link(DMALinkInfo const * dmaLink, uint32_t direction, bool periphInc, bool memInc, uint8_t periphDataAlignment, uint8_t memDataAlignment, uint32_t mode);

/**
 * @brief Free a DMA channel.
 *
 * This frees memory associated with it and unlocks the hardware DMA channel so that it can be used by somebody else.
 *
 * @param handle pointer to the DMA channel handle to free.
 */
void stm_free_dma_link(DMAHandlePointer *handle);

#if defined(__cplusplus)
}
#endif

#endif //MBED_OS_STM_DMA_UTILS_H

/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * Copyright (c) 2017, STMicroelectronics.
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

#if DEVICE_QSPI

#include "qspi_api.h"
#include "mbed_error.h"
#include "mbed_debug.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "mbed-trace/mbed_trace.h"

#if defined(OCTOSPI1)
#define TRACE_GROUP "STOS"
#else
#define TRACE_GROUP "STQS"
#endif /* OCTOSPI1 */

#include "stm_dma_info.h"

// activate / de-activate extra debug
#define qspi_api_c_debug 0

/* Max amount of flash size is 4Gbytes */
/* hence 2^(31+1), then FLASH_SIZE_DEFAULT = 1<<31 */
#define QSPI_FLASH_SIZE_DEFAULT 0x80000000

/* Minimum number of bytes to be transferred using DMA, when DCACHE is not available */
/* When less than 32 bytes of data is transferred at a time, using DMA may actually be slower than polling */
/* When DACHE is available, DMA will be used when the buffer contains at least one cache-aligned block */
#define QSPI_DMA_THRESHOLD_BYTES 32

#if defined(QUADSPI)
static QSPI_HandleTypeDef * qspiHandle; // Handle of whatever QSPI structure is used for QUADSPI

// Store the spi_s * inside an SPI handle, for later retrieval in callbacks
static inline void store_qspi_pointer(QSPI_HandleTypeDef * qspiHandle, struct qspi_s * qspis) {
    // Annoyingly, STM neglected to provide any sort of "user data" pointer inside QSPI_HandleTypeDef for use
    // in callbacks.  However, there are some variables in the Init struct that are never accessed after HAL_QSPI_Init().
    // So, we can reuse those to store our pointer.
    qspiHandle->Init.ChipSelectHighTime = (uint32_t)qspis;
}

// Get spi_s * from SPI_HandleTypeDef
static inline struct qspi_s * get_qspi_pointer(QSPI_HandleTypeDef * qspiHandle) {
    return (struct qspi_s *) qspiHandle->Init.ChipSelectHighTime;
}

void QUADSPI_IRQHandler()
{
    HAL_QSPI_IRQHandler(qspiHandle);
}

#if MBED_CONF_RTOS_PRESENT
void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef * handle)
{
    osSemaphoreRelease(get_qspi_pointer(handle)->semaphoreId);
}

void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef * handle)
{
    osSemaphoreRelease(get_qspi_pointer(handle)->semaphoreId);
}
#endif

void HAL_QSPI_ErrorCallback(QSPI_HandleTypeDef * handle)
{
    handle->State = HAL_QSPI_STATE_ERROR;
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreRelease(get_qspi_pointer(handle)->semaphoreId);
#endif
}

void HAL_QSPI_TimeOutCallback(QSPI_HandleTypeDef * handle)
{
    handle->State = HAL_QSPI_STATE_ERROR;
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreRelease(get_qspi_pointer(handle)->semaphoreId);
#endif
}
#endif

#if defined(OCTOSPI1)
static OSPI_HandleTypeDef * ospiHandle1;

// Store the qspi_s * inside an OSPI handle, for later retrieval in callbacks
static inline void store_qspi_pointer(OSPI_HandleTypeDef * ospiHandle, struct qspi_s * qspis) {
    // Annoyingly, STM neglected to provide any sort of "user data" pointer inside OSPI_HandleTypeDef for use
    // in callbacks.  However, there are some variables in the Init struct that are never accessed after HAL_OSPI_Init().
    // So, we can reuse those to store our pointer.
    ospiHandle->Init.ChipSelectHighTime = (uint32_t)qspis;
}

// Get qspi_s * from OSPI_HandleTypeDef
static inline struct qspi_s * get_qspi_pointer(OSPI_HandleTypeDef * ospiHandle) {
    return (struct qspi_s *) ospiHandle->Init.ChipSelectHighTime;
}

void OCTOSPI1_IRQHandler()
{
    HAL_OSPI_IRQHandler(ospiHandle1);
}

#if MBED_CONF_RTOS_PRESENT
void HAL_OSPI_TxCpltCallback(OSPI_HandleTypeDef * handle)
{
    osSemaphoreRelease(get_qspi_pointer(handle)->semaphoreId);
}

void HAL_OSPI_RxCpltCallback(OSPI_HandleTypeDef * handle)
{
    osSemaphoreRelease(get_qspi_pointer(handle)->semaphoreId);
}
#endif

void HAL_OSPI_ErrorCallback(OSPI_HandleTypeDef * handle)
{
    handle->State = HAL_OSPI_STATE_ERROR;
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreRelease(get_qspi_pointer(handle)->semaphoreId);
#endif
}

void HAL_OSPI_TimeOutCallback(OSPI_HandleTypeDef * handle)
{
    handle->State = HAL_OSPI_STATE_ERROR;
#if MBED_CONF_RTOS_PRESENT
    osSemaphoreRelease(get_qspi_pointer(handle)->semaphoreId);
#endif
}
#endif

#if defined(OCTOSPI2)
static OSPI_HandleTypeDef * ospiHandle2;

void OCTOSPI2_IRQHandler()
{
    HAL_OSPI_IRQHandler(ospiHandle2);
}
#endif

#if defined(OCTOSPI1)
static uint32_t get_alt_bytes_size(const uint32_t num_bytes)
{
    switch (num_bytes) {
        case 1:
            return HAL_OSPI_ALTERNATE_BYTES_8_BITS;
        case 2:
            return HAL_OSPI_ALTERNATE_BYTES_16_BITS;
        case 3:
            return HAL_OSPI_ALTERNATE_BYTES_24_BITS;
        case 4:
            return HAL_OSPI_ALTERNATE_BYTES_32_BITS;
    }
    error("Invalid alt bytes size");
    return 0xFFFFFFFF;
}
#else /* OCTOSPI1 */
static uint32_t get_alt_bytes_size(const uint32_t num_bytes)
{
    switch (num_bytes) {
        case 1:
            return QSPI_ALTERNATE_BYTES_8_BITS;
        case 2:
            return QSPI_ALTERNATE_BYTES_16_BITS;
        case 3:
            return QSPI_ALTERNATE_BYTES_24_BITS;
        case 4:
            return QSPI_ALTERNATE_BYTES_32_BITS;
    }
    error("Invalid alt bytes size");
    return 0xFFFFFFFF;
}
#endif /* OCTOSPI1 */

#if defined(OCTOSPI1)
qspi_status_t qspi_prepare_command(const qspi_command_t *command, OSPI_RegularCmdTypeDef *st_command)
{
    debug_if(qspi_api_c_debug, "qspi_prepare_command In: instruction.value %x dummy_count %x address.bus_width %x address.disabled %x address.value %x address.size %x\n",
             command->instruction.value, command->dummy_count, command->address.bus_width, command->address.disabled, command->address.value, command->address.size);

    st_command->FlashId = HAL_OSPI_FLASH_ID_1;

    if (command->instruction.disabled == true) {
        st_command->InstructionMode = HAL_OSPI_INSTRUCTION_NONE;
        st_command->Instruction = 0;
    } else {
        st_command->Instruction = command->instruction.value;
        switch (command->instruction.bus_width) {
            case QSPI_CFG_BUS_SINGLE:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
                break;
            case QSPI_CFG_BUS_DUAL:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_2_LINES;
                break;
            case QSPI_CFG_BUS_QUAD:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_4_LINES;
                break;
            default:
                error("Command param error: wrong instruction format\n");
                return QSPI_STATUS_ERROR;
        }
    }

    st_command->InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
    st_command->InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    st_command->DummyCycles = command->dummy_count;
    // these are target specific settings, use default values
    st_command->SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
    st_command->DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE;
    st_command->AddressDtrMode = HAL_OSPI_ADDRESS_DTR_DISABLE;
    st_command->AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
    st_command->DQSMode = HAL_OSPI_DQS_DISABLE;

    st_command->OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
    if (command->address.disabled == true) {
        st_command->AddressMode = HAL_OSPI_ADDRESS_NONE;
        st_command->AddressSize = 0;
    } else {
        st_command->Address = command->address.value;
        switch (command->address.bus_width) {
            case QSPI_CFG_BUS_SINGLE:
                st_command->AddressMode = HAL_OSPI_ADDRESS_1_LINE;
                break;
            case QSPI_CFG_BUS_DUAL:
                st_command->AddressMode = HAL_OSPI_ADDRESS_2_LINES;
                break;
            case QSPI_CFG_BUS_QUAD:
                st_command->AddressMode = HAL_OSPI_ADDRESS_4_LINES;
                break;
            default:
                error("Command param error: wrong address size\n");
                return QSPI_STATUS_ERROR;
        }
        switch (command->address.size) {
            case QSPI_CFG_ADDR_SIZE_8:
                st_command->AddressSize = HAL_OSPI_ADDRESS_8_BITS;
                break;
            case QSPI_CFG_ADDR_SIZE_16:
                st_command->AddressSize = HAL_OSPI_ADDRESS_16_BITS;
                break;
            case QSPI_CFG_ADDR_SIZE_24:
                st_command->AddressSize = HAL_OSPI_ADDRESS_24_BITS;
                break;
            case QSPI_CFG_ADDR_SIZE_32:
                st_command->AddressSize = HAL_OSPI_ADDRESS_32_BITS;
                break;
            default:
                error("Command param error: wrong address size\n");
                return QSPI_STATUS_ERROR;
        }
    }

    if (command->alt.disabled == true) {
        st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
        st_command->AlternateBytesSize = 0;
    } else {
        uint8_t alt_lines = 0;
        switch (command->alt.bus_width) {
            case QSPI_CFG_BUS_SINGLE:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_1_LINE;
                alt_lines = 1;
                break;
            case QSPI_CFG_BUS_DUAL:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_2_LINES;
                alt_lines = 2;
                break;
            case QSPI_CFG_BUS_QUAD:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_4_LINES;
                alt_lines = 4;
                break;
            default:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
                error("Command param error: invalid alt bytes mode\n");
                return QSPI_STATUS_ERROR;
        }

        // Alt size must be a multiple of the number of bus lines used (i.e. a whole number of cycles)
        if (command->alt.size % alt_lines != 0) {
            error("Command param error: incompatible alt size and alt bus width\n");
            return QSPI_STATUS_ERROR;
        }

        // Round up to nearest byte - unused parts of byte act as dummy cycles
        uint32_t alt_bytes = ((command->alt.size - 1) >> 3) + 1;
        // Maximum of 4 alt bytes
        if (alt_bytes > 4) {
            error("Command param error: alt size exceeds maximum of 32 bits\n");
            return QSPI_STATUS_ERROR;
        }

        // Unused bits in most significant byte of alt
        uint8_t leftover_bits = (alt_bytes << 3) - command->alt.size;
        if (leftover_bits != 0) {
            // Account for dummy cycles that will be spent in the alt portion of the command
            uint8_t integrated_dummy_cycles = leftover_bits / alt_lines;
            if (st_command->DummyCycles < integrated_dummy_cycles) {
                // Not enough dummy cycles to account for a short alt
                error("Command param error: not enough dummy cycles to make up for given alt size\n");
                return QSPI_STATUS_ERROR;
            }
            st_command->DummyCycles -= integrated_dummy_cycles;

            // Align alt value to the end of the most significant byte
            st_command->AlternateBytes = command->alt.value << leftover_bits;
        } else {
            st_command->AlternateBytes = command->alt.value;
        }

        st_command->AlternateBytesSize = get_alt_bytes_size(alt_bytes);
    }

    switch (command->data.bus_width) {
        case QSPI_CFG_BUS_SINGLE:
            st_command->DataMode = HAL_OSPI_DATA_1_LINE;
            break;
        case QSPI_CFG_BUS_DUAL:
            st_command->DataMode = HAL_OSPI_DATA_2_LINES;
            break;
        case QSPI_CFG_BUS_QUAD:
            st_command->DataMode = HAL_OSPI_DATA_4_LINES;
            break;
        default:
            st_command->DataMode = HAL_OSPI_DATA_NONE;
            break;
    }

    debug_if(qspi_api_c_debug, "qspi_prepare_command Out: InstructionMode %x Instruction %x AddressMode %x AddressSize %x Address %x DataMode %x\n",
             st_command->InstructionMode, st_command->Instruction, st_command->AddressMode, st_command->AddressSize, st_command->Address, st_command->DataMode);

    return QSPI_STATUS_OK;
}
#else /* OCTOSPI */
qspi_status_t qspi_prepare_command(const qspi_command_t *command, QSPI_CommandTypeDef *st_command)
{
    debug_if(qspi_api_c_debug, "qspi_prepare_command In: instruction.value 0x%" PRIx8 " dummy_count 0x%" PRIx8 " address.bus_width 0x%x address.disabled %d address.value 0x%" PRIx32 " address.size %x\n",
             command->instruction.value, command->dummy_count, command->address.bus_width, command->address.disabled, command->address.value, command->address.size);

    // TODO: shift these around to get more dynamic mapping
    switch (command->instruction.bus_width) {
        case QSPI_CFG_BUS_SINGLE:
            st_command->InstructionMode = QSPI_INSTRUCTION_1_LINE;
            break;
        case QSPI_CFG_BUS_DUAL:
            st_command->InstructionMode = QSPI_INSTRUCTION_2_LINES;
            break;
        case QSPI_CFG_BUS_QUAD:
            st_command->InstructionMode = QSPI_INSTRUCTION_4_LINES;
            break;
        default:
            st_command->InstructionMode = QSPI_INSTRUCTION_NONE;
            break;
    }

    st_command->Instruction = command->instruction.value;
    st_command->DummyCycles = command->dummy_count;
    // these are target specific settings, use default values
    st_command->SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    st_command->DdrMode = QSPI_DDR_MODE_DISABLE;
#if defined(QSPI_DDR_HHC_ANALOG_DELAY)
    st_command->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
#endif

    if (command->address.disabled == true) {
        st_command->AddressMode = QSPI_ADDRESS_NONE;
        st_command->AddressSize = 0;
    } else {
        st_command->Address = command->address.value;
        switch (command->address.bus_width) {
            case QSPI_CFG_BUS_SINGLE:
                st_command->AddressMode = QSPI_ADDRESS_1_LINE;
                break;
            case QSPI_CFG_BUS_DUAL:
                st_command->AddressMode = QSPI_ADDRESS_2_LINES;
                break;
            case QSPI_CFG_BUS_QUAD:
                st_command->AddressMode = QSPI_ADDRESS_4_LINES;
                break;
            default:
                error("Command param error: wrong address size\n");
                return QSPI_STATUS_ERROR;
        }
        switch (command->address.size) {
            case QSPI_CFG_ADDR_SIZE_8:
                st_command->AddressSize = QSPI_ADDRESS_8_BITS;
                break;
            case QSPI_CFG_ADDR_SIZE_16:
                st_command->AddressSize = QSPI_ADDRESS_16_BITS;
                break;
            case QSPI_CFG_ADDR_SIZE_24:
                st_command->AddressSize = QSPI_ADDRESS_24_BITS;
                break;
            case QSPI_CFG_ADDR_SIZE_32:
                st_command->AddressSize = QSPI_ADDRESS_32_BITS;
                break;
            default:
                error("Command param error: wrong address size\n");
                return QSPI_STATUS_ERROR;
        }
    }

    uint8_t alt_lines = 0;
    switch (command->alt.bus_width) {
        case QSPI_CFG_BUS_SINGLE:
            st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_1_LINE;
            alt_lines = 1;
            break;
        case QSPI_CFG_BUS_DUAL:
            st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_2_LINES;
            alt_lines = 2;
            break;
        case QSPI_CFG_BUS_QUAD:
            st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
            alt_lines = 4;
            break;
        default:
            st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
            break;
    }

    if (command->alt.disabled == true) {
        st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
        st_command->AlternateBytesSize = 0;
    } else {
        // Alt size must be a multiple of the number of bus lines used (i.e. a whole number of cycles)
        if ((alt_lines == 0) || (command->alt.size % alt_lines != 0)) {
            return QSPI_STATUS_ERROR;
        }

        // Round up to nearest byte - unused parts of byte act as dummy cycles
        uint32_t alt_bytes = ((command->alt.size - 1) >> 3) + 1;
        // Maximum of 4 alt bytes
        if (alt_bytes > 4) {
            return QSPI_STATUS_ERROR;
        }

        // Unused bits in most significant byte of alt
        uint8_t leftover_bits = (alt_bytes << 3) - command->alt.size;
        if (leftover_bits != 0) {
            // Account for dummy cycles that will be spent in the alt portion of the command
            uint8_t integrated_dummy_cycles = leftover_bits / alt_lines;
            if (st_command->DummyCycles < integrated_dummy_cycles) {
                // Not enough dummy cycles to account for a short alt
                return QSPI_STATUS_ERROR;
            }
            st_command->DummyCycles -= integrated_dummy_cycles;

            // Align alt value to the end of the most significant byte
            st_command->AlternateBytes = command->alt.value << leftover_bits;
        } else {
            st_command->AlternateBytes = command->alt.value;
        }

        st_command->AlternateBytesSize = get_alt_bytes_size(alt_bytes);
    }

    switch (command->data.bus_width) {
        case QSPI_CFG_BUS_SINGLE:
            st_command->DataMode = QSPI_DATA_1_LINE;
            break;
        case QSPI_CFG_BUS_DUAL:
            st_command->DataMode = QSPI_DATA_2_LINES;
            break;
        case QSPI_CFG_BUS_QUAD:
            st_command->DataMode = QSPI_DATA_4_LINES;
            break;
        default:
            st_command->DataMode = QSPI_DATA_NONE;
            break;
    }

    st_command->NbData = 0;

    debug_if(qspi_api_c_debug, "qspi_prepare_command Out: InstructionMode 0x%" PRIx32 " Instruction 0x%" PRIx32 " AddressMode 0x%" PRIx32 " AddressSize 0x%" PRIx32 " Address 0x%" PRIx32 " DataMode %" PRIx32 "\n",
             st_command->InstructionMode, st_command->Instruction, st_command->AddressMode, st_command->AddressSize, st_command->Address, st_command->DataMode);

    return QSPI_STATUS_OK;
}
#endif /* OCTOSPI */


#if defined(OCTOSPI1)

/**
 * Initialize the DMA for an QSPI object
 * Does nothing if DMA is already initialized.
 */
static void qspi_init_dma(struct qspi_s * obj)
{
    if(!obj->dmaInitialized)
    {
        // Get DMA handle
        DMALinkInfo const *dmaLink;
#if defined(OCTOSPI2)
        if(obj->qspi == (QSPIName) OSPI_1)
        {
            dmaLink = &OSPIDMALinks[0];
        }
        else
        {
            dmaLink = &OSPIDMALinks[1];
        }
#else
        dmaLink = &OSPIDMALinks[0];
#endif
        // Initialize DMA channel
        DMAHandlePointer dmaHandle = stm_init_dma_link(dmaLink, DMA_PERIPH_TO_MEMORY, false, true, 1, 1, DMA_NORMAL);
        if(dmaHandle.hdma == NULL)
        {
            mbed_error(MBED_ERROR_ALREADY_IN_USE, "DMA channel already used by something else!", 0, MBED_FILENAME, __LINE__);
        }
#if defined(MDMA)
        __HAL_LINKDMA(&obj->handle, hmdma, *dmaHandle.hmdma);
#else
        __HAL_LINKDMA(&obj->handle, hdma, *dmaHandle.hdma);
#endif
        obj->dmaInitialized = true;
#if MBED_CONF_RTOS_PRESENT
        osSemaphoreAttr_t attr = { 0 };
        attr.cb_mem = &obj->semaphoreMem;
        attr.cb_size = sizeof(osRtxSemaphore_t);
        obj->semaphoreId = osSemaphoreNew(1, 0, &attr);
#endif
    }
}

#if STATIC_PINMAP_READY
#define QSPI_INIT_DIRECT qspi_init_direct
qspi_status_t qspi_init_direct(qspi_t *obj, const qspi_pinmap_t *pinmap, uint32_t hz, uint8_t mode)
#else
#define QSPI_INIT_DIRECT _qspi_init_direct
static qspi_status_t _qspi_init_direct(qspi_t *obj, const qspi_pinmap_t *pinmap, uint32_t hz, uint8_t mode)
#endif
{
    tr_debug("qspi_init mode %u", mode);

    // Reset handle internal state
    obj->handle.State = HAL_OSPI_STATE_RESET;

    // Set default OCTOSPI handle values
    obj->handle.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;
#if defined(TARGET_MX25LM51245G)
    obj->handle.Init.MemoryType = HAL_OSPI_MEMTYPE_MACRONIX; // Read sequence in DTR mode: D1-D0-D3-D2
#else
    obj->handle.Init.MemoryType = HAL_OSPI_MEMTYPE_MICRON;   // Read sequence in DTR mode: D0-D1-D2-D3
#endif
    obj->handle.Init.ClockPrescaler = 4; // default value, will be overwritten in qspi_frequency
    obj->handle.Init.FifoThreshold = 4;
    obj->handle.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_NONE;
    obj->handle.Init.DeviceSize = POSITION_VAL(QSPI_FLASH_SIZE_DEFAULT) - 1;
    obj->handle.Init.ChipSelectHighTime = 3;
    obj->handle.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
#if defined(HAL_OSPI_WRAP_NOT_SUPPORTED) // removed in STM32L4
    obj->handle.Init.WrapSize = HAL_OSPI_WRAP_NOT_SUPPORTED;
#endif
    obj->handle.Init.ClockMode = mode == 0 ? HAL_OSPI_CLOCK_MODE_0 : HAL_OSPI_CLOCK_MODE_3;
    obj->handle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
    obj->handle.Init.ChipSelectBoundary = 0;
#if defined(HAL_OSPI_DELAY_BLOCK_USED) // STM32L5
    obj->handle.Init.DelayBlockBypass = HAL_OSPI_DELAY_BLOCK_USED;
#endif

    // tested all combinations, take first
    obj->qspi = pinmap->peripheral;
    obj->dmaInitialized = false;

#if defined(OCTOSPI1)
    if (obj->qspi == QSPI_1) {
        obj->handle.Instance = OCTOSPI1;
        obj->qspiIRQ = OCTOSPI1_IRQn;
        ospiHandle1 = &obj->handle;
    }
#endif
#if defined(OCTOSPI2)
    if (obj->qspi == QSPI_2) {
        obj->handle.Instance = OCTOSPI2;
        obj->qspiIRQ = OCTOSPI2_IRQn;
        ospiHandle2 = &obj->handle;
    }
#endif

#if defined(OCTOSPI1)
    if (obj->qspi == QSPI_1) {
        __HAL_RCC_OSPI1_CLK_ENABLE();
        __HAL_RCC_OSPI1_FORCE_RESET();
        __HAL_RCC_OSPI1_RELEASE_RESET();
    }
#endif
#if defined(OCTOSPI2)
    if (obj->qspi == QSPI_2) {
        __HAL_RCC_OSPI2_CLK_ENABLE();
        __HAL_RCC_OSPI2_FORCE_RESET();
        __HAL_RCC_OSPI2_RELEASE_RESET();
    }
#endif

    // pinmap for pins (enable clock)
    obj->io0 = pinmap->data0_pin;
    pin_function(pinmap->data0_pin, pinmap->data0_function);
    pin_mode(pinmap->data0_pin, PullNone);
    obj->io1 = pinmap->data1_pin;
    pin_function(pinmap->data1_pin, pinmap->data1_function);
    pin_mode(pinmap->data1_pin, PullNone);
    obj->io2 = pinmap->data2_pin;
    pin_function(pinmap->data2_pin, pinmap->data2_function);
    pin_mode(pinmap->data2_pin, PullNone);
    obj->io3 = pinmap->data3_pin;
    pin_function(pinmap->data3_pin, pinmap->data3_function);
    pin_mode(pinmap->data3_pin, PullNone);

    obj->sclk  = pinmap->sclk_pin;
    pin_function(pinmap->sclk_pin, pinmap->sclk_function);
    pin_mode(pinmap->sclk_pin, PullNone);
    obj->ssel = pinmap->ssel_pin;
    pin_function(pinmap->ssel_pin, pinmap->ssel_function);
    pin_mode(pinmap->ssel_pin, PullNone);

#if defined(OCTOSPI2)
#if defined(__HAL_RCC_OSPIM_CLK_ENABLE)
    __HAL_RCC_OSPIM_CLK_ENABLE();
#endif

    OSPIM_CfgTypeDef OSPIM_Cfg_Struct = {0};

    /* The OctoSPI IO Manager OCTOSPIM configuration is supported in a simplified mode in mbed-os
     * QSPI1 signals are mapped to port 1 and QSPI2 signals are mapped to port 2.
     * This  is coded in this way in PeripheralPins.c */
    if (obj->qspi == QSPI_1) {
        OSPIM_Cfg_Struct.ClkPort = 1;
        OSPIM_Cfg_Struct.DQSPort    = 1;
        OSPIM_Cfg_Struct.NCSPort = 1;
#if QSPI_OSPIM_IOPORT_HIGH
        OSPIM_Cfg_Struct.IOLowPort = HAL_OSPIM_IOPORT_1_HIGH;
        OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_NONE;
#else
        OSPIM_Cfg_Struct.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;
        OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
#endif
    } else {
        OSPIM_Cfg_Struct.ClkPort = 2;
        OSPIM_Cfg_Struct.DQSPort    = 2;
        OSPIM_Cfg_Struct.NCSPort = 2;
#if QSPI_OSPIM_IOPORT_HIGH
        OSPIM_Cfg_Struct.IOLowPort = HAL_OSPIM_IOPORT_2_HIGH;
        OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_NONE;
#else
        OSPIM_Cfg_Struct.IOLowPort = HAL_OSPIM_IOPORT_2_LOW;
        OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_2_HIGH;
#endif
    }

    if (HAL_OSPIM_Config(&obj->handle, &OSPIM_Cfg_Struct, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        tr_error("HAL_OSPIM_Config error");
        return QSPI_STATUS_ERROR;
    }
#endif

    return qspi_frequency(obj, hz);
}

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    QSPIName qspiio0name = (QSPIName)pinmap_peripheral(io0, PinMap_QSPI_DATA0);
    QSPIName qspiio1name = (QSPIName)pinmap_peripheral(io1, PinMap_QSPI_DATA1);
    QSPIName qspiio2name = (QSPIName)pinmap_peripheral(io2, PinMap_QSPI_DATA2);
    QSPIName qspiio3name = (QSPIName)pinmap_peripheral(io3, PinMap_QSPI_DATA3);
    QSPIName qspiclkname = (QSPIName)pinmap_peripheral(sclk, PinMap_QSPI_SCLK);
    QSPIName qspisselname = (QSPIName)pinmap_peripheral(ssel, PinMap_QSPI_SSEL);

    QSPIName qspi_data_first = (QSPIName)pinmap_merge(qspiio0name, qspiio1name);
    QSPIName qspi_data_second = (QSPIName)pinmap_merge(qspiio2name, qspiio3name);
    QSPIName qspi_data_third = (QSPIName)pinmap_merge(qspiclkname, qspisselname);

    if (qspi_data_first != qspi_data_second || qspi_data_second != qspi_data_third ||
            qspi_data_first != qspi_data_third) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    int peripheral = (int)qspi_data_first;
    int function_io0 = (int)pinmap_find_function(io0, PinMap_QSPI_DATA0);
    int function_io1 = (int)pinmap_find_function(io1, PinMap_QSPI_DATA1);
    int function_io2 = (int)pinmap_find_function(io2, PinMap_QSPI_DATA2);
    int function_io3 = (int)pinmap_find_function(io3, PinMap_QSPI_DATA3);
    int function_sclk = (int)pinmap_find_function(sclk, PinMap_QSPI_SCLK);
    int function_ssel = (int)pinmap_find_function(ssel, PinMap_QSPI_SSEL);

    const qspi_pinmap_t static_pinmap = {peripheral, io0, function_io0, io1, function_io1, io2, function_io2, io3, function_io3, sclk, function_sclk, ssel, function_ssel};

    return QSPI_INIT_DIRECT(obj, &static_pinmap, hz, mode);
}
#else /* OCTOSPI */

/**
 * Initialize the DMA for an QSPI object
 * Does nothing if DMA is already initialized.
 */
static void qspi_init_dma(struct qspi_s * obj)
{
    if(!obj->dmaInitialized)
    {
        // Get DMA handle
        DMALinkInfo const *dmaLink = &QSPIDMALinks[0];

        // Initialize DMA channel
        DMAHandlePointer dmaHandle = stm_init_dma_link(dmaLink, DMA_PERIPH_TO_MEMORY, false, true, 1, 1, DMA_NORMAL);
        if(dmaHandle.hdma == NULL)
        {
            mbed_error(MBED_ERROR_ALREADY_IN_USE, "DMA channel already used by something else!", 0, MBED_FILENAME, __LINE__);
        }
#if defined(MDMA)
        __HAL_LINKDMA(&obj->handle, hmdma, *dmaHandle.hmdma);
#else
        __HAL_LINKDMA(&obj->handle, hdma, *dmaHandle.hdma);
#endif
        obj->dmaInitialized = true;
#if MBED_CONF_RTOS_PRESENT
        osSemaphoreAttr_t attr = { 0 };
        attr.cb_mem = &obj->semaphoreMem;
        attr.cb_size = sizeof(osRtxSemaphore_t);
        obj->semaphoreId = osSemaphoreNew(1, 0, &attr);
#endif
    }
}

#if STATIC_PINMAP_READY
#define QSPI_INIT_DIRECT qspi_init_direct
qspi_status_t qspi_init_direct(qspi_t *obj, const qspi_pinmap_t *pinmap, uint32_t hz, uint8_t mode)
#else
#define QSPI_INIT_DIRECT _qspi_init_direct
static qspi_status_t _qspi_init_direct(qspi_t *obj, const qspi_pinmap_t *pinmap, uint32_t hz, uint8_t mode)
#endif
{
    tr_debug("qspi_init mode %u", mode);
    // Enable interface clock for QSPI
    __HAL_RCC_QSPI_CLK_ENABLE();

    // Reset QSPI
#if defined(DUAL_CORE) && (TARGET_STM32H7)
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID)) {
    }
#endif /* DUAL_CORE */
    __HAL_RCC_QSPI_FORCE_RESET();
    __HAL_RCC_QSPI_RELEASE_RESET();
#if defined(DUAL_CORE) && (TARGET_STM32H7)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, HSEM_CR_COREID_CURRENT);
#endif /* DUAL_CORE */

    // Reset handle internal state
    obj->handle.State = HAL_QSPI_STATE_RESET;
    obj->handle.Lock = HAL_UNLOCKED;

    // Set default QSPI handle values
    obj->handle.Init.ClockPrescaler = 1;
    obj->handle.Init.FifoThreshold = 1;
#if defined(QSPI_NO_SAMPLE_SHIFT)
    obj->handle.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
#else
    obj->handle.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
#endif
    obj->handle.Init.FlashSize = POSITION_VAL(QSPI_FLASH_SIZE_DEFAULT) - 1;
    obj->handle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
    obj->handle.Init.ClockMode = QSPI_CLOCK_MODE_0;
#ifdef QSPI_DUALFLASH_ENABLE
    obj->handle.Init.FlashID = QSPI_FLASH_ID_1;
    obj->handle.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
#endif

    obj->handle.Init.ClockMode = mode == 0 ? QSPI_CLOCK_MODE_0 : QSPI_CLOCK_MODE_3;

    // tested all combinations, take first
    obj->handle.Instance = (QUADSPI_TypeDef *)pinmap->peripheral;

    // pinmap for pins (enable clock)
    obj->io0 = pinmap->data0_pin;
    pin_function(pinmap->data0_pin, pinmap->data0_function);
    pin_mode(pinmap->data0_pin, PullNone);
    obj->io1 = pinmap->data1_pin;
    pin_function(pinmap->data1_pin, pinmap->data1_function);
    pin_mode(pinmap->data1_pin, PullNone);
    obj->io2 = pinmap->data2_pin;
    pin_function(pinmap->data2_pin, pinmap->data2_function);
    pin_mode(pinmap->data2_pin, PullNone);
    obj->io3 = pinmap->data3_pin;
    pin_function(pinmap->data3_pin, pinmap->data3_function);
    pin_mode(pinmap->data3_pin, PullNone);

    obj->sclk  = pinmap->sclk_pin;
    pin_function(pinmap->sclk_pin, pinmap->sclk_function);
    pin_mode(pinmap->sclk_pin, PullNone);
    obj->ssel = pinmap->ssel_pin;
    pin_function(pinmap->ssel_pin, pinmap->ssel_function);
    pin_mode(pinmap->ssel_pin, PullNone);

    qspiHandle = &obj->handle;

    return qspi_frequency(obj, hz);
}

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    QSPIName qspiio0name = (QSPIName)pinmap_peripheral(io0, PinMap_QSPI_DATA0);
    QSPIName qspiio1name = (QSPIName)pinmap_peripheral(io1, PinMap_QSPI_DATA1);
    QSPIName qspiio2name = (QSPIName)pinmap_peripheral(io2, PinMap_QSPI_DATA2);
    QSPIName qspiio3name = (QSPIName)pinmap_peripheral(io3, PinMap_QSPI_DATA3);
    QSPIName qspiclkname = (QSPIName)pinmap_peripheral(sclk, PinMap_QSPI_SCLK);
    QSPIName qspisselname = (QSPIName)pinmap_peripheral(ssel, PinMap_QSPI_SSEL);

    QSPIName qspi_data_first = (QSPIName)pinmap_merge(qspiio0name, qspiio1name);
    QSPIName qspi_data_second = (QSPIName)pinmap_merge(qspiio2name, qspiio3name);
    QSPIName qspi_data_third = (QSPIName)pinmap_merge(qspiclkname, qspisselname);

    if (qspi_data_first != qspi_data_second || qspi_data_second != qspi_data_third ||
            qspi_data_first != qspi_data_third) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    int peripheral = (int)qspi_data_first;
    int function_io0 = (int)pinmap_find_function(io0, PinMap_QSPI_DATA0);
    int function_io1 = (int)pinmap_find_function(io1, PinMap_QSPI_DATA1);
    int function_io2 = (int)pinmap_find_function(io2, PinMap_QSPI_DATA2);
    int function_io3 = (int)pinmap_find_function(io3, PinMap_QSPI_DATA3);
    int function_sclk = (int)pinmap_find_function(sclk, PinMap_QSPI_SCLK);
    int function_ssel = (int)pinmap_find_function(ssel, PinMap_QSPI_SSEL);

    const qspi_pinmap_t static_pinmap = {peripheral, io0, function_io0, io1, function_io1, io2, function_io2, io3, function_io3, sclk, function_sclk, ssel, function_ssel};

    return QSPI_INIT_DIRECT(obj, &static_pinmap, hz, mode);
}

#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_free(qspi_t *obj)
{
    tr_debug("qspi_free");

    if(obj->dmaInitialized)
    {
        // Get DMA handle
        DMALinkInfo const *dmaLink;
#if defined(OCTOSPI2)
        if(obj->qspi == (QSPIName) OSPI_1)
        {
            dmaLink = &OSPIDMALinks[0];
        }
        else
        {
            dmaLink = &OSPIDMALinks[1];
        }
#else
        dmaLink = &OSPIDMALinks[0];
#endif
        stm_free_dma_link(dmaLink);
    }

    if (HAL_OSPI_DeInit(&obj->handle) != HAL_OK) {
        return QSPI_STATUS_ERROR;
    }

#if defined(OCTOSPI1)
    if (obj->qspi == QSPI_1) {
        __HAL_RCC_OSPI1_FORCE_RESET();
        __HAL_RCC_OSPI1_CLK_DISABLE();
    }
#endif
#if defined(OCTOSPI2)
    if (obj->qspi == QSPI_2) {
        __HAL_RCC_OSPI2_FORCE_RESET();
        __HAL_RCC_OSPI2_CLK_DISABLE();
    }
#endif

    // Configure GPIOs back to reset value
    pin_function(obj->io0, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io1, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io2, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io3, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->sclk, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->ssel, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));

    (void)(obj);
    return QSPI_STATUS_OK;
}
#else /* OCTOSPI */
qspi_status_t qspi_free(qspi_t *obj)
{
    tr_debug("qspi_free");

    if(obj->dmaInitialized)
    {
        // Get DMA handle
        DMALinkInfo const *dmaLink = &QSPIDMALinks[0];
        stm_free_dma_link(dmaLink);

#if MBED_CONF_RTOS_PRESENT
        // Free semaphore
        osSemaphoreRelease(obj->semaphoreId);
#endif
    }

    if (HAL_QSPI_DeInit(&obj->handle) != HAL_OK) {
        return QSPI_STATUS_ERROR;
    }

    // Reset QSPI
#if defined(DUAL_CORE) && (TARGET_STM32H7)
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID)) {
    }
#endif /* DUAL_CORE */
    __HAL_RCC_QSPI_FORCE_RESET();
    __HAL_RCC_QSPI_RELEASE_RESET();
#if defined(DUAL_CORE) && (TARGET_STM32H7)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, HSEM_CR_COREID_CURRENT);
#endif /* DUAL_CORE */

    // Disable interface clock for QSPI
    __HAL_RCC_QSPI_CLK_DISABLE();

    // Configure GPIOs
    pin_function(obj->io0, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io1, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io2, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io3, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->sclk, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->ssel, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));

    (void)(obj);
    return QSPI_STATUS_OK;
}
#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    tr_debug("qspi_frequency hz %d", hz);
    qspi_status_t status = QSPI_STATUS_OK;

    // Reset flag used by store_qspi_pointer()
    obj->handle.Init.ChipSelectHighTime = 3;

    /* HCLK drives QSPI. QSPI clock depends on prescaler value:
    *  0: Freq = HCLK
    *  1: Freq = HCLK/2
    *  ...
    *  255: Freq = HCLK/256 (minimum value)
    */

    int div = HAL_RCC_GetHCLKFreq() / hz;
    if (div > 255) {
        div = 255;
    }

    obj->handle.Init.ClockPrescaler = div;

    HAL_OSPI_DeInit(&obj->handle);
    if (HAL_OSPI_Init(&obj->handle) != HAL_OK) {
        tr_error("HAL_OSPI_Init error");
        status = QSPI_STATUS_ERROR;
    }

    store_qspi_pointer(&obj->handle, obj);

    return status;
}
#else /* OCTOSPI */
qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    tr_debug("qspi_frequency hz %d", hz);
    qspi_status_t status = QSPI_STATUS_OK;

    // Reset flag used by store_qspi_pointer()
    obj->handle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;

    /* HCLK drives QSPI. QSPI clock depends on prescaler value:
    *  0: Freq = HCLK
    *  1: Freq = HCLK/2
    *  ...
    *  255: Freq = HCLK/256 (minimum value)
    */

    int div = HAL_RCC_GetHCLKFreq() / hz;
    if (div > 255) {
        div = 255;
    } else {
        if ((HAL_RCC_GetHCLKFreq() % hz) == 0) {
            div = div - 1;
        }
    }

    obj->handle.Init.ClockPrescaler = div;

    if (HAL_QSPI_Init(&obj->handle) != HAL_OK) {
        status = QSPI_STATUS_ERROR;
    }

    store_qspi_pointer(&obj->handle, obj);

    return status;
}
#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    debug_if(qspi_api_c_debug, "qspi_write size %u\n", *length);

    OSPI_RegularCmdTypeDef st_command;
    qspi_status_t status = qspi_prepare_command(command, &st_command);
    if (status != QSPI_STATUS_OK) {
        return status;
    }

    st_command.NbData = *length;

    if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        tr_error("HAL_OSPI_Command error");
        status = QSPI_STATUS_ERROR;
    } else {
        if(st_command.NbData >= QSPI_DMA_THRESHOLD_BYTES) {
            qspi_init_dma(obj);
            NVIC_ClearPendingIRQ(obj->qspiIRQ);
            NVIC_SetPriority(obj->qspiIRQ, 1);
            NVIC_EnableIRQ(obj->qspiIRQ);
#if defined(__DCACHE_PRESENT)
            // For chips with a cache (e.g. Cortex-M7), we need to evict the Tx fill data from cache to main memory.
            // This ensures that the DMA controller can see the most up-to-date copy of the data.
            SCB_CleanDCache_by_Addr((volatile void *)data, *length);
#endif
            if (HAL_OSPI_Transmit_DMA(&obj->handle, (uint8_t *)data) != HAL_OK) {
                tr_error("HAL_OSPI_Transmit error");
                status = QSPI_STATUS_ERROR;
            }
            else {
                // wait until transfer complete or timeout
#if MBED_CONF_RTOS_PRESENT
                osSemaphoreAcquire(obj->semaphoreId, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
#else
                while(obj->handle.State == HAL_OSPI_STATE_BUSY_TX);
#endif
                if(obj->handle.State != HAL_OSPI_STATE_READY) {
                    status = QSPI_STATUS_ERROR;
                    obj->handle.State = HAL_OSPI_STATE_READY;
                }
            }
            NVIC_DisableIRQ(obj->qspiIRQ);
        }
        else {
            if (HAL_OSPI_Transmit(&obj->handle, (uint8_t *)data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
                tr_error("HAL_OSPI_Transmit error");
                status = QSPI_STATUS_ERROR;
            }
        }
    }

    return status;
}
#else /* OCTOSPI */
qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    debug_if(qspi_api_c_debug, "qspi_write size %u\n", *length);

    QSPI_CommandTypeDef st_command;
    qspi_status_t status = qspi_prepare_command(command, &st_command);
    if (status != QSPI_STATUS_OK) {
        return status;
    }

    st_command.NbData = *length;

    if (HAL_QSPI_Command(&obj->handle, &st_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        status = QSPI_STATUS_ERROR;
    } else {
        if(st_command.NbData >= QSPI_DMA_THRESHOLD_BYTES) {
            qspi_init_dma(obj);
            NVIC_ClearPendingIRQ(QUADSPI_IRQn);
            NVIC_SetPriority(QUADSPI_IRQn, 1);
            NVIC_EnableIRQ(QUADSPI_IRQn);
#if defined(__DCACHE_PRESENT)
            // For chips with a cache (e.g. Cortex-M7), we need to evict the Tx fill data from cache to main memory.
            // This ensures that the DMA controller can see the most up-to-date copy of the data.
            SCB_CleanDCache_by_Addr((volatile void *)data, *length);
#endif
            if (HAL_QSPI_Transmit_DMA(&obj->handle, (uint8_t *)data) != HAL_OK) {
                status = QSPI_STATUS_ERROR;
            }
            else {
                // wait until transfer complete or timeout
#if MBED_CONF_RTOS_PRESENT
                osSemaphoreAcquire(obj->semaphoreId, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
#else
                while(obj->handle.State == HAL_QSPI_STATE_BUSY_INDIRECT_TX);
#endif
                if(obj->handle.State != HAL_QSPI_STATE_READY) {
                    status = QSPI_STATUS_ERROR;
                    obj->handle.State = HAL_QSPI_STATE_READY;
                }
            }
            NVIC_DisableIRQ(QUADSPI_IRQn);
        }
        else {
            if (HAL_QSPI_Transmit(&obj->handle, (uint8_t *)data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
                status = QSPI_STATUS_ERROR;
            }
        }
    }

    return status;
}
#endif /* OCTOSPI */

#if defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
static void split_buffer_by_cacheline(void *buffer, const size_t *length, size_t *pre_aligned_size, size_t *aligned_size, size_t *post_aligned_size)
{
    *pre_aligned_size = 0;
    *aligned_size = 0;
    *post_aligned_size = 0;
    if(*length < __SCB_DCACHE_LINE_SIZE)
    {
        *pre_aligned_size = *length;
    }
    else
    {
        size_t address_remainder = (size_t) buffer % __SCB_DCACHE_LINE_SIZE;
        if(address_remainder == 0)
        {
            *aligned_size = *length & ~(__SCB_DCACHE_LINE_SIZE - 1);
            *post_aligned_size = *length - *aligned_size;
        }
        else
        {
            *pre_aligned_size = __SCB_DCACHE_LINE_SIZE - address_remainder;
            *aligned_size = (*length - *pre_aligned_size) & ~(__SCB_DCACHE_LINE_SIZE - 1);
            *post_aligned_size = *length - *pre_aligned_size - *aligned_size;
        }
        if(*aligned_size == 0)
        {
            *pre_aligned_size = *length;
            *post_aligned_size = 0;
        }
    }
}
#endif


#if defined(OCTOSPI1)
qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    OSPI_RegularCmdTypeDef st_command;
    qspi_status_t status = qspi_prepare_command(command, &st_command);
    if (status != QSPI_STATUS_OK) {
        return status;
    }

#if defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    size_t pre_aligned_size, aligned_size, post_aligned_size;
    split_buffer_by_cacheline(data, length, &pre_aligned_size, &aligned_size, &post_aligned_size);
    if(pre_aligned_size > 0)
    {
        st_command.NbData = pre_aligned_size;
        if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            tr_error("HAL_OSPI_Command error");
            status = QSPI_STATUS_ERROR;
        } else {
            if (HAL_OSPI_Receive(&obj->handle, data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
                tr_error("HAL_OSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
        }
        st_command.Address += pre_aligned_size;
        data += pre_aligned_size;
    }
    if(status == QSPI_STATUS_OK && aligned_size > 0)
    {
        st_command.NbData = aligned_size;
        if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            tr_error("HAL_OSPI_Command error");
            status = QSPI_STATUS_ERROR;
        } else {
            qspi_init_dma(obj);
            NVIC_ClearPendingIRQ(obj->qspiIRQ);
            NVIC_SetPriority(obj->qspiIRQ, 1);
            NVIC_EnableIRQ(obj->qspiIRQ);
            SCB_CleanInvalidateDCache_by_Addr((volatile void *)data, *length);
            if (HAL_OSPI_Receive_DMA(&obj->handle, data) != HAL_OK) {
                tr_error("HAL_OSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
            else {
                // wait until transfer complete or timeout
#if MBED_CONF_RTOS_PRESENT
                osSemaphoreAcquire(obj->semaphoreId, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
#else
                while(obj->handle.State == HAL_OSPI_STATE_BUSY_RX);
#endif
                if(obj->handle.State != HAL_OSPI_STATE_READY) {
                    status = QSPI_STATUS_ERROR;
                    obj->handle.State = HAL_OSPI_STATE_READY;
                }
            }
            NVIC_DisableIRQ(obj->qspiIRQ);
        }
        st_command.Address += aligned_size;
        data += aligned_size;
    }
    if(status == QSPI_STATUS_OK && post_aligned_size > 0)
    {
        st_command.NbData = post_aligned_size;
        if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            tr_error("HAL_OSPI_Command error");
            status = QSPI_STATUS_ERROR;
        } else {
            if (HAL_OSPI_Receive(&obj->handle, data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
                tr_error("HAL_OSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
        }
    }
#else
    st_command.NbData = *length;

    if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        tr_error("HAL_OSPI_Command error");
        status = QSPI_STATUS_ERROR;
    } else {
        if(st_command.NbData >= QSPI_DMA_THRESHOLD_BYTES) {
            qspi_init_dma(obj);
            NVIC_ClearPendingIRQ(obj->qspiIRQ);
            NVIC_SetPriority(obj->qspiIRQ, 1);
            NVIC_EnableIRQ(obj->qspiIRQ);
            if (HAL_OSPI_Receive_DMA(&obj->handle, data) != HAL_OK) {
                tr_error("HAL_OSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
            else {
                // wait until transfer complete or timeout
#if MBED_CONF_RTOS_PRESENT
                osSemaphoreAcquire(obj->semaphoreId, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
#else
                while(obj->handle.State == HAL_OSPI_STATE_BUSY_RX);
#endif
                if(obj->handle.State != HAL_OSPI_STATE_READY) {
                    status = QSPI_STATUS_ERROR;
                    obj->handle.State = HAL_OSPI_STATE_READY;
                }
            }
            NVIC_DisableIRQ(obj->qspiIRQ);
        }
        else {
            if (HAL_OSPI_Receive(&obj->handle, data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
                tr_error("HAL_OSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
        }
    }
#endif
    debug_if(qspi_api_c_debug, "qspi_read size %u\n", *length);

    return status;
}
#else /* OCTOSPI */
qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    QSPI_CommandTypeDef st_command;
    qspi_status_t status = qspi_prepare_command(command, &st_command);
    if (status != QSPI_STATUS_OK) {
        return status;
    }

#if defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    size_t pre_aligned_size, aligned_size, post_aligned_size;
    split_buffer_by_cacheline(data, length, &pre_aligned_size, &aligned_size, &post_aligned_size);
    if(pre_aligned_size > 0)
    {
        st_command.NbData = pre_aligned_size;
        if (HAL_QSPI_Command(&obj->handle, &st_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            tr_error("HAL_QSPI_Command error");
            status = QSPI_STATUS_ERROR;
        } else {
            if (HAL_QSPI_Receive(&obj->handle, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
                tr_error("HAL_QSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
        }
        st_command.Address += pre_aligned_size;
        data += pre_aligned_size;
    }
    if(status == QSPI_STATUS_OK && aligned_size > 0)
    {
        st_command.NbData = aligned_size;
    if (HAL_QSPI_Command(&obj->handle, &st_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            tr_error("HAL_QSPI_Command error");
        status = QSPI_STATUS_ERROR;
    } else {
            qspi_init_dma(obj);
            NVIC_ClearPendingIRQ(QUADSPI_IRQn);
            NVIC_SetPriority(QUADSPI_IRQn, 1);
            NVIC_EnableIRQ(QUADSPI_IRQn);
            SCB_CleanInvalidateDCache_by_Addr((volatile void *)data, *length);
            if (HAL_QSPI_Receive_DMA(&obj->handle, data) != HAL_OK) {
                tr_error("HAL_QSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
            else {
                // wait until transfer complete or timeout
#if MBED_CONF_RTOS_PRESENT
                osSemaphoreAcquire(obj->semaphoreId, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
#else
                while(obj->handle.State == HAL_QSPI_STATE_BUSY_INDIRECT_RX);
#endif
                if(obj->handle.State != HAL_QSPI_STATE_READY) {
                    status = QSPI_STATUS_ERROR;
                    obj->handle.State = HAL_QSPI_STATE_READY;
                }
            }
            NVIC_DisableIRQ(QUADSPI_IRQn);
        }
        st_command.Address += aligned_size;
        data += aligned_size;
    }
    if(status == QSPI_STATUS_OK && post_aligned_size > 0)
    {
        st_command.NbData = post_aligned_size;
        if (HAL_QSPI_Command(&obj->handle, &st_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            tr_error("HAL_QSPI_Command error");
            status = QSPI_STATUS_ERROR;
        } else {
            if (HAL_QSPI_Receive(&obj->handle, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
                tr_error("HAL_QSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
        }
            }
#else
    st_command.NbData = *length;

    if (HAL_QSPI_Command(&obj->handle, &st_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        tr_error("HAL_QSPI_Command error");
        status = QSPI_STATUS_ERROR;
    } else {
        if(st_command.NbData >= QSPI_DMA_THRESHOLD_BYTES) {
            qspi_init_dma(obj);
            NVIC_ClearPendingIRQ(QUADSPI_IRQn);
            NVIC_SetPriority(QUADSPI_IRQn, 1);
            NVIC_EnableIRQ(QUADSPI_IRQn);
            if (HAL_QSPI_Receive_DMA(&obj->handle, data) != HAL_OK) {
                tr_error("HAL_QSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
            else {
                // wait until transfer complete or timeout
#if MBED_CONF_RTOS_PRESENT
                osSemaphoreAcquire(obj->semaphoreId, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
#else
                while(obj->handle.State == HAL_QSPI_STATE_BUSY_INDIRECT_RX);
#endif
                if(obj->handle.State != HAL_QSPI_STATE_READY) {
                    status = QSPI_STATUS_ERROR;
                    obj->handle.State = HAL_QSPI_STATE_READY;
                }
            }
            NVIC_DisableIRQ(QUADSPI_IRQn);
        }
        else {
            if (HAL_QSPI_Receive(&obj->handle, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
                tr_error("HAL_QSPI_Receive error %d", obj->handle.ErrorCode);
                status = QSPI_STATUS_ERROR;
            }
        }
    }
#endif
    debug_if(qspi_api_c_debug, "qspi_read size %u\n", *length);

    return status;
}
#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    tr_debug("qspi_command_transfer tx %u rx %u command %#04x", tx_size, rx_size, command->instruction.value);

    qspi_status_t status = QSPI_STATUS_OK;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0)) {
        // only command, no rx or tx
        OSPI_RegularCmdTypeDef st_command;
        status = qspi_prepare_command(command, &st_command);
        if (status != QSPI_STATUS_OK) {
            return status;
        }

        st_command.NbData = 1;
        st_command.DataMode = HAL_OSPI_DATA_NONE; /* Instruction only */
        if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            status = QSPI_STATUS_ERROR;
            tr_error("HAL_OSPI_Command error");
            return status;
        }
    } else {
        // often just read a register, check if we need to transmit anything prior reading
        if (tx_data != NULL && tx_size) {
            size_t tx_length = tx_size;
            status = qspi_write(obj, command, tx_data, &tx_length);
            if (status != QSPI_STATUS_OK) {
                tr_error("qspi_write error");
                return status;
            }
        }

        if (rx_data != NULL && rx_size) {
            size_t rx_length = rx_size;
            status = qspi_read(obj, command, rx_data, &rx_length);
        }
    }
    return status;
}
#else /* OCTOSPI */
qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    tr_debug("qspi_command_transfer tx %u rx %u command %#04x", tx_size, rx_size, command->instruction.value);
    qspi_status_t status = QSPI_STATUS_OK;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0)) {
        // only command, no rx or tx
        QSPI_CommandTypeDef st_command;
        status = qspi_prepare_command(command, &st_command);
        if (status != QSPI_STATUS_OK) {
            return status;
        }

        st_command.NbData = 1;
        st_command.DataMode = QSPI_DATA_NONE; /* Instruction only */
        if (HAL_QSPI_Command(&obj->handle, &st_command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            status = QSPI_STATUS_ERROR;
            return status;
        }
    } else {
        // often just read a register, check if we need to transmit anything prior reading
        if (tx_data != NULL && tx_size) {
            size_t tx_length = tx_size;
            status = qspi_write(obj, command, tx_data, &tx_length);
            if (status != QSPI_STATUS_OK) {
                return status;
            }
        }

        if (rx_data != NULL && rx_size) {
            size_t rx_length = rx_size;
            status = qspi_read(obj, command, rx_data, &rx_length);
        }
    }
    return status;
}
#endif /* OCTOSPI */


const PinMap *qspi_master_sclk_pinmap()
{
    return PinMap_QSPI_SCLK;
}

const PinMap *qspi_master_ssel_pinmap()
{
    return PinMap_QSPI_SSEL;
}

const PinMap *qspi_master_data0_pinmap()
{
    return PinMap_QSPI_DATA0;
}

const PinMap *qspi_master_data1_pinmap()
{
    return PinMap_QSPI_DATA1;
}

const PinMap *qspi_master_data2_pinmap()
{
    return PinMap_QSPI_DATA2;
}

const PinMap *qspi_master_data3_pinmap()
{
    return PinMap_QSPI_DATA3;
}

#endif

/** @}*/

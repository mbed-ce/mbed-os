/*
 * Copyright 2020 Cypress Semiconductor Corporation
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

/** @file
 *  Provides the porting layer for STM32 SDIO on WHD driver
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "cyabs_rtos.h"
#include "cyhal_sdio.h"
#include <cyhal.h>

#include <minimal_cyhal_config.h>

// Debug print control
#define SDIO_DEBUG 0
#if SDIO_DEBUG
#define SDIO_PRINT_DEBUG(...) printf(__VA_ARGS__)
#else
#define SDIO_PRINT_DEBUG(...) 
#endif

#define SDIO_RESPONSE_SHORT             SDMMC_RESPONSE_SHORT
#define SDIO_WAIT_NO                    SDMMC_WAIT_NO
#define SDIO_CPSM_ENABLE                SDMMC_CPSM_ENABLE
#define SDIO_CMDTRANS                   SDMMC_CMD_CMDTRANS
#define SDIO_FLAG_CMDACT                (SDMMC_STA_CPSMACT | SDMMC_STA_DPSMACT)
#define SDIO_TRANSFER_DIR_TO_SDIO       SDMMC_TRANSFER_DIR_TO_SDMMC
#define SDIO_TRANSFER_DIR_TO_CARD       SDMMC_TRANSFER_DIR_TO_CARD
#define SDIO_TRANSFER_MODE_BLOCK        SDMMC_TRANSFER_MODE_BLOCK
#define SDIO_DPSM_ENABLE                SDMMC_DPSM_ENABLE
#define SDIO_DPSM_DISABLE               SDMMC_DPSM_DISABLE
#define SDIO_DCTRL_SDIOEN               SDMMC_DCTRL_SDIOEN       /* 1 << 11 */
#define SDIO_DCTRL_DTMODE_1             SDMMC_DCTRL_DTMODE_1     /* 1<<3 */
#define SDIO_STA_DTIMEOUT               SDMMC_STA_DTIMEOUT
#define SDIO_STA_CTIMEOUT               SDMMC_STA_CTIMEOUT
#define SDIO_STA_CCRCFAIL               SDMMC_STA_CCRCFAIL
#define SDIO_STA_DCRCFAIL               SDMMC_STA_DCRCFAIL
#define SDIO_STA_TXUNDERR               SDMMC_STA_TXUNDERR
#define SDIO_STA_RXOVERR                SDMMC_STA_RXOVERR
#define SDIO_STA_TXACT                  SDMMC_STA_CPSMACT
#define SDIO_STA_RXACT                  SDMMC_STA_DPSMACT
#define SDIO_STA_CMDREND                SDMMC_STA_CMDREND
#define SDIO_STA_CMDSENT                SDMMC_STA_CMDSENT
#define SDIO_CMD_CMDTRANS               SDMMC_CMD_CMDTRANS
#define SDIO                            SDMMC1

#define COMMAND_FINISHED_CMD52_TIMEOUT_LOOPS (100000)
#define BUS_LEVEL_MAX_RETRIES                      10

#define SDIO_CMD_5      5
#define SDIO_CMD_53     53

#define  SDIO_STA_STBITERR               ((uint32_t)0x00000200)        /*!<Start bit not detected on all data signals in wide bus mode */
#define SDIO_ERROR_MASK                  ( SDMMC_STA_CCRCFAIL | SDMMC_STA_DCRCFAIL | SDMMC_STA_CTIMEOUT | SDMMC_STA_DTIMEOUT | SDMMC_STA_TXUNDERR | SDMMC_STA_RXOVERR )

#define LINK_MTU        1024
#define MAX(a,b)        (a>b)?a:b

/* D-cache maintenance for DMA buffers */
#if defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    #define _CYHAL_DCACHE_MAINTENANCE
    #define _CYHAL_DMA_BUFFER_ALIGN_BYTES      (32u)
#else
    #define _CYHAL_DMA_BUFFER_ALIGN_BYTES      (4u)
#endif /* defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) */

/* Macro to ALIGN */
#if defined (__ARMCC_VERSION) /* ARM Compiler */
    #define ALIGN_HAL_COMMON(buf, x) __align(x) buf
#elif defined   (__GNUC__)    /* GNU Compiler */
    #define ALIGN_HAL_COMMON(buf, x)  buf __attribute__ ((aligned (x)))
#elif defined (__ICCARM__)    /* IAR Compiler */
    #define ALIGN_HAL_COMMON(buf, x) __ALIGNED(x) buf
#endif

/* Macro to get variable aligned for cache maintenance purpose */
#define CYHAL_ALIGN_DMA_BUFFER(arg) ALIGN_HAL_COMMON(arg, _CYHAL_DMA_BUFFER_ALIGN_BYTES)

/* Pin configuration */
typedef struct
{
  GPIO_TypeDef      *port;
  GPIO_InitTypeDef  config;
} sdio_pinconfig_t;

sdio_pinconfig_t SDIOPinConfig[] = {
    [CYBSP_WIFI_SDIO_CMD] =  WIFI_SDIO_CMD,
    [CYBSP_WIFI_SDIO_CLK] =  WIFI_SDIO_CLK,
    [CYBSP_WIFI_SDIO_D0 ] =  WIFI_SDIO_D0,
    [CYBSP_WIFI_SDIO_D1 ] =  WIFI_SDIO_D1,
    [CYBSP_WIFI_SDIO_D2 ] =  WIFI_SDIO_D2,
    [CYBSP_WIFI_SDIO_D3 ] =  WIFI_SDIO_D3,
};

extern  SD_HandleTypeDef hsd;

// Copied from whd_bus_sdio_protocol.h
typedef struct
{
    unsigned int count            : 9;  /* 0-8   */
    unsigned int register_address : 17; /* 9-25  */
    unsigned int op_code          : 1;  /* 26    */
    unsigned int block_mode       : 1;  /* 27    */
    unsigned int function_number  : 3;  /* 28-30 */
    unsigned int rw_flag          : 1;  /* 31    */
} whd_bus_sdio_cmd53_argument_t;
typedef union
{
    uint32_t value;
    whd_bus_sdio_cmd53_argument_t cmd53;
} sdio_cmd_argument_t;

/* for debug prints only */
//static int num = 0;

static uint32_t      dctrl;

static void* irq_handler_arg;
static cyhal_sdio_irq_handler_t sdio_irq_handler;

CYHAL_ALIGN_DMA_BUFFER(static uint8_t       temp_dma_buffer[2048]);
static uint8_t                     *user_data;
static uint32_t                     user_data_size;
static uint8_t                     *dma_data_source;
static uint32_t                     dma_transfer_size;
static cyhal_transfer_t    current_transfer_direction;

static cy_semaphore_t sdio_transfer_finished_semaphore;
static volatile uint32_t       sdio_transfer_failed = 0;
static volatile uint32_t       irqstatus = 0;
static int current_command = 0;

cy_rslt_t sdio_enable_high_speed(void)
{
    SDMMC_InitTypeDef sdio_init_structure;

    sdio_init_structure.ClockDiv       = SDMMC_HSpeed_CLK_DIV;
    sdio_init_structure.ClockEdge      = SDMMC_CLOCK_EDGE_RISING;
    sdio_init_structure.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    sdio_init_structure.BusWide        = SDMMC_BUS_WIDE_4B;
    sdio_init_structure.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;

    SDMMC_Init(SDMMC1, sdio_init_structure);
    return CY_RSLT_SUCCESS;
}

static uint32_t find_optimal_block_size(uint32_t data_size)
{
    if (data_size > (uint32_t) 256) {
        return 512;
    }
    if (data_size > (uint32_t) 128) {
        return 256;
    }
    if (data_size > (uint32_t) 64) {
        return 128;
    }
    if (data_size > (uint32_t) 32) {
        return 64;
    }
    if (data_size > (uint32_t) 16) {
        return 32;
    }
    if (data_size > (uint32_t) 8) {
        return 16;
    }
    if (data_size > (uint32_t) 4) {
        return 8;
    }
    if (data_size > (uint32_t) 2) {
        return 4;
    }

    return 4;
}

static uint32_t sdio_get_blocksize(uint32_t blocksize)
{
    uint32_t      n = 0;
    blocksize >>= 1;
    while (blocksize) {
        n++;
        blocksize >>= 1;
    }
    n <<= 4;
    return n;
}

static void sdio_prepare_data_transfer(cyhal_transfer_t direction, uint32_t block_size, uint8_t *data, uint16_t data_size)
{
    /* Setup a single transfer using the temp buffer */
    user_data         = data;
    user_data_size    = data_size;
    dma_transfer_size = (uint32_t)(((data_size + (uint16_t) block_size - 1) / (uint16_t) block_size) * (uint16_t) block_size);

    if (direction == CYHAL_WRITE) {
        memcpy(temp_dma_buffer, data, data_size);
        dma_data_source = temp_dma_buffer;
    } else {
        dma_data_source = (uint8_t *) temp_dma_buffer;
    }

#ifdef _CYHAL_DCACHE_MAINTENANCE
    if (direction == CYHAL_WRITE)
    {
        SCB_CleanDCache_by_Addr((uint32_t*)dma_data_source, block_size * dma_transfer_size);
    }
    else
    {
        /* Cache-Invalidate the output from DMA */
        SCB_InvalidateDCache_by_Addr((uint32_t*)dma_data_source,
                                        data_size + __SCB_DCACHE_LINE_SIZE);
    }
#endif

    SDMMC1->DTIMER = (uint32_t) 0xFFFFFFFF;
    SDMMC1->DLEN   = dma_transfer_size;
    dctrl = sdio_get_blocksize(block_size) | ((direction == CYHAL_READ) ? SDIO_TRANSFER_DIR_TO_SDIO : SDIO_TRANSFER_DIR_TO_CARD) | SDIO_TRANSFER_MODE_BLOCK | SDIO_DPSM_DISABLE  | SDIO_DCTRL_SDIOEN;
    SDMMC1->DCTRL = dctrl;

    SDMMC1->IDMACTRL  = SDMMC_ENABLE_IDMA_SINGLE_BUFF;
    SDMMC1->IDMABASE0 = (uint32_t) dma_data_source;
}

static void sdio_enable_bus_irq(void)
{
    SDMMC1->MASK = SDIO_ERROR_MASK | SDMMC_IT_DATAEND | SDMMC_IT_CMDREND | SDMMC_IT_CMDSENT;
}

static void sdio_disable_bus_irq(void)
{
    SDMMC1->MASK = 0;
}

void SDMMC1_IRQHandler(void)
{
    uint32_t intstatus = SDIO->STA;

    /* Check whether the external interrupt was triggered */
    if (intstatus & SDMMC_STA_SDIOIT) {
        /* Clear the interrupt */
        SDMMC1->ICR = SDMMC_STA_SDIOIT;
        /* Inform WICED WWD thread */
        sdio_irq_handler(irq_handler_arg, CYHAL_SDIO_CARD_INTERRUPT);
    }

    irqstatus = intstatus;
    if ((intstatus & SDIO_ERROR_MASK) != 0) {
        sdio_transfer_failed = true;
        SDIO->ICR = (uint32_t) 0xffffffff;
        cy_rtos_set_semaphore(&sdio_transfer_finished_semaphore, true);
    } else {
        if ((intstatus & (SDMMC_STA_CMDREND | SDMMC_STA_CMDSENT)) != 0) {
            if ((SDMMC1->RESP1 & 0x800) != 0) {
                sdio_transfer_failed = true;
                cy_rtos_set_semaphore(&sdio_transfer_finished_semaphore, true);
            }

            /* Clear all command/response interrupts */
            SDMMC1->ICR = (SDMMC_STA_CMDREND | SDMMC_STA_CMDSENT);
        }

        if (intstatus & SDMMC_STA_DATAEND) {
            SDMMC1->ICR      = SDMMC_STA_DATAEND;
            SDMMC1->DLEN     = 0;
            SDMMC1->DCTRL    = SDMMC_DCTRL_SDIOEN;
            SDMMC1->IDMACTRL = SDMMC_DISABLE_IDMA;
            SDMMC1->CMD      = 0;
            sdio_transfer_failed = false;
            cy_rtos_set_semaphore(&sdio_transfer_finished_semaphore, true);
        }
    }
}

//void  sdio_dma_irq(void)
//{
//    /* Clear interrupt */
//    DMA2->LIFCR = (uint32_t)(0x3F << 22);
//    cy_rtos_set_semaphore(&sdio_transfer_finished_semaphore, WHD_TRUE);
//}

cy_rslt_t cyhal_sdio_init(cyhal_sdio_t *obj, cyhal_gpio_t cmd, cyhal_gpio_t clk, cyhal_gpio_t data0, cyhal_gpio_t data1, cyhal_gpio_t data2, cyhal_gpio_t data3)
{
    cy_rslt_t     ret = CY_RSLT_SUCCESS;

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_SDMMC1_CLK_ENABLE();

    HAL_GPIO_Init(SDIOPinConfig[cmd].port, &SDIOPinConfig[cmd].config);
    HAL_GPIO_Init(SDIOPinConfig[clk].port, &SDIOPinConfig[clk].config);
    HAL_GPIO_Init(SDIOPinConfig[data0].port, &SDIOPinConfig[data0].config);
    HAL_GPIO_Init(SDIOPinConfig[data1].port, &SDIOPinConfig[data1].config);
    HAL_GPIO_Init(SDIOPinConfig[data2].port, &SDIOPinConfig[data2].config);
    HAL_GPIO_Init(SDIOPinConfig[data3].port, &SDIOPinConfig[data3].config);

    /* Reset SDIO Block */
    SDMMC_PowerState_OFF(SDMMC1);
    __HAL_RCC_SDMMC1_FORCE_RESET();
    __HAL_RCC_SDMMC1_RELEASE_RESET();

    /* Enable the SDIO Clock */
    __HAL_RCC_SDMMC1_CLK_ENABLE();

    // Lower  speed configuration
    SDMMC_InitTypeDef sdio_init_structure;

    sdio_init_structure.ClockDiv            = SDMMC_INIT_CLK_DIV;
    sdio_init_structure.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
    sdio_init_structure.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    sdio_init_structure.BusWide             = SDMMC_BUS_WIDE_1B;
    sdio_init_structure.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
    ret                              = SDMMC_Init(SDMMC1, sdio_init_structure);
    ret                             |= SDMMC_PowerState_ON(SDMMC1);
    ret                             |= SDMMC_SetSDMMCReadWaitMode(SDMMC1, SDMMC_READ_WAIT_MODE_CLK);
    if (ret) {
        return ret;
    }

    /* Clear all SDIO interrupts */
    SDMMC1->ICR = (uint32_t) 0xffffffff;

    /* Turn on SDIO IRQ */
    /* Must be lower priority than the value of configMAX_SYSCALL_INTERRUPT_PRIORITY */
    /* otherwise FreeRTOS will not be able to mask the interrupt */
    /* keep in mind that ARMCM7 interrupt priority logic is inverted, the highest value */
    /* is the lowest priority */
    HAL_NVIC_EnableIRQ((IRQn_Type) SDMMC1_IRQn);
    HAL_NVIC_SetPriority(SDMMC1_IRQn, 5, 0);

    if (cy_rtos_init_semaphore(&sdio_transfer_finished_semaphore, 1, 0) != CY_RSLT_SUCCESS) {
        cy_rtos_deinit_semaphore(&sdio_transfer_finished_semaphore);
        return -1;
    }

    SDIO_PRINT_DEBUG("cy_rtos_init_semaphore: %p\n", sdio_transfer_finished_semaphore);

    return ret;
}

void cyhal_sdio_free(cyhal_sdio_t *obj)
{
    //TODO
}

cy_rslt_t cyhal_sdio_configure(cyhal_sdio_t *obj, const cyhal_sdio_cfg_t *config)
{
    return sdio_enable_high_speed();
}

cy_rslt_t cyhal_sdio_send_cmd(const cyhal_sdio_t *obj, cyhal_transfer_t direction, cyhal_sdio_command_t command, uint32_t argument, uint32_t *response)
{
    uint32_t loop_count = 0;
    cy_rslt_t result;
    uint32_t sta_value;

    if (response != NULL) {
        *response = 0;
    }
    current_command = 0;

    SDIO->ICR = (uint32_t) 0xFFFFFFFF;

    /* Send the command */
    SDIO->ARG = argument;
    SDIO->CMD = (uint32_t)(command | SDIO_RESPONSE_SHORT | SDIO_WAIT_NO | SDIO_CPSM_ENABLE);
    loop_count = (uint32_t) COMMAND_FINISHED_CMD52_TIMEOUT_LOOPS;
    do {
        sta_value = SDIO->STA;
        loop_count--;
        if (loop_count == 0 || (sta_value & SDIO_ERROR_MASK) != 0) {
            HAL_Delay(10U);
        }
    } while ((sta_value & SDIO_FLAG_CMDACT) != 0);

    if(command == CYHAL_SDIO_CMD_GO_IDLE_STATE && (sta_value & SDMMC_STA_CTIMEOUT_Msk))
    {
        // OK, CMD0 always seems to generate a timeout on STM32
    }
    else if(command == CYHAL_SDIO_CMD_IO_SEND_OP_COND && (sta_value & SDMMC_STA_CCRCFAIL))
    {
        // OK, CMD5 always generates a CRC error on STM32
    }
    else if((sta_value & SDIO_ERROR_MASK) != 0)
    {
        printf("Warning: SDIO CMD%d failed with STA register 0x%" PRIx32 ".\n", command, sta_value);
    }

    if (response != NULL) {
        *response = SDIO->RESP1;
        result = CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_HAL, 2);
    }
    result = CY_RSLT_SUCCESS;

    SDMMC1->CMD = 0;

    SDIO_PRINT_DEBUG("%s cmd%d  arg 0x%" PRIx32 " resp 0x%" PRIx32"\n",
                     (direction!=CYHAL_READ)?"Write":"Read",
                     command,
                     argument,
                     (response)?*response:0);
    return result;
}

cy_rslt_t cyhal_sdio_bulk_transfer(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t *data, uint16_t length, uint32_t *response)
{
    cy_rslt_t result;
    uint32_t  block_size = 64;
    sdio_cmd_argument_t arg;
    uint32_t      cmd;
    current_command = SDIO_CMD_53;
    current_transfer_direction = direction;
    arg.value = argument;

    if (response != NULL) {
        *response = 0;
    }

    sdio_transfer_failed = 0;
    SDMMC1->ICR = (uint32_t) 0xFFFFFFFF;

    /* Dodgy STM32 hack to set the CMD53 byte mode size to be the same as the block size */
    if (arg.cmd53.block_mode == 0) {
        block_size = find_optimal_block_size(arg.cmd53.count);
        if (block_size < 512) {
            arg.cmd53.count = block_size;
        } else {
            arg.cmd53.count = 0;
        }
        argument = arg.value;
    }

    SDMMC1->CMD |= SDMMC_CMD_CMDTRANS;

    /* Prepare the SDIO for a data transfer */
    sdio_prepare_data_transfer(direction, block_size, (uint8_t *) data, (uint32_t) length);

    // Once the transfer is set up, enable IRQs, as the STAR.CMDREND flag could still have been
    // set from an earlier transfer, but it appears to clear by this point.
    sdio_enable_bus_irq();

    /* Send the command */
    SDMMC1->ARG = argument;
    cmd = (uint32_t)(SDIO_CMD_53 | SDMMC_RESPONSE_SHORT | SDMMC_WAIT_NO | SDMMC_CPSM_ENABLE | SDMMC_CMD_CMDTRANS);
    SDMMC1->CMD = cmd;

    /* Wait for the whole transfer to complete */
    result = cy_rtos_get_semaphore(&sdio_transfer_finished_semaphore, 50, false);

    if (result != CY_RSLT_SUCCESS) {
        if(result == CY_RTOS_TIMEOUT) {
            printf("Warning: SDIO bulk transfer timed out, STA register is 0x%" PRIx32 ".\n", SDMMC1->STA);
        } else {
            printf("Warning: SDIO bulk transfer semaphore acquire failure, error 0x%" PRIx32 ".\n", result);
        }
        result = CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_HAL, 3);
    }
    if (sdio_transfer_failed) {
        printf("Warning: SDIO bulk transfer failed with STA register 0x%" PRIx32 ".\n", irqstatus);
        result = CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_HAL, 2);
    }

    if (direction == CYHAL_READ) {
#ifdef _CYHAL_DCACHE_MAINTENANCE
        SCB_CleanInvalidateDCache_by_Addr(
                (uint32_t*)((uint32_t)dma_data_source & ~(__SCB_DCACHE_LINE_SIZE - 1U)),
                user_data_size + __SCB_DCACHE_LINE_SIZE);
#endif /* if defined(_CYHAL_DCACHE_MAINTENANCE) */

        memcpy(user_data, dma_data_source, (size_t) user_data_size);
    }

    if (response != NULL) {
        *response = SDIO->RESP1;
    }

    SDMMC1->CMD = 0;
    sdio_disable_bus_irq();

    SDIO_PRINT_DEBUG("%s cmd53 arg 0x%" PRIx32 " datasize 0x%" PRIx16"  blocknumber 0x%x resp 0x%" PRIx32"\n",
                     (direction!=CYHAL_READ)?"Write":"Read",
                     argument,
                     length,
                     arg.cmd53.count,
                     (response)?*response:0);

    // TODO driver is NOT happy if I remove this sleep
    osDelay(1);


    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_sdio_transfer_async(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t *data, uint16_t length)
{
    //TODO
    return CYHAL_SDIO_RET_NO_SP_ERRORS;
}

bool cyhal_sdio_is_busy(const cyhal_sdio_t *obj)
{
    return false;
}

cy_rslt_t cyhal_sdio_abort_async(const cyhal_sdio_t *obj)
{
    //TODO
    return CYHAL_SDIO_RET_NO_SP_ERRORS;
}

void cyhal_sdio_register_callback(cyhal_sdio_t *obj, cyhal_sdio_irq_handler_t handler, void *handler_arg)
{
    irq_handler_arg = handler_arg;
    sdio_irq_handler = handler;
}

void cyhal_sdio_enable_event(cyhal_sdio_t *obj, cyhal_sdio_irq_event_t event, uint8_t intr_priority, bool enable)
{
    //TODO
}


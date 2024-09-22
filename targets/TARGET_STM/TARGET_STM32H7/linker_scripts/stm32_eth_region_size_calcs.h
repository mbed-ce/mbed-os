/* mbed Microcontroller Library
 * Copyright (c) 2024 Jamie Smith
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
 * This file contains calculations for the size of the memory region used for the Ethernet buffer descriptors.
 * This region must be a power-of-2 size so that it can be used as an MPU region.  Also, in dual core
 * CPUs, the CM4 and CM7 have to agree on its size so they don't define conflicting memory regions.
 * 
 * So, this header has some fancy math to calculate it.  Note that this header is included by
 * linker scripts so it can only contain preprocessor code.
 */

/* from here: https://stackoverflow.com/questions/22925016/rounding-up-to-powers-of-2-with-preprocessor-constants */
#define POW2_CEIL(v) (1 + \
(((((((((v) - 1) | (((v) - 1) >> 0x10) | \
      (((v) - 1) | (((v) - 1) >> 0x10) >> 0x08)) | \
     ((((v) - 1) | (((v) - 1) >> 0x10) | \
      (((v) - 1) | (((v) - 1) >> 0x10) >> 0x08)) >> 0x04))) | \
   ((((((v) - 1) | (((v) - 1) >> 0x10) | \
      (((v) - 1) | (((v) - 1) >> 0x10) >> 0x08)) | \
     ((((v) - 1) | (((v) - 1) >> 0x10) | \
      (((v) - 1) | (((v) - 1) >> 0x10) >> 0x08)) >> 0x04))) >> 0x02))) | \
 ((((((((v) - 1) | (((v) - 1) >> 0x10) | \
      (((v) - 1) | (((v) - 1) >> 0x10) >> 0x08)) | \
     ((((v) - 1) | (((v) - 1) >> 0x10) | \
      (((v) - 1) | (((v) - 1) >> 0x10) >> 0x08)) >> 0x04))) | \
   ((((((v) - 1) | (((v) - 1) >> 0x10) | \
      (((v) - 1) | (((v) - 1) >> 0x10) >> 0x08)) | \
     ((((v) - 1) | (((v) - 1) >> 0x10) | \
      (((v) - 1) | (((v) - 1) >> 0x10) >> 0x08)) >> 0x04))) >> 0x02))) >> 0x01))))

/* Size of an ETH_DMADescTypeDef structure in bytes*/
#define STM32_SIZEOF_ETH_DMA_DESCRIPTOR 24

/* Calculation of RAM size */
#define STM32_TOTAL_DMA_DESCRIPTOR_RAM_NEEDED (STM32_SIZEOF_ETH_DMA_DESCRIPTOR * (MBED_CONF_STM32_EMAC_ETH_RXBUFNB + MBED_CONF_STM32_EMAC_ETH_TXBUFNB))
#define STM32_DMA_DESCRIP_REGION_SIZE POW2_CEIL(STM32_TOTAL_DMA_DESCRIPTOR_RAM_NEEDED)
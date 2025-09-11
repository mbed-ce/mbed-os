/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "flash_api.h"
#include "platform/mbed_critical.h"

#if DEVICE_FLASH
#include "mbed_assert.h"
#include "cmsis.h"

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
    uint32_t bank = 0;

    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) {
        bank = FLASH_BANK_1;
    } else {
        bank = FLASH_BANK_2;
    }

    return bank;
}

/**
  * @brief  Gets the sector of a given address
  * @param  Address: Flash address
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if (Address < (FLASH_BASE + FLASH_BANK_SIZE)) {
        sector = (Address - FLASH_BASE) / FLASH_SECTOR_SIZE;
    } else {
        sector = (Address - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_SECTOR_SIZE;
    }

    return sector;
}

/** Initialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_init(flash_t *obj)
{
    return 0;
}

/** Uninitialize the flash peripheral and the flash_t object
 *
 * @param obj The flash object
 * @return 0 for success, -1 for error
 */
int32_t flash_free(flash_t *obj)
{
    return 0;
}

/** Erase one sector starting at defined address
 *
 * The address should be at sector boundary. This function does not do any check for address alignments
 * @param obj The flash object
 * @param address The sector starting address
 * @return 0 for success, -1 for error
 */
int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    uint32_t PAGEError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if (HAL_ICACHE_Disable() != HAL_OK)
    {
        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    core_util_critical_section_enter();

    /* Clear error programming flags */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

    /* Increase Flash latency while programming
     * Refer to STM32H562xx/563xx/573xx errata sheet, section 2.2.9 for more details */
    __HAL_FLASH_SET_LATENCY(6);

    /* MBED HAL erases 1 page  / sector at a time */
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks       = GetBank(address);
    EraseInitStruct.Sector      = GetSector(address);
    EraseInitStruct.NbSectors   = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
        status = -1;
    }

    /* Restore normal Flash latency */
    __HAL_FLASH_SET_LATENCY(5);

    core_util_critical_section_exit();

    if (HAL_FLASH_Lock() != HAL_OK) {
        return -1;
    }

    if (HAL_ICACHE_Enable() != HAL_OK)
    {
        return -1;
    }

    return status;
}

/** Program one page starting at defined address
 *
 * The page should be at page boundary, should not cross multiple sectors.
 * This function does not do any check for address alignments or if size
 * is aligned to a page size.
 * @param obj The flash object
 * @param address The sector starting address
 * @param data The data buffer to be programmed
 * @param size The number of bytes to program
 * @return 0 for success, -1 for error
 */
int32_t flash_program_page(flash_t *obj, uint32_t address,
                           const uint8_t *data, uint32_t size)
{
    uint32_t StartAddress = 0;
    int32_t status = 0;

    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return -1;
    }

    if ((size % 16) != 0) {
        /* H5 flash devices can only be programmed 128bits/16 bytes at a time */
        return -1;
    }

    if (HAL_ICACHE_Disable() != HAL_OK)
    {
        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return -1;
    }

    /* Clear error programming flags */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

    /* Increase Flash latency while programming
     * Refer to STM32H562xx/563xx/573xx errata sheet, section 2.2.9 for more details */
    __HAL_FLASH_SET_LATENCY(6);

    /* Program the user Flash area word by word */
    StartAddress = address;

    while ((address < (StartAddress + size)) && (status == 0)) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, address,
                                (uint32_t) data)
                == HAL_OK) {
            address = address + 16;
            data = data + 16;
        } else {
            status = -1;
        }
    }

    /* Restore normal Flash latency */
    __HAL_FLASH_SET_LATENCY(5);

    if (HAL_FLASH_Lock() != HAL_OK) {
        return -1;
    }

    if (HAL_ICACHE_Enable() != HAL_OK)
    {
        return -1;
    }

    return status;
}

/** Get sector size
 *
 * @param obj The flash object
 * @param address The sector starting address
 * @return The size of a sector
 */
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if ((address >= (FLASH_BASE + FLASH_SIZE)) || (address < FLASH_BASE)) {
        return MBED_FLASH_INVALID_SIZE;
    }
    return (FLASH_SECTOR_SIZE);
}

/** Get page size
 *
 * @param obj The flash object
 * @param address The page starting address
 * @return The size of a page
 */
uint32_t flash_get_page_size(const flash_t *obj)
{
    /*  Page size is the minimum programable size, which 16 bytes */
    return 16;
}

/** Get start address for the flash region
 *
 * @param obj The flash object
 * @return The start address for the flash region
 */
uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_BASE;
}

/** Get the flash region size
 *
 * @param obj The flash object
 * @return The flash region size
 */
uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
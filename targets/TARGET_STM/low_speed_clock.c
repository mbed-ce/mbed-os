/* Copyright (c) 2026 MbedCE Community Contributors (Jan Kamidra)
 * SPDX-License-Identifier: Apache-2.0
 */

#include "low_speed_clock.h"

/*********************************************************************************
 * LSE power drive section
 *********************************************************************************/

#if MBED_CONF_TARGET_LSE_AVAILABLE

#if !MBED_CONF_TARGET_LSE_BYPASS && (defined(RCC_LSE_HIGHDRIVE_MODE) || defined(RCC_LSEDRIVE_HIGH))
#   define LSE_CONFIG_AVAILABLE
#endif

// set defaults for LSE drive load level
#if defined(LSE_CONFIG_AVAILABLE)

#   if defined(MBED_CONF_TARGET_LSE_DRIVE_LOAD_LEVEL)
#       define LSE_DRIVE_LOAD_LEVEL    MBED_CONF_TARGET_LSE_DRIVE_LOAD_LEVEL
#   else
#       if defined(RCC_LSE_HIGHDRIVE_MODE) // STM32F4
#           define LSE_DRIVE_LOAD_LEVEL    RCC_LSE_LOWPOWER_MODE
#       else
#           define LSE_DRIVE_LOAD_LEVEL    RCC_LSEDRIVE_LOW
#       endif
#   endif

/**
 * @brief configure the LSE crystal driver load
 * This setting is target hardware dependent and
 * depends on the crystal that is used for LSE clock.
 * For low power requirements, crystals with low load capacitors can be used and
 * driver setting is RCC_LSEDRIVE_LOW.
 * For higher stability, crystals with higher load capacities can be used and
 * driver setting is RCC_LSEDRIVE_HIGH.
 *
 * A detailed description about this setting can be found here:
 * https://www.st.com/resource/en/application_note/cd00221665-oscillator-design-guide-for-stm8afals-stm32-mcus-and-mpus-stmicroelectronics.pdf
 *
 * LSE crystal load drive setting is necessary before
 * enabling LSE.
 *
 * @param None
 * @retval None
 */
static void LSEDriveConfig(void)
{
#if defined(__HAL_RCC_LSEDRIVE_CONFIG)
    __HAL_RCC_LSEDRIVE_CONFIG(LSE_DRIVE_LOAD_LEVEL);
#else
    HAL_RCCEx_SelectLSEMode(LSE_DRIVE_LOAD_LEVEL);
#endif
}
#endif  // LSE_CONFIG_AVAILABLE

/*********************************************************************************
 * LSE start section
 *********************************************************************************/

/**
 * @brief Check if the low-speed external clock (LSE) is ready.
 * @return true if ready, false otherwise.
 */
static bool lsc_lse_is_ready(void)
{
#if TARGET_STM32WB0
    return __HAL_RCC_GET_LSE_READYFLAG();
#else
    return __HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET;
#endif
}

/**
 * @brief Start the low-speed external clock (LSE).
 * @return true if successful or already running, false otherwise.
 */
static bool lsc_start_lse(void) {
    if (lsc_lse_is_ready()) {
        return true; // Already running
    }
    bool lse_ready = true;
    // enable power clock and backup access to configure LSE
#ifdef __HAL_RCC_PWR_CLK_ENABLE
    __HAL_RCC_PWR_CLK_ENABLE();
#endif
    HAL_PWR_EnableBkUpAccess();

#if defined(LSE_CONFIG_AVAILABLE)
    // LSE oscillator drive capability set before LSE is started
    LSEDriveConfig();
#endif

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
#if MBED_CONF_TARGET_LSE_BYPASS
    RCC_OscInitStruct.LSEState       = RCC_LSE_BYPASS;
#else
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;
#endif
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE; // No PLL update
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        lse_ready = false; // Failed to start LSE 
    }
    return lse_ready; 
}
#else// MBED_CONF_TARGET_LSE_AVAILABLE

/*********************************************************************************
 * LSI start section
 *********************************************************************************/

/**
 * @brief Check if the low-speed internal clock (LSI) is ready.
 * @return true if ready, false otherwise.
 */
static bool lsc_lsi_is_ready(void)
{
#if TARGET_STM32WB0
    return __HAL_RCC_GET_LSI_READYFLAG();
#elif TARGET_STM32WB
    return __HAL_RCC_GET_FLAG(RCC_FLAG_LSI1RDY) != RESET;
#else
    return __HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) != RESET;
#endif
}

/**
 * @brief Start the low-speed internal clock (LSI).
 * @return true if successful or already running, false otherwise.
 */
static bool lsc_start_lsi(void) {
    if (lsc_lsi_is_ready()) {
        return true; // Already running
    }
    bool lsi_ready = true;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    /* Enable LSI clock */
#if TARGET_STM32WB
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI1;
#else
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
#endif
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE; // No PLL update

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        lsi_ready = false;
    }
    return lsi_ready;
}
#endif // MBED_CONF_TARGET_LSE_AVAILABLE

/*********************************************************************************
 * LSC start section
 *********************************************************************************/

lsc_t lsc_start(void) {
    lsc_t lsc_status = LSC_NONE;
#if MBED_CONF_TARGET_LSE_AVAILABLE
    lsc_status = lsc_start_lse() ? LSC_LSE : LSC_NONE;
#else // MBED_CONF_TARGET_LSE_AVAILABLE
    // If LSE is not available starting LSI
    lsc_status = lsc_start_lsi() ? LSC_LSI : LSC_NONE;
#endif // MBED_CONF_TARGET_LSE_AVAILABLE
    if (lsc_status == LSC_NONE) {
        error("Low Speed Clock initialization ERROR\n");
    }
    return lsc_status;
}

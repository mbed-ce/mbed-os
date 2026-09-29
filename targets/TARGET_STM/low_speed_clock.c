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
#if defined(__HAL_RCC_PWR_IS_CLK_DISABLED) && defined(__HAL_RCC_PWR_CLK_ENABLE) && defined(__HAL_RCC_PWR_CLK_DISABLE)
    bool rcc_pwr_clk_enable_changed = false;
    // enable power clock and backup access to configure LSE
    if (__HAL_RCC_PWR_IS_CLK_DISABLED()) {
        __HAL_RCC_PWR_CLK_ENABLE();
        rcc_pwr_clk_enable_changed = true;
    }
#endif
    HAL_PWR_EnableBkUpAccess();

#if defined(__HAL_RCC_LSEDRIVE_CONFIG)
    __HAL_RCC_LSEDRIVE_CONFIG(LSE_DRIVE_LOAD_LEVEL);
#else
    HAL_RCCEx_SelectLSEMode(LSE_DRIVE_LOAD_LEVEL);
#endif

#if defined(__HAL_RCC_PWR_IS_CLK_DISABLED) && defined(__HAL_RCC_PWR_CLK_ENABLE) && defined(__HAL_RCC_PWR_CLK_DISABLE)
    // Restore the RCC power clock to its original state.
    if (rcc_pwr_clk_enable_changed) {
        __HAL_RCC_PWR_CLK_DISABLE();
    }
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
 * @brief Check if the low-speed external clock (LSE) is explicitly enabled.
 * @return true if enabled, false otherwise.
 */
static bool lsc_lse_is_enabled(void)
{
#if TARGET_STM32WB0
    return (RCC->CR & RCC_CR_LSEON) != 0U;
#elif defined(RCC_BDCR_LSEON)
    return (RCC->BDCR & RCC_BDCR_LSEON) != 0U;
#elif defined(RCC_CSR_LSEON)
    return (RCC->CSR & RCC_CSR_LSEON) != 0U;
#else
    return false;
#endif
}

/**
 * @brief Start the low-speed external clock (LSE).
 * @return true if successful or already running, false otherwise.
 */
static bool lsc_start_lse(void)
{
    if (lsc_lse_is_enabled() && lsc_lse_is_ready()) {
        return true; // Already running
    }

#if defined(LSE_CONFIG_AVAILABLE)
    // LSE oscillator drive capability set before LSE is started
    LSEDriveConfig();
#endif

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
#if TARGET_STM32WB0
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
#if MBED_CONF_TARGET_LSE_BYPASS
    RCC_OscInitStruct.OscillatorType |= RCC_OSCILLATORTYPE_LSE_BYPASS;
    RCC_OscInitStruct.LSEBYPASSState = RCC_LSE_BYPASS_ON;
#else
    RCC_OscInitStruct.LSEBYPASSState = RCC_LSE_BYPASS_OFF;
#endif
#else
#if MBED_CONF_TARGET_LSE_BYPASS
    RCC_OscInitStruct.LSEState       = RCC_LSE_BYPASS;
#else
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;
#endif
#endif
#if !TARGET_STM32WB0
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE; // No PLL update
#endif
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return false;
    }
    return true;
}
#else // MBED_CONF_TARGET_LSE_AVAILABLE

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
 * @brief Check if the low-speed internal clock (LSI) is explicitly enabled.
 * @return true if enabled, false otherwise.
 */
static bool lsc_lsi_is_enabled(void)
{
#if TARGET_STM32WB0
    return (RCC->CR & RCC_CR_LSION) != 0U;
#elif defined(RCC_CSR_LSI1ON)     // WB
    return (RCC->CSR & RCC_CSR_LSI1ON) != 0U;
#elif defined(RCC_CSR_LSION)    // F0-7, G0, G4, H7, L0-5, U0, WL
    return (RCC->CSR & RCC_CSR_LSION) != 0U;
#elif defined(RCC_BDCR_LSION)   // H5, U5
    return (RCC->BDCR & RCC_BDCR_LSION) != 0U;
#else
    return false;
#endif
}

/**
 * @brief Start the low-speed internal clock (LSI).
 * @return true if successful or already running, false otherwise.
 */
static bool lsc_start_lsi(void)
{
    if (lsc_lsi_is_enabled() && lsc_lsi_is_ready()) {
        return true; // Already running
    }
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    /* Enable LSI clock */
#if TARGET_STM32WB
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI1;
#else
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
#endif
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
#if !TARGET_STM32WB0
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE; // No PLL update
#endif

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        return false;
    }
    return true;
}
#endif // MBED_CONF_TARGET_LSE_AVAILABLE

/*********************************************************************************
 * LSC start section
 *********************************************************************************/

void lsc_start(void)
{
#if MBED_CONF_TARGET_LSE_AVAILABLE
    if (!lsc_start_lse()) {
        error("Low Speed Clock start failed\n");
    }
#else // MBED_CONF_TARGET_LSE_AVAILABLE
    // If LSE is not available starting LSI
    if (!lsc_start_lsi()) {
        error("Low Speed Clock start failed\n");
    }
#endif // MBED_CONF_TARGET_LSE_AVAILABLE
}

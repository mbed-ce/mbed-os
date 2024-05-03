/**
  ******************************************************************************
  * @file    system_stm32u5xx.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M33 Device System Source File for STM32U5xx devices.
  ******************************************************************************
  * @attention
  *
  * This software component is provided to you as part of a software package and
  * applicable license terms are in the  Package_license file. If you received this
  * software component outside of a package or without applicable license terms,
  * the terms of the Apache-2.0 license shall apply. 
  * You may obtain a copy of the Apache-2.0 at:
  * https://opensource.org/licenses/Apache-2.0
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32u5xx_system
  * @{
  */

#ifndef SYSTEM_STM32U5XX_H
#define SYSTEM_STM32U5XX_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup STM32U5xx_System_Includes
  * @{
  */
  
/**
  * @}
  */

/** @addtogroup STM32U5xx_System_Exported_Variables
  * @{
  */
  /* The SystemCoreClock variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
   */
extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

extern const uint8_t  AHBPrescTable[16];    /*!< AHB prescalers table values */
extern const uint8_t  APBPrescTable[8];     /*!< APB prescalers table values */
extern const uint32_t MSIRangeTable[16];    /*!< MSI ranges table values     */

/**
  * @}
  */


/** @addtogroup STM32U5xx_System_Exported_Functions
  * @{
  */

/**
  * @brief Setup the microcontroller system.
  *
  * Initialize the System and update the SystemCoreClock variable.
  */
extern void SystemInit (void);


/**
  * @brief  Update SystemCoreClock variable.
  *
  * Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
  */
extern void SystemCoreClockUpdate (void);


/**
  * @brief Update SystemCoreClock variable from secure application and return its value
  *        when security is implemented in the system (Non-secure callable function).
  *
  * Returns the SystemCoreClock value with current core Clock retrieved from cpu registers.
  */
extern uint32_t SECURE_SystemCoreClockUpdate(void);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_STM32U5XX_H */

/**
  * @}
  */

/**
  * @}
  */


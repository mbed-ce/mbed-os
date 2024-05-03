/**
  ******************************************************************************
  * @file    stm32u5xx_ll_pwr.c
  * @author  MCD Application Team
  * @brief   PWR LL module driver.
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

#if defined (USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_ll_pwr.h"

/** @addtogroup STM32U5xx_LL_Driver
  * @{
  */

#if defined (PWR)

/** @defgroup PWR_LL PWR
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup PWR_LL_Exported_Functions
  * @{
  */

/** @addtogroup PWR_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize the PWR registers to their default reset values.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS : PWR registers are de-initialized.
  *          - ERROR   : not applicable.
  */
ErrorStatus LL_PWR_DeInit(void)
{
  /* Clear PWR low power flags */
  LL_PWR_ClearFlag_STOP();

  /* Clear PWR wake up flags */
  LL_PWR_ClearFlag_WU();

  /* Reset privilege attribute for nsecure attribute */
  LL_PWR_DisableNSecurePrivilege();

#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
  /* Reset privilege attribute for nsecure attribute */
  LL_PWR_DisableSecurePrivilege();

  /* Reset secure attribute */
  LL_PWR_ConfigSecure(0);
#endif /* defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U) */

  return SUCCESS;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* defined(PWR) */
/**
  * @}
  */

#endif /* defined (USE_FULL_LL_DRIVER) */

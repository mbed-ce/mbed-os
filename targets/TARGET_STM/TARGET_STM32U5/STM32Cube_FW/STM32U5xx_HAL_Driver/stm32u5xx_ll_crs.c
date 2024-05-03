/**
  ******************************************************************************
  * @file    stm32u5xx_ll_crs.h
  * @author  MCD Application Team
  * @brief   CRS LL module driver.
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
#if defined(USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_ll_crs.h"
#include "stm32u5xx_ll_bus.h"

/** @addtogroup STM32U5xx_LL_Driver
  * @{
  */

#if defined(CRS)

/** @defgroup CRS_LL CRS
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup CRS_LL_Exported_Functions
  * @{
  */

/** @addtogroup CRS_LL_EF_Init
  * @{
  */

/**
  * @brief  De-Initializes CRS peripheral registers to their default reset values.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: CRS registers are de-initialized
  *          - ERROR: not applicable
  */
ErrorStatus LL_CRS_DeInit(void)
{
  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_CRS);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_CRS);

  return  SUCCESS;
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

#endif /* defined(CRS) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

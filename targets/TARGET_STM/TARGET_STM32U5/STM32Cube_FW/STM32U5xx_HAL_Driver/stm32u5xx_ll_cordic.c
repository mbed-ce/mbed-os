/**
  ******************************************************************************
  * @file    stm32u5xx_ll_cordic.c
  * @author  MCD Application Team
  * @brief   CORDIC LL module driver.
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
#include "stm32u5xx_ll_cordic.h"
#include "stm32u5xx_ll_bus.h"
#ifdef  USE_FULL_ASSERT
#include "stm32_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

/** @addtogroup STM32U5xx_LL_Driver
  * @{
  */

#if defined(CORDIC)

/** @addtogroup CORDIC_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup CORDIC_LL_Exported_Functions
  * @{
  */

/** @addtogroup CORDIC_LL_EF_Init
  * @{
  */

/**
  * @brief  De-Initialize CORDIC peripheral registers to their default reset values.
  * @param  CORDICx CORDIC Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: CORDIC registers are de-initialized
  *          - ERROR: CORDIC registers are not de-initialized
  */
ErrorStatus LL_CORDIC_DeInit(const CORDIC_TypeDef *CORDICx)
{
  ErrorStatus status = SUCCESS;

  /* Check the parameters */
  assert_param(IS_CORDIC_ALL_INSTANCE(CORDICx));

  if (CORDICx == CORDIC)
  {
    /* Force CORDIC reset */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_CORDIC);

    /* Release CORDIC reset */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_CORDIC);
  }
  else
  {
    status = ERROR;
  }

  return (status);
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

#endif /* defined(CORDIC) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

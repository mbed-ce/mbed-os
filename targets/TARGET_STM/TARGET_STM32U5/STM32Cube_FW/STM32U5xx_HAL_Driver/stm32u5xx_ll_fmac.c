/**
  ******************************************************************************
  * @file    stm32u5xx_ll_fmac.c
  * @author  MCD Application Team
  * @brief   Header for stm32u5xx_ll_fmac.c module
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
#include "stm32u5xx_ll_fmac.h"
#include "stm32u5xx_ll_bus.h"
#ifdef USE_FULL_ASSERT
#include "stm32_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

/** @addtogroup STM32U5xx_LL_Driver
  * @{
  */

#if defined(FMAC)

/** @addtogroup FMAC_LL
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/** @addtogroup FMAC_LL_Exported_Functions
  * @{
  */

/** @addtogroup FMAC_LL_EF_Init
  * @{
  */

/**
  * @brief  Initialize FMAC peripheral registers to their default reset values.
  * @param  FMACx FMAC Instance
  * @retval ErrorStatus enumeration value:
  *          - SUCCESS: FMAC registers are initialized
  *          - ERROR: FMAC registers are not initialized
  */
ErrorStatus LL_FMAC_Init(FMAC_TypeDef *FMACx)
{
  ErrorStatus status = SUCCESS;

  /* Check the parameters */
  assert_param(IS_FMAC_ALL_INSTANCE(FMACx));

  if (FMACx == FMAC)
  {
    /* Perform the reset */
    LL_FMAC_EnableReset(FMACx);

    /* Wait until flag is reset */
    while (LL_FMAC_IsEnabledReset(FMACx) != 0UL)
    {
    }
  }
  else
  {
    status = ERROR;
  }

  return (status);
}

/**
  * @brief  De-Initialize FMAC peripheral registers to their default reset values.
  * @param  FMACx FMAC Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: FMAC registers are de-initialized
  *          - ERROR: FMAC registers are not de-initialized
  */
ErrorStatus LL_FMAC_DeInit(const FMAC_TypeDef *FMACx)
{
  ErrorStatus status = SUCCESS;

  /* Check the parameters */
  assert_param(IS_FMAC_ALL_INSTANCE(FMACx));

  if (FMACx == FMAC)
  {
    /* Force FMAC reset */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_FMAC);

    /* Release FMAC reset */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_FMAC);
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

#endif /* defined(FMAC) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

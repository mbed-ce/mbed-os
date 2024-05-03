/**
  ******************************************************************************
  * @file    stm32u5xx_hal_cryp_ex.h
  * @author  MCD Application Team
  * @brief   Header file of CRYPEx HAL module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U5xx_HAL_CRYP_EX_H
#define STM32U5xx_HAL_CRYP_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

#if defined(AES)

/** @defgroup CRYPEx CRYPEx
  * @brief CRYP Extension HAL module driver.
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup CRYPEx_Exported_Types CRYPEx Exported Types
  * @{
  */

/**
  * @}
  */
/* Exported constants --------------------------------------------------------*/
/** @defgroup CRYPEx_Exported_Constants CRYPEx Constants
  * @{
  */

/**
  * @}
  */
/* Private types -------------------------------------------------------------*/
/** @defgroup CRYPEx_Private_Types CRYPEx Private Types
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup CRYPEx_Private_Variables CRYPEx Private Variables
  * @{
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup CRYPEx_Private_Constants CRYPEx Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup CRYPEx_Private_Macros CRYPEx Private Macros
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup CRYPEx_Private_Functions CRYPEx Private Functions
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup CRYPEx_Exported_Functions CRYPEx Exported Functions
  * @{
  */

/** @addtogroup CRYPEx_Exported_Functions_Group1 Extended AES processing functions
  * @{
  */
HAL_StatusTypeDef HAL_CRYPEx_AESGCM_GenerateAuthTAG(CRYP_HandleTypeDef *hcryp, const uint32_t *pAuthTag,
                                                    uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYPEx_AESCCM_GenerateAuthTAG(CRYP_HandleTypeDef *hcryp, const uint32_t *pAuthTag,
                                                    uint32_t Timeout);
/**
  * @}
  */

/** @addtogroup CRYPEx_Exported_Functions_Group2 Wrap and Unwrap key functions
  * @{
  */
HAL_StatusTypeDef HAL_CRYPEx_UnwrapKey(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYPEx_WrapKey(CRYP_HandleTypeDef *hcryp, uint32_t *pInput, uint32_t *pOutput, uint32_t Timeout);
/**
  * @}
  */

/** @addtogroup CRYPEx_Exported_Functions_Group3 Encrypt and Decrypt Shared key functions
  * @{
  */
HAL_StatusTypeDef HAL_CRYPEx_EncryptSharedKey(CRYP_HandleTypeDef *hcryp, uint32_t *pKey, uint32_t *pOutput, uint32_t ID,
                                              uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYPEx_DecryptSharedKey(CRYP_HandleTypeDef *hcryp, uint32_t *pKey, uint32_t ID, uint32_t Timeout);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* AES */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_HAL_CRYP_EX_H */

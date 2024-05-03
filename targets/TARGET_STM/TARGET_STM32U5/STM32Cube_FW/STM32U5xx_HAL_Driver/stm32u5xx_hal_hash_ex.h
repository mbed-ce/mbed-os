/**
  ******************************************************************************
  * @file    stm32u5xx_hal_hash_ex.h
  * @author  MCD Application Team
  * @brief   Header file of HASH HAL module.
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
#ifndef STM32U5xx_HAL_HASH_EX_H
#define STM32U5xx_HAL_HASH_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */
#if defined (HASH)
/** @addtogroup HASHEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/

/** @addtogroup HASHEx_Exported_Functions HASH Extended Exported Functions
  * @{
  */

/** @addtogroup HASHEx_Exported_Functions_Group1 HASH extended processing functions in polling mode
  * @{
  */

HAL_StatusTypeDef HAL_HASHEx_SHA224_Start(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                          uint8_t *pOutBuffer, uint32_t Timeout);
HAL_StatusTypeDef HAL_HASHEx_SHA224_Accmlt(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HASHEx_SHA224_Accmlt_End(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                               uint8_t *pOutBuffer, uint32_t Timeout);
HAL_StatusTypeDef HAL_HASHEx_SHA256_Start(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                          uint8_t *pOutBuffer, uint32_t Timeout);
HAL_StatusTypeDef HAL_HASHEx_SHA256_Accmlt(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HASHEx_SHA256_Accmlt_End(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                               uint8_t *pOutBuffer, uint32_t Timeout);

/**
  * @}
  */

/** @addtogroup HASHEx_Exported_Functions_Group2 HASH extended processing functions in interrupt mode
  * @{
  */

HAL_StatusTypeDef HAL_HASHEx_SHA224_Start_IT(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                             uint8_t *pOutBuffer);
HAL_StatusTypeDef HAL_HASHEx_SHA224_Accmlt_IT(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HASHEx_SHA224_Accmlt_End_IT(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                                  uint8_t *pOutBuffer);
HAL_StatusTypeDef HAL_HASHEx_SHA256_Start_IT(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                             uint8_t *pOutBuffer);
HAL_StatusTypeDef HAL_HASHEx_SHA256_Accmlt_IT(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HASHEx_SHA256_Accmlt_End_IT(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                                  uint8_t *pOutBuffer);

/**
  * @}
  */

/** @addtogroup HASHEx_Exported_Functions_Group3 HASH extended processing functions in DMA mode
  * @{
  */
HAL_StatusTypeDef HAL_HASHEx_SHA224_Start_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HASHEx_SHA224_Finish(HASH_HandleTypeDef *hhash, uint8_t *pOutBuffer, uint32_t Timeout);
HAL_StatusTypeDef HAL_HASHEx_SHA256_Start_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HASHEx_SHA256_Finish(HASH_HandleTypeDef *hhash, uint8_t *pOutBuffer, uint32_t Timeout);

/**
  * @}
  */

/** @addtogroup HASHEx_Exported_Functions_Group4 HMAC extended processing functions in polling mode
  * @{
  */
HAL_StatusTypeDef HAL_HMACEx_SHA224_Start(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                          uint8_t *pOutBuffer, uint32_t Timeout);
HAL_StatusTypeDef HAL_HMACEx_SHA256_Start(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                          uint8_t *pOutBuffer, uint32_t Timeout);
/**
  * @}
  */

/** @addtogroup HASHEx_Exported_Functions_Group5 HMAC extended processing functions in interrupt mode
  * @{
  */

HAL_StatusTypeDef HAL_HMACEx_SHA224_Start_IT(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                             uint8_t *pOutBuffer);
HAL_StatusTypeDef HAL_HMACEx_SHA256_Start_IT(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size,
                                             uint8_t *pOutBuffer);

/**
  * @}
  */

/** @addtogroup HASHEx_Exported_Functions_Group6 HMAC extended processing functions in DMA mode
  * @{
  */

HAL_StatusTypeDef HAL_HMACEx_SHA224_Start_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_SHA256_Start_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);

/**
  * @}
  */

/** @addtogroup HASHEx_Exported_Functions_Group7 Multi-buffer HMAC extended processing functions in DMA mode
  * @{
  */

HAL_StatusTypeDef HAL_HMACEx_MD5_Step1_2_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_MD5_Step2_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_MD5_Step2_3_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);

HAL_StatusTypeDef HAL_HMACEx_SHA1_Step1_2_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_SHA1_Step2_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_SHA1_Step2_3_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_SHA224_Step1_2_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_SHA224_Step2_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_SHA224_Step2_3_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);

HAL_StatusTypeDef HAL_HMACEx_SHA256_Step1_2_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_SHA256_Step2_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
HAL_StatusTypeDef HAL_HMACEx_SHA256_Step2_3_DMA(HASH_HandleTypeDef *hhash, uint8_t *pInBuffer, uint32_t Size);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /*  HASH*/
/**
  * @}
  */


#ifdef __cplusplus
}
#endif


#endif /* STM32U5xx_HAL_HASH_EX_H */


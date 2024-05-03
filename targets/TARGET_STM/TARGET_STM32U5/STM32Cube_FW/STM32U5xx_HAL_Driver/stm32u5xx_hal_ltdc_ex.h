/**
  ******************************************************************************
  * @file    stm32u5xx_hal_ltdc_ex.h
  * @author  MCD Application Team
  * @brief   Header file of LTDC HAL Extension module.
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
#ifndef STM32U5xx_HAL_LTDC_EX_H
#define STM32U5xx_HAL_LTDC_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

#if defined (LTDC) && defined (DSI)

#include "stm32u5xx_hal_dsi.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @addtogroup LTDCEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup LTDCEx_Exported_Functions
  * @{
  */

/** @addtogroup LTDCEx_Exported_Functions_Group1
  * @{
  */
HAL_StatusTypeDef HAL_LTDCEx_StructInitFromVideoConfig(LTDC_HandleTypeDef *hltdc, DSI_VidCfgTypeDef *VidCfg);
HAL_StatusTypeDef HAL_LTDCEx_StructInitFromAdaptedCommandConfig(LTDC_HandleTypeDef *hltdc, DSI_CmdCfgTypeDef *CmdCfg);
/**
  * @}
  */

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#endif /* LTDC && DSI */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_HAL_LTDC_EX_H */

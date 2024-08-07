/**
  ******************************************************************************
  * @file    partition_stm32u5xx.h
  * @author  MCD Application Team
  * @brief   CMSIS STM32U5xx Device Header File for Initial Setup for
  *          Secure / Non-Secure Zones based on CMSIS CORE V5.4.0
  *
  *          The file is included in system_stm32u5xx_s.c in secure application.
  *          It includes the configuration section that allows to select the
  *          STM32U5xx device partitioning file for system core secure attributes
  *          and interrupt secure and non-secure assignment.
  *
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

/** @addtogroup stm32u5xx
  * @{
  */

#ifndef PARTITION_STM32U5XX_H
#define PARTITION_STM32U5XX_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** @addtogroup Secure_configuration_section
  * @{
  */

#if defined(STM32U575xx)
  #include "partition_stm32u575xx.h"
#elif defined(STM32U585xx)
  #include "partition_stm32u585xx.h"
#elif defined(STM32U595xx)
  #include "partition_stm32u595xx.h"
#elif defined(STM32U5A5xx)
  #include "partition_stm32u5a5xx.h"
#elif defined(STM32U599xx)
  #include "partition_stm32u599xx.h"
#elif defined(STM32U5A9xx)
  #include "partition_stm32u5a9xx.h"
#elif defined(STM32U5F7xx)
  #include "partition_stm32u5f7xx.h"
#elif defined(STM32U5G7xx)
  #include "partition_stm32u5g7xx.h"
#elif defined(STM32U5F9xx)
  #include "partition_stm32u5f9xx.h"
#elif defined(STM32U5G9xx)
  #include "partition_stm32u5g9xx.h"
#elif defined(STM32U535xx)
  #include "partition_stm32u535xx.h"
#elif defined(STM32U545xx)
  #include "partition_stm32u545xx.h"
#else
  #error "Please select first the target STM32U5xx device used in your application (in stm32u5xx.h file)"
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PARTITION_STM32U5XX_H */
/**
  * @}
  */

/**
  * @}
  */





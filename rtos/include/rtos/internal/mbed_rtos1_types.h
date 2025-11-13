/* mbed Microcontroller Library
 * Copyright (c) 2006-2025 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_RTOS_RTX1_TYPES_H
#define MBED_RTOS_RTX1_TYPES_H

#if MBED_CONF_RTOS_PRESENT

#include "cmsis_os.h"

// Provide CMSIS-RTOS v1 compatibility types as aliases to CMSIS-RTOS2 types
typedef osStatus_t osStatus;
typedef osPriority_t osPriority;
typedef osThreadId_t osThreadId;
typedef osTimerId_t osTimerId;
typedef osMutexId_t osMutexId;
typedef osSemaphoreId_t osSemaphoreId;

// CMSIS-RTOS v1 priority values (map to CMSIS-RTOS2 equivalents)
#ifndef osPriorityIdle
#define osPriorityIdle         osPriorityLow
#endif
#ifndef osPriorityLow
#define osPriorityLow          osPriorityBelowNormal
#endif
#ifndef osPriorityBelowNormal
#define osPriorityBelowNormal  osPriorityBelowNormal7
#endif
#ifndef osPriorityNormal
#define osPriorityNormal       osPriorityNormal7
#endif
#ifndef osPriorityAboveNormal
#define osPriorityAboveNormal  osPriorityAboveNormal7
#endif
#ifndef osPriorityHigh
#define osPriorityHigh         osPriorityHigh7
#endif
#ifndef osPriorityRealtime
#define osPriorityRealtime     osPriorityRealtime7
#endif
#ifndef osPriorityError
#define osPriorityError        osPriorityError
#endif

// CMSIS-RTOS v1 event structure (used by Queue and Mail)
typedef struct {
    osStatus status;     ///< status code: event or error information
    union {
        uint32_t v;      ///< message as 32-bit value
        void *p;         ///< message or mail as void pointer
        int32_t signals; ///< signal flags
    } value;             ///< event value
    union {
        osMessageQueueId_t message_id; ///< message queue ID obtained by osMessageQueueNew
        void *p;                       ///< generic pointer
    } def;               ///< event definition
} osEvent;

// CMSIS-RTOS v1 status codes (map to CMSIS-RTOS2 equivalents)
#define osOK                      osOK
#define osEventSignal             ((osStatus)0x08)
#define osEventMessage            ((osStatus)0x10)
#define osEventMail               ((osStatus)0x20)
#define osEventTimeout            ((osStatus)0x40)
#define osErrorParameter          osErrorParameter
#define osErrorResource           osErrorResource
#define osErrorTimeoutResource    osErrorTimeout
#define osErrorISR                osErrorISR
#define osErrorISRRecursive       ((osStatus)(-126))
#define osErrorPriority           ((osStatus)(-127))
#define osErrorNoMemory           osErrorNoMemory
#define osErrorValue              ((osStatus)(-104))
#define osErrorOS                 osError

#else

typedef int32_t osStatus;

#endif

#endif

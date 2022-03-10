/*
 * Copyright (c) 2021 Chipsea Technologies (Shenzhen) Corp., Ltd. All rights reserved.
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
/**
 ****************************************************************************************
 *
 * @file rtos_trace.h
 *
 * @brief Declaration of FreeRTOS trace function.
 *
 ****************************************************************************************
 */

#ifndef RTOS_TRACE_H_
#define RTOS_TRACE_H_

#include "plf.h"
#include "dbg.h"

enum {
    RTOS_TRACE_SWITCH_IN = 1,
    RTOS_TRACE_SWITCH_OUT,
    RTOS_TRACE_CREATE,
    RTOS_TRACE_DELETE,
    RTOS_TRACE_SUSPEND,
    RTOS_TRACE_RESUME,
    RTOS_TRACE_RESUME_FROM_ISR,
    RTOS_TRACE_ALLOC,
    RTOS_TRACE_FREE,
};

#if ( configUSE_TRACE_FACILITY == 1 ) || ( configUSE_RW_TASK_ID == 1 )
#define traceTASK_SWITCHED_IN()                                                          \
{                                                                                        \
    PROF_RTOS_TASK_SET(uxTaskGetTaskNumber(xTaskGetCurrentTaskHandle()));                \
    rtos_trace_task(RTOS_TRACE_SWITCH_IN, xTaskGetCurrentTaskHandle());                  \
}
#else
#define traceTASK_SWITCHED_IN() rtos_trace_task(RTOS_TRACE_SWITCH_IN, xTaskGetCurrentTaskHandle())
#endif // ( configUSE_TRACE_FACILITY == 1 ) || ( configUSE_RW_TASK_ID == 1 )
#define traceTASK_SWITCHED_OUT() rtos_trace_task(RTOS_TRACE_SWITCH_OUT, xTaskGetCurrentTaskHandle())
#define traceTASK_DELETE(pxTask) rtos_trace_task(RTOS_TRACE_DELETE, pxTask)
#define traceTASK_SUSPEND(pxTask) rtos_trace_task(RTOS_TRACE_SUSPEND, pxTask)
#define traceTASK_RESUME(pxTask) rtos_trace_task(RTOS_TRACE_RESUME, pxTask)
#define traceTASK_RESUME_FROM_ISR(pxTask) rtos_trace_task(RTOS_TRACE_RESUME_FROM_ISR, pxTask)
#define traceTASK_CREATE(pxTask) rtos_trace_task(RTOS_TRACE_CREATE, pxTask)

// Set to 1 to trace all alloc
// Set to 0 to only trace failed allocation
#define RTOS_MALLOC_TRACE_LEVEL 1
#define traceMALLOC(ptr, size) rtos_trace_mem(RTOS_TRACE_ALLOC, ptr, size, xPortGetFreeHeapSize())
#define traceFREE(ptr, size) rtos_trace_mem(RTOS_TRACE_FREE, ptr, size, xPortGetFreeHeapSize())

void rtos_trace_task(int id, void *task);
void rtos_trace_mem(int id, void *ptr, int size, int free_size);

#endif /* RTOS_TRACE_H_*/

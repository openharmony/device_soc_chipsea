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
 * @file rtos_al.c
 *
 * @brief Implementation of the FreeRTOS abstraction layer.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rtos_def.h"
#include "rtos.h"
#include "rtos_al.h"
#include <string.h>
#include <stdio.h>
#include "cmsis_os2.h"
#include "los_task.h"
#include "dbg.h"

TickType_t rtos_timeout_2_tickcount(int timeout_ms);

uint32_t rtos_now(bool isr)
{
    return osKernelGetTickCount();
}

void *rtos_malloc(uint32_t size)
{
    return LOS_MemAlloc(OS_SYS_MEM_ADDR, size);
}

void *rtos_calloc(uint32_t nb_elt, uint32_t size)
{
    void * res = LOS_MemAlloc(OS_SYS_MEM_ADDR, nb_elt * size);
    if (res)
        memset(res, 0, nb_elt * size);

    return res;
}

void rtos_free(void *ptr)
{
    LOS_MemFree(OS_SYS_MEM_ADDR, ptr);
}

void rtos_heap_info(int *total_size, int *free_size, int *min_free_size)
{
    *total_size = LOS_MemPoolSizeGet(OS_SYS_MEM_ADDR);
    *free_size = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
    *min_free_size = LOS_MemPoolSizeGet(OS_SYS_MEM_ADDR);
}

rtos_task_handle rtos_get_idle_task_handle(void)
{
    return g_idleTaskID;
}

rtos_task_handle rtos_get_task_handle(void)
{
    return osThreadGetId();
}

int rtos_task_create(rtos_task_fct func,
                     const char * const name,
                     enum rtos_task_id task_id,
                     const uint16_t stack_depth,
                     void * const params,
                     rtos_prio prio,
                     rtos_task_handle * const task_handle)
{
    osThreadId_t tid;
    osThreadAttr_t attr = {0};
    attr.stack_size = stack_depth;
    if (prio < osPriorityNormal) {
        dbg("rtos_task_create prio err! task:%s,prio = %d\r\n", attr.name, prio);
        prio = osPriorityNormal;
    }
    attr.priority = prio;
    attr.name = name;
    tid = osThreadNew((osThreadFunc_t)func, params, &attr);
    if (tid == NULL) {
        dbg("rtos_task_create err! task:%s\r\n",attr.name);
        return 1;
    }
    if (task_handle) {
        *task_handle = tid;
    }
    return 0;
}

void rtos_task_delete(rtos_task_handle task_handle)
{
    if (!task_handle) {
        task_handle = osThreadGetId();
    }
    osThreadTerminate(task_handle);
}

void rtos_task_suspend(int duration)
{
    if (duration <= 0)
        return;
    osDelay(LOS_MS2Tick(duration));
}

void rtos_task_set_priority(rtos_task_handle task_handle, uint32_t priority)
{
    osThreadSetPriority(task_handle, priority);
}

#define TASK_NOTICE_MAX_VALUE    (0x00FFFFFF)
#define TASK_NOTICE_ZERO_BIT     (0x01000000)
int rtos_task_init_notification(rtos_task_handle task)
{
    LosTaskCB *taskCB = NULL;
    UINT32 intSave;
    UINT32 ret = LOS_OK;

    intSave = LOS_IntLock();
    taskCB = (LosTaskCB*)task;
    LOS_IntRestore(intSave);

    if (NULL == taskCB){
       ret = LOS_ERRNO_TSK_NOT_CREATED;
    }
     #if 0
     if (LOS_OK == ret){
       if (taskCB->taskFlags & OS_TASK_FLAG_SYSTEM){
            ret =  LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK;
       }
     }
     #endif
     if (LOS_OK == ret){
        ret = LOS_EventInit(&taskCB->event);
     }

     if (ret){
        dbg("rtos_task_init_notifi: err code %d\r\n", ret);
        return -1;
    }
    else{
        return 0;
    }
}

uint32_t rtos_task_wait_notification(int timeout)
{
    UINT32 intSave;
    rtos_task_handle task;
    UINT32 notifi_value = 0;
    LosTaskCB *taskCB = NULL;

    intSave = LOS_IntLock();
    taskCB = OS_TCB_FROM_TID(LOS_CurTaskIDGet());
    LOS_IntRestore(intSave);

    if (NULL == taskCB){
        dbg("rtos_task_wait_notifi: task null\r\n");
    }
    else{
         notifi_value = LOS_EventRead(&taskCB->event, TASK_NOTICE_MAX_VALUE|TASK_NOTICE_ZERO_BIT, LOS_WAITMODE_OR|LOS_WAITMODE_CLR, rtos_timeout_2_tickcount(timeout));

    if (notifi_value & 0x02000000){	//means error
          notifi_value = 0;
        }
        notifi_value &= TASK_NOTICE_MAX_VALUE;
    }

    return notifi_value;
}

void rtos_task_notify(rtos_task_handle task_handle, uint32_t value, bool isr)
{
    UINT32 intSave;
    UINT32 ret;
    LosTaskCB *taskCB = NULL;

    intSave = LOS_IntLock();
    taskCB = (LosTaskCB*)task_handle;
    LOS_IntRestore(intSave);
    if (NULL == taskCB){
       dbg("rtos_task_wait_notifi: task null\r\n");
    }

    if (value > TASK_NOTICE_MAX_VALUE){
       dbg("rtos_task_notify:value=0x%x, too big\r\n", value);
       return;
    }

    if (0 == value){
        value = TASK_NOTICE_ZERO_BIT;	//0 convert to special bit set
    }

    ret = LOS_EventClear(&taskCB->event, 0UL);	//clear old value
    if (ret != LOS_OK){
        dbg("rtos_task_notify:clear err code is %d\r\n", ret);
    }

    ret = LOS_EventWrite(&taskCB->event, value);
    if (ret != LOS_OK){
       dbg("rtos_task_notify:write err code is %d\r\n", ret);
    }
    return;
}

int rtos_queue_create(int elt_size, int nb_elt, rtos_queue *queue)
{
    *queue = osMessageQueueNew(nb_elt, elt_size, NULL);
    if (*queue == NULL) {
        dbg("rtos_queue_create err!\r\n");
        return -1;
    }
    return 0;
}

void rtos_queue_delete(rtos_queue queue)
{
    osMessageQueueDelete(queue);
}

bool rtos_queue_is_empty(rtos_queue queue)
{
    uint32_t cnt;
    cnt = osMessageQueueGetCount(queue);
    return (cnt == 0) ? true : false;
}

int rtos_queue_cnt(rtos_queue queue)
{
    return osMessageQueueGetCount(queue);
}

int rtos_queue_write(rtos_queue queue, void *msg, int timeout, bool isr)
{
    osStatus_t ret = osMessageQueuePut(queue, msg, 0, timeout);
    return (ret == 0) ? 0 : 1;
}

int rtos_queue_read(rtos_queue queue, void *msg, int timeout, bool isr)
{
    osStatus_t ret = osMessageQueueGet(queue, msg, NULL, rtos_timeout_2_tickcount(timeout));
    return (ret == 0) ? 0 : 1;
}

int rtos_semaphore_create(rtos_semaphore *semaphore, int max_count, int init_count)
{
    *semaphore = osSemaphoreNew(max_count, init_count, NULL);
    if (*semaphore == NULL) {
        dbg("rtos_semaphore_create err!\r\n");
        return -1;
    }
    return 0;
}

void rtos_semaphore_delete(rtos_semaphore semaphore)
{
    osSemaphoreDelete(semaphore);
}

int rtos_semaphore_get_count(rtos_semaphore semaphore)
{
    return osSemaphoreGetCount(semaphore);
}

int rtos_semaphore_wait(rtos_semaphore semaphore, int timeout)
{
    osStatus_t ret = osSemaphoreAcquire(semaphore, LOS_MS2Tick(timeout));
    return (ret == osOK) ? 0 : 1;
}

int rtos_semaphore_signal(rtos_semaphore semaphore, bool isr)
{
    osStatus_t ret = osSemaphoreRelease(semaphore);
    return (ret == osOK) ? 0 : 1;
}

TimerHandle_t rtos_timer_create( const char * const pcTimerName,
                            const TickType_t xTimerPeriodInTicks,
                            const UBaseType_t uxAutoReload,
                            void * const pvTimerID,
                            TimerCallbackFunction_t pxCallbackFunction )
{
    (void)pcTimerName;
    uint8_t mode;
    uint32_t usSwTmrID;
    uint32_t uwRet;
    mode = ((uxAutoReload == 0) ? LOS_SWTMR_MODE_NO_SELFDELETE : LOS_SWTMR_MODE_PERIOD);

    uwRet = LOS_SwtmrCreate(xTimerPeriodInTicks, mode, (SWTMR_PROC_FUNC)pxCallbackFunction, &usSwTmrID, 0);
    if (uwRet != LOS_OK)  {
        return (osTimerId_t)NULL;
    }
    return (osTimerId_t)OS_SWT_FROM_SID(usSwTmrID);
}

int rtos_timer_start(TimerHandle_t xTimer,TickType_t xTicksToWait, bool isr)
{
    SWTMR_CTRL_S *pstSwtmr;
    uint32_t uwRet;

    pstSwtmr = (SWTMR_CTRL_S *)xTimer;

    uwRet = LOS_SwtmrStart(pstSwtmr->usTimerID);

    return (uwRet == LOS_OK) ? 0 : 1;
}

int rtos_timer_stop(TimerHandle_t xTimer,TickType_t xTicksToWait)
{
    (void)xTicksToWait;
    uint32_t uwRet;
    SWTMR_CTRL_S *pstSwtmr = (SWTMR_CTRL_S *)xTimer;

    uwRet = LOS_SwtmrStop(pstSwtmr->usTimerID);
    return (uwRet == LOS_OK) ? 0 : 1;
}
int rtos_timer_delete(TimerHandle_t xTimer,TickType_t xTicksToWait)
{
    (void)xTicksToWait;
    uint32_t uwRet;
    SWTMR_CTRL_S *pstSwtmr = (SWTMR_CTRL_S *)xTimer;
    uwRet = LOS_SwtmrDelete(pstSwtmr->usTimerID);
    return (uwRet == LOS_OK) ? 0 : 1;
}

int rtos_timer_change_period(TimerHandle_t xTimer, TickType_t xNewPeriod,TickType_t xTicksToWait)
{
    uint32_t uwRet;
    SWTMR_CTRL_S *pstSwtmr = (SWTMR_CTRL_S *)xTimer;

    uwRet = LOS_SwtmrStop(pstSwtmr->usTimerID);
    if (uwRet != LOS_OK) {
        return 1;
    }
    pstSwtmr->uwInterval = xNewPeriod;
    uwRet = LOS_SwtmrStart(pstSwtmr->usTimerID);

    return (uwRet == LOS_OK) ? 0 : 1;
}

int rtos_timer_restart(TimerHandle_t xTimer,TickType_t xTicksToWait, bool isr)
{
    (void)xTicksToWait;
    uint32_t uwRet;
    SWTMR_CTRL_S *pstSwtmr = (SWTMR_CTRL_S *)xTimer;

    uwRet = LOS_SwtmrStop(pstSwtmr->usTimerID);
    if (uwRet != LOS_OK) {
        return 1;
    }
    uwRet = LOS_SwtmrStart(pstSwtmr->usTimerID);

    return (uwRet == LOS_OK) ? 0 : 1;
}

 TickType_t rtos_timeout_2_tickcount(int timeout_ms)
{
    if (timeout_ms < 0)
    {
        return portMAX_DELAY;
    }
    else
    {
        return pdMS_TO_TICKS(timeout_ms);
    }
}

int rtos_mutex_create(rtos_mutex *mutex)
{
    *mutex = osMutexNew(NULL);
    if (*mutex == 0) {
        dbg("rtos_mutex_create err!\r\n");
        return -1;
    }

    return 0;
}

void rtos_mutex_delete(rtos_mutex mutex)
{
    osMutexDelete(mutex);
}

int rtos_mutex_lock(rtos_mutex mutex, int timeout)
{
    return osMutexAcquire(mutex, timeout);
}

int rtos_mutex_unlock(rtos_mutex mutex)
{
    return osMutexRelease(mutex);
}

int rtos_mutex_recursive_create(rtos_mutex *mutex)
{
   *mutex = osMutexNew(NULL);
    return (*mutex == 0) ? -1 : 0;
}
int rtos_mutex_recursive_lock(rtos_mutex mutex)
{
   return osMutexAcquire(mutex, 0);
}

int rtos_mutex_recursive_unlock(rtos_mutex mutex)
{
    return osMutexRelease(mutex);
}

uint32_t rtos_protect(void)
{
    return osKernelLock();
}

void rtos_unprotect(uint32_t protect)
{
    osKernelRestoreLock(protect);
}

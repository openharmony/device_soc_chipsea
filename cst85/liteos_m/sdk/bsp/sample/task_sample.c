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
#include "task_sample.h"
#include "los_config.h"
#include "los_interrupt.h"
#include "los_task.h"
#include "los_tick.h"

void sample_task_entry1(void) {
    while (1) {
        printf("%s: sample_task_entry1 runing\r\n", __func__);
        LOS_TaskDelay(1000);
    }
}

void sample_task_entry2(void) {
    while (1) {
        printf("%s: sample_task_entry2 runing\r\n", __func__);
        LOS_TaskDelay(1000);
    }
}

void create_task(void) {
    UINT32 ret;
    TSK_INIT_PARAM_S sample_task1 = {0};
    TSK_INIT_PARAM_S sample_task2 = {0};
    UINT32 sample_taskid1;
    UINT32 sample_taskid2;

    sample_task1.pfnTaskEntry = (TSK_ENTRY_FUNC)sample_task_entry1;
    sample_task1.uwStackSize = 0x1000;
    sample_task1.pcName = "sample_task_entry1";
    sample_task1.usTaskPrio = 6; /* Os task priority is 6 */
    ret = LOS_TaskCreate(&sample_task1, &sample_task1);
    if (ret != LOS_OK) {
        printf("%s: sample task1 create failed!\r\n", __func__);
    }

    sample_task2.pfnTaskEntry = (TSK_ENTRY_FUNC)sample_task_entry2;
    sample_task2.uwStackSize = 0x1000;
    sample_task2.pcName = "sample_task_entry2";
    sample_task2.usTaskPrio = 6; /* Os task priority is 6 */
    ret = LOS_TaskCreate(&sample_task2, &sample_task2);
    if (ret != LOS_OK) {
        printf("%s: sample task2 create failed!\r\n", __func__);
    }

    printf("%s: sample tasks create ok.\r\n", __func__);
}


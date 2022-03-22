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
#ifndef RTOS_H_
#define RTOS_H_

#include "rtos_ohos_al.h"
#include "task_al.h"

/// Definitions of the different FHOST task priorities
enum
{
    TASK_PRIORITY_CONSOLE              = RTOS_TASK_PRIORITY(1),
    TASK_PRIORITY_TEST                 = RTOS_TASK_PRIORITY(1),
    TASK_PRIORITY_BT_TASK              = RTOS_TASK_PRIORITY(2),
    TASK_PRIORITY_INTERSYS_TASK        = RTOS_TASK_PRIORITY(2),
    TASK_PRIORITY_ASIO                 = RTOS_TASK_PRIORITY(3),
    TASK_PRIORITY_AUDIO                = RTOS_TASK_PRIORITY(3),
    TASK_PRIORITY_DSP                  = RTOS_TASK_PRIORITY(2),
    TASK_PRIORITY_WIFI_VOICE           = RTOS_TASK_PRIORITY(1),
    TASK_PRIORITY_WIFI_TCPIP           = RTOS_TASK_PRIORITY(3),
    TASK_PRIORITY_WIFI_CNTRL           = RTOS_TASK_PRIORITY(2),
    TASK_PRIORITY_WIFI_IPC             = RTOS_TASK_PRIORITY(4),
    TASK_PRIORITY_WIFI_WPA             = RTOS_TASK_PRIORITY(1),
    TASK_PRIORITY_WIFI_TG_SEND         = RTOS_TASK_PRIORITY(2),
    TASK_PRIORITY_WIFI_PING_SEND       = RTOS_TASK_PRIORITY(1),
    TASK_PRIORITY_WIFI_IPERF           = RTOS_TASK_PRIORITY(1),
    TASK_PRIORITY_WIFI_SMARTCONF       = RTOS_TASK_PRIORITY(2),
    TASK_PRIORITY_WIFI_TX              = RTOS_TASK_PRIORITY(4),
    TASK_PRIORITY_CO_MAIN              = RTOS_TASK_PRIORITY(2),
    TASK_PRIORITY_WIFI_USER            = RTOS_TASK_PRIORITY(1),
    TASK_PRIORITY_USB_BT               = RTOS_TASK_PRIORITY(2),
    TASK_PRIORITY_MAX                  = RTOS_TASK_PRIORITY(5),
};

typedef struct {
    int priority;
    int stack_size;
} rtos_task_cfg_st;
rtos_task_cfg_st get_task_cfg(uint8_t task_id);

/*
 * FUNCTIONS
 ****************************************************************************************
 */

/**
 * Save user data that declared with PRIVATE_HOST_*(G3USER)
 */
void user_data_save(void);

/**
 * Restore user data that declared with PRIVATE_HOST_*(G3USER)
 */
void user_data_restore(void);

/**
 ****************************************************************************************
 * @brief Main function of the RTOS
 *
 * Called after hardware initialization to create all RTOS tasks and start the scheduler.
 ****************************************************************************************
 */
void rtos_main(void);

#endif // RTOS_H_

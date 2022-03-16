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
#ifndef _TASK_AL_H
#define _TASK_AL_H

/// Definitions of the different FHOST task stack size requirements
enum
{
    TASK_STACK_SIZE_CONSOLE              = 512,
    TASK_STACK_SIZE_TEST                 = 2048,
    TASK_STACK_SIZE_BT_TASK              = 3072,
    TASK_STACK_SIZE_ASIO                 = 2048,
    TASK_STACK_SIZE_AUDIO                = 2048,
    TASK_STACK_SIZE_BLE_TASK_ONLY        = 2048,
    TASK_STACK_SIZE_BT_BLE_TASK          = 4096,
    TASK_STACK_SIZE_DSP                  = 512,
    TASK_STACK_SIZE_WIFI_VOICE           = 512,
    TASK_STACK_SIZE_WIFI_TCPIP           = 2048,
    TASK_STACK_SIZE_WIFI_CNTRL           = 3072,
    TASK_STACK_SIZE_WIFI_IPC             = 2048,
    TASK_STACK_SIZE_WIFI_WPA             = 3072,
    TASK_STACK_SIZE_WIFI_TG_SEND         = 1024,
    TASK_STACK_SIZE_WIFI_PING_SEND       = 512,
    TASK_STACK_SIZE_WIFI_IPERF           = 1024,
    TASK_STACK_SIZE_WIFI_SMARTCONF       = 1024,
    TASK_STACK_SIZE_WIFI_TX              = 1024,
    TASK_STACK_SIZE_CO_MAIN              = 1024,
    TASK_STACK_SIZE_WIFI_USER            = 512,
    TASK_STACK_SIZE_USB_BT               = 512,
};

#endif

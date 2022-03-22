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
#ifndef APP_SMARTCONFIG_H_
#define APP_SMARTCONFIG_H_

#include "ble_ip_config.h"     // SW configuration

#if (BLE_APP_SMARTCONFIG)

#include <stdint.h>          // Standard Integer Definition
#include "ble_ke_task.h"

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    NO_ERROR = 0,
    WIFI_ERR,
    TIME_ERR,
};

#define INVALID_CONNECTION_INDEX                            0xFF

/// Smartconfig Application Module Environment Structure
struct app_smartconfig_env_tag
{
    /// Connection handle
    uint8_t conidx;
    uint8_t wifi_state_ntf_en;
    uint8_t time_state_ntf_en;
};

#define DATA_BUF_SIZE 32

struct ble_smartconfig {
    /// SSID
    char ssid[DATA_BUF_SIZE];
    /// Keyphrase
    char pwd[DATA_BUF_SIZE];
    //current time
    char time[DATA_BUF_SIZE];
};

/// Table of message handlers
extern const struct app_subtask_handlers app_smartconfig_handlers;

typedef void(*app_smartconfig_recv_ap_info_cb)(uint8_t *data, uint32_t length);
typedef void(*app_smartconfig_recv_state_info_cb)(uint8_t *data, uint32_t length);

void app_smartconfig_init(void);

void app_smartconfig_enable_prf(uint8_t conidx);

void app_smartconfig_add_smartconfig(void);

void app_smartconfig_disconnected(void);

void app_smartconfig_send_rsp(uint8_t* data, uint32_t length);

void app_smartconfig_register_ap_info_cb(app_smartconfig_recv_ap_info_cb callback);
void app_smartconfig_register_state_info_cb(app_smartconfig_recv_state_info_cb callback);


#ifdef __cplusplus
    }
#endif

#endif //(BLE_APP_SMARTCONFIG)

#endif // APP_SMARTCONFIG_H_

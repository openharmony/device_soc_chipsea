/*
 * Copyright (c) 2022 CHIPSEA Co., Ltd. All rights reserved.
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
#ifndef __APPOTABOX_H
#define __APPOTABOX_H
#include "cs_target_config.h"
#if PLF_BT_OTA == 1
#include "cs_adp_api.h"

enum {
    OTA_INFO_HEADER = (0x01UL << 0),
    OTA_DATA_HEADER = (0x01UL << 1),
};

enum APP_OTA_ACCESS{
    OTA_BY_SPP = 1,
    OTA_BY_VENDOR,
    OTA_BY_BLE,
};
void app_ota_set_access(enum APP_OTA_ACCESS access);
enum APP_OTA_ACCESS app_ota_get_access(void);
void app_ota_init(void);
void app_ota_msg_handle(CS_EVENT *Event);
uint8_t app_ota_send_data(uint8_t *data, uint16_t len, uint8_t header);
void app_ota_receive_data(uint8_t *rxbuf, uint16_t len);
#if APP_SUPPORT_OTA_BOX
void app_ota_connect_device(BT_ADDR * bdaddr);
void app_ota_clean_addr_list(void);
void app_ota_connect_by_list_id(uint8_t id);
void app_ota_add_addr_list(BT_ADDR *ota_addr);
#endif
#endif
#endif

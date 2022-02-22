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
/* create @20200301 by cs*/

#ifndef WB_COEX_H
#define WB_COEX_H

#include <stdint.h>

struct coex_controller {
    uint8_t wifi_on;
    uint8_t bt_on;
    uint8_t wifi_connected;
    uint8_t bt_connected;
    uint8_t wifi_dhcp;
    uint8_t a2dp_on;//ftp,opp/hid
    uint8_t sco_on;
    uint8_t wifi_deepsleep;
};

int wb_coex_wifi_on_get(void);

void wb_coex_wifi_on_set(int on);

int wb_coex_bt_on_get(void);

void wb_coex_bt_on_set(int on);

int wb_coex_wifi_connected_get(void);

void wb_coex_wifi_connected_set(int connected);

int wb_coex_bt_connected_get(void);

void wb_coex_bt_connected_set(int connected);

int wb_coex_bt_a2dp_on_get(void);

void wb_coex_bt_a2dp_on_set(int on);

int wb_coex_bt_sco_on_get(void);

void wb_coex_bt_sco_on_set(int on);

#endif

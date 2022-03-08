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
/* create @20200301 by cs*/

#include "wb_coex.h"

static struct coex_controller g_coex_controller;

int wb_coex_wifi_on_get(void)
{
    return g_coex_controller.wifi_on;
}

void wb_coex_wifi_on_set(int on)
{
    g_coex_controller.wifi_on = (uint8_t)on;
}

int wb_coex_bt_on_get(void)
{
    return g_coex_controller.bt_on;
}

void wb_coex_bt_on_set(int on)
{
    g_coex_controller.bt_on = (uint8_t)on;
}

int wb_coex_wifi_connected_get(void)
{
    return g_coex_controller.wifi_connected;
}

void wb_coex_wifi_connected_set(int connected)
{
    g_coex_controller.wifi_connected = (uint8_t)connected;
}

int wb_coex_bt_connected_get(void)
{
    return g_coex_controller.bt_connected;
}

void wb_coex_bt_connected_set(int connected)
{
    g_coex_controller.bt_connected = (uint8_t)connected;
}

int wb_coex_bt_a2dp_on_get(void)
{
    return g_coex_controller.a2dp_on;
}

void wb_coex_bt_a2dp_on_set(int on)
{
    g_coex_controller.a2dp_on = (uint8_t)on;
}

int wb_coex_bt_sco_on_get(void)
{
    return g_coex_controller.sco_on;
}

void wb_coex_bt_sco_on_set(int on)
{
    g_coex_controller.sco_on = (uint8_t)on;
}

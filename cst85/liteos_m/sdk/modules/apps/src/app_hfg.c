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
#include "cs_target_config.h"

#if APP_SUPPORT_HFG
#include "rtos.h"
#include "cs_adp_api.h"
#include "app_bt_queue.h"

BT_ADDR test_device = {{0xaa,0xc0,0x00,0x88,0x88,0x33}};
void app_hfg_connect_def_device(void)
{
    TRACE("APP:app_hfg_connect_def_device\n");
    app_bt_connect_hfg(&test_device);
}

void app_hfg_connect_sco(void)
{
    TRACE("APP:app_hfg_connect_sco\n");
    app_bt_hfp_connect_sco(&test_device);
}

void app_hfg_disconnect_sco(void)
{
    TRACE("APP:app_hfg_disconnect_sco\n");
    app_bt_hfp_disconnect_sco(&test_device);
}
#endif

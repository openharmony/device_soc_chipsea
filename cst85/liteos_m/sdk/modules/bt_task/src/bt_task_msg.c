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
#include "bt_task_msg.h"
#include "cs_adp_api.h"
#ifdef CFG_BLE_STACK
#include "csble.h"
#endif

#include "app_bt.h"

APP_BT_HANDLER handler_register_list[HANDLER_REG_MAX] = {0};

int app_bt_handler_register(enum APP_BT_HANDLER_REG_T user, APP_BT_HANDLER handler)
{
    handler_register_list[user]= handler;
    return 0;
}

void handler_reg_list_poll(void)
{
    uint8_t i;
    for (i=0; i<HANDLER_REG_MAX; i++){
        if (handler_register_list[i]){
            handler_register_list[i]();
        }
    }
}


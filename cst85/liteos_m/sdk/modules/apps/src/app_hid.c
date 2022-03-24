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
#include "cs_target_config.h"
#if APP_SUPPORT_HID == 1
#include "cs_adp_api.h"
#include "al_rtos.h"
#include "app_bt.h"

BOOL app_hid_key_handle(uint32_t key)
{
    BOOL ret = FALSE;

    switch(key){
        case APP_KEY_VOLSUB|APP_KEY_PRESS:
            break;
        case APP_KEY_VOLADD|APP_KEY_DOUBLE_CLICK:
            break;
        default:
            break;
    }
    return ret;
}

void app_hid_msg_handle(CS_EVENT *event)
{
    CS_ADP_HID_EVENT *cs_hid_msg = (CS_ADP_HID_EVENT *)event->Param;

    TRACE("APP:app_hid_msg_handle\n");

    switch(event->EventId){
        case CS_ADP_HID_CONNECTED:

        break;
        case CS_ADP_HID_DISCONNECTED:

        break;
    }
}


#endif

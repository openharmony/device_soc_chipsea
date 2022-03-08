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
#ifndef _APP_HSP_H_
#define _APP_HSP_H_
#if APP_SUPPORT_HSP == 1 

typedef enum {
    HS_IDLE,
    HS_CONNECTED,
    HS_PLAY_SCO,
}app_hsp_state_t;

void app_hsp_msg_handle(CS_EVENT *event);
#endif
#endif

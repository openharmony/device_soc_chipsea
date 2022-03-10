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
#ifndef _APP_A2DP_H_
#define _APP_A2DP_H_
#if APP_SUPPORT_A2DP_SBC == 1 || APP_SUPPORT_A2DP_AAC ==1 ||APP_SUPPORT_A2DP_SOURCE == 1


typedef enum {
    A2DP_IDLE,
    A2DP_CONNECTED,
    A2DP_PLAY,
}app_a2dp_state_t;

void app_a2dp_msg_handle(CS_EVENT *event);
app_a2dp_state_t app_get_a2dp_state(void);
void app_set_a2dp_state(app_a2dp_state_t state);
void app_a2dp_pause(void);

#endif
#endif

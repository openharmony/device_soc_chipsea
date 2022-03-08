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
#ifndef _APP_AVRCP_H_
#define _APP_AVRCP_H_
#if APP_SUPPORT_AVRCP == 1
void app_set_avrcp_volume(BT_ADDR *bdaddr,S8 volume);
S8 app_get_avrcp_volume(BT_ADDR *bdaddr);
BOOL app_avrcp_key_handle(uint32_t key);
void app_avrcp_msg_handle(CS_EVENT *event);
#endif
#endif

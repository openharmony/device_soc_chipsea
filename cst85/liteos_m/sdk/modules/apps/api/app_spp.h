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
#ifndef __APP_SPP_H__
#define __APP_SPP_H__
#if APP_SUPPORT_SPP == 1

#include "cs_adp_api.h"


typedef void(*app_spp_tx_done_t)(void);
void app_spp_register_tx_done(app_spp_tx_done_t callback);
void app_spp_msg_handle(CS_EVENT *event);
void app_spp_connect(BT_ADDR * bdaddr);
void app_spp_send_data(uint8_t* ptrData, uint16_t length);

#if MAX_THROUGHPUT_TEST == 1
void app_spp_connect_test(void);
#endif

#endif
#endif

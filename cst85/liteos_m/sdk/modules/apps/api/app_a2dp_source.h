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
#ifndef _APP_A2DP_SOURCE_H_
#define _APP_A2DP_SOURCE_H_
#include "cs_adp_a2dp.h"

void app_a2dp_source_init(void);
void app_a2dp_source_start(void);
void app_a2dp_source_suspend(void);
void app_a2dp_source_sbc_data_send_timer_begin(void);
void app_a2dp_source_connect_default_device(void);
void app_bt_source_send_a2dp_test_sbc_packet(BT_ADDR default_device );

#endif

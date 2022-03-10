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
#ifndef _CSBLE_H_
#define _CSBLE_H_
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/

typedef void (*BleDeInitCallback)(uint8_t flag, uint8_t status);
extern BleDeInitCallback ble_callback;

void cs_ble_init(void);
void cs_ble_deinit(void);
void cs_ble_reset(void);
void cs_ble_schedule(void);
bool cs_ble_tx_rx_allow_sleep(void);
bool cs_ble_stack_allow_sleep(void);
#endif

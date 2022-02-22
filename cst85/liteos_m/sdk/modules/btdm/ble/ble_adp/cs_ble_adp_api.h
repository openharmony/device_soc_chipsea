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
#ifndef CSADPMGR
#define CSADPMGR
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/
#include "bt_types_def.h"
#include "cs_host_cfg.h"

void cs_stack_init_ble_only(cs_host_cfg_t cfg);
BOOL cs_adp_stack_config_ble_only(void);
void cs_stack_loop_ble_only(void);
void cs_adp_send_hci_data_direct_ble_only(const uint8_t *buff,uint8_t len);
void cs_adp_send_generate_key_ble_only(void);
void cs_adp_set_lp_level_ble_only(uint8_t level);
void cs_adp_ble_deinit(void);
void cs_adp_free_stack_memory_ble_only(void);
void cs_adp_rw_timer_notify(void);
#endif

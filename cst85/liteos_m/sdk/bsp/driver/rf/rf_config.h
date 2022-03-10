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
#ifndef __RF_CONFIG_H
#define __RF_CONFIG_H

#include "reg_wlan_rf.h"
#include "reg_bt_rf.h"
#include "reg_wcnaon.h"

uint32_t wifi_rxgain_table_24g[48][4];
uint32_t wifi_rxgain_table_5g[32][4];
uint32_t wifi_txgain_table[32];
uint32_t bt_rxgain_table[16][4];
uint32_t bt_txgain_table[16][2];

uint32_t backup_wifi_rxgain_table_24g[48][4];
uint32_t backup_wifi_rxgain_table_5g[32][4];
uint32_t backup_wifi_txgain_table[32];
uint32_t backup_bt_rxgain_table[16][4];
uint32_t backup_bt_txgain_table[16][2];

extern void wlan_rf_config(uint8_t band);
extern void bt_rf_config(void);
extern void common_rf_config(void);
extern void bt_only_mdm_config(void);
extern void bt_combo_mdm_config(void);
extern void init_aon_ram(void);
extern void update_rxgain_table(uint32_t wifi_rxgain_2g4_addr, uint32_t wifi_rxgain_5g_addr);


#endif

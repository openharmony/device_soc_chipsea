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
#ifndef __AON_SRAM_H__
#define  __AON_SRAM_H__
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/

#ifdef CFG_AON
#include "al_rtos.h"
#include "target_config.h"
#include <stdint.h>
#include <string.h>
#ifndef CFG_BLE_ONLY
#include "cs_adp_api.h"
#include "app_a2dp.h"
#include "app_hfp.h"
#include "app_bt.h"
#include "bt_task_msg.h"

#if APP_SUPPORT_A2DP_SBC == 1 || APP_SUPPORT_A2DP_AAC == 1 || APP_SUPPORT_A2DP_SOURCE == 1
extern app_a2dp_state_t t_a2dp_state;
#endif
#if APP_SUPPORT_HFP == 1
extern app_hfp_state_t s_hfp_state;
extern uint16_t  s_hfp_scocodec;
#endif
extern APP_BT_HANDLER handler_register_list[HANDLER_REG_MAX];
extern app_connect_manager app_bt_connect_struct[LINK_MAX];
extern app_connect_manager *app_con_ptr[LINK_MAX];
extern app_bt_state_t s_bt_state;
extern AppBtScanMode bt_scan_mode;

#if APP_SUPPORT_AES == 1
extern U32 secret_key[8];
#endif
extern U32 cs_host_log ;

typedef struct{
#if APP_SUPPORT_A2DP_SBC == 1 || APP_SUPPORT_A2DP_AAC == 1 || APP_SUPPORT_A2DP_SOURCE == 1
    app_a2dp_state_t t_a2dp_state;
#endif
#if APP_SUPPORT_HFP == 1
    app_hfp_state_t s_hfp_state;
    uint16_t s_hfp_scocodec;
#endif
    app_bt_state_t s_bt_state;
    AppBtScanMode bt_scan_mode;
    APP_BT_HANDLER handler_register_list[HANDLER_REG_MAX];
    app_connect_manager app_bt_connect_struct[LINK_MAX];
    app_connect_manager *app_con_ptr[LINK_MAX];
    U32 cs_host_log;
#if APP_SUPPORT_AES == 1
    U32 secret_key[8];
#endif
}AppBtStatic;
#endif

#if PLF_BLE_STACK == 1
#include "app_present.h"
#include "hal_profile.h"
#include "ble_ke_mem.h"
#if BLE_APP_HID
#include "hal_profile_utils.h"
#include "bass.h"
#include "hid_over_gatt_device.h"
#endif
#if BLE_APP_SMARTCONFIG
#include "smartconfig.h"
#endif

extern uint8_t ble_enabled;

typedef struct
{
    struct prf_env_tag prf_env;
#if BLE_APP_HID
    struct bass_env_tag bass_env;
    struct hogpd_env_tag hogpd_env;
#endif
#if BLE_APP_SMARTCONFIG
    struct smartconfig_env_tag smartconfig_env;
#endif
}AppBlePrfEnv;

extern struct app_env_tag app_env;
extern struct ble_app_user_info aon_ble_app_user_info;
typedef struct{
    uint8_t aon_ble_enabled;
    AonBleAppEnv aon_ble_app_env;
    AonBleAppUserInfo aon_ble_app_user_info;
}AppBleStatic;
#endif

extern uint8_t bt_lp_level;

typedef struct{
#ifndef CFG_BLE_ONLY
    AppBtStatic btstatic;
#endif
#if PLF_BLE_STACK == 1
    AppBleStatic blestatic;
    AppBlePrfEnv ble_prf_env;
#endif
    uint8_t bt_lp_level;
}AppStatic;

typedef struct
{
    uint32_t len;
    uint8_t app_static[AON_APP_STATIC_LEN];
}AonAppStatic;


#endif
#endif

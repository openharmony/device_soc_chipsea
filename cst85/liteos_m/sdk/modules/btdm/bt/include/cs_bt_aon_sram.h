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
#ifndef _CS_BT_AON_SRAM_H
#define _CS_BT_AON_SRAM_H

#ifdef CFG_AON

enum host_deepsleep_status {
    // CPU need powerdown
    POWER_ALIVE  = 0,
    // CPU no need powerdown
    POWER_DOWN = 1,
};

enum host_aon_sram_id {
    HOST_AON_ID_BT,
#ifndef CFG_BLE_ONLY
    HOST_AON_ID_CMGR,
    HOST_AON_ID_AVDEV,
    HOST_AON_ID_A2DP,
    HOST_AON_ID_AVRCP,
    HOST_AON_ID_AVCTP,
    HOST_AON_ID_HF,
    HOST_AON_ID_AVDTP,
    HOST_AON_ID_APP_BT_DEVICE,
    HOST_AON_ID_TWS,
    HOST_AON_ID_HOST_STATIC,
#endif
#ifdef CFG_BLE_STACK
    //HOST_AON_ID_BLE_PARAM,
    HOST_AON_ID_BLE_KE_TASK_ENV,
    HOST_AON_ID_BLE_KE_ENV,
    //HOST_AON_ID_BLE_APP_ENV,
    HOST_AON_ID_BLE_KE_EVENT,
    HOST_AON_ID_BLE_GAPM,
    //HOST_AON_ID_BLE_PRF_ENV,
    HOST_AON_ID_BLE_GATTM,
    HOST_AON_ID_BLE_L2CM,
#endif
    HOST_AON_ID_NUM,
};

enum app_aon_sram_id {
    APP_AON_ID_APP_STATIC,
    APP_AON_ID_NUM,
};

//extern HOST_AON_STRUCT *host_aon;
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern void pm_bt_save(void);
extern void pm_bt_restore(void);

#ifndef CFG_BLE_ONLY
extern void pm_cmgr_save(void);
extern void pm_cmgr_restore(void);

extern void pm_avdev_save(void);
extern void pm_avdev_restore(void);

extern void pm_a2dp_save(void);
extern void pm_a2dp_restore(void);

extern void pm_avrcp_save(void);
extern void pm_avrcp_restore(void);

extern void pm_avctp_save(void);
extern void pm_avctp_restore(void);

extern void pm_hf_save(void);
extern void pm_hf_restore(void);

extern void pm_avdtp_save(void);
extern void pm_avdtp_restore(void);

extern void pm_app_bt_dev_t_save(void);
extern void pm_app_bt_dev_t_restore(void);

extern void pm_tws_save(void);
extern void pm_tws_restore(void);

extern void pm_host_static_save(void);
extern void pm_host_static_restore(void);
#endif

extern void pm_app_static_save(void *aon_app_static);
extern void pm_app_static_restore(void *aon_app_static);

#ifdef CFG_BLE_STACK
extern void pm_ble_ke_task_env_save(void);
extern void pm_ble_ke_task_env_restore(void);

extern void pm_ble_ke_env_save(void);
extern void pm_ble_ke_env_restore(void);

extern void pm_ble_ke_event_env_save(void);
extern void pm_ble_ke_event_env_restore(void);

extern void pm_ble_gapm_env_save(void);
extern void pm_ble_gapm_env_restore(void);

extern void pm_ble_gattm_env_save(void);
extern void pm_ble_gattm_env_restore(void);

extern void pm_ble_l2cm_env_save(void);
extern void pm_ble_l2cm_env_restore(void);
#endif

void host_aon_sram_hi_mux_to_cpu_sys(void);
void host_aon_sram_hi_mux_to_aon(void);
void host_aon_sram_init(void);
void host_aon_sram_save(enum host_aon_sram_id id);
void host_aon_sram_restore(enum host_aon_sram_id id);
void host_aon_sram_save_all(void);
bool host_aon_sram_restore_all(void);
uint32_t host_power_on_mode(void);
void host_aon_deepsleep_status_set(enum host_deepsleep_status mode);
enum host_deepsleep_status host_aon_deepsleep_status_get(void);
bool aon_sram_hi_using_by_aon_get(void);
void cpup_ready_set(bool ready);
void aon_power_on_cpup_set(bool power_on);
bool aon_power_on_cpup_get(void);
void aon_power_on_cpus_req_set(bool power_on);
bool aon_power_on_cpus_req_get(void);
void host_aon_interface_init(void);

#endif
#endif // _CS_BT_AON_SRAM_H

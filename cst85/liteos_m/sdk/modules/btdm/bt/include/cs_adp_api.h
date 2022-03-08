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
#ifndef _CS_ADP_API_H_
#define _CS_ADP_API_H_
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/
#include "cs_target_config.h"
#include "cs_adp_type.h"

#include "cs_adp_mgr.h"
#include "cs_adp_a2dp.h"
#include "cs_adp_avrcp.h"
#include "cs_adp_dev.h"
#include "cs_adp_hfp.h"
#include "cs_adp_hsp.h"
#include "cs_adp_spp.h"
#include "cs_adp_hid.h"
#include "cs_adp_test.h"
#include "cs_adp_tws.h"
#include "flash_api.h"
#include "cs_host_cfg.h"

void cs_stack_loop(void);
void cs_stack_init(cs_host_cfg_t cfg);
void cs_adp_mgr_init(void);
void cs_adp_register_app_msg_handle(cs_bt_msg_module module,app_msg_handle_cb callback);

APP_BT_MGR *cs_adp_get_app_bt_mgr_ptr(void);
void cs_adp_send_generate_key(void);
void cs_adp_set_lp_level(uint8_t level);
Status_BTDef cs_adp_inquiry_cancel(void);
Status_BTDef cs_adp_inquiry(U8 len, U8 maxResp);
Status_BTDef cs_adp_set_bt_name(const U8 *name, U8 length);
Status_BTDef cs_adp_sco_switch(uint16_t        scohandle);
Status_BTDef cs_adp_role_switch(BT_ADDR bdaddr);
void cs_adp_disconnect_acl(BT_ADDR bdaddr);
Status_BTDef cs_adp_stop_sniff(BT_ADDR bdaddr);
Status_BTDef cs_adp_set_scan_mode(AppBtScanMode mode);
Status_BTDef cs_adp_set_linkpolicy(BT_ADDR bdaddr, AppBtLinkPolicy policy);
Status_BTDef cs_adp_set_sniff_timer(BT_ADDR bdaddr,         AppBtSniffInfo* sniff_info,TimeT Time);
BOOL app_bt_check_invalid_bdaddr(BT_ADDR *bdaddr);
void cs_adp_save_flash(void);
void cs_adp_close_dev(void);
btdev_info* cs_adp_get_devinfo_by_addr(BT_ADDR *bdAddr);
uint8_t cs_adp_get_current_activeCons(void);
BOOL cs_adp_stack_config(void);
void cs_adp_erased_flash(void);
void cs_adp_send_hci_data_direct(const uint8_t *buff,uint8_t len);
APP_DEVLIST *cs_adp_get_a2dp_current_devinfo(void);
APP_DEVLIST *cs_adp_get_hfp_current_devinfo(void);
APP_DEVLIST *cs_adp_get_hsp_current_devinfo(void);
#if PLF_BLE_STACK && PLF_BLE_ONLY == 0
void cs_adp_rw_timer_notify(void);
#endif
void cs_change_host_log_level(BOOL flag,U32 level);
#endif

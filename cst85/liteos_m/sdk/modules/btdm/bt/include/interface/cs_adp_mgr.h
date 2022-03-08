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
#ifndef CSADPMGR
#define CSADPMGR
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/

#include "bt_types_def.h"
#include "cs_adp_type.h"
#include "cs_adp_dev.h"
#include "cs_adp_a2dp.h"
#include "cs_adp_avrcp.h"
#include "cs_adp_hfp.h"
#include "cs_adp_hsp.h"
#include "cs_adp_spp.h"
#include "cs_adp_hid.h"

#define APP_BT_ROLE_SWITCH_LIMIT   2


typedef struct _APP_DEVLIST{
    BT_ADDR bdaddr;
    U8 acl_mode;
    U8 role;
    bt_mgr_adp_state adp_state;
    bt_a2dp_adp_state a2dp_state;
    bt_avrcp_adp_state avrcp_state;
    bt_hid_adp_state hid_state;
    bt_hfp_adp_state hfp_state;
    bt_hfp_adp_call_status hfp_call_status;
    bt_hfp_adp_callhold_status hfp_call_hold;
    bt_hfp_adp_siri_status hfp_siri_status;
    BT_A2DP_ROLE a2dp_role;
    A2DP_AUDIO_CAP a2dp_audio_cap;
    U16 sco_handle;
    U16 negotiated_codec;
    S8 peer_volume_hfp;
    S8 peer_volume_a2dp;
    bt_class_of_device cod;
}APP_DEVLIST;

typedef struct _APP_BT_MGR{
    APP_DEVLIST device_list[LINK_MAX];
    BT_ADDR local_addr;
    BT_ADDR le_addr;
    U8 max_device_num;
    U8 connect_index;
    bt_class_of_device local_cod;
}APP_BT_MGR;

typedef struct _CS_EVENT_{
    U32 EventId;
    U32 Param;
}CS_EVENT;

typedef void (*app_msg_handle_cb)(CS_EVENT *event);

typedef struct _APP_ADP_MSG_HANDLE{
    cs_bt_msg_module module;
    app_msg_handle_cb callback;
}app_adp_msg_handle;

typedef struct {
    bt_class_of_device cod;
}Mgr_InquiryResult_t;

typedef struct _CS_ADP_MGR_EVENT_{
    BT_ADDR bdaddr;
    union {
        bt_err_type reason;
        Mgr_InquiryResult_t inqResultP;
    }p;
}CS_ADP_MGR_EVENT;

APP_DEVLIST *app_bt_get_mgr_devinfo(BT_ADDR bdaddr);
extern void cs_adp_dispatch_msg(CS_EVENT *event);


#endif

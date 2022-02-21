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
#ifndef _CS_ADP_TWS_
#define _CS_ADP_TWS_
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/

#include "cs_adp_type.h"

typedef enum {
    TWS_SLAVE = 0,
    TWS_MASTER,
    TWS_NOROLE,
    TWS_NOSUPPORT,
}CS_ADP_TWS_ROLE;

#ifndef RWIP_TIME_T
#define RWIP_TIME_T
typedef struct
{
    /// Integer part of the time (in half-slot)
    uint32_t hs;
    /// Fractional part of the time (in half-us) (range: 0-624)
    uint32_t hus;
} rwip_time_t;

typedef struct
{
    /// Integer part of the time (in half-slot)
    uint32_t clk_off_hs;
    /// Fractional part of the time (in half-us) (range: 0-624)
    uint32_t clk_off_hus;
} TWS_BT_CLK_INFO_Type;
#endif

typedef enum {
    CS_IDLE = 0,
    CS_PENDING,
    CS_CONNECTED,
    CS_STOP,
    CS_START,
    CS_CALL,
    CS_STATE_END = 0xff,
}CS_TWS_STATE;

typedef struct {
    bt_err_type reason;
}Tws_Con_t;

typedef struct {
    bt_err_type reason;
    CS_ADP_TWS_ROLE last_role;
}Tws_Disc_t;

typedef struct {
    U32 tws_trig_data;//redefined by AppTwsMediaDataStruct
}Tws_TrigNum_t;

typedef struct {
    U32 tws_sync_data;//redefined by AppTwsMediaDataStruct
}Tws_AudSync_t;

typedef struct {
    host_data_struct * buff;
}Tws_DataSend_t;

typedef struct {
    host_data_struct * buff;
}Tws_DataInd_t;


typedef struct _CS_ADP_TWS_EVENT_{
    BT_ADDR bdaddr;
    union {
        Tws_Con_t conP;
        Tws_Disc_t discP;
        Tws_TrigNum_t trigP;
        Tws_AudSync_t syncP;
        Tws_DataSend_t datasendP;
        Tws_DataInd_t dataindP;
    }p;
}CS_ADP_TWS_EVENT;

void cs_adp_tws_init(void);
void cs_adp_connect_tws_peer_device(BT_ADDR * bdaddr);
void cs_adp_disconnect_tws(BT_ADDR * bdaddr);
uint8_t cs_tws_get_porttype(void);
BOOL cs_adp_send_data(uint8_t* ptrData, uint16_t length);
BOOL cs_adp_check_cmp_twsaddr(BT_ADDR bdaddr);
CS_ADP_TWS_ROLE tws_app_get_role(void);
CS_ADP_TWS_ROLE tws_app_get_flash_role(void);
void cs_adp_update_tws_flash_role(CS_ADP_TWS_ROLE role);
rwip_time_t cs_adp_get_native_clk_plus_offset(void);
TWS_BT_CLK_INFO_Type cs_adp_get_slave_bt_offset(void);
void cs_adp_set_audio_sync_param(U32 synclk,U32 counternum,U32 samplerate,U32 bt_counter,U32 bt_counter_offset,U32 strb_cycle_offset);
void cs_adp_tws_restart(void);
void cs_adp_host_control_tws_sync_con(BOOL con_or_disc);
void cs_adp_host_m_s_switch(void);
void cs_adp_host_sync_stop(void);

/************************************************
function: cs_adp_tws_set_scan_para
          set ble scan param and then make scan enable,the param was set in adp layer.

          reset: 1:scan allow adv all
                 0:scan allow adv in white list
*************************************************/
void cs_adp_tws_set_scan_para(U8 reset);

/************************************************
function: cs_adp_tws_set_scan_disable
          set ble scan disable
*************************************************/
void cs_adp_tws_set_scan_disable(void);

/************************************************
function: cs_adp_tws_start_adv
          set ble adv enable,the param was set in adp layer.
*************************************************/
void cs_adp_tws_start_adv(BT_ADDR bdaddr);

/************************************************
function: cs_adp_tws_stop_adv
          disable ble adv
*************************************************/
void cs_adp_tws_stop_adv(void);
/************************************************
function: cs_adp_tws_clear_whitelist
          clear whitelist in fw
*************************************************/
void cs_adp_tws_clear_whitelist(void);

#endif


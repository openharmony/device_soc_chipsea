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
#ifdef CFG_BT_STACK
#ifdef CFG_BLE_ONLY
#include "cs_target_config.h"
#include <string.h>
#include "rtos.h"
#include "flash_api.h"
#include "ble_task_msg.h"
#if (PLF_CONSOLE)
#include "console.h"
#include "ble_task.h"
#endif // (PLF_CONSOLE)
#include "sleep_api.h"
#include "app_ble_only.h"



uint8_t bt_addr[6] = {
    0xaa,0xc0,0x00,0x88,0x88,0x33
};
uint8_t ble_addr[6] = {
    0xaa,0xc0,0x00,0x45,0x56,0x67
};
#if (BLE_APP_SMARTCONFIG)
#define APP_DFLT_DEVICE_NAME            ("BLE_SMARTCONFIG")
#elif (BLE_APP_HID)
#define APP_DFLT_DEVICE_NAME            ("CS_BLE_HID")
#else
#define APP_DFLT_DEVICE_NAME            ("CS_BLE")
#endif
#define APP_DFLT_DEVICE_NAME_LEN        (sizeof(APP_DFLT_DEVICE_NAME))

#if (BLE_APP_HID)
static const uint8_t adv_data_uuid[] = {
    03, GAP_AD_TYPE_COMPLETE_LIST_16_BIT_UUID,
    ATT_SVC_HID & 0xFF,
    ATT_SVC_HID >> 8,
};
#endif //(BLE_APP_HID)

static const uint8_t adv_data_appeareance[] = {
    03, GAP_AD_TYPE_APPEARANCE,
#if (BLE_APP_HID)
    GAPC_APPEARANCE_HID_MOUSE & 0xFF,
    GAPC_APPEARANCE_HID_MOUSE >> 8,
#else
    GAPC_APPEARANCE_UNKNOWN & 0xFF,
    GAPC_APPEARANCE_UNKNOWN >> 8,
#endif //(BLE_APP_HID)
};



static struct gapm_adv_create_param adv_param;
static uint8_t adv_data[ADV_DATA_LEN];

typedef void (*app_add_svc_func_t)(void);
/// List of service to add in the database
enum app_svc_list
{
    #if (BLE_APP_BATT)
    APP_SVC_BATT,
    #endif //(BLE_APP_BATT)
    #if (BLE_APP_HID)
    APP_SVC_HIDS,
    #endif //(BLE_APP_HID)
    #if (BLE_APP_SMARTCONFIG)
    APP_SVC_SMARTCONFIG,
    #endif //(BLE_APP_SMARTCONFIG)

    APP_SVC_LIST_STOP,
};

/// List of functions used to create the database
static const app_add_svc_func_t app_add_svc_func_list[APP_SVC_LIST_STOP] =
{
    #if (BLE_APP_BATT)
    (app_add_svc_func_t)app_batt_add_bas,
    #endif //(BLE_APP_BATT)
    #if (BLE_APP_HID)
    (app_add_svc_func_t)app_hid_add_hids,
    #endif //(BLE_APP_HID)
    #if (BLE_APP_SMARTCONFIG)
    (app_add_svc_func_t)app_smartconfig_add_smartconfig,
    #endif //(BLE_APP_SMARTCONFIG)
};

static void init_adv_params()
{
    memset(&adv_param, 0, sizeof(adv_param));
    adv_param.type = GAPM_ADV_TYPE_LEGACY;
    adv_param.disc_mode = GAPM_ADV_MODE_GEN_DISC;
    adv_param.prop = GAPM_ADV_PROP_UNDIR_CONN_MASK;
    adv_param.filter_pol = ADV_ALLOW_SCAN_ANY_CON_ANY;
    adv_param.prim_cfg.chnl_map = APP_ADV_CHMAP;
    adv_param.prim_cfg.phy = GAP_PHY_LE_1MBPS;
    adv_param.prim_cfg.adv_intv_min = APP_ADV_INT_MIN;
    adv_param.prim_cfg.adv_intv_max = APP_ADV_INT_MAX;
}

static uint8_t build_adv_data()
{
    uint8_t i = 0;
    uint8_t *buf = &adv_data[0];
    uint8_t len = 0;

    #if (BLE_APP_HID)
    len = sizeof(adv_data_uuid);
    memcpy(&buf[i], adv_data_uuid, len);
    i += len;
    #endif

    len = sizeof(adv_data_appeareance);
    memcpy(&buf[i], adv_data_appeareance, len);
    i += len;

    buf[i] = APP_DFLT_DEVICE_NAME_LEN + 1;
    buf[i + 1] = GAP_AD_TYPE_COMPLETE_NAME;
    memcpy(buf + i + 2, APP_DFLT_DEVICE_NAME, APP_DFLT_DEVICE_NAME_LEN);
    i += APP_DFLT_DEVICE_NAME_LEN + 2;

    return i;
}

#if (PLF_CONSOLE)

static int app_ble_scan_stop(int argc, char *argv[])
{
    dbg("%s\r\n", __func__);
    app_ble_scan_msg_stop();
    return ERR_NONE;
}

static int app_ble_scan_start(int argc, char *argv[])
{
    dbg("%s\r\n", __func__);
    app_ble_scan_msg_start();
    return ERR_NONE;
}

static int app_ble_adv_data_update(int argc, char *argv[])
{
    app_ble_adv_data_update_msg_send();
    return ERR_NONE;
}

static int app_ble_adv_param_update(int argc, char *argv[])
{
    app_ble_adv_param_update_msg_send();

    return ERR_NONE;
}

static int app_ble_adv_stop(int argc, char *argv[])
{
    dbg("%s\r\n", __func__);
    app_ble_adv_stop_msg_send();
    return ERR_NONE;
}

static int app_ble_adv_start(int argc, char *argv[])
{
    dbg("%s\r\n", __func__);
    app_ble_adv_start_msg_send();
    return ERR_NONE;
}

static int app_ble_discon(int argc, char *argv[])
{
    dbg("%s\r\n", __func__);
    app_ble_disconnect_msg_send();
    return ERR_NONE;
}

static int app_ble_con_param_update(int argc, char *argv[])
{
    app_ble_con_param_msg_send();
    return ERR_NONE;
}

static int app_ble_onoff_control(int argc, char *argv[])
{
    unsigned int on_off;

    if (argc < 2) {
        return ERR_WRONG_ARGS;
    }

    on_off = console_cmd_strtoul(argv[1], NULL, 10);
    if (on_off > 1) {
        return ERR_WRONG_ARGS;
    }

    if(on_off){
        ble_task_init();
    }else{
        ble_task_deinit();
    }

    return ERR_NONE;
}

#if (BLE_APP_BATT)
static int app_batt_set(int argc, char *argv[])
{
    uint8_t data = app_batt_get_lvl();
    dbg("%s set %d\r\n", __func__, data);
    app_batt_send_lvl(data);
    return ERR_NONE;
}
#endif // (BLE_APP_BATT)

static int app_ble_user_sleep(int argc, char *argv[])
{
    unsigned int sleep;

    if (argc < 2) {
        return ERR_WRONG_ARGS;
    }

    sleep = console_cmd_strtoul(argv[1], NULL, 10);
    if (sleep > 1) {
        return ERR_WRONG_ARGS;
    }

    if (sleep) {
        user_sleep_allow(1);
    } else {
        user_sleep_allow(0);
    }
    return ERR_NONE;
}

static int app_ble_sleep_level(int argc, char *argv[])
{
    unsigned int level;

    if (argc < 2) {
        return ERR_WRONG_ARGS;
    }

    if(*argv[1] == '?') {
        dbg("%d\n", sleep_level_get());
        return ERR_NONE;
    }

    level = console_cmd_strtoul(argv[1], NULL, 10);
    if (level >= PM_LEVEL_LEVEL_NUM) {
        return ERR_WRONG_ARGS;
    }

    sleep_level_set(level);

    return ERR_NONE;
}

static int app_ble_user_wakeup_source(int argc, char *argv[])
{
    int src, en, arg = 0;

    if (argc < 3) {
        return ERR_WRONG_ARGS;
    }

    src = console_cmd_strtoul(argv[1], NULL, 10);
    if (src >= WAKESRC_NUM_MAX) {
        return ERR_WRONG_ARGS;
    }
    en  = console_cmd_strtoul(argv[2], NULL, 10);
    if (src == WAKESRC_TIMER) {
        if (argc < 4) {
            return ERR_WRONG_ARGS;
        }
        arg = console_cmd_strtoul(argv[3], NULL, 10);
    }

    user_sleep_wakesrc_set(src, en, arg);

    return ERR_NONE;
}

static int app_ble_del_bond(int argc, char *argv[])
{
    dbg("%s \r\n", __func__);
    app_ble_del_bond_msg_send();
    return ERR_NONE;
}

static int app_ble_enable_dut(int argc, char *argv[])
{
    app_ble_enter_dut_msg_send();
    return ERR_NONE;
}

static int app_ble_scan_en(int argc, char *argv[])
{
    unsigned int scan_en;
    unsigned int dut = 0;

    if (argc < 2) {
        return ERR_WRONG_ARGS;
    }

    scan_en = console_cmd_strtoul(argv[1], NULL, 0);
    if (scan_en > 0x03) {
        return ERR_WRONG_ARGS;
    }
    dut = console_cmd_strtoul(argv[2], NULL, 10);
    TRACE("scan_en %d,dut %d\n",scan_en,dut);
    app_ble_wr_scan_en(scan_en);

    return ERR_NONE;
}

static int app_ble_addr_rdwr(int argc, char *argv[])
{
    if ((argc < 2) || ((strlen(argv[1]) == 1) && (argv[1][0] == '?'))) { // read
        struct bd_addr user_addr;
        if (flash_btdm_le_rand_addr_read(&user_addr.addr[0]) == INFO_READ_DONE) {
            memcpy(&ble_addr[0], &user_addr.addr[0], BD_ADDR_LEN);
        } else {
            dbg("no ble addr in flash, use default\r\n");
        }
        dbg("read ble addr: %02x%02x%02x%02x%02x%02x\r\n",
            ble_addr[0], ble_addr[1], ble_addr[2], ble_addr[3], ble_addr[4], ble_addr[5]);
    } else { // write
        char str0[8 + 1];
        char *str1;
        unsigned char addr[6];
        uint32_t addr0, addr1;
        if (strlen(argv[1]) != 12) {
            return -1;
        }
        str1 = &argv[1][0];
        memcpy(str0, &argv[1][4], 8);
        str0[8] = '\0';
        str1[4] = '\0';
        addr0 = console_cmd_strtoul(str0, NULL, 16);
        addr1 = console_cmd_strtoul(str1, NULL, 16) & 0x0000ffff;
        addr[0] = (unsigned char)(addr1 >> 8);
        addr[1] = (unsigned char)(addr1 >> 0);
        addr[2] = (unsigned char)(addr0 >> 24);
        addr[3] = (unsigned char)(addr0 >> 16);
        addr[4] = (unsigned char)(addr0 >> 8);
        addr[5] = (unsigned char)(addr0 >> 0);
        memcpy(&ble_addr[0], &addr[0], BD_ADDR_LEN);
        flash_btdm_le_rand_addr_write(addr);
        dbg("write ble addr: %04x%08x\r\n", addr1, addr0);
    }
    return 0;
}

void app_ble_test_command_add(void)
{
    console_cmd_add("ble_scan_stop", "  ble_scan_stop", 1, app_ble_scan_stop);
    console_cmd_add("ble_scan_start", "  ble_scan_start", 1, app_ble_scan_start);
    console_cmd_add("ble_adv_data", "  ble adv data update", 1, app_ble_adv_data_update);
    console_cmd_add("ble_adv_param", "  ble adv param update", 1, app_ble_adv_param_update);
    console_cmd_add("ble_adv_stop", "  ble adv stop", 1, app_ble_adv_stop);
    console_cmd_add("ble_adv_start", "  ble adv start", 1, app_ble_adv_start);
    console_cmd_add("ble_discon", "  ble disconnet", 1, app_ble_discon);
    console_cmd_add("ble_con_param", "  ble con param update", 1, app_ble_con_param_update);
    console_cmd_add("ble_on", "  ble_on/off", 2, app_ble_onoff_control);
#if (BLE_APP_BATT)
    console_cmd_add("batt_set", "  batt set", 1, app_batt_set);
#endif // (BLE_APP_BATT)
    console_cmd_add("ble_del_bond", "  ble delete bond", 2, app_ble_del_bond);
    console_cmd_add("ble_usrslp", "  Allow user sleep", 2, app_ble_user_sleep);
    console_cmd_add("ble_slplvl", "  Set system sleep level", 2, app_ble_sleep_level);
    console_cmd_add("ble_usrwusrc", "  Set user wakeup source", 4, app_ble_user_wakeup_source);
    console_cmd_add("ble_addr", "  rdwr ble addr", 2, app_ble_addr_rdwr);
    console_cmd_add("dut", "  dut  :enter dut", 1, app_ble_enable_dut);
    console_cmd_add("scan_en", "  scan_en <scan_type> <is_dut>", 3, app_ble_scan_en);
}
#endif // (PLF_CONSOLE)

void ble_user_app_init_cb()
{
#if (BLE_APP_SEC)
    // Security Module
    app_sec_init();
#endif // (BLE_APP_SEC)

#if (BLE_APP_HID)
    // HID Module
    app_hid_init();
#endif //(BLE_APP_HID)

#if (BLE_APP_BATT)
    // Battery Module
    app_batt_init();
#endif //(BLE_APP_BATT)

#if (BLE_APP_SMARTCONFIG)
    // Smart Config Module
    app_smartconfig_init();
#endif //(BLE_APP_SMARTVOICE)

}

void ble_user_app_init_complete_cb()
{
    dbg("!!!!!!!!!!!@@@@@@@@@@@@@@@@@\n");
}

bool ble_user_add_svc_cb()
{
    // Indicate if more services need to be added in the database
    bool more_svc = false;

    // Check if another should be added in the database
    if (app_env.next_svc != APP_SVC_LIST_STOP)
    {
        ASSERT_INFO(app_add_svc_func_list[app_env.next_svc] != NULL, app_env.next_svc, 1);

        // Call the function used to add the required service
        app_add_svc_func_list[app_env.next_svc]();

        // Select following service to add
        app_env.next_svc++;
        more_svc = true;
    }

    return more_svc;

}

void ble_user_enable_prf_cb(uint8_t conidx)
{
    #if (BLE_APP_BATT)
    // Enable Battery Service
    app_batt_enable_prf(conidx);
    #endif //(BLE_APP_BATT)

    #if (BLE_APP_HID)
    // Enable HID Service
    app_hid_enable_prf(conidx);
    #endif //(BLE_APP_HID)

    #if (BLE_APP_SMARTCONFIG)
    app_smartconfig_enable_prf(conidx);
    #endif //(BLE_APP_SMARTCONFIG)
}

void ble_app_connection_cb(uint8_t conidx, struct gapc_connection_req_ind const *param)
{
    user_sleep_allow(0);
    dbg("%s connect %d\r\n", __func__, conidx);
}

void ble_app_disconnect_cb(struct gapc_disconnect_ind const *param)
{
    dbg("%s disconnect %d %d\r\n", __func__, param->conhdl, param->reason);
    #if (BLE_APP_HID)
    app_hid_stop_timer();
    // Restart Advertising
    if (app_env.del_bond) {
        app_env.del_bond = false;
        appm_delete_advertising();
    } else {
        app_ble_start_adv();
    }
    user_sleep_allow(1);
    #endif //(BLE_APP_HID)

    #if BLE_APP_SMARTCONFIG
    app_smartconfig_disconnected();
    #endif
}

void ble_app_update_con_params_cb(struct gapc_param_update_req_ind const *param)
{
    dbg("%s con param update %d %d\r\n", __func__, param->intv_max, param->time_out);

}

void ble_app_adv_stop_cb(struct gapm_activity_stopped_ind *param)
{
    dbg("%s con param update reason %d\r\n", __func__, param->reason);
}

void ble_app_adv_status_cb(struct gapm_cmp_evt const *param)
{
    dbg("%s operation %d, status %d\r\n", __func__, param->operation, param->status);
}

static struct app_callbacks ble_app_callbacks = {
   .app_on_init                        = ble_user_app_init_cb,
   .app_on_init_complete               = ble_user_app_init_complete_cb,
   .app_on_add_svc                     = ble_user_add_svc_cb,
   .app_on_enable_prf                  = ble_user_enable_prf_cb,
   .app_on_connection                  = ble_app_connection_cb,
   .app_on_disconnect                  = ble_app_disconnect_cb,
   .app_on_update_params_request       = ble_app_update_con_params_cb,
   .app_on_adv_status                  = ble_app_adv_status_cb,
};

void app_ble_init(void)
{
    struct bd_addr user_addr = {
    0xaa,0xc0,0x00,0x45,0x56,0x67
};
#if FPGA == 0
    bt_factory_info_t bt_factory_info;
    if(flash_btdm_bt_factory_read((void *)&bt_factory_info,sizeof(bt_factory_info_t)) == INFO_READ_DONE){
        memcpy(bt_addr , bt_factory_info.local_bt_addr ,6);
        memcpy(ble_addr , bt_factory_info.local_ble_addr ,6);
    }
#endif
    #if (PLF_CONSOLE)
    app_ble_test_command_add();
    #endif // (PLF_CONSOLE)

    if (flash_btdm_le_rand_addr_read(&user_addr.addr[0])) {
        dbg("addr error, use default addr\r\n");
    } else {
        memcpy(&ble_addr[0], &user_addr.addr[0], BD_ADDR_LEN);
        flash_btdm_le_rand_addr_write(ble_addr);
    }
    app_ble_env_init();
    app_ble_register_callbak(&ble_app_callbacks);
    app_ble_addr_init(ble_addr, GAPM_STATIC_ADDR);
    app_ble_set_dev_name(APP_DFLT_DEVICE_NAME, APP_DFLT_DEVICE_NAME_LEN);
    app_ble_set_adv_data(adv_data, build_adv_data());
    init_adv_params();
    if (app_ble_set_adv_params(&adv_param, 0))
    {
        TRACE("params error\r\n");
    }
    TRACE("app_ble_init\n");
}
#endif
#endif

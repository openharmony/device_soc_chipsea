/**
 ****************************************************************************************
 *
 * @file app_console.c
 *
 * @brief Implementation of the fully hosted console task.
 *
 * Copyright (C) RivieraWaves 2017-2018
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_CONSOLE
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#ifdef CFG_BT_STACK
#ifndef CFG_BLE_ONLY
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/
#include "plf.h"
#if PLF_CONSOLE
#include "cs_target_config.h"
#include <stdlib.h>
#include "rtos.h"
#include "dbg_assert.h"
#include "console.h"
#include "command.h"
#include "app_bt_queue.h"
#if APP_SUPPORT_A2DP_SOURCE == 1
#include "app_a2dp_source.h"
#endif
#if APP_SUPPORT_HFG == 1
#include "app_hfg.h"
#endif
#include "sleep_api.h"
#if APP_SUPPORT_OTA_BOX
#include "app_ota_box.h"
#endif

#if APP_SUPPORT_TWS == 1
static int  app_tws_sync_control(int argc, char *argv[])
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
        app_bt_tws_sync_control(1);
    }else{
        app_bt_tws_sync_control(0);
    }

    return ERR_NONE;
}

static int  app_tws_mss_control(int argc, char *argv[])
{
    app_bt_tws_mss_control();

    return ERR_NONE;
}

static int  app_tws_stop_local_sync_control(int argc, char *argv[])
{
    app_bt_tws_stop_local_sync_control();

    return ERR_NONE;
}

static int  app_tws_reset(int argc, char *argv[])
{
    app_bt_tws_reset();

    return ERR_NONE;
}

static int  app_tws_disc(int argc, char *argv[])
{
    app_bt_tws_disc();

    return ERR_NONE;
}

static int  app_tws_visible_reopen(int argc, char *argv[])
{
    app_bt_tws_visible_reopen();

    return ERR_NONE;
}
#endif
static int  app_key_enter(int argc, char *argv[])
{
    unsigned int key;

    if (argc < 2) {
        return ERR_WRONG_ARGS;
    }

    key = console_cmd_strtoul(argv[1], NULL, 10);

    app_bt_send_key(key);
    return ERR_NONE;
}
#if FPGA == 0
static int app_erase_flash(int argc, char *argv[])
{
    app_bt_erased_flash();
    return ERR_NONE;
}
#endif
static int app_enable_dut(int argc, char *argv[])
{
    app_bt_dut_mode();
    return ERR_NONE;
}

static int app_onoff_bt_trace(int argc, char *argv[])
{
    unsigned int on_off;
    unsigned int level;

    if (argc < 3) {
        return ERR_WRONG_ARGS;
    }

    on_off = console_cmd_strtoul(argv[1], NULL, 10);
    if (on_off > 1) {
        return ERR_WRONG_ARGS;
    }

    level = console_cmd_strtoul(argv[2], NULL, 10);
    TRACE("level 0x%x\n",level);

    if(on_off){
        cs_change_host_log_level(TRUE,level);
    }else{
        cs_change_host_log_level(FALSE,level);
    }

    return ERR_NONE;
}

static int app_scan_en(int argc, char *argv[])
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
    app_bt_wr_scan_en(scan_en,dut);

    return ERR_NONE;
}

static int app_inquiry_dev(int argc, char *argv[])
{
    unsigned int len;
    unsigned int maxResp;

    if (argc < 2) {
        return ERR_WRONG_ARGS;
    }

    len = console_cmd_strtoul(argv[1], NULL, 10);
    maxResp = console_cmd_strtoul(argv[2], NULL, 10);
    TRACE("inquiry_dev len %d,maxResp %d\n",len,maxResp);
    app_bt_inquiry_dev(len,maxResp);
#if APP_SUPPORT_OTA_BOX
    app_ota_clean_addr_list();
#endif
    return ERR_NONE;
}

static int app_inquiry_cancel(int argc, char *argv[])
{
    TRACE("inquiry_cancel\n");
    app_bt_inquiry_cancel();
    return ERR_NONE;
}

static int app_disc_all_link(int argc, char *argv[])
{
    TRACE("app_disc_all_link\n");
    app_bt_disconnect_all_acl();
    return ERR_NONE;
}

#if APP_SUPPORT_OTA_BOX
static int app_ota_connect(int argc, char *argv[])
{
    BT_ADDR ota_addr;

    if (argc < 7) {
        return ERR_WRONG_ARGS;
    }

    ota_addr.addr[0] = (uint8_t)console_cmd_strtoul(argv[1], NULL, 16);
    ota_addr.addr[1] = (uint8_t)console_cmd_strtoul(argv[2], NULL, 16);
    ota_addr.addr[2] = (uint8_t)console_cmd_strtoul(argv[3], NULL, 16);
    ota_addr.addr[3] = (uint8_t)console_cmd_strtoul(argv[4], NULL, 16);
    ota_addr.addr[4] = (uint8_t)console_cmd_strtoul(argv[5], NULL, 16);
    ota_addr.addr[5] = (uint8_t)console_cmd_strtoul(argv[6], NULL, 16);

    app_bt_ota_connect(&ota_addr);
    return ERR_NONE;
}

static int app_ota_connect_by_id(int argc, char *argv[])
{
    unsigned int id;

    if (argc < 2) {
        return ERR_WRONG_ARGS;
    }
    id = (uint8_t)console_cmd_strtoul(argv[1], NULL, 10);
    dbg("select %d\n",id);
    app_bt_ota_connect_by_id(id);
    return ERR_NONE;
}

#endif

#if APP_SUPPORT_A2DP_SOURCE == 1
static int app_a2dp_source_control(int argc, char *argv[])
{
    unsigned int on_off;

    if (argc < 2) {
        return ERR_WRONG_ARGS;
    }

    on_off = console_cmd_strtoul(argv[1], NULL, 10);
    if (on_off > 2) {
        return ERR_WRONG_ARGS;
    }

    switch(on_off){
        case 0:
            app_a2dp_source_connect_default_device();
            break;
        case 1:
            app_a2dp_source_start();
            break;
        case 2:
            app_a2dp_source_suspend();
            break;
        default:
            break;
    }

    return ERR_NONE;
}
#endif

#if APP_SUPPORT_HFG == 1
static int app_hfg_control(int argc, char *argv[])
{
    unsigned int value;

    if (argc < 2) {
        return ERR_WRONG_ARGS;
    }

    value = console_cmd_strtoul(argv[1], NULL, 10);
    if (value > 2) {
        return ERR_WRONG_ARGS;
    }

    switch(value){
        case 0:
            app_hfg_connect_def_device();
            break;
        case 1:
            app_hfg_connect_sco();
            break;
        case 2:
            app_hfg_disconnect_sco();
            break;
        default:
            break;
    }

    return ERR_NONE;
}
#endif

static int app_user_sleep(int argc, char *argv[])
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

static int app_sleep_level(int argc, char *argv[])
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

static int app_user_wakeup_source(int argc, char *argv[])
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

void app_console_command_add(void)
{
#if APP_SUPPORT_TWS == 1
    console_cmd_add("tws_sync", "tws_sync_control <on/off>", 2, app_tws_sync_control);
    console_cmd_add("tws_mss", "tws_mss", 1, app_tws_mss_control);
    console_cmd_add("tws_stop", "tws_stop", 1, app_tws_stop_local_sync_control);
    console_cmd_add("tws_reset", "tws_reset", 1, app_tws_reset);
    console_cmd_add("tws_disc", "tws_disc", 1, app_tws_disc);
    console_cmd_add("tws_vr", "tws_vr  :tws_visible_reopen", 1, app_tws_visible_reopen);
#endif
    console_cmd_add("key", "key <value>  :app key enter", 2, app_key_enter);
#if FPGA == 0
    console_cmd_add("erased", "erased  :erased flash", 2, app_erase_flash);
#endif
    console_cmd_add("dut", "dut  :enter dut", 1, app_enable_dut);
    console_cmd_add("bt_trace", "bt_trace <on/off> <level>", 3, app_onoff_bt_trace);
    console_cmd_add("scan_en", "scan_en <scan_type> <is_dut>", 3, app_scan_en);
    console_cmd_add("inquiry", "inquiry <len> <maxResp>", 3, app_inquiry_dev);
    console_cmd_add("inquiry_cancel", "inquiry_cancel", 1, app_inquiry_cancel);
    console_cmd_add("bt_disc", "bt_disc", 1, app_disc_all_link);
#if APP_SUPPORT_A2DP_SOURCE == 1
    console_cmd_add("source_control", "source_control <value>", 2, app_a2dp_source_control);
#endif
#if APP_SUPPORT_HFG == 1
    console_cmd_add("hfg_control", "hfg_control <value>", 2, app_hfg_control);
#endif
    console_cmd_add("bt_usrslp", "bt_usrslp <of/off>  :Allow user sleep", 2, app_user_sleep);
    console_cmd_add("bt_slplvl", "bt_slplvl <sleep_level>  :Set system sleep level", 2, app_sleep_level);
    console_cmd_add("bt_usrwusrc", "bt_usrwusrc <src> <en> <arg>  :Set user wakeup source", 4, app_user_wakeup_source);
#if APP_SUPPORT_OTA_BOX
    console_cmd_add("bt_otac", "bt_otac <addr[0]> <addr[1]> <addr[2]> <addr[3]> <addr[4]> <addr[5]>  :ota connect by bt addr", 7, app_ota_connect);
    console_cmd_add("bt_otac_id", "bt_otac_id <addr_list_id>  :ota connect by id", 2, app_ota_connect_by_id);
#endif
}
#endif
#endif
#endif

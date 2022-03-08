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
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/
#include "rtos.h"
#include "ble_task_msg.h"
#if PLF_CONSOLE
#include "console.h"
#endif
#include "ble_task.h"
#include "cs_ble_adp_api.h"
#include "bt_cs8800_drvif.h"
#include "app_ble_only.h"


APP_BLE_HANDLER ble_handler_register_list[BLE_HANDLER_REG_MAX] = {0};
#define APP_BLE_ONLY_MSG_MAX (5)
static rtos_queue app_ble_only_msg_queue;
extern uint8_t ble_addr[6];

int app_ble_handler_register(enum APP_BLE_HANDLER_REG_T user, APP_BLE_HANDLER handler)
{
    ble_handler_register_list[user]= handler;
    return 0;
}

void ble_handler_reg_list_poll(void)
{
    uint8_t i;
    for (i=0; i<BLE_HANDLER_REG_MAX; i++){
        if (ble_handler_register_list[i]){
            ble_handler_register_list[i]();
        }
    }
}

int app_ble_only_msg_process(APP_BLE_ONLY_MSG* msg)
{
    int status = -1;
    TRACE("APP:ble_only_msg id:%d \n",msg->msg_id);
    switch (msg->msg_id) {
        case BLE_DEINIT_DONE:
            status = ble_task_delete();
            break;
        case BLE_SET_LP_LEVEL:
            TRACE("APP: ble lp level = %x\n",msg->param);
            cs_adp_set_lp_level_ble_only((uint8_t)msg->param);
            break;
        case DUT_TEST:
            TRACE("APP: enter dut test\n");
            bt_drv_enable_dut();
            break;
        case DUT_NO_SCAN:
        case DUT_ISCAN:
        case DUT_PSCAN:
        case DUT_BOTH_SCAN:
            {
                bt_drv_scan_en(msg->msg_id - DUT_NO_SCAN);
            }
            break;
        case BLE_TIMER_NOTIFY:
            cs_adp_rw_timer_notify();
            break;
        case BLE_SCAN_START:
            app_ble_start_scan();
            break;
        case BLE_SCAN_STOP:
            app_ble_stop_scan();
            break;
        case BLE_ADV_DATA_UPDATE:
            {
                uint8_t buf[32];

                buf[0] = APP_UPDATE_DATA_LEN + 1;
                buf[1] = GAP_AD_TYPE_COMPLETE_NAME;
                memcpy(buf + 2, APP_UPDATE_DATA, APP_UPDATE_DATA_LEN);
                app_ble_set_adv_data(buf, APP_UPDATE_DATA_LEN + 2);
                app_ble_update_adv_data();
            }
            break;
        case BLE_ADV_PARAM_UPDATE:
            {
                if (app_ble_update_adv_params(APP_ADV_INT_MIN*2, APP_ADV_INT_MIN *2)) {
                    TRACE("ERROR\r\n");
                    return -1;
                }
            }
            break;
        case BLE_ADV_STOP:
            app_ble_stop_adv();
            break;
        case BLE_ADV_START:
            app_ble_start_adv();
            break;
        case BLE_DISCONNECT:
            app_ble_disconnect();
            break;
        case BLE_CON_PARAM_UPDATE:
            app_ble_update_con_params(16, 16, 25, 400);
            break;
        case BLE_DELETE_BOND:
            {
                app_env.del_bond = true;
                app_sec_remove_bond();
                ble_addr[0] = co_rand_byte();
                flash_btdm_le_rand_addr_write(&ble_addr[0]);
                app_ble_update_addr(&ble_addr[0]);
                if (app_env.adv_state == APP_ADV_STATE_STARTED) {
                    app_ble_stop_adv();
                    appm_delete_advertising();
                }
                if (app_env.conhdl || app_env.conidx) {
                    app_ble_disconnect();
                }
                //app_ble_start_adv();
            }
            break;
        default:
            break;
    }
    TRACE("APP: msg id:%d :status:%d exit\n", msg->msg_id, status);
    return status;
}

int app_ble_only_msg_send(APP_BLE_ONLY_MSG* msg)
{
    if(rtos_queue_write(app_ble_only_msg_queue,(void *)msg,(TickType_t)1,false)){
        ASSERT_ERR(0);
    }
    ble_task_queue_notify(false);
    return 0;
}

int app_ble_only_msg_send_isr(APP_BLE_ONLY_MSG* msg)
{
    if(rtos_queue_write(app_ble_only_msg_queue,(void *)msg,(TickType_t)1,true)){
        ASSERT_ERR(0);
    }
    ble_task_queue_notify(true);
    return 0;
}


int app_ble_only_msg_get(APP_BLE_ONLY_MSG* QueueMsg)
{
    if(rtos_queue_read(app_ble_only_msg_queue,(void *)QueueMsg,0,0)) {
        return -1;
    }
    return 0;
}

static void app_ble_only_msg_poll(void)
{
    APP_BLE_ONLY_MSG QueueMsg;
    if (!app_ble_only_msg_get(&QueueMsg)){
        app_ble_only_msg_process(&QueueMsg);
    }
}

int app_ble_only_msg_init(void)
{
    if (rtos_queue_create(sizeof(APP_BLE_ONLY_MSG),APP_BLE_ONLY_MSG_MAX,&app_ble_only_msg_queue)){
        TRACE("APP: Failed to Create app_ble_only_msg_queue\n");
        return -1;
    }
    app_ble_handler_register(BLE_HANDLER_REG_1, app_ble_only_msg_poll);

    return 0;
}

int app_ble_only_msg_deinit(void)
{
    if(app_ble_only_msg_queue){
        rtos_queue_delete(app_ble_only_msg_queue);
        app_ble_only_msg_queue = NULL;
    }
    app_ble_handler_register(BLE_HANDLER_REG_1, NULL);
    return 0;
}

int app_ble_deint_done_msg_send(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_DEINIT_DONE;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_lp_level_msg_send(uint8_t level)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_SET_LP_LEVEL;
    msg.param = (uint32_t)level;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_enter_dut_msg_send(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = DUT_TEST;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_adv_data_update_msg_send(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_ADV_DATA_UPDATE;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_adv_param_update_msg_send(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_ADV_PARAM_UPDATE;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_adv_stop_msg_send(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_ADV_STOP;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_adv_start_msg_send(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_ADV_START;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_disconnect_msg_send(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_DISCONNECT;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_con_param_msg_send(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_CON_PARAM_UPDATE;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_del_bond_msg_send(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_DELETE_BOND;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_scan_msg_start(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_SCAN_START;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_scan_msg_stop(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_SCAN_STOP;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_ble_wr_scan_en(unsigned int scan_en)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = DUT_NO_SCAN + scan_en;
    app_ble_only_msg_send(&msg);
    return 0;
}

int app_rw_timer_notify(void)
{
    APP_BLE_ONLY_MSG msg;
    msg.msg_id = BLE_TIMER_NOTIFY;
    app_ble_only_msg_send(&msg);
    return 0;
}

#if PLF_CONSOLE
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

int do_show_heap(int argc, char *argv[])
{
    int total, used, free, max_used;

    rtos_heap_info(&total, &free, &max_used);
    used = total - free;
    max_used = total - max_used;
    TRACE("RTOS HEAP: free=%d used=%d max_used=%d/%d\n",
                free, used, max_used, total);

    return ERR_NONE;
}



void app_ble_console_command_add(void)
{
    console_cmd_add("ble_on", "ble_on <on/off>  :task on/off", 2, app_ble_onoff_control);
    console_cmd_add("HEAP",     "HEAP  :Show remain heap",         1, do_show_heap);
}
#endif

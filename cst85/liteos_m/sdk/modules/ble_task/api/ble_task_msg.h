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
/***
 * ble_task_msg.h
 */

#ifndef BLE_TASK_MSG_H
#define BLE_TASK_MSG_H

enum APP_BLE_HANDLER_REG_T {
    BLE_HANDLER_REG_0 = 0,
    BLE_HANDLER_REG_1,
    BLE_HANDLER_REG_MAX
};

typedef void (*APP_BLE_HANDLER)(void);

typedef enum _ble_only_msg {
        BLE_DEINIT_DONE                         = 0,
        BLE_SET_LP_LEVEL                           ,
        BLE_TIMER_NOTIFY                           ,
        BLE_SCAN_START                             ,
        BLE_SCAN_STOP                              ,
        BLE_ADV_DATA_UPDATE                        ,
        BLE_ADV_PARAM_UPDATE                       ,
        BLE_ADV_STOP                               ,
        BLE_ADV_START                              ,
        BLE_DISCONNECT                             ,
        BLE_CON_PARAM_UPDATE                       ,
        BLE_DELETE_BOND                            ,
        DUT_TEST                                   ,
        DUT_NO_SCAN                                ,
        DUT_ISCAN                                  ,
        DUT_PSCAN                                  ,
        DUT_BOTH_SCAN                              ,
}ble_only_msg;

typedef struct {
    uint32_t msg_id;
    uint32_t param;
} APP_BLE_ONLY_MSG;

int app_ble_only_msg_init(void);
int app_ble_only_msg_deinit(void);
int app_ble_lp_level_msg_send(uint8_t level);

int app_ble_only_msg_send(APP_BLE_ONLY_MSG* msg);
int app_ble_only_msg_send_isr(APP_BLE_ONLY_MSG* msg);

int app_ble_handler_register(enum APP_BLE_HANDLER_REG_T user, APP_BLE_HANDLER handler);
void ble_handler_reg_list_poll(void);


int app_ble_enter_dut_msg_send(void);
int app_ble_wr_scan_en(unsigned int scan_en);
int app_ble_scan_msg_start(void);
int app_ble_scan_msg_stop(void);
int app_ble_adv_data_update_msg_send(void);
int app_ble_adv_param_update_msg_send(void);
int app_ble_adv_stop_msg_send(void);
int app_ble_adv_start_msg_send(void);
int app_ble_disconnect_msg_send(void);
int app_ble_con_param_msg_send(void);
int app_ble_del_bond_msg_send(void);

#endif /* BT_TASK_MSG_H */


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
/***
 * bt_task_msg.h
 */

#ifndef BT_TASK_MSG_H
#define BT_TASK_MSG_H

enum APP_BT_HANDLER_REG_T {
    HANDLER_REG_0 = 0,
    HANDLER_REG_1,
    HANDLER_REG_2,
    HANDLER_REG_3,
    HANDLER_REG_4,
    HANDLER_REG_MAX
};

typedef void (*APP_BT_HANDLER)(void);


int app_bt_handler_register(enum APP_BT_HANDLER_REG_T user, APP_BT_HANDLER handler);
void handler_reg_list_poll(void);

#endif /* BT_TASK_MSG_H */


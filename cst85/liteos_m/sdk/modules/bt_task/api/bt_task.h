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
#ifndef _BTTASK_H_
#define _BTTASK_H_

#define BT_TASK_SUSPEND_VALUE_1  1
#define BT_TASK_SUSPEND_VALUE_2  2
#define BT_TASK_SUSPEND_VALUE_3  3

int bt_task_init(void);
void bt_task_resume(bool isr,uint32_t value);
uint32_t bt_task_suspend(void);
void bt_task_queue_notify(bool isr);
void bt_task_set_max_priority(void);
void bt_task_set_default_priority(void);

#endif


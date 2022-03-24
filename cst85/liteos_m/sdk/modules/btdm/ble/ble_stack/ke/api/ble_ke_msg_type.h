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
#ifndef _BLE_KE_MSG_TYPE_H
#define _BLE_KE_MSG_TYPE_H

// Task State
typedef uint8_t ke_state_t;

// Task Identifier. Composed by the task type and the task index.
typedef uint16_t ke_task_id_t;

// Builds the task identifier from the type and the index of that task.
#define KE_BUILD_ID(type, index) ( (ke_task_id_t)(((index) << 8)|(type)) )

// Retrieves task type from task id.
#define KE_TYPE_GET(ke_task_id) ((ke_task_id) & 0xFF)

// Retrieves task index number from task id.
#define KE_IDX_GET(ke_task_id) (((ke_task_id) >> 8) & 0xFF)


// Message Identifier. The number of messages is limited to 0xFFFF.
// The message ID is divided in two parts:
// bits[15~8]: task index (no more than 255 tasks support)
// bits[7~0]: message index(no more than 255 messages per task)
typedef uint16_t ke_msg_id_t;

#define KE_MSG_ALLOC(id, dest, src, param_str) \
    (struct param_str*) ke_msg_alloc(id, dest, src, sizeof(struct param_str))

#define KE_MSG_FREE(param_ptr) ke_msg_free(ke_param2msg((param_ptr)))

#define KE_MSG_ALLOC_DYN(id, dest, src, param_str,length)  (struct param_str*)ke_msg_alloc(id, dest, src, \
    (sizeof(struct param_str) + (length)));

#endif // _BLE_KE_MSG_TYPE_H

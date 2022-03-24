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
#ifndef _BLE_KE_MSG_EXTERN_H
#define _BLE_KE_MSG_EXTERN_H

bool ke_msg_in_queue(void const *param_ptr);
ke_msg_id_t ke_msg_src_id_get(void const *param_ptr);
ke_msg_id_t ke_msg_dest_id_get(void const *param_ptr);
void ke_msg_free(struct ke_msg const *param);
void ke_msg_forward_new_id(void const *param_ptr,
                           ke_msg_id_t const msg_id, ke_task_id_t const dest_id, ke_task_id_t const src_id);
void ke_msg_forward(void const *param_ptr, ke_task_id_t const dest_id, ke_task_id_t const src_id);
void ke_msg_send_basic(ke_msg_id_t const id, ke_task_id_t const dest_id, ke_task_id_t const src_id);
void ke_msg_send(void const *param_ptr);
void *ke_msg_alloc(ke_msg_id_t const id, ke_task_id_t const dest_id,
                   ke_task_id_t const src_id, uint16_t const param_len);

#endif // _BLE_KE_MSG_EXTERN_H

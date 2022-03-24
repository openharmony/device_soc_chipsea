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
#ifndef _BLE_KE_MSG_STRUCT_H
#define _BLE_KE_MSG_STRUCT_H

/// Message structure.
struct ke_msg
{
    struct co_list_hdr hdr;     ///< List header for chaining

    ke_msg_id_t     id;         ///< Message id.
    ke_task_id_t    dest_id;    ///< Destination kernel identifier.
    ke_task_id_t    src_id;     ///< Source kernel identifier.
    uint16_t        param_len;  ///< Parameter embedded struct length.
    uint32_t        param[__ARRAY_EMPTY];   ///< Parameter embedded struct. Must be word-aligned.
};


/// Status returned by a task when handling a message
/*@TRACE*/
enum ke_msg_status_tag
{
    KE_MSG_CONSUMED = 0, ///< consumed, msg and ext are freed by the kernel
    KE_MSG_NO_FREE,      ///< consumed, nothing is freed by the kernel
    KE_MSG_SAVED,        ///< not consumed, will be pushed in the saved queue
};

__INLINE struct ke_msg * ke_param2msg(void const *param_ptr)
{
    return (struct ke_msg*) (((uint8_t*)param_ptr) - offsetof(struct ke_msg, param));
}

__INLINE void * ke_msg2param(struct ke_msg const *msg)
{
    return (void*) (((uint8_t*) msg) + offsetof(struct ke_msg, param));
}

#endif // _BLE_KE_MSG_STRUCT_H

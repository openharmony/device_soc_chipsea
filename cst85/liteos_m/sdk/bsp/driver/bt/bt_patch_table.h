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
#ifndef _BT_PATCH_TABLE_H_
#define _BT_PATCH_TABLE_H_

#include <stdint.h>

typedef struct {
    uint32_t dimension;
    uint32_t *ptr;
    uint32_t size;
} bt_patch_tbl_t;

typedef void(*bt_patch_cb_t)(void);

extern uint32_t fw_patch_extra_addr;
extern uint32_t minisys_patch_extra_addr;

extern bt_patch_tbl_t *bt_patch_tbl_a1;
extern bt_patch_tbl_t *bt_patch_tbl_a2;

extern bt_patch_tbl_t *bt_patch_tbl_b1;
extern bt_patch_tbl_t *bt_patch_tbl_b2;

extern bt_patch_tbl_t *bt_patch_tbl_c1;
extern bt_patch_tbl_t *bt_patch_tbl_c2;

extern bt_patch_tbl_t *bt_patch_tbl_d1;
extern bt_patch_tbl_t *bt_patch_tbl_d2;

extern uint32_t bt_patch_tbl_a1_size;
extern uint32_t bt_patch_tbl_a2_size;
extern uint32_t bt_patch_tbl_b1_size;
extern uint32_t bt_patch_tbl_b2_size;
extern uint32_t bt_patch_tbl_c1_size;
extern uint32_t bt_patch_tbl_c2_size;
extern uint32_t bt_patch_tbl_d1_size;
extern uint32_t bt_patch_tbl_d2_size;

void bt_patch_prepare(void);

#endif

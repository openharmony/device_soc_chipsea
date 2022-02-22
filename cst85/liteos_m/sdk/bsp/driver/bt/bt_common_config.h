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
#ifndef _BT_COMMON_CONFIG_H_
#define _BT_COMMON_CONFIG_H_

uint32_t bt_common_get_defalut_fw_addr(void);
/*
 * get bt hci ipc ram base
 * @return bt hci ipc ram base;
 */
uint32_t *bt_common_get_default_ram_base(void);
struct csbt_patch_init_desc *bt_common_get_patch_init_desc(void);
/*
 * bt_common_change_fw_load_in_ram
 * only used for debug fw in ram addr ,fw in rom will mask this function.
 */
void bt_common_change_fw_load_in_ram(void);

#endif

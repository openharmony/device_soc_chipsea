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

#ifndef _BLE_IP_TASK_H
#define _BLE_IP_TASK_H

// Link Layer Tasks
#define TASK_ID_LLM (0)
#define TASK_ID_LLC (1)
#define TASK_ID_LLD (2)
#define TASK_ID_LLI (3)
#define TASK_ID_DBG (4)

// BT Controller Tasks
#define TASK_ID_LM (5)
#define TASK_ID_LC (6)
#define TASK_ID_LB (7)
#define TASK_ID_LD (8)

#define TASK_ID_L2CC  (10)
#define TASK_ID_GATTM (11)
#define TASK_ID_GATTC (12)
#define TASK_ID_GAPM  (13)
#define TASK_ID_GAPC  (14)
#define TASK_ID_APP   (15)

#define TASK_ID_AHI     (16)
#define TASK_ID_HCI     (17)
#define TASK_ID_DISPLAY (19)

#define TASK_ID_DISS        (20)
#define TASK_ID_DISC        (21)
#define TASK_ID_PROXM       (22)
#define TASK_ID_PROXR       (23)
#define TASK_ID_FINDL       (24)
#define TASK_ID_FINDT       (25)
#define TASK_ID_HTPC        (26)
#define TASK_ID_HTPT        (27)
#define TASK_ID_BLPS        (28)
#define TASK_ID_BLPC        (29)
#define TASK_ID_HRPS        (30)
#define TASK_ID_HRPC        (31)
#define TASK_ID_TIPS        (32)
#define TASK_ID_TIPC        (33)
#define TASK_ID_SCPPS       (34)
#define TASK_ID_SCPPC       (35)
#define TASK_ID_BASS        (36)
#define TASK_ID_BASC        (37)
#define TASK_ID_HOGPD       (38)
#define TASK_ID_HOGPBH      (39)
#define TASK_ID_HOGPRH      (40)
#define TASK_ID_GLPS        (41)
#define TASK_ID_GLPC        (42)
#define TASK_ID_RSCPS       (43)
#define TASK_ID_RSCPC       (44)
#define TASK_ID_CSCPS       (45)
#define TASK_ID_CSCPC       (46)
#define TASK_ID_ANPS        (47)
#define TASK_ID_ANPC        (48)
#define TASK_ID_PASPS       (49)
#define TASK_ID_PASPC       (50)
#define TASK_ID_CPPS        (51)
#define TASK_ID_CPPC        (52)
#define TASK_ID_LANS        (53)
#define TASK_ID_LANC        (54)
#define TASK_ID_IPSS        (55)
#define TASK_ID_IPSC        (56)
#define TASK_ID_ENVS        (57)
#define TASK_ID_ENVC        (58)
#define TASK_ID_WSCS        (59)
#define TASK_ID_WSCC        (60)
#define TASK_ID_UDSS        (61)
#define TASK_ID_UDSC        (62)
#define TASK_ID_BCSS        (63)
#define TASK_ID_BCSC        (64)
#define TASK_ID_SMARTCONFIG (66)
#define TASK_ID_AM0         (240)
#define TASK_ID_AM0_HAS     (241)
#define TASK_ID_INVALID     (0xFF)

/// Build the first message ID of a task. (in fact a ke_msg_id_t)
#define TASK_FIRST_MSG(task) ((uint16_t)((task) << 8))

/// Builds the task identifier from the type and the index of that task.
#define TASK_BUILD(type, index) ((uint16_t)(((index) << 8)|(type)) )

/// Retrieves task type from task id.
#define TASK_TYPE_GET(ke_task_id) (((uint16_t)ke_task_id) & 0xFF)

/// Retrieves task index number from task id.
#define TASK_IDX_GET(ke_task_id) ((((uint16_t)ke_task_id) >> 8) & 0xFF)

#endif // _BLE_IP_TASK_H

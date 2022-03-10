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
#ifndef CSADPHID
#define CSADPHID

#include "bt_types_def.h"


typedef U8 bt_hid_adp_state;
#define HID_ADP_STATE_IDLE                    0
#define HID_ADP_STATE_PENDING                 1
#define HID_ADP_STATE_CONNECTED               2


typedef struct _CS_ADP_HID_EVENT_{
    BT_ADDR bdaddr;
    U32 hidparam;
}CS_ADP_HID_EVENT;


void cs_adp_hid_register(   void);

#endif

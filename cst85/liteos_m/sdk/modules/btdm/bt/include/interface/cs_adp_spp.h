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
#ifndef _CS_ADP_SPP_H_
#define _CS_ADP_SPP_H_
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/
#include"bt_types_def.h"

typedef struct _CS_ADP_SPP_EVENT_{
    BT_ADDR bdaddr;
    U32 sppparam;
    U32 sppparam1;
}CS_ADP_SPP_EVENT;


typedef struct
{
    U8*     pTxBuf;
    U16     txDataLength;
} adpspptxdone;

typedef struct
{
    U8*     pRxBuf;
    U16     RxDataLength;
} adpspprxdone;


void cs_adp_spp_init(void);
void cs_adp_spp_connect(BT_ADDR * bdaddr);
void cs_adp_spp_send_data(uint8_t* ptrData, uint16_t length);

#endif

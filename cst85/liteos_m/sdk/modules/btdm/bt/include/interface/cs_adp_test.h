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
#ifndef CSADPTEST
#define CSADPTEST
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/
#include "cs_adp_hfp.h"
typedef struct _CS_ADP_TEST_EVENT_{
    BT_ADDR bdaddr;
    U32 testparam;
}CS_ADP_TEST_EVENT;

typedef struct
{
    U8*     txbuf;
    U16     len;
} TestTxDone;

typedef struct
{
    U8*     rxbuf;
    U16     len;
} TestRxDone;

void cs_adp_test_init(void);
void cs_adp_test_send_sco_data(CS_ADP_HFP_AUDIO_DATA *buffer);
void cs_adp_test_connect(BT_ADDR *Addr);
void cs_adp_test_disconnect(void);
void cs_adp_test_connect_sco(void);
Status_BTDef cs_adp_test_send_data(U8 *data, U16 len, U8 header);

#endif

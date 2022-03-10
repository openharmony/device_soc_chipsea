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
#ifndef CSADPHSP
#define CSADPHSP

#include "bt_types_def.h"


void cs_adp_hsp_init(void);

void cs_adp_hsp_register(   void);
Status_BTDef cs_adp_hsp_send_volume(BT_ADDR bdaddr,uint8_t Gain);
Status_BTDef cs_adp_hsp_send_ckpd_control(BT_ADDR bdaddr);

Status_BTDef cs_adp_hs_connect(BT_ADDR bdaddr);
Status_BTDef cs_adp_hs_sco_connect(BT_ADDR bdaddr);
Status_BTDef cs_adp_hs_sco_disconnect(BT_ADDR bdaddr);
Status_BTDef cs_adp_hs_disconnect(BT_ADDR bdaddr);



#endif

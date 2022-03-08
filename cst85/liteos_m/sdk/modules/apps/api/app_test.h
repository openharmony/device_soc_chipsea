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
#ifndef APPTEST_H
#define APPTEST_H
#include "cs_target_config.h"

#if SCO_CVSD_PLC_TEST == 1

#define HF_SENDBUFF_SIZE (320)
#define HF_SENDBUFF_MEMPOOL_NUM (2)


typedef struct {
    uint8_t buffer[HF_SENDBUFF_SIZE];
    uint16_t len;
}ScoSendData_t;

typedef struct {
    ScoSendData_t mempool[HF_SENDBUFF_MEMPOOL_NUM];
    uint8_t idx;
} HF_SENDBUFF_T;

extern TimerHandle_t connect_sco_timer;

void app_test_msg_handle(CS_EVENT *event);
void app_test_connect_default(void);
void app_test_connect_sco( TimerHandle_t xTimer );


#endif
#endif

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
#ifndef WIFI_CONFIG_H_
#define WIFI_CONFIG_H_

#include "rtos_ohos_al.h"
#include "los_list.h"
#include "wifi_device.h"
#include "wifi_hotspot_config.h"
#include "wifi_host_api.h"

#define PARAM_CHECK(para) do {          \
    if (para == NULL) {                 \
        return ERROR_WIFI_UNKNOWN; \
    }                                   \
} while(0)

typedef struct {
    uint16_t eventId;
    uint32_t payLoad;
} ChipseaWifiMsg;

typedef enum {
    WIFI_START_SCAN = 0,
    WIFI_START_ADVANCE_SCAN = 1,
} WifiEventIdList;

typedef enum {
    WIFI_INACTIVE = 0,
    WIFI_ACTIVE = 1,
    WIFI_CONNECT = 2,
} WifiStaState;

typedef struct {
    uint8_t used;
    WifiDeviceConfig devConf;
} WifiConnectDevice;

typedef struct {
    uint8_t networkId;
    WifiLinkedInfo linkinfo;
} WifiConnectedInfo;

typedef struct {
    WifiEvent *event;
    LOS_DL_LIST node;
} WifiEventNode;

typedef struct {
    WifiStaState state;
    rtos_queue wifiQueue;
    rtos_task_handle wifiTask;
    int32_t scanSize;
    struct fhost_cntrl_link *consoleCntrlLink;
    WifiConnectedInfo connectedInfo;
    WifiConnectDevice deviceTab[WIFI_MAX_CONFIG_SIZE];
    uint8_t eventCnt;
} ChipseaWifiStaData;

typedef struct {
    StationInfo station;
    LOS_DL_LIST node;
} WifiStationNode;

typedef struct {
    uint8_t state;
    rtos_mutex mutex;
    struct fhost_vif_ap_cfg vifApCfg;
    HotspotConfig hostpotCfg;
    LOS_DL_LIST stationHead;
    uint8_t stationCnt;
} ChipseaWifiApData;

void DoScanCallBack(int state, uint8_t size);
void DoStaConnectCallBack(int state, WifiLinkedInfo *info);
void DoStaJoinCallBack(StationInfo *info);
void DoStaLeaveCallBack(StationInfo *info);
void DoApStateCallBack(int state);
void DoApStateCallBack(int state);
int32_t WifiLock();
int32_t WifiUnlock();
void SecureHm2Chipsea(int hmType, uint32_t *akm);
void SecureChipsea2Hm(int *hmType, uint32_t akm);
#endif
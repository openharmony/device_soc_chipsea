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
#include "wifi_device.h"
#include "wifi_config.h"
#include "rtos_ohos_al.h"
#include "dbg.h"

typedef struct {
    rtos_mutex eventMutex;
    LOS_DL_LIST eventLst;
    uint8_t eventCnt;
    uint8_t initFlag;
} WifiEventData;

WifiEventData g_eventData = {0};

static int32_t EventLock()
{
    if (g_eventData.eventMutex == NULL) {
        if (rtos_mutex_create(&g_eventData.eventMutex) != 0) {
            dbg("wifiDevice:mutex create err\r\n");
            return ERROR_WIFI_NOT_AVAILABLE;
        }
    }
    if (g_eventData.eventMutex != NULL) { \
        if (rtos_mutex_lock(g_eventData.eventMutex, osWaitForever) != 0) {
            dbg("wifiDevice:mutex lock err\r\n");
            return ERROR_WIFI_NOT_AVAILABLE;
        }
    }
    return WIFI_SUCCESS;
}

static int32_t EventUnLock()
{
    if (g_eventData.eventMutex == NULL) {
        dbg("wifiDevice:mutex lock null\r\n");
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    if (rtos_mutex_unlock(g_eventData.eventMutex) != 0) {
        dbg("wifiDevice:mutex unlock err\r\n");
        return ERROR_WIFI_NOT_AVAILABLE;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode RegisterWifiEvent(WifiEvent *event)
{
    PARAM_CHECK(event);

    EventLock();

    if (g_eventData.eventLst.pstNext == NULL) {
        LOS_ListInit(&g_eventData.eventLst);
    }

    if (g_eventData.eventCnt >= WIFI_MAX_EVENT_SIZE) {
        dbg("wifiDevice: RegisterWifiEvent! eventCnt overflow %d\r\n", g_eventData.eventCnt);
        EventUnLock();
        return WIFI_SUCCESS;
    }

    WifiEventNode *eventNode = (WifiEventNode *)rtos_calloc(1, sizeof(*eventNode));
    if (eventNode == NULL) {
        EventUnLock();
        return ERROR_WIFI_UNKNOWN;
    }

    eventNode->event = event;
    LOS_ListTailInsert(&g_eventData.eventLst, &eventNode->node);
    g_eventData.eventCnt++;
    EventUnLock();

    dbg("wifiDevice: RegisterWifiEvent!\r\n");
    return WIFI_SUCCESS;
}

WifiErrorCode UnRegisterWifiEvent(const WifiEvent *event)
{
    PARAM_CHECK(event);
    WifiEventNode *pos = NULL;

    EventLock();

    if (g_eventData.eventCnt == 0) {
        dbg("wifiDevice: eventCnt = 0!\r\n");
        EventUnLock();
        return ERROR_WIFI_UNKNOWN;
    }

    LOS_DL_LIST_FOR_EACH_ENTRY(pos, &g_eventData.eventLst, WifiEventNode, node) {
        if (pos->event == event) {
            LOS_ListDelete(&pos->node);
            rtos_free(pos);
            g_eventData.eventCnt--;
            EventUnLock();
            return WIFI_SUCCESS;
        }
    }
    EventUnLock();
    return ERROR_WIFI_UNKNOWN;
}

void DoScanCallBack(int state, uint8_t size)
{
    WifiEventNode *pos = NULL;
    EventLock();
    /* scan callback */
    LOS_DL_LIST_FOR_EACH_ENTRY(pos, &g_eventData.eventLst, WifiEventNode, node) {
        if (pos->event->OnWifiScanStateChanged != NULL) {
            pos->event->OnWifiScanStateChanged(state, size);
        }
    }
    EventUnLock();
}

void DoStaConnectCallBack(int state, WifiLinkedInfo *info)
{
    WifiEventNode *pos = NULL;
    EventLock();
    if (info != NULL) {
        LOS_DL_LIST_FOR_EACH_ENTRY(pos, &g_eventData.eventLst, WifiEventNode, node) {
            if (pos->event->OnWifiConnectionChanged != NULL) {
                pos->event->OnWifiConnectionChanged(state, info);
            }
        }
    }
    EventUnLock();
}

void DoStaJoinCallBack(StationInfo *info)
{
    WifiEventNode *pos = NULL;
    EventLock();
    if (info != NULL) {
        LOS_DL_LIST_FOR_EACH_ENTRY(pos, &g_eventData.eventLst, WifiEventNode, node) {
            if (pos->event->OnHotspotStaJoin != NULL) {
                pos->event->OnHotspotStaJoin(info);
            }
        }
    }
    EventUnLock();
}

void DoStaLeaveCallBack(StationInfo *info)
{
    WifiEventNode *pos = NULL;
    dbg("DoStaLeaveCallBack\r\n");
    EventLock();
    if (info != NULL) {
        LOS_DL_LIST_FOR_EACH_ENTRY(pos, &g_eventData.eventLst, WifiEventNode, node) {
            if (pos->event->OnHotspotStaLeave != NULL) {
                pos->event->OnHotspotStaLeave(info);
            }
        }
    }
    EventUnLock();
}

void DoApStateCallBack(int state)
{
    WifiEventNode *pos = NULL;
    EventLock();
    /* scan callback */
    LOS_DL_LIST_FOR_EACH_ENTRY(pos, &g_eventData.eventLst, WifiEventNode, node) {
        if (pos->event->OnHotspotStateChanged != NULL) {
            pos->event->OnHotspotStateChanged(state);
        }
    }
    EventUnLock();
}

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
#include "wifi_config.h"
#include "wifi_host.h"
#include "wifi_host_config.h"
#include "wlan_if.h"
#include "porting_net_def.h"
#include "rwnx_defs.h"
#include "wifi_cfg_nx.h"
#include "securec.h"

#define DEFAULT_STA_VIF 0
#define MAX_WIFI_EVENT_NUM 10
#define MAX_WIFI_KEY_LEN 64

static ChipseaWifiStaData g_wifiData = {0};

rtos_mutex g_wifiMutex = NULL;

#define WIFI_STATE_INVALID_CHECK(sta) do { \
    if (g_wifiData.state == (sta)) {       \
        return ERROR_WIFI_UNKNOWN;   \
    }                                      \
} while(0)

#define WIFI_CLOSE_LINK(link) do { \
    if ((link) != NULL) { \
        fhost_cntrl_cfgrwnx_link_close(link); \
        (link) = NULL; \
    } \
} while(0)

void WifiDeviceEntry(void *arg);

void SecureHm2Chipsea(int hmType, uint32_t *akm)
{
    *akm = 0;
    switch (hmType) {
        case WIFI_SEC_TYPE_WEP:
            *akm |= CO_BIT(MAC_AKM_PRE_RSN);
            break;
        case WIFI_SEC_TYPE_PSK:
            *akm |= CO_BIT(MAC_AKM_PRE_RSN) | CO_BIT(MAC_AKM_PSK);
            break;
        case WIFI_SEC_TYPE_SAE:
            *akm |= CO_BIT(MAC_AKM_SAE);
            break;
        default:
            *akm |= CO_BIT(MAC_AKM_NONE);
    }
    dbg("SecureHm2Chipsea hmType = %d,akm = %d\r\n", hmType, *akm);
}

void SecureChipsea2Hm(int *hmType, uint32_t akm)
{
    if (akm & CO_BIT(MAC_AKM_PRE_RSN)) {
        *hmType = WIFI_SEC_TYPE_WEP;
    } else if (akm & CO_BIT(MAC_AKM_PSK)) {
        *hmType = WIFI_SEC_TYPE_WEP;
    } else if (akm & CO_BIT(MAC_AKM_SAE)) {
        *hmType = WIFI_SEC_TYPE_SAE;
    } else {
        *hmType = WIFI_SEC_TYPE_OPEN;
    }
    dbg("SecureChipsea2Hm akm = %d,hmType = %d\r\n", akm, *hmType);
}

int32_t WifiCreateLock()
{
    if (g_wifiMutex == NULL) {
        if (rtos_mutex_create(&g_wifiMutex) != 0) {
            dbg("wifiDevice:mutex create err\r\n");
            return ERROR_WIFI_NOT_AVAILABLE;
        }
    }
    if (g_wifiMutex != NULL) {
        if (rtos_mutex_lock(g_wifiMutex, osWaitForever) != 0) {
            dbg("wifiDevice:mutex lock err\r\n");
            return ERROR_WIFI_NOT_AVAILABLE;
        }
    }
    return WIFI_SUCCESS;
}

int32_t WifiUnlock()
{
    if (g_wifiMutex == NULL) {
        dbg("wifiDevice:mutex lock null\r\n");
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    if (rtos_mutex_unlock(g_wifiMutex) != 0) {
        dbg("wifiDevice:mutex unlock err\r\n");
        return ERROR_WIFI_NOT_AVAILABLE;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode AddDeviceConfig(const WifiDeviceConfig *config, int *result)
{
    PARAM_CHECK(config);
    PARAM_CHECK(result);
    uint8_t index;

    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    for (index = 0; index < WIFI_MAX_CONFIG_SIZE; index++) {
        if (g_wifiData.deviceTab[index].used == 0) {
            memcpy(&g_wifiData.deviceTab[index].devConf, config, sizeof(WifiDeviceConfig));
            g_wifiData.deviceTab[index].used = 1;
            g_wifiData.deviceTab[index].devConf.netId = index;
            *result = index;
            WifiUnlock();
            dbg("wifiDevice:AddDeviceConfig success netid = %d\r\n", *result);
            return WIFI_SUCCESS;
        }
    }

    WifiUnlock();
    dbg("wifiDevice:AddDeviceConfig err index = %d\r\n", index);
    return ERROR_WIFI_BUSY;
}

WifiErrorCode GetDeviceConfigs(WifiDeviceConfig *result, unsigned int *size)
{
    PARAM_CHECK(result);
    PARAM_CHECK(size);
    uint8_t index;
    uint8_t cnt;

    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    for (index = 0, cnt = 0; index < WIFI_MAX_CONFIG_SIZE; index++) {
        if (g_wifiData.deviceTab[index].used == 1) {
            memcpy(result + cnt, &g_wifiData.deviceTab[index].devConf, sizeof(WifiDeviceConfig));
            cnt++;
        }
    }

    *size = cnt;
    WifiUnlock();
    dbg("wifiDevice:GetDeviceConfigs size = %d\r\n", *size);
    return (*size == 0) ? ERROR_WIFI_NOT_AVAILABLE : WIFI_SUCCESS;
}

WifiErrorCode RemoveDevice(int networkId)
{
    memset(&g_wifiData.deviceTab[networkId], 0, sizeof(WifiConnectDevice));
    dbg("wifiDevice:RemoveDevice networkId = %d\r\n", networkId);
    return WIFI_SUCCESS;
}

static int32_t SetStaVif()
{
    ipc_host_cntrl_start();

    WIFI_CLOSE_LINK(g_wifiData.consoleCntrlLink);
    g_wifiData.consoleCntrlLink = fhost_cntrl_cfgrwnx_link_open();
    if (g_wifiData.consoleCntrlLink == NULL) {
        dbg("Failed to open link with control task\n");
        return ERROR_WIFI_UNKNOWN;
    }

    if (fhost_set_vif_type(g_wifiData.consoleCntrlLink, DEFAULT_STA_VIF, VIF_UNKNOWN, false) ||
        fhost_set_vif_type(g_wifiData.consoleCntrlLink, DEFAULT_STA_VIF, VIF_STA, false)) {
        WIFI_CLOSE_LINK(g_wifiData.consoleCntrlLink);
        return ERROR_WIFI_UNKNOWN;
    }

    return WIFI_SUCCESS;
}

WifiErrorCode Scan(void)
{
    WIFI_STATE_INVALID_CHECK(WIFI_INACTIVE);

    ChipseaWifiMsg msg = {
        .eventId = WIFI_START_SCAN,
        .payLoad = 0,
    };

    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }
    if (rtos_queue_write(g_wifiData.wifiQueue, &msg, 1, false) != 0) {
        dbg("wifiDevice:rtos_queue_write err\r\n");
        WifiUnlock();
        return ERROR_WIFI_NOT_AVAILABLE;
    }
    WifiUnlock();
    return WIFI_SUCCESS;
}

int32_t DoScan(void)
{
    WIFI_STATE_INVALID_CHECK(WIFI_STA_NOT_ACTIVE);

    g_wifiData.scanSize = 0;
    if (SetStaVif() != WIFI_SUCCESS) {
        dbg("SetStaVif err\r\n");
        DoScanCallBack(WIFI_STATE_NOT_AVAILABLE, 0);
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    g_wifiData.scanSize = fhost_scan(g_wifiData.consoleCntrlLink, DEFAULT_STA_VIF, NULL);
    if (g_wifiData.scanSize < 0) {
        dbg("wifiDevice:scan size err:%d\r\n", g_wifiData.scanSize);
        WIFI_CLOSE_LINK(g_wifiData.consoleCntrlLink);
        DoScanCallBack(WIFI_STATE_NOT_AVAILABLE, 0);
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    DoScanCallBack(WIFI_STATE_AVAILABLE, g_wifiData.scanSize);
    return WIFI_SUCCESS;
}

WifiErrorCode AdvanceScan(WifiScanParams *params)
{
    WIFI_STATE_INVALID_CHECK(WIFI_STA_NOT_ACTIVE);
    PARAM_CHECK(params);

    dbg("AdvanceScan start = %d\r\n", params->scanType);

    WifiScanParams *paramSave = rtos_calloc(1, sizeof(WifiScanParams));
    if (paramSave == NULL) {
        return ERROR_WIFI_UNKNOWN;
    }

    paramSave->scanType = params->scanType;
    switch (params->scanType) {
        case WIFI_FREQ_SCAN:
            if (params->freqs == 0) {
                dbg("check freqs = 0\r\n");
                return ERROR_WIFI_UNKNOWN;
            }
            break;
        case WIFI_SSID_SCAN:
            if ((params->ssidLen == 0) || (memcmp(paramSave->ssid, params->ssid, params->ssidLen) == 0)) {
                dbg("ssid len = 0\r\n");
                return ERROR_WIFI_UNKNOWN;
            }
            paramSave->ssidLen = params->ssidLen;
            memcpy_s(paramSave->ssid, WIFI_MAX_SSID_LEN, params->ssid, params->ssidLen);
            break;
        case WIFI_BSSID_SCAN:
            if (memcmp(paramSave->bssid, params->bssid, sizeof(paramSave->bssid)) == 0) {
                dbg("bssid len = 0\r\n");
                return ERROR_WIFI_UNKNOWN;
            }
            memcpy_s(paramSave->bssid, WIFI_MAC_LEN, params->bssid, WIFI_MAC_LEN);
            break;
        case WIFI_BAND_SCAN:
            paramSave->band = params->band;
            break;
        default:
            paramSave->scanType = WIFI_BAND_SCAN;
            paramSave->band = PHY_BAND_2G4;
            dbg("default scan,use band 2g\r\n");
            break;
    }

    ChipseaWifiMsg msg = {
        .eventId = WIFI_START_ADVANCE_SCAN,
        .payLoad = (uint32_t)paramSave,
    };

    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }
    if (rtos_queue_write(g_wifiData.wifiQueue, &msg, 1, false) != 0) {
        dbg("wifiDevice:rtos_queue_write err\r\n");
        WifiUnlock();
        return ERROR_WIFI_NOT_AVAILABLE;
    }
    WifiUnlock();
    return WIFI_SUCCESS;
}

void DoAdvanceScan(WifiScanParams *params)
{
    PARAM_CHECK(params);
    struct fhost_super_scan_t scanParam = {0};
    struct cfgrwnx_scan_ssid ssid = {0};

    g_wifiData.scanSize = 0;

    switch (params->scanType) {
        case WIFI_FREQ_SCAN:
            scanParam.band = -1;
            scanParam.freqs = &params->freqs;
            break;
        case WIFI_SSID_SCAN:
            ssid.len = params->ssidLen;
            ssid.ssid = (uint8_t *)params->ssid;
            scanParam.ssid_cnt = 1;
            scanParam.ssids = &ssid;
            break;
        case WIFI_BSSID_SCAN:
            scanParam.bssid = (uint8_t *)params->bssid;
            break;
        case WIFI_BAND_SCAN:
            scanParam.band = params->band;
            break;
        default:
            rtos_free(params);
            DoScanCallBack(WIFI_STATE_AVAILABLE, 0);
            return;
        break;
    }
    rtos_free(params);

    if (SetStaVif() != WIFI_SUCCESS) {
        dbg("SetStaVif err\r\n");
        DoScanCallBack(WIFI_STATE_AVAILABLE, 0);
        return;
    }

    g_wifiData.scanSize = fhost_super_scan(g_wifiData.consoleCntrlLink, 0, &scanParam);
    if (g_wifiData.scanSize < 0) {
        WIFI_CLOSE_LINK(g_wifiData.consoleCntrlLink);
        g_wifiData.scanSize = 0;
        dbg("fhost_super_scan,no result!\r\n");
    }

    DoScanCallBack(WIFI_STATE_AVAILABLE, g_wifiData.scanSize);
}

WifiErrorCode GetScanInfoList(WifiScanInfo *result, unsigned int *size)
{
    PARAM_CHECK(result);
    PARAM_CHECK(size);

    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    struct mac_scan_result scanRst;
    int32_t rstIndex = 0;

    if (g_wifiData.scanSize <= 0 || g_wifiData.consoleCntrlLink == NULL) {
        *size = 0;
        WifiUnlock();
        dbg("get scan list not init!\r\n");
        return WIFI_SUCCESS;
    }

    while(fhost_get_scan_results(g_wifiData.consoleCntrlLink, rstIndex, 1, &scanRst)) {
        scanRst.ssid.array[scanRst.ssid.length] = '\0';

        strcpy((result + rstIndex)->ssid, (char *)scanRst.ssid.array);
        memcpy(result->bssid, scanRst.bssid.array, WIFI_MAC_LEN);

        (result + rstIndex)->band = scanRst.chan->band;
        (result + rstIndex)->frequency = scanRst.chan->freq;
        SecureChipsea2Hm((&(result + rstIndex)->securityType), scanRst.akm);
        (result + rstIndex)->rssi = (int8_t)scanRst.rssi;

        dbg("wifiDevice:(%3d dBm) CH=%3d BSSID=%02x:%02x:%02x:%02x:%02x:%02x SSID=%s,group_cipher = %d,akm = %d\n",
            (int8_t)scanRst.rssi, phy_freq_to_channel(scanRst.chan->band, scanRst.chan->freq),
            ((uint8_t *)scanRst.bssid.array)[0], ((uint8_t *)scanRst.bssid.array)[1],
            ((uint8_t *)scanRst.bssid.array)[2], ((uint8_t *)scanRst.bssid.array)[3],
            ((uint8_t *)scanRst.bssid.array)[4], ((uint8_t *)scanRst.bssid.array)[5],
            (char *)scanRst.ssid.array, scanRst.group_cipher, scanRst.akm);
        rstIndex++;
    }

    WIFI_CLOSE_LINK(g_wifiData.consoleCntrlLink);
    *size = rstIndex;
    /* close already,scan again */
    g_wifiData.state = WIFI_ACTIVE;
    WifiUnlock();
    return WIFI_SUCCESS;
}

static void AfterConnect(int networkId, WifiConnectDevice *device)
{
    WifiConnectedInfo *info;
    info = &g_wifiData.connectedInfo;
    memset(info, 0, sizeof(WifiConnectedInfo));
    info->networkId = networkId;
    memcpy(info->linkinfo.bssid, device->devConf.bssid, WIFI_MAC_LEN);
    strcpy(info->linkinfo.ssid, device->devConf.ssid);
    info->linkinfo.rssi = data_pkt_rssi_get();
    DoStaConnectCallBack(WIFI_STATE_AVAILABLE, &info->linkinfo);
}

static int32_t StaConfig(WifiConnectDevice *device, struct fhost_vif_sta_cfg *staCfg)
{
    memset(staCfg, 0, sizeof(struct fhost_vif_sta_cfg));
    //timeout 30s
    staCfg->timeout_ms = 30000;
    if (memcpy_s(staCfg->ssid.array, MAC_SSID_LEN, device->devConf.ssid, strlen(device->devConf.ssid)) != EOK) {
        dbg("set ssid fail!\rn");
        return ERROR_WIFI_UNKNOWN;
    }
    staCfg->ssid.length = strlen(device->devConf.ssid);

    if (strcpy_s(staCfg->key, MAX_WIFI_KEY_LEN, device->devConf.preSharedKey) != EOK) {
        return ERROR_WIFI_UNKNOWN;
    }

    SecureHm2Chipsea(device->devConf.securityType, &staCfg->akm);
    return WIFI_SUCCESS;
}

WifiErrorCode ConnectTo(int networkId)
{
    int32_t ret;
    net_if_t *net_if = NULL;
    WifiConnectDevice *device;
    struct fhost_vif_sta_cfg staCfg;

    if (networkId >= WIFI_MAX_CONFIG_SIZE) {
        goto connect_err;
    }

    dbg("Connect to %d\r\n", networkId);
    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    if ((g_wifiData.state == WIFI_STA_NOT_ACTIVE) || (g_wifiData.state == WIFI_CONNECT)) {
        dbg("device state err %d\r\n", g_wifiData.state);
        goto connect_err;
    }

    device = &g_wifiData.deviceTab[networkId];
    if (device->used != 1) {
        dbg("network id not used\r\n");
        goto connect_err;
    }

    ret = SetStaVif();
    WIFI_CLOSE_LINK(g_wifiData.consoleCntrlLink);
    if (ret != WIFI_SUCCESS) {
        dbg("SetStaVif err\r\n");
        goto connect_err;
    }

    ret = StaConfig(device, &staCfg);
    if (ret != WIFI_SUCCESS) {
        dbg("StaConfig err\r\n");
        goto connect_err;
    }

    dbg("connect param ssid = %s,len = %d, key = %s,akm = %d\r\n",
        staCfg.ssid.array,staCfg.ssid.length,staCfg.key,staCfg.akm);
    if (fhost_sta_cfg(DEFAULT_STA_VIF, &staCfg)) {
        dbg("enable wifi device err!\r\n");
        goto connect_err;
    }

    net_if = net_if_find_from_wifi_idx(DEFAULT_STA_VIF);
    if (net_if == NULL) {
        dbg("[CS] net_if_find_from_wifi_idx fail\r\n");
        goto connect_err;
    }

    if (wlan_dhcp(net_if)) {
        wlan_disconnect_sta(DEFAULT_STA_VIF);
        dbg("[CS] dhcp fail\r\n");
        goto connect_err;
    }

    net_if_set_default(net_if);

    g_wifiData.state = WIFI_CONNECT;

    AfterConnect(networkId, device);
    WifiUnlock();
    return WIFI_SUCCESS;
connect_err:
    WifiUnlock();
    DoStaConnectCallBack(WIFI_STATE_NOT_AVAILABLE, NULL);
    return ERROR_WIFI_UNKNOWN;
}

WifiErrorCode Disconnect(void)
{
    dbg("Disconnect!\r\n");
    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    if (g_wifiData.state != WIFI_CONNECT) {
        WifiUnlock();
        return ERROR_WIFI_UNKNOWN;
    }

    wlan_disconnect_sta(DEFAULT_STA_VIF);
    (void)memset(&g_wifiData.connectedInfo, 0, sizeof(WifiConnectedInfo));
    g_wifiData.state = WIFI_ACTIVE;
    WifiUnlock();
    return WIFI_SUCCESS;
}

WifiErrorCode GetLinkedInfo(WifiLinkedInfo *result)
{
    PARAM_CHECK(result);
    result->rssi = data_pkt_rssi_get();
    memcpy(result, &g_wifiData.connectedInfo.linkinfo, sizeof(WifiLinkedInfo));
    return WIFI_SUCCESS;
}

WifiErrorCode GetDeviceMacAddress(unsigned char *result)
{
    PARAM_CHECK(result);
    unsigned char *mac = get_mac_address();
    if (mac != NULL) {
        (void)memcpy(result, mac, WIFI_MAC_LEN);
        return WIFI_SUCCESS;
    }
    return ERROR_WIFI_UNKNOWN;
}

WifiErrorCode SetCountryCode(const char *countryCode)
{
    PARAM_CHECK(countryCode);
    return WIFI_SUCCESS;
}

WifiErrorCode GetCountryCode(char *countryCode, unsigned int *len)
{
    PARAM_CHECK(countryCode);
    PARAM_CHECK(len);
    return WIFI_SUCCESS;
}

WifiErrorCode EnableWifi(void)
{
    dbg("wifiDevice:start wifi device!\r\n");
    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    if (g_wifiData.state != WIFI_INACTIVE) {
        WifiUnlock();
        dbg("wifiDevice:wifi device busy!\r\n");
        return ERROR_WIFI_BUSY;
    }

    g_wifiData.eventCnt = 0;

    if (g_wifiData.wifiQueue == NULL) {
        if (rtos_queue_create(sizeof(ChipseaWifiMsg), MAX_WIFI_EVENT_NUM, &g_wifiData.wifiQueue) != 0) {
            WifiUnlock();
            dbg("wifiDevice:rtos_queue_create err!\r\n");
            return ERROR_WIFI_NOT_AVAILABLE;
        }
    }

    if (g_wifiData.wifiTask == NULL) {
        if (rtos_task_create(WifiDeviceEntry, "wifi device", 0, 0x500, NULL, RTOS_TASK_PRIORITY(2),
            &g_wifiData.wifiTask)!= 0) {
            dbg("wifiDevice:rtos_task_create err!\r\n");
            WifiUnlock();
            return ERROR_WIFI_NOT_AVAILABLE;
        }
    }

    g_wifiData.state = WIFI_ACTIVE;

    WifiUnlock();
    return WIFI_SUCCESS;
}

WifiErrorCode DisableWifi(void)
{
    dbg("Disable wifi!\r\n");
    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    if (g_wifiData.state == WIFI_INACTIVE) {
        WifiUnlock();
        return ERROR_WIFI_NOT_STARTED;
    }

    if (g_wifiData.state == WIFI_CONNECT) {
        wlan_disconnect_sta(DEFAULT_STA_VIF);
        (void)memset(&g_wifiData.connectedInfo, 0, sizeof(WifiConnectedInfo));
    }

    g_wifiData.state = WIFI_INACTIVE;
    WifiUnlock();
    return WIFI_SUCCESS;
}

int IsWifiActive(void)
{
    return (g_wifiData.state == WIFI_ACTIVE) ? WIFI_ACTIVE : WIFI_STA_NOT_ACTIVE;
}

void WifiDeviceEntry(void *arg)
{
    ChipseaWifiMsg wifiMsg;

    while (1) {
        if (rtos_queue_read(g_wifiData.wifiQueue, &wifiMsg, osWaitForever, false) == 0) {
            switch (wifiMsg.eventId) {
                case WIFI_START_SCAN:
                    DoScan();
                    break;
                case WIFI_START_ADVANCE_SCAN:
                    DoAdvanceScan((WifiScanParams *)wifiMsg.payLoad);
                    break;
                default :
                    dbg("wifiDevice:not support cmd %d\r\n", wifiMsg.eventId);
                    break;
            }
        }

        rtos_task_suspend(1);
    }
}

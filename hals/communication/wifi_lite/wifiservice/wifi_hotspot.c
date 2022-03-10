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
#include "wifi_event.h"
#include "wifi_error_code.h"
#include "wifi_device_config.h"
#include "wifi_hotspot_config.h"
#include "inet.h"
#include "fhost.h"
#include "dhcp.h"
#include "dhcps.h"
#include "wlan_if.h"
#include "wifi_config.h"
#include "securec.h"

#define IP_MASK 0x00FFFFFF
#define DEFAULT_AP_VIF 0
#define DEFAULT_BAND PHY_BAND_2G4
#define DEFAULT_CHANNLE 7
#define RSSI_LEVEL_4_2_G (-65)
#define RSSI_LEVEL_3_2_G (-75)
#define RSSI_LEVEL_2_2_G (-82)
#define RSSI_LEVEL_1_2_G (-88)
#define RSSI_LEVEL_4_5_G (-65)
#define RSSI_LEVEL_3_5_G (-72)
#define RSSI_LEVEL_2_5_G (-79)
#define RSSI_LEVEL_1_5_G (-85)

ChipseaWifiApData g_apData = {0};

static int32_t SetApVif()
{
    struct fhost_vif_tag *fhost_vif;
    struct fhost_cntrl_link *apLink;
    ipc_host_cntrl_start();

    apLink = fhost_cntrl_cfgrwnx_link_open();
    if (apLink == NULL) {
        dbg("Failed to open link with control task\n");
        return ERROR_WIFI_UNKNOWN;
    }

    if (fhost_set_vif_type(apLink, DEFAULT_AP_VIF, VIF_UNKNOWN, false) ||
        fhost_set_vif_type(apLink, DEFAULT_AP_VIF, VIF_AP, false)) {
        fhost_cntrl_cfgrwnx_link_close(apLink);
        return ERROR_WIFI_UNKNOWN;
    }

    fhost_cntrl_cfgrwnx_link_close(apLink);

    fhost_vif = &fhost_env.vif[DEFAULT_AP_VIF];
    MAC_ADDR_CPY(&(vif_info_tab[DEFAULT_AP_VIF].mac_addr), &(fhost_vif->mac_addr));

    return WIFI_SUCCESS;
}

WifiErrorCode EnableHotspot(void)
{
    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    if (g_apData.state == WIFI_HOTSPOT_ACTIVE) {
        dbg("ap has enabled\r\n");
        WifiUnlock();
        return ERROR_WIFI_UNKNOWN;
    }

    LOS_ListInit(&g_apData.stationHead);

    if (g_apData.vifApCfg.ssid.length == 0) {
        dbg("Failed to start AP, check ssid\r\n");
        goto enable_err;
    }

    if (SetApVif() != WIFI_SUCCESS) {
        dbg("SetApVif err\r\n");
        goto enable_err;
    }

    if (fhost_ap_cfg(DEFAULT_AP_VIF, &g_apData.vifApCfg)) {
        dbg("Failed to start AP, check your configuration\r\n");
        goto enable_err;
    }

    net_if_t *net_if = fhost_to_net_if(DEFAULT_AP_VIF);
    if (net_if == NULL) {
        dbg("[CS] net_if_find_from_wifi_idx fail\r\n");
        goto enable_err;
    }

    uint32_t ip_addr = get_ap_ip_addr();
    net_if_set_ip(net_if, ip_addr, IP_MASK, 0);

    //set up DHCP server
    dhcpServerStart(net_if);

    // Now that we got an IP address use this interface as default
    net_if_set_default(net_if);

    fhost_tx_task_init();

    dbg("DHCPS init: ip=%d.%d.%d.%d\r\n", (ip_addr)&0xFF, (ip_addr>>8)&0xFF, (ip_addr>>16)&0xFF, (ip_addr>>24)&0xFF);

    g_apData.state = WIFI_HOTSPOT_ACTIVE;
    DoApStateCallBack(WIFI_STATE_AVALIABLE);
    WifiUnlock();
    return WIFI_SUCCESS;
enable_err:
    DoApStateCallBack(WIFI_STATE_NOT_AVALIABLE);
    WifiUnlock();
    return ERROR_WIFI_UNKNOWN;
}

WifiErrorCode DisableHotspot(void)
{
    WifiStationNode *pos = NULL;
    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }
    if (g_apData.state == WIFI_HOTSPOT_NOT_ACTIVE) {
        dbg("wifi ap has closed\r\n");
        WifiUnlock();
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    wlan_stop_ap();
    LOS_DL_LIST_FOR_EACH_ENTRY(pos, &g_apData.stationHead, WifiStationNode, node) {
        LOS_ListDelete(&pos->node);
        rtos_free(pos);
    }
    g_apData.stationCnt = 0;
    g_apData.state = WIFI_HOTSPOT_NOT_ACTIVE;
    DoApStateCallBack(WIFI_STATE_NOT_AVALIABLE);
    WifiUnlock();
    return WIFI_SUCCESS;
}

static void SetSecType(WifiSecurityType security_type,struct fhost_vif_ap_cfg *cfg)
{
    cfg->akm = 0;
    switch (security_type) {
        case WIFI_SEC_TYPE_WEP:
            cfg->akm |= CO_BIT(MAC_AKM_PRE_RSN);
            break;
        case WIFI_SEC_TYPE_PSK:
            cfg->akm |= CO_BIT(MAC_AKM_PRE_RSN) | CO_BIT(MAC_AKM_PSK);
            break;
        case WIFI_SEC_TYPE_SAE:
            cfg->akm |= CO_BIT(MAC_AKM_SAE);
            break;
        default:
            cfg->akm |= CO_BIT(MAC_AKM_NONE);
            break;
    }
    dbg("WifiAp:set akm =%d\r\n", cfg->akm);
}

static int32_t SetChannel(int channelNum, int band, struct fhost_vif_ap_cfg *cfg)
{
    if (band == HOTSPOT_BAND_TYPE_2G) {
        cfg->chan.band = PHY_BAND_2G4;
    } else if (band == HOTSPOT_BAND_TYPE_5G) {
        cfg->chan.band = PHY_BAND_5G;
    } else {
        dbg("Invalid band set default 2G4\r\n");
        cfg->chan.band = DEFAULT_BAND;
    }

    channelNum = (channelNum == 0) ? DEFAULT_CHANNLE : channelNum;
    cfg->chan.prim20_freq = phy_channel_to_freq(cfg->chan.band, channelNum);
    cfg->chan.type = PHY_CHNL_BW_20;
    cfg->chan.center1_freq = cfg->chan.prim20_freq;
    return WIFI_SUCCESS;
}

WifiErrorCode SetHotspotConfig(const HotspotConfig *config)
{
    PARAM_CHECK(config);

    if (WifiCreateLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }
    memset(&g_apData.vifApCfg, 0, sizeof(struct fhost_vif_ap_cfg));

    if (strcpy_s((char *)g_apData.vifApCfg.ssid.array, MAC_SSID_LEN, config->ssid) != EOK) {
        dbg("Invalid SSID\r\n");
        WifiUnlock();
        return ERROR_WIFI_UNKNOWN;
    }
    g_apData.vifApCfg.ssid.length = strlen(config->ssid);

    SetSecType(config->securityType, &g_apData.vifApCfg);

    if (SetChannel(config->channelNum, config->band, &g_apData.vifApCfg) != WIFI_SUCCESS) {
        dbg("set channel err chan = %d.band = %d\r\n", config->channelNum, config->band);
        WifiUnlock();
        return ERROR_WIFI_UNKNOWN;
    }

    if (strcpy_s(g_apData.vifApCfg.key, sizeof(g_apData.vifApCfg.key), config->preSharedKey) != EOK) {
        dbg("Invalid key\r\n");
        WifiUnlock();
        return ERROR_WIFI_UNKNOWN;
    }

    memcpy(&g_apData.hostpotCfg, config, sizeof(HotspotConfig));
    WifiUnlock();
    return WIFI_SUCCESS;
}

WifiErrorCode GetHotspotConfig(HotspotConfig *result)
{
    PARAM_CHECK(result);
    memcpy(result, &g_apData.hostpotCfg, sizeof(HotspotConfig));
    return WIFI_SUCCESS;
}

int IsHotspotActive(void)
{
    return g_apData.state;
}

WifiErrorCode GetStationList(StationInfo *result, unsigned int *size)
{
    uint8_t cnt = 0;
    WifiStationNode *pos = NULL;

    if (g_apData.state != WIFI_HOTSPOT_ACTIVE) {
        dbg("hotspot not enable\r\n");
        return ERROR_WIFI_UNKNOWN;
    }

    LOS_DL_LIST_FOR_EACH_ENTRY(pos, &g_apData.stationHead, WifiStationNode, node) {
        (void)memcpy_s(result, sizeof(StationInfo), &(pos->station), sizeof(StationInfo));
        cnt++;
    }

    *size = cnt;
    return WIFI_SUCCESS;
}

WifiErrorCode SetBand(int band)
{
    dbg("SetBand = %d\r\n", band);
    g_apData.hostpotCfg.band = band;
    if (band == HOTSPOT_BAND_TYPE_2G) {
        g_apData.vifApCfg.chan.band = PHY_BAND_2G4;
    } else if (band == HOTSPOT_BAND_TYPE_5G) {
        g_apData.vifApCfg.chan.band = PHY_BAND_5G;
    } else {
        dbg("Invalid band\r\n");
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

WifiErrorCode GetBand(int *result)
{
    PARAM_CHECK(result);
    *result = g_apData.hostpotCfg.band;
    if ((*result != HOTSPOT_BAND_TYPE_2G) && (*result != HOTSPOT_BAND_TYPE_5G)) {
        dbg("Invalid band = %d\r\n", g_apData.hostpotCfg.band);
        return ERROR_WIFI_UNKNOWN;
    }
    return WIFI_SUCCESS;
}

int GetSignalLevel(int rssi, int band)
{
    if (band == HOTSPOT_BAND_TYPE_2G) {
        if (rssi >= RSSI_LEVEL_4_2_G)
            return RSSI_LEVEL_4;
        if (rssi >= RSSI_LEVEL_3_2_G)
            return RSSI_LEVEL_3;
        if (rssi >= RSSI_LEVEL_2_2_G)
            return RSSI_LEVEL_2;
        if (rssi >= RSSI_LEVEL_1_2_G)
            return RSSI_LEVEL_1;
    }

    if (band == HOTSPOT_BAND_TYPE_5G) {
        if (rssi >= RSSI_LEVEL_4_5_G)
            return RSSI_LEVEL_4;
        if (rssi >= RSSI_LEVEL_3_5_G)
            return RSSI_LEVEL_3;
        if (rssi >= RSSI_LEVEL_2_5_G)
            return RSSI_LEVEL_2;
        if (rssi >= RSSI_LEVEL_1_5_G)
            return RSSI_LEVEL_1;
    }
    return ERROR_WIFI_INVALID_ARGS;
}

void HandleStaConnect(uint8_t *macAddr, ip4_addr_t clientIP)
{
    if (g_apData.state != WIFI_HOTSPOT_ACTIVE) {
        dbg("ap not enabled\r\n");
        return;
    }

    if (macAddr == NULL) {
        dbg("null check err\r\n");
        return;
    }
    if (WifiCreateLock() != WIFI_SUCCESS) {
        return;
    }

    if (g_apData.stationCnt > WIFI_MAX_STA_NUM) {
        dbg("station cnt over flow\r\n");
        WifiUnlock();
        return;
    }

    WifiStationNode *stationNode = (WifiStationNode *)rtos_calloc(1, sizeof(*stationNode));
    if (stationNode == NULL) {
        WifiUnlock();
        return;
    }
    memcpy(&stationNode->station.macAddress, macAddr, WIFI_MAC_LEN);
    stationNode->station.ipAddress = ntohl(ip4_addr_get_u32(&clientIP));

    LOS_ListTailInsert(&g_apData.stationHead, &stationNode->node);
    g_apData.stationCnt++;
    dbg("station join: mac->%02X:%02X:%02X:%02X:%02X:%02X ip-> %s,cnt= %d\r\n", macAddr[0], macAddr[1],
         macAddr[2], macAddr[3], macAddr[4], macAddr[5], inet_ntoa(clientIP), g_apData.stationCnt);
    DoStaJoinCallBack(&stationNode->station);
    WifiUnlock();
}

void HandleStaDisconnect(uint8_t *macAddr)
{
    if (g_apData.state != WIFI_HOTSPOT_ACTIVE) {
        dbg("ap not enabled\r\n");
        return;
    }

    if (macAddr == NULL) {
        dbg("null check err\r\n");
        return;
    }
    if (WifiCreateLock() != WIFI_SUCCESS) {
        return;
    }

    WifiStationNode *pos = NULL;

    LOS_DL_LIST_FOR_EACH_ENTRY(pos, &g_apData.stationHead, WifiStationNode, node) {
        if (memcmp(&pos->station.macAddress, macAddr, WIFI_MAC_LEN) == 0) {
            LOS_ListDelete(&pos->node);
            rtos_free(pos);
            g_apData.stationCnt--;
            DoStaLeaveCallBack(&pos->station);
            dbg("station leave: %02X:%02X:%02X:%02X:%02X:%02X -> %s,cnt= %d\r\n", macAddr[0],
                macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5], g_apData.stationCnt);
            break;
        }
    }
    WifiUnlock();
}

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
#include <stdlib.h>
#include "al_rtos.h"
#include "wifi_mac.h"
#include "wifi_host.h"
#include "wifi_host_wpa.h"
#include "wifi_host_cntrl.h"
#include "wifi_host_config.h"
#include "wifi_host_iperf.h"
#include "dbg_assert.h"
#include "rwnx_utils.h"
#include "wifi_nx_msg_tx.h"
#include "porting_net_al.h"
#include "console.h"
#include "wlan_if.h"
#include "command.h"
#include "wifi_host_tx.h"
#include "fhost_command_common.h"

/// Console task message queue size
#define FHOST_CONSOLE_QUEUE_SIZE    10

/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */
/// Master message queue
static rtos_queue console_queue;

/// Link to control task
static struct fhost_cntrl_link *console_cntrl_link;

/**
 ****************************************************************************************
 * @brief Convert string containing ip address
 *
 * The string may should be of the form a.b.c.d/e (/e being optional)
 *
 * @param[in]  str   String to parse
 * @parse[out] ip    Updated with the numerical value of the ip address
 * @parse[out] mask  Updated with the numerical value of the network mask
 *                   (or 32 if not present)
 * @return 0 if string contained what looks like a valid ip address and -1 otherwise
 ****************************************************************************************
 */
static int fhost_network_parse_ip4(char *str, uint32_t *ip, uint32_t *mask)
{
    char *token;
    uint32_t a, i, j;

    #define check_is_num(_str)  for (j = 0 ; j < strlen(_str); j++) {  \
            if (_str[j] < '0' || _str[j] > '9')                        \
                return -1;                                             \
        }

    // Check if mask is present
    token = strchr(str, '/');
    if (token && mask) {
        *token++ = '\0';
        check_is_num(token);
        a = atoi(token);
        if (a == 0 || a > 32)
            return -1;
        *mask = (1<<a) - 1;
    }
    else if (mask)
    {
        *mask = 0xffffffff;
    }

    // parse the ip part
    *ip = 0;
    for (i = 0; i < 4; i ++)
    {
        if (i < 3)
        {
            token = strchr(str, '.');
            if (!token)
                return -1;
            *token++ = '\0';
        }
        check_is_num(str);
        a = atoi(str);
        if (a > 255)
            return -1;
        str = token;
        *ip += (a << (i * 8));
    }

    return 0;
}

/**
 ****************************************************************************************
 * @brief Find the interface index
 *
 * @param[in] name Name of the interace
 * @return Index of the FHOST wifi interface or -1 if interface doesn't exist.
 ****************************************************************************************
 */
static int fhost_console_search_itf(char *name)
{
    int fhost_vif_idx = fhost_vif_idx_from_name(name);

    if (fhost_vif_idx >= 0)
        return fhost_vif_idx;

    dbg("Cannot find interface %s\n", name);
    return -1;
}

#if NX_FHOST_MONITOR
/**
 ****************************************************************************************
 * @brief callback function
 *
 * Extract received packet informations (frame length, type, mac addr ...) in monitor mode
 *
 * @param[in] info  RX Frame information.
 * @param[in] arg   Not used
 ****************************************************************************************
 */
static void fhost_console_monitor_cb(struct fhost_frame_info *info, void *arg)
{
    if (info->payload == NULL) {
        dbg("Unsupported frame: length = %d\r\n", info->length);
    } else {
        struct mac_hdr *hdr = (struct mac_hdr *)info->payload;
        uint8_t *adr1 = ((uint8_t *)(hdr->addr1.array));
        uint8_t *adr2 = ((uint8_t *)(hdr->addr2.array));
        uint8_t *adr3 = ((uint8_t *)(hdr->addr3.array));
        dbg("a1=%02x:%02x:%02x:%02x:%02x:%02x a2=%02x:%02x:%02x:%02x:%02x:%02x "
            "a3=%02x:%02x:%02x:%02x:%02x:%02x fc=%04X SN:%d len=%d\n",
            adr1[0], adr1[1], adr1[2], adr1[3], adr1[4], adr1[5],
            adr2[0], adr2[1], adr2[2], adr2[3], adr2[4], adr2[5],
            adr3[0], adr3[1], adr3[2], adr3[3], adr3[4], adr3[5],
            hdr->fctl, hdr->seq >> 4, info->length);
    }
}
#endif /* NX_FHOST_MONITOR */

#if PLF_IPERF
static int do_iperf (int argc, char *argv[])
{
    char iperf_params[128] = {0};
    unsigned int idx = 0, j = 0;

    if ((argc < 1)) {
        dbg ("Usage:\n  iperf <-s|-c|-h> [options]\n");
        return 1;
    }

    if (!netif_is_up(net_if_find_from_wifi_idx(fhost_vif_idx)))  {
        dbg ("Usage:\n Connect AP first\n");
        return 1;
    }

    memcpy(&(iperf_params[idx]), argv[1], strlen(argv[1]));
    idx += strlen(argv[1]);

    if(strstr(iperf_params, "stop")) {
        fhost_iperf_sigkill_handler(iperf_task_handle);
        return 0;
    }

    j = 3;
    while(j <= argc) {
        iperf_params[idx] = ' ';
        idx ++;
        memcpy(&(iperf_params[idx]), argv[j - 1], strlen(argv[j - 1]));
        idx += strlen(argv[j - 1]);
        j++;
    }

    fhost_console_iperf(iperf_params);

    return 0;
}
#endif /* PLF_IPERF */
#if PLF_PING
static int do_ping (int argc, char *argv[])
{
    char ping_params[64] = {0};
    unsigned int idx = 0, j = 0;
    if ((argc < 2)) {
        dbg ("Usage:\n  ping dst_ip|stop ID\n");
        return 1;
    }

    if (!netif_is_up(net_if_find_from_wifi_idx(fhost_vif_idx)))  {
        dbg ("Usage:\n Connect first\n");
        return 1;
    }

    memcpy(&(ping_params[idx]), argv[1], strlen(argv[1]));
    idx += strlen(argv[1]);
    j = 3;
    while(j <= argc) {
        ping_params[idx] = ' ';
        idx ++;
        memcpy(&(ping_params[idx]), argv[j - 1], strlen(argv[j - 1]));
        idx += strlen(argv[j - 1]);
        j++;
    }

    fhost_console_ping(ping_params);

    return 0;
}
#endif /* PLF_PING */


/**
 ****************************************************************************************
 * @brief Process function for 'scan' command
 *
 * @param[in] params Not used
 * @return 0 on success and !=0 if error occurred
 ****************************************************************************************
 */
static int do_scan(int argc, char *argv[])
{
    int nb_res;
    struct mac_scan_result result;
    unsigned int fvif_idx;

    if (argc < 2) {
        return -1;
    }

    fvif_idx = console_cmd_strtoul(argv[1], NULL, 10);
    if (fvif_idx >= NX_VIRT_DEV_MAX) {
        dbg("invalid fvif index\n");
        return -2;
    }
    ipc_host_cntrl_start();

    console_cntrl_link = fhost_cntrl_cfgrwnx_link_open();
    if (console_cntrl_link == NULL) {
        dbg(D_ERR "Failed to open link with control task\n");
        ASSERT_ERR(0);
    }
    // Reset STA interface (this will end previous wpa_supplicant task)
    if (fhost_set_vif_type(console_cntrl_link, fvif_idx, VIF_UNKNOWN, false) ||
        fhost_set_vif_type(console_cntrl_link, fvif_idx, VIF_STA, false)) {

        fhost_cntrl_cfgrwnx_link_close(console_cntrl_link);
        return -1;
   }

    nb_res = fhost_scan(console_cntrl_link, fvif_idx, NULL);
    dbg("Got %d scan results\n", nb_res);

    nb_res = 0;
    while(fhost_get_scan_results(console_cntrl_link, nb_res++, 1, &result)) {
        result.ssid.array[result.ssid.length] = '\0'; // set ssid string ending
        dbg("(%3d dBm) CH=%3d BSSID=%02x:%02x:%02x:%02x:%02x:%02x SSID=%s,group_cipher=%d,pairwise_cipher=%d\n",
            (int8_t)result.rssi, phy_freq_to_channel(result.chan->band, result.chan->freq),
            ((uint8_t *)result.bssid.array)[0], ((uint8_t *)result.bssid.array)[1],
            ((uint8_t *)result.bssid.array)[2], ((uint8_t *)result.bssid.array)[3],
            ((uint8_t *)result.bssid.array)[4], ((uint8_t *)result.bssid.array)[5],
            (char *)result.ssid.array,result.group_cipher,result.pairwise_cipher);
    }

    fhost_cntrl_cfgrwnx_link_close(console_cntrl_link);
    return 0;
}

/**
 ****************************************************************************************
 * @brief Process function for 'status' command
 *
 * @param[in] params Optional command parameter
 *                   Valid parameter: "chan", "vif"
 *
 * @return FHOST_IPC_SUCCESS on success and FHOST_IPC_ERROR if error occurred
 ****************************************************************************************
 */
static int do_status(int argc, char *argv[])
{
    struct fhost_status status;
    bool show_vif = true, show_chan = true;
    int i;

    if (argc > 1) {
        if (!strncmp("vif", argv[1], 3))
            show_chan = false;
        else if (!strncmp("chan", argv[1], 4))
            show_vif = false;
        else {
            dbg("invalid subcmd\n");
            return -1;
        }
    }

    fhost_get_status(&status);

    if (show_chan) {
        const struct mac_chan_def *chan_def;
        dbg("Available Channels:\n");

        for (chan_def = &(status.chan_2g4[0]), i = 0;
             i < (status.chan_2g4_cnt + status.chan_5g_cnt);
             chan_def++, i++) {
            if (i == status.chan_2g4_cnt)
                chan_def = &(status.chan_5g[0]);

            dbg("ch: %3d, freq: %dMhz, TX pwr: %2d dBm, flags: %s%s\n",
                phy_freq_to_channel(chan_def->band, chan_def->freq),
                chan_def->freq, chan_def->tx_power,
                chan_def->flags & CHAN_NO_IR ? "[NO_IR]" : "",
                chan_def->flags & CHAN_DISABLED ? "[DISABLED]" : "");
        }
        dbg("\n");
    }

    if (show_vif) {
        for (i = 0; i < status.vif_max_cnt; i++) {
            struct fhost_vif_status vif_status;
            char vif_name[4];
            fhost_get_vif_status(i, &vif_status);
            memset(vif_name, 0, sizeof(vif_name));
            fhost_vif_name(i, vif_name, sizeof(vif_name) - 1);
            dbg("[%d] %s: MAC=%02x:%02x:%02x:%02x:%02x:%02x ",
                i, vif_name[0] ? vif_name : "nul",
                vif_status.mac_addr[0], vif_status.mac_addr[1],
                vif_status.mac_addr[2], vif_status.mac_addr[3],
                vif_status.mac_addr[4], vif_status.mac_addr[5]);
            switch (vif_status.type) {
                case VIF_STA:
                    dbg("STA");
                    break;
                case VIF_IBSS:
                    dbg("IBSS");
                    break;
                case VIF_AP:
                    dbg("AP");
                    break;
                case VIF_MESH_POINT:
                    dbg("MESH");
                    break;
                case VIF_MONITOR:
                    dbg("MONITOR");
                    break;
                default:
                    dbg("INACTIVE\n");
                    break;
            }
            if (vif_status.type != VIF_UNKNOWN) {
                if (vif_status.chan.prim20_freq != 0) {
                    dbg(", Operating Channel: %dMHz@%dMHz\n",
                                vif_status.chan.prim20_freq,
                                (1 << vif_status.chan.type) * 20);
                } else {
                    dbg(", No Operating Channel\n");
                }
            }
        }
    }

    return 0;
}

/**
 ****************************************************************************************
 * @brief Process function for 'ip' command
 *
 * ip command can be used for.
 * - address managing:
   @verbatim
      ip show [itf-name]
      ip add <ip>/<mask> [gw] <itf-name>
      ip del <itf-name>
   @endverbatim
 *
 * @param[in] params Port number
 * @return 0 on success and !=0 if error occurred
 ****************************************************************************************
 */
static int do_ip(int argc, char *argv[])
{
    int fhost_vif_idx = -1;
    if (argc < 2) {
        dbg("wrong # of args\n");
        return -1;
    }
    if (!strcmp("show", argv[1])) {
        int i = 0;
        int i_end = NX_VIRT_DEV_MAX;
        if (argc > 2) {
            fhost_vif_idx = fhost_console_search_itf(argv[2]);
            if (fhost_vif_idx < 0) {
                dbg("invalid itf\r\n");
                return -2;
            }
            i = fhost_vif_idx;
            i_end = fhost_vif_idx + 1;
        }
        for (; i < i_end; i++) {
            char ifname[NET_AL_MAX_IFNAME];
            char *state, *connected;
            struct fhost_vif_ip_addr_cfg ip_cfg;
            uint32_t ip, gw;
            const uint8_t *mac_addr;

            if (fhost_env.vif[i].mac_vif &&
                (fhost_env.vif[i].mac_vif->type != VIF_UNKNOWN)) {
                state = "UP";
                if (fhost_env.vif[i].mac_vif->active)
                    connected = ",CONNECTED";
                else
                    connected = "";
            } else {
                state = "DOWN";
                connected = "";
            }
            mac_addr = net_if_get_mac_addr(fhost_to_net_if(i));

            if (fhost_vif_name(i, ifname, NET_AL_MAX_IFNAME) < 0)
                return -3;

            if (fhost_get_vif_ip(i, &ip_cfg))
                return -4;

            ip = ip_cfg.ipv4.addr;
            gw = ip_cfg.ipv4.gw;
            dbg("[%d] %s: MAC=%02x:%02x:%02x:%02x:%02x:%02x ip=%d.%d.%d.%d/%d gw=%d.%d.%d.%d %s%s\n", i, ifname[0] ? ifname : "nul",
                mac_addr[0], mac_addr[1], mac_addr[2],
                mac_addr[3], mac_addr[4], mac_addr[5],
                ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, (ip >> 24) & 0xff,
                32 - co_clz(ip_cfg.ipv4.mask),
                gw & 0xff, (gw >> 8) & 0xff, (gw >> 16) & 0xff, (gw >> 24) & 0xff,
                state, connected);
        }
    } else if (!strcmp("add", argv[1])) {
        struct fhost_vif_ip_addr_cfg ip_cfg;
        char *ip_str, *gw_str = NULL, *itf = NULL;

        memset(&ip_cfg, 0, sizeof(ip_cfg));
        ip_str = argv[2];
        if (argc == 4) {
            itf = argv[3];
        } else if (argc > 4) {
            gw_str = argv[3];
            itf = argv[4];
        }

        fhost_vif_idx = fhost_console_search_itf(itf);
        if (fhost_vif_idx < 0) {
            return -5;
        }

        ip_cfg.mode = IP_ADDR_STATIC_IPV4;
        if (fhost_network_parse_ip4(ip_str, &ip_cfg.ipv4.addr, &ip_cfg.ipv4.mask)) {
            return -6;
        }
        if (gw_str && fhost_network_parse_ip4(gw_str, &ip_cfg.ipv4.gw, NULL)) {
            return -7;
        }
        if (fhost_set_vif_ip(fhost_vif_idx, &ip_cfg))
            return -8;
        dbg("add ip=%s gw=%s itf=%s\r\n",ip_str, gw_str ? gw_str : "null", itf);
    } else if (!strcmp("del", argv[1])) {
        struct fhost_vif_ip_addr_cfg ip_cfg;
        char *itf = argv[2];
        fhost_vif_idx = fhost_console_search_itf(itf);
        if (fhost_vif_idx < 0)
            return -9;

        ip_cfg.mode = IP_ADDR_NONE;
        fhost_set_vif_ip(fhost_vif_idx, &ip_cfg);
        dbg("del itf=%s\r\n", itf);
    } else {
        dbg("invalid subcmd\n");
    }
    return 0;
}

static void fhost_nw_upper(char *str, char *stop)
{
    char *ptr = str;
    char c;

    if (stop)
    {
        c = *stop;
        *stop = 0;
    }
    while (*ptr)
    {
        if ((*ptr >= 'a') && (*ptr <= 'z'))
            *ptr -= 'a' - 'A';
        ptr++;
    }

    if (stop)
        *stop = c;
}
static struct fhost_cntrl_link *sta_link;

#if NX_BEACONING
static struct fhost_cntrl_link *ap_link;
#include "dhcps.h"
#endif
/**
 ****************************************************************************************
 * @brief Process function for 'ap' command
 *
 * Start an AP
 * @verbatim
   ap [-i <itf>] -s <SSID> -f <freq>[+-@] [-a <akm>[,<akm 2>]] [-k <key>]
      [-b bcn_int[,dtim_period]] [-u <unicast cipher>[,<unicast cipher 2>]]
      [-g <group cipher>] [-m <mfp: 0|1|2>]
   @endverbatim
 * The '+/-' after the frequency allow to configure a 40MHz channel with the secondary
 * channel being the upper/lower one. The '@' allow to configure a 80 MHz channel, this
 * is only allowed for valid primary channel and center freq is automatically computed.
 *
 * @param[in] params  Connection parameters
 * @return 0 on success and !=0 if error occurred
 ****************************************************************************************
 */
int do_ap (int argc, char *argv[])
{
#if NX_BEACONING
    char ap_params[256] = {0};
    unsigned int idx = 0, j = 0;
    struct fhost_vif_ap_cfg cfg;
    int fhost_vif_idx = 0;//fhost_search_first_valid_itf();

    if ((argc < 1)) {
        dbg("Usage:\n  ap \n");
        return ERR_WRONG_ARGS;
    }

    j = 2;
    while(j <= argc) {
        memcpy(&(ap_params[idx]), argv[j - 1], strlen(argv[j - 1]));
        idx += strlen(argv[j - 1]);
        ap_params[idx] = ' ';
        idx ++;
        j++;
    }

    char *token, *next = ap_params;
    memset(&cfg, 0, sizeof(cfg));

    while ((token = fhost_nw_next_token(&next)))
    {
        char option;

        if ((token[0] != '-') | (token[2] != '\0'))
            return ERR_WRONG_ARGS;

        option = token[1];
        token = fhost_nw_next_token(&next);
        if (!token)
            return ERR_WRONG_ARGS;

        switch(option)
        {
            #if 0
            case 'i':
            {
                fhost_vif_idx = fhost_search_itf(token);
                if (fhost_vif_idx < 0)
                    return ERR_CMD_FAIL;
                break;
            }
            #endif
            case 's':
            {
                size_t ssid_len = strlen(token);
                if (ssid_len > sizeof(cfg.ssid.array))
                {
                    dbg("Invalid SSID\r\n");
                    return ERR_CMD_FAIL;
                }

                memcpy(cfg.ssid.array, token, ssid_len);
                cfg.ssid.length = ssid_len;
                break;
            }
            case 'k':
            {
                size_t key_len = strlen(token);
                if ((key_len + 1) > sizeof(cfg.key))
                {
                    dbg("Invalid Key\r\n");
                    return ERR_CMD_FAIL;
                }
                strcpy(cfg.key, token);
                break;
            }
            case 'f':
            {
                int len = strlen(token) - 1;
                struct mac_chan_def *chan = NULL;
                int offset = 0;
                if (token[len] == '+')
                {
                    token[len] = 0;
                    offset = 10;
                    cfg.chan.type = PHY_CHNL_BW_40;
                }
                else if (token[len] == '-')
                {
                    token[len] = 0;
                    offset = -10;
                    cfg.chan.type = PHY_CHNL_BW_40;
                }
                else if (token[len] == '@')
                {
                    token[len] = 0;
                    cfg.chan.type = PHY_CHNL_BW_80;
                }
                else
                {
                    cfg.chan.type = PHY_CHNL_BW_20;
                }

                cfg.chan.prim20_freq = atoi(token);
                chan = fhost_chan_get(cfg.chan.prim20_freq);
                if (!chan)
                {
                    dbg("Invalid channel\n");
                    return ERR_CMD_FAIL;
                }

                if (cfg.chan.prim20_freq >= PHY_FREQ_5G)
                    cfg.chan.band = PHY_BAND_5G;
                else
                    cfg.chan.band = PHY_BAND_2G4;

                if (cfg.chan.type == PHY_CHNL_BW_80)
                {
                    if ((cfg.chan.prim20_freq < 5180) ||
                        (cfg.chan.prim20_freq > 5805))
                    {
                        dbg("Invalid primary for 80MHz channel\n");
                        return ERR_CMD_FAIL;
                    }
                    offset = (cfg.chan.prim20_freq - 5180) % 80;
                    if (offset < 20)
                        offset = 30;
                    else if (offset < 40)
                        offset = 10;
                    else if (offset < 60)
                        offset = -10;
                    else
                        offset = -30;
                }
                cfg.chan.center1_freq = cfg.chan.prim20_freq + offset;
                break;
            }
            case 'a':
            {
                char *next_akm;
                fhost_nw_upper(token, NULL);
                next_akm = strchr(token, ',');
                while (token)
                {
                    if (strncmp(token, "OPEN", 4) == 0)
                    {
                        cfg.akm |= CO_BIT(MAC_AKM_NONE);
                    }
                    else if (strncmp(token, "WEP", 4) == 0)
                    {
                        cfg.akm |= CO_BIT(MAC_AKM_PRE_RSN);
                    }
                    else if (strncmp(token, "WPA", 3) == 0)
                    {
                        cfg.akm |= CO_BIT(MAC_AKM_PRE_RSN) | CO_BIT(MAC_AKM_PSK);
                    }
                    else if (strncmp(token, "RSN", 3) == 0)
                    {
                        cfg.akm |= CO_BIT(MAC_AKM_PSK);
                    }
                    else if (strncmp(token, "SAE", 3) == 0)
                    {
                        cfg.akm |= CO_BIT(MAC_AKM_SAE);
                    }
                    else
                    {
                        dbg("The following AKM are supported [%s]:\n"
                                    "OPEN: For open AP\n"
                                    "WEP: For AP with WEP security\n"
                                    "WPA: For AP with WPA/PSK security (pre WPA2)\n"
                                    "RSN: For AP with WPA2/PSK security\n"
                                    "SAE: For AP with WPA3/PSK security\n", token);
                        if (strncmp(token, "HELP", 4) == 0)
                            return ERR_NONE;
                        else
                            return ERR_CMD_FAIL;
                    }

                    token = next_akm;
                    if (token)
                    {
                        token++;
                        next_akm = strchr(token, ',');
                    }
                }
                break;
            }
            case 'u':
            case 'g':
            {
                char *next_cipher;
                uint32_t cipher = 0;

                fhost_nw_upper(token, NULL);
                next_cipher = strchr(token, ',');
                while (token)
                {
                    if (strncmp(token, "CCMP", 4) == 0)
                    {
                        cipher |= CO_BIT(MAC_CIPHER_CCMP);
                    }
                    else if (strncmp(token, "TKIP", 4) == 0)
                    {
                        cipher |= CO_BIT(MAC_CIPHER_TKIP);
                    }
                    else if (strncmp(token, "WEP40", 5) == 0)
                    {
                        cipher |= CO_BIT(MAC_CIPHER_WEP40);
                    }
                    else if (strncmp(token, "WEP104", 6) == 0)
                    {
                        cipher |= CO_BIT(MAC_CIPHER_WEP104);
                    }
                    else if (strncmp(token, "SMS4", 4) == 0)
                    {
                        cipher |= CO_BIT(MAC_CIPHER_WPI_SMS4);
                    }
                    else
                    {
                        dbg("The following cipher are supported [%s]:\n"
                                    "CCMP, TKIP, WEP40, WEP104, SMS4", token);
                        if (strncmp(token, "HELP", 4) == 0)
                            return ERR_NONE;
                        else
                            return ERR_CMD_FAIL;
                    }

                    token = next_cipher;
                    if (token)
                    {
                        token++;
                        next_cipher = strchr(token, ',');
                    }
                }

                if (option == 'u')
                    cfg.unicast_cipher = cipher;
                else
                    cfg.group_cipher = cipher;

                break;
            }
            case 'b':
            {
                char *dtim = strchr(token, ',');
                if (dtim)
                {
                    *dtim++ = 0;
                    cfg.dtim_period = atoi(dtim);
                }
                cfg.bcn_interval = atoi(token);

                break;
            }
            case 'm':
            {
                cfg.mfp = atoi(token);
                break;
            }
            case 'h':
            {
                cfg.hidden_ssid = atoi(token);
                break;
            }
            default:
            {
                dbg("Invalid option %c\n", option);
                return ERR_WRONG_ARGS;
            }
        }
    }

    if (fhost_vif_idx < 0)
        return ERR_CMD_FAIL;

    if ((cfg.ssid.length == 0) || (cfg.chan.prim20_freq == 0))
        return ERR_WRONG_ARGS;

    // try to select the best AKM if not set
    if (cfg.akm == 0)
    {
        if (strlen(cfg.key) == 0)
            cfg.akm = CO_BIT(MAC_AKM_NONE);
        else if (strlen(cfg.key) == 5)
            cfg.akm = CO_BIT(MAC_AKM_PRE_RSN);
        else
            cfg.akm = CO_BIT(MAC_AKM_PSK);
    }
    ipc_host_cntrl_start();

    struct fhost_vif_tag *fhost_vif;

    ap_link = fhost_cntrl_cfgrwnx_link_open();
    if (ap_link == NULL) {
        dbg(D_ERR "Failed to open link with control task\n");
        ASSERT_ERR(0);
    }

    // (Re)Set interface type to AP
    if (fhost_set_vif_type(ap_link, fhost_vif_idx, VIF_UNKNOWN, false) ||
        fhost_set_vif_type(ap_link, fhost_vif_idx, VIF_AP, false))
        return ERR_CMD_FAIL;

    fhost_cntrl_cfgrwnx_link_close(ap_link);

    fhost_vif = &fhost_env.vif[fhost_vif_idx];
    MAC_ADDR_CPY(&(vif_info_tab[fhost_vif_idx].mac_addr), &(fhost_vif->mac_addr));

    if (fhost_ap_cfg(fhost_vif_idx, &cfg))
    {
        dbg("Failed to start AP, check your configuration");
        return ERR_CMD_FAIL;
    }

    net_if_t *net_if = fhost_to_net_if(fhost_vif_idx);
    if (net_if == NULL) {
        dbg("[CS] net_if_find_from_wifi_idx fail\r\n");
        return 1;
    }
    uint32_t ip_mask = 0x00FFFFFF;
    uint32_t ip_addr = get_ap_ip_addr();
    net_if_set_ip(net_if, ip_addr, ip_mask, 0);

    //set up DHCP server
    dhcpServerStart(net_if);

    // Now that we got an IP address use this interface as default
    net_if_set_default(net_if);

    fhost_tx_task_init();
    dbg("DHCPS init: ip=%d.%d.%d.%d\r\n",
          (ip_addr)&0xFF, (ip_addr>>8)&0xFF, (ip_addr>>16)&0xFF, (ip_addr>>24)&0xFF);

#endif // NX_BEACONING


    return ERR_NONE;
}

/**
 ****************************************************************************************
 * @brief Convert string containing MAC address
 *
 * The string may should be of the form xx:xx:xx:xx:xx:xx
 *
 * @param[in]  str   String to parse
 * @param[out] addr  Updated with MAC address
 * @return 0 if string contained what looks like a valid MAC address and -1 otherwise
 ****************************************************************************************
 */
static int fhost_nw_parse_mac_addr(char *str, struct mac_addr *addr)
{
    char *ptr = str;
    uint32_t i;

    if (!str || strlen(str) < 17 || !addr)
        return -1;

    for (i = 0 ; i < 6 ; i++)
    {
        char *next;
        long int hex = strtol(ptr, &next, 16);
        if (((unsigned)hex > 255) || ((hex == 0) && (next == ptr)) ||
            ((i < 5) && (*next != ':')) ||
            ((i == 5) && (*next != '\0')))
            return -1;

        ((uint8_t *)addr)[i] = (uint8_t)hex;
        ptr = ++next;
    }

    return 0;
}

int do_sta (int argc, char *argv[])
{
    char sta_params[256] = {0};
    unsigned int idx = 0, j = 0;
    int fhost_vif_idx = 0;//fhost_search_first_valid_itf();
    struct fhost_vif_sta_cfg cfg;
    net_if_t *net_if = NULL;

    if ((argc < 1)) {
        dbg("Usage:\n  ap \n");
        return ERR_WRONG_ARGS;
    }

    j = 2;
    while(j <= argc) {
        memcpy(&(sta_params[idx]), argv[j - 1], strlen(argv[j - 1]));
        idx += strlen(argv[j - 1]);
        sta_params[idx] = ' ';
        idx ++;
        j++;
    }
    char *token, *next = sta_params;

    memset(&cfg, 0, sizeof(cfg));

    cfg.timeout_ms = 30000;
    while ((token = fhost_nw_next_token(&next)))
    {
        char option;

        if ((token[0] != '-') | (token[2] != '\0'))
            return ERR_WRONG_ARGS;

        option = token[1];
        token = fhost_nw_next_token(&next);
        if (!token)
            return ERR_WRONG_ARGS;

        switch(option)
        {
            #if 0
            case 'i':
            {
                cfg.fhost_vif_idx = fhost_search_itf(token);
                if (cfg.fhost_vif_idx < 0)
                    return FHOST_IPC_ERROR;
                break;
            }
            #endif
            case 's':
            {
                size_t ssid_len = strlen(token);
                if (ssid_len > sizeof(cfg.ssid.array))
                {
                    dbg("Invalid SSID\r\n");
                    return ERR_CMD_FAIL;
                }

                memcpy(cfg.ssid.array, token, ssid_len);
                cfg.ssid.length = ssid_len;

                break;
            }
            case 'b':
            {
                if (fhost_nw_parse_mac_addr(token, &cfg.bssid))
                {
                    dbg("Invalid BSSID\r\n");
                    return ERR_CMD_FAIL;
                }
                break;
            }
            case 'k':
            {
                size_t key_len = strlen(token);
                if ((key_len + 1) > sizeof(cfg.key))
                {
                    dbg("Invalid Key\r\n");
                    return ERR_CMD_FAIL;
                }
                strcpy(cfg.key, token);
                break;
            }
            case 'f':
            {
                unsigned int i;
                char *next_freq = strchr(token, ',');
                for (i = 0 ; i <  CO_ARRAY_SIZE(cfg.freq); i++)
                {
                    cfg.freq[i] = atoi(token);
                    if (!next_freq)
                        break;
                    *next_freq++ = '\0';
                    token = next_freq;
                    next_freq = strchr(token, ',');
                }
                break;
            }
            case 'a':
            {
                char *next_akm;
                fhost_nw_upper(token, NULL);
                next_akm = strchr(token, ',');
                while (token)
                {
                    if (strncmp(token, "OPEN", 4) == 0)
                    {
                        cfg.akm |= CO_BIT(MAC_AKM_NONE);
                    }
                    else if (strncmp(token, "WEP", 4) == 0)
                    {
                        cfg.akm |= CO_BIT(MAC_AKM_NONE);
                    }
                    else if (strncmp(token, "PSK", 3) == 0)
                    {
                        cfg.akm |= CO_BIT(MAC_AKM_PSK);
                    }
                    else if (strncmp(token, "SAE", 3) == 0)
                    {
                        cfg.akm |= CO_BIT(MAC_AKM_SAE);
                    }
                    else
                    {
                        dbg("The following AKM are supported [%s]:\n"
                                    "OPEN: For open AP\n"
                                    "WEP: For AP configured with WEP\n"
                                    "PSK: For AP configured with a password and WPA/WPA2\n"
                                    "SAE: For AP configured with a password and WPA3\n", token);
                        if (strncmp(token, "HELP", 4) == 0)
                            return ERR_NONE;
                        else
                            return ERR_CMD_FAIL;
                    }

                    token = next_akm;
                    if (token)
                    {
                        token++;
                        next_akm = strchr(token, ',');
                    }
                }
                break;
            }
            case 't':
            {
                cfg.timeout_ms = atoi(token);
                break;
            }
            default:
                return ERR_WRONG_ARGS;
        }
    }

    if (cfg.ssid.length == 0)
        return ERR_WRONG_ARGS;
    ipc_host_cntrl_start();

    sta_link = fhost_cntrl_cfgrwnx_link_open();
    if (sta_link == NULL) {
        dbg(D_ERR "Failed to open link with control task\n");
        ASSERT_ERR(0);
    }

    // Reset STA interface (this will end previous wpa_supplicant task)
    if (fhost_set_vif_type(sta_link, fhost_vif_idx, VIF_UNKNOWN, false) ||
        fhost_set_vif_type(sta_link, fhost_vif_idx, VIF_STA, false))
        return ERR_CMD_FAIL;
    fhost_cntrl_cfgrwnx_link_close(sta_link);

    if (fhost_sta_cfg(fhost_vif_idx, &cfg))
        return ERR_CMD_FAIL;

    // Get the first network interface (created by CNTRL task).
    net_if = net_if_find_from_wifi_idx(fhost_vif_idx);
    if (net_if == NULL) {
        dbg("[CS] net_if_find_from_wifi_idx fail\r\n");
        return 1;
    }

    // Start DHCP client to retrieve ip address
    if (wlan_dhcp(net_if)) {
        wlan_disconnect_sta((uint8_t)fhost_vif_idx);
        dbg("[CS] dhcp fail\r\n");
        return 3;
    }

    // Now that we got an IP address use this interface as default
    net_if_set_default(net_if);

    return ERR_NONE;
}

int do_set_deepsleep_param (int argc, char *argv[])
{
    unsigned int listen_interval = console_cmd_strtoul(argv[1], NULL, 10);

    set_deepsleep_param(listen_interval, 1);

    return ERR_NONE;
}

#ifdef CFG_PRERELEASE_CODE
#include "fhost_console_prerelease.c"
#endif

static void fhost_command_add(void)
{
    RWNX_DBG(RWNX_FN_ENTRY_STR);

    #if NX_BEACONING
    console_cmd_add("startap", "  startap band ssid <pwd>", 4, do_start_ap);
    console_cmd_add("stopap", "  stopap", 1, do_stop_ap);
    console_cmd_add("runap", "  runap -s <SSID> -f <freq>[+-@] [-a <akm>[,<akm 2>]] [-k <key>] "
                         "[-b bcn_int[,dtim_period]] [-m <mfp: 0|1|2>] "
                         "[-u <unicast cipher>[,<unicast cipher 2>]] [-g <group cipher>] [-m <mfp: 0|1|2>] [-h hidden]", 20, do_ap);
    #endif /* NX_BEACONING */
    console_cmd_add("runsta", "  runsta -s <SSID> [-k <key>] [-b <bssid>] "
                        "[-f <freq>[,freq]] [-a <akm>] [-t <timeout>]", 20, do_sta);
    console_cmd_add("connect_wep", "  connect_wep 0/1 ssid <pwd>", 4, do_connect_wep);
    console_cmd_add("connect", "  connect 0/1 ssid <pwd>", 4, do_connect);
    console_cmd_add("mac",     "  mac ?/[hex_str]",    2, do_mac);
    console_cmd_add("setdsparam", "  setdsparam listen_interval", 4, do_set_deepsleep_param);
    console_cmd_add("clrfi",   "  clr flash info",  1, do_restore);
    #if NX_WPS
    console_cmd_add("wps", "  wps", 1, do_wps_pbc);
    #endif /* NX_WPS */
    console_cmd_add("disconnect", "  disconnect", 1, do_disconnect);
    #if PLF_PING
    console_cmd_add("ping", "  ping dst_ip|stop ID", 20, do_ping);
    #endif /* PLF_PING */
    #if PLF_IPERF
    console_cmd_add("iperf", "  iperf <-s|-c|-h> [options]", 20, do_iperf);
    #endif /* PLF_IPERF */

    console_cmd_add("scan", "  scan fvif_idx", 2, do_scan);
    console_cmd_add("status", "  status [chan|vif]", 2, do_status);
    console_cmd_add("ip",   "  ip show [itf]\n"
                            "  ip add <ip>/<mask> [gw] <itf>\n"
                            "  ip del <itf>", 5, do_ip);
    #if NX_SMARTCONFIG
    console_cmd_add("smartconf",  "  smartconf", 1, do_smartconf);
    console_cmd_add("stopsc",  "  stopsc", 1, do_stop_smartconf);
    #endif /* NX_SMARTCONFIG */
    #ifdef CFG_PRERELEASE_CODE
    fhost_prerelease_command_add();
    #endif
}

/**
 ****************************************************************************************
 * @brief Request the RTOS to resume the console task.
 * This function sends a msg to console_queue to realize the resume.
 * Note that currently this function is supposed to be called from interrupt.
 *
 * @param[in] isr Indicates if called from ISR
 ****************************************************************************************
 */
static void fhost_console_resume(bool isr)
{
    int res;
    struct fhost_msg msg;

    msg.id   = FHOST_MSG_ID(FHOST_MSG_CONSOLE, 0);
    msg.len  = 0;
    msg.data = NULL;

    res = rtos_queue_write(console_queue, &msg, 0, isr);
    ASSERT_ERR(res == 0);
}

/**
 ****************************************************************************************
 * @brief console task implementation.
 ****************************************************************************************
 */
static RTOS_TASK_FCT(fhost_console_task)
{
    struct fhost_msg msg;

    if (wlan_connected) {
        fhost_sta_recover_connection();
    }

    for (;;) {
        rtos_queue_read(console_queue, &msg, -1, false);

        switch (FHOST_MSG_TYPE(msg.id)) {
        case FHOST_MSG_CONSOLE:
            console_schedule();
            break;
        default:
            break;
        }
    }
}

#if CFG_APP_CONSOLEWIFI
int fhost_application_init(void)
{
    RWNX_DBG(RWNX_FN_ENTRY_STR);

    // Initialize console
    console_init();

    // Create the console task
    if (rtos_task_create(fhost_console_task, "CONSOLE", APPLICATION_TASK,
                         2048, NULL, RTOS_TASK_PRIORITY(1), NULL)) {
        return 1;
    }

    if (rtos_queue_create(sizeof(struct fhost_msg), FHOST_CONSOLE_QUEUE_SIZE, &console_queue))
        return 2;

    console_ntf_register(fhost_console_resume);

    fhost_command_add();

    return 0;
}
#else
// If NOT define CFG_APP_CONSOLEWIFI, define a stub function.
int fhost_application_init(void)
{
    return 0;
}
#endif

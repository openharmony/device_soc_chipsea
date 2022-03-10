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
#ifndef _WLAN_IF_H_
#define _WLAN_IF_H_

#include "mac.h"

 /**
 ****************************************************************************************
 * @brief * wlan_start_sta : Start STA to connect to router, until obtain IP address 
 *                           or timeout
 *
 * @param[]  ssid: router name
 *           pw  : router's password(if open, set NULL)
 *           timeout_ms: 0 -> 10000, -1 -> no timeout, (timeout_ms > 0 && bit0 is 1) -> wep
 * @return value: -1:password < 8, -6: dhcp fail
 ****************************************************************************************
 */
int wlan_start_sta(uint8_t *ssid, uint8_t *pw, int timeout_ms);
/**
****************************************************************************************
* @brief * wlan_disconnect_sta : STA disconnect
*
* @param[]  idx: 0
* @return value: 1: fail, 0: success
****************************************************************************************
*/
int wlan_disconnect_sta(uint8_t idx);
/**
****************************************************************************************
* @brief * wlan_dhcp : run dhcp client, if using 'wlan_start_sta', no need to call it
*
* @param[]  net_if: 
* @return value: 0: success
****************************************************************************************
*/
int wlan_dhcp(net_if_t *net_if);
/**
****************************************************************************************
* @brief * wlan_get_connect_status : get STA connect status
*
* @param[]  none
* @return value: 0: disconnected, 1: connected
****************************************************************************************
*/
int wlan_get_connect_status(void);
/**
****************************************************************************************
* @brief * set_sta_connect_chan_num : Set channel num before calling 'wlan_start_sta', 
*                                     if kowns the router's channel num
*
* @param[]  chan_num : channel number
****************************************************************************************
*/
void set_sta_connect_chan_num(uint32_t chan_num);

/**
****************************************************************************************
* @brief * wlan_start_ap : Create softAP
*
* @param[]  band: 0 -> 2.4G, 1 -> 5G
*           ssid: name
*           pw  : password(if open, set NULL)
* @return value: -1: fail, 0: success
****************************************************************************************
*/
int wlan_start_ap(uint8_t band, uint8_t *ssid, uint8_t *pw);

/**
****************************************************************************************
* @brief * wlan_stop_ap : Stop softAP
*
* @param[]  none
* @return value: -1: fail, 0: success
****************************************************************************************
*/
int wlan_stop_ap(void);

/**
****************************************************************************************
* @brief * wlan_ap_switch_channel : softAP switch to specified channel
*
* @param[]  chan_num: channel number
* @return value: -1: fail, 0: success
****************************************************************************************
*/
int wlan_ap_switch_channel(uint8_t chan_num);
/**
****************************************************************************************
* @brief * wlan_ap_disassociate_sta : softAP disassociate specified STA
*
* @param[]  macaddr: mac address of specified STA
* @return value: -1: fail, 0: success
****************************************************************************************
*/
int wlan_ap_disassociate_sta(struct mac_addr *macaddr);
/**
****************************************************************************************
 * @brief * All the bellow API related to AP should be called before 'wlan_start_ap'
****************************************************************************************
*/

/**
****************************************************************************************
* @brief * set_ap_ip_addr : Set IP start address for softAP
*
* @param[]  new_ip_addr: IP address
*                        Default: (192 | (168 << 8) | (88 << 16) | (1 << 24))
****************************************************************************************
*/
void set_ap_ip_addr(uint32_t new_ip_addr);
/**
****************************************************************************************
* @brief * set_ap_subnet_mask : Set subnet mask for softAP
*
* @param[]  new_mask: subnet mask
*                     Default: 255.255.255.0 -> 0x00FFFFFF
****************************************************************************************
*/
void set_ap_subnet_mask(uint32_t new_mask);
/**
****************************************************************************************
* @brief * set_ap_bcn_interval : Set Beacon interval for softAP
*
* @param[]  bcn_interval_ms: beacon interval(unit:ms)
*                            Default: 100
****************************************************************************************
*/
void set_ap_bcn_interval(uint32_t bcn_interval_ms);

/**
****************************************************************************************
* @brief * set_ap_channel_num : Set channel number for softAP, if not set, auto select
*
* @param[]  num: channel nuber, 2.4G(1~14), 5G(depend on 'fhost_chan')
****************************************************************************************
*/
void set_ap_channel_num(uint8_t num);
/**
****************************************************************************************
* @brief * set_ap_hidden_ssid : Set hidden ssid for softAP
*
* @param[]  val: 0 -> unhidden, 1 -> hidden
****************************************************************************************
*/
void set_ap_hidden_ssid(uint8_t val);
/**
****************************************************************************************
* @brief * set_ap_enable_he_rate : Enable HE rate for softAP
*
* @param[]  en: 1 -> enable, 0(Default) -> diable
****************************************************************************************
*/
void set_ap_enable_he_rate(uint8_t en);
/**
****************************************************************************************
* @brief * set_ap_allow_sta_inactivity_s : Set max inactivity time for connected STA (if
*                                          long time not receive/transmit, send a NULL 
*                                          frame to detect it. If detected fail, 
*                                          disassociate it).
*
* @param[]  s: default -> 60s
****************************************************************************************
*/
void set_ap_allow_sta_inactivity_s(uint8_t s);

/**
****************************************************************************************
* @brief * wlan_start_wps : Start push button WPS
*
* @param[]  none
* @return value: 0: success
****************************************************************************************
*/
int wlan_start_wps(void);

/**
****************************************************************************************
* @brief * wlan_ap_generate_ssid_pw_for_pairing & wlan_sta_auto_pairing are used as pairing
*
* @param[]  out: ssid, pw
*           in : times: paring retry times
* @return value: 0: success
****************************************************************************************
*/
int wlan_ap_generate_ssid_pw_for_pairing(char *ssid, char *pw);
int wlan_sta_auto_pairing(char *ssid, char *pw, int8_t times);

uint32_t get_ap_ip_addr(void);
uint32_t get_ap_subnet_mask(void);
uint8_t *get_selected_channel_numbers(uint8_t band);
uint32_t get_sta_connect_chan_freq(void);

#if (PLF_BT_STACK || PLF_BLE_STACK) && (PLF_WIFI_STACK)
__WEAK void wb_coex_wifi_on_set(int on);
__WEAK void wb_coex_wifi_connected_set(int connected);
__WEAK int wb_coex_bt_connected_get(void);
__WEAK int wb_coex_bt_a2dp_on_get(void);
__WEAK int wb_coex_bt_sco_on_get(void);
#endif

extern uint8_t is_fixed_ip;
extern uint32_t fixed_ip, fixed_gw, fixed_mask;
extern uint8_t disconnected_by_user;
#endif /* _WLAN_IF_H_ */

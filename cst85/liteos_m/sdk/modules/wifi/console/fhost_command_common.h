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

#ifndef _FHOST_COMMAND_COMMON_H_
#define _FHOST_COMMAND_COMMON_H_

/// Network Command result
enum fhost_network_res {
    FHOST_NW_SUCCESS,
    FHOST_NW_ERROR,
    FHOST_NW_UNKWN_CMD,
    FHOST_NW_NO_RESP,
};

extern uint8_t fhost_vif_idx;

int do_restore(int argc, char *argv[]);
int do_show_heap(int argc, char *argv[]);
int do_set_power(int argc, char *argv[]);
int do_mac(int argc, char *argv[]);
int do_auto_connect (int argc, char *argv[]);
int do_connect (int argc, char *argv[]);
int do_connect_wep(int argc, char * argv [ ]);
int do_disconnect (int argc, char *argv[]);
int do_create_ap (int argc, char *argv[]);
int do_start_ap (int argc, char *argv[]);
int do_stop_ap (int argc, char *argv[]);
int do_smartconf(int argc, char *argv[]);
int do_stop_smartconf(int argc, char *argv[]);
#ifdef CFG_WPS
int do_wps_pbc (int argc, char *argv[]);
#endif /* CFG_WPS */



int fhost_console_ping(char *params);
#if PLF_IPERF
rtos_task_handle iperf_task_handle;
int fhost_console_iperf(char *params);
#endif
char *fhost_nw_next_token(char **params);
int sta_auto_connect(void);
int fhost_nw_parse_ip4(char *str, uint32_t *ip, uint32_t *mask);

#endif /* _FHOST_COMMAND_COMMON_H_ */

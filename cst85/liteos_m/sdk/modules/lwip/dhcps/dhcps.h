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
#ifndef _DHCPS_H_
#define _DHCPS_H_

#include <stdio.h>
#include <string.h>
#include "lwip/arch.h"
#include "lwip/netif.h"
#include "lwip/udp.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "ethernet.h"

err_t dhcpServerStart(struct netif* netif);

err_t dhcpServerStop(struct netif* netif);

uint32_t get_client_ip_by_index(uint8_t index);

uint8_t get_client_ip_offered_cnt(void);

#endif /* _DHCPS_H_ */

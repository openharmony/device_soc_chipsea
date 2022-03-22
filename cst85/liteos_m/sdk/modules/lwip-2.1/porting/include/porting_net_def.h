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
#ifndef _PORTING_NET_DEF_H
#define _PORTING_NET_DEF_H

//chipsea
#include "lwip/tcpip.h"
#include "lwip/pbuf.h"

/*
 * DEFINITIONS
 ****************************************************************************************
 */
/// Net interface
typedef struct netif        net_if_t;

/// Net buffer
typedef struct pbuf_custom  net_buf_rx_t;

/// Net TX buffer
typedef struct pbuf         net_buf_tx_t;

/// Maximum size of a interface name (including null character)
#define NET_AL_MAX_IFNAME 4

#endif // _PORTING_NET_DEF_H

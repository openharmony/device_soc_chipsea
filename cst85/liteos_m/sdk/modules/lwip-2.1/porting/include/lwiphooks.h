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

#ifndef _LWIPHOOKS_H_
#define _LWIPHOOKS_H_

#define LWIP_HOOK_UNKNOWN_ETH_PROTOCOL  net_eth_receive

/**
 ****************************************************************************************
 * @brief LWIP hook to process ethernet packet not supported.
 *
 * Check if a socket has been created for this (netif, ethertype) couple.
 * If so push the buffer in the socket buffer list. (no buffer copy is done)
 *
 * @param[in] pbuf  Buffer containing the ethernet frame
 * @param[in] netif Pointer to the network interface that received the frame
 *
 * @return ERR_OK if a L2 socket exist for this frame and buffer has been successfully
 * pushed, ERR_MEM/ERR_VAL otherwise.
 ****************************************************************************************
 */
err_t net_eth_receive(struct pbuf *pbuf, struct netif *netif);

#define SO_CONNINFO 0x100c

#define LWIP_HOOK_SOCKETS_GETSOCKOPT lwip_sockopt_hook

#endif /* _LWIPHOOKS_H_ */

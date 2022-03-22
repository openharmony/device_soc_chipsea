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
#ifndef _WIFI_HOST_RX_DEF_H
#define _WIFI_HOST_RX_DEF_H

/// Number of RX buffers
#if NX_REORD
#define FHOST_RX_BUF_CNT            (NX_REORD * NX_REORD_BUF_SIZE + 2)
#else
#define FHOST_RX_BUF_CNT             4
#endif
/// Number of elements in the RX descriptor queue
#define FHOST_RX_QUEUE_DESC_ELT_CNT (FHOST_RX_BUF_CNT * 2)
/// Size of a RX buffer
#define FHOST_RX_BUF_SIZE           (RX_MAX_AMSDU_SUBFRAME_LEN + 1)

#endif // _WIFI_HOST_RX_DEF_H


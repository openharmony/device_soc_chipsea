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
#ifndef _REG_IPC_MUTEX_H_
#define _REG_IPC_MUTEX_H_

#include "reg_access.h"
#include "chip.h"

/**
 * IPC_MUTEX registers
 */
#define IPC_MUTEX_BASE_ADDR   (CS_IPCAPP_BASE + 0x0040)

__INLINE uint32_t ipc_mutex_get(int idx)
{
    return REG_PL_RD(IPC_MUTEX_BASE_ADDR + (idx << 2));
}

__INLINE void ipc_mutex_set(int idx, uint32_t value)
{
    REG_PL_WR(IPC_MUTEX_BASE_ADDR + (idx << 2), value);
}

/**
 * IPC_MUTEX ID definition
 */
enum {
    IPC_MUTEX_ID_UART0_OUTPUT   = 0,
    IPC_MUTEX_ID_UART1_OUTPUT   = 1,
    IPC_MUTEX_ID_UART2_OUTPUT   = 2,
    IPC_MUTEX_ID_WIFI_A2E_TX    = 3,
    IPC_MUTEX_ID_WIFI_E2A_MSG   = 4,
    IPC_MUTEX_ID_WIFI_E2A_RX    = 5,
    IPC_MUTEX_ID_WIFI_E2A_UF    = 6,
    IPC_MUTEX_ID_7              = 7,
    IPC_MUTEX_ID_MAX
};

#endif /* _REG_IPC_MUTEX_H_ */

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
#ifndef _WIFI_NX_CMDS_H
#define _WIFI_NX_CMDS_H

#if 0
#include <linux/spinlock.h>
#include <linux/completion.h>
#endif
#include <stddef.h>

#include "wifi_msg.h"
#include "cs_list.h"
#include "rtos_ohos_al.h"
#include "wb_co_int.h"
#include "wb_co_bool.h"
#include "dbg_assert.h"
#include "compiler.h"

#include "wb_co_list.h"



#ifdef CONFIG_RWNX_SDM
#define RWNX_80211_CMD_TIMEOUT_MS    (20 * 300)
#elif defined(CONFIG_RWNX_FHOST)
#define RWNX_80211_CMD_TIMEOUT_MS    (10000)
#else
#define RWNX_80211_CMD_TIMEOUT_MS    300
#endif

#define RWNX_CMD_FLAG_NONBLOCK      BIT(0)
#define RWNX_CMD_FLAG_REQ_CFM       BIT(1)
#define RWNX_CMD_FLAG_WAIT_PUSH     BIT(2)
#define RWNX_CMD_FLAG_WAIT_ACK      BIT(3)
#define RWNX_CMD_FLAG_WAIT_CFM      BIT(4)
#define RWNX_CMD_FLAG_DONE          BIT(5)
/* ATM IPC design makes it possible to get the CFM before the ACK,
 * otherwise this could have simply been a state enum */
#define RWNX_CMD_WAIT_COMPLETE(flags) \
    (!(flags & (RWNX_CMD_FLAG_WAIT_ACK | RWNX_CMD_FLAG_WAIT_CFM)))

#define RWNX_CMD_MAX_QUEUED         8


#include "wifi_ipc_shared.h"
#define rwnx_cmd_e2amsg ipc_e2a_msg
#define rwnx_cmd_a2emsg lmac_msg
#define RWNX_CMD_A2EMSG_LEN(m) (sizeof(struct lmac_msg) + m->param_len)
#define RWNX_CMD_E2AMSG_LEN_MAX (IPC_E2A_MSG_PARAM_SIZE * 4)


struct rwnx_hw;
struct rwnx_cmd;
typedef int (*msg_cb_fct)(struct rwnx_hw *rwnx_hw, struct rwnx_cmd *cmd,
                          struct rwnx_cmd_e2amsg *msg);

enum rwnx_cmd_mgr_state {
    RWNX_CMD_MGR_STATE_DEINIT,
    RWNX_CMD_MGR_STATE_INITED,
    RWNX_CMD_MGR_STATE_CRASHED,
};

typedef struct rwnx_cmd {
    struct list_head list;
    lmac_msg_id_t id;
    lmac_msg_id_t reqid;
    struct rwnx_cmd_a2emsg *a2e_msg;
    char *e2a_msg;
    uint32_t tkn;
    uint16_t flags;
    rtos_semaphore sema;

    //struct completion complete;
    uint32_t result;
    #ifdef CONFIG_RWNX_FHOST
    struct rwnx_term_stream *stream;
    #endif
} rwnx_cmd;

struct rwnx_cmd_mgr {
    enum rwnx_cmd_mgr_state state;
    //spinlock_t lock;
    rtos_mutex mutex;
    uint32_t next_tkn;
    uint32_t queue_sz;
    uint32_t max_queue_sz;

    struct list_head cmds;

    int  (*queue)(struct rwnx_cmd_mgr *, struct rwnx_cmd *);
    int  (*llind)(struct rwnx_cmd_mgr *, struct rwnx_cmd *);
    int  (*msgind)(struct rwnx_cmd_mgr *, struct rwnx_cmd_e2amsg *, msg_cb_fct);
    void (*print)(struct rwnx_cmd_mgr *);
    void (*drain)(struct rwnx_cmd_mgr *);
};

void rwnx_cmd_mgr_init(struct rwnx_cmd_mgr *cmd_mgr);
void rwnx_cmd_mgr_deinit(struct rwnx_cmd_mgr *cmd_mgr);

#endif // _WIFI_NX_CMDS_H

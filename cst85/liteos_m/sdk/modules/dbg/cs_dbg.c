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
#include "dbg.h"
#include <string.h>

#ifdef CFG_RTOS
#include "rtos_ohos_al.h"
#endif

/// Debug module environment definition. (moved here for host)
struct debug_env_tag dbg_env;

#if defined(CFG_RTOS) && DBG_MUTEX_ENABLED
extern rtos_mutex dbg_mutex;
#endif


void dbg_init(void)
{
    // Reset the environment
    memset(&dbg_env, 0, sizeof(dbg_env));

    // Enable only warnings and more critical per default
    dbg_env.filter_module = DBG_MOD_ALL;
    dbg_env.filter_severity = DBG_SEV_IDX_ERR;
}

#if defined(CFG_RTOS) && DBG_MUTEX_ENABLED
void dbg_rtos_init(void)
{
    if (rtos_mutex_create(&dbg_mutex)) {
        dbg("create dbg_mutex err\n");
    }
}
#endif

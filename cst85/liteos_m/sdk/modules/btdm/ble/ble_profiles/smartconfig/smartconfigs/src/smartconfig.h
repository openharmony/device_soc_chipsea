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
#ifndef _SMARTCONFIG_H_
#define _SMARTCONFIG_H_

#include "ble_ip_config.h"

#if (BLE_SMARTCONFIG)
#include "hal_prf_types.h"
#include "hal_profile.h"
#include "smartconfig_task.h"
#include "hal_attm.h"
#include "hal_profile_utils.h"


#define SMARTCONFIG_MAX_LEN            (200)

/// Possible states of the SMARTCONFIG task
enum
{
    /// Idle state
    SMARTCONFIG_IDLE,
    /// Connected state
    SMARTCONFIG_BUSY,

    /// Number of defined states.
    SMARTCONFIG_STATE_MAX,
};

///Attributes State Machine
enum
{
    SMARTCONFIG_IDX_SVC,

    SMARTCONFIG_IDX_WIFI_STATE_CHAR,
    SMARTCONFIG_IDX_WIFI_STATE_VAL,
    SMARTCONFIG_IDX_WIFI_STATE_NTF_CFG,

    SMARTCONFIG_IDX_AP_INFO_CHAR,
    SMARTCONFIG_IDX_AP_INFO_VAL,

    SMARTCONFIG_IDX_TIME_STATE_CHAR,
    SMARTCONFIG_IDX_TIME_STATE_VAL,
    SMARTCONFIG_IDX_TIME_STATE_NTF_CFG,

    SMARTCONFIG_IDX_TIME_INFO_CHAR,
    SMARTCONFIG_IDX_TIME_INFO_VAL,

    SMARTCONFIG_IDX_NB,
};


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Datapath Profile Server environment variable
struct smartconfig_env_tag
{
    /// profile environment
    prf_env_t   prf_env;
    /// Notification configuration
    uint8_t     wifi_state_ntf_cfg;
    /// Notification configuration
    uint8_t     time_ntf_cfg;
    /// Service Start Handle
    uint16_t    shdl;
    /// State of different task instances
    ke_state_t  state;
};


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Retrieve HRP service profile interface
 *
 * @return HRP service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* smartconfig_prf_itf_get(void);


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * Initialize task handler
 *
 * @param task_desc Task descriptor to fill
 ****************************************************************************************
 */
void smartconfig_task_init(struct ke_task_desc *task_desc);



#endif /* #if (BLE_SMARTCONFIG) */

#endif /* _SMARTCONFIG_H_ */


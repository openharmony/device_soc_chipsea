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

#ifndef _BLE_BASS_TASK_H
#define _BLE_BASS_TASK_H

#include "hal_prf_types.h"
#include "ble_ip_task.h" // Task definitions

#define BAS_BATTERY_LVL_MAX               (100)
///Maximal number of BAS that can be added in the DB
#define BASS_NB_BAS_INSTANCES_MAX         (2)

/// Messages for Battery Server
/*@TRACE*/
enum bass_msg_id
{
    /// Start the Battery Server - at connection used to restore bond data
    BASS_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_BASS),//!< BASS_ENABLE_REQ
    /// Confirmation of the Battery Server start
    BASS_ENABLE_RSP,                               //!< BASS_ENABLE_RSP
    /// Battery Level Value Update Request
    BASS_BATT_LEVEL_UPD_REQ,                       //!< BASS_BATT_LEVEL_UPD_REQ
    /// Inform APP if Battery Level value has been notified or not
    BASS_BATT_LEVEL_UPD_RSP,                       //!< BASS_BATT_LEVEL_UPD_RSP
    /// Inform APP that Battery Level Notification Configuration has been changed - use to update bond data
    BASS_BATT_LEVEL_NTF_CFG_IND,                   //!< BASS_BATT_LEVEL_NTF_CFG_IND
};

/// Features Flag Masks
enum bass_features
{
    /// Battery Level Characteristic doesn't support notifications
    BAS_BATT_LVL_NTF_NOT_SUP,
    /// Battery Level Characteristic support notifications
    BAS_BATT_LVL_NTF_SUP,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

/// Parameters for the database creation
struct bass_db_cfg
{
    /// Number of BAS to add
    uint8_t bas_nb;
    /// Features of each BAS instance
    uint8_t features[BASS_NB_BAS_INSTANCES_MAX];
    /// Battery Level Characteristic Presentation Format - Should not change during connection
    struct prf_char_pres_fmt batt_level_pres_format[BASS_NB_BAS_INSTANCES_MAX];
};

/// Parameters of the @ref BASS_ENABLE_REQ message
struct bass_enable_req
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration
    uint8_t  ntf_cfg;
    /// Old Battery Level used to decide if notification should be triggered
    uint8_t  old_batt_lvl[BASS_NB_BAS_INSTANCES_MAX];
};

/// Parameters of the @ref BASS_ENABLE_RSP message
struct bass_enable_rsp
{
    /// connection index
    uint8_t conidx;
    ///status
    uint8_t status;
};

///Parameters of the @ref BASS_BATT_LEVEL_UPD_REQ message
struct bass_batt_level_upd_req
{
    /// BAS instance
    uint8_t bas_instance;
    /// Battery Level
    uint8_t batt_level;
};

///Parameters of the @ref BAPS_BATT_LEVEL_UPD_RSP message
struct bass_batt_level_upd_rsp
{
    ///status
    uint8_t status;
};

///Parameters of the @ref BASS_BATT_LEVEL_NTF_CFG_IND message
struct bass_batt_level_ntf_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Notification Configuration
    uint8_t  ntf_cfg;
};

#endif // _BLE_BASS_TASK_H

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
#ifndef _HAL_PROFILE_H
#define _HAL_PROFILE_H

#include "ble_ip_config.h"

#if (BLE_PROFILES)

#include "ble_ke_task.h"
#include "hal_gapm_task.h"

/**
 * Profile task fields
 *
 *  15   14   13   12   11   10   9    8    7    6    5    4    3    2    1    0
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 * | MI |                               TASK Number                                |
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 *
 * Bit [0-14] : Task number value
 * Bit [15]   : Task is multi-instantiated (Connection index is conveyed)
 */
enum prf_perm_mask
{
    /// Task number value
    PERM_MASK_PRF_TASK      = 0x7FFF,
    PERM_POS_PRF_TASK       = 0,
    /// Task is multi-instantiated
    PERM_MASK_PRF_MI        = 0x8000,
    PERM_POS_PRF_MI         = 15,
};
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/// Profile Environment Data
typedef struct prf_env
{
    /// Application Task Number - if MSB bit set, Multi-Instantiated task
    ke_task_id_t app_task;
    /// Profile Task  Number    - if MSB bit set, Multi-Instantiated task
    ke_task_id_t prf_task;
} prf_env_t;



/// Profile task environment variable definition to dynamically allocate a Task.
struct prf_task_env
{
    /// Profile Task description
    struct ke_task_desc desc;
    /// pointer to the allocated memory used by profile during runtime.
    prf_env_t*          env;
    /// Profile Task Number
    ke_task_id_t        task;
    /// Profile Task Identifier
    ke_task_id_t        id;
};

/**
 ****************************************************************************************
 * @brief Initialization of the Profile module.
 * This function performs all the initializations of the Profile module.
 *  - Creation of database (if it's a service)
 *  - Allocation of profile required memory
 *  - Initialization of task descriptor to register application
 *      - Task State array
 *      - Number of tasks
 *      - Default task handler
 *
 * @param[out]    env        Collector or Service allocated environment data.
 * @param[in|out] start_hdl  Service start handle (0 - dynamically allocated), only applies for services.
 * @param[in]     app_task   Application task number.
 * @param[in]     sec_lvl    Security level (AUTH, EKS and MI field of @see enum attm_value_perm_mask)
 * @param[in]     param      Configuration parameters of profile collector or service (32 bits aligned)
 *
 * @return status code to know if profile initialization succeed or not.
 ****************************************************************************************
 */
typedef uint8_t (*prf_init_fnct)    (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  void* params);

/**
 ****************************************************************************************
 * @brief Destruction of the Profile module - due to a reset for instance.
 * This function clean-up allocated memory (attribute database is destroyed by another
 * procedure)
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 ****************************************************************************************
 */
typedef void    (*prf_destroy_fnct) (struct prf_task_env* env);

/**
 ****************************************************************************************
 * @brief Handles Connection creation
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 ****************************************************************************************
 */
typedef void    (*prf_create_fnct)  (struct prf_task_env* env, uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Handles Disconnection
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 * @param[in]        reason     Detach reason
 ****************************************************************************************
 */
typedef void    (*prf_cleanup_fnct) (struct prf_task_env* env, uint8_t conidx, uint8_t reason);

/// Profile task callbacks.
struct prf_task_cbs
{
    /// Initialization callback
    prf_init_fnct    init;
    /// Destroy profile callback
    prf_destroy_fnct destroy;
    /// Connection callback
    prf_create_fnct  create;
    /// Disconnection callback
    prf_cleanup_fnct cleanup;
};


/// Profile Manager environment structure
struct prf_env_tag
{
    /// Array of profile tasks that can be managed by Profile manager.
    struct prf_task_env prf[BLE_NB_PROFILES];
};

/*
 * MACROS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
extern struct prf_env_tag prf_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Perform Profile initialization
 *
 * @param[in] init_type  Type of initialization (@see enum rwip_init_type)
 ****************************************************************************************
 */
void prf_init(uint8_t reset);


/**
 ****************************************************************************************
 * @brief Create Profile (collector or service) task creation and initialize it.
 *
 * @param[in|out] params   Collector or Service parameter used for profile task creation
 * @param[out]    prf_task Allocated Task number
 *
 * @return status of adding profile task
 ****************************************************************************************
 */
uint8_t prf_add_profile(struct gapm_profile_task_add_cmd * params, ke_task_id_t *prf_task);


/**
 ****************************************************************************************
 * @brief Link creation event, update profiles states.
 *
 * @param[in] conidx        connection index
 *
 ****************************************************************************************
 */
void prf_create(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Link disconnection event, clean-up profiles.
 *
 * @param[in] conidx        connection index
 * @param[in] reason        detach reason
 *
 ****************************************************************************************
 */
void prf_cleanup(uint8_t conidx, uint8_t reason);



/**
 ****************************************************************************************
 * @brief Retrieve environment variable allocated for a profile
 *
 * @param[in] prf_id        Profile Task Identifier
 *
 * @return Environment variable allocated for a profile
 ****************************************************************************************
 */
prf_env_t* prf_env_get(uint16_t prf_id);


/**
 ****************************************************************************************
 * @brief Retrieve source profile task number value
 *
 * @param[in] env     Profile Environment
 * @param[in] conidx  Connection index
 *
 * @return Source profile task number value
 ****************************************************************************************
 */
ke_task_id_t prf_src_task_get(prf_env_t* env, uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Retrieve destination application task number value
 *
 * @param[in] env     Profile Environment
 * @param[in] conidx  Connection index
 *
 * @return Destination application task number value
 ****************************************************************************************
 */
ke_task_id_t prf_dst_task_get(prf_env_t* env, uint8_t conidx);


/**
 ****************************************************************************************
 * @brief Retrieve Task Identifier from Task number
 * (automatically update index of task in returned task id)
 *
 * @param task Task number
 * @return Task Identifier
 ****************************************************************************************
 */
ke_task_id_t prf_get_id_from_task(ke_msg_id_t task);

/**
 ****************************************************************************************
 * @brief Retrieve Task Number from Task Identifier
 * (automatically update index of task in returned task id)
 *
 * @param id Task Identifier
 * @return Task Number
 ****************************************************************************************
 */
ke_task_id_t prf_get_task_from_id(ke_msg_id_t id);

/**
 ****************************************************************************************
 * @brief Retrieve Profile task callbacks
 *
 * @param task_id Profile task identifier
 * @return Profile task callbacks.
 ****************************************************************************************
*/
const struct prf_task_cbs * prf_itf_get(uint16_t task_id);

#endif // (BLE_PROFILES)

#endif // _HAL_PROFILE_H

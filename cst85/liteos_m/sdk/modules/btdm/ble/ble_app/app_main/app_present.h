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
#ifndef _APP_PRESETN_H
#define _APP_PRESETN_H

#include "ble_ip_config.h"     // SW configuration

#if (BLE_APP_PRESENT)

#include <stdint.h>          // Standard Integer Definition
#include "bt_common.h"           // Common BT Definitions
#include "arch.h"            // Platform Definitions
#include "hal_gapc.h"            // GAPC Definitions

#if (NVDS_SUPPORT)
#include "nvds.h"
#endif // (NVDS_SUPPORT)
#include "hal_gapm_task.h"
#include "flash_api.h"

/*
 * DEFINES
 ****************************************************************************************
 */
/// Maximal length of the Device Name value
#define APP_DEVICE_NAME_MAX_LEN      (32)

/*
 * MACROS
 ****************************************************************************************
 */

#define APP_HANDLERS(subtask)    {&subtask##_msg_handler_list[0], ARRAY_LEN(subtask##_msg_handler_list)}

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

#if (NVDS_SUPPORT)
/// List of Application NVDS TAG identifiers
enum app_nvds_tag
{
    /// BD Address
    NVDS_TAG_BD_ADDRESS                 = 0x01,
    NVDS_LEN_BD_ADDRESS                 = 6,

    /// Device Name
    NVDS_TAG_DEVICE_NAME                = 0x02,
    NVDS_LEN_DEVICE_NAME                = 62,

    /// BLE Application Advertising data
    NVDS_TAG_APP_BLE_ADV_DATA           = 0x0B,
    NVDS_LEN_APP_BLE_ADV_DATA           = 32,

    /// BLE Application Scan response data
    NVDS_TAG_APP_BLE_SCAN_RESP_DATA     = 0x0C,
    NVDS_LEN_APP_BLE_SCAN_RESP_DATA     = 32,

    /// Mouse Sample Rate
    NVDS_TAG_MOUSE_SAMPLE_RATE          = 0x38,
    NVDS_LEN_MOUSE_SAMPLE_RATE          = 1,
    /// Peripheral Bonded
    NVDS_TAG_PERIPH_BONDED              = 0x39,
    NVDS_LEN_PERIPH_BONDED              = 1,
    /// Mouse NTF Cfg
    NVDS_TAG_MOUSE_NTF_CFG              = 0x3A,
    NVDS_LEN_MOUSE_NTF_CFG              = 2,
    /// Mouse Timeout value
    NVDS_TAG_MOUSE_TIMEOUT              = 0x3B,
    NVDS_LEN_MOUSE_TIMEOUT              = 2,
    /// Peer Device BD Address
    NVDS_TAG_PEER_BD_ADDRESS            = 0x3C,
    NVDS_LEN_PEER_BD_ADDRESS            = 7,
    /// Mouse Energy Safe
    NVDS_TAG_MOUSE_ENERGY_SAFE          = 0x3D,
    NVDS_LEN_MOUSE_SAFE_ENERGY          = 2,
    /// EDIV (2bytes), RAND NB (8bytes),  LTK (16 bytes), Key Size (1 byte)
    NVDS_TAG_LTK                        = 0x3E,
    NVDS_LEN_LTK                        = 28,
    /// PAIRING
    NVDS_TAG_PAIRING                    = 0x3F,
    NVDS_LEN_PAIRING                    = 54,

    /// Audio mode 0 task
    NVDS_TAG_AM0_FIRST                  = 0x90,
    NVDS_TAG_AM0_LAST                   = 0x9F,

    /// Local device Identity resolving key
    NVDS_TAG_LOC_IRK                    = 0xA0,
    NVDS_LEN_LOC_IRK                    = KEY_LEN,

    /// Peer device Resolving identity key (+identity address)
    NVDS_TAG_PEER_IRK                   = 0xA1,
    NVDS_LEN_PEER_IRK                   = sizeof(struct gapc_irk),
};
#endif // (NVDS_SUPPORT)

/// Advertising state machine
enum app_adv_state
{
    /// Advertising activity does not exists
    APP_ADV_STATE_IDLE = 0,
    /// Creating advertising activity
    APP_ADV_STATE_CREATING,
    /// Setting advertising data
    APP_ADV_STATE_SETTING_ADV_DATA,
    /// Setting scan response data
    APP_ADV_STATE_SETTING_SCAN_RSP_DATA,

    /// Advertising activity created
    APP_ADV_STATE_CREATED,
    /// Starting advertising activity
    APP_ADV_STATE_STARTING,
    /// Advertising activity started
    APP_ADV_STATE_STARTED,
    /// Stopping advertising activity
    APP_ADV_STATE_STOPPING,
    /// Upddate advertising data
    APP_ADV_STATE_UPDATE_ADV_DATA,
};

/// Scan state machine
enum app_scan_state
{
    ///
    APP_SCAN_STATE_IDLE = 0,
    ///
    APP_SCAN_STATE_CREATING,
    ///
    APP_SCAN_STATE_CREATED,
    ///
    APP_SCAN_STATE_STARTING,
    ///
    APP_SCAN_STATE_STARTED,
    ///
    APP_SCAN_STATE_STOPPING,
};

enum app_deinit_flag
{
    APP_DEINIT_FLAG_ADV = BIT0,
    APP_DEINIT_FLAG_SCAN = BIT1,
    APP_DEINIT_FLAG_CON = BIT2,
};
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Structure containing information about the handlers for an application subtask
struct app_subtask_handlers
{
    /// Pointer to the message handler table
    const struct ke_msg_handler *p_msg_handler_tab;
    /// Number of messages handled
    uint16_t msg_cnt;
};

/// Application environment structure
struct app_env_tag
{
    /// Connection handle
    uint16_t conhdl;
    /// Connection Index
    uint8_t  conidx;

    /// Advertising activity index
    uint8_t adv_actv_idx;
    /// Previous advertising state (@see enum app_adv_state)
    uint8_t adv_state_prv;

    /// Current advertising state (@see enum app_adv_state)
    uint8_t adv_state;
    /// Next expected operation completed event
    uint8_t adv_op;

    /// Last initialized profile
    uint8_t next_svc;

    /// Bonding status
    bool bonded;

    /// Device Name length
    uint8_t dev_name_len;
    /// Device Name
    uint8_t dev_name[APP_DEVICE_NAME_MAX_LEN];

    /// Appearance
    uint16_t appearance;

    /// Local device IRK
    uint8_t loc_irk[KEY_LEN];

    /// Secure Connections on current link
    bool sec_con_enabled;

    /// Counter used to generate IRK
    uint8_t rand_cnt;

    /// Delete bonding
    bool del_bond;

    /// Scan activity index
    uint8_t scan_actv_idx;
    /// Current scan state (@see enum app_scan_state)
    uint8_t scan_state;
    /// Next expected operation completed event
    uint8_t scan_op;
};

typedef struct app_env_tag AonBleAppEnv;

struct ble_app_user_info
{
    /// Device Name length
    uint16_t dev_name_len;
    /// Device Name
    uint8_t dev_name[32];
    /// Advertising Data length
    uint16_t adv_data_len;
    /// Advertising data
    uint8_t adv_data[ADV_DATA_LEN];
    /// Advertising parameters
    bool user_adv_param;
    /// Own address type (@see enum gapm_own_addr)
    uint8_t own_addr_type;
    /// BD Address of device
    bd_addr_t own_addr;
    /// Advertising duration (in unit of 10ms). 0 means that advertising continues
    /// until the host disable it
    uint16_t adv_duration;
    /// For prop parameter, @see enum gapm_leg_adv_prop, @see enum gapm_ext_adv_prop and @see enum gapm_per_adv_prop for help
    struct gapm_adv_create_param adv_param;
    struct gapm_scan_param scan_param;
    uint16_t scan_duration;
    /// app_user_callback
    struct app_callbacks *user_app_callback;
};

typedef struct ble_app_user_info AonBleAppUserInfo;

/// APP callbacks
struct app_callbacks
{
    /// Callback app init
    void (*app_on_init)(void);

    /// Callback app init complete
    void (*app_on_init_complete)(void);

    /// Callback app add svc
    bool (*app_on_add_svc)(void);

    /// Callback app enable prf
    void (*app_on_enable_prf)(uint8_t);

    /// Callback connect
    void (*app_on_connection)(const uint8_t, struct gapc_connection_req_ind const *);

    /// Callback disconnect
    void (*app_on_disconnect)(struct gapc_disconnect_ind const *);

    /// Callback con param update
    void (*app_on_update_params_request)(struct gapc_param_update_req_ind const *);

    /// Callback adv operation status
    void (*app_on_adv_status)(struct gapm_cmp_evt const *);

};

typedef struct _BleAppOp {
    list_node_t   node;
    uint8_t       opType;
    uint8_t       adv_state;
    uint8_t       adv_state_prv;
} BleAppOp;

/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */

/// Application environment
extern struct app_env_tag app_env;
extern struct ble_app_user_info ble_user_info;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize the BLE demo application.
 ****************************************************************************************
 */
void appm_init(void);

/**
 ****************************************************************************************
 * @brief Add a required service in the database
 ****************************************************************************************
 */
bool appm_add_svc(void);


/**
 ****************************************************************************************
 * @brief
 ****************************************************************************************
 */
void appm_adv_fsm_next(void);

/**
 ****************************************************************************************
 * @brief
 ****************************************************************************************
 */
void appm_scan_fsm_next(void);

/**
 ****************************************************************************************
 * @brief Send to request to update the connection parameters
 ****************************************************************************************
 */
void appm_update_param(struct gapc_conn_param *conn_param);

/**
 ****************************************************************************************
 * @brief Send a disconnection request
 ****************************************************************************************
 */
void appm_disconnect(void);

/**
 ****************************************************************************************
 * @brief Retrieve device name
 *
 * @param[out] device name
 *
 * @return name length
 ****************************************************************************************
 */
uint8_t appm_get_dev_name(uint8_t* name);

/**
 ****************************************************************************************
 * @brief Start/stop advertising
 *
 * @param[in] start     True if advertising has to be started, else false
 ****************************************************************************************
 */
void appm_update_adv_state(bool start);

/**
 ****************************************************************************************
 * @brief delete advertising
 *
 * @param[in] none
 ****************************************************************************************
 */

void appm_delete_advertising(void);
/**
 ****************************************************************************************
 * @brief Return if the device is currently bonded
 ****************************************************************************************
 */
bool app_sec_get_bond_status(void);

uint8_t appm_set_dev_name(char* name, uint8_t len);
void appm_set_dev_appearance(uint16_t appearance);
void appm_set_dev_info(struct gapc_set_dev_info_req_ind const *param, uint8_t* status);

void ble_enable(void);
void ble_disable(void);
void ble_disabled(void);
bool ble_activity_enabled(void);
void app_ble_env_init();
void app_ble_addr_init(uint8_t *addr, uint8_t own_addr_type);
void app_ble_set_dev_name(char* name, uint8_t len);
void app_ble_set_adv_data(uint8_t *buf, uint16_t buf_len);
uint8_t app_ble_set_adv_params(struct gapm_adv_create_param* params, uint16_t adv_duration);
uint8_t app_ble_update_adv_data();
uint8_t app_ble_update_adv_params(uint32_t adv_intv_min, uint32_t adv_intv_max);
uint8_t app_ble_update_con_params(uint16_t conn_intv_min, uint16_t conn_intv_max,uint16_t conn_latency, uint16_t time_out);
uint8_t app_ble_start_scan();
uint8_t app_ble_stop_scan();
uint8_t app_ble_start_adv();
uint8_t app_ble_stop_adv();
uint8_t app_ble_disconnect();
void app_ble_register_callbak(struct app_callbacks *ble_app_callbak);
void app_ble_update_addr(uint8_t *addr);
void app_ble_oplist_init(void);
void app_ble_check_oplist(void);
void app_ble_oplist_insert(uint8_t optype ,uint8_t adv_state ,uint8_t adv_state_prv ,void *cmdptr);
bool app_env_check(void);

/// @} APP

#endif //(BLE_APP_PRESENT)

#endif // _APP_PRESETN_H

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
#ifndef _HID_OVER_GATT_COMMON_H
#define _HID_OVER_GATT_COMMON_H

#include "hal_prf_types.h"
#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// HID Information bit values
enum hogp_info_bit
{
    /// Device capable of providing wake-up signal to a HID host
    HIDS_REMOTE_WAKE_CAPABLE           = 0x01,
    /// Normally connectable support bit
    HIDS_NORM_CONNECTABLE              = 0x02,
};

/// HID Control Point Characteristic value keys
enum hogp_ctnl_pt
{
    /// Suspend
    HOGP_CTNL_PT_SUSPEND            = 0x00,
    /// Exit suspend
    HOGP_CTNL_PT_EXIT_SUSPEND,
};

/// Protocol Mode Char. value Keys
enum hogp_boot_prot_mode
{
    /// Boot Protocol Mode
    HOGP_BOOT_PROTOCOL_MODE         = 0x00,
    /// Report Protocol Mode
    HOGP_REPORT_PROTOCOL_MODE,
};

/// Report Reference structure
struct hids_report_ref
{
    ///Report ID
    uint8_t report_id;
    ///Report Type
    uint8_t report_type;
};

/// HID Information structure
struct hids_hid_info
{
    /// bcdHID
    uint16_t bcdHID;
    /// bCountryCode
    uint8_t  bCountryCode;
    /// Flags
    uint8_t  flags;
};

#endif // _HID_OVER_GATT_COMMON_H

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
#ifndef _BLE_APP_CONFIG_H
#define _BLE_APP_CONFIG_H

/******************************************************************************************/
/* -------------------------   BLE APPLICATION SETTINGS      -----------------------------*/
/******************************************************************************************/

/// Health Thermometer Application
#if defined(CFG_APP_HT)
#define BLE_APP_HT           1
#else // defined(CFG_APP_HT)
#define BLE_APP_HT           0
#endif // defined(CFG_APP_HT)

#if defined(CFG_APP_HR)
#define BLE_APP_HR           1
#else
#define BLE_APP_HR           0
#endif

/// HID Application
#if defined(CFG_APP_HID)
#define BLE_APP_HID          1
#else // defined(CFG_APP_HID)
#define BLE_APP_HID          0
#endif // defined(CFG_APP_HID)

/// DIS Application
#if defined(CFG_APP_DIS)
#define BLE_APP_DIS          1
#else // defined(CFG_APP_DIS)
#define BLE_APP_DIS          0
#endif // defined(CFG_APP_DIS)

/// Audio Application
#if defined(CFG_APP_AM0)
#define BLE_APP_AM0          1
#else // defined(CFG_APP_AM0)
#define BLE_APP_AM0          0
#endif // defined(CFG_APP_AM0)

/// Battery Service Application
#if (BLE_APP_HID)
#define BLE_APP_BATT          1
#else
#define BLE_APP_BATT          0
#endif // (BLE_APP_HID)

/// Smart Config Application
#if defined(CFG_APP_SMARTCONFIG)
#define BLE_APP_SMARTCONFIG           1
#else // defined(CFG_APP_SMARTCONFIG)
#define BLE_APP_SMARTCONFIG           0
#endif // defined(CFG_APP_SMARTCONFIG)


/// Security Application
#if (defined(CFG_APP_SEC) || BLE_APP_HID || BLE_APP_AM0)
#define BLE_APP_SEC          1
#else //(defined(CFG_APP_SEC) || BLE_APP_HID || BLE_APP_AM0)
#define BLE_APP_SEC          0
#endif //(defined(CFG_APP_SEC) || BLE_APP_HID || BLE_APP_AM0)

/// Secure Connection
#if (BLE_APP_AM0)
#define BLE_APP_SEC_CON      1
#else //(BLE_APP_AM0)
#define BLE_APP_SEC_CON      0
#endif ////(BLE_APP_AM0)

/// Hearing Aid Service Configuration
#if (BLE_APP_AM0)
#define AM0_APP_OPTIONAL_CHARACTERISTICS        0
/// Default Settings for Optional Characteristics
/// Specification Default Value
#define AM0_APP_DEFAULT_TREBLE                  0
#define AM0_APP_DEFAULT_BASS                    0
/// Following have no specification Defaults - as application dependent.
/// Please change to suit product.
#define AM0_APP_DEFAULT_MIXED_VOL_STEP          5
#define AM0_APP_DEFAULT_MIXED_VOL               10
#define AM0_APP_DEFAULT_MIC_VOL_STEP            8
#define AM0_APP_DEFAULT_STREAM_VOL_STEP         7
#define AM0_APP_DEFAULT_MIC_SENSITIVITY         127
#define AM0_APP_DEFAULT_SENSITIVITY_STEP        12
/// Default is to have no active streaming program id.
#define AM0_APP_DEFAULT_ACT_STREAM_PROG_ID      0
#else
#define AM0_APP_OPTIONAL_CHARACTERISTICS        0
#endif //BLE_APP_AM0

#endif // _BLE_APP_CONFIG_H

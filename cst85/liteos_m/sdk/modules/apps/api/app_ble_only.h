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
#ifndef __APP_BLE_ONLY_H__
#define __APP_BLE_ONLY_H__
#ifdef CFG_BLE_ONLY
#include "cs_target_config.h"
#include "al_rtos.h"
#include "app_present.h"
#include "hal_att.h"
#if (BLE_APP_SEC)
#include "app_present_sec.h"                 // Application security Definition
#endif // (BLE_APP_SEC)

#if (BLE_APP_BATT)
#include "app_batt.h"                // Battery Application Definitions
#endif //(BLE_APP_DIS)

#if (BLE_APP_HID)
#include "app_hid.h"                 // HID Application Definitions
#endif //(BLE_APP_HID)

#if (BLE_APP_SMARTCONFIG)
#include "app_smartconfig.h"         // Smart Config Application Definitions
#endif //(BLE_APP_SMARTCONFIG)

#define APP_UPDATE_DATA          ("BLE_UPDATE_ADV_DATA")
#define APP_UPDATE_DATA_LEN      sizeof(APP_UPDATE_DATA)

/// Advertising channel map - 37, 38, 39
#define APP_ADV_CHMAP           (0x07)
/// Advertising minimum interval - 40ms (64*0.625ms)
#define APP_ADV_INT_MIN         (64)
/// Advertising maximum interval - 40ms (64*0.625ms)
#define APP_ADV_INT_MAX         (64)
/// Fast advertising interval
#define APP_ADV_FAST_INT        (32)

void app_ble_init(void);

#endif
#endif /* CSBT_H */

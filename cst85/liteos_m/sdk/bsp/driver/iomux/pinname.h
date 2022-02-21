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
#ifndef _PINNAME_H_
#define _PINNAME_H_

#define PIN_TYPE_SHIFT    8
#define PIN_TYPE_MASK     0xFF
#define PIN_IDX_MASK      0xFF

#define PIN_TYPE(pin)     ((pin >> PIN_TYPE_SHIFT) & PIN_TYPE_MASK)
#define PIN_IDX(pin)      (pin & PIN_IDX_MASK)
#define PIN(type, idx)    ((type << PIN_TYPE_SHIFT) | idx)

typedef enum
{
    PIN_TYPE_NC  = 0x00,
    PIN_TYPE_A   = 0x01,
    PIN_TYPE_B   = 0x02,
    PIN_TYPE_PWR = 0x03
} PinType;

typedef enum {
    PA_0 = PIN(PIN_TYPE_A, 0),
    PA_1,
    PA_2,
    PA_3,
    PA_4,
    PA_5,
    PA_6,
    PA_7,
    PA_8,
    PA_9,
    PA_10,
    PA_11,
    PA_12,
    PA_13,
    PA_14,
    PA_15,
    PA_16,
    PA_17,
    PA_18,
    PA_19,
    PA_20,
    PA_21,
    PA_22,
    PA_23,
    PA_24,
    PA_25,
    PA_26,
    PA_27,
    PA_28,
    PA_29,
    PA_30,
    PA_31,
    PB_0 = PIN(PIN_TYPE_B, 0),
    PB_1,
    PB_2,
    PB_3,
    PB_4,
    PB_5,
    PB_6,
    PB_7,
    PB_8,
    PB_9,
    PB_10,
    PB_11,
    PB_12,
    PB_13,
    PB_14,
    PB_15,
    PB_16,
    PB_17,
    PB_18,
    PB_19,
    PB_20,
    PB_21,
    PB_22,
    PB_23,
    PB_24,
    PB_25,
    PB_26,
    PB_27,
    PB_28,
    PB_29,
    PB_30,
    PB_31,
    PIN_PWR = PIN(PIN_TYPE_PWR, 0),
    PIN_NC = PIN(PIN_TYPE_NC, 0),
} PinName;

#endif /* _PINNAME_H_ */

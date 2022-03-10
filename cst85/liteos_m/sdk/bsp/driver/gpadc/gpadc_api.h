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
#ifndef _GPADC_API_H_
#define _GPADC_API_H_

#include "plf.h"

enum {
    GPADC_SAMPLERATE_DFLT   = 0x02,
    GPADC_SAMPLERATE_HIGH   = 0x40,
    GPADC_SAMPLERATE_MID    = 0x80,
    GPADC_SAMPLERATE_LOW    = 0xF0,
};

enum {
    GPADC_TYPE_VBAT = 0,
    GPADC_TYPE_VIO,
    GPADC_TYPE_TEMP0,
};

/**
 * @brief       Init gpadc func
 * @param[in]   Gpiob index
 */
void gpadc_init(int gpbidx);

/**
 * @brief       Set smaple rate
 * @param[in]   Rate val
 */
void gpadc_samplerate_set(int rate);

/**
 * @brief       Free gpadc func
 * @param[in]   Gpiob index
 */
void gpadc_free(int gpbidx);

/**
 * @brief       Read voltage from gpiob pin
 * @param[in]   Gpiob index
 * @return      Voltage value, range: 0 ~ 1175mV
 */
int gpadc_read(int gpbidx);

/**
 * @brief       Measure voltage/temperature
 * @param[in]   GPADC type
 * @return      Voltage/temperature value (mV/ C)
 */
int gpadc_measure(int type);

#endif /* _GPADC_API_H_ */

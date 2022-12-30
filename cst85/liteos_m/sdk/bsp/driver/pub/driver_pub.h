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
#ifndef DRIVER_PUBLIC_H
#define DRIVER_PUBLIC_H

/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/

#include <time.h>

#define TIME_FUNC_GRP_0_IMPL    1
#define TIME_FUNC_GRP_1_IMPL    0

enum time_origin_t {
    /** Since boot time */
    SINCE_BOOT,
    /** Since Epoch : 1970-01-01 00:00:00 +0000 (UTC) */
    SINCE_EPOCH,
};

#if TIME_FUNC_GRP_0_IMPL
#define YEAR0          1900
#define EPOCH_YEAR     1970

struct tm* gmtime_offset_r(const time_t* timer, struct tm *st_time, int offset);
time_t mk_gmtime_offset_r(const struct tm *st_time, int offset);
#endif

void cs_time_init(uint32_t sec, uint32_t usec);
void cs_time_update(uint32_t sec, uint32_t usec);
int cs_time_get(enum time_origin_t origin, uint32_t *sec, uint32_t *usec);
int cs_time_us_get(enum time_origin_t origin, uint64_t *usec);
#endif


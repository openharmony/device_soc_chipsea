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
#ifndef _CO_ERRORS_H_
#define _CO_ERRORS_H_

#include "co_types_def.h"

/*---------------------------------------------------------------------------
 *
 * common error status.
 */
typedef U8 Co_Error;

#define CO_ERROR_ERROR_NONE         0
#define CO_ERROR_ERROR              1
#define CO_ERROR_PENDING            2
#define CO_ERROR_DISCONNECT         3
#define CO_ERROR_NOCON              4
#define CO_ERROR_IS_USEING          5
#define CO_ERROR_PLAY_NOT_DONE      6
#define CO_ERROR_PLAY_DONE          7
#define CO_ERROR_NO_PROGRESS        8
#define CO_ERROR_IS_CON             9
#define CO_ERROR_SEND_OUT           10
#define CO_ERROR_NOT_DONE           11
#define CO_ERROR_NO_RESOURCES       12
#define CO_ERROR_NOT_FOUND          13
#define CO_ERROR_DEVICE_NOT_FOUND   14
#define CO_ERROR_CON_ERROR          15
#define CO_ERROR_TIMER_TIMEOUT      16
#define CO_ERROR_NO_CONN            17
#define CO_ERROR_INVALID_PARAM      18
#define CO_ERROR_IS_GOING_ON        19
#define CO_ERROR_IS_LIMITED         20
#define CO_ERROR_INVALID_TYPE       21
#define CO_ERROR_HCI_NOT_OPEN       22
#define CO_ERROR_NOT_SUPPORTED      23
#define CO_ERROR_CONTINUE           24
#define CO_ERROR_CANCELLED          25
#define CO_ERROR_UNDEFINED          30
/* End of Co_Error */

#endif /* _CO_ERRORS_H_ */

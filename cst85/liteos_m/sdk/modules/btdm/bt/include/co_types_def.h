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
#ifndef _CO_TYPES_DEF_H_
#define _CO_TYPES_DEF_H_

/****************************************************************************
 *
 * Common definitions
 *
 ****************************************************************************/

#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif

typedef enum { FALSE = 0, TRUE = 1 } Boolean;
/*chipsea define  conflicting LOS */
//typedef unsigned char               BOOL;
typedef unsigned char               U8;
typedef unsigned short              U16;
typedef unsigned int                U32;
typedef signed char                 S8;
typedef signed short                S16;
typedef signed int                  S32;
typedef unsigned char               u8;
typedef unsigned short              u16;
typedef unsigned long               u32;
typedef unsigned long long          u64;
typedef signed char                 s8;
typedef signed short                s16;
typedef signed long                 s32;
typedef signed long long            s64;
typedef unsigned char               byte;           /*  unsigned 8-bit data     */
typedef unsigned short              word;           /*  unsigned 16-bit data    */
typedef unsigned long               dword;          /*  unsigned 32-bit data    */

#endif /* _CO_TYPES_DEF_H_ */

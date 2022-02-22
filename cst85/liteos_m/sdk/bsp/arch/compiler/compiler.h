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
/**
 ****************************************************************************************
 *
 * @file compiler.h
 *
 * @brief Definitions of compiler specific directives.
 *
 ****************************************************************************************
 */

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "cmsis_compiler.h"

/// define the static keyword for this compiler
#ifndef __STATIC
#define __STATIC static
#endif

/// define size of an empty array (used to declare structure with an array size not defined)
#ifndef __ARRAY_EMPTY
#define __ARRAY_EMPTY
#endif

/// Align instantiated lvalue or struct member on 4 bytes
#define __ALIGN4 __attribute__((aligned(4)))

/// __MODULE__ comes from the RVDS compiler that supports it
#define __MODULE__ __BASE_FILE__

#define __SHAREDRAM __attribute__ ((section("SHAREDRAM")))
#define __HOST_RXBUF __attribute__ ((section("HOST_RXBUF")))
#define __HOST_LPIRAM __attribute__ ((section("HOST_LPIRAM")))
#define XNSTR(s)            NSTR(s)
#define NSTR(s)             #s
/* Host shared memory */
#define __SHARED_HOST_N(m,n) __attribute__ ((section("SHAREDMEM_HOST_" XNSTR(m) "." XNSTR(n))))
#define SHARED_HOST_DECLARE(module, type, name) \
    type backup_ ## name __SHARED_HOST_N(module, name)
#define SHARED_HOST_EXT_STATEMENT(type, name) \
    extern type backup_ ## name
#define SHARED_HOST_ARRAY_DECLARE(module, type, count, name) \
    type backup_ ## name[count] __SHARED_HOST_N(module, name)
#define SHARED_HOST_ARRAY_EXT_STATEMENT(type, count, name) \
    extern type backup_ ## name[count]
#define SHARED_HOST_ARRAY2_DECLARE(module, type, count1, count2, name) \
    type backup_ ## name[count1][count2] __SHARED_HOST_N(module, name)
#define SHARED_HOST_ARRAY2_EXT_STATEMENT(type, count1, count2, name) \
    extern type backup_ ## name[count1][count2]
/* Host private memory, using groups: G0RTOS/G1WLAN/G2BTDM/G3USER */
#define __PRIVATE_HOST_N(m,n) __attribute__ ((section("PRIVATEMEM_HOST_" XNSTR(m) "." XNSTR(n))))
#define PRIVATE_HOST_DECLARE(module, type, name) \
    type backup_ ## name __PRIVATE_HOST_N(module, name)
#define PRIVATE_HOST_EXT_STATEMENT(type, name) \
    extern type backup_ ## name
#define PRIVATE_HOST_ARRAY_DECLARE(module, type, count, name) \
    type backup_ ## name[count] __PRIVATE_HOST_N(module, name)
#define PRIVATE_HOST_ARRAY_EXT_STATEMENT(type, count, name) \
    extern type backup_ ## name[count]
#define PRIVATE_HOST_ARRAY2_DECLARE(module, type, count1, count2, name) \
    type backup_ ## name[count1][count2] __PRIVATE_HOST_N(module, name)
#define PRIVATE_HOST_ARRAY2_EXT_STATEMENT(type, count1, count2, name) \
    extern type backup_ ## name[count1][count2]

#define MCAT(a,b)                   a##b

#define VAR_WITH_VERx(name,v)       MCAT(name ## _U0, v)
#define VAR_WITH_VER(name)          VAR_WITH_VERx(name, CFG_ROM_VER)

#endif // _COMPILER_H_

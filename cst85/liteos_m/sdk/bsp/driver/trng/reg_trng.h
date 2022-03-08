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
#ifndef _REG_TRNG_H_
#define _REG_TRNG_H_

#include "chip.h"

typedef struct
{
  __IO uint32_t                    trng_en;                      //0x00000000
  __IO uint32_t                    trng_status;                  //0x00000004
  __IO uint32_t                    trng_data;                    //0x00000008
  __IO uint32_t Reserved_0000000C;              //0x0000000C
  __IO uint32_t                    trng_div_dly;                 //0x00000010
  __IO uint32_t                    trng_sel;                     //0x00000014
} CS_TRNG_TypeDef;

static CS_TRNG_TypeDef * const CS_TRNG = ((CS_TRNG_TypeDef *)CS_TRNG_BASE);

//trng_en
#define TRNG_STRNGENQ              (1<<2)
#define TRNG_STRNGIEQ              (1<<3)
#define TRNG_STRNGEEQ              (1<<4)

//trng_status
#define TRNG_SDRDYQ                (1<<0)
#define TRNG_SSECSQ                (1<<2)
#define TRNG_SSEISQ                (1<<6)

//trng_data
#define TRNG_SDRQ(n)               (((n)&0xFFFFFFFF)<<0)

//trng_div_dly
#define TRNG_SDIVNUMQ(n)           (((n)&0xFF)<<0)
#define TRNG_SDIVHIGHQ(n)          (((n)&0xFF)<<8)
#define TRNG_SDLYNUMQ(n)           (((n)&0xFF)<<16)

//trng_sel
#define TRNG_STRNGSEL0Q(n)         (((n)&0xFF)<<0)
#define TRNG_STRNGSEL1Q(n)         (((n)&0xFF)<<8)
#define TRNG_SLFSRGEN0Q(n)         (((n)&0xFF)<<16)
#define TRNG_SLFSRGEN1Q(n)         (((n)&0xFF)<<24)

#endif /* _REG_TRNG_H_ */

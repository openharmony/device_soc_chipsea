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
#ifndef _CS1000LITE_GPIO_H_
#define _CS1000LITE_GPIO_H_

#include "chip.h"

/* ========================================================================== */
/* ================             CS1000LITE_GPIO             ================ */
/* ========================================================================== */
#define REG_CS1000LITE_GPIO_BASE  0x50011000

typedef struct {
    __IO uint32_t VR;           /* 0x000 (R/W) : Val Reg */
    __IO uint32_t MR;           /* 0x004 (R/W) : Msk Reg */
    __IO uint32_t DR;           /* 0x008 (R/W) : Dir Reg */
    __IO uint32_t TELR;         /* 0x00C (R/W) : Trig Edg or Lvl Reg */
    __IO uint32_t TER;          /* 0x010 (R/W) : Trig Edg Reg */
    __IO uint32_t TLR;          /* 0x014 (R/W) : Trig Lvl Reg */
    __IO uint32_t ICR;          /* 0x018 (R/W) : Int Ctrl Reg */
    __I  uint32_t RESERVED0;    /* 0x01C (R)   : Reserved */
    __I  uint32_t ISR;          /* 0x020 (R)   : Int Stat Reg */
    __O  uint32_t IRR;          /* 0x024 (W)   : Int Rm Reg */
    __IO uint32_t TIR;          /* 0x028 (R/W) : Trig In Reg */
    __IO uint32_t FR;           /* 0x02C (R/W) : Fltr Reg */
} HWP_CS1000LITE_GPIO_T;

static HWP_CS1000LITE_GPIO_T * const cs1000liteGpio = ((HWP_CS1000LITE_GPIO_T *)REG_CS1000LITE_GPIO_BASE);

#endif /* _CS1000LITE_GPIO_H_ */

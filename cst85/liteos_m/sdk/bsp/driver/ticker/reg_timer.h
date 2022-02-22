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
#ifndef _REG_TIMER_H_
#define _REG_TIMER_H_

#include "chip.h"

/**
 * Timer registers
 */
typedef struct {
  struct {
    __IO uint32_t CTL;          /* 0x000: Control */
    __IO uint32_t TC;           /* 0x004: Timer Counter */
    __IO uint32_t PR;           /* 0x008: Prescale */
    __IO uint32_t PC;           /* 0x00C: Prescale Counter */
    __IO uint32_t MR;           /* 0x010: Match Register */
    __IO uint32_t TD;           /* 0x014: Tick Divider */
    __IO uint32_t IC;           /* 0x018: Interrupt Control */
    __I  uint32_t IS;           /* 0x01C: Interrupt Status */
  } TMR[3];
} CS_TIM_TypeDef;

typedef struct {
  __IO uint32_t CTL;            /* 0x000: Control */
  __IO uint32_t TD;             /* 0x004: Tick Divider */
  __IO uint32_t IC;             /* 0x008: Interrupt Control */
  __I  uint32_t IS;             /* 0x00C: Interrupt Status */
  __IO uint32_t TCL;            /* 0x010: Timer Counter low-32bit */
  __IO uint32_t TCH;            /* 0x014: Timer Counter high-16bit */
       uint32_t RESERVED0[2U];
  struct {
    __IO uint32_t L;            /* 0x020: Match Register low-32bit */
    __IO uint32_t H;            /* 0x024: Match Register high-16bit */
  } MR[4];
} CS_AONTIM_TypeDef;

/**
 * Timer Peripherals
 */
static CS_TIM_TypeDef    * const CS_TIM0 = ((CS_TIM_TypeDef    *)CS_TIM0_BASE);
static CS_TIM_TypeDef    * const CS_TIM1 = ((CS_TIM_TypeDef    *)CS_TIM1_BASE);
static CS_AONTIM_TypeDef * const CS_TIM2 = ((CS_AONTIM_TypeDef *)CS_TIM2_BASE);

#endif /* _REG_TIMER_H_ */

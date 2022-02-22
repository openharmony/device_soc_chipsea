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
 * @file system.h
 *
 ****************************************************************************************
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "chip.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * EXPORTED VARIABLE DECLARATION
 ****************************************************************************************
 */

extern uint32_t DSPSysCoreClock;        /*!< DSP System Clock Frequency (Fclk)  */
extern uint32_t SystemCoreClock;        /*!< System Clock Frequency (Hclk)      */
extern uint32_t PeripheralClock;        /*!< Peripheral Clock Frequency (Pclk)  */
extern uint32_t PsramCntrlClock;        /*!< PSRAM Control Clock Frequency      */

/*
 * EXPORTED FUNCTION DECLARATION
 ****************************************************************************************
 */

void SystemCoreClockUpdate(void);
void SystemInit(void);
uint8_t ChipIdGet(int ch);
uint8_t ChipRomVerGet(void);

#endif // _SYSTEM_H_

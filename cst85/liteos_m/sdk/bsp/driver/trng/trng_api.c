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
 * @file trng_api.c
 *
 * @brief TRNG API functions
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "trng_api.h"
#include "reg_trng.h"
#include "reg_sysctrl.h"
#include "ll.h"
#include "dbg_assert.h"

#define __USE_TRNG_ISR  0

#if __USE_TRNG_ISR
void TRNG_IRQHandler(void)
{
    // clear interrupt
    unsigned int trng_data = CS_TRNG->trng_data;
    trng_data = trng_data; // fix warning
    ASSERT_WARN((CS_TRNG->trng_status & (TRNG_SSECSQ | TRNG_SSEISQ)) == 0x00UL);
}
#endif

void trng_init(void)
{
    // clk en if required
    cpusysctrl_pclkme_set(CSC_PCLKME_TRNG_EN_BIT);
    #if __USE_TRNG_ISR
    NVIC_SetPriority(TRNG_IRQn, __NVIC_PRIO_LOWEST);
    NVIC_EnableIRQ(TRNG_IRQn);
    #endif
}

void trng_free(void)
{
    // clk dis
    #if __USE_TRNG_ISR
    NVIC_DisableIRQ(TRNG_IRQn);
    #endif
}

unsigned int trng_get_word(void)
{
    unsigned int data;
    #if !(__USE_TRNG_ISR)
    unsigned int status;
    #endif

    CS_TRNG->trng_en |= (TRNG_STRNGENQ | TRNG_STRNGIEQ);

    #if __USE_TRNG_ISR
    critical_section_start();
    do {
        __WFI();
    } while (!(CS_TRNG->trng_status & TRNG_SDRDYQ));
    critical_section_end();
    data = CS_TRNG->trng_data;

    #else
    do {
        __WFE();
        status = CS_TRNG->trng_status;
        ASSERT_WARN((status & (TRNG_SSECSQ | TRNG_SSEISQ)) == 0x00UL);
    } while (!(status & TRNG_SDRDYQ));
    // read data to clear trng int, make sure do it before ClearPendingIRQ
    data = CS_TRNG->trng_data;
    __DSB();
    NVIC_ClearPendingIRQ(TRNG_IRQn);
    #endif

    return data;
}

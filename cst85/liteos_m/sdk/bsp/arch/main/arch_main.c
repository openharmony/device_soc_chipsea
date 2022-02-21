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
 * @file arch_main.c
 *
 * @brief Main loop of the application.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "plf.h"       // SW configuration

#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include "boot.h"      // boot definition
#include "time_api.h"

#include "dbg.h"

#include "stdio_uart.h"
#include "system.h"

#if (PLF_CONSOLE)
#include "console.h"
#endif /* PLF_CONSOLE */

#if PLF_TEST
#include "test_main.h"
#endif /* PLF_TEST */

#if PLF_PMIC
#include "pwrkey_api.h"
#endif

/*chipsea_ohos include begin */
#ifdef CFG_LITEOS
#include "rtos.h"
#include "los_arch_interrupt.h"
#include "los_tick.h"
#endif
/*chipsea_ohos include end*/

/*
 * DEFINES
 ****************************************************************************************
 */
/*chipsea_ohos define hard fault irqn begin */
#ifdef CFG_LITEOS
#define HARDFAULT_IRQN                  (-13)
#endif
/*chipsea_ohos define hard fault irqn end*/

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * LOCAL FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/*chipsea_ohos set os isr begin */
#ifdef CFG_LITEOS
static void OsVectorInit(void)
{
    NVIC_SetVector(NonMaskableInt_IRQn, (uint32_t)HalExcNMI);
    NVIC_SetVector(HARDFAULT_IRQN, (uint32_t)HalExcHardFault);
    NVIC_SetVector(MemoryManagement_IRQn, (uint32_t)HalExcMemFault);
    NVIC_SetVector(BusFault_IRQn, (uint32_t)HalExcBusFault);
    NVIC_SetVector(UsageFault_IRQn, (uint32_t)HalExcUsageFault);
    NVIC_SetVector(SVCall_IRQn, (uint32_t)HalExcSvcCall);
    NVIC_SetVector(PendSV_IRQn, (uint32_t)HalPendSV);
    NVIC_SetVector(SysTick_IRQn, (uint32_t)OsTickHandler);
}
#endif
/*chipsea_ohos set os isr end*/

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief RW main function.
 *
 * This function is called right after the booting process has completed.
 *
 * @return status   exit status
 ****************************************************************************************
 */
void rw_main(void)
{
    /*
     ************************************************************************************
     * Platform initialization
     ************************************************************************************
     */
    /*chipsea_ohos set os isr begin*/
#ifdef CFG_LITEOS
     //update vectors
    OsVectorInit();
#endif
    /*chipsea_ohos set os isr end*/

    // Update system clocks
    SystemCoreClockUpdate();

    // Initialize stdio uart
    stdio_uart_init();

    // Embedded modules initialization
    dbg_init();

    // finally start interrupt handling
    GLOBAL_INT_START();

    #if PLF_PMIC
    pwrkey_poweron_check();
    #endif

    dbg("\r\nhost_wb start\r\n");

    cs_time_init(0, 0);

    /*chipsea_ohos os main begin*/
#ifndef CFG_LITEOS
    // Initialize console
    #if (PLF_CONSOLE)
    console_init();
    #endif
    #if PLF_TEST
    test_main();
    #endif /* PLF_TEST */

    /*
     ************************************************************************************
     * Main loop
     ************************************************************************************
     */

    while(1)
    {
        // schedule all pending console commands
        #if (PLF_CONSOLE)
        console_schedule();
        #endif

        GLOBAL_INT_DISABLE();
        #if (PLF_CONSOLE)
        if (console_buf_empty() == 1)
        #endif
        {
            // Wait for interrupt
            __WFI();
        }
        GLOBAL_INT_RESTORE();
    }
#else //CFG_LITEOS
    rtos_main();
#endif
    /*chipsea_ohos os main end*/
}

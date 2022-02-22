/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Description: Provide a task example.
 */

// define CFG_DBG to use dbg interface.
#define CFG_DBG 1

#include "task_sample.h"
//#include "Led_service.h"
#include "los_config.h"
#include "los_debug.h"
#include "los_interrupt.h"
#include "los_task.h"
#include "los_tick.h"
//#include "gpio.h"
#include "dbg.h"
//#include "hdf_log.h"

/*
#define HDF_LED_SERVICE "led_driver"

static struct LedService *GetLedService()
{
    static struct LedService *service = NULL;
    if (service == NULL) {
        struct LedService *LedCntlr = (struct LedService *)DevSvcManagerClntGetService(HDF_LED_SERVICE);
        if (LedCntlr == NULL) {
            HDF_LOGE("failed to get service %s", HDF_LED_SERVICE);
			dbgprint("failed to get service\r\n");
            return NULL;
        }
       // service = (struct LedService *)LedCntlr->priv; // 
	    service = (struct LedService *)LedCntlr; // 
    }
    return service;
}

void LedSet(void)
{
    struct LedService *service = GetLedService();
    if (service == NULL || service->setData == NULL) {
		HDF_LOGE("failed to set data %s", HDF_LED_SERVICE);
		dbgprint("failed to set data\r\n");
        return;
    }
    service->setData();
}
*/



VOID TaskSampleEntry2(VOID)
{
    //led_init();
    while (1) {
        dbgprint("\r\nTaskSampleEntry2 runing\r\n");
        LOS_TaskDelay(1000);
        
    }
}

//LVGL UI界面
VOID TaskSampleEntry1(VOID)
{
	#if 0
    int cnt = 0;
#if 1
    timer1_init();
    
    dbgprint("___>>>>>> %s %d\r\n", __FILE__, __LINE__);
    /* configure the EXMC access mode */
    exmc_lcd_init();
    
    dbgprint("___>>>>>> %s %d\r\n", __FILE__, __LINE__);
    /* configure the GPIO of SPI touch panel */
    touch_panel_gpio_configure();

    dbgprint("___>>>>>> %s %d\r\n", __FILE__, __LINE__);
        /* initialize the LCD */
    lcd_init();

    dbgprint("___>>>>>> %s %d\r\n", __FILE__, __LINE__);
	lv_init(); //lvgl 系统初始化
	lv_port_disp_init(); //lvgl 显示接口初始化,放在 lv_init()的后面
	lv_port_indev_init(); //lvgl 输入接口初始化,放在 lv_init()的后面

	dbgprint("___>>>>>> %s %d\r\n", __FILE__, __LINE__);

	lv_ui_home_demo(500);

    dbgprint("___>>>>>> %s %d\r\n", __FILE__, __LINE__);

    while (1) {
        LOS_TaskDelay(1); /* 2 Seconds */
        lv_task_handler();
        
    }
    #endif
	#endif 
	//DeviceManagerStart();
    //OHOS_SystemInit();
	 while (1) {
        dbgprint("\r\nTaskSampleEntry1 runing\r\n");
		//LedSet();
        LOS_TaskDelay(1000);
        
    }
}

VOID TaskSample(VOID)
{
    UINT32 uwRet;
    UINT32 taskID1;
    UINT32 taskID2;
    TSK_INIT_PARAM_S stTask = {0};

    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskSampleEntry1;
    stTask.uwStackSize = 0x1000;
    stTask.pcName = "TaskSampleEntry1";
    stTask.usTaskPrio = 6; /* Os task priority is 6 */
    uwRet = LOS_TaskCreate(&taskID1, &stTask);
    if (uwRet != LOS_OK) {
        dbgprint("\r\nTask1 create failed\r\n");
    }

    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskSampleEntry2;
    stTask.uwStackSize = 0x1000;
    stTask.pcName = "TaskSampleEntry2";
    stTask.usTaskPrio = 7; /* Os task priority is 7 */
    uwRet = LOS_TaskCreate(&taskID2, &stTask);
    if (uwRet != LOS_OK) {
        dbgprint("\r\nTask2 create failed\r\n");
    }
}

VOID RunTaskSample(VOID)
{
    UINT32 ret;
    ret = LOS_KernelInit();

    dbgprint("\r\n\13510979604\r\n");
    dbgprint("\r\nOpen Harmony 3.0 start ...\r\n\r\n");

    if (ret == LOS_OK) {
        TaskSample();
        
        //启动OpenHarmony OS 内核
		dbgprint("\r\n create task ok\r\n");
		dbgprint("\r\n start task \r\n");
        LOS_Start();
    }
}


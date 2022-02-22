/*
 * Copyright (C) 2018-2020 CSSemi Ltd.
 *
 *
 *  Bt task init and manager in this file .
 */

/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/
#include "rtos.h"
#include "app_ble_only.h"
#include "ble_task_msg.h"
#include "csble.h"
#include "bt_cs8800_drvif.h"
#include "sysctrl_api.h"
#include "ble_task.h"
#include "bt_types_def.h"
#include "cs_ble_adp_api.h"
#include "system.h"

/// Handle of the BLE task
static rtos_task_handle ble_task_handle = NULL;
/// Indicates whether BLE task is suspended or not
static bool ble_task_suspended;
static rtos_queue app_ble_task_queue;
static cs_host_cfg_t cfg = {
    .support_sniff = true,
#if APP_SUPPORT_MULTIPLE_PHONE == 0
    .multiple_phone = false,
#else
    .multiple_phone = true,
#endif
#if (AON_SUPPORT)
    .support_aon = true,
#ifdef APP_SUPPORT_DEEPSLEEP
    .support_sleep = true,
#else
    .support_sleep = false,
#endif
#else
    .support_aon = false,
    .support_sleep = false,
#endif
#ifdef CFG_BTDM_RAM_VER
    .rom_version = 0,
#else
    .rom_version = CFG_ROM_VER,
#endif
};

#define CS_BLE_STACK_SIZE TASK_STACK_SIZE_BLE_TASK_ONLY




uint32_t ble_task_suspend(void)
{
     //TRACE("ble_task_suspend \n");
#if 1   
    uint32_t retvalue = 0;
    GLOBAL_INT_DISABLE();
    ble_task_suspended = true;
    GLOBAL_INT_RESTORE();

    // wait for notification
    retvalue = rtos_task_wait_notification(-1);
    return retvalue;
#endif	
}



void ble_task_resume(bool isr,uint32_t value)
{
    //TRACE("ble_task_resume \n");
    //if (!ble_task_suspended) {
        //return;
    //}
    ble_task_suspended = false;
    //dbg("ble_task_handle = 0x%x\n",ble_task_handle);

    // Notify the bt task
    rtos_task_notify(ble_task_handle, value, isr);
}

/**
 * @brief Main loop for the BLE Task
 */
static RTOS_TASK_FCT(ble_task)
{
    BOOL ret = TRUE;
#if APP_SUPPORT_AES == 1
    uint32_t pwrmd = pwrctrl_pwrmd_cpusys_sw_record_getf();
    if(pwrmd < CPU_SYS_POWER_DOWN) {
        // clk en
        cpusysctrl_hclkme_set(CSC_HCLKME_DMA_EN_BIT);
        bt_drv_generate_secret_key();
        bt_drv_start_generate_key();
        app_ble_handler_register(BLE_HANDLER_REG_0, bt_drv_pool_key_complete);
    }
#endif
    app_ble_init();
    #if (CFG_ROM_VER == 255)
    uint8_t chip_id = ChipIdGet(0);
    if (chip_id == 0x03) {
        cfg.rom_version = 2;
    } else if (chip_id == 0x07) {
        cfg.rom_version = 3;
    }
    #endif
    cs_stack_init_ble_only(cfg);
    ret = cs_adp_stack_config_ble_only();
    if(ret == TRUE){
        GLOBAL_INT_DISABLE();
        cs_bt_start();
        GLOBAL_INT_RESTORE();
        ble_task_set_default_priority();
    }
    ble_task_queue_notify(false);
    for (;;)
    {
        uint32_t msg;
        // Suspend the BLE task while waiting for new events
        rtos_queue_read(app_ble_task_queue,(void *)&msg, -1, 0);
        // schedule all pending events
        cs_stack_loop_ble_only();
        ble_handler_reg_list_poll();
        cs_ble_schedule();
    }
}


void ble_task_queue_notify(bool isr)
{
    bool ret = 0;
    uint32_t msg = 0xff;
    if(rtos_queue_cnt(app_ble_task_queue) < 5){
        ret = rtos_queue_write(app_ble_task_queue,&msg , 0, isr);
        if(isr){
            ret += 0;
            //TRACE(" notify %d \n",ret);
        }
    }
}


int ble_task_init(void)
{
    uint32_t protect;
    app_ble_only_msg_init();

    if (rtos_queue_create(sizeof(uint32_t),5,&app_ble_task_queue)){
        TRACE("BT_TASK: Failed to Create app_bt_queue\n");
        return -1;
    }
    protect = rtos_protect();
    ble_task_suspended = false;
    // Create the BLE task
    if (rtos_task_create(ble_task, "BLE_TASK", BLE_TASK,
                         CS_BLE_STACK_SIZE, NULL, TASK_PRIORITY_MAX, &ble_task_handle)) {
        rtos_queue_delete(app_ble_task_queue);
        return -1;
    }
    //TRACE("ble_task_handle = 0x%x\n",ble_task_handle);
    rtos_unprotect(protect);
    return 0;
}


int ble_task_deinit(void)
{
    cs_adp_ble_deinit();
    return 0;
}

int ble_task_delete(void)
{
    cs_adp_free_stack_memory_ble_only();
    //TRACE("ble_task_deinit ble_task_handle = 0x%x\n",ble_task_handle);
    app_ble_only_msg_deinit();
    if(app_ble_task_queue){
        rtos_queue_delete(app_ble_task_queue);
        app_ble_task_queue = NULL;
    }
    if(ble_task_handle){
        rtos_task_delete(ble_task_handle);
        ble_task_handle = NULL;
    }
    return 0;
}

void ble_task_set_max_priority(void)
{
    rtos_task_set_priority(ble_task_handle, TASK_PRIORITY_MAX);
}

void ble_task_set_default_priority(void)
{
    rtos_task_set_priority(ble_task_handle, TASK_PRIORITY_BT_TASK);
}


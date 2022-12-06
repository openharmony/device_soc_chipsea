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
/**
 ****************************************************************************************
 *
 * @file rtos.c
 *
 * @brief Entry point for WiFi stack integration within an RTOS.
 *
 ****************************************************************************************
 */
#include "al_rtos.h"
#include "dbg.h"

#if PLF_CONSOLE
#include "console_task.h"
#endif /* PLF_CONSOLE */

#if PLF_LETTER_SHELL
#include "shell_port.h"
#endif /* PLF_LETTER_SHELL */

#if PLF_ASIO
#include "asio.h"
#endif /* PLF_ASIO */

#if PLF_AUDIO
#include "app_audio.h"
#include "audio_eq.h"
#include "audio_drc.h"
#endif /* PLF_AUDIO */

#if PLF_TEST
#include "test_main.h"
#endif /* PLF_TEST */

#if PLF_BT_STACK
#ifndef CFG_BLE_ONLY
#include "bt_task.h"
#else
#include "ble_task.h"
#endif
#ifdef CFG_AON
#include "cs_bt_aon_sram.h"
#endif
#endif /* PLF_BT_STACK */

#if PLF_WIFI_STACK
#ifdef CFG_HOSTIF
#include "hostif.h"
#endif
#ifdef CONFIG_RWNX_LWIP
#include "porting_net_al.h"
#include "wifi_host.h"
#include "wifi_host_wpa.h"
#ifdef CFG_HOSTIF
#include "rwnx_defs.h"
struct rwnx_hw hw_env;
#endif /* CFG_HOSTIF */
#endif /* CONFIG_RWNX_LWIP */
#if !defined(CFG_HOSTIF) || defined(CFG_DEVICE_IPC)
#include "rwnx_defs.h"
struct rwnx_hw hw_env;
#endif
#endif /* PLF_WIFI_STACK */

#include "co_main.h"

#if PLF_DSP
#include "dsp_task.h"
#endif /* PLF_DSP */

#if PLF_KEY_MODULE
#include "key.h"
#endif
#if PLF_LED_MODULE
#include "led.h"
#endif
#if PLF_LS_MODULE
#include "light_sensor.h"
#endif

#include "sysctrl_api.h"
#include "sleep_api.h"
#include "cmsis_os2.h"

extern int fhost_application_smartconfig_init(void);
rtos_task_cfg_st get_task_cfg(uint8_t task_id)
{
    rtos_task_cfg_st cfg = {0, 0};

    switch (task_id) {
        case IPC_CNTRL_TASK:
            cfg.priority   = TASK_PRIORITY_WIFI_IPC;
            cfg.stack_size = TASK_STACK_SIZE_WIFI_IPC;
            break;
        case SUPPLICANT_TASK:
            cfg.priority   = TASK_PRIORITY_WIFI_WPA;
            cfg.stack_size = TASK_STACK_SIZE_WIFI_WPA;
            break;
        case CONTROL_TASK:
            cfg.priority   = TASK_PRIORITY_WIFI_CNTRL;
            cfg.stack_size = TASK_STACK_SIZE_WIFI_CNTRL;
            break;
        case APP_FHOST_TX_TASK:
            cfg.priority   = TASK_PRIORITY_WIFI_TX;
            cfg.stack_size = TASK_STACK_SIZE_WIFI_TX;
            break;
        default:
            break;
    }

    return cfg;
}

/**
 * Save user data that declared with PRIVATE_HOST_*(G3USER)
 */
__WEAK void user_data_save(void)
{
    // VOID
}

/**
 * Restore user data that declared with PRIVATE_HOST_*(G3USER)
 */
__WEAK void user_data_restore(void)
{
    // VOID
}

void timer_callback(void *arg) {
    printf("%s: begin\n", __func__);
    OHOS_SystemInit();
}

void rtos_main(void)
{
    dbg("Enter rtos_main ok\r\n");
    //uart_puts("M\r\n");
    if(LOS_KernelInit())
    {
        dbg("LOS_KernelInit\r\n");
    }
    #if (PLF_HW_PXP == 1)
    dbg("RUNNING IN SIMULATION MODE\r\n");
    #endif

    #if DBG_MUTEX_ENABLED
    dbg_rtos_init();
    #endif

    co_main_init();

    #if PLF_KEY_MODULE
    key_open();
    #endif

    #if PLF_LED_MODULE
    led_init();
    #endif

    #if PLF_LS_MODULE
    ls_init();
    #endif

    #if PLF_WIFI_STACK
    #if defined(CONFIG_RWNX_LWIP)
    #ifndef CFG_WIFI_RAM_VER
    wifi_patch_prepare();
    #endif
    rwnx_ipc_init(&hw_env, &ipc_shared_env);
    #endif /* CONFIG_RWNX_LWIP */
    #if defined(CFG_DEVICE_IPC)
    rwnx_ipc_init(&hw_env, &ipc_shared_env);
    #endif
    #endif /* PLF_WIFI_STACK */

    #if (PLF_CONSOLE && !(PLF_WIFI_STACK && (defined(CFG_APP_CONSOLEWIFI) || defined(CFG_APP_UARTWIFI))))
    console_task_init();
    #endif /* PLF_CONSOLE && !PLF_WIFI_STACK */

    #if PLF_LETTER_SHELL
    userShellInit();
    #endif

    #if PLF_AUD_USED
    #if PLF_ASIO
    asio_init();
    #endif

    #if PLF_AUDIO
    app_audio_open();
    audio_eq_init();
    audio_drc_init();
    #if PLF_CONSOLE
    audio_eq_cmd_init();
    audio_drc_cmd_init();
    #endif
    #endif
    #endif

    #if PLF_TEST
    test_task_init();
    #endif /* PLF_TEST */

    #if PLF_BT_STACK
    #if !(defined(CFG_TEST_AF) || defined(CFG_TEST_SBC) || defined(CFG_TEST_AAC) || defined(CFG_TEST_SDCARD_AUDIO) || defined(CFG_TEST_HCI))
    #ifdef CFG_AON
    host_aon_interface_init();
    #endif
    #ifndef CFG_BLE_ONLY
   // bt_task_init();
    #else
    ble_task_init();
    #endif
    #ifdef CFG_AON
    cpup_ready_set(true);
    #endif
    #endif
    #endif /* PLF_BT_STACK */

    #ifdef CFG_HOSTIF
    init_host(0);
    #endif
    #if PLF_WIFI_STACK
    #ifdef CONFIG_RWNX_LWIP
    net_init();
    // Initialize the FHOST module
    fhost_init(NULL);
    #endif /* CONFIG_RWNX_LWIP */
    #endif /* PLF_WIFI_STACK */
    // if (pwrctrl_pwrmd_cpusys_sw_record_getf() >= CPU_SYS_POWER_DOWN) {
    //     // restore data
    //     sleep_data_restore();
    //     #if PLF_WIFI_STACK
    //     #ifdef CONFIG_RWNX_LWIP
    //     fhost_data_restore();
    //     lwip_data_restore();
    //     if (wlan_connected) {
    //         fhost_sta_ipc_rxbuf_recover();
    //     }
    //     //wpas_data_restore();
    //     #endif /* CONFIG_RWNX_LWIP */
    //     #endif /* PLF_WIFI_STACK */
    //    // user_data_restore();
    //     sys_wakeup_indicate();
    // }

    #if PLF_DSP
    dsp_task_init();
    #endif

    /*chipsea_ohos start device manager and init system , begin*/
    init_trace_system();
    DeviceManagerStart();

    osTimerId_t timerId;
    unsigned int timerExec = 5000;
    osStatus_t status;
    timerId = osTimerNew((osTimerFunc_t)timer_callback, osTimerOnce, &timerExec, NULL );
    if (timerId != NULL) {
        status = osTimerStart(timerId, 5000U);
        if (status != osOK) {
            printf("Failed to start timer!\n");
        }
    } else {
        printf("Failed to create timer!\n");
    }
#ifdef CONFIG_FS_LITTLEFS
    //hal_vfs_init();
#endif
    /*chipsea_ohos start device manager and init system , end*/

    // Start the scheduler

    dbg("\r\n start task \r\n");
    LOS_Start();


    // Should never reach here
    for( ;; );
}

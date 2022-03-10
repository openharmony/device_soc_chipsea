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
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/
#include "cs_target_config.h"
#if APP_SUPPORT_SPP == 1
#include "cs_adp_api.h"
#include "rtos.h"
#include "app_spp.h"
#include "app_test.h"
#include "app_bt.h"

static app_spp_tx_done_t app_spp_tx_done_func = NULL;
#if MAX_THROUGHPUT_TEST == 1
uint8_t spp_test_buffer[650] = {0x1,};

void app_spp_send_test_data(void)
{
    //TRACE("APP:spp_send_test_data\n");
    app_spp_send_data(spp_test_buffer, 650);
}

void app_spp_connect_test(void)
{
    BT_ADDR temp_bdaddr = {0xa1,0xc0,0x00,0x45,0x56,0x67};
    //0x77,0xEE,0xAE,0x77,0xAE,0xAE
    //0xAE,0xAE,0x77,0xAE,0xEE,0x77
    app_spp_connect(&temp_bdaddr);
}

#endif


void app_spp_send_data(uint8_t* ptrData, uint16_t length)
{
    cs_adp_spp_send_data(ptrData, length);
}


void app_spp_register_tx_done(app_spp_tx_done_t callback)
{
    TRACE("APP:register tx done cb = 0x%x\n",callback);
    app_spp_tx_done_func = callback;
}

void app_spp_init(void)
{
    TRACE("APP:app_spp_init\n");
#if MAX_THROUGHPUT_TEST == 1
    app_spp_register_tx_done(app_spp_send_test_data);
#endif
}

void app_spp_connect(BT_ADDR * bdaddr)
{
    app_spp_init();
    TRACE("APP:app_spp_connect \n");
    cs_adp_spp_connect(bdaddr);
}

void app_spp_msg_handle(CS_EVENT *event)
{
    CS_ADP_SPP_EVENT *cs_spp_msg = (CS_ADP_SPP_EVENT *)event->Param;

    switch(event->EventId){
        case CS_ADP_SPP_CONNECTED_IND:
            if(cs_spp_msg->sppparam == 0){
                TRACE("APP:spp connect ind\n");
                app_bt_set_scan_mode(BT_NOSCAN);
            }else{

            }
            
        break;
        case CS_ADP_SPP_CONNECTED_CNF:
            if(cs_spp_msg->sppparam == 0){
                TRACE("APP:spp connect cnf\n");
                app_bt_set_scan_mode(BT_NOSCAN);
#if MAX_THROUGHPUT_TEST == 1 && TEST_MODE_ROLE_SEND == 1
                app_spp_send_test_data();
#endif
#if SCO_CVSD_PLC_TEST == 1 && TEST_MODE_ROLE_SEND == 1
                app_test_connect_sco(NULL);
#endif
            }else{
            
            }
        break;
        case CS_ADP_SPP_DISCONNECTED:
            TRACE("APP:spp disconnected reason = %d",cs_spp_msg->sppparam);
            if(cs_spp_msg->sppparam == 0){
                  app_spp_tx_done_func = NULL;
            }else{
            
            }
        break;
        case CS_ADP_SPP_DATA_SENT:
            {
                //adpspptxdone *tx_done = (adpspptxdone *)cs_spp_msg->sppparam;
                if(app_spp_tx_done_func)
                {
                    app_spp_tx_done_func();
                }
            }
        break;
        case CS_ADP_SPP_DATA_IND:
            {
                adpspprxdone *rx_done = (adpspprxdone *)cs_spp_msg->sppparam;
                if(*rx_done->pRxBuf != 0)
                {
                    TRACE("APP:spp_receive_data");
                }
            }
        break;
        default:
        break;
    }
}

#endif

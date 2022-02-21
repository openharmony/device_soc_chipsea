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
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/
#include "cs_target_config.h"
#if APP_SUPPORT_HSP == 1 
#include "cs_adp_api.h"
#include "rtos.h"
#include "app_bt.h"

BOOL app_hsp_key_handle(uint32_t key)
{
    BOOL ret = FALSE;
    APP_DEVLIST * hfp_devinfo = NULL;

    hfp_devinfo = cs_adp_get_hsp_current_devinfo();
    if(hfp_devinfo == NULL){
        return ret;
    }

    switch(key){
        case APP_KEY_PLAY|APP_KEY_PRESS:
            
            break;
        case APP_KEY_VOLADD|APP_KEY_PRESS:
        case APP_KEY_VOLADD|APP_KEY_HOLD:
        case APP_KEY_VOLSUB|APP_KEY_PRESS:
        case APP_KEY_VOLSUB|APP_KEY_HOLD:

            break;
        case APP_KEY_PLAY|APP_KEY_DOUBLE_CLICK:
            break;
        case APP_KEY_PLAY|APP_KEY_HOLD:
            break;
        default:
            break;
    }
    return ret;
}


void app_hsp_msg_handle(CS_EVENT *event)
{
    CS_ADP_HFP_EVENT *cs_hfp_msg = (CS_ADP_HFP_EVENT *)event->Param;

    TRACE("APP:app_hsp_msg_handle event = %d",event->EventId);

    switch(event->EventId)
    {
        case CS_ADP_HSP_CONNECTED:
            if(cs_hfp_msg->hfpparam == 0){

            }
            break;
        case CS_ADP_HSP_DISCONNECTED:
            if(cs_hfp_msg->hfpparam == 0){

            }else{

            }
            break;
        case CS_ADP_HSP_AUDIO_CONNECTED:
            if(cs_hfp_msg->hfpparam == 0){

            }else{

            }
            break;
        case CS_ADP_HSP_AUDIO_DISCONNECTED:
            if(cs_hfp_msg->hfpparam == 0){

            }else{

            }
            break;
        case CS_ADP_HSP_AUDIO_DATA_SENT:

            break;
        case CS_ADP_HSP_AUDIO_DATA_IND:
            {
                CS_ADP_HFP_AUDIO_DATA *audiodata = (CS_ADP_HFP_AUDIO_DATA *)cs_hfp_msg->hfpparam;
                //cs_adp_hfp_send_sco_data(Chan, &hf_sendbuff_ctrl.mempool[idx].packet);
            }
            break;
        case CS_ADP_HSP_RING_IND:

            break;
        case CS_ADP_HSP_SPEAKER_VOLUME:
            {
                U8 volume = (uint8_t)cs_hfp_msg->hfpparam;

            }
            break;

    }
}

#endif

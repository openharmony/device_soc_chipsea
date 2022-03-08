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
 * chipsea_ohos add for prefix proguard.
 */
#ifndef __CS_PROGUARD_H
#define __CS_PROGUARD_H

#define PREFIX_1 ai
#define PREFIX_2 c
#define PREFIX_UPER_1 AI
#define PREFIX_UPER_2 C

// 3-part macro to function/variable/enum string
#define PRE_PROGUARD_P3_I(p0,p1,p2)    p0##p1##p2
#define PRE_PROGUARD_P3(p0,p1,p2)      PRE_PROGUARD_P3_I(p0, p1, p2)
// 4-part macro to function/variable/enum string
#define PRE_PROGUARD_P4_I(p0,p1,p2,p3)    p0##p1##p2##p3
#define PRE_PROGUARD_P4(p0,p1,p2,p3)      PRE_PROGUARD_P4_I(p0, p1, p2)

/*chipsea_ohos proguard prefix begin*/

#define cs_adp_a2dp_connect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_connect)
#define cs_adp_a2dp_disconnect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_disconnect)
#define cs_adp_a2dp_get_audio_cap_by_addr PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_get_audio_cap_by_addr)
#define cs_adp_a2dp_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_init)
#define cs_adp_a2dp_register_sink_aac PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_register_sink_aac)
#define cs_adp_a2dp_register_sink_sbc PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_register_sink_sbc)
#define cs_adp_a2dp_register_source PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_register_source)
#define cs_adp_a2dp_send_sbc_data PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_send_sbc_data)
#define cs_adp_a2dp_source_connect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_source_connect)
#define cs_adp_a2dp_source_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_source_init)
#define cs_adp_a2dp_source_start PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_source_start)
#define cs_adp_a2dp_source_suspend PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_source_suspend)
#define cs_adp_a2dp_support_cb PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_a2dp_support_cb)
#define cs_adp_avrcp_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_init)
#define cs_adp_avrcp_register PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_register)
#define cs_adp_avrcp_send_key_backward PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_send_key_backward)
#define cs_adp_avrcp_send_key_forward PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_send_key_forward)
#define cs_adp_avrcp_send_key_pause PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_send_key_pause)
#define cs_adp_avrcp_send_key_play PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_send_key_play)
#define cs_adp_avrcp_send_key_stop PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_send_key_stop)
#define cs_adp_avrcp_send_key_volume_down PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_send_key_volume_down)
#define cs_adp_avrcp_send_key_volume_up PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_send_key_volume_up)
#define cs_adp_avrcp_send_volume_report PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_send_volume_report)
#define cs_adp_avrcp_support_volume_sync PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_avrcp_support_volume_sync)
#define cs_adp_ble_deinit PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_ble_deinit)
#define cs_adp_check_cmp_twsaddr PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_check_cmp_twsaddr)
#define cs_adp_close_dev PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_close_dev)
#define cs_adp_connect_tws_peer_device PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_connect_tws_peer_device)
#define cs_adp_disconnect_acl PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_disconnect_acl)
#define cs_adp_disconnect_tws PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_disconnect_tws)
#define cs_adp_dispatch_msg PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_dispatch_msg)
#define cs_adp_erased_flash PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_erased_flash)
#define cs_adp_free_stack_memory_ble_only PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_free_stack_memory_ble_only)
#define cs_adp_get_a2dp_current_devinfo PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_get_a2dp_current_devinfo)
#define cs_adp_get_app_bt_mgr_ptr PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_get_app_bt_mgr_ptr)
#define cs_adp_get_current_activeCons PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_get_current_activeCons)
#define cs_adp_get_devinfo_by_addr PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_get_devinfo_by_addr)
#define cs_adp_get_hfp_current_devinfo PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_get_hfp_current_devinfo)
#define cs_adp_get_native_clk_plus_offset PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_get_native_clk_plus_offset)
#define cs_adp_get_slave_bt_offset PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_get_slave_bt_offset)
#define cs_adp_hf_connect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hf_connect)
#define cs_adp_hf_disconnect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hf_disconnect)
#define cs_adp_hfg_connect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfg_connect)
#define cs_adp_hfp_battery_report_proc PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_battery_report_proc)
#define cs_adp_hfp_call_answer PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_call_answer)
#define cs_adp_hfp_call_hold PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_call_hold)
#define cs_adp_hfp_call_redial PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_call_redial)
#define cs_adp_hfp_call_release PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_call_release)
#define cs_adp_hfp_disable_NREC PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_disable_NREC)
#define cs_adp_hfp_get_active_prf PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_get_active_prf)
#define cs_adp_hfp_get_negotiated_codec_by_addr PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_get_negotiated_codec_by_addr)
#define cs_adp_hfp_get_scohandle_by_addr PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_get_scohandle_by_addr)
#define cs_adp_hfp_get_voicerecognition_state PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_get_voicerecognition_state)
#define cs_adp_hfp_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_init)
#define cs_adp_hfp_list_current_calls PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_list_current_calls)
#define cs_adp_hfp_register PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_register)
#define cs_adp_hfp_send_at_cclk PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_send_at_cclk)
#define cs_adp_hfp_send_raw_data PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_send_raw_data)
#define cs_adp_hfp_send_sco_data PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_send_sco_data)
#define cs_adp_hfp_send_siri_report PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_send_siri_report)
#define cs_adp_hfp_send_voice_recognition PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_send_voice_recognition)
#define cs_adp_hfp_send_volume PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_send_volume)
#define cs_adp_hfp_set_battery_level PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hfp_set_battery_level)
#define cs_adp_hf_sco_connect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hf_sco_connect)
#define cs_adp_hf_sco_disconnect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hf_sco_disconnect)
#define cs_adp_hf_sco_state PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_hf_sco_state)
#define cs_adp_host_control_tws_sync_con PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_host_control_tws_sync_con)
#define cs_adp_host_m_s_switch PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_host_m_s_switch)
#define cs_adp_host_sync_stop PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_host_sync_stop)
#define cs_adp_inquiry PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_inquiry)
#define cs_adp_inquiry_cancel PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_inquiry_cancel)
#define cs_adp_mgr_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_mgr_init)
#define cs_adp_register_app_msg_handle PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_register_app_msg_handle)
#define cs_adp_role_switch PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_role_switch)
#define cs_adp_rw_timer_notify PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_rw_timer_notify)
#define cs_adp_save_flash PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_save_flash)
#define cs_adp_sco_switch PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_sco_switch)
#define cs_adp_send_data PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_send_data)
#define cs_adp_send_generate_key PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_send_generate_key)
#define cs_adp_send_generate_key_ble_only PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_send_generate_key_ble_only)
#define cs_adp_send_hci_data_direct PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_send_hci_data_direct)
#define cs_adp_send_hci_data_direct_ble_only PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_send_hci_data_direct_ble_only)
#define cs_adp_set_audio_sync_param PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_set_audio_sync_param)
#define cs_adp_set_bt_name PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_set_bt_name)
#define cs_adp_set_linkpolicy PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_set_linkpolicy)
#define cs_adp_set_lp_level PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_set_lp_level)
#define cs_adp_set_lp_level_ble_only PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_set_lp_level_ble_only)
#define cs_adp_set_scan_mode PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_set_scan_mode)
#define cs_adp_set_sniff_timer PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_set_sniff_timer)
#define cs_adp_spp_connect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_spp_connect)
#define cs_adp_spp_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_spp_init)
#define cs_adp_spp_send_data PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_spp_send_data)
#define cs_adp_stack_config PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_stack_config)
#define cs_adp_stack_config_ble_only PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_stack_config_ble_only)
#define cs_adp_stop_sniff PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_stop_sniff)
#define cs_adp_test_connect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_test_connect)
#define cs_adp_test_connect_sco PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_test_connect_sco)
#define cs_adp_test_disconnect PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_test_disconnect)
#define cs_adp_test_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_test_init)
#define cs_adp_test_send_data PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_test_send_data)
#define cs_adp_test_send_sco_data PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_test_send_sco_data)
#define cs_adp_tws_clear_whitelist PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_tws_clear_whitelist)
#define cs_adp_tws_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_tws_init)
#define cs_adp_tws_restart PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_tws_restart)
#define cs_adp_tws_set_scan_disable PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_tws_set_scan_disable)
#define cs_adp_tws_set_scan_para PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_tws_set_scan_para)
#define cs_adp_tws_start_adv PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_tws_start_adv)
#define cs_adp_tws_stop_adv PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_tws_stop_adv)
#define cs_adp_update_tws_flash_role PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _adp_update_tws_flash_role)
#define csble_dbgarea PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, ble_dbgarea)
#define csble_dbg_area PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, ble_dbg_area)
#define csble_dbg_hex_dump PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, ble_dbg_hex_dump)
#define csble_dbg_level PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, ble_dbg_level)
#define cs_ble_deinit PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _ble_deinit)
#define csble_dump_area PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, ble_dump_area)
#define cs_ble_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _ble_init)
#define cs_ble_reset PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _ble_reset)
#define cs_ble_schedule PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _ble_schedule)
#define cs_ble_stack_allow_sleep PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _ble_stack_allow_sleep)
#define cs_ble_tx_rx_allow_sleep PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _ble_tx_rx_allow_sleep)
#define cs_bt_start PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _bt_start)
#define cs_change_host_log_level PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _change_host_log_level)
#define cs_get_ecc_generate_key_ready PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _get_ecc_generate_key_ready)
#define cs_host_log PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _host_log)
#define cs_stack_init PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _stack_init)
#define cs_stack_init_ble_only PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _stack_init_ble_only)
#define cs_stack_loop PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _stack_loop)
#define cs_stack_loop_ble_only PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _stack_loop_ble_only)
#define cs_time_get PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _time_get)
#define cs_tws_get_porttype PRE_PROGUARD_P3(PREFIX_1, PREFIX_2, _tws_get_porttype)
/*chipsea_ohos proguard prefix end*/

#endif //__CS_PROGUARD_H

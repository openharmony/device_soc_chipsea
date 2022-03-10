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
#include "plf.h"
#include "system.h"
#include "bt_hci.h"
#include "bt_cs8800_drv_config.h"
#include "dbg.h"

#define CSBT_FW_DEFAULT_START_ADDR              0x80000
#define CSBT_FW_RAM_START_ADDR                  0x100000

static uint32_t bt_fw_start_addr = CSBT_FW_DEFAULT_START_ADDR;
uint32_t bt_fw_patch_addr = 0;
static uint32_t bt_hci_ram_base[BT_HCI_CH_NUM] = {0,};
static struct csbt_patch_init_desc patch_init_desc;

/*
 * set bt fw addr
 * @param addr : if value is 0 or this function is not used, it will use default fw addr;
 */
void bt_common_set_defalut_fw_addr(uint32_t addr)
{
    if(addr && addr != bt_fw_start_addr){
        bt_fw_start_addr = addr;
        TRACE("fw_start_addr = 0x%x\n",bt_fw_start_addr);
    }
}

uint32_t bt_common_get_defalut_fw_addr(void)
{
    return bt_fw_start_addr;
}

/*
 * set bt hci ipc ram base
 * @param ram_base[BT_HCI_CH_NUM] : if value is 0 or this function is not used, it will use default ipc ram base;
 */
void bt_common_set_default_ram_base(uint32_t *ram_base)
{
    for(uint8_t i = 0; i < BT_HCI_CH_NUM; i++){
        if (ram_base[i]) {
            bt_hci_ram_base[i] = ram_base[i];
            TRACE("hci_ram_base %x = 0x%x\n",i,bt_hci_ram_base[i]);
        }
    }
}

uint32_t *bt_common_get_default_ram_base(void)
{
    return &bt_hci_ram_base[0];
}

struct csbt_patch_init_desc *bt_common_get_patch_init_desc(void)
{
    struct csbt_patch_init_desc *desc;

#ifdef CFG_BTDM_RAM_VER
    //TODO
    desc = NULL;
#else
    uint8_t rom_ver;
#if (CFG_ROM_VER == 255)
    rom_ver = ChipRomVerGet();
#else
    rom_ver = CFG_ROM_VER;
#endif

    switch (rom_ver) {
    case 0x02: {
        desc = &patch_init_desc;
        desc->chip_id = CSBT_CHIP_ID_U02;
        desc->adid_size = CSBT_ADID_SIZE_U02;
        desc->adid_ram_base_addr = (uint8_t *)CSBT_ADID_RAM_BASE_ADDR_U02;
        desc->adid_rom_base_addr = (uint8_t *)CSBT_ADID_ROM_BASE_ADDR_U02;
        desc->patch_base_addr = (uint8_t *)CSBT_PATCH_BASE_ADDR_U02;
        desc->patch_data_ptr = (uint8_t *)CSBT_PATCH_DATA_PTR_U02;
        desc->patch_data_size = CSBT_PATCH_DATA_SIZE_U02;
        desc->patch_table_ptr = (uint8_t *)CSBT_PATCH_TABLE_PTR_U02;
        desc->patch_table_size = CSBT_PATCH_TABLE_SIZE_U02;
        desc->head = NULL;
        break;
    }
    case 0x03: {
        desc = &patch_init_desc;
        desc->chip_id = CSBT_CHIP_ID_U03;
        desc->adid_size = CSBT_ADID_SIZE_U03;
        desc->adid_ram_base_addr = (uint8_t *)CSBT_ADID_RAM_BASE_ADDR_U03;
        desc->adid_rom_base_addr = (uint8_t *)CSBT_ADID_ROM_BASE_ADDR_U03;
        desc->patch_base_addr = (uint8_t *)CSBT_PATCH_BASE_ADDR_U03;
        desc->patch_data_ptr = (uint8_t *)CSBT_PATCH_DATA_PTR_U03;
        desc->patch_data_size = CSBT_PATCH_DATA_SIZE_U03;
        desc->patch_table_ptr = (uint8_t *)CSBT_PATCH_TABLE_PTR_U03;
        desc->patch_table_size = CSBT_PATCH_TABLE_SIZE_U03;
        desc->head = NULL;
        break;
    }
    case 0x04: {
        desc = &patch_init_desc;
        desc->chip_id = CSBT_CHIP_ID_U04;
        desc->adid_size = CSBT_ADID_SIZE_U04;
        desc->adid_ram_base_addr = (uint8_t *)CSBT_ADID_RAM_BASE_ADDR_U04;
        desc->adid_rom_base_addr = (uint8_t *)CSBT_ADID_ROM_BASE_ADDR_U04;
        desc->patch_base_addr = (uint8_t *)CSBT_PATCH_BASE_ADDR_U04;
        desc->patch_data_ptr = (uint8_t *)CSBT_PATCH_DATA_PTR_U04;
        desc->patch_data_size = CSBT_PATCH_DATA_SIZE_U04;
        desc->patch_table_ptr = (uint8_t *)CSBT_PATCH_TABLE_PTR_U04;
        desc->patch_table_size = CSBT_PATCH_TABLE_SIZE_U04;
        desc->head = NULL;
        break;
    }
    default:
        desc = NULL;
        break;
    }
#endif

    return desc;
}

/*
 * bt_common_change_fw_load_in_ram
 * only used for debug fw in ram addr ,fw in rom will mask this function.
 */
void bt_common_change_fw_load_in_ram(void)
{
    uint32_t mb_base[BT_HCI_CH_NUM] = {(0x00170000 + 0x10000 - 0x100)};
    bt_common_set_default_ram_base(&mb_base[0]);
    bt_common_set_defalut_fw_addr(CSBT_FW_RAM_START_ADDR);
}



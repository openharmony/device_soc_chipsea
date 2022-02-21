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
#ifndef __BT_CS8800_DRV_CONFIG_H__
#define  __BT_CS8800_DRV_CONFIG_H__
#include "compiler.h"
#include "reg_access.h"

#include "fw_patch_u02.h"
#include "fw_patch_table_u02.h"
#include "fw_patch_u03.h"
#include "fw_patch_table_u03.h"

#define CSBT_CHIP_ID_U02                      0x03
#define CSBT_CHIP_ID_U03                      0x07
#define CSBT_CHIP_ID_U04                      CSBT_CHIP_ID_U03

#ifdef CFG_BT_PATCH_TWS
#define CSBT_PATCH_BASE_ADDR_U02              0x100000
#define CSBT_PATCH_BASE_ADDR_U03              0x100000
#define CSBT_PATCH_BASE_ADDR_U04              CSBT_PATCH_BASE_ADDR_U03
#else
#define CSBT_PATCH_BASE_ADDR_U02              0x16AD64
#define CSBT_PATCH_BASE_ADDR_U03              0x16AD70
#define CSBT_PATCH_BASE_ADDR_U04              CSBT_PATCH_BASE_ADDR_U03
#endif

#define CSBT_ADID_RAM_BASE_ADDR_U02           0x00161928
#define CSBT_ADID_ROM_BASE_ADDR_U02           0x0004d868
#define CSBT_ADID_SIZE_U02                    0x4b4
#define CSBT_PATCH_DATA_PTR_U02               (uint8_t *)(&fw_patch_u02[0])
#define CSBT_PATCH_DATA_SIZE_U02              CSBT_FW_PATCH_U02_SIZE
#define CSBT_PATCH_TABLE_PTR_U02              (uint8_t *)(&fw_patch_table_u02[0])
#define CSBT_PATCH_TABLE_SIZE_U02             CSBT_FW_PATCH_TABLE_U02_SIZE

#define CSBT_ADID_RAM_BASE_ADDR_U03           0x00161928
#define CSBT_ADID_ROM_BASE_ADDR_U03           0x0004d868
#define CSBT_ADID_SIZE_U03                    0x4b8
#define CSBT_PATCH_DATA_PTR_U03               (uint8_t *)(&fw_patch_u03[0])
#define CSBT_PATCH_DATA_SIZE_U03              CSBT_FW_PATCH_U03_SIZE
#define CSBT_PATCH_TABLE_PTR_U03              (uint8_t *)(&fw_patch_table_u03[0])
#define CSBT_PATCH_TABLE_SIZE_U03             CSBT_FW_PATCH_TABLE_U03_SIZE

#define CSBT_ADID_RAM_BASE_ADDR_U04           CSBT_ADID_RAM_BASE_ADDR_U03
#define CSBT_ADID_ROM_BASE_ADDR_U04           CSBT_ADID_ROM_BASE_ADDR_U03
#define CSBT_ADID_SIZE_U04                    CSBT_ADID_SIZE_U03
#define CSBT_PATCH_DATA_PTR_U04               CSBT_PATCH_DATA_PTR_U03
#define CSBT_PATCH_DATA_SIZE_U04              CSBT_PATCH_DATA_SIZE_U03
#define CSBT_PATCH_TABLE_PTR_U04              CSBT_PATCH_TABLE_PTR_U03
#define CSBT_PATCH_TABLE_SIZE_U04             CSBT_PATCH_TABLE_SIZE_U03


#define CSBT_TABLE_TYPE_TRAP_TABLE             0x01
#define CSBT_TABLE_TYPE_PATCH_B4_TABLE         0x02
#define CSBT_TABLE_TYPE_PATCH_BT_MODE          0x03
#define CSBT_TABLE_TYPE_POWER_ON_TABLE         0x04
#define CSBT_TABLE_TYPE_PATCH_AF_TABLE         0x05
#define CSBT_TABLE_TYPE_PATCH_VER_INFO         0x06

typedef struct csbt_patch_table_desc
{
    char         table_tag[16];
    uint32_t     type;
    uint32_t     len;
    uint32_t     *data;
} APTD;

struct csbt_patch_table_desc_hdr
{
    APTD desc;
    APTD *next;
};

#define CSBT_PT_TAG_SIZE                   16
#define CSBT_PT_TAG                        "CSBT_PT_TAG"
#define CSBT_PT_TRAP_TAG                   "CSBT_TRAP_T";
#define CSBT_PT_PATCH_TB4_TAG              "CSBT_PATCH_TB4";
#define CSBT_PT_MODE_TAG                   "CSBT_MODE_T";
#define CSBT_PT_PWRON_TAG                  "CSBT_POWER_ON";
#define CSBT_PT_PATCH_TAF_TAG              "CSBT_PATCH_TAF";


struct csbt_patch_init_desc
{
    uint16_t     chip_id;
    uint16_t     adid_size;
    uint8_t     *adid_ram_base_addr;
    uint8_t     *adid_rom_base_addr;
    uint8_t     *patch_base_addr;
    uint8_t     *patch_data_ptr;
    uint32_t     patch_data_size;
    uint8_t     *patch_table_ptr;
    uint32_t     patch_table_size;
    struct csbt_patch_table_desc_hdr *head;
};


#endif

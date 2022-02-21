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
#include "csble_dbg.h"

uint32_t csble_dbg_area = 0x0;
uint32_t csble_dbg_level = 0x0;
uint32_t csble_dump_area = 0x0;

void csble_set_dbgarea(uint32_t dbg_area)
{
    csble_dbg_area = dbg_area;
}

void csble_set_dbglevel(uint32_t dbg_level)
{
    csble_dbg_level= dbg_level;
}

void csble_set_dumparea(uint32_t dump_area)
{
    csble_dump_area= dump_area;
}

uint32_t csble_get_dbgarea()
{
    return csble_dbg_area;
}

uint32_t csble_get_dbglevel()
{
    return csble_dbg_level;
}

uint32_t csble_get_dumparea()
{
    return csble_dump_area;
}

void dump_buf(const uint8_t *buf, uint16_t len)
{
    int i;

    for (i=0;i<len;i++) {
        if((i%8 == 0))
            dbg("  ");
        dbg("%02x ", buf[i]);
        if((i+1)%16 == 0)
            dbg("\n");
    }
    dbg("\n");
}

char *csble_dbgarea(int dbg_flags)
{
    switch (dbg_flags) {
    case CSBLE_HCI_VAL:
        return "[BLE_HCI]";
    case CSBLE_L2CAP_VAL:
        return "[BLE_L2CAP]";
    case CSBLE_ATT_VAL:
        return "[BLE_ATT]";
    case CSBLE_GATT_VAL:
        return "[BLE_GATT]";
    case CSBLE_SMP_VAL:
        return "[BLE_SMP]";
    case CSBLE_GAP_VAL:
        return "[BLE_GAP]";
    case CSBLE_APP_VAL:
        return "[BLE_APP]";
    case CSBLE_PRF_VAL:
        return "[BLE_PRF]";
    default:
        return "[BLE_UNKNOW]";
    }
}

char *csble_dumparea(int dump_area)
{
    switch (dump_area) {
    case CSBLE_TX_ACL_AREA:
        return "[TX_ACL]";
    case CSBLE_TX_CMD_AREA:
        return "[TX_CMD]";
    case CSBLE_RX_EVENT_AREA:
        return "[RX_EVENT]";
    case CSBLE_RX_ACL_AREA:
        return "[RX_ACL]";
    case CSBLE_LTK_AREA:
        return "[LTK]";
    default:
        return "[UNKNOW]";
    }
}

void csble_dbg_hex_dump(int area, const void *data, size_t size)
{
    dbg("%s============== Hex Data Begin ==============\n",
        csble_dumparea(area));
    dump_buf(data, size);
    dbg("%s============== Hex Data End ================\n",
       csble_dumparea(area));
}


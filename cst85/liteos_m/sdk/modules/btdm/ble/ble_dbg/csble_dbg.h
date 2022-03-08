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
#ifndef __CSBLE_DBG_H
#define __CSBLE_DBG_H
/*chipsea_ohos proguard begin*/
#include "cs_proguard.h"
/*chipsea_ohos proguard end*/

#include "rtos.h"
#include "dbg.h"
#include "co_int.h"

enum {
    CSBLE_ERROR_LEVEL  = 0,
    CSBLE_WARN_LEVEL   = 1,
    CSBLE_NOTICE_LEVEL = 2,
    CSBLE_INFO_LEVEL   = 3,
    CSBLE_DEBUG_LEVEL  = 4,
};

#define CSBLE_HCI_VAL                  BIT0
#define CSBLE_L2CAP_VAL                BIT1
#define CSBLE_GAP_VAL                  BIT2
#define CSBLE_GATT_VAL                 BIT3
#define CSBLE_ATT_VAL                  BIT4
#define CSBLE_SMP_VAL                  BIT5
#define CSBLE_APP_VAL                  BIT6
#define CSBLE_PRF_VAL                  BIT7
#define CSBLE_DBG_MASK                 (0xffffffff)
#define CSBLE_NON_DBG                  (0)

#define CSBLE_TX_ACL_AREA              BIT0
#define CSBLE_TX_CMD_AREA              BIT1
#define CSBLE_RX_EVENT_AREA            BIT2
#define CSBLE_RX_ACL_AREA              BIT3
#define CSBLE_LTK_AREA                 BIT4
#define CSBLE_AREA_MASK                 (0xFFFFFFFF)
#define CSBLE_NONE_AREA                (0)

#define TX_ACL                  (csble_dump_area & CSBLE_TX_ACL_AREA)
#define TX_CMD                  (csble_dump_area & CSBLE_TX_CMD_AREA)
#define RX_EVENT                (csble_dump_area & CSBLE_RX_EVENT_AREA)
#define RX_ACL                  (csble_dump_area & CSBLE_RX_ACL_AREA)
#define LTK                     (csble_dump_area & CSBLE_LTK_AREA)

extern uint32_t csble_dbg_area;
extern uint32_t csble_dbg_level;
extern uint32_t csble_dump_area;

char *csble_dbgarea(int dbg_flags);
void csble_set_dbgarea(uint32_t dbg_area);
void csble_set_dbgarea(uint32_t dbg_area);
void csble_set_dumparea(uint32_t dump_area);
uint32_t csble_get_dbgarea();
uint32_t csble_get_dbglevel();
uint32_t csble_get_dumparea();

#ifdef CFG_DBG
#define CSBLE_DBG(area, level, fmt, ...) \
        do { \
            uint32_t dbg_area = CSBLE_##area##_VAL & csble_dbg_area; \
            uint32_t dbg_level = CSBLE_##level##_LEVEL; \
            if (dbg_area && (dbg_level <= csble_dbg_level)){ \
                    const char *prefix = NULL; \
                    prefix = csble_dbgarea(dbg_area); \
                    dbg("%s<%s,%d>", prefix, __func__, __LINE__); \
                    dbg(fmt, ##__VA_ARGS__); \
                    dbg("\n");\
            } \
        } while (0)
#else /* CFG_DBG */
#define CSBLE_DBG(area, level, fmt, ...)
#endif /* CFG_DBG */

#ifdef CFG_DBG
void csble_dbg_hex_dump(int area, const void *data, size_t size);
#define CSBLE_DUMP(area, data, len)\
        do {\
            if (area){\
                dbg("[BLE_DUMP]:<%s,%d>: (len:%d)\n", __func__, __LINE__,(int)len);\
                csble_dbg_hex_dump(area, data, len);\
            }\
        } while (0)
#else /* CFG_DBG */
#define CSBLE_DUMP(area, data, len)
#endif /* CFG_DBG */

#endif /* __CSBLE_DBG_H */

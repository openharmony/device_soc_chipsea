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
/**
 ****************************************************************************************
 *
 * @file dma_generic.h
 *
 * @brief DMA generic header file
 *
 ****************************************************************************************
 */

#ifndef _DMA_GENERIC_H_
#define _DMA_GENERIC_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "reg_dma.h"
#include "dma_common.h"

/// DMA channel indexes
enum {
    /// Channels for IPC
    DMA_CHANNEL_IPC_DATA_AC0_TX = 0,
    DMA_CHANNEL_IPC_DATA_AC1_TX = 1,
    DMA_CHANNEL_IPC_DATA_AC2_TX = 2,
    DMA_CHANNEL_IPC_DATA_AC3_TX = 3,
    DMA_CHANNEL_IPC_DATA_BCN_TX = 4,
    DMA_CHANNEL_IPC_DATA_RX0    = 5,
    DMA_CHANNEL_IPC_MSG         = 6,
    DMA_CHANNEL_IPC_DBG         = 7,
    DMA_CHANNEL_IPC_CFM_TX      = 8,
    DMA_CHANNEL_IPC_GP_DL       = 9,
    DMA_CHANNEL_IPC_DBG_DUMP    = 10,
    DMA_CHANNEL_IPC_GP_UL       = 11,
    DMA_CHANNEL_IPC_MAX,
    /// Channels for UART
    DMA_CHANNEL_UART_RX         = 8,
    DMA_CHANNEL_UART_TX         = 9,
    /// Channels for SPI0
    DMA_CHANNEL_SPI0_RX         = 8,
    DMA_CHANNEL_SPI0_TX         = 9,
    /// Channels for SDMMC
    DMA_CHANNEL_SDMMC_RX        = 10,
    DMA_CHANNEL_SDMMC_TX        = 11,
    /// Channels for CRYPTO
    DMA_CHANNEL_CYPT_IDX0       = 12,
    DMA_CHANNEL_CYPT_IDX1       = 13,

    DMA_CHANNEL_MAX             = 20
};

/// DMA descriptors
typedef struct {
    uint32_t SAR;               /* Src Addr             */
    uint32_t DAR;               /* Dest Addr            */
    uint32_t TBL0CR;            /* Trans Blk Lvl-0 Cnt  */
    uint32_t TBL1CR;            /* Trans Blk Lvl-1 Cnt  */
    uint32_t TBL2CR;            /* Trans Blk Lvl-2 Cnt  */
    uint32_t TSR;               /* Trans Sz             */
    uint32_t RESERVED0[2];
    uint32_t LNAR;              /* Lli Node Addr        */
    uint32_t RESERVED1[3];
}dma_desc_t;

#endif // _DMA_GENERIC_H_

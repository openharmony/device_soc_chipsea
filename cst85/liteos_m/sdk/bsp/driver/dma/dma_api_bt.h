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
 * @file dma_api_bt.h
 *
 * @brief DMA utility functions
 *
 ****************************************************************************************
 */

#ifndef _DMA_API_BT_H_
#define _DMA_API_BT_H_

#include "reg_dma.h"
#include "dma_common.h"

typedef struct {
    uint32_t SAR;           /* Src Addr                     */
    uint32_t DAR;           /* Dest Addr                    */
    uint32_t TBL0CR;        /* Trans Blk Lvl-0 Cnt          */
    uint32_t TBL1CR;        /* Trans Blk Lvl-1 Cnt          */
    uint32_t TBL2CR;        /* Trans Blk Lvl-2 Cnt          */
    uint32_t TSR;           /* Trans Sz                     */
    uint32_t WMAR;          /* Wrp Match Addr               */
    uint32_t WJAR;          /* Wrp Jump Addr                */
    uint32_t LNAR;          /* Lli Node Addr(DWord Align)   */
    uint32_t TBL0SR;        /* Trans Blk Lvl-0 Sz           */
    uint32_t TBL1SSR;       /* Trans Blk Lvl-1 Src Sz       */
    uint32_t TBL1DSR;       /* Trans Blk Lvl-1 Dest Sz      */
} DMA_CX_DESC_T;

void dma_cx_handler_register(uint8_t ch, CS_DMA_IRQ_HANDLER_T handler);
void dma_cx_halt_set(CS_DMA_CH_ENUM ch, bool pause);
void dma_cx_enable_set(CS_DMA_CH_ENUM ch, bool enable);
void dma_cx_request(CS_DMA_CH_ENUM ch);
void dma_get_default_config(DMA_TYPE_ENUM type, DMA_CFG_T *cfg);
void dma_cx_config(CS_DMA_CH_ENUM ch, DMA_CFG_T *cfg);
void dma_cx_desc_link(DMA_CFG_T *cfg, DMA_CX_DESC_T *desc, DMA_CX_DESC_T *next);
uint32_t dma_cx_get_periph_addr(DMA_CX_PERIPH_ENUM periph);
uint32_t dma_cx_get_addr_rd(CS_DMA_CH_ENUM ch);
uint32_t dma_cx_get_addr_wr(CS_DMA_CH_ENUM ch);
uint16_t dma_cx_get_lli_cntr(CS_DMA_CH_ENUM ch);
void dma_cx_set_lli_cntr(CS_DMA_CH_ENUM ch, uint16_t val);
void dma_cx_free(CS_DMA_CH_ENUM ch);
void dma_cx_reg_dump(CS_DMA_CH_ENUM ch);

#endif

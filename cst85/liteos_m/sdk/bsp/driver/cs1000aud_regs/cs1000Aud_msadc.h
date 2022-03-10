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
#ifndef _CS1000AUD_MSADC_H_
#define _CS1000AUD_MSADC_H_

#include "chip.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// CS1000AUD_MSADC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_CS1000AUD_MSADC_BASE  0x5001B000

typedef struct {
    __IO uint32_t               cfg_msadc_mode;             //0x00000000
    __IO uint32_t               cfg_msadc_sw_ctrl0;         //0x00000004
    __IO uint32_t               cfg_msadc_sw_ctrl1;         //0x00000008
    __IO uint32_t               cfg_msadc_ana_ctrl0;        //0x0000000C
    __IO uint32_t               cfg_msadc_ro_acc;           //0x00000010
    __IO uint32_t               cfg_msadc_ro_test_acc_0;    //0x00000014
    __IO uint32_t               cfg_msadc_ro_test_acc_1;    //0x00000018
    __IO uint32_t               cfg_msadc_int_raw;          //0x0000001C
    __IO uint32_t               cfg_msadc_int_status;       //0x00000020
    __IO uint32_t               cfg_msadc_int_mask;         //0x00000024
} HWP_CS1000AUD_MSADC_T;

static HWP_CS1000AUD_MSADC_T * const cs1000audMsadc = ((HWP_CS1000AUD_MSADC_T *)REG_CS1000AUD_MSADC_BASE);


//cfg_msadc_mode
#define CS1000AUD_MSADC_CFG_MSADC_MODE (1<<0)
#define CS1000AUD_MSADC_CFG_MSADC_HW_TEST_CHANNEL (1<<1)

//cfg_msadc_sw_ctrl0
#define CS1000AUD_MSADC_CFG_MSADC_SW_START_PULSE (1<<0)
#define CS1000AUD_MSADC_CFG_MSADC_SW_STOP_PULSE (1<<1)
#define CS1000AUD_MSADC_CFG_MSADC_SW_CLEAR_PULSE (1<<2)

//cfg_msadc_sw_ctrl1
#define CS1000AUD_MSADC_CFG_MSADC_SW_MUX_BITS(n) (((n)&15)<<0)
#define CS1000AUD_MSADC_CFG_MSADC_TEST_0_MUX_BITS(n) (((n)&15)<<4)
#define CS1000AUD_MSADC_CFG_MSADC_TEST_1_MUX_BITS(n) (((n)&15)<<8)
#define CS1000AUD_MSADC_CFG_MSADC_WAIT_CNT(n) (((n)&15)<<12)
#define CS1000AUD_MSADC_CFG_MSADC_WINDOW(n) (((n)&0xFFF)<<16)
#define CS1000AUD_MSADC_CFG_MSADC_SW_DIFF_MODE (1<<28)

//cfg_msadc_ana_ctrl0
#define CS1000AUD_MSADC_CFG_ANA_MSADC_CHNP_SEL (1<<0)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_CMP_BIT (1<<1)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_VBE_CAL_BIT(n) (((n)&0x7F)<<2)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_TS_MODE (1<<9)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_SDM_MODE (1<<10)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_TEST_MODE (1<<11)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_IBIAS_BIT(n) (((n)&7)<<12)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_CHOP_EN (1<<15)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_SDM_GAIN_BIT (1<<16)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_DEM_EN(n) (((n)&3)<<17)
#define CS1000AUD_MSADC_CFG_ANA_MSADC_ADC_FF_EN (1<<19)

//cfg_msadc_ro_acc
#define CS1000AUD_MSADC_RO_MSADC_DAT_ACC(n) (((n)&0x3FFFFF)<<0)

//cfg_msadc_ro_test_acc_0
#define CS1000AUD_MSADC_RO_MSADC_TEST_ACC_0(n) (((n)&0x3FFFFF)<<0)

//cfg_msadc_ro_test_acc_1
#define CS1000AUD_MSADC_RO_MSADC_TEST_ACC_1(n) (((n)&0x3FFFFF)<<0)

//cfg_msadc_int_raw
#define CS1000AUD_MSADC_MASDC_INT_RAW (1<<0)

//cfg_msadc_int_status
#define CS1000AUD_MSADC_MSADC_INT_STATUS (1<<0)

//cfg_msadc_int_mask
#define CS1000AUD_MSADC_CFG_MSADC_INT_MASK (1<<0)

#endif

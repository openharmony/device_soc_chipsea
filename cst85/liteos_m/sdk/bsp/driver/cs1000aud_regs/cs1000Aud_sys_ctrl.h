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
#ifndef _CS1000AUD_SYS_CTRL_H_
#define _CS1000AUD_SYS_CTRL_H_

#include "chip.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// CS1000AUD_SYS_CTRL_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_CS1000AUD_SYSCTRL_BASE 0x50010000

typedef struct {
    __IO uint32_t               CHIP_ID;                    //0x00000000
    __IO uint32_t               clk_sel;                    //0x00000004
    __IO uint32_t               pclk_ctrl_mode;             //0x00000008
    __IO uint32_t               others_clk_ctrl_mode;       //0x0000000C
    __IO uint32_t               pclk_div;                   //0x00000010
    __IO uint32_t               msadc_clk_div;              //0x00000014
    __IO uint32_t               TOUCH_CTRL;                 //0x00000018
    __IO uint32_t               dcdc_ref_clk_div;           //0x0000001C
    __IO uint32_t               dcdc_ref_clk_ctrl0;         //0x00000020
    __IO uint32_t               dcdc_ref_clk_ctrl1;         //0x00000024
    __IO uint32_t               TPORTS_SEL;                 //0x00000028
    __IO uint32_t               MEM_CFG;                    //0x0000002C
    __IO uint32_t               TPORTS_SEL_2;               //0x00000030
} HWP_CS1000AUD_SYS_CTRL_T;

static HWP_CS1000AUD_SYS_CTRL_T * const cs1000audSysctrl = ((HWP_CS1000AUD_SYS_CTRL_T *)REG_CS1000AUD_SYSCTRL_BASE);


//CHIP_ID
#define CS1000AUD_SYS_CTRL_CHIP_ID(n) (((n)&0xFFFF)<<0)
#define CS1000AUD_SYS_CTRL_METAL_ID(n) (((n)&0xFF)<<16)
#define CS1000AUD_SYS_CTRL_BOND_ID(n) (((n)&15)<<24)
#define CS1000AUD_SYS_CTRL_REV_ID(n) (((n)&15)<<28)

//clk_sel
#define CS1000AUD_SYS_CTRL_CFG_FAST_HWEN (1<<0)
#define CS1000AUD_SYS_CTRL_CFG_FAST_CLK_SEL (1<<1)
#define CS1000AUD_SYS_CTRL_CFG_CLK_PWM_SEL (1<<2)
#define CS1000AUD_SYS_CTRL_CFG_CLK_MSADC_INV_ANA (1<<3)
#define CS1000AUD_SYS_CTRL_CFG_CLK_26M_SEL (1<<4)

//pclk_ctrl_mode
#define CS1000AUD_SYS_CTRL_CFG_PCLK_MODE(n) (((n)&0xFFFFFFFF)<<0)

//others_clk_ctrl_mode
#define CS1000AUD_SYS_CTRL_CFG_OTHERS_CLK_MODE(n) (((n)&0xFFFF)<<0)

//pclk_div
#define CS1000AUD_SYS_CTRL_CFG_PCLK_DIV_DENOM(n) (((n)&0xFF)<<0)
#define CS1000AUD_SYS_CTRL_CFG_PCLK_DIV_UPDATE (1<<8)

//msadc_clk_div
#define CS1000AUD_SYS_CTRL_CFG_CLK_MSADC_DIV_DENOM(n) (((n)&0xFF)<<0)
#define CS1000AUD_SYS_CTRL_CFG_CLK_MSADC_DIV_UPDATE (1<<8)

//TOUCH_CTRL
#define CS1000AUD_SYS_CTRL_CFG_CLK_TOUCH_DIV_MODE(n) (((n)&3)<<0)

//dcdc_ref_clk_div
#define CS1000AUD_SYS_CTRL_CFG_DCDC_REF_CLK_DIV_DENOM(n) (((n)&0xFF)<<0)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_REF_CLK_DIV_UPDATA (1<<8)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_RF_TX_ON_EN (1<<9)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_RF_RX_ON_EN (1<<10)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_PU_MDLL_EN (1<<11)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_REF_CLK_DIV_DENOM_EXT(n) (((n)&0xFF)<<12)

//dcdc_ref_clk_ctrl0
#define CS1000AUD_SYS_CTRL_CFG_DCDC_REF_CLK_TRIANGLE_CYCLE(n) (((n)&0xFFFF)<<0)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_REF_CLK_TRIANGLE_AMP(n) (((n)&3)<<16)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_REF_CLK_TRIANGLE_EN (1<<18)

//dcdc_ref_clk_ctrl1
#define CS1000AUD_SYS_CTRL_CFG_DCDC_RF_OFFSET(n) (((n)&15)<<0)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_PA_OFFSET(n) (((n)&15)<<4)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_CORE_OFFSET(n) (((n)&15)<<8)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_RF_CLK_EN (1<<12)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_PA_CLK_EN (1<<13)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_CORE_CLK_EN (1<<14)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_RF_CLK_HWEN (1<<15)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_PA_CLK_HWEN (1<<16)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_CORE_CLK_HWEN (1<<17)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_RF_DIV(n) (((n)&3)<<18)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_PA_DIV(n) (((n)&3)<<20)
#define CS1000AUD_SYS_CTRL_CFG_DCDC_CORE_DIV(n) (((n)&3)<<22)

//TPORTS_SEL
#define CS1000AUD_SYS_CTRL_CFG_TOP_TPORTS_SEL(n) (((n)&0xFF)<<0)
#define CS1000AUD_SYS_CTRL_CFG_CLK_TPORTS_SEL(n) (((n)&15)<<8)

//MEM_CFG
#define CS1000AUD_SYS_CTRL_CFG_MEM(n) (((n)&0xFFFF)<<0)

//TPORTS_SEL_2
#define CS1000AUD_SYS_CTRL_CFG_TOP_TPORTS_SEL_2(n) (((n)&0xFFFFFFFF)<<0)

#endif

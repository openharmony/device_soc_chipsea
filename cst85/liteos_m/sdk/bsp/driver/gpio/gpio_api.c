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
 * @file gpio_api.c
 *
 * @brief GPIO API functions
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include "gpio_api.h"
#include "reg_gpio.h"
#include "reg_iomux.h"
#include "arch.h"
#if PLF_PMIC
#include "pmic_api.h"
#endif

void gpio_init(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    // iomux
    if ((gpidx < 2) || ((gpidx > 7) && (gpidx < 10))) {
        iomux_gpio_config_sel_setf(gpidx, 1);
    } else if (gpidx < 16) {
        iomux_gpio_config_sel_setf(gpidx, 0);
    } else if (gpidx < 32) {
        iomux_agpio_config_sel_setf((gpidx - 16), 2);
    }
    // mask
    CS_GPIO->MR |=  gpmsk;
}

void gpio_deinit(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    // mask
    CS_GPIO->MR &= ~gpmsk;
}

void gpio_dir_in(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    CS_GPIO->DR &= ~gpmsk;
}

void gpio_dir_out(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    CS_GPIO->DR |=  gpmsk;
}

void gpio_set(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    CS_GPIO->VR |=  gpmsk;
}

void gpio_clr(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    CS_GPIO->VR &= ~gpmsk;
}

int gpio_get(int gpidx)
{
    int val;
    unsigned int gpmsk = 0x01UL << gpidx;
    val = (CS_GPIO->VR & gpmsk) ? 1 : 0;
    return val;
}

void gpio_force_pull_up_enable(int gpidx)
{
    if (gpidx < 16) {
        iomux_gpio_config_pull_dn_setf(gpidx, 0);
        iomux_gpio_config_pull_up_setf(gpidx, 1);
        iomux_gpio_config_pull_frc_setf(gpidx, 1);
    } else if (gpidx < 32) {
        iomux_agpio_config_pull_dn_setf(gpidx, 0);
        iomux_agpio_config_pull_up_setf(gpidx, 1);
        iomux_agpio_config_pull_frc_setf(gpidx, 1);
    }
}

void gpio_force_pull_dn_enable(int gpidx)
{
    if (gpidx < 16) {
        iomux_gpio_config_pull_up_setf(gpidx, 0);
        iomux_gpio_config_pull_dn_setf(gpidx, 1);
        iomux_gpio_config_pull_frc_setf(gpidx, 1);
    } else if (gpidx < 32) {
        iomux_agpio_config_pull_up_setf(gpidx, 0);
        iomux_agpio_config_pull_dn_setf(gpidx, 1);
        iomux_agpio_config_pull_frc_setf(gpidx, 1);
    }
}

void gpio_force_pull_none_enable(int gpidx)
{
    if (gpidx < 16) {
        iomux_gpio_config_pull_dn_setf(gpidx, 0);
        iomux_gpio_config_pull_up_setf(gpidx, 0);
        iomux_gpio_config_pull_frc_setf(gpidx, 1);
    } else if (gpidx < 32) {
        iomux_agpio_config_pull_dn_setf(gpidx, 0);
        iomux_agpio_config_pull_up_setf(gpidx, 0);
        iomux_agpio_config_pull_frc_setf(gpidx, 1);
    }
}

void gpio_force_pull_up_dn_disable(int gpidx)
{
    if (gpidx < 16) {
        iomux_gpio_config_pull_up_setf(gpidx, 0);
        iomux_gpio_config_pull_dn_setf(gpidx, 0);
        iomux_gpio_config_pull_frc_setf(gpidx, 0);
    } else if (gpidx < 32) {
        iomux_agpio_config_pull_up_setf(gpidx, 0);
        iomux_agpio_config_pull_dn_setf(gpidx, 0);
        iomux_agpio_config_pull_frc_setf(gpidx, 0);
    }
}

gpio_irq_handler_t gpio_irq_handler[GPIOA_IDX_MAX] = {NULL};

void gpio_irq_en_set(int gpidx, int enable)
{
    if(enable) {
        //CS_GPIO->IRR = (0x01UL << gpidx);
        CS_GPIO->ICR   |=  (0x01UL << gpidx); // int en
    } else {
        CS_GPIO->ICR   &=  ~(0x01UL << gpidx); // int disable
    }
}

void gpio_irq_init(int gpidx, int type, gpio_irq_handler_t handler)
{
    unsigned int reg_val;
    gpio_init(gpidx);
    gpio_dir_in(gpidx);
    CS_GPIO->TIR |=  (0x01UL << gpidx); // input en
    CS_GPIO->TELR   &= ~(0x01UL << gpidx); // edge
    if (type == GPIOIRQ_TYPE_EDGE_BOTH) {
        CS_GPIO->TER  |=  (0x01UL << gpidx); // both edge
    } else {
        CS_GPIO->TER  &= ~(0x01UL << gpidx); // not both edge
        if (type == GPIOIRQ_TYPE_EDGE_RISE) {
            CS_GPIO->TLR  |=  (0x01UL << gpidx); // rising edge
        } else {
            CS_GPIO->TLR  &= ~(0x01UL << gpidx); // falling edge
        }
    }
    CS_GPIO->ICR   |=  (0x01UL << gpidx); // int en
    reg_val = CS_GPIO->FR & ~(0x07UL << (gpidx & ~0x03UL));
    CS_GPIO->FR = reg_val |  (0x07UL << (gpidx & ~0x03UL)); // filter
    gpio_irq_handler[gpidx] = handler;
    if (!NVIC_GetEnableIRQ(GPIO_IRQn)) {
        NVIC_SetPriority(GPIO_IRQn, __NVIC_PRIO_LOWEST);
        NVIC_EnableIRQ(GPIO_IRQn);
    }
}

void GPIO_IRQHandler(void)
{
    unsigned int int_status = CS_GPIO->ISR;
    while (int_status) {
        int bit = 31 - __CLZ(int_status);
        if (bit >= GPIOA_IDX_MAX) {
            // error occors
            break;
        }
        CS_GPIO->IRR = (0x01UL << bit);
        __ISB();
        if (gpio_irq_handler[bit]) {
            int event = GPIOIRQ_EVENT_EDGE_FALL;
            if (CS_GPIO->TER & (0x01UL << bit)) {
                event = GPIOIRQ_EVENT_EDGE_BOTH;
            } else if (CS_GPIO->TLR & (0x01UL << bit)) {
                event = GPIOIRQ_EVENT_EDGE_RISE;
            }
            gpio_irq_handler[bit](GPIOIRQ_CB_PARAM(bit, event)); // pass gpio num to cb func
        }
        int_status = CS_GPIO->ISR;
    }
}

#if PLF_PMIC
#if PLF_PMIC_VER_LITE
static HWP_CS1000LITE_GPIO_T * const PMIC_GPIOB = cs1000liteGpio;
#endif
#if PLF_PMIC_VER_AUD
static HWP_CS1000AUD_GPIO_T * const PMIC_GPIOB = cs1000audGpio;
#endif

static uint32_t gpiob_irq_history = 0;

void gpiob_init(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
#if PLF_PMIC_VER_LITE
    // iomux
    if (gpidx < 2) {
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->GPCFG[gpidx]),
            (CS1000LITE_IOMUX_PAD_GPIO_SEL(1)),
            (CS1000LITE_IOMUX_PAD_GPIO_SEL(0xF)));
    } else if (gpidx < 16) {
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->GPCFG[gpidx]),
            (CS1000LITE_IOMUX_PAD_GPIO_SEL(0)),
            (CS1000LITE_IOMUX_PAD_GPIO_SEL(0xF)));
        if ((gpidx == 14) || (gpidx == 15)) {
            PMIC_MEM_WRITE((unsigned int)(&cs1000liteSysctrl->MISC_CTRL), CS1000LITE_SYS_CTRL_CFG_USB_IO_USED_FOR_GPIO);
        }
    } else {
        return;
    }
#endif
#if PLF_PMIC_VER_AUD
    // iomux
    if (gpidx < 2) {
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->GPCFG[gpidx]),
            (CS1000AUD_IOMUX_PAD_GPIO_SEL(1)),
            (CS1000AUD_IOMUX_PAD_GPIO_SEL(0xF)));
    } else if (gpidx < 8) {
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->GPCFG[gpidx]),
            (CS1000AUD_IOMUX_PAD_GPIO_SEL(0)),
            (CS1000AUD_IOMUX_PAD_GPIO_SEL(0xF)));
    } else {
        return;
    }
#endif
    // mask
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->MR), gpmsk, gpmsk);
}

void gpiob_deinit(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    // mask
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->MR), 0, gpmsk);
}

void gpiob_dir_in(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    if ((gpidx == 14) || (gpidx == 15)) {
        gpmsk = ((0x01UL << 14) | (0x01UL << 15));
    }
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->DR), 0, gpmsk);
}

void gpiob_dir_out(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    if ((gpidx == 14) || (gpidx == 15)) {
        gpmsk = ((0x01UL << 14) | (0x01UL << 15));
    }
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->DR), gpmsk, gpmsk);
}

void gpiob_set(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->VR), gpmsk, gpmsk);
}

void gpiob_clr(int gpidx)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->VR), 0, gpmsk);
}

int gpiob_get(int gpidx)
{
    int val;
    unsigned int gpmsk = 0x01UL << gpidx;
    val = PMIC_MEM_READ((unsigned int)(&PMIC_GPIOB->VR));
    val = (val & gpmsk) ? 1 : 0;
    return val;
}

void gpiob_force_pull_up_enable(int gpidx)
{
    if (gpidx < 16) {
        #if PLF_PMIC_VER_LITE
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->GPCFG[gpidx]),
            (CS1000LITE_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_UP   |
            0),
            (CS1000LITE_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_UP   |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_DN));
        #endif
        #if PLF_PMIC_VER_AUD
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->GPCFG[gpidx]),
            (CS1000AUD_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_UP   |
            0),
            (CS1000AUD_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_UP   |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_DN));
        #endif
    } else if (gpidx < 32) {
        #if PLF_PMIC_VER_LITE
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->AGPCFG[gpidx - 16]),
            (CS1000LITE_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_UP   |
            0),
            (CS1000LITE_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_UP   |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_DN));
        #endif
        #if PLF_PMIC_VER_AUD
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->AGPCFG[gpidx - 16]),
            (CS1000AUD_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_UP   |
            0),
            (CS1000AUD_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_UP   |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_DN));
        #endif
    }
}

void gpiob_force_pull_dn_enable(int gpidx)
{
    if (gpidx < 16) {
        #if PLF_PMIC_VER_LITE
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->GPCFG[gpidx]),
            (CS1000LITE_IOMUX_PAD_GPIO_PULL_FRC |
            0                                    |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_DN),
            (CS1000LITE_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_UP   |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_DN));
        #endif
        #if PLF_PMIC_VER_AUD
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->GPCFG[gpidx]),
            (CS1000AUD_IOMUX_PAD_GPIO_PULL_FRC |
            0                                   |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_DN),
            (CS1000AUD_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_UP   |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_DN));
        #endif
    } else if (gpidx < 32) {
        #if PLF_PMIC_VER_LITE
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->AGPCFG[gpidx - 16]),
            (CS1000LITE_IOMUX_PAD_AGPIO_PULL_FRC |
            0                                     |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_DN),
            (CS1000LITE_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_UP   |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_DN));
        #endif
        #if PLF_PMIC_VER_AUD
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->AGPCFG[gpidx - 16]),
            (CS1000AUD_IOMUX_PAD_AGPIO_PULL_FRC |
            0                                    |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_DN),
            (CS1000AUD_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_UP   |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_DN));
        #endif
    }
}

void gpiob_force_pull_none_enable(int gpidx)
{
    if (gpidx < 16) {
        #if PLF_PMIC_VER_LITE
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->GPCFG[gpidx]),
            (CS1000LITE_IOMUX_PAD_GPIO_PULL_FRC |
            0                                    |
            0),
            (CS1000LITE_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_UP   |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_DN));
        #endif
        #if PLF_PMIC_VER_AUD
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->GPCFG[gpidx]),
            (CS1000AUD_IOMUX_PAD_GPIO_PULL_FRC |
            0                                   |
            0),
            (CS1000AUD_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_UP   |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_DN));
        #endif
    } else if (gpidx < 32) {
        #if PLF_PMIC_VER_LITE
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->AGPCFG[gpidx - 16]),
            (CS1000LITE_IOMUX_PAD_AGPIO_PULL_FRC |
            0                                     |
            0),
            (CS1000LITE_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_UP   |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_DN));
        #endif
        #if PLF_PMIC_VER_AUD
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->AGPCFG[gpidx - 16]),
            (CS1000AUD_IOMUX_PAD_AGPIO_PULL_FRC |
            0                                    |
            0),
            (CS1000AUD_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_UP   |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_DN));
        #endif
    }
}

void gpiob_force_pull_up_dn_disable(int gpidx)
{
    if (gpidx < 16) {
        #if PLF_PMIC_VER_LITE
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->GPCFG[gpidx]),
            (0                                   |
            0                                    |
            0),
            (CS1000LITE_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_UP   |
            CS1000LITE_IOMUX_PAD_GPIO_PULL_DN));
        #endif
        #if PLF_PMIC_VER_AUD
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->GPCFG[gpidx]),
            (0                                  |
            0                                   |
            0),
            (CS1000AUD_IOMUX_PAD_GPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_UP   |
            CS1000AUD_IOMUX_PAD_GPIO_PULL_DN));
        #endif
    } else if (gpidx < 32) {
        #if PLF_PMIC_VER_LITE
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteIomux->AGPCFG[gpidx - 16]),
            (0                                    |
            0                                     |
            0),
            (CS1000LITE_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_UP   |
            CS1000LITE_IOMUX_PAD_AGPIO_PULL_DN));
        #endif
        #if PLF_PMIC_VER_AUD
        PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000audIomux->AGPCFG[gpidx - 16]),
            (0                                   |
            0                                    |
            0),
            (CS1000AUD_IOMUX_PAD_AGPIO_PULL_FRC |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_UP   |
            CS1000AUD_IOMUX_PAD_AGPIO_PULL_DN));
        #endif
    }
}

void pmic_clock_set_for_gpiob(void)
{
    #if PLF_PMIC_VER_LITE
    // hclk divider
    PMIC_MEM_WRITE((unsigned int)(&cs1000liteSysctrl->hclk_div),
        (CS1000LITE_SYS_CTRL_CFG_HCLK_DIV_DENOM(0x1) | CS1000LITE_SYS_CTRL_CFG_HCLK_DIV_UPDATE));
    // hclk sel
    PMIC_MEM_MASK_WRITE((unsigned int)(&cs1000liteSysctrl->clk_sel),
        (CS1000LITE_SYS_CTRL_CFG_FAST_HWEN), (CS1000LITE_SYS_CTRL_CFG_FAST_HWEN));
    #endif
}

gpio_irq_handler_t gpiob_irq_handler[GPIOB_IDX_MAX] = {NULL};

void GPIOB_IRQHandler(void)
{
    unsigned int int_status = PMIC_MEM_READ((unsigned int)(&PMIC_GPIOB->ISR));
    while (int_status) {
        int bit = 31 - __CLZ(int_status);
        unsigned int bitmsk = (0x01UL << bit);
        if (bit >= GPIOB_IDX_MAX) {
            // error occors
            break;
        }
        PMIC_MEM_WRITE((unsigned int)(&PMIC_GPIOB->IRR), bitmsk);
        __ISB();
        if (gpiob_irq_handler[bit]) {
            int event = GPIOIRQ_EVENT_EDGE_FALL;
            if (PMIC_MEM_READ((unsigned int)(&PMIC_GPIOB->TER)) & bitmsk) {
                event = GPIOIRQ_EVENT_EDGE_BOTH;
            } else if (PMIC_MEM_READ((unsigned int)(&PMIC_GPIOB->TLR)) & bitmsk) {
                event = GPIOIRQ_EVENT_EDGE_RISE;
            }
            gpiob_irq_handler[bit](GPIOIRQ_CB_PARAM(bit, event)); // pass gpiob num to cb func
        } else {
            gpiob_irq_history_set(0x01UL << bit);
        }
        int_status = PMIC_MEM_READ((unsigned int)(&PMIC_GPIOB->ISR));
    }
}

void gpiob_irq_init(int gpidx, int type, gpio_irq_handler_t handler, int filter)
{
    unsigned int gpmsk = 0x01UL << gpidx;
    gpiob_init(gpidx);
    gpiob_dir_in(gpidx);
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->TIR), gpmsk, gpmsk); // input en
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->TELR), 0, gpmsk); // edge
    if (type == GPIOIRQ_TYPE_EDGE_BOTH) {
        PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->TER), gpmsk, gpmsk); // both edge
    } else {
        PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->TER), 0, gpmsk); // not both edge
        if (type == GPIOIRQ_TYPE_EDGE_RISE) {
            PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->TLR), gpmsk, gpmsk); // rising edge
        } else {
            PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->TLR), 0, gpmsk); // falling edge
        }
    }
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->ICR), gpmsk, gpmsk); // int en
#if PLF_PMIC_VER_LITE
    PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->FR),
        (0x06UL << (gpidx & ~0x03UL)), (0x07UL << (gpidx & ~0x03UL))); // filter
#endif
#if PLF_PMIC_VER_AUD
    if(filter) {
        unsigned int fltr_div = (filter/7 - 1) & 0xffUL;
        if(fltr_div) {
            PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->FNR),
                (0x07UL << (gpidx & ~0x03UL)), (0x07UL << (gpidx & ~0x03UL))); // filter num
            PMIC_MEM_MASK_WRITE((unsigned int)(&PMIC_GPIOB->FDR), fltr_div, 0xffUL); // filter div
        }
    }
#endif
    gpiob_irq_handler[gpidx] = handler;
    pmic_irq_enable(0x01UL << PMIC_GPIO_IRQn);
}

void gpiob_irq_history_set(uint32_t gpidx_bit)
{
    gpiob_irq_history |= gpidx_bit;
}

void gpiob_irq_history_clear(uint32_t gpidx_bit)
{
    gpiob_irq_history &= ~gpidx_bit;
}

uint32_t gpiob_irq_history_get(void)
{
    return gpiob_irq_history;
}

#endif

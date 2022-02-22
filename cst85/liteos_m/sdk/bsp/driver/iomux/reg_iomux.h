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
#ifndef _REG_IOMUX_H_
#define _REG_IOMUX_H_

#include "chip.h"

/* ================================================================================ */
/* ================                      IOMUX                     ================ */
/* ================================================================================ */
typedef struct {
  __IO uint32_t GPCFG[16];      /* 0x000: GPIO Config 0 ~ 15 */
  __IO uint32_t AGPCFG[16];     /* 0x040: AGPIO Config 0 ~ 15 */
} CS_IOMUX_TypeDef;

static CS_IOMUX_TypeDef * const CS_IOMUX = ((CS_IOMUX_TypeDef *)CS_IOMUX_BASE);

/**
 * IOMUX GPIO CFG
 */

#define IOMUX_GPIO_CONFIG_SEL_LSB           (0)
#define IOMUX_GPIO_CONFIG_SEL_WIDTH         (4)
#define IOMUX_GPIO_CONFIG_SEL_MASK          (((1 << IOMUX_GPIO_CONFIG_SEL_WIDTH) - 1) << IOMUX_GPIO_CONFIG_SEL_LSB)
#define IOMUX_GPIO_CONFIG_PULL_DN_LSB       (8)
#define IOMUX_GPIO_CONFIG_PULL_DN_WIDTH     (1)
#define IOMUX_GPIO_CONFIG_PULL_DN_MASK      (((1 << IOMUX_GPIO_CONFIG_PULL_DN_WIDTH) - 1) << IOMUX_GPIO_CONFIG_PULL_DN_LSB)
#define IOMUX_GPIO_CONFIG_PULL_UP_LSB       (9)
#define IOMUX_GPIO_CONFIG_PULL_UP_WIDTH     (1)
#define IOMUX_GPIO_CONFIG_PULL_UP_MASK      (((1 << IOMUX_GPIO_CONFIG_PULL_UP_WIDTH) - 1) << IOMUX_GPIO_CONFIG_PULL_UP_LSB)
#define IOMUX_GPIO_CONFIG_PULL_FRC_LSB      (16)
#define IOMUX_GPIO_CONFIG_PULL_FRC_WIDTH    (1)
#define IOMUX_GPIO_CONFIG_PULL_FRC_MASK     (((1 << IOMUX_GPIO_CONFIG_PULL_FRC_WIDTH) - 1) << IOMUX_GPIO_CONFIG_PULL_FRC_LSB)

#define IOMUX_AGPIO_CONFIG_SEL_LSB          (0)
#define IOMUX_AGPIO_CONFIG_SEL_WIDTH        (4)
#define IOMUX_AGPIO_CONFIG_SEL_MASK         (((1 << IOMUX_AGPIO_CONFIG_SEL_WIDTH) - 1) << IOMUX_AGPIO_CONFIG_SEL_LSB)
#define IOMUX_AGPIO_CONFIG_PULL_DN_LSB      (8)
#define IOMUX_AGPIO_CONFIG_PULL_DN_WIDTH    (1)
#define IOMUX_AGPIO_CONFIG_PULL_DN_MASK     (((1 << IOMUX_AGPIO_CONFIG_PULL_DN_WIDTH) - 1) << IOMUX_AGPIO_CONFIG_PULL_DN_LSB)
#define IOMUX_AGPIO_CONFIG_PULL_UP_LSB      (9)
#define IOMUX_AGPIO_CONFIG_PULL_UP_WIDTH    (1)
#define IOMUX_AGPIO_CONFIG_PULL_UP_MASK     (((1 << IOMUX_AGPIO_CONFIG_PULL_UP_WIDTH) - 1) << IOMUX_AGPIO_CONFIG_PULL_UP_LSB)
#define IOMUX_AGPIO_CONFIG_PULL_FRC_LSB     (16)
#define IOMUX_AGPIO_CONFIG_PULL_FRC_WIDTH   (1)
#define IOMUX_AGPIO_CONFIG_PULL_FRC_MASK    (((1 << IOMUX_AGPIO_CONFIG_PULL_FRC_WIDTH) - 1) << IOMUX_AGPIO_CONFIG_PULL_FRC_LSB)

__STATIC_INLINE uint8_t iomux_gpio_config_sel_getf(int gp_idx)
{
    return ((CS_IOMUX->GPCFG[gp_idx] & IOMUX_GPIO_CONFIG_SEL_MASK) >> IOMUX_GPIO_CONFIG_SEL_LSB);
}

__STATIC_INLINE void iomux_gpio_config_sel_setf(int gp_idx, uint8_t sel_val)
{
    uint32_t local_val = CS_IOMUX->GPCFG[gp_idx] & ~IOMUX_GPIO_CONFIG_SEL_MASK;
    CS_IOMUX->GPCFG[gp_idx] = local_val | ((sel_val << IOMUX_GPIO_CONFIG_SEL_LSB) & IOMUX_GPIO_CONFIG_SEL_MASK);
}

__STATIC_INLINE uint8_t iomux_gpio_config_pull_dn_getf(int gp_idx)
{
    return ((CS_IOMUX->GPCFG[gp_idx] & IOMUX_GPIO_CONFIG_PULL_DN_MASK) >> IOMUX_GPIO_CONFIG_PULL_DN_LSB);
}

__STATIC_INLINE void iomux_gpio_config_pull_dn_setf(int gp_idx, uint8_t sel_val)
{
    uint32_t local_val = CS_IOMUX->GPCFG[gp_idx] & ~IOMUX_GPIO_CONFIG_PULL_DN_MASK;
    CS_IOMUX->GPCFG[gp_idx] = local_val | ((sel_val << IOMUX_GPIO_CONFIG_PULL_DN_LSB) & IOMUX_GPIO_CONFIG_PULL_DN_MASK);
}

__STATIC_INLINE uint8_t iomux_gpio_config_pull_up_getf(int gp_idx)
{
    return ((CS_IOMUX->GPCFG[gp_idx] & IOMUX_GPIO_CONFIG_PULL_UP_MASK) >> IOMUX_GPIO_CONFIG_PULL_UP_LSB);
}

__STATIC_INLINE void iomux_gpio_config_pull_up_setf(int gp_idx, uint8_t sel_val)
{
    uint32_t local_val = CS_IOMUX->GPCFG[gp_idx] & ~IOMUX_GPIO_CONFIG_PULL_UP_MASK;
    CS_IOMUX->GPCFG[gp_idx] = local_val | ((sel_val << IOMUX_GPIO_CONFIG_PULL_UP_LSB) & IOMUX_GPIO_CONFIG_PULL_UP_MASK);
}

__STATIC_INLINE uint8_t iomux_gpio_config_pull_frc_getf(int gp_idx)
{
    return ((CS_IOMUX->GPCFG[gp_idx] & IOMUX_GPIO_CONFIG_PULL_FRC_MASK) >> IOMUX_GPIO_CONFIG_PULL_FRC_LSB);
}

__STATIC_INLINE void iomux_gpio_config_pull_frc_setf(int gp_idx, uint8_t sel_val)
{
    uint32_t local_val = CS_IOMUX->GPCFG[gp_idx] & ~IOMUX_GPIO_CONFIG_PULL_FRC_MASK;
    CS_IOMUX->GPCFG[gp_idx] = local_val | ((sel_val << IOMUX_GPIO_CONFIG_PULL_FRC_LSB) & IOMUX_GPIO_CONFIG_PULL_FRC_MASK);
}

__STATIC_INLINE uint8_t iomux_agpio_config_sel_getf(int agp_idx)
{
    return ((CS_IOMUX->AGPCFG[agp_idx] & IOMUX_AGPIO_CONFIG_SEL_MASK) >> IOMUX_AGPIO_CONFIG_SEL_LSB);
}

__STATIC_INLINE void iomux_agpio_config_sel_setf(int agp_idx, uint8_t sel_val)
{
    uint32_t local_val = CS_IOMUX->AGPCFG[agp_idx] & ~IOMUX_AGPIO_CONFIG_SEL_MASK;
    CS_IOMUX->AGPCFG[agp_idx] = local_val | ((sel_val << IOMUX_AGPIO_CONFIG_SEL_LSB) & IOMUX_AGPIO_CONFIG_SEL_MASK);
}

__STATIC_INLINE uint8_t iomux_agpio_config_pull_dn_getf(int gp_idx)
{
    return ((CS_IOMUX->AGPCFG[gp_idx] & IOMUX_AGPIO_CONFIG_PULL_DN_MASK) >> IOMUX_AGPIO_CONFIG_PULL_DN_LSB);
}

__STATIC_INLINE void iomux_agpio_config_pull_dn_setf(int gp_idx, uint8_t sel_val)
{
    uint32_t local_val = CS_IOMUX->AGPCFG[gp_idx] & ~IOMUX_AGPIO_CONFIG_PULL_DN_MASK;
    CS_IOMUX->AGPCFG[gp_idx] = local_val | ((sel_val << IOMUX_AGPIO_CONFIG_PULL_DN_LSB) & IOMUX_AGPIO_CONFIG_PULL_DN_MASK);
}

__STATIC_INLINE uint8_t iomux_agpio_config_pull_up_getf(int gp_idx)
{
    return ((CS_IOMUX->AGPCFG[gp_idx] & IOMUX_AGPIO_CONFIG_PULL_UP_MASK) >> IOMUX_AGPIO_CONFIG_PULL_UP_LSB);
}

__STATIC_INLINE void iomux_agpio_config_pull_up_setf(int gp_idx, uint8_t sel_val)
{
    uint32_t local_val = CS_IOMUX->AGPCFG[gp_idx] & ~IOMUX_AGPIO_CONFIG_PULL_UP_MASK;
    CS_IOMUX->AGPCFG[gp_idx] = local_val | ((sel_val << IOMUX_AGPIO_CONFIG_PULL_UP_LSB) & IOMUX_AGPIO_CONFIG_PULL_UP_MASK);
}

__STATIC_INLINE uint8_t iomux_agpio_config_pull_frc_getf(int gp_idx)
{
    return ((CS_IOMUX->AGPCFG[gp_idx] & IOMUX_AGPIO_CONFIG_PULL_FRC_MASK) >> IOMUX_AGPIO_CONFIG_PULL_FRC_LSB);
}

__STATIC_INLINE void iomux_agpio_config_pull_frc_setf(int gp_idx, uint8_t sel_val)
{
    uint32_t local_val = CS_IOMUX->AGPCFG[gp_idx] & ~IOMUX_AGPIO_CONFIG_PULL_FRC_MASK;
    CS_IOMUX->AGPCFG[gp_idx] = local_val | ((sel_val << IOMUX_AGPIO_CONFIG_PULL_FRC_LSB) & IOMUX_AGPIO_CONFIG_PULL_FRC_MASK);
}

#endif // _REG_IOMUX_H_

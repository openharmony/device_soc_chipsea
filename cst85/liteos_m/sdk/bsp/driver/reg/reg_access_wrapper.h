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
#ifndef _REG_WRAPPER_H
#define _REG_WRAPPER_H

#include <stddef.h>

/**
 * Register access wrapper.
 */

// BLE register wrapper.
#define BLE_REG_READ(addr) (*(volatile uint32_t *)(addr))
#define BLE_REG_WRITE(addr, value) (*(volatile uint32_t *)(addr))
#define BLE_CTRL_READ(addr) (*(volatile uint16_t *)(addr))
#define BLE_CTRL_WRITE(addr,value) (*(volatile uint16_t *)(addr)) = (value)

// BT register wrapper
#define BT_REG_READ(addr) (*(volatile uint32_t *)(addr))
#define BT_REG_WRITE(addr, value) (*(volatile uint32_t *)(addr)) = (value)
#define BT_CTRL_READ(addr) (*(volatile uint16_t *)(addr))
#define BT_CTRL_WRITE(addr, value) (*(volatile uint16_t *)(addr)) = (value)

// Platform register wrapper.
#define PLATFORM_REG_READ(addr) (*(volatile uint32_t *)(addr))
#define PLATFORM_REG_WRITE(addr, value) (*(volatile uint32_t *)(addr)) = (value)
#define IP_REG_READ(addr) (*(volatile uint32_t *)(addr))
#define IP_REG_WRITE(addr, value) (*(volatile uint32_t *)(addr)) = (value)

#endif // _REG_WRAPPER_H

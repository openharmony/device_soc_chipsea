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
#ifndef _UART_H
#define _UART_H

#include <stdbool.h>          // standard boolean definitions
#include <stdint.h>           // standard integer functions
#include "reg_stdio_uart.h"   // uart register
#include "reg_ipc_mutex.h"
#include "chip.h"

#define IPC_MUTEX_UART_OUTPUT   (IPC_MUTEX_ID_UART0_OUTPUT + UART_INDEX)

/// 3-part macro to function/variable/enum string
#define M2STR_P3_I(p0,p1,p2)    p0##p1##p2
#define M2STR_P3(p0,p1,p2)      M2STR_P3_I(p0, p1, p2)

/// Replace WEAK function in startup.S
#define stdio_uart_isr      M2STR_P3(UART, UART_INDEX, _IRQHandler)

typedef void (*stdio_uart_rx_func_t)(void);

extern int stdio_uart_inited;

// function declarations.
void stdio_uart_init(void);
uint32_t stdio_uart_baud_get(void);
void stdio_uart_baud_set(uint32_t baud);
void stdio_uart_format_get(uint32_t *bits, uint32_t *parity, uint32_t *stop);
void stdio_uart_format_set(uint32_t bits, uint32_t parity, uint32_t stop);
void stdio_uart_putc(char ch);
char stdio_uart_getc(void);
bool stdio_uart_tstc(void);
int  stdio_uart_get_rx_count(void);
void register_stdio_uart_rx_function(stdio_uart_rx_func_t func);
void stdio_uart_isr(void);

#endif //_UART_H

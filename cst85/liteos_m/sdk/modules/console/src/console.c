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
#include "console.h"
#include "command.h"
#include "uart.h"

volatile bool cmd_exe_in_irq = false;
#ifdef CFG_RTOS
static rtos_notify_cb_t console_notify_cb = NULL;
#endif

static void console_irq_handler(void)
{
    bool cmd_valid = false;
    while (stdio_uart_get_rx_count()) {
        unsigned char c = (unsigned char)stdio_uart_getc();
        if (command_handle_char(c) > 0) {
            cmd_valid = true;
        }
    }

    if (cmd_valid) {
        /* if cmd_exe_in_irq was true, we check and execute commands here */
        if (cmd_exe_in_irq) {
            console_schedule();
        }
#ifdef CFG_RTOS
        else if (console_notify_cb) {
            console_notify_cb(true);
        }
#endif /* CFG_RTOS */
    }
}

void console_putc(char c)
{
    stdio_uart_putc(c);
}

void console_puts(const char *s)
{
    while (*s) {
        stdio_uart_putc(*s++);
    }
}

void console_init(void)
{
    if (!stdio_uart_inited) {
        stdio_uart_init();
    }

    command_init();

    register_stdio_uart_rx_function(console_irq_handler);
}

void console_schedule(void)
{
    command_parser();
}

int console_cmd_add(const char *name, const char *usage, int maxargs, int (*func)(int, char *[]))
{
    return command_add(name, usage, maxargs, (cmd_func_t)func);
}

unsigned int console_cmd_strtoul(const char *cp, char **endp, unsigned int base)
{
    return command_strtoul(cp, endp, base);
}

unsigned int console_buf_empty(void)
{
    return command_pend_list_empty();
}

#ifdef CFG_RTOS
void console_ntf_register(rtos_notify_cb_t notify_cb)
{
    if (!console_notify_cb) {
        console_notify_cb = notify_cb;
    }
}
#endif /* CFG_RTOS */

#if CONSOLE_GLOBAL_DEBUG_MODE
#include "reg_stdio_uart.h"

void console_global_dbgmode_enable(void)
{
    NVIC_SetPriority(UART_IRQn, 0x07UL);
    cmd_exe_in_irq = true;
}
#endif /* CONSOLE_GLOBAL_DEBUG_MODE */

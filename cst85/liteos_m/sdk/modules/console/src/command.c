/*
 * Copyright (C) 2018-2020 CSSemi Ltd.
 *
 * Most of this code is derived from the U-Boot Command Processor Table.
 *
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
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "command.h"
#include "console.h"
#include "lnx_list.h"
#if PLF_PMIC
#include "pmic_api.h"
#if (PLF_PMIC_VER_LITE && PLF_EXT_CS1000)
#include "cs1000_psi_api.h"
#endif
#endif

#if CONSOLE_GLOBAL_DEBUG_MODE
#define CMDISR_SUPER_STR    "#6*"
#endif /* CONSOLE_GLOBAL_DEBUG_MODE */

#define IS_PMIC_SRAM_VALID(addr)    (((unsigned int)(addr) >> 24) == (CS_PMIC_RAM_BASE >> 24)) // 0x01000000 ~ 0x01FFFFFF
#define IS_PMIC_REG_VALID(addr)     (((unsigned int)(addr) >> 20) == (CS_PMIC_REG_BASE >> 20)) // 0x50000000 ~ 0x500FFFFF

#define CONFIG_SYS_CBSIZE   64
#define CONFIG_SYS_PROMPT   "cs> "
#define CONFIG_SYS_MAXARGS  16
#define CONFIG_LINE_BYTES   16

#define CONFIG_CMDTBL_SIZE  36
#define CONFIG_CMDBUF_SIZE  4

typedef struct {
    struct list_head entry;
    char buffer[CONFIG_SYS_CBSIZE];
} cmd_buf_node_t;

static const char erase_seq[] = "\b \b";        /* erase sequence       */
static const char   tab_seq[] = "    ";         /* used to expand TABs  */
static char console_buffer[CONFIG_SYS_CBSIZE];  /* console I/O buffer   */

static cmd_buf_node_t cmd_buf_tab[CONFIG_CMDBUF_SIZE];
static struct list_head cmd_free_list;
static struct list_head cmd_pend_list;

static cmd_tbl_t cmd_tbl_list[CONFIG_CMDTBL_SIZE] = { {0,}, };
static unsigned int cmd_tbl_curidx = 0;

#define isblank(c)      ((c) == ' ' || (c) == '\t')
#define isascii(c)      (((unsigned char)(c)) <= 0x7F)

__STATIC_INLINE int isprint(unsigned char c)
{
    if ((c >= 0x1F) && (c <= 0x7E))
        return 1;
    return 0;
}

__STATIC_INLINE int isdigit(unsigned char c)
{
    return ((c >= '0') && (c <='9'));
}

__STATIC_INLINE int isxdigit(unsigned char c)
{
    if ((c >= '0') && (c <='9'))
        return 1;
    if ((c >= 'a') && (c <='f'))
        return 1;
    if ((c >= 'A') && (c <='F'))
        return 1;
    return 0;
}

__STATIC_INLINE int islower(unsigned char c)
{
    return ((c >= 'a') && (c <='z'));
}

__STATIC_INLINE int isupper(unsigned char c)
{
    return ((c >= 'A') && (c <='Z'));
}

__STATIC_INLINE unsigned char tolower(unsigned char c)
{
    if (isupper(c))
        c -= 'A'-'a';
    return c;
}

__STATIC_INLINE unsigned char toupper(unsigned char c)
{
    if (islower(c))
        c -= 'a'-'A';
    return c;
}

__STATIC_INLINE int make_argv(char *s, char *argv[])
{
    int argc = 0;

    while (argc < CONFIG_SYS_MAXARGS) {
        /* skip any white space */
        while (isblank(*s)) {
            ++s;
        }

        if (*s == '\0') {       /* end of line, no more args */
            argv[argc] = 0;
            return argc;
        }

        if(*s == '\"') {        /* args with quotation marks */
            s++;                /* the first quotation mark */

            argv[argc++] = s;   /* begin of argument string */

            while(*s && (*s != '\"')) { /* the second quotation mark */
                ++s;
            }
        } else {
            argv[argc++] = s;   /* begin of argument string */

            /* find end of string */
            while(*s && !isblank(*s)) {
                ++s;
            }
        }

        if (*s == '\0') {       /* end of line, no more args */
            argv[argc] = 0;
            return argc;
        }

        *s++ = '\0';         /* terminate current arg */
    }

    console_putf("** Too many args (max. %d) **\n", CONFIG_SYS_MAXARGS);

    return argc;
}

__STATIC_INLINE cmd_tbl_t * find_cmd(const char *cmd)
{
    cmd_tbl_t *cmdtp;
    cmd_tbl_t *cmdtp_temp = &cmd_tbl_list[0];    /* Init value */
    const char *p;
    unsigned int len;
    int n_found = 0;
    int i;

    p = strchr(cmd, '.');
    len = (p == NULL) ? strlen(cmd) : (unsigned int)(p - cmd);

    for (i = 0; i < (int)cmd_tbl_curidx; i++) {
        cmdtp = &cmd_tbl_list[i];
        if (strncmp(cmd, cmdtp->name, len) == 0) {
            if (len == strlen(cmdtp->name))
                return cmdtp;      /* full match */

            cmdtp_temp = cmdtp;    /* abbreviated command ? */
            n_found++;
        }
    }
    if (n_found == 1) {  /* exactly one match */
        return cmdtp_temp;
    }

    return 0;   /* not found or ambiguous command */
}

/**
 * Call a command function.
 *
 * @param cmdtp     Pointer to the command to execute
 * @param argc      Number of arguments (arg 0 must be the command text)
 * @param argv      Arguments
 * @return 0 if command succeeded, else non-zero
 */
__STATIC_INLINE int cmd_call(cmd_tbl_t *cmdtp, int argc, char *const argv[])
{
    int result;
    result = (cmdtp->cmd)(argc, argv);
    if (result) {
        console_putf("Command failed, result=%d\n", result);
    }
    return result;
}

__STATIC_INLINE int cmd_process(int argc, char *const argv[])
{
    cmd_tbl_t *cmdtp;

    /* Look up command in command table */
    cmdtp = find_cmd(argv[0]);
    if (cmdtp == NULL) {
        console_putf("Unknown command '%s'\n", argv[0]);
        return -1;
    }

    /* found - check max args */
    if (argc > cmdtp->maxargs) {
        console_putf("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    /* If OK so far, then do the command */
    if (cmd_call(cmdtp, argc, argv)) {
        return -1;
    }

    return 0;
}

static int cmd_run(char *cmd)
{
    char *argv[CONFIG_SYS_MAXARGS + 1];    /* NULL terminated */
    int argc;

    /* separate into argv */
    argc = make_argv(cmd, argv);
    if (argc == 0) {
        return -1;    /* no command at all */
    }

    if (cmd_process(argc, argv)) {
        return -1;
    }

    return 0;
}

static char *delete_char(char *buffer, char *p, int *colp, int *np, int plen)
{
    char *s;

    if (*np == 0) {
        return p;
    }

    if (*(--p) == '\t') {           /* will retype the whole line */
        while (*colp > plen) {
            console_puts(erase_seq);
            (*colp)--;
        }
        for (s = buffer; s < p; ++s) {
            if (*s == '\t') {
                console_puts(tab_seq + ((*colp) & 0x07));
                *colp += 8 - ((*colp) & 0x07);
            } else {
                ++(*colp);
                console_putc(*s);
            }
        }
    } else {
        console_puts(erase_seq);
        (*colp)--;
    }
    (*np)--;

    return p;
}

static int readchar_into_buffer(const char c, char *prompt) {
    static char *p   = console_buffer;
    static int  n    = 0;   /* buffer index         */
    static int  plen = 0;   /* prompt length        */
    static int  col;        /* output column cnt    */

    if (prompt) {
        plen = strlen(prompt);
        p = console_buffer;
        n = 0;
        return 0;
    }
    col = plen;

    /*
     * Special character handling
     */
    switch (c) {
        case '\r':                  /* Enter            */
        case '\n':
            *p = '\0';
            console_puts("\r\n");
            return (p - console_buffer);

        case '\0':                  /* nul              */
            return -1;

        case 0x03:                  /* ^C - break       */
            console_buffer[0] = '\0';  /* discard input */
            return 0;

        case 0x15:                  /* ^U - erase line  */
            while (col > plen) {
                console_puts(erase_seq);
                --col;
            }
            p = console_buffer;
            n = 0;
            return -1;

        case 0x17:                  /* ^W - erase word  */
            p = delete_char(console_buffer, p, &col, &n, plen);
            while ((n > 0) && (*p != ' ')) {
                p = delete_char(console_buffer, p, &col, &n, plen);
            }
            return -1;

        case 0x08:                  /* ^H  - backspace  */
        case 0x7F:                  /* DEL - backspace  */
            p = delete_char(console_buffer, p, &col, &n, plen);
            return -1;

        default:
            /*
             * Must be a normal character then
             */
            if (n < CONFIG_SYS_CBSIZE - 2) {
                if (c == '\t') {    /* expand TABs      */
                    console_puts(tab_seq + (col & 0x07));
                    col += 8 - (col & 0x07);
                } else {
                    ++col;          /* Echo input       */
                    console_putc(c);
                }
                *p++ = c;
                ++n;
            } else {                /* Buffer full      */
                console_putc('\a');
            }
    }

    return -1;
}

/*
 * print data block
 */
void print_buffer(uint32_t addr, uint32_t count, uint8_t width, bool ascii_on)
{
    uint32_t nbytes;

    if ((count == 0) || !((width == 4) || (width == 2) || (width == 1))) {
        return;
    }
    addr &= ~((uint32_t)width - 0x01UL);
    nbytes = count * width;

    /* print lines */
    do {
        uint8_t  *str = (uint8_t *)addr;
        uint32_t linebytes = 0;
        uint32_t i = 0;

        console_putf("%08x:", addr);

        linebytes = (nbytes > CONFIG_LINE_BYTES) ? CONFIG_LINE_BYTES : nbytes;

        #if PLF_PMIC
        if (IS_PMIC_SRAM_VALID(addr) || IS_PMIC_REG_VALID(addr)) {
            ascii_on = false;
            for (i = 0; i < linebytes; i += 4) {
                uint32_t rdata = PMIC_MEM_READ(addr);
                if(width == 4) {
                    console_putf(" %08X", rdata);
                } else if(width == 2) {
                    console_putf(" %04X %04X", (uint16_t)rdata, (uint16_t)(rdata >> 16));
                } else {
                    console_putf(" %02X %02X %02X %02X", (uint8_t)rdata, (uint8_t)(rdata >> 8),
                        (uint8_t)(rdata >> 16), (uint8_t)(rdata >> 24));
                }
                addr += 4;
            }
        } else
        #endif
        for (i = 0; i < linebytes; i += width) {
            if(width == 4) {
                console_putf(" %08X", *((uint32_t *)addr));
            } else if(width == 2) {
                console_putf(" %04X", *((uint16_t *)addr));
            } else {
                console_putf(" %02X", *((uint8_t *)addr));
            }
            addr += width;
        }

        if (ascii_on) {
            for (i = linebytes; i < CONFIG_LINE_BYTES; i++) {
                if (((i - linebytes) & ((uint32_t)width - 0x01UL)) == 0x00UL) {
                    console_putc(' ');
                }
                console_puts("  ");
            }
            console_puts("    ");

            /* print ASCII */
            for (i = 0; i < (linebytes + 0); i++) {
                if (isascii(str[i]) && isprint(str[i])) {
                    console_putc(str[i]);
                } else {
                    console_putc('.');
                }
            }
        }

        console_puts("\r\n");
        nbytes -= linebytes;
    } while(nbytes > 0);
}

int do_help(int argc, char *argv[])
{
    cmd_tbl_t *cmdtemp = &cmd_tbl_list[0];    /*Init value */
    int i = 0;

    console_putf("\r\n");
    for (i = 1; i < cmd_tbl_curidx; i++) {
        cmdtemp = &cmd_tbl_list[i];
        if(cmdtemp->usage) {
            console_putf("%s\r\n", cmdtemp->usage);
        }
    }
    return 0;
}

int do_mem_read(int argc, char *argv[])
{
    uint32_t addr, cnt = 1;
    int size;

    if(argc < 2) {
        return -1;
    }

    addr = console_cmd_strtoul(argv[1], NULL, 16);

    if(argc > 2) {
        cnt = console_cmd_strtoul(argv[2], NULL, 0);
    }

    if (argc > 3) {
        size = console_cmd_strtoul(argv[3], NULL, 0);
        if ((4 != size) && (2 != size) && (1 != size)) {
            console_putf("Unknown data size:%d\n", size);
            return -2;
        }
    } else {
        size = 4;
    }

    /* Compatible with previous version which cnt always = 1 */
    if(cnt == 1) {
        #if PLF_PMIC
        if (IS_PMIC_SRAM_VALID(addr) || IS_PMIC_REG_VALID(addr)) {
            console_putf("[0x%08X] = 0x%08X\n", addr, PMIC_MEM_READ(addr));
        } else
        #endif
        if(size == 4) {
            console_putf("[0x%08lX] = 0x%08lX\n", addr & 0xFFFFFFFC,
                *((volatile uint32_t*)(addr & 0xFFFFFFFC)));
        } else if(size == 2) {
            console_putf("[0x%08lX] = 0x%04X\n", addr & 0xFFFFFFFE,
                *((volatile uint16_t*)(addr & 0xFFFFFFFE)));
        } else if(size == 1) {
            console_putf("[0x%08lX] = 0x%02X\n", addr,
                *((volatile uint8_t*)(addr)));
        }
        return 0;
    }

    print_buffer(addr, cnt, size, false);

    return 0;
}

int do_mem_write(int argc, char *argv[])
{
    uint32_t addr;
    uint32_t value;

    if ((argc < 3)) {
        return -1;
    }

    addr = console_cmd_strtoul(argv[1], NULL, 16);
    console_putf("Addr  = 0x%08lX\n", addr);

    value = console_cmd_strtoul(argv[2], NULL, 16);
    console_putf("Value = 0x%08lX\n", value);

    #if PLF_PMIC
    if (IS_PMIC_SRAM_VALID(addr) || IS_PMIC_REG_VALID(addr)) {
        PMIC_MEM_WRITE(addr, value);
    } else
    #endif
    *((volatile uint32_t *)addr) = value;
    console_putf("Write Done\n");

    return 0;
}

#if (PLF_PMIC && PLF_PMIC_VER_LITE && PLF_EXT_CS1000)
/*
 * print data block
 */
void aprint_buffer(uint32_t addr, uint32_t count, uint8_t width, bool ascii_on)
{
    uint32_t nbytes;

    if ((count == 0) || !((width == 4) || (width == 2) || (width == 1))) {
        return;
    }
    addr &= ~((uint32_t)width - 0x01UL);
    nbytes = count * width;

    /* print lines */
    do {
        uint8_t  *str = (uint8_t *)addr;
        uint32_t linebytes = 0;
        uint32_t i = 0;

        console_putf("%08x:", addr);

        linebytes = (nbytes > CONFIG_LINE_BYTES) ? CONFIG_LINE_BYTES : nbytes;

        if (IS_PMIC_SRAM_VALID(addr) || IS_PMIC_REG_VALID(addr)) {
            ascii_on = false;
            for (i = 0; i < linebytes; i += 4) {
                uint32_t rdata = psi_read_op(addr);
                if(width == 4) {
                    console_putf(" %08X", rdata);
                } else if(width == 2) {
                    console_putf(" %04X %04X", (uint16_t)rdata, (uint16_t)(rdata >> 16));
                } else {
                    console_putf(" %02X %02X %02X %02X", (uint8_t)rdata, (uint8_t)(rdata >> 8),
                        (uint8_t)(rdata >> 16), (uint8_t)(rdata >> 24));
                }
                addr += 4;
            }
        }

        if (ascii_on) {
            for (i = linebytes; i < CONFIG_LINE_BYTES; i++) {
                if (((i - linebytes) & ((uint32_t)width - 0x01UL)) == 0x00UL) {
                    console_putc(' ');
                }
                console_puts("  ");
            }
            console_puts("    ");

            /* print ASCII */
            for (i = 0; i < (linebytes + 0); i++) {
                if (isascii(str[i]) && isprint(str[i])) {
                    console_putc(str[i]);
                } else {
                    console_putc('.');
                }
            }
        }

        console_puts("\r\n");
        nbytes -= linebytes;
    } while(nbytes > 0);
}

int do_amem_read(int argc, char *argv[])
{
    uint32_t addr, cnt = 1;
    int size;

    if(argc < 2) {
        return -1;
    }

    addr = console_cmd_strtoul(argv[1], NULL, 16);

    if(argc > 2) {
        cnt = console_cmd_strtoul(argv[2], NULL, 0);
    }

    if (argc > 3) {
        size = console_cmd_strtoul(argv[3], NULL, 0);
        if ((4 != size) && (2 != size) && (1 != size)) {
            console_putf("Unknown data size:%d\n", size);
            return -2;
        }
    } else {
        size = 4;
    }

    if (!(IS_PMIC_SRAM_VALID(addr) || IS_PMIC_REG_VALID(addr))) {
        return -3;
    }

    /* Compatible with previous version which cnt always = 1 */
    if(cnt == 1) {
        if (IS_PMIC_SRAM_VALID(addr) || IS_PMIC_REG_VALID(addr)) {
            console_putf("[0x%08X] = 0x%08X\n", addr, psi_read_op(addr));
        }
        return 0;
    }

    aprint_buffer(addr, cnt, size, false);

    return 0;
}

int do_amem_write(int argc, char *argv[])
{
    uint32_t addr;
    uint32_t value;

    if ((argc < 3)) {
        return -1;
    }

    addr = console_cmd_strtoul(argv[1], NULL, 16);
    console_putf("Addr  = 0x%08lX\n", addr);

    value = console_cmd_strtoul(argv[2], NULL, 16);
    console_putf("Value = 0x%08lX\n", value);

    if (IS_PMIC_SRAM_VALID(addr) || IS_PMIC_REG_VALID(addr)) {
        psi_write_op(addr, value);
    } else {
        return -2;
    }
    console_putf("Write Done\n");

    return 0;
}
#endif

/**
 * Public function
 */
void command_init(void)
{
    int i;
    CONSOLE_CRITICAL_START();
    INIT_LIST_HEAD(&cmd_free_list);
    INIT_LIST_HEAD(&cmd_pend_list);
    for (i = 0; i < CONFIG_CMDBUF_SIZE; i++) {
        cmd_buf_node_t *buf = &(cmd_buf_tab[i]);
        list_add(&buf->entry, &cmd_free_list);
    }
    CONSOLE_CRITICAL_END();
    console_cmd_add("help", " help info", 1, do_help);
    console_cmd_add("r", "  r addr <cnt> <size>", 4, do_mem_read);
    console_cmd_add("w", "  w addr val", 3, do_mem_write);
    #if (PLF_PMIC && PLF_PMIC_VER_LITE && PLF_EXT_CS1000)
    console_cmd_add("ar", " ar addr <cnt> <size>", 4, do_amem_read);
    console_cmd_add("aw", " aw addr val", 3, do_amem_write);
    #endif
}

/**
 * called in isr
 */
int command_handle_char(char ch)
{
    int len = readchar_into_buffer(ch, 0);
    if (len >= 0) {
        if (len) {
            #if CONSOLE_GLOBAL_DEBUG_MODE
            if ((strlen(CMDISR_SUPER_STR) == len) && !strcmp(CMDISR_SUPER_STR, console_buffer)) {
                uint32_t regCTRL = __get_CONTROL();
                uint32_t regXPSR = __get_xPSR();
                uint32_t regPSP  = __get_PSP();
                uint32_t regMSP  = __get_MSP();
                uint32_t regPMSK = __get_PRIMASK();
                uint32_t regBPRI = __get_BASEPRI();
                console_putf("CONTROL=%08x, xPSR   =%08x\r\n"
                            "PSP    =%08x, MSP    =%08x\r\n"
                            "PRIMASK=%08x, BASEPRI=%08x\r\n",
                            regCTRL, regXPSR, regPSP, regMSP, regPMSK, regBPRI);
                if (regMSP) {
                    int32_t idx;
                    uint32_t addr = regMSP & ~0x0FUL;
                    console_putf("\nDumpMSP:");
                    for (idx = 0; idx < 64; idx++) {
                        if (!(addr & 0x0FUL)) {
                            console_putf("\n[%08x]:", addr);
                        }
                        console_putf(" %08X", *((uint32_t *)addr));
                        addr += 4;
                    }
                }
                if (regPSP) {
                    int32_t idx;
                    uint32_t addr = regPSP & ~0x0FUL;
                    console_putf("\nDumpPSP:");
                    for (idx = 0; idx < 64; idx++) {
                        if (!(addr & 0x0FUL)) {
                            console_putf("\n[%08x]:", addr);
                        }
                        console_putf(" %08X", *((uint32_t *)addr));
                        addr += 4;
                    }
                }
                len = 0;
                console_puts("\r\n" CONFIG_SYS_PROMPT);
            } else
            #endif /* CONSOLE_GLOBAL_DEBUG_MODE */
            if (!list_empty(&cmd_free_list)) {
                cmd_buf_node_t *buf = list_first_entry(&cmd_free_list, cmd_buf_node_t, entry);
                strcpy(buf->buffer, console_buffer);
                list_move_tail(&(buf->entry), &cmd_pend_list);
            } else {
                console_puts("err: cmd_free_list empty\r\n");
            }
        } else {
            console_puts(CONFIG_SYS_PROMPT);
        }
        readchar_into_buffer('0', CONFIG_SYS_PROMPT);
    }
    return len;
}

int command_add(const char *name, const char *usage, int maxargs, cmd_func_t func)
{
    cmd_tbl_t *tmp_cmd;

    if(CONFIG_CMDTBL_SIZE <= cmd_tbl_curidx) {
        console_putf("No more cmds supported.\n");
        return -1;
    }

    tmp_cmd = &(cmd_tbl_list[cmd_tbl_curidx]);
    cmd_tbl_curidx++;
    tmp_cmd->name = name;
    tmp_cmd->usage = usage;
    tmp_cmd->maxargs = maxargs;
    tmp_cmd->cmd = func;

    return 0;
}

/**
 * called in task
 */
void command_parser(void)
{
    cmd_buf_node_t *buf;

    CONSOLE_CRITICAL_START();
    buf = list_empty(&cmd_pend_list) ? NULL : list_first_entry(&cmd_pend_list, cmd_buf_node_t, entry);
    CONSOLE_CRITICAL_END();

    while (buf) {
        if(cmd_run(buf->buffer) < 0) {
            console_puts("command fail\r\n");
        }
        console_puts(CONFIG_SYS_PROMPT);

        CONSOLE_CRITICAL_START();
        list_move_tail(&(buf->entry), &cmd_free_list);
        buf = list_empty(&cmd_pend_list) ? NULL : list_first_entry(&cmd_pend_list, cmd_buf_node_t, entry);
        CONSOLE_CRITICAL_END();
    }
}

unsigned int command_strtoul(const char *cp, char **endp, unsigned int base)
{
    unsigned int result = 0, value;

    if (*cp == '0') {
        cp++;
        if ((tolower(*cp) == 'x') && isxdigit(*(cp + 1))) {
            base = 16;
            cp++;
        }
        if (!base) {
            base = 8;
        }
    }
    if (!base) {
        base = 10;
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp - '0' : (islower(*cp)
        ? toupper(*cp) : *cp) - 'A' + 10) < base) {
        result = result * base + value;
        cp++;
    }
    if (endp)
        *endp = (char *)cp;
    return result;
}

unsigned int command_pend_list_empty(void)
{
    int ret;
    CONSOLE_CRITICAL_START();
    ret = list_empty(&cmd_pend_list);
    CONSOLE_CRITICAL_END();
    return ret;
}

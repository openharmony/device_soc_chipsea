/*
 * Copyright (C) 2022 CHIPSEA TECHNOLOGY CO., LTD. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the
 *       distribution.
 *    3. Neither the name of CHIPSEA TECHNOLOGY CO., LTD. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include "fs_operations.h"
#include "los_config.h"
#include "hal_vfs.h"
#include "flash_api.h"


static struct lfs_manager *VfsOps = NULL;

static int lfs_block_read(const struct lfs_config *c, lfs_block_t block,
                          lfs_off_t off, void *dst, lfs_size_t size)
{
    uint32_t addr = c->block_size * block + off + VfsOps->start_addr;
    flash_read(addr, size, dst);
    return 0;
}

static int lfs_block_write(const struct lfs_config *c, lfs_block_t block,
                           lfs_off_t off, const void *dst, lfs_size_t size)
{
    uint32_t addr = c->block_size * block + off + VfsOps->start_addr;
    flash_write(addr, size, dst);
    return 0;
}

static int lfs_block_erase(const struct lfs_config *c, lfs_block_t block)
{
    uint32_t addr = c->block_size * block + VfsOps->start_addr;

    return flash_erase(addr, c->block_size);
}

static int lfs_block_sync(const struct lfs_config *c)
{
    return 0;
}

int32_t hal_vfs_init(void)
{
    printf("hal_vfs_init: +++++++++++++++++++++++++++++++++++++++++++++++ \n");
    VfsOps = malloc(sizeof(struct lfs_manager));
    if (VfsOps == NULL) {
        printf("+++ hal_vfs_init: NO memory!!\n");
        return -1;
    } else {
        memset(VfsOps, 0, sizeof(struct lfs_manager));
    }

    VfsOps->LfsOps.read = lfs_block_read;
    VfsOps->LfsOps.prog = lfs_block_write;
    VfsOps->LfsOps.erase = lfs_block_erase;
    VfsOps->LfsOps.sync = lfs_block_sync;
    VfsOps->LfsOps.read_size = 256;
    VfsOps->LfsOps.prog_size = 256;
    VfsOps->LfsOps.cache_size = 256;
    VfsOps->LfsOps.lookahead_size = 16;
    VfsOps->LfsOps.block_cycles = 500;
    VfsOps->start_addr = LFS_DEFAULT_START_ADDR;
    VfsOps->LfsOps.block_size = LFS_DEFAULT_BLOCK_SIZE;
    VfsOps->LfsOps.block_count = LFS_DEFAULT_BLOCK_COUNT;

    SetDefaultMountPath(0,"/data");
    if (LOS_FsMount(NULL, "/data", "littlefs", 0, VfsOps) != FS_SUCCESS) {
        printf("+++ hal_vfs_init: Mount littlefs faild!\n");
        free(VfsOps);
        return -1;
    }

    if (LOS_Mkdir("/data", 0777) != 0 ) {
        printf("+++ hal_vfs_init: Make dir faild!\n");
    }

    flash_user_data_addr_length_set(LFS_DEFAULT_START_ADDR,
            LFS_DEFAULT_BLOCK_SIZE * LFS_DEFAULT_BLOCK_COUNT);

    printf("+++ hal_vfs_init: Mount littlefs success!\n");
    return 0;
}

void hal_vfs_deinit(void)
{
    LOS_FsUmount(RootPath);
    free(VfsOps);
}



/*
 * Copyright (c) 2021 Chipsea Technologies (Shenzhen) Corp., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "hdf_device_desc.h"
#include "hdf_log.h"
#include "led_service.h"

#define HDF_LOG_TAG sample_driver

static const char *LedServiceGetData(void)
{
    return "sample";
}

static int32_t LedServiceSetData(void)
{

    HDF_LOGD("%s:%s", __func__);
    printf("led test start\r\n");
    return HDF_SUCCESS;
}

static void HdfLedDriverRelease(struct HdfDeviceObject *deviceObject)
{
    (void)deviceObject;
    return;
}

static int HdfLedDriverBind(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGD("%s::enter, deviceObject=%p", __func__, deviceObject);
    if (deviceObject == NULL) {
        return HDF_FAILURE;
    }
    #if 0
    static struct LedService LedService = {
        .getData = LedServiceGetData,
        .setData = LedServiceSetData,
    };
    deviceObject->service = &LedService.service;
    #endif
    return HDF_SUCCESS;
}

static int HdfLedDriverInit(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGD("%s::enter, deviceObject=%p", __func__, deviceObject);
    if (deviceObject == NULL) {
        HDF_LOGE("%s::ptr is null!", __func__);
        printf("%s::ptr is null!\r\n", __func__);
        return HDF_FAILURE;
    }
static struct LedService LedService = {
        .getData = LedServiceGetData,
        .setData = LedServiceSetData,
    };
    deviceObject->service = &LedService.service;
    HDF_LOGD("%s:Init success", __func__);
    printf("%s::ptr is null!\r\n", __func__);
    return HDF_SUCCESS;
}

struct HdfDriverEntry g_LedDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "led_driver",
    .Bind = HdfLedDriverBind,
    .Init = HdfLedDriverInit,
    .Release = HdfLedDriverRelease,
};

HDF_INIT(g_LedDriverEntry);

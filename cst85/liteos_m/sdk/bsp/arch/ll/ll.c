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
/*****************************************************************************/
/* File Includes                                                             */
/*****************************************************************************/

#include "ll.h"

/*****************************************************************************/
/* Variables                                                                 */
/*****************************************************************************/

static unsigned int critical_sect_count = 0U;
static unsigned int critical_ctxt_saved = 0U;

/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/

void critical_section_start(void)
{
    if (__get_IPSR() == 0U) {
        if  (0U == critical_sect_count) {
            critical_ctxt_saved = __get_PRIMASK();
            __disable_irq();
        }
        critical_sect_count++;
    }
}

void critical_section_end(void)
{
    if (__get_IPSR() == 0U) {
        critical_sect_count--;
        if  (0U == critical_sect_count) {
            __set_PRIMASK(critical_ctxt_saved);
        }
    }
}

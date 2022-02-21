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
#ifndef _PSIM_API_H_
#define _PSIM_API_H_

void psim_init(void);
unsigned int psim_read(unsigned int addr);
void psim_write(unsigned int addr, unsigned int data);
void psim_mask_write(unsigned int addr, unsigned int data, unsigned int mask);

#endif /* _PSIM_API_H_ */

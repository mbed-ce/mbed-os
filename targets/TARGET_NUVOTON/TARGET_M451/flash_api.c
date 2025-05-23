/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
 * SPDX-License-Identifier: Apache-2.0
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

#include "flash_api.h"
#include "flash_data.h"
#include "mbed_critical.h"
#include "M451_mem.h"

// This is a flash algo binary blob. It is PIC (position independent code) that should be stored in RAM
// NOTE: On ARMv7-M/ARMv8-M, instruction fetches are always little-endian.
static uint32_t FLASH_ALGO[] = {
    0x4603b530, 0x2164460c, 0x4dd72059, 0x20166028, 0xf8c5070d, 0x20880100, 0x0100f8c5, 0xf8d006c0, 
    0xf0000100, 0xb9080001, 0xbd302001, 0x680048cf, 0x0004f040, 0x4580f04f, 0x0200f8c5, 0xf8d04628, 
    0xf0400204, 0xf8c50004, 0xbf000204, 0xf1a11e08, 0xd1fb0101, 0x680048c6, 0x002df040, 0x60284dc4, 
    0x68004628, 0x0001f000, 0x2001b908, 0x48c0e7dd, 0xf0406800, 0x4dbe0040, 0x20006028, 0x4601e7d5, 
    0x48bbbf00, 0xf0006900, 0x28000001, 0x48b8d1f9, 0xf0206800, 0x4ab6002d, 0x20006010, 0x60104ab2, 
    0x46014770, 0x48b2bf00, 0xf0006900, 0x28000001, 0x48afd1f9, 0xf0406800, 0x4aad0040, 0x20226010, 
    0xf02160d0, 0x60500003, 0x61102001, 0x8f60f3bf, 0x48a7bf00, 0xf0006900, 0x28000001, 0x48a4d1f9, 
    0xf0006800, 0xb1380040, 0x680048a1, 0x0040f040, 0x60104a9f, 0x47702001, 0xe7fc2000, 0x4604b570, 
    0x4615460b, 0x46292200, 0x000ff103, 0x030ff020, 0x4897bf00, 0xf0006900, 0x28000001, 0x4894d1f9, 
    0xf0406800, 0x4e920040, 0xf0246030, 0x6070000f, 0x60f02027, 0x1c524610, 0x0020f851, 0x36804e8c, 
    0x46106030, 0xf8511c52, 0x4e890020, 0x0084f8c6, 0x1c524610, 0x0020f851, 0x36884e85, 0x46106030, 
    0xf8511c52, 0x1d360020, 0x20016030, 0x61304e80, 0xe02c3b10, 0x487ebf00, 0x680030c0, 0x0030f000, 
    0xd1f82800, 0x1c524610, 0x0020f851, 0x36804e78, 0x46106030, 0xf8511c52, 0x4e750020, 0x0084f8c6, 
    0x4873bf00, 0x680030c0, 0x00c0f000, 0xd1f82800, 0x1c524610, 0x0020f851, 0x36884e6d, 0x46106030, 
    0xf8511c52, 0x4e6a0020, 0x008cf8c6, 0x2b003b10, 0xbf00d1d0, 0x69004866, 0x0001f000, 0xd1f92800, 
    0xb510bd70, 0x1cc84603, 0x0103f020, 0x4860bf00, 0xf0006900, 0x28000001, 0x485dd1f9, 0xf0406800, 
    0x4c5b0040, 0x20216020, 0xe02060e0, 0x0003f023, 0x60604c57, 0x60a06810, 0x61202001, 0x8f60f3bf, 
    0x4853bf00, 0xf0006900, 0x28000001, 0x4850d1f9, 0xf0006800, 0xb1380040, 0x6800484d, 0x0040f040, 
    0x60204c4b, 0xbd102001, 0x1d121d1b, 0x29001f09, 0x2000d1dc, 0xe92de7f7, 0x460547f0, 0x4616460c, 
    0x0800f04f, 0xbf0046c2, 0x69004841, 0x0001f000, 0xd1f92800, 0x6800483e, 0x0040f040, 0x6008493c, 
    0xf0201ce0, 0xe02d0403, 0xb958b2e8, 0xd9092cff, 0x7780f44f, 0x0208eb06, 0x46284639, 0xff2ef7ff, 
    0xe0164682, 0x0008f3c5, 0x2c10b958, 0xf024d309, 0xeb06070f, 0x46390208, 0xf7ff4628, 0x4682ff1f, 
    0x4627e007, 0x0208eb06, 0x46284639, 0xff89f7ff, 0x443d4682, 0x1be444b8, 0x0f00f1ba, 0x2001d002, 
    0x87f0e8bd, 0xd1cf2c00, 0xe7f92000, 0x1ccbb510, 0x0103f023, 0x4b1ebf00, 0xf003691b, 0x2b000301, 
    0x4b1bd1f9, 0xf043681b, 0x4c190340, 0x23006023, 0xe02560e3, 0x0303f020, 0x60634c15, 0x60a32300, 
    0x61232301, 0x8f60f3bf, 0x4b11bf00, 0xf003691b, 0x2b000301, 0x4b0ed1f9, 0xf003681b, 0xb1330340, 
    0x681b4b0b, 0x0340f043, 0x60234c09, 0x4b08bd10, 0x6814689b, 0xd00042a3, 0x1d00e7f8, 0x1f091d12, 
    0xd1d72900, 0xe7f1bf00, 0x40000100, 0x40000200, 0x4000c000, 0x00000000, 
};

static const flash_algo_t flash_algo_config = {
    .init = 0x00000001,
    .uninit = 0x0000007f,
    .erase_sector = 0x000000a3,
    .program_page = 0x00000257,
    .static_base = 0x00000374,
    .algo_blob = FLASH_ALGO
};

static const sector_info_t sectors_info[] = {
    {MBED_ROM_BANK_IROM1_START, 0x800},        // (start, sector size)
};

static const flash_target_config_t flash_target_config = {
    .page_size  = 4,                // 4 bytes
                                    // Here page_size is program unit, which is different than FMC definition.
    .flash_start = MBED_ROM_BANK_IROM1_START,
    .flash_size = MBED_ROM_BANK_IROM1_SIZE,
    .sectors = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
}

/* Copyright (c) 2024 Jamie Smith
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


#include "CompositeEMAC.h"

void mbed::CompositeEMAC::get_ifname(char *name, uint8_t size) const {
    // Note that LwIP only supports a two character interface name prefix.
    // So, no point in going longer than that.
    // Also note that we don't want to copy the terminating null if it doesn't fit.
    const char * const ifPrefix = "en";
    memcpy(name, ifPrefix, (size < strlen(ifPrefix) + 1) ? size : strlen(ifPrefix) + 1);
}

void mbed::CompositeEMAC::set_hwaddr(const uint8_t *addr) {
    if(mac.se)
}

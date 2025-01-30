/* mbed Microcontroller Library
* Copyright (c) 2006-2013 ARM Limited
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

uint32_t mcgirc_frequency(void) {
    uint32_t mcgirc_clock = 0;

    if (MCG->C1 & MCG_C1_IREFSTEN_MASK) {
        mcgirc_clock  = (MCG->C2 & MCG_C2_IRCS_MASK) ? 8000000u : 2000000u;
        mcgirc_clock /= 1u + ((MCG->SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT);
        mcgirc_clock /= 1u +  (MCG->MC & MCG_MC_LIRC_DIV2_MASK);
    }

    return mcgirc_clock;
}
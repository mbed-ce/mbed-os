/** Copyright (c) 2025 MbedCE Community Contributors (Jan Kamidra)
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

/* The cmsis_os2.h have to be used instead of obsolete version cmsis_os.h 
which is included in many files so this file is just a wrapper because RTX 5.9 does not
contain it any more. */
#include "cmsis_os2.h"
/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#include "GEMALTO_CINTERION_CellularContext.h"
#include "GEMALTO_CINTERION_CellularStack.h"
#include "CellularLog.h"
#include "CellularDevice.h"

namespace mbed {

GEMALTO_CINTERION_CellularContext::GEMALTO_CINTERION_CellularContext(ATHandler &at, CellularDevice *device,
                                                                     const char *apn, bool cp_req, bool nonip_req) : AT_CellularContext(at, device, apn, cp_req, nonip_req)
{
}

GEMALTO_CINTERION_CellularContext::~GEMALTO_CINTERION_CellularContext()
{
}

#if !NSAPI_PPP_AVAILABLE
NetworkStack *GEMALTO_CINTERION_CellularContext::get_stack()
{
    if (_pdp_type == NON_IP_PDP_TYPE || _cp_in_use) {
        tr_error("Requesting stack for NON-IP context! Should request control plane netif: get_cp_netif()");
        return NULL;
    }

    if (!_stack) {
        _stack = new GEMALTO_CINTERION_CellularStack(_at, _apn, _uname, _pwd, _cid, (nsapi_ip_stack_t)_pdp_type, *get_device());
        if (static_cast<GEMALTO_CINTERION_CellularStack *>(_stack)->socket_stack_init() != NSAPI_ERROR_OK) {
            delete _stack;
            _stack = NULL;
        }
    }
    return _stack;
}
#endif // NSAPI_PPP_AVAILABLE

nsapi_error_t GEMALTO_CINTERION_CellularContext::do_user_authentication()
{
    // if user has defined user name and password we need to call CGAUTH before activating or modifying context
    if (_pwd && _uname) {
        if (!get_device()->get_property(AT_CellularDevice::PROPERTY_AT_CGAUTH)) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        _at.at_cmd_discard("^SGAUTH", "=", "%d%d%s%s", _cid, _authentication_type, _uname, _pwd);

        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_AUTH_FAILURE;
        }
    }

    return NSAPI_ERROR_OK;
}

void GEMALTO_CINTERION_CellularContext::enable_access_technology()
{
    if (!_rat.has_value()) {
        return;
    }

    switch (*_rat)
    {
    case CATM1:
        _at.at_cmd_discard("^SXRAT", "=","%d", *_rat);
        _at.cmd_start_stop("^SCFG", "=","%s%d", "Radio/Band/CatM", *_band);
        _at.resp_start("^SCFG");
        _at.cmd_start_stop("^SCFG", "=","%s%d%d", "Radio/Band/CatNB",0,0);
        _at.resp_start("^SCFG");
        break;

    case CATNB:
        _at.at_cmd_discard("^SXRAT", "=","%d", *_rat);
        _at.cmd_start_stop("^SCFG", "=","%s%d", "Radio/Band/CatNB", *_band);
        _at.resp_start("^SCFG");
        _at.cmd_start_stop("^SCFG", "=","%s%d%d", "Radio/Band/CatM",0,0);
        _at.resp_start("^SCFG");
        break;

    default:
        break;
    }

    _at.cmd_start_stop("^SCFG", "=", "%s%s", "Tcp/withURCs", "on");
    _at.resp_start("^SCFG");
}

} /* namespace mbed */

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
#include "GEMALTO_CINTERION_CellularInformation.h"
#include "GEMALTO_CINTERION_CellularNetwork.h"
#include "GEMALTO_CINTERION.h"
#include "AT_CellularNetwork.h"
#include "CellularLog.h"

using namespace mbed;
using namespace events;

GEMALTO_CINTERION::Module GEMALTO_CINTERION::_module;

GEMALTO_CINTERION::GEMALTO_CINTERION(FileHandle *fh) : AT_CellularDevice(fh)
{
}

time_t GEMALTO_CINTERION::get_time()
{
    tr_info("GEMALTO_CINTERION::get_time\n");

    _at.lock();

    //"+CCLK: \"%y/%m/%d,%H:%M:%S+ZZ"
    _at.cmd_start_stop("+CCLK", "?");
    _at.resp_start("+CCLK:");

    char time_str[50];
    time_t now = 0;
    while (_at.info_resp()) {
        int date_len = _at.read_string(time_str, sizeof(time_str));
        tr_debug("Read %d bytes for the date\n", date_len);
        if (date_len > 0) {
            now = parse_time(time_str);
        }
    }
    _at.resp_stop();

    _at.unlock();

    // adjust for timezone offset which is +/- in 15 minute increments
    time_t delta = ((time_str[18] - '0') * 10 + (time_str[19] - '0')) * (15 * 60);

    if (time_str[17] == '-') {
        now = now + delta;
    } else if (time_str[17] == '+') {
        now = now - delta;
    }

    return now;
}

time_t GEMALTO_CINTERION::get_local_time()
{
    tr_info("GEMALTO_CINTERION::get_local_time\n");

    _at.lock();

    //"+CCLK: \"%y/%m/%d,%H:%M:%S"
    _at.cmd_start_stop("+CCLK", "?");
    _at.resp_start("+CCLK:");

    char time_str[50];
    time_t now;
    while (_at.info_resp()) {
        int date_len = _at.read_string(time_str, sizeof(time_str));
        tr_debug("Read %d bytes for the date\n", date_len);
        if (date_len > 0) {
            now = parse_time(time_str);
        }
    }
    _at.resp_stop();

    _at.unlock();

    return now;
}

void GEMALTO_CINTERION::set_time(time_t const epoch, int const timezone)
{
    char time_buf[21];
    strftime(time_buf, sizeof(time_buf), "%g/%m/%d,%H:%M:%S", gmtime(&epoch));
    snprintf(time_buf + 17, 4, "%+03d", timezone);

    _at.lock();
    _at.at_cmd_discard("+CCLK", "=", "%s", time_buf);
    _at.unlock();
}


AT_CellularContext *GEMALTO_CINTERION::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return new GEMALTO_CINTERION_CellularContext(at, this, apn, cp_req, nonip_req);
}

AT_CellularInformation *GEMALTO_CINTERION::open_information_impl(ATHandler &at)
{
    if (_module == ModuleBGS2) {
        return new GEMALTO_CINTERION_CellularInformation(at, *this);
    }
    return AT_CellularDevice::open_information_impl(at);
}

AT_CellularNetwork *GEMALTO_CINTERION::open_network_impl(ATHandler &at)
{
    return new GEMALTO_CINTERION_CellularNetwork(at, *this, _module);
}

nsapi_error_t GEMALTO_CINTERION::init()
{
    nsapi_error_t err = AT_CellularDevice::init();
    if (err != NSAPI_ERROR_OK) {
        return err;
    }

    CellularInformation *information = open_information();
    if (!information) {
        return NSAPI_ERROR_NO_MEMORY;
    }
    char model[sizeof("EHS5-E") + 1]; // sizeof need to be long enough to hold just the model text
    nsapi_error_t ret = information->get_model(model, sizeof(model));
    close_information();
    if (ret != NSAPI_ERROR_OK) {
        tr_error("Cellular model not found!");
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (memcmp(model, "ELS61", sizeof("ELS61") - 1) == 0) {
        init_module_els61();
    } else if (memcmp(model, "BGS2", sizeof("BGS2") - 1) == 0) {
        init_module_bgs2();
    } else if (memcmp(model, "EMS31", sizeof("EMS31") - 1) == 0) {
        init_module_ems31();
    } else if (memcmp(model, "EHS5-E", sizeof("EHS5-E") - 1) == 0) {
        init_module_ehs5e();
    } else if (memcmp(model, "TX62", sizeof("TX62") - 1) == 0) {
        init_module_tx62();
    } else {
        tr_error("Cinterion model unsupported %s", model);
        return NSAPI_ERROR_UNSUPPORTED;
    }
    tr_info("Cinterion model %s (%d)", model, _module);

    set_at_urcs();

    return NSAPI_ERROR_OK;
}

GEMALTO_CINTERION::Module GEMALTO_CINTERION::get_module()
{
    return _module;
}

void GEMALTO_CINTERION::init_module_bgs2()
{
    // BGS2-W_ATC_V00.100
    static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
        AT_CellularNetwork::RegistrationModeDisable,  // C_EREG
        AT_CellularNetwork::RegistrationModeEnable,  // C_GREG
        AT_CellularNetwork::RegistrationModeLAC,  // C_REG
        0,  // AT_CGSN_WITH_TYPE
        1,  // AT_CGDATA
        1,  // AT_CGAUTH
        1,  // AT_CNMI
        1,  // AT_CSMP
        1,  // AT_CMGF
        1,  // AT_CSDH
        1,  // PROPERTY_IPV4_STACK
        0,  // PROPERTY_IPV6_STACK
        0,  // PROPERTY_IPV4V6_STACK
        0,  // PROPERTY_NON_IP_PDP_TYPE
        1,  // PROPERTY_AT_CGEREP
        1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
        10, // PROPERTY_SOCKET_COUNT
        1,  // PROPERTY_IP_TCP
        1,  // PROPERTY_IP_UDP
        100,// PROPERTY_AT_SEND_DELAY, if baud is below 9600 this must be longer
    };
    set_cellular_properties(cellular_properties);
    _module = ModuleBGS2;
}

void GEMALTO_CINTERION::init_module_els61()
{
    // ELS61-E2_ATC_V01.000
    static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
        AT_CellularNetwork::RegistrationModeDisable, // C_EREG
        AT_CellularNetwork::RegistrationModeEnable,  // C_GREG
        AT_CellularNetwork::RegistrationModeLAC,     // C_REG
        0,  // AT_CGSN_WITH_TYPE
        1,  // AT_CGDATA
        1,  // AT_CGAUTH
        1,  // AT_CNMI
        1,  // AT_CSMP
        1,  // AT_CMGF
        1,  // AT_CSDH
        1,  // PROPERTY_IPV4_STACK
        1,  // PROPERTY_IPV6_STACK
        0,  // PROPERTY_IPV4V6_STACK
        0,  // PROPERTY_NON_IP_PDP_TYPE
        1,  // PROPERTY_AT_CGEREP
        1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
        10, // PROPERTY_SOCKET_COUNT
        1,  // PROPERTY_IP_TCP
        1,  // PROPERTY_IP_UDP
        100,// PROPERTY_AT_SEND_DELAY, if baud is below 9600 this must be longer
    };
    set_cellular_properties(cellular_properties);
    _module = ModuleELS61;
}

void GEMALTO_CINTERION::init_module_ems31()
{
    // EMS31-US_ATC_V4.9.5
    static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
        AT_CellularNetwork::RegistrationModeLAC,        // C_EREG
        AT_CellularNetwork::RegistrationModeDisable,    // C_GREG
        AT_CellularNetwork::RegistrationModeDisable,    // C_REG
        1,  // AT_CGSN_WITH_TYPE
        1,  // AT_CGDATA
        1,  // AT_CGAUTH
        1,  // AT_CNMI
        1,  // AT_CSMP
        1,  // AT_CMGF
        1,  // AT_CSDH
        1,  // PROPERTY_IPV4_STACK
        1,  // PROPERTY_IPV6_STACK
        1,  // PROPERTY_IPV4V6_STACK
        0,  // PROPERTY_NON_IP_PDP_TYPE
        1,  // PROPERTY_AT_CGEREP
        1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
        10, // PROPERTY_SOCKET_COUNT
        1,  // PROPERTY_IP_TCP
        1,  // PROPERTY_IP_UDP
        100,// PROPERTY_AT_SEND_DELAY, if baud is below 9600 this must be longer
    };
    set_cellular_properties(cellular_properties);
    _module = ModuleEMS31;
}

void GEMALTO_CINTERION::init_module_ehs5e()
{
    // EHS5-E
    static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
        AT_CellularNetwork::RegistrationModeDisable, // C_EREG
        AT_CellularNetwork::RegistrationModeLAC, // C_GREG
        AT_CellularNetwork::RegistrationModeLAC, // C_REG
        0,  // AT_CGSN_WITH_TYPE
        1,  // AT_CGDATA
        1,  // AT_CGAUTH
        1,  // AT_CNMI
        1,  // AT_CSMP
        1,  // AT_CMGF
        1,  // AT_CSDH
        1,  // PROPERTY_IPV4_STACK
        1,  // PROPERTY_IPV6_STACK
        0,  // PROPERTY_IPV4V6_STACK
        0,  // PROPERTY_NON_IP_PDP_TYPE
        1,  // PROPERTY_AT_CGEREP
        1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
        10, // PROPERTY_SOCKET_COUNT
        1,  // PROPERTY_IP_TCP
        1,  // PROPERTY_IP_UDP
        100,// PROPERTY_AT_SEND_DELAY, if baud is below 9600 this must be longer
    };
    set_cellular_properties(cellular_properties);
    _module = ModuleEHS5E;
}

void GEMALTO_CINTERION::init_module_tx62()
{
    // TX-62
    static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
        AT_CellularNetwork::RegistrationModeLAC,// C_EREG
        AT_CellularNetwork::RegistrationModeDisable,    // C_GREG
        AT_CellularNetwork::RegistrationModeDisable,    // C_REG
        0,  // AT_CGSN_WITH_TYPE
        0,  // AT_CGDATA
        1,  // AT_CGAUTH
        1,  // AT_CNMI
        1,  // AT_CSMP
        1,  // AT_CMGF
        1,  // AT_CSDH
        1,  // PROPERTY_IPV4_STACK
        0,  // PROPERTY_IPV6_STACK
        0,  // PROPERTY_IPV4V6_STACK
        0,  // PROPERTY_NON_IP_PDP_TYPE
        1,  // PROPERTY_AT_CGEREP
        1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
        7,  // PROPERTY_SOCKET_COUNT
        1,  // PROPERTY_IP_TCP
        1,  // PROPERTY_IP_UDP
        0,  // PROPERTY_AT_SEND_DELAY
    };
    set_cellular_properties(cellular_properties);
    _module = ModuleTX62;

    // Enable network time zone updates
    _at.at_cmd_discard("+CTZU", "=", "%d", 1);
}

time_t GEMALTO_CINTERION::parse_time(char const *time_str) {
    struct tm now;

    now.tm_year = std::strtol(time_str, NULL, 10) + 100; // mktime starts from 1900
    time_str += 3; // Skip '/'
    now.tm_mon = std::strtol(time_str, NULL, 10);
    time_str += 3; // Skip '/'
    now.tm_mday = std::strtol(time_str, NULL, 10);
    time_str += 3; // Skip ','
    now.tm_hour = std::strtol(time_str, NULL, 10);
    time_str += 3; // Skip ':'
    now.tm_min = std::strtol(time_str, NULL, 10);
    time_str += 3;
    now.tm_sec = std::strtol(time_str, NULL, 10);

    tr_debug("Year: %d, month: %d, day:%d, hour:%d, minute:%d, second:%d\n", now.tm_year, now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);

    return mktime(&now);
}

#if MBED_CONF_GEMALTO_CINTERION_PROVIDE_DEFAULT
#include "drivers/BufferedSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static BufferedSerial serial(MBED_CONF_GEMALTO_CINTERION_TX, MBED_CONF_GEMALTO_CINTERION_RX, MBED_CONF_GEMALTO_CINTERION_BAUDRATE);
#if defined(MBED_CONF_GEMALTO_CINTERION_RTS) && defined(MBED_CONF_GEMALTO_CINTERION_CTS)
    tr_debug("GEMALTO_CINTERION flow control: RTS %d CTS %d", MBED_CONF_GEMALTO_CINTERION_RTS, MBED_CONF_GEMALTO_CINTERION_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_GEMALTO_CINTERION_RTS, MBED_CONF_GEMALTO_CINTERION_CTS);
#endif
    static GEMALTO_CINTERION device(&serial);
    return &device;
}

/*
 * With e.g. GCC linker option "--undefined=<LINK_FOO>", pull in this
 * object file anyway for being able to override weak symbol successfully
 * even though from static library. See:
 * https://stackoverflow.com/questions/42588983/what-does-the-gnu-ld-undefined-option-do
 *
 * NOTE: For C++ name mangling, 'extern "C"' is necessary to match the
 *       <LINK_FOO> symbol correctly.
 */
extern "C"
void LINK_GEMALTO_CINTERION_CPP(void)
{
}
#endif

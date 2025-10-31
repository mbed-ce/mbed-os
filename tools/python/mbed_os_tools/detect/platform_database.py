# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Functions that manage a platform database"""

import datetime
import json
import re
from collections import OrderedDict, defaultdict
from copy import copy
from io import open
from os import makedirs
from os.path import join, dirname, getmtime
from appdirs import user_data_dir
from fasteners import InterProcessLock

import logging

logger = logging.getLogger("mbedls.platform_database")
logger.addHandler(logging.NullHandler())
del logging

try:
    unicode
except NameError:
    unicode = str

LOCAL_PLATFORM_DATABASE = join(user_data_dir("mbedls"), "platforms.json")
LOCAL_MOCKS_DATABASE = join(user_data_dir("mbedls"), "mock.json")

DEFAULT_PLATFORM_DB = {
    "daplink": {
        "0200": "KL25Z",
        "0201": "KW41Z",
        "0210": "KL05Z",
        "0214": "HEXIWEAR",
        "0217": "K82F",
        "0218": "KL82Z",
        "0220": "KL46Z",
        "0227": "MIMXRT1050_EVK",
        "0228": "RAPIDIOT_K64F",
        "0230": "K20D50M",
        "0231": "K22F",
        "0234": "RAPIDIOT_KW41Z",
        "0236": "LPC55S69",
        "0240": "K64F",
        "0245": "K64F",
        "0250": "KW24D",
        "0261": "KL27Z",
        "0262": "KL43Z",
        "0300": "MTS_GAMBIT",
        "0305": "MTS_MDOT_F405RG",
        "0310": "MTS_DRAGONFLY_F411RE",
        "0311": "K66F",
        "0312": "MTS_DRAGONFLY_L471QG",
        "0313": "MTS_DRAGONFLY_L496VG",
        "0315": "MTS_MDOT_F411RE",
        "0316": "MTS_DRAGONFLY_F413RH",
        "0350": "XDOT_L151CC",
        "0360": "HANI_IOT",
        "0400": "MAXWSNENV",
        "0405": "MAX32600MBED",
        "0407": "MAX32620HSP",
        "0408": "MAX32625NEXPAQ",
        "0409": "MAX32630FTHR",
        "0410": "ETTEPLAN_LORA",
        "0415": "MAX32625MBED",
        "0416": "MAX32625PICO",
        "0418": "MAX32620FTHR",
        "0419": "MAX35103EVKIT2",
        "0421": "MAX32660EVSYS",
        "0424": "MAX32670EVKIT",
        "0450": "MTB_UBLOX_ODIN_W2",
        "0451": "MTB_MXCHIP_EMW3166",
        "0452": "MTB_LAIRD_BL600",
        "0453": "MTB_MTS_XDOT",
        "0454": "MTB_MTS_DRAGONFLY",
        "0455": "MTB_UBLOX_NINA_B1",
        "0456": "MTB_MURATA_ABZ",
        "0457": "MTB_RAK811",
        "0458": "ADV_WISE_1510",
        "0459": "ADV_WISE_1530",
        "0460": "ADV_WISE_1570",
        "0461": "MTB_LAIRD_BL652",
        "0462": "MTB_USI_WM_BN_BM_22",
        "0465": "MTB_LAIRD_BL654",
        "0466": "MTB_MURATA_WSM_BL241",
        "0467": "MTB_STM_S2LP",
        "0468": "MTB_STM_L475",
        "0469": "MTB_STM32_F439",
        "0472": "MTB_ACONNO_ACN52832",
        "0602": "EV_COG_AD3029LZ",
        "0603": "EV_COG_AD4050LZ",
        "0604": "SDP_K1",
        "0700": "NUCLEO_F103RB",
        "0705": "NUCLEO_F302R8",
        "0710": "NUCLEO_L152RE",
        "0715": "NUCLEO_L053R8",
        "0720": "NUCLEO_F401RE",
        "0725": "NUCLEO_F030R8",
        "0729": "NUCLEO_G071RB",
        "0730": "NUCLEO_F072RB",
        "0735": "NUCLEO_F334R8",
        "0740": "NUCLEO_F411RE",
        "0742": "NUCLEO_F413ZH",
        "0743": "DISCO_F413ZH",
        "0744": "NUCLEO_F410RB",
        "0745": "NUCLEO_F303RE",
        "0746": "DISCO_F303VC",
        "0747": "NUCLEO_F303ZE",
        "0750": "NUCLEO_F091RC",
        "0755": "NUCLEO_F070RB",
        "0760": "NUCLEO_L073RZ",
        "0764": "DISCO_L475VG_IOT01A",
        "0765": "NUCLEO_L476RG",
        "0766": "SILICA_SENSOR_NODE",
        "0770": "NUCLEO_L432KC",
        "0774": "DISCO_L4R9I",
        "0775": "NUCLEO_F303K8",
        "0776": "NUCLEO_L4R5ZI",
        "0777": "NUCLEO_F446RE",
        "0778": "NUCLEO_F446ZE",
        "0779": "NUCLEO_L433RC_P",
        "0780": "NUCLEO_L011K4",
        "0781": "NUCLEO_L4R5ZI_P",
        "0783": "NUCLEO_L010RB",
        "0785": "NUCLEO_F042K6",
        "0788": "DISCO_F469NI",
        "0790": "NUCLEO_L031K6",
        "0791": "NUCLEO_F031K6",
        "0795": "DISCO_F429ZI",
        "0796": "NUCLEO_F429ZI",
        "0797": "NUCLEO_F439ZI",
        "0805": "DISCO_L053C8",
        "0810": "DISCO_F334C8",
        "0812": "NUCLEO_F722ZE",
        "0813": "NUCLEO_H743ZI",
        "0814": "DISCO_H747I",
        "0815": "DISCO_F746NG",
        "0816": "NUCLEO_F746ZG",
        "0817": "DISCO_F769NI",
        "0818": "NUCLEO_F767ZI",
        "0820": "DISCO_L476VG",
        "0821": "NUCLEO_L452RE",
        "0822": "DISCO_L496AG",
        "0823": "NUCLEO_L496ZG",
        "0824": "LPC824",
        "0825": "DISCO_F412ZG",
        "0826": "NUCLEO_F412ZG",
        "0827": "NUCLEO_L486RG",
        "0828": "NUCLEO_L496ZG_P",
        "0829": "NUCLEO_L452RE_P",
        "0830": "DISCO_F407VG",
        "0832": "NUCLEO_H503RB",
        "0833": "DISCO_L072CZ_LRWAN1",
        "0835": "NUCLEO_F207ZG",
        "0836": "NUCLEO_H743ZI2",
        "0839": "NUCLEO_WB55RG",
        "0840": "B96B_F446VE",
        "0841": "NUCLEO_G474RE",
        "0842": "NUCLEO_H753ZI",
        "0843": "NUCLEO_H745ZI_Q",
        "0844": "NUCLEO_H755ZI_Q",
        "0847": "DISCO_H745I",
        "0849": "NUCLEO_G070RB",
        "0850": "NUCLEO_G431RB",
        "0851": "NUCLEO_G431KB",
        "0852": "NUCLEO_G031K8",
        "0853": "NUCLEO_F301K8",
        "0854": "NUCLEO_L552ZE_Q",
        "0855": "DISCO_L562QE",
        "0858": "DISCO_H750B",
        "0859": "DISCO_H7B3I",
        "0860": "NUCLEO_H7A3ZI_Q",
        "0863": "DISCO_L4P5G",
        "0865": "NUCLEO_L4P5ZG",
        "0866": "NUCLEO_WL55JC",
        "0871": "NUCLEO_H723ZG",
        "0872": "NUCLEO_G0B1RE",
        "0875": "DISCO_H735G",
        "0878": "NUCLEO_H563ZI",
        "0879": "NUCLEO_F756ZG",
        "0882": "NUCLEO_G491RE",
        "0883": "NUCLEO_WB15CC",
        "0884": "DISCO_WB5MMG",
        "0885": "B_L4S5I_IOT01A",
        "0886": "NUCLEO_U575ZI_Q",
        "0887": "B_U585I_IOT02A",
        "0888": "NUCLEO_U083RC",
        "0900": "SAMR21G18A",
        "0905": "SAMD21G18A",
        "0910": "SAML21J18A",
        "0915": "SAMD21J18A",
        "1000": "LPC2368",
        "1010": "LPC1768",
        "1017": "HRM1017",
        "1018": "SSCI824",
        "1019": "TY51822R3",
        "1022": "RO359B",
        "1034": "LPC11U34",
        "1040": "LPC11U24",
        "1045": "LPC11U24",
        "1050": "LPC812",
        "1054": "LPC54114",
        "1056": "LPC546XX",
        "1060": "LPC4088",
        "1061": "LPC11U35_401",
        "1062": "LPC4088_DM",
        "1070": "NRF51822",
        "1075": "NRF51822_OTA",
        "1080": "OC_MBUINO",
        "1090": "RBLAB_NRF51822",
        "1093": "RBLAB_BLENANO2",
        "1095": "RBLAB_BLENANO",
        "1100": "NRF51_DK",
        "1101": "NRF52_DK",
        "1102": "NRF52840_DK",
        "1105": "NRF51_DK_OTA",
        "1114": "LPC1114",
        "1120": "NRF51_DONGLE",
        "1130": "NRF51822_SBK",
        "1140": "WALLBOT_BLE",
        "1168": "LPC11U68",
        "1200": "NCS36510",
        "1234": "UBLOX_C027",
        "1236": "UBLOX_EVK_ODIN_W2",
        "1237": "UBLOX_EVK_NINA_B1",
        "1280": "OKDO_ODIN_W2",
        "1300": "NUC472-NUTINY",
        "1301": "NUMBED",
        "1302": "NUMAKER_PFM_NUC472",
        "1303": "NUMAKER_PFM_M453",
        "1304": "NUMAKER_PFM_M487",
        "1305": "NU_PFM_M2351",
        "1306": "NUMAKER_PFM_NANO130",
        "1307": "NUMAKER_PFM_NUC240",
        "1308": "NUMAKER_IOT_M487",
        "1309": "NUMAKER_IOT_M252",
        "1310": "NUMAKER_IOT_M263A",
        "1312": "NU_M2354",
        "1313": "NUMAKER_IOT_M467",
        "1500": "RHOMBIO_L476DMW1K",
        "1549": "LPC1549",
        "1600": "LPC4330_M4",
        "1605": "LPC4330_M4",
        "1900": "CY8CKIT_062_WIFI_BT",
        "1901": "CY8CPROTO_062_4343W",
        "1902": "CY8CKIT_062_BLE",
        "1903": "CYW9P62S1_43012EVB_01",
        "1904": "CY8CPROTO_063_BLE",
        "1905": "CY8CKIT_062S2_4343W",
        "1906": "CYW943012P6EVB_01",
        "1907": "CY8CPROTO_064_SB",
        "1908": "CYW9P62S1_43438EVB_01",
        "1909": "CY8CPROTO_062S2_43012",
        "190A": "CY8CKIT_064S2_4343W",
        "190B": "CY8CKIT_062S2_43012",
        "190C": "CY8CPROTO_064B0S3",
        "190E": "CY8CPROTO_062S3_4343W",
        "190F": "CY8CPROTO_064B0S1_BLE",
        "1910": "CY8CKIT064B0S2_4343W",
        "1911": "CY8CKIT064S0S2_4343W",
        "1912": "CYSBSYSKIT_01",
        "2000": "EFM32_G8XX_STK",
        "2005": "EFM32HG_STK3400",
        "2010": "EFM32WG_STK3800",
        "2015": "EFM32GG_STK3700",
        "2020": "EFM32LG_STK3600",
        "2025": "EFM32TG_STK3300",
        "2030": "EFM32ZG_STK3200",
        "2035": "EFM32PG_STK3401",
        "2040": "EFM32PG12_STK3402",
        "2041": "TB_SENSE_12",
        "2042": "EFM32GG11_STK3701",
        "2043": "EFM32TG11_STK3301",
        "2045": "TB_SENSE_1",
        "2100": "XBED_LPC1768",
        "2201": "WIZWIKI_W7500",
        "2202": "WIZWIKI_W7500ECO",
        "2203": "WIZWIKI_W7500P",
        "2600": "EP_AGORA",
        "3001": "LPC11U24",
        "3101": "SDT32620B",
        "3102": "SDT32625B",
        "3103": "SDT51822B",
        "3104": "SDT52832B",
        "3105": "SDT64B",
        "3701": "S5JS100",
        "3702": "S3JT100",
        "3703": "S1SBP6A",
        "4000": "LPC11U35_Y5_MBUG",
        "4005": "NRF51822_Y5_MBUG",
        "4100": "MOTE_L152RC",
        "4337": "LPC4337",
        "4500": "DELTA_DFCM_NNN40",
        "4501": "DELTA_DFBM_NQ620",
        "4502": "DELTA_DFCM_NNN50",
        "4600": "REALTEK_RTL8195AM",
        "5000": "ARM_MPS2",
        "5001": "ARM_IOTSS_BEID",
        "5002": "ARM_BEETLE_SOC",
        "5003": "ARM_MPS2_M0P",
        "5004": "ARM_CM3DS_MPS2",
        "5005": "ARM_MPS2_M0DS",
        "5006": "ARM_MUSCA_A1",
        "5007": "ARM_MUSCA_B1",
        "5009": "ARM_MUSCA_S1",
        "5020": "HOME_GATEWAY_6LOWPAN",
        "5500": "RZ_A1H",
        "5501": "GR_LYCHEE",
        "5502": "GR_MANGO",
        "6000": "FUTURE_SEQUANA",
        "6660": "NZ32_SC151",
        "7011": "TMPM066",
        "7012": "TMPM3H6",
        "7013": "TMPM46B",
        "7014": "TMPM3HQ",
        "7015": "TMPM4G9",
        "7020": "TMPM4KN",
        "7402": "MBED_BR_HAT",
        "7778": "TEENSY3_1",
        "8001": "UNO_91H",
        "8012": "TT_M3HQ",
        "8013": "TT_M4G9",
        "8080": "FF1705_L151CC",
        "8081": "FF_LPC546XX",
        "9001": "LPC1347",
        "9002": "LPC11U24",
        "9003": "LPC1347",
        "9004": "ARCH_PRO",
        "9006": "LPC11U24",
        "9007": "LPC11U35_501",
        "9008": "XADOW_M0",
        "9009": "ARCH_BLE",
        "9010": "ARCH_GPRS",
        "9011": "ARCH_MAX",
        "9012": "SEEED_TINY_BLE",
        "9014": "WIO_3G",
        "9015": "WIO_BG96",
        "9017": "WIO_EMW3166",
        "9020": "UHURU_RAVEN",
        "9900": "NRF51_MICROBIT",
        "C002": "VK_RZ_A1H",
        "C005": "MTM_MTCONNECT04S",
        "C006": "VBLUNO51",
        "C008": "SAKURAIO_EVB_01",
        "C030": "UBLOX_C030_U201",
        "C031": "UBLOX_C030_N211",
        "C032": "UBLOX_C030_R404M",
        "C033": "UBLOX_C030_R410M",
        "C034": "UBLOX_C030_S200",
        "C035": "UBLOX_C030_R3121",
        "C036": "UBLOX_C030_R412M",
        "RIOT": "RIOT",
    },
    "jlink": {
        "X729475D28G": {"platform_name": "NRF51_DK", "jlink_device_name": "nRF51422_xxAC"},
        "X349858SLYN": {"platform_name": "NRF52_DK", "jlink_device_name": "nRF52832_xxaa"},
        "FRDM-KL25Z": {"platform_name": "KL25Z", "jlink_device_name": "MKL25Z128xxx4"},
        "FRDM-KL27Z": {"platform_name": "KL27Z", "jlink_device_name": "MKL27Z64xxx4"},
        "FRDM-KL43Z": {"platform_name": "KL43Z", "jlink_device_name": "MKL43Z256xxx4"},
    },
    "atmel": {"2241": "SAML21J18A"},
}


def _get_modified_time(path):
    try:
        mtime = getmtime(path)
    except OSError:
        mtime = 0
    return datetime.datetime.fromtimestamp(mtime)


def _older_than_me(path):
    return _get_modified_time(path) < _get_modified_time(__file__)


def _modify_data_format(data, verbose_data, simple_data_key="platform_name"):
    if isinstance(data, dict):
        if verbose_data:
            return data

        return data[simple_data_key]
    else:
        if verbose_data:
            return {simple_data_key: data}

        return data


def _overwrite_or_open(db):
    try:
        if db is LOCAL_PLATFORM_DATABASE and _older_than_me(db):
            raise ValueError("Platform Database is out of date")
        with open(db, encoding="utf-8") as db_in:
            return json.load(db_in)
    except (IOError, ValueError) as exc:
        if db is LOCAL_PLATFORM_DATABASE:
            logger.warning("Error loading database %s: %s; Recreating", db, str(exc))
            try:
                makedirs(dirname(db))
            except OSError:
                pass
            try:
                with open(db, "w", encoding="utf-8") as out:
                    out.write(unicode(json.dumps(DEFAULT_PLATFORM_DB)))
            except IOError:
                pass
            return copy(DEFAULT_PLATFORM_DB)
        else:
            return {}


class PlatformDatabase(object):
    """Represents a union of multiple platform database files.
    Handles inter-process synchronization of database files.
    """

    target_id_pattern = re.compile(r"^[a-fA-F0-9]{4}$")

    def __init__(self, database_files, primary_database=None):
        """Construct a PlatformDatabase object from a series of platform database
        files
        """
        self._prim_db = primary_database
        if not self._prim_db and len(database_files) == 1:
            self._prim_db = database_files[0]
        self._dbs = OrderedDict()
        self._keys = defaultdict(set)
        for db in database_files:
            new_db = _overwrite_or_open(db)
            first_value = None
            if new_db.values():
                first_value = next(iter(new_db.values()))
                if not isinstance(first_value, dict):
                    new_db = {"daplink": new_db}

            if new_db:
                for device_type in new_db:
                    duplicates = self._keys[device_type].intersection(set(new_db[device_type].keys()))
                    duplicates = set(["%s.%s" % (device_type, k) for k in duplicates])
                    if duplicates:
                        logger.warning(
                            "Duplicate platform ids found: %s, ignoring the definitions from %s",
                            " ".join(duplicates),
                            db,
                        )
                    self._dbs[db] = new_db
                    self._keys[device_type] = self._keys[device_type].union(new_db[device_type].keys())
            else:
                self._dbs[db] = new_db

    def items(self, device_type="daplink"):
        for db in self._dbs.values():
            for entry in db.get(device_type, {}).items():
                yield entry

    def all_ids(self, device_type="daplink"):
        return iter(self._keys[device_type])

    def get(self, index, default=None, device_type="daplink", verbose_data=False):
        """Standard lookup function. Works exactly like a dict. If 'verbose_data'
        is True, all data for the platform is returned as a dict."""
        for db in self._dbs.values():
            if device_type in db:
                maybe_answer = db[device_type].get(index, None)
                if maybe_answer:
                    return _modify_data_format(maybe_answer, verbose_data)

        return default

    def _update_db(self):
        if self._prim_db:
            lock = InterProcessLock("%s.lock" % self._prim_db)
            acquired = lock.acquire(blocking=False)
            if not acquired:
                logger.debug("Waiting 60 seconds for file lock")
                acquired = lock.acquire(blocking=True, timeout=60)
            if acquired:
                try:
                    with open(self._prim_db, "w", encoding="utf-8") as out:
                        out.write(unicode(json.dumps(self._dbs[self._prim_db])))
                    return True
                finally:
                    lock.release()
            else:
                logger.error("Could not update platform database: Lock acquire failed after 60 seconds")
                return False
        else:
            logger.error("Can't update platform database: destination database is ambiguous")
            return False

    def add(self, id, platform_name, permanent=False, device_type="daplink"):
        """Add a platform to this database, optionally updating an origin
        database
        """
        if self.target_id_pattern.match(id):
            if self._prim_db:
                if device_type not in self._dbs[self._prim_db]:
                    self._dbs[self._prim_db][device_type] = {}
                self._dbs[self._prim_db][device_type][id] = platform_name
            else:
                cur_db = next(iter(self._dbs.values()))
                if device_type not in cur_db:
                    cur_db[device_type] = {}
                cur_db[device_type][id] = platform_name
            self._keys[device_type].add(id)
            if permanent:
                self._update_db()
        else:
            raise ValueError("Invald target id: %s" % id)

    def remove(self, id, permanent=False, device_type="daplink", verbose_data=False):
        """Remove a platform from this database, optionally updating an origin
        database. If 'verbose_data' is True, all data for the platform is returned
        as a dict.
        """
        logger.debug("Trying remove of %s", id)
        if id == "*" and device_type in self._dbs[self._prim_db]:
            self._dbs[self._prim_db][device_type] = {}
            if permanent:
                self._update_db()
        else:
            for db in self._dbs.values():
                if device_type in db and id in db[device_type]:
                    logger.debug("Removing id...")
                    removed = db[device_type][id]
                    del db[device_type][id]
                    self._keys[device_type].remove(id)
                    if permanent:
                        self._update_db()

                    return _modify_data_format(removed, verbose_data)

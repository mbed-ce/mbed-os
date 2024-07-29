/*
 * Copyright 2024 Arduino SA
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

#include <wiced_filesystem.h>
#include <minimal_cyhal_config.h>

#include <mbed-target-config.h>
#include <mbed_error.h>
#include <QSPIFBlockDevice.h>
#include <MBRBlockDevice.h>
#include <FATFileSystem.h>

MBED_WEAK void wiced_filesystem_mount_error(void)
{
    error("Failed to mount the filesystem containing the WiFi firmware.\n\r");
}

MBED_WEAK void wiced_filesystem_firmware_error(void)
{
    error("WICED wifi module firmware not found at path " WIFI_DEFAULT_FIRMWARE_PATH " on the external block device.  Perhaps it needs to be installed via your board's instructions?\n\r");
}

MBED_WEAK wiced_result_t whd_firmware_check_hook(const char *mounted_name, int mount_err)
{
    DIR *dir;
    struct dirent *ent;
    std::string dir_name(mounted_name);
    if (mount_err) {
        wiced_filesystem_mount_error();
    } else {
        if ((dir = opendir(mounted_name)) != NULL) {
            // print all the files and directories within directory
            while ((ent = readdir(dir)) != NULL) {
                std::string fullname =  "/" + dir_name + "/" + std::string(ent->d_name);
                if (fullname == WIFI_DEFAULT_FIRMWARE_PATH) {
                    closedir(dir);
                    return WICED_SUCCESS;
                }
            }
            closedir(dir);
        }
        wiced_filesystem_firmware_error();
    }
    return WICED_ERROR;
}

wiced_filesystem_t resource_fs_handle = 0;

wiced_result_t wiced_filesystem_setup()
{
    static QSPIFBlockDevice *qspi_bd = nullptr;
    static mbed::MBRBlockDevice *mbr_bd = nullptr;
    static mbed::FATFileSystem *wifi_fs = nullptr;

    // First initialize QSPI flash
    if(qspi_bd == nullptr)
    {
        qspi_bd = new QSPIFBlockDevice(); // default settings OK thanks to JSON configuration
        if(qspi_bd->init() != mbed::BD_ERROR_OK)
        {
            delete qspi_bd;
            qspi_bd = nullptr;
            return WICED_ERROR;
        }
    }

    // Then initialize MBR block device on it
    if(mbr_bd == nullptr)
    {
        mbr_bd = new mbed::MBRBlockDevice(qspi_bd, WIFI_DEFAULT_PARTITION);
        if(mbr_bd->init() != mbed::BD_ERROR_OK)
        {
            whd_firmware_check_hook(WIFI_DEFAULT_MOUNT_NAME, true);
            delete mbr_bd;
            mbr_bd = nullptr;
            return WICED_ERROR;
        }
    }

    // Finally initialize FAT file system on MBR partition
    if(wifi_fs == nullptr)
    {
        wifi_fs = new mbed::FATFileSystem(WIFI_DEFAULT_MOUNT_NAME);
        if(wifi_fs->mount(mbr_bd) != mbed::BD_ERROR_OK)
        {
            whd_firmware_check_hook(WIFI_DEFAULT_MOUNT_NAME, true);
            delete mbr_bd;
            mbr_bd = nullptr;
            return WICED_ERROR;
        }
    }

    whd_firmware_check_hook(WIFI_DEFAULT_MOUNT_NAME, false);

    return WICED_SUCCESS;
}

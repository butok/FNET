/**************************************************************************
*
* Copyright 2017 by Andrey Butok. FNET Community.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
***************************************************************************
*
*  FNET Shell Demo (Wi-Fi).
*
***************************************************************************/

#include "fapp.h"

#if FNET_CFG_CPU_WIFI

#include "fapp_prv.h"
#include "fapp_wifi.h"

/************************************************************************
* DESCRIPTION: Connect to Wi-Fi access point.
*************************************************************************/
void fapp_wifi_connect_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_wifi_connect_params_t  connect_params;

    connect_params.ssid = fapp_params_wifi_config.ssid;
    connect_params.wpa_passphrase = fapp_params_wifi_config.passphrase;

    /* Change default parameters. */
    if(argc >= 2u)
    {
        connect_params.ssid = argv[1];

        if(argc >= 3u)
        {
            connect_params.wpa_passphrase = argv[2];
        }
    }

    if(fnet_wifi_connect(FNET_CPU_WIFI_IF, &connect_params) == FNET_ERR)
    {
        fnet_shell_println(desc, "Wi-Fi connection is failed.");
    }
    else
    {
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
        fnet_shell_println(desc, " Connecting to Wi-Fi access point:");
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "SSID",  connect_params.ssid );
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Passphrase", connect_params.wpa_passphrase );
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
    }
}

/************************************************************************
* DESCRIPTION: Turn on Wi-Fi Access Point.
*************************************************************************/
void fapp_wifi_ap_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_wifi_access_point_params_t access_point_params;
    fnet_index_t                    i;

    /* Default Wi-Fi Access Point parameters */
    fnet_memset_zero(&access_point_params, sizeof(access_point_params));
    access_point_params.ssid = FAPP_CFG_WIFI_AC_SSID;
    access_point_params.wpa_passphrase = FAPP_CFG_WIFI_AC_WPA_PASSPHRASE;
    access_point_params.wpa_mode = FAPP_CFG_WIFI_AC_WPA_MODE;

    /* Change default parameters. */
    if(argc >= 2u)
    {
        access_point_params.ssid = argv[1];

        if(argc >= 3u)
        {
            access_point_params.wpa_passphrase = argv[2];

            if(argc >= 4u)
            {
                if (!fnet_strcmp(argv[3], "wpa"))
                {
                    access_point_params.wpa_mode = FNET_WIFI_WPA;
                }
                else if (!fnet_strcmp(argv[3], "wpa2"))
                {
                    access_point_params.wpa_mode = FNET_WIFI_WPA2;
                }
                else
                {
                    i = 3;
                    goto ERROR_PARAMETER;
                }
            }
        }
    }

    if(fnet_wifi_access_point(FNET_CPU_WIFI_IF, &access_point_params) == FNET_ERR)
    {
        fnet_shell_println(desc, "Wi-Fi Access Point is failed.");
    }
    else
    {
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
        fnet_shell_println(desc, " Switching to Wi-Fi access point mode:");
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "SSID",  access_point_params.ssid );
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Passphrase", access_point_params.wpa_passphrase );
        fnet_shell_println(desc, FAPP_DELIMITER_STR);

        /* Set default IPv4 parameters for Wi-Fi AP mode */
        fnet_netif_set_ip4_addr(FNET_CPU_WIFI_IF, FAPP_CFG_WIFI_AP_IP4_ADDR, FAPP_CFG_WIFI_AP_IP4_MASK);
        fnet_netif_set_ip4_gateway(FNET_CPU_WIFI_IF, FAPP_CFG_WIFI_AP_IP4_GW);
#if FNET_CFG_DNS
        fnet_netif_set_ip4_dns(FNET_CPU_WIFI_IF, FAPP_CFG_WIFI_AP_IP4_DNS);
#endif
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

/************************************************************************
* DESCRIPTION: Turn off the Wi-Fi interface radio.
*************************************************************************/
void fapp_wifi_disconnect_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    if(fnet_wifi_disconnect(FNET_CPU_WIFI_IF) == FNET_ERR)
    {
        fnet_shell_println(desc, "Wi-Fi disconnection is failed.");
    }
}

#if FNET_CFG_CPU_WIFI_FW_UPDATE

/* Wi-Fi firmware image */
#include <qca_firmware.h>
#define FAPP_WIFI_FW_IMAGE QCA_FW_BUFFER

/************************************************************************
* DESCRIPTION: Wi-Fi firmware update.
*************************************************************************/
void fapp_wifi_fw_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_return_t result;

    fnet_shell_println(desc, "Wi-Fi FW update. Do not turn off...");

    result = fnet_wifi_fw_update(FNET_CPU_WIFI_IF, FAPP_WIFI_FW_IMAGE, sizeof(FAPP_WIFI_FW_IMAGE));

    fnet_shell_println(desc, "Wi-Fi FW update is %s", (result == FNET_OK) ? "completed" : "failed");
}

#endif /* FNET_CFG_CPU_WIFI_FW_UPDATE */

#endif /* FNET_CFG_CPU_WIFI */

/**************************************************************************
*
* Copyright 2017-2018 by Andrey Butok. FNET Community.
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
*  Wi-Fi QCA4002 module driver implementation.
*
***************************************************************************/

#include "fnet.h"
#if FNET_CFG_CPU_WIFI

#include "stack/fnet_eth_prv.h"
#include "fnet_qca.h"
#include "stack/fnet_wifi_prv.h"

/* QCA includes */
#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <htc.h>
#include <atheros_wifi_api.h>
#include <atheros_wifi_internal.h>
#include <hw20_apb_map.h>
#include <hw20_mbox_reg.h>
#include <hw40_rtc_reg.h>
#include <targaddrs.h>
#include <bmi.h>
#include <bmi_msg.h>

#if FNET_CFG_DEBUG_QCA && FNET_CFG_DEBUG
    #define FNET_DEBUG_QCA   FNET_DEBUG_PRINTF
#else
    #define FNET_DEBUG_QCA(...)   do{}while(0)
#endif

/* FNET does not use QCA off-load stack */
#if ENABLE_STACK_OFFLOAD
    #error ENABLE_STACK_OFFLOAD must be set to 0 in a_config.h
#endif

#if QOSAL_TASK_DESTRUCTION == 0
    #error QOSAL_TASK_DESTRUCTION must be set to 1 in a_config.h
#endif

#define FNET_QCA_DEVICE_ID     0 /* Device ID */

/************************************************************************
*     Function Prototypes
*************************************************************************/
static fnet_return_t _fnet_qca_init(fnet_netif_t *netif);
static void _fnet_qca_release(fnet_netif_t *netif);
static fnet_return_t _fnet_qca_get_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr);
static fnet_return_t _fnet_qca_get_statistics(fnet_netif_t *netif, struct fnet_netif_statistics *statistics);
static fnet_bool_t _fnet_qca_is_connected(fnet_netif_t *netif);
static fnet_return_t _fnet_qca_set_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr);
static fnet_return_t _fnet_qca_wifi_connect(struct fnet_netif *netif, fnet_wifi_connect_params_t *params);
static fnet_return_t _fnet_qca_wifi_access_point(fnet_netif_t *netif, fnet_wifi_access_point_params_t *params);
static fnet_return_t _fnet_qca_wifi_disconnect(struct fnet_netif *netif);
static void _fnet_qca_on_connect(uint8_t event, uint8_t devId, char *bssid, uint8_t bssConn);
static fnet_return_t _fnet_qca_get_ssid_info(const char *ssid, WLAN_AUTH_MODE *auth_mode, WLAN_CRYPT_TYPE *encrypt_mode);
static fnet_wifi_op_mode_t _fnet_qca_get_op_mode(struct fnet_netif *netif);
static fnet_uint32_t _fnet_qca_fw_get_version(struct fnet_netif *netif);
static void _fnet_qca_input(void *cookie);
static fnet_return_t _fnet_qca_wifi_get_country_code(fnet_netif_t *netif, fnet_char_t *country_code);
static fnet_return_t _fnet_qca_wifi_set_country_code(fnet_netif_t *netif, const fnet_char_t *country_code);
#if FNET_CFG_CPU_WIFI_FW_UPDATE
    static fnet_return_t _fnet_qca_wifi_fw_update(fnet_netif_t *netif, const fnet_uint8_t *fw_buffer, fnet_size_t fw_buffer_size);
#endif

/************************************************************************
*     Global Data Structures
*************************************************************************/

/******************************************************************************
 * Wi-Fi QCA4002/4 interface API structure.
 ******************************************************************************/
const fnet_wifi_api_t fnet_qca_wifi_api =
{
    .wifi_connect = _fnet_qca_wifi_connect,
    .wifi_access_point = _fnet_qca_wifi_access_point,
    .wifi_disconnect =  _fnet_qca_wifi_disconnect,
    .wifi_get_op_mode = _fnet_qca_get_op_mode,
    .wifi_fw_get_version = _fnet_qca_fw_get_version,
    .wifi_set_country_code = _fnet_qca_wifi_set_country_code,
    .wifi_get_country_code = _fnet_qca_wifi_get_country_code,
#if FNET_CFG_CPU_WIFI_FW_UPDATE
    .wifi_fw_update = _fnet_qca_wifi_fw_update,
#endif
};

/*****************************************************************************
 * Wi-Fi QCA4002 network-interface general API structure.
 ******************************************************************************/
const fnet_netif_api_t fnet_qca_api =
{
    .netif_type = FNET_NETIF_TYPE_WIFI,                         /* Data-link type. */
    .netif_hw_addr_size = sizeof(fnet_mac_addr_t),
    .netif_init = _fnet_qca_init,                                /* Initialization function.*/
    .netif_release = _fnet_qca_release,                          /* Shutdown function.*/
#if FNET_CFG_IP4
    .netif_output_ip4 = _fnet_eth_output_ip4,                    /* IPv4 Transmit function.*/
#endif
    .netif_change_addr_notify = _fnet_eth_change_addr_notify,    /* Address change notification function.*/
    .netif_drain = _fnet_eth_drain,                              /* Drain function.*/
    .netif_get_hw_addr = _fnet_qca_get_hw_addr,
    .netif_set_hw_addr = _fnet_qca_set_hw_addr,
    .netif_is_connected = _fnet_qca_is_connected,
    .netif_get_statistics = _fnet_qca_get_statistics,
#if FNET_CFG_MULTICAST
#if FNET_CFG_IP4
    .netif_multicast_join_ip4 = _fnet_eth_multicast_join_ip4,
    .netif_multicast_leave_ip4 = _fnet_eth_multicast_leave_ip4,
#endif
#if FNET_CFG_IP6
    .netif_multicast_join_ip6 = _fnet_eth_multicast_join_ip6,
    .netif_multicast_leave_ip6 = _fnet_eth_multicast_leave_ip6,
#endif
#endif
#if FNET_CFG_IP6
    .netif_output_ip6 = _fnet_eth_output_ip6,           /* IPv6 Transmit function.*/
#endif
    .wifi_api = &fnet_qca_wifi_api,
};

/* QCA4002 Module control data structure.*/
fnet_qca_if_t fnet_qca_if;

/*****************************************************************************
 * This is the WIFI function pointer struct defined in QCA driver file cust_api_init.c
 ******************************************************************************/
extern const QCA_MAC_IF_STRUCT ATHEROS_WIFI_IF;

extern ATH_CUSTOM_INIT_T ath_custom_init;
extern uint32_t ar4XXX_boot_param;
/* To access internal functions */
extern uint32_t Custom_Api_Mediactl(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t command_id, void *inout_param);

const QCA_IF_STRUCT qca_0 =
{
    .MAC_IF       = &ATHEROS_WIFI_IF,   /* pointer to MAC interface struct */
    .MAC_NUMBER   = 1,                  /* MAC device number */
    .PHY_NUMBER   = 1,                  /* MAC device number for communication with PHY */
    .PHY_ADDRESS  = 0,                  /* PHY address */
};

const QCA_PARAM_STRUCT fnet_qca_default_params =
{
    .QCA_IF            = &qca_0,
    .MODE              = Auto_Negotiate,
    .OPTIONS           = 0,
    .NUM_RX_PCBS       = WLAN_CONFIG_NUM_PRE_ALLOC_RX_BUFFERS,  //  # Number of RX PCBs (Packet Control Blocks)
};


/* QCA driver context */
QCA_CONTEXT_STRUCT          wifiCtx; /* TBD: make it part of control structure */
static WLAN_AUTH_MODE       fnet_qca_auth_mode = WLAN_AUTH_NONE;
static QCOM_WLAN_DEV_MODE   fnet_qca_dev_mode = QCOM_WLAN_DEV_MODE_STATION;

/************************************************************************
* DESCRIPTION: QCA4002 Wi-Fi module initialization.
*************************************************************************/
static fnet_return_t _fnet_qca_init(fnet_netif_t *netif)
{
    FNET_ASSERT(netif != FNET_NULL);
    FNET_ASSERT(netif->netif_prv != FNET_NULL);

    fnet_return_t   result = FNET_ERR;
    fnet_qca_if_t   *qca_if;
    uint32_t        res;

    qca_if = (fnet_qca_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

    FNET_ASSERT(qca_if != FNET_NULL);

    /* Clear control structure. */
    fnet_memset_zero(qca_if, sizeof(*qca_if));

    qca_if->input_event = fnet_event_init(_fnet_qca_input, qca_if);

    if(qca_if->input_event)
    {
        WIFISHIELD_Init(); /* Initialize Wi-Fi shield. */

        /* Initialize QCA driver */

        /* Power off the WLAN and wait 1000ms*/
        CUSTOM_HW_POWER_UP_DOWN(NULL, false);
        A_MDELAY(1000);

        /* Initialize WIFI driver by calling Custom_Api_Initialize() which
        * allocates memory for various structures and starts the driver RTOS task
        * represented by Atheros_Driver_Task() function.
        * The driver task establishes SPI communication with the QCA400x chip and sets
        * up various registers of the WLAN chip.
        * Note that this initialization involves the currently running task to wait
        * for the ready signal from the Atheros_Driver_Task() where upon we return
        * from the function call. This process takes approximately 60ms.*/
        wifiCtx.PARAM_PTR = &fnet_qca_default_params;
        res = ATHEROS_WIFI_IF.INIT(&wifiCtx);
        if (res != 0)
        {
            FNET_DEBUG_QCA("[QCA] ERROR: Failed to initialize driver\r\n");
            fnet_event_release(qca_if->input_event);
        }
        else
        {
            FNET_DEBUG_QCA("[QCA] Initialized\r\n");

#if FNET_CFG_DEBUG_QCA && FNET_CFG_DEBUG
            {
                ATH_VERSION_STR versionstr;
                if(A_FAILED(qcom_get_versionstr(&versionstr)))
                {
                    FNET_DEBUG_QCA("ERROR: Failed to get firmware version\r\n");
                    return FNET_ERR;
                }

                if((ar4XXX_boot_param & AR4XXX_PARAM_MODE_BMI) == AR4XXX_PARAM_MODE_BMI)
                {
                    /* FW version number in BMI mode is O. */
                    FNET_DEBUG_QCA("[QCA] BMI Mode\r\n");
                    FNET_DEBUG_QCA("[QCA] Target version %s\r\n", versionstr.target_ver);
                }
                else
                {
                    FNET_DEBUG_QCA("[QCA] Normal Mode\r\n");
                    /* Print Firmware version number */
                    FNET_DEBUG_QCA("[QCA] Firmware version %s\r\n", versionstr.wlan_ver);
                    /* In this mode, target version number is not real.*/
                }
            }
#endif

            qca_if->netif = netif;

            A_MDELAY(100); /* Suggested delay. Nobody knows why. */

            result = FNET_OK;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: QCA4002 Wi-Fi module release.
*************************************************************************/
static void _fnet_qca_release(fnet_netif_t *netif)
{
    FNET_ASSERT(netif != FNET_NULL);
    FNET_ASSERT(netif->netif_prv != FNET_NULL);

    if(fnet_qca_if.netif) /* If initialized */
    {
        if((fnet_qca_if.is_connected == FNET_TRUE)
           && (_fnet_qca_wifi_disconnect(netif) != FNET_OK))
        {
            FNET_DEBUG_QCA("ERROR: qcom_disconnect failed\r\n");
        }
        ATHEROS_WIFI_IF.STOP(&wifiCtx);

        fnet_event_release(fnet_qca_if.input_event);

        fnet_qca_if.netif = FNET_NULL;
    }

    FNET_DEBUG_QCA("[QCA] Release\r\n");
}

/************************************************************************
* DESCRIPTION: Prints QCA4002 scan info.
*************************************************************************/
#if FNET_CFG_DEBUG_QCA && FNET_CFG_DEBUG
static void fnet_qca_print_ssid_info(QCA_SCAN_INFO_PTR  scan_info)
{
    FNET_ASSERT(scan_info != FNET_NULL);
    FNET_ASSERT(scan_info->ssid_len < FNET_WIFI_SSID_SIZE);

    uint8_t temp_ssid[FNET_WIFI_SSID_SIZE];

    fnet_memcpy(temp_ssid, scan_info->ssid, scan_info->ssid_len);
    temp_ssid[scan_info->ssid_len] = '\0';

    if (scan_info->ssid_len == 0)
    {
        FNET_DEBUG_QCA("ssid = SSID Not available\r\n");
    }
    else
    {
        FNET_DEBUG_QCA("\t ssid = %s\r\n", temp_ssid);

        FNET_DEBUG_QCA("\t bssid = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\r\n", scan_info->bssid[0],
                       scan_info->bssid[1], scan_info->bssid[2],
                       scan_info->bssid[3], scan_info->bssid[4],
                       scan_info->bssid[5]);
        FNET_DEBUG_QCA("\t channel = %d\r\n", scan_info->channel);

        FNET_DEBUG_QCA("\t indicator = %d\r\n", scan_info->rssi);

        FNET_DEBUG_QCA("\t security = ");

        if (scan_info->security_enabled)
        {
            if (scan_info->rsn_auth || scan_info->rsn_cipher)
            {
                FNET_DEBUG_QCA("YES\r\n");
                FNET_DEBUG_QCA("\t RSN/WPA2= ");
            }

            if (scan_info->rsn_auth)
            {
                FNET_DEBUG_QCA(" { ");
                if (scan_info->rsn_auth & SECURITY_AUTH_1X)
                {
                    FNET_DEBUG_QCA("802.1X ");
                }

                if (scan_info->rsn_auth & SECURITY_AUTH_PSK)
                {
                    FNET_DEBUG_QCA("PSK ");
                }
                FNET_DEBUG_QCA("}");
            }

            if (scan_info->rsn_cipher)
            {
                FNET_DEBUG_QCA(" { ");
                /* AP security can support multiple options hence
                * we check each one separately. Note rsn == wpa2 */
                if (scan_info->rsn_cipher & ATH_CIPHER_TYPE_WEP)
                {
                    FNET_DEBUG_QCA("WEP ");
                }

                if (scan_info->rsn_cipher & ATH_CIPHER_TYPE_TKIP)
                {
                    FNET_DEBUG_QCA("TKIP ");
                }

                if (scan_info->rsn_cipher & ATH_CIPHER_TYPE_CCMP)
                {
                    FNET_DEBUG_QCA("AES ");
                }
                FNET_DEBUG_QCA("}");
            }

            if (scan_info->wpa_auth || scan_info->wpa_cipher)
            {
                FNET_DEBUG_QCA("\r\n");
                FNET_DEBUG_QCA("\t WPA= ");
            }

            if (scan_info->wpa_auth)
            {
                FNET_DEBUG_QCA(" { ");
                if (scan_info->wpa_auth & SECURITY_AUTH_1X)
                {
                    FNET_DEBUG_QCA("802.1X ");
                }

                if (scan_info->wpa_auth & SECURITY_AUTH_PSK)
                {
                    FNET_DEBUG_QCA("PSK ");
                }
                FNET_DEBUG_QCA("}");
            }

            if (scan_info->wpa_cipher)
            {
                FNET_DEBUG_QCA(" { ");
                if (scan_info->wpa_cipher & ATH_CIPHER_TYPE_WEP)
                {
                    FNET_DEBUG_QCA("WEP ");
                }

                if (scan_info->wpa_cipher & ATH_CIPHER_TYPE_TKIP)
                {
                    FNET_DEBUG_QCA("TKIP ");
                }

                if (scan_info->wpa_cipher & ATH_CIPHER_TYPE_CCMP)
                {
                    FNET_DEBUG_QCA("AES ");
                }
                FNET_DEBUG_QCA("}");
            }
            /* It may be old-fashioned WEP this is identified by
            * absent wpa and rsn ciphers */
            if (scan_info->rsn_cipher == 0 && scan_info->wpa_cipher == 0)
            {
                FNET_DEBUG_QCA("\t WEP ");
            }
        }
        else
        {
            FNET_DEBUG_QCA("NONE! ");
        }
    }
    FNET_DEBUG_QCA("\r\n");
}
#endif

/************************************************************************
* DESCRIPTION: Scans for SSID authentication parameters.
*************************************************************************/
static fnet_return_t _fnet_qca_get_ssid_info(const char *ssid, WLAN_AUTH_MODE *auth_mode, WLAN_CRYPT_TYPE *encrypt_mode)
{
    FNET_ASSERT(ssid != FNET_NULL);
    FNET_ASSERT(auth_mode != FNET_NULL);
    FNET_ASSERT(encrypt_mode != FNET_NULL);

    fnet_return_t       result = FNET_ERR;
    QCA_SCAN_LIST       param = {0};
    int16_t             num_results = 0;
    fnet_index_t        i;

    /* Set maximum power */
    if(qcom_power_set_mode(FNET_QCA_DEVICE_ID, MAX_PERF_POWER, USER) != A_OK)
    {
        FNET_DEBUG_QCA("[QCA] ERROR: qcom_power_set_mode failed\r\n");
    }
    /* Set SSID filter */
    else if (qcom_set_ssid(FNET_QCA_DEVICE_ID, (QCOM_SSID *)ssid) != A_OK)
    {
        FNET_DEBUG_QCA("[QCA] ERROR: qcom_set_ssid failed\r\n");
    }
    else
    {
        for(i = 0; (num_results == 0) && (i < FNET_CFG_QCA_SCAN_MAX); i++)
        {
            FNET_DEBUG_QCA("[QCA] Scanning for SSID \"%s\"...\r\n", ssid);

            /* Start BSS channel scan. */
            if(qcom_set_scan(FNET_QCA_DEVICE_ID, NULL) != A_OK)
            {
                FNET_DEBUG_QCA("[QCA] ERROR:  qcom_set_scan() failed\r\n");
                break;
            }
            A_MDELAY(100);
            /* Get the scan results*/
            if(qcom_get_scan(FNET_QCA_DEVICE_ID, (QCOM_BSS_SCAN_INFO **) & (param.scan_info_list), &num_results) != A_OK)
            {
                FNET_DEBUG_QCA("[QCA] ERROR: qcom_get_scan() failed\r\n");
                break;
            }
        }

        FNET_DEBUG_QCA("[QCA] Scan result count:%d\r\n", num_results);

        if(num_results)
        {
            QCA_SCAN_INFO_PTR  scan_info = &param.scan_info_list[0]; /* Take first result */

            /* Print result */
#if FNET_CFG_DEBUG_QCA && FNET_CFG_DEBUG
            fnet_qca_print_ssid_info(scan_info);
#endif

            /* Default values.*/
            *auth_mode = WLAN_AUTH_NONE;
            *encrypt_mode = WLAN_CRYPT_NONE;

            if (scan_info->security_enabled)
            {
                /* AP security can support multiple options*/
                if (scan_info->rsn_auth) /* WPA2 */
                {
                    if (scan_info->rsn_auth & SECURITY_AUTH_PSK)
                    {
                        if (scan_info->rsn_cipher & ATH_CIPHER_TYPE_CCMP)
                        {
                            *encrypt_mode = WLAN_CRYPT_AES_CRYPT;
                        }
                        else if(scan_info->rsn_cipher & ATH_CIPHER_TYPE_TKIP)
                        {
                            *encrypt_mode = WLAN_CRYPT_TKIP_CRYPT;
                        }

                        if(*encrypt_mode)
                        {
                            *auth_mode = WLAN_AUTH_WPA2_PSK;
                        }
                    }
                }

                if((*auth_mode == WLAN_AUTH_NONE) && (scan_info->wpa_auth)) /*WPA*/
                {
                    if (scan_info->wpa_auth & SECURITY_AUTH_PSK)
                    {
                        if (scan_info->wpa_cipher & ATH_CIPHER_TYPE_CCMP)
                        {
                            *encrypt_mode = WLAN_CRYPT_AES_CRYPT;
                        }
                        else if(scan_info->wpa_cipher & ATH_CIPHER_TYPE_TKIP)
                        {
                            *encrypt_mode = WLAN_CRYPT_TKIP_CRYPT;
                        }

                        if(*encrypt_mode)
                        {
                            *auth_mode = WLAN_AUTH_WPA_PSK;
                        }
                    }
                }

                /* WEP is identified by absent wpa and rsn ciphers */
                if (scan_info->rsn_cipher == 0 && scan_info->wpa_cipher == 0)
                {
                    *auth_mode = WLAN_AUTH_WEP;
                    *encrypt_mode = WLAN_CRYPT_WEP_CRYPT;
                }
            }
            else
            {
                FNET_DEBUG_QCA("[QCA] SSID \"%s\" no security.\r\n", ssid);
                *auth_mode = WLAN_AUTH_NONE;
                *encrypt_mode = WLAN_CRYPT_NONE;
            }

            result = FNET_OK;
        }
        else
        {
            FNET_DEBUG_QCA("[QCA] SSID \"%s\" is not found. Applying default parameters.\r\n", ssid);
            /* By default use WPA2 - AES */
            *auth_mode = WLAN_AUTH_WPA2_PSK;
            *encrypt_mode = WLAN_CRYPT_AES_CRYPT;
            result = FNET_OK;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Callback function for link status change.
*************************************************************************/
static void _fnet_qca_on_connect(uint8_t event, uint8_t devId, char *bssid, uint8_t bssConn)
{
    /* We support only one device.*/
    FNET_ASSERT(devId == FNET_QCA_DEVICE_ID);

    switch (event)
    {
        case QCOM_ONCONNECT_EVENT_SUCCESS:
            FNET_DEBUG_QCA("[QCA] (EVENT) %s connected\r\n", bssConn ? "AP" : "CLIENT");

            if(fnet_qca_auth_mode == WLAN_AUTH_NONE) /* Open security.*/
            {
                if(((fnet_qca_dev_mode == QCOM_WLAN_DEV_MODE_STATION) && (bssConn == 0))  ||
                   ((fnet_qca_dev_mode == QCOM_WLAN_DEV_MODE_AP) && (bssConn == 1)))
                {
                    fnet_qca_if.is_connected = FNET_TRUE;
                }
            }
            /* else, a station is not fully connected until PEER_FIRST_NODE_JOIN_EVENT */
            break;
        case INVALID_PROFILE:
            /* This event is used to indicate RSNA failure */
            FNET_DEBUG_QCA("[QCA] (EVENT) 4 way handshake failure \r\n");
            if(((fnet_qca_dev_mode == QCOM_WLAN_DEV_MODE_STATION) && (bssConn == 0))  ||
               ((fnet_qca_dev_mode == QCOM_WLAN_DEV_MODE_AP) && (bssConn == 1)))
            {
                fnet_qca_if.is_connected = FNET_FALSE;
                fnet_qca_if.was_disconneced = FNET_TRUE;
            }
            break;
        case PEER_FIRST_NODE_JOIN_EVENT:
            /* This event is used to RSNA success */
            FNET_DEBUG_QCA("[QCA] (EVENT) 4 way handshake success\r\n");
            if(((fnet_qca_dev_mode == QCOM_WLAN_DEV_MODE_STATION) && (bssConn == 0))  ||
               ((fnet_qca_dev_mode == QCOM_WLAN_DEV_MODE_AP) && (bssConn == 1)))
            {
                fnet_qca_if.is_connected = FNET_TRUE;
            }
            break;
        case QCOM_ONCONNECT_EVENT_DISCONNECT:
            /* Disconnected.*/
            FNET_DEBUG_QCA("[QCA] (EVENT) %s disconnect\r\n", bssConn ? "AP" : "CLIENT");
            if(((fnet_qca_dev_mode == QCOM_WLAN_DEV_MODE_STATION) && (bssConn == 0))  ||
               ((fnet_qca_dev_mode == QCOM_WLAN_DEV_MODE_AP) && (bssConn == 1)))
            {
                fnet_qca_if.is_connected = FNET_FALSE;
                fnet_qca_if.was_disconneced = FNET_TRUE;
            }
            break;
        default:
            FNET_DEBUG_QCA("[QCA] (EVENT) code %d\r\n", event);
            break;
    }
}

/************************************************************************
* DESCRIPTION: Connects to AP.
*************************************************************************/
static fnet_return_t _fnet_qca_wifi_connect(struct fnet_netif *netif, fnet_wifi_connect_params_t *params)
{
    fnet_return_t       result = FNET_ERR;
    WLAN_CRYPT_TYPE     encrypt_mode; /* It may be:  WLAN_CRYPT_NONE, WLAN_CRYPT_AES_CRYPT, WLAN_CRYPT_WEP_CRYPT, WLAN_CRYPT_TKIP_CRYPT */

    /* Check SSID length */
    if(params->ssid && (fnet_strlen(params->ssid) > 0))
    {
        fnet_qca_auth_mode = WLAN_AUTH_NONE;
        encrypt_mode = WLAN_CRYPT_NONE;

        /* Disconnect. If the host has issued a CONNECT command, it must issue the
           DISCONNECT command before it issues the next CONNECT command.*/
        if((fnet_qca_if.is_connected == FNET_TRUE)
           && (_fnet_qca_wifi_disconnect(netif) != FNET_OK))
        {
            FNET_DEBUG_QCA("ERROR: qcom_disconnect failed\r\n");
        }
        else
        {
            FNET_DEBUG_QCA("[QCA] (MODE) Station\r\n");

#if 1 /* Reduce background scan period for better reconnection (optional).*/
            if( _fnet_qca_fw_get_version(netif) > 0x30080e38) /* For old GT202 (v3.0) it may cause disconnection */
            {
                qcom_scan_params_t scan_params = {.fgStartPeriod = 1,
                                                  .fgEndPeriod = 2,
                                                  .bgPeriod = 1,
                                                  .shortScanRatio = WMI_SHORTSCANRATIO_DEFAULT,
                                                  .scanCtrlFlags = DEFAULT_SCAN_CTRL_FLAGS,
                                                  .minActChDwellTimeInMs = 0,
                                                  .maxActScanPerSsid = 0,
                                                  .maxDfsChActTimeInMs = 0
                                                 };

                if (qcom_scan_params_set(FNET_QCA_DEVICE_ID, &scan_params) != A_OK)
                {
                    FNET_DEBUG_QCA("ERROR: qcom_scan_params_set failed\r\n");
                }
            }
#endif

            if(params->wpa_passphrase && fnet_strlen(params->wpa_passphrase)) /* Security */
            {
                /* Scan for security parameters.*/
                if(_fnet_qca_get_ssid_info(params->ssid, &fnet_qca_auth_mode, &encrypt_mode) == FNET_ERR)
                {
                    FNET_DEBUG_QCA("[QCA] ERROR: _fnet_qca_get_ssid_info failed\r\n");
                    goto EXIT;
                }
            }

            FNET_DEBUG_QCA("[QCA] Connecting to \"%s\"...\r\n", params->ssid);

            /* Set station mode */
            if(qcom_op_set_mode(FNET_QCA_DEVICE_ID, QCOM_WLAN_DEV_MODE_STATION) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR: qcom_op_set_mode failed\r\n");
            }
            /* Set to MAX PERF Mode */
            else if(qcom_power_set_mode(FNET_QCA_DEVICE_ID, MAX_PERF_POWER, USER) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR: qcom_power_set_mode failed\r\n");
            }
            /* NOTE: qcom API requires to first set a valid SSID (before auth, cipher and passphrase)*/
            else if (qcom_set_ssid(FNET_QCA_DEVICE_ID, (QCOM_SSID *)params->ssid) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR: qcom_set_ssid failed\r\n");
            }
            /* Note that only 4 of all modes listed in QCA 80-Y9106-1 are supported!
            * The modes are: WLAN_AUTH_NONE, WLAN_AUTH_WPA_PSK, WLAN_AUTH_WPA2_PSK, WLAN_AUTH_WEP*/
            else if(qcom_sec_set_auth_mode(FNET_QCA_DEVICE_ID, fnet_qca_auth_mode) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR: qcom_sec_set_auth_mode failed\r\n");
            }
            /* Set encryption mode.*/
            else if(qcom_sec_set_encrypt_mode(FNET_QCA_DEVICE_ID, encrypt_mode) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR:qcom_sec_set_encrypt_mode failed\r\n");
            }
            else
            {
                if(params->wpa_passphrase && fnet_strlen(params->wpa_passphrase))
                {
                    fnet_size_t passphrase_len = fnet_strlen(params->wpa_passphrase);
                    if(passphrase_len < (FNET_WIFI_PASSPHRASE_SIZE_MAX - 1 /*64*/))
                    {
                        /* 8-63 passphrase*/
                        if(qcom_sec_set_passphrase(FNET_QCA_DEVICE_ID, (QCOM_PASSPHRASE *)(params->wpa_passphrase)) != A_OK)
                        {
                            FNET_DEBUG_QCA("ERROR: qcom_sec_set_passphrase() failed\r\n");
                            goto EXIT;
                        }
                    }
                    else if(passphrase_len == (FNET_WIFI_PASSPHRASE_SIZE_MAX - 1 /*64*/))
                    {
                        /* Pre-shared key */
                        if(qcom_sec_set_pmk(FNET_QCA_DEVICE_ID, params->wpa_passphrase) != A_OK)
                        {
                            FNET_DEBUG_QCA("[QCA] ERROR: qcom_sec_set_pmk() failed\r\n");
                            goto EXIT;
                        }
                    }
                    else
                    {
                        FNET_DEBUG_QCA("[QCA] ERROR: Passphrase wrong length %d \r\n", (int)passphrase_len);
                        goto EXIT;
                    }
                }

                /* Save currrent device mode.*/
                fnet_qca_dev_mode = QCOM_WLAN_DEV_MODE_STATION;

                /* The connect callback is actually used for four different callbacks:
                *     onConnect(1 (TRUE), uint8_t devId, uint8_t *ssid, uint8_t bssMac[6]);
                *     onDisconnect(status, uint8_t devId, uint8_t *ssid, uint8_t bssMac[6]);
                *         where status = 0x10 (INVALID_PROFILE) on ?
                *                      =    0 on normal disconnect
                *     onRSNASuccessEvent(uint8_t code, uint8_t devId, NULL, 0)
                *     onBitRateEvent_tx(wmi_rateTable[rateIndex][0], devId, NULL, 0);
                * It is not possible to discern the onBitRateEvent_tx event from the others*/
                if(qcom_set_connect_callback(FNET_QCA_DEVICE_ID, (void *) _fnet_qca_on_connect) != A_OK)
                {
                    FNET_DEBUG_QCA("ERROR: qcom_set_connect_callback failed\r\n");
                    goto EXIT;
                }

                A_MDELAY(1000); /* Workaround QCA connection issue */

                /* Connect to an AP with the security configuration by user. */
                if(qcom_commit(FNET_QCA_DEVICE_ID) != A_OK)
                {
                    FNET_DEBUG_QCA("ERROR: qcom_commit failed\r\n");
                    goto EXIT;
                }

                result = FNET_OK;
            }
        }

    }

EXIT:
    return result;
}

/************************************************************************
* DESCRIPTION: Initializes AP.
*************************************************************************/
static fnet_return_t _fnet_qca_wifi_access_point(fnet_netif_t *netif, fnet_wifi_access_point_params_t *params)
{
    fnet_return_t       result = FNET_ERR;
    WLAN_CRYPT_TYPE     encrypt_mode; /* It may be:  WLAN_CRYPT_NONE, WLAN_CRYPT_AES_CRYPT, WLAN_CRYPT_WEP_CRYPT, WLAN_CRYPT_TKIP_CRYPT */

    /* Check SSID length */
    if(params->ssid && (fnet_strlen(params->ssid) > 0))
    {
        fnet_qca_auth_mode = WLAN_AUTH_NONE;
        encrypt_mode = WLAN_CRYPT_NONE;

        /* Disconnect. If the host has issued a CONNECT command, it must issue the
           DISCONNECT command before it issues the next CONNECT command.*/
        if((fnet_qca_if.is_connected == FNET_TRUE)
           && (_fnet_qca_wifi_disconnect(netif) != FNET_OK))
        {
            FNET_DEBUG_QCA("ERROR: qcom_disconnect failed\r\n");
        }
        else
        {
            FNET_DEBUG_QCA("[QCA] (MODE) Access Point\r\n");

            if((params->wpa_mode != FNET_WIFI_WPA_NONE)
               && params->wpa_passphrase
               && fnet_strlen(params->wpa_passphrase)) /* Security */
            {
                switch(params->wpa_mode)
                {
                    case FNET_WIFI_WPA: /* WPA TKIP */
                        fnet_qca_auth_mode = WLAN_AUTH_WPA_PSK;
                        encrypt_mode = WLAN_CRYPT_TKIP_CRYPT;
                        break;
                    case FNET_WIFI_WPA2: /* WPA2 AES */
                        fnet_qca_auth_mode = WLAN_AUTH_WPA2_PSK;
                        encrypt_mode = WLAN_CRYPT_AES_CRYPT;
                        break;
                    default:
                        goto EXIT;
                }
            }

            /* Set station mode */
            if(qcom_op_set_mode(FNET_QCA_DEVICE_ID, QCOM_WLAN_DEV_MODE_AP) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR: qcom_op_set_mode failed\r\n");
            }
            /* Set to MAX PERF Mode */
            else if(qcom_power_set_mode(FNET_QCA_DEVICE_ID, MAX_PERF_POWER, USER) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR: qcom_power_set_mode failed\r\n");
            }
            /* NOTE: qcom API requires to first set a valid SSID (before auth, cipher and passphrase)*/
            else if (qcom_set_ssid(FNET_QCA_DEVICE_ID, (QCOM_SSID *)params->ssid) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR: qcom_set_ssid failed\r\n");
            }
            /* Note that only 4 of all modes listed in QCA 80-Y9106-1 are supported!
            * The modes are: WLAN_AUTH_NONE, WLAN_AUTH_WPA_PSK, WLAN_AUTH_WPA2_PSK, WLAN_AUTH_WEP*/
            else if(qcom_sec_set_auth_mode(FNET_QCA_DEVICE_ID, fnet_qca_auth_mode) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR: qcom_sec_set_auth_mode failed\r\n");
            }
            /* Set encryption mode.*/
            else if(qcom_sec_set_encrypt_mode(FNET_QCA_DEVICE_ID, encrypt_mode) != A_OK)
            {
                FNET_DEBUG_QCA("ERROR:qcom_sec_set_encrypt_mode failed\r\n");
            }
            else
            {
                if(params->wpa_passphrase)
                {
                    fnet_size_t passphrase_len = fnet_strlen(params->wpa_passphrase);
                    if(passphrase_len < (FNET_WIFI_PASSPHRASE_SIZE_MAX - 1 /*64*/))
                    {
                        /* 8-63 passphrase*/
                        if(qcom_sec_set_passphrase(FNET_QCA_DEVICE_ID, (QCOM_PASSPHRASE *)(params->wpa_passphrase)) != A_OK)
                        {
                            FNET_DEBUG_QCA("ERROR: qcom_sec_set_passphrase() failed\r\n");
                            goto EXIT;
                        }
                    }
                    else if(passphrase_len == (FNET_WIFI_PASSPHRASE_SIZE_MAX - 1 /*64*/))
                    {
                        /* Pre-shared key */
                        if(qcom_sec_set_pmk(FNET_QCA_DEVICE_ID, params->wpa_passphrase) != A_OK)
                        {
                            FNET_DEBUG_QCA("[QCA] ERROR: qcom_sec_set_pmk() failed\r\n");
                            goto EXIT;
                        }
                    }
                    else
                    {
                        FNET_DEBUG_QCA("[QCA] ERROR: Passphrase wrong length %d \r\n", (int)passphrase_len);
                        goto EXIT;
                    }
                }

                /* Save currrent device mode.*/
                fnet_qca_dev_mode = QCOM_WLAN_DEV_MODE_AP;

                /* The connect callback is actually used for four different callbacks:
                *     onConnect(1 (TRUE), uint8_t devId, uint8_t *ssid, uint8_t bssMac[6]);
                *     onDisconnect(status, uint8_t devId, uint8_t *ssid, uint8_t bssMac[6]);
                *         where status = 0x10 (INVALID_PROFILE) on ?
                *                      =    0 on normal disconnect
                *     onRSNASuccessEvent(uint8_t code, uint8_t devId, NULL, 0)
                *     onBitRateEvent_tx(wmi_rateTable[rateIndex][0], devId, NULL, 0);
                * It is not possible to discern the onBitRateEvent_tx event from the others*/
                if(qcom_set_connect_callback(FNET_QCA_DEVICE_ID, (void *) _fnet_qca_on_connect) != A_OK)
                {
                    FNET_DEBUG_QCA("ERROR: qcom_set_connect_callback failed\r\n");
                    goto EXIT;
                }

                /* Add Information Element (IE) to Probe-Response and Beacon frames.*/
                if(qcom_set_appie(FNET_QCA_DEVICE_ID, WMI_FRAME_BEACON, params->ie, params->ie_size) != A_OK)
                {
                    FNET_DEBUG_QCA("ERROR: qcom_set_appie failed\r\n");
                    goto EXIT;
                }
                if(qcom_set_appie(FNET_QCA_DEVICE_ID, WMI_FRAME_PROBE_RESP, params->ie, params->ie_size) != A_OK)
                {
                    FNET_DEBUG_QCA("ERROR: qcom_set_appie failed\r\n");
                    goto EXIT;
                }

                A_MDELAY(1000); /* Workaround QCA connection issue */

                /* Commit parameters.*/
                if(qcom_commit(FNET_QCA_DEVICE_ID) != A_OK)
                {
                    FNET_DEBUG_QCA("ERROR: qcom_commit failed\r\n");
                    goto EXIT;
                }

                result = FNET_OK;
            }
        }
    }

EXIT:
    return result;
}

/************************************************************************
* DESCRIPTION: Turns off the Wi-Fi interafce radio.
*************************************************************************/
static fnet_return_t _fnet_qca_wifi_disconnect(struct fnet_netif *netif)
{
    fnet_return_t   result;

    fnet_qca_if.is_connected = FNET_FALSE; /* To avoid TX before QCA event */

    /* Disconnect the QCA4002 wifi */
    if(qcom_disconnect(FNET_QCA_DEVICE_ID) != A_OK)
    {
        FNET_DEBUG_QCA("[QCA] ERROR: qcom_disconnect failed\r\n");
        result = FNET_ERR;
    }
    else
    {
        result = FNET_OK;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Gets HW address.
*************************************************************************/
static fnet_return_t _fnet_qca_get_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr)
{
    fnet_return_t result = FNET_ERR;

    if(netif && (netif->netif_api->netif_type == FNET_NETIF_TYPE_WIFI) && (hw_addr) )
    {
        if(qcom_get_bssid(FNET_QCA_DEVICE_ID, (uint8_t *)hw_addr) == A_OK)
        {
            result = FNET_OK;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Returns Ethernet statistics information
*************************************************************************/
static fnet_return_t _fnet_qca_get_statistics(fnet_netif_t *netif, struct fnet_netif_statistics *statistics)
{
    FNET_ASSERT(netif != FNET_NULL);
    FNET_ASSERT(netif->netif_prv != FNET_NULL);

    fnet_return_t   result = FNET_ERR;
    fnet_qca_if_t   *qca_if;

    if(netif && (netif->netif_api->netif_type == FNET_NETIF_TYPE_WIFI))
    {
        qca_if = (fnet_qca_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

        FNET_ASSERT(qca_if != FNET_NULL);

        *statistics = qca_if->statistics;
        result = FNET_OK;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: QCA low-level output function.
*************************************************************************/
void fnet_qca_output(fnet_netif_t *netif, fnet_netbuf_t *nb)
{
    A_NETBUF        *a_netbuf_ptr;
    fnet_qca_if_t   *qca_if;

    if(netif && netif->netif_prv
       && nb && (nb->total_length >= FNET_ETH_HDR_SIZE))
    {
        qca_if = (fnet_qca_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

        if(qca_if && (qca_if->is_connected == FNET_TRUE))
        {
            /* Allocate atheros pcb. */
            if ((a_netbuf_ptr = (A_NETBUF *)A_NETBUF_ALLOC(nb->total_length)) == NULL)
            {
                FNET_DEBUG_QCA("[QCA] ERROR: A_NETBUF_ALLOC failed\r\n");
                goto EXIT_1;
            }

            _fnet_netbuf_to_buf(nb, 0u, FNET_NETBUF_COPYALL, a_netbuf_ptr->tail);
            A_NETBUF_PUT(a_netbuf_ptr, nb->total_length);

            /* Ensure there is enough headroom to complete the tx operation */
            if (A_NETBUF_HEADROOM(a_netbuf_ptr) <
                sizeof(ATH_MAC_HDR) + sizeof(ATH_LLC_SNAP_HDR) + sizeof(WMI_DATA_HDR) + HTC_HDR_LENGTH + WMI_MAX_TX_META_SZ)
            {
                FNET_DEBUG_QCA("[QCA] ERROR: A_NETBUF_HEADROOM failed\r\n");
                goto EXIT_2;
            }

            if (A_OK != Api_DataTxStart(wifiCtx.MAC_CONTEXT_PTR, a_netbuf_ptr))
            {
                FNET_DEBUG_QCA("[QCA] ERROR: Api_DataTxStart failed\r\n");
                goto EXIT_2;
            }

            qca_if->statistics.tx_packet++;
        }
    }
EXIT_1:
    _fnet_netbuf_free_chain(nb);
    return;
EXIT_2:
    A_NETBUF_FREE((void *)a_netbuf_ptr);
    _fnet_netbuf_free_chain(nb);
    return;
}

#if FNET_CFG_CPU_WIFI_FW_UPDATE
/************************************************************************
* DESCRIPTION: QCA Firmware update.
*************************************************************************/
static fnet_return_t _fnet_qca_wifi_fw_update(fnet_netif_t *netif, const fnet_uint8_t *fw_buffer, fnet_size_t fw_buffer_size)
{
    fnet_qca_if_t   *qca_if;
    fnet_return_t   result = FNET_ERR;

    if(netif && (netif->netif_api->netif_type == FNET_NETIF_TYPE_WIFI) && netif->netif_prv && fw_buffer && fw_buffer_size)
    {
        qca_if = (fnet_qca_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

        if(qca_if)
        {
            ATH_PROGRAM_FLASH_STRUCT flash_msg;
            ATH_IOCTL_PARAM_STRUCT inout_param;

            /* Switch to BMI mode */

            /* Release QCA driver */
            _fnet_qca_release(netif);

            /* Prepare QCA initialization parameters for BMI mode */
            {
                ath_custom_init.skipWmi = true;
                ath_custom_init.exitAtBmi = true;

                ar4XXX_boot_param = AR4XXX_PARAM_MODE_BMI;
            }

            /* Init QCA driver in BMI mode*/
            result = _fnet_qca_init(netif);

#if 1 /* TARGET_AR400X_REV2 does not support FW update (stuck). Proved on practice. */
            {
                ATH_VERSION qca_version;
                if(A_FAILED(qcom_get_version(&qca_version)))
                {
                    FNET_DEBUG_QCA("[QCA] ERROR: Failed to get QCA version\r\n");
                    goto EXIT;
                }

                if(qca_version.target_ver == TARGET_AR400X_REV2)
                {
                    FNET_DEBUG_QCA("[QCA] ERROR: AR400X_REV2 does not support fw update!\r\n");
                    goto EXIT;
                }
            }
#endif

            if(result == FNET_OK)
            {
                /*------- Load the firmware buffer to QCA --------------*/
                FNET_DEBUG_QCA("[QCA] Flashing firmware %d bytes ...\r\n", fw_buffer_size);
                fnet_memset_zero(&flash_msg, sizeof(flash_msg));
                fnet_memset_zero(&inout_param, sizeof(inout_param));

                flash_msg.load_addr = BMI_SEGMENTED_WRITE_ADDR;
                flash_msg.result = 0;
                flash_msg.buffer = (uint8_t *)fw_buffer;
                flash_msg.length = fw_buffer_size;

                inout_param.cmd_id = ATH_PROGRAM_FLASH;
                inout_param.data = &flash_msg;
                inout_param.length = sizeof(flash_msg);

                if (A_OK != Custom_Api_Mediactl(&wifiCtx, QCA_MEDIACTL_VENDOR_SPECIFIC, &inout_param))
                {
                    FNET_DEBUG_QCA("[QCA] ERROR: Custom_Api_Mediactl(ATH_PROGRAM_FLASH) failed\r\n");
                    goto EXIT;
                }

                /*---------- Execute Flashing ---------------------------*/
                fnet_memset_zero(&flash_msg, sizeof(flash_msg));
                fnet_memset_zero(&inout_param, sizeof(inout_param));

                flash_msg.load_addr = 0xA02800; /* Address - undocumented reversed-engineered magic number - ??  9-12 Bytes of binary (swaped)*/
                flash_msg.result = 0x1A0;       /* Option - undocumented reversed-engineered magic number - ?? 46-47 Bytes of binary (not swaped)*/
                flash_msg.buffer = 0;
                flash_msg.length = 0;

                inout_param.cmd_id = ATH_EXECUTE_FLASH;
                inout_param.data = &flash_msg;
                inout_param.length = sizeof(flash_msg);

                if (A_OK != Custom_Api_Mediactl(&wifiCtx, QCA_MEDIACTL_VENDOR_SPECIFIC, &inout_param))
                {
                    FNET_DEBUG_QCA("[QCA] ERROR: Custom_Api_Mediactl(ATH_EXECUTE_FLASH) failed\r\n");
                    goto EXIT;
                }

                FNET_DEBUG_QCA("[QCA] Flashing is done.\r\n");
            }
        EXIT:
            /* Switch to the normal mode */

            /* Release driver */
            _fnet_qca_release(netif);

            /* Prepare QCA initialization parameters for normal mode */
            {
                ath_custom_init.skipWmi = false;
                ath_custom_init.exitAtBmi = false;

                ar4XXX_boot_param = AR4XXX_PARAM_MODE_NORMAL;
            }

            /* Init driver */
            result = _fnet_qca_init(netif);
        }
    }

    return result;
}
#endif /* FNET_CFG_CPU_WIFI_FW_UPDATE */

/************************************************************************
* DESCRIPTION: Retrieves Wi-Fi interface firmware version number.
*************************************************************************/
static fnet_uint32_t _fnet_qca_fw_get_version(struct fnet_netif *netif)
{
    fnet_uint32_t       fw_version = 0;
    fnet_qca_if_t       *qca_if;

    if(netif && netif->netif_prv)
    {
        qca_if = (fnet_qca_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

        if(qca_if)
        {
            QCOM_WLAN_DEV_MODE mode;

            if(qcom_op_get_mode(FNET_QCA_DEVICE_ID, &mode) == A_OK)
            {
                ATH_VERSION qca_version;
                if(A_FAILED(qcom_get_version(&qca_version)))
                {
                    FNET_DEBUG_QCA("[QCA] ERROR: Failed to get QCA version\r\n");
                }
                else
                {
                    fw_version = qca_version.wlan_ver;
                }
            }
        }
    }

    return fw_version;
}

/************************************************************************
* DESCRIPTION: Link status.
*************************************************************************/
static fnet_bool_t _fnet_qca_is_connected(fnet_netif_t *netif)
{
    fnet_bool_t     res = FNET_FALSE;
    fnet_qca_if_t   *qca_if;

    if(netif && netif->netif_prv)
    {
        qca_if = (fnet_qca_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

        if(qca_if)
        {

            if(qca_if->was_disconneced == FNET_TRUE)
            {
                res = FNET_FALSE;
                qca_if->was_disconneced = FNET_FALSE; /* Reset flag */
            }
            else
            {
                res = qca_if->is_connected;
            }
        }
    }

    return res;
}

/************************************************************************
* DESCRIPTION: Retrieves current operation mode of the Wi-Fi interface.
*************************************************************************/
static fnet_wifi_op_mode_t _fnet_qca_get_op_mode(struct fnet_netif *netif)
{
    fnet_wifi_op_mode_t result = FNET_WIFI_OP_MODE_NONE;
    fnet_qca_if_t       *qca_if;

    if(netif && netif->netif_prv)
    {
        qca_if = (fnet_qca_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

        if(qca_if && (qca_if->is_connected == FNET_TRUE))
        {
            QCOM_WLAN_DEV_MODE mode;

            if(qcom_op_get_mode(FNET_QCA_DEVICE_ID, &mode) == A_OK)
            {
                switch(mode)
                {
                    case QCOM_WLAN_DEV_MODE_STATION:
                        result = FNET_WIFI_OP_MODE_STATION;
                        break;
                    case QCOM_WLAN_DEV_MODE_AP:
                        result = FNET_WIFI_OP_MODE_ACCESS_POINT;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Sets MAC address.
*************************************************************************/
static fnet_return_t _fnet_qca_set_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr)
{
    /* TBD. Not supported.*/
    return FNET_OK;
}

/************************************************************************
* DESCRIPTION: Retrieve country code used by the Wi-Fi interface.
*************************************************************************/
static fnet_return_t _fnet_qca_wifi_get_country_code(fnet_netif_t *netif, fnet_char_t *country_code)
{
    fnet_return_t   result = FNET_ERR;
    fnet_qca_if_t   *qca_if;

    if(netif && netif->netif_prv && country_code)
    {
        qca_if = (fnet_qca_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

        if(qca_if)
        {
            if(qcom_get_country_code(FNET_QCA_DEVICE_ID, (uint8_t *)country_code) == A_OK)
            {
                result = FNET_OK;
            }
            else
            {
                FNET_DEBUG_QCA("[QCA] ERROR: Failed to get country code\r\n");
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Set country code of the Wi-Fi interface.
*************************************************************************/
static fnet_return_t _fnet_qca_wifi_set_country_code(fnet_netif_t *netif, const fnet_char_t *country_code)
{
    fnet_return_t       result = FNET_ERR;
    fnet_qca_if_t       *qca_if;

    if(netif && netif->netif_prv && country_code)
    {
        qca_if = (fnet_qca_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

        if(qca_if)
        {
            if(qcom_set_country_code(FNET_QCA_DEVICE_ID, (uint8_t *)country_code) == A_OK)
            {
                result = FNET_OK;
            }
            else
            {
                FNET_DEBUG_QCA("[QCA] ERROR: Failed to set country code\r\n");
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: QCA low-level input function. Handled as event.
*************************************************************************/
static void _fnet_qca_input(void *cookie)
{
    fnet_qca_if_t   *qca_if = (fnet_qca_if_t *)cookie;
    void            *pReq;
    A_NETBUF        *a_netbuf_ptr;
    fnet_uint8_t    *frame;
    fnet_size_t     frame_len;

    taskENTER_CRITICAL();

    pReq = qca_if->pReq;
    a_netbuf_ptr = (A_NETBUF *)(pReq);

    if(a_netbuf_ptr && qca_if->netif)
    {
        frame_len = A_NETBUF_LEN(pReq);
        frame = A_NETBUF_DATA(pReq);

        qca_if->statistics.rx_packet++;

        fnet_isr_lock();

        /* Ethernet input.*/
        _fnet_eth_input(qca_if->netif, frame, frame_len);

        fnet_isr_unlock();
    }

    A_NETBUF_FREE(pReq);
    qca_if->pReq = FNET_NULL;

    taskEXIT_CRITICAL();
}

/************************************************************************
***********************  QCA Interface adapter **************************
*************************************************************************/

/*****************************************************************************/
/*  Custom_DeliverFrameToNetworkStack - Called by API_RxComplete to
 *   deliver a data frame to the network stack. This code will perform
 *   platform specific operations.
 *      void *pCxt - the driver context.
 *      void *pReq - the packet.
 *****************************************************************************/
void Custom_DeliverFrameToNetworkStack(void *pCxt, void *pReq)
{
    if(fnet_qca_if.pReq) /* Just in case */
    {
        A_NETBUF_FREE(fnet_qca_if.pReq);
    }
    fnet_qca_if.pReq = pReq;

    /* Initiate S/W Interrupt*/
    fnet_event_raise(fnet_qca_if.input_event);
}

QCA_CONTEXT_STRUCT *wlan_get_context(void)
{
    return &wifiCtx;
}

/* This functions is not used, it is here just to make compilable the QCA driver */
uint32_t Custom_Api_Send(QCA_CONTEXT_STRUCT_PTR qca_ptr, PCB_PTR pcb_ptr, uint32_t size, uint32_t frags, uint32_t flags);
uint32_t Custom_Api_Send(QCA_CONTEXT_STRUCT_PTR qca_ptr, PCB_PTR pcb_ptr, uint32_t size, uint32_t frags, uint32_t flags)
{
    return QCA_ERROR;
}

#endif /* FNET_CFG_CPU_WIFI */

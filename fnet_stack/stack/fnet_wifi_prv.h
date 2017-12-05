/**************************************************************************
*
* Copyright 2011-2017 by Andrey Butok. FNET Community.
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
*  Private. Wi-Fi platform independent API functions.
*
***************************************************************************/

#ifndef _FNET_WIFI_PRV_H_

#define _FNET_WIFI_PRV_H_

#include "fnet.h"

/************************************************************************
*     Definitions
*************************************************************************/
/**************************************************************************/ /*!
 * @internal
 * @brief    Wi-Fi interface API structure.
 ******************************************************************************/
typedef struct
{
    fnet_return_t       (*wifi_connect)(struct fnet_netif *netif, fnet_wifi_connect_params_t *params);               /* Starts Wi-Fi connection in station mode.*/
    fnet_return_t       (*wifi_access_point)(struct fnet_netif *netif_desc, fnet_wifi_access_point_params_t *params);/* Initializes a Wi-Fi access point.*/
    fnet_return_t       (*wifi_disconnect)(struct fnet_netif *netif);                                                /* Disconnects from a Wi-Fi network.*/
    fnet_wifi_op_mode_t (*wifi_get_op_mode)(struct fnet_netif *netif);                                               /* Retrieves current operation mode of the Wi-Fi interface.*/
} fnet_wifi_api_t;

/************************************************************************
*     Global Data Structures
*************************************************************************/

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _FNET_WIFI_PRV_H_ */

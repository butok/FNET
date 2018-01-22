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
* @brief FNET Shell Demo (Wi-Fi).
*
***************************************************************************/

/************************************************************************
*     Private Definitions.
*************************************************************************/
#ifndef _FAPP_WIFI_H_

#define _FAPP_WIFI_H_

#if FNET_CFG_CPU_WIFI

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_wifi_connect_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_wifi_ap_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_wifi_disconnect_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );

#if FNET_CFG_CPU_WIFI_FW_UPDATE
void fapp_wifi_fw_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif

#if defined(__cplusplus)
}
#endif

#endif /* FNET_CFG_CPU_WIFI */

#endif /* _FAPP_WIFI_H_ */

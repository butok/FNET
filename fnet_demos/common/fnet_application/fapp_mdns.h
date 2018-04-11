/**************************************************************************
*
* Copyright 2016-2018 by Andrey Butok. FNET Community.
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
*  FNET Shell Demo (MDNS Server).
*
***************************************************************************/

/************************************************************************
*     Private Definitions.
*************************************************************************/
#ifndef _FAPP_MDNS_H_

#define _FAPP_MDNS_H_

#include "fapp.h"

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_mdns_cmd(fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv);
void fapp_mdns_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif);
void fapp_mdns_release(void);
void fapp_mdns_service_register_http(void);
void fapp_mdns_service_unregister_http(void);
void fapp_mdns_service_register_http_tls(void);
void fapp_mdns_service_unregister_http_tls(void);
void fapp_mdns_change_name_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );

#if defined(__cplusplus)
}
#endif

#endif /* FAPP_CFG_MDNS_CMD */

#endif /* _FAPP_MDNS_H_ */

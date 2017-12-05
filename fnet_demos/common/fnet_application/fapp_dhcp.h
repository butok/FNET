/**************************************************************************
*
* Copyright 2011-2017 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
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
*  FNET Shell Demo API.
*
***************************************************************************/

#ifndef _FAPP_DHCP_H_

#define _FAPP_DHCP_H_

#include "fapp_config.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN && FNET_CFG_IP4
void fapp_dhcp_cln_cmd(fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv);
void fapp_dhcp_cln_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif);
void fapp_dhcp_cln_release(void);
#endif

#if FAPP_CFG_DHCP_CMD && FNET_CFG_DHCP_SRV && FNET_CFG_IP4
void fapp_dhcp_srv_cmd(fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv);
void fapp_dhcp_srv_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif);
void fapp_dhcp_srv_release(void);
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _FAPP_DHCP_H_ */

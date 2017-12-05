/**************************************************************************
*
* Copyright 2016-2017 by Andrey Butok. FNET Community.
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
*  Network-interface initialization.
*
***************************************************************************/

#ifndef _FAPP_NETIF_H_

#define _FAPP_NETIF_H_

#if defined(__cplusplus)
extern "C" {
#endif

/* Network-interface initialization. */
fnet_return_t fapp_netif_init( fnet_shell_desc_t desc );
void fapp_netif_info_print( fnet_shell_desc_t desc, fnet_netif_desc_t netif);
void fapp_netif_addr_print( fnet_shell_desc_t desc, fnet_address_family_t family, fnet_netif_desc_t netif, fnet_bool_t print_type);
void fapp_netif_info_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_netif_stat_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );

extern const fnet_char_t *const fapp_netif_connection_state_str[];

#if defined(__cplusplus)
}
#endif

#endif

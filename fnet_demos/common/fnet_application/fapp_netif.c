/**************************************************************************
*
* Copyright 2016 by Andrey Butok. FNET Community.
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
**********************************************************************/
/*!
*
* @brief Network-interface initialization.
*
***************************************************************************/

#include "fapp.h"

/************************************************************************
*     Definitions.
*************************************************************************/
struct fapp_netif_init_param
{
    fnet_netif_desc_t   netif_desc;
    fnet_mac_addr_t     netif_mac_addr;
    fnet_ip4_addr_t     netif_ip4_addr;
    fnet_ip4_addr_t     netif_ip4_subnet_mask;
    fnet_ip4_addr_t     netif_ip4_gateway;
#if FNET_CFG_DNS
    fnet_ip4_addr_t     netif_ip4_dns;
#endif
};

/* Default address parameters.*/
struct fapp_netif_init_param fapp_netif_init_param_list[] =
{
#if FNET_CFG_CPU_ETH0
    {
        .netif_desc = FNET_CPU_ETH0_IF,
        .netif_mac_addr = FAPP_CFG_CPU_ETH0_MAC_ADDR,
        .netif_ip4_addr = FAPP_CFG_ETH0_IP4_ADDR,
        .netif_ip4_subnet_mask = FAPP_CFG_ETH0_IP4_MASK,
        .netif_ip4_gateway = FAPP_CFG_ETH0_IP4_GW,
#if FNET_CFG_DNS
        .netif_ip4_dns = FAPP_CFG_ETH0_IP4_DNS,
#endif
    },
#endif
#if FNET_CFG_CPU_ETH1
    {
        .netif_desc = FAPPT_CPU_ETH1_IF,
        .netif_mac_addr = FAPP_CFG_CPU_ETH1_MAC_ADDR,
        .netif_ip4_addr = FAPP_CFG_ETH1_IP4_ADDR,
        .netif_ip4_subnet_mask = FAPP_CFG_ETH1_IP4_MASK,
        .netif_ip4_gateway = FAPP_CFG_ETH1_IP4_GW,
#if FNET_CFG_DNS
        .netif_ip4_dns = FAPP_CFG_ETH1_IP4_DNS,
#endif
    },
#endif
    /* Here put your new network interfaces.*/
    {.netif_desc = FNET_NULL} /* END */
};

/************************************************************************
* DESCRIPTION: Initialization of all supported interfaces.
*************************************************************************/
fnet_return_t fapp_netif_init( void )
{
    fnet_return_t   result = FNET_OK;
    fnet_index_t    i;

    /***********************************
     * Initialize all IFs.
     ************************************/
    for(i = 0; fapp_netif_init_param_list[i].netif_desc; i++)
    {
        result = fnet_netif_init(fapp_netif_init_param_list[i].netif_desc, fapp_netif_init_param_list[i].netif_mac_addr, sizeof(fnet_mac_addr_t));
        if(result == FNET_ERR)
        {
            break;
        }
        else
        {
            fnet_netif_set_ip4_addr(fapp_netif_init_param_list[i].netif_desc, fapp_netif_init_param_list[i].netif_ip4_addr, fapp_netif_init_param_list[i].netif_ip4_subnet_mask);
            fnet_netif_set_ip4_gateway(fapp_netif_init_param_list[i].netif_desc, fapp_netif_init_param_list[i].netif_ip4_gateway);
        #if FNET_CFG_DNS
            fnet_netif_set_ip4_dns(fapp_netif_init_param_list[i].netif_desc, fapp_netif_init_param_list[i].netif_ip4_dns);
        #endif
        }
    }

    fnet_netif_set_default(FAPP_CFG_DEFAULT_IF); /* Set default interface.*/

    return result;
}

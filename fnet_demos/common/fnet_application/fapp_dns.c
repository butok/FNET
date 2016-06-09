/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
**********************************************************************/
/*!
*
* @file fapp_dns.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo implementation (DNS Resolver).
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_dns.h"
#include "fnet.h"

#if FAPP_CFG_DNS_CMD && FNET_CFG_DNS && FNET_CFG_DNS_RESOLVER
/************************************************************************
*     Definitions.
*************************************************************************/
#define FNET_DNS_RESOLUTION_FAILED  "Resolution is FAILED"
#define FNET_DNS_UNKNOWN            "DNS server is unknown"

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_dns_callback_resolved (const struct fnet_dns_resolved_addr *addr_list, fnet_size_t addr_list_size, fnet_uint32_t cookie);
static void fapp_dns_on_ctrlc(fnet_shell_desc_t desc);

/************************************************************************
* NAME: fapp_dhcp_callback_resolved
*
* DESCRIPTION: Event handler callback on new IP from DHCP client.
************************************************************************/
static void fapp_dns_callback_resolved (const struct fnet_dns_resolved_addr *addr_list, fnet_size_t addr_list_size, fnet_uint32_t cookie)
{
    fnet_char_t                ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
    fnet_shell_desc_t   desc = (fnet_shell_desc_t) cookie;
    fnet_index_t        i;

    fnet_shell_unblock((fnet_shell_desc_t)cookie); /* Unblock the shell. */

    if(addr_list && addr_list_size)
    {
        for(i = 0u; i < addr_list_size; i++)
        {
            fnet_shell_printf(desc, FAPP_SHELL_INFO_FORMAT_S, "Resolved address",
                              fnet_inet_ntop(addr_list->resolved_addr.sa_family, addr_list->resolved_addr.sa_data, ip_str, sizeof(ip_str)));
            fnet_shell_println(desc, "\t TTL=%d", addr_list->resolved_addr_ttl);

            addr_list++;
        }
    }
    else
    {
        fnet_shell_println(desc, FNET_DNS_RESOLUTION_FAILED);
    }
}

/************************************************************************
* NAME: fapp_dhcp_on_ctrlc
*
* DESCRIPTION:
************************************************************************/
static void fapp_dns_on_ctrlc(fnet_shell_desc_t desc)
{
    /* Terminate DNS service. */
    fnet_dns_release();
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* NAME: fapp_dns_cmd
*
* DESCRIPTION: Start DNS client/resolver.
************************************************************************/
void fapp_dns_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{

    struct fnet_dns_params      dns_params;
    fnet_netif_desc_t           netif = fnet_netif_get_default();
    fnet_char_t                 ip_str[FNET_IP_ADDR_STR_SIZE];
    fnet_index_t                error_param;

    FNET_COMP_UNUSED_ARG(argc);

    /* Set DNS client/resolver parameters.*/
    fnet_memset_zero(&dns_params, sizeof(struct fnet_dns_params));

    /**** Define addr type to request ****/
    if (!fnet_strcmp(argv[2], "4"))
    {
        dns_params.addr_family = AF_INET;
    }
    else if (!fnet_strcmp(argv[2], "6"))
    {
        dns_params.addr_family = AF_INET6;
    }
    else
    {
        error_param = 2u;
        goto ERROR_PARAMETER;
    }


    /**** Define DNS server address.****/
    if(argc == 4u)
    {
        if(fnet_inet_ptos(argv[3], &dns_params.dns_server_addr) == FNET_ERR)
        {
            error_param = 3u;
            goto ERROR_PARAMETER;
        }
    }
    else /* The DNS server address is not provided by user.*/
    {
#if FNET_CFG_IP6
        /* IPv6 DNS has higher priority over IPv4.*/
        if(fnet_netif_get_ip6_dns(netif, 0U, (fnet_ip6_addr_t *)&dns_params.dns_server_addr.sa_data) == FNET_TRUE)
        {
            dns_params.dns_server_addr.sa_family = AF_INET6;
        }
        else
#endif
#if FNET_CFG_IP4
            if( (((struct sockaddr_in *)(&dns_params.dns_server_addr))->sin_addr.s_addr = fnet_netif_get_ip4_dns(netif)) != (fnet_ip4_addr_t)0)
            {
                dns_params.dns_server_addr.sa_family = AF_INET;
            }
            else
#endif
            {
                fnet_shell_println(desc, FNET_DNS_UNKNOWN);
                return;
            }
    }

    dns_params.host_name = argv[1];                     /* Host name to resolve.*/
    dns_params.callback = fapp_dns_callback_resolved;   /* Callback function.*/
    dns_params.cookie = (fnet_uint32_t)desc;            /* Application-specific parameter
                                                        which will be passed to fapp_dns_callback_resolved().*/

    /* Run DNS cliebt/resolver. */
    if(fnet_dns_init(&dns_params) != FNET_ERR)
    {
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Resolving", dns_params.host_name);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "DNS Server",
                           fnet_inet_ntop(dns_params.dns_server_addr.sa_family, dns_params.dns_server_addr.sa_data, ip_str, sizeof(ip_str)));
        fnet_shell_println(desc, FAPP_TOCANCEL_STR);
        fnet_shell_println(desc, FAPP_DELIMITER_STR);

        fnet_shell_block(desc, fapp_dns_on_ctrlc); /* Block the shell input.*/
    }
    else
    {
        fnet_shell_println(desc, FAPP_INIT_ERR, "DNS");
    }
    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[error_param]);
    return;
}

#endif /* FAPP_CFG_DNS_CMD && FNET_CFG_DNS && FNET_CFG_DNS_RESOLVER */















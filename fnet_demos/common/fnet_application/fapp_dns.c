/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community.
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
* FNET Shell Demo implementation (DNS Resolver).
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_dns.h"
#include "fnet.h"

#if FAPP_CFG_DNS_CMD && FNET_CFG_DNS
/************************************************************************
*     Definitions.
*************************************************************************/
#define FNET_DNS_RESOLUTION_FAILED  "Resolution is FAILED"
#define FNET_DNS_UNKNOWN            "DNS server is unknown"

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_dns_callback_resolved (const struct fnet_dns_resolved_addr *addr_list, fnet_size_t addr_list_size, void *cookie);
static void fapp_dns_on_ctrlc(fnet_shell_desc_t desc, void *cookie);

static fnet_dns_desc_t fapp_dns_desc = 0; /* DNS service descriptor. */

/************************************************************************
* DESCRIPTION: Event handler callback on resolved IP address.
************************************************************************/
static void fapp_dns_callback_resolved (const struct fnet_dns_resolved_addr *addr_list, fnet_size_t addr_list_size, void *cookie)
{
    fnet_char_t         ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
    fnet_shell_desc_t   desc = (fnet_shell_desc_t) cookie;
    fnet_index_t        i;

    fnet_shell_unblock((fnet_shell_desc_t)cookie); /* Unblock the shell. */

    if(addr_list && addr_list_size)
    {
        for(i = 0u; i < addr_list_size; i++)
        {
            fnet_shell_printf(desc, FAPP_SHELL_INFO_FORMAT_S, "Resolved address",
                              fnet_inet_ntop(addr_list->resolved_addr.sa_family, addr_list->resolved_addr.sa_data, ip_str, sizeof(ip_str)));
            fnet_shell_println(desc, "\t TTL=%u", addr_list->resolved_addr_ttl);

            addr_list++;
        }
    }
    else
    {
        fnet_shell_println(desc, FNET_DNS_RESOLUTION_FAILED);
    }
}

/************************************************************************
* DESCRIPTION: Ctr+C termination handler.
************************************************************************/
static void fapp_dns_on_ctrlc(fnet_shell_desc_t desc, void *cookie)
{
    /* Terminate DNS service. */
    fnet_dns_release(fapp_dns_desc);
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* DESCRIPTION: Start DNS client/resolver.
************************************************************************/
void fapp_dns_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_dns_params      dns_params;
    fnet_netif_desc_t           netif = fnet_netif_get_default();
    fnet_char_t                 ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
    fnet_dns_desc_t             dns_desc;
    fnet_index_t                i;

    /* Set DNS client/resolver parameters.*/
    fnet_memset_zero(&dns_params, sizeof(struct fnet_dns_params));

    dns_params.addr_family = AF_INET;

    /* [-n <if name>] [-s <server ip>] [4|6] */
    for(i = 1u; i < (argc - 1) /*avoid the last parameter.*/; i++)
    {
        if (!fnet_strcmp(argv[i], "-n")) /*[-n <if name>] */
        {
            i++;
            if(i < argc)
            {
                netif = fnet_netif_get_by_name(argv[i]);
            }
            else
            {
                goto ERROR_PARAMETER;
            }
        }
        else if (!fnet_strcmp(argv[i], "-s")) /*[-s <server ip>] */
        {
            i++;
            if(i < argc)
            {
                if(fnet_inet_ptos(argv[i], &dns_params.dns_server_addr) == FNET_ERR)
                {
                    goto ERROR_PARAMETER;
                }
            }
            else
            {
                goto ERROR_PARAMETER;
            }
        }
        /* Addr family to request */
        else if (!fnet_strcmp(argv[i], "4"))
        {
            dns_params.addr_family = AF_INET;
        }
        else if (!fnet_strcmp(argv[i], "6"))
        {
            dns_params.addr_family = AF_INET6;
        }
        else/* Wrong parameter.*/
        {
            goto ERROR_PARAMETER;
        }
    }

    if(netif)
    {
        dns_params.dns_server_addr.sa_scope_id = fnet_netif_get_scope_id(netif); /* Add interface filter.*/
    }

    dns_params.host_name = argv[argc - 1];              /* Host name to resolve - last parameter.*/
    dns_params.callback = fapp_dns_callback_resolved;   /* Callback function.*/
    dns_params.cookie = desc;                           /* Application-specific parameter
                                                        which will be passed to fapp_dns_callback_resolved().*/
    /* Run DNS client/resolver. */
    dns_desc = fnet_dns_init(&dns_params);
    if(dns_desc)
    {
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Resolving", dns_params.host_name);
        if(fnet_socket_addr_is_unspecified(&dns_params.dns_server_addr) == FNET_FALSE) /* If DNS server is specified */
        {
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "DNS Server",
                               fnet_inet_ntop(dns_params.dns_server_addr.sa_family, dns_params.dns_server_addr.sa_data, ip_str, sizeof(ip_str)));
        }
        fnet_shell_println(desc, FAPP_TOCANCEL_STR);
        fnet_shell_println(desc, FAPP_DELIMITER_STR);

        fapp_dns_desc = dns_desc;

        fnet_shell_block(desc, fapp_dns_on_ctrlc, FNET_NULL); /* Block the shell input.*/
    }
    else
    {
        fnet_shell_println(desc, FAPP_INIT_ERR, "DNS");
    }
    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

#endif /* FAPP_CFG_DNS_CMD && FNET_CFG_DNS */

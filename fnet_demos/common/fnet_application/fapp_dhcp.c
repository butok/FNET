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
*  FNET Shell Demo implementation (DHCP Server and Client).
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_dhcp.h"
#include "fapp_autoip.h"

#if ((FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN) || (FAPP_CFG_DHCP_CMD && FNET_CFG_DHCP_SRV)) && FNET_CFG_IP4
/************************************************************************
*     Definitions.
*************************************************************************/
#if FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN
    #define FAPP_DHCP_CLN_DISCOVER_STR      "[DHCP] Discovering..."
    #define FAPP_DHCP_CLN_COMMAND_REBOOT    "reboot"
    #define FAPP_DHCP_CLN_COMMAND_AUTOIP    "autoip"
    #define FAPP_DHCP_SRV_COMMAND_POOL      "pool"

    static fnet_int32_t     fapp_dhcp_cln_discover_counter;
    static fnet_ip4_addr_t  fapp_dhcp_cln_ip_old;
    static fnet_ip4_addr_t  fapp_dhcp_cln_subnet_mask_old;
    #if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP
        static fnet_bool_t      fapp_dhcp_cln_autoip = FNET_FALSE; /* Flag that means to start the Auto-IP service automatically if a DHCP server is not found. */
    #endif
#endif /* FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN */

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN
    static void fapp_dhcp_cln_on_ctrlc(fnet_shell_desc_t desc, void *cookie);
    static void fapp_dhcp_cln_callback_updated(fnet_dhcp_cln_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc);
    static void fapp_dhcp_cln_callback_updated_unblock(fnet_dhcp_cln_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc);
    static void fapp_dhcp_cln_callback_discover(fnet_dhcp_cln_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc);
#endif

#if FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN
/************************************************************************
* DESCRIPTION: Ctr+C termination handler.
************************************************************************/
static void fapp_dhcp_cln_on_ctrlc(fnet_shell_desc_t desc, void *cookie)
{
    fnet_netif_desc_t            netif = (fnet_netif_desc_t)cookie;

    /* Release DHCP. */
    fnet_dhcp_cln_release(fnet_dhcp_cln_get_by_netif(netif));
    /* Restore old ip address, as DHCP set it to zero. */
    fnet_netif_set_ip4_addr(netif, fapp_dhcp_cln_ip_old, fapp_dhcp_cln_subnet_mask_old );

    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* DESCRIPTION: Event handler callback on new IP from DHCP client.
************************************************************************/
static void fapp_dhcp_cln_callback_updated(fnet_dhcp_cln_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    fapp_addr_callback_updated(desc, netif);
}
static void fapp_dhcp_cln_callback_updated_unblock(fnet_dhcp_cln_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    fnet_dhcp_cln_set_callback_discover(dhcp_desc, FNET_NULL, FNET_NULL); /* Disable discovery callback */
    fnet_dhcp_cln_set_callback_updated(dhcp_desc, fapp_dhcp_cln_callback_updated, shl_desc); /* Use shell non-blocking version */

    fapp_dhcp_cln_callback_updated(dhcp_desc, netif, shl_desc);

    fnet_shell_unblock(desc); /* Unblock the shell. */
}

/************************************************************************
* DESCRIPTION: Event handler callback on new discover iteration.
************************************************************************/
static void fapp_dhcp_cln_callback_discover(fnet_dhcp_cln_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    FNET_COMP_UNUSED_ARG(dhcp_desc);

    /* Counter is  0, so cancel DHCP */
    if(fapp_dhcp_cln_discover_counter == 0)
    {
        fnet_shell_unblock(desc);
        fapp_dhcp_cln_on_ctrlc(desc, netif); /* Cancel DHCP.*/
#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP
        /* Satrt AutoIP */
        if(fapp_dhcp_cln_autoip == FNET_TRUE)
        {
            fapp_autoip_init(desc, netif);
        }
#endif
    }
    else
    {
        fapp_dhcp_cln_discover_counter--;
        fnet_shell_println(desc, FAPP_DHCP_CLN_DISCOVER_STR);
    }
}

/************************************************************************
* DESCRIPTION: Releases all DHCP clients.
*************************************************************************/
void fapp_dhcp_cln_release(void)
{
    fnet_netif_desc_t       netif;
    fnet_index_t            i;
    fnet_dhcp_cln_desc_t    dhcp;

    /* Release all DHCP clients.*/
    for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
    {
        dhcp = fnet_dhcp_cln_get_by_netif(netif);
        if(dhcp)
        {
            fnet_dhcp_cln_release(dhcp);
        }
    }
}

/************************************************************************
* DESCRIPTION: Enable DHCPv4 client.
************************************************************************/
void fapp_dhcp_cln_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t               netif = fnet_netif_get_default(); /* By default is "default" netif*/
    fnet_bool_t                     init = FNET_TRUE; /* By default is "init".*/
    fnet_index_t                    i;
    struct fnet_dhcp_cln_params     dhcp_params;
    fnet_dhcp_cln_desc_t                dhcp_desc;

#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP
    fapp_dhcp_cln_autoip = FNET_FALSE;
#endif

    /* "[-n <if name>] [release|autoip]" */
    for(i = 1u; i < argc; i++)
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
        else if (!fnet_strcmp(argv[i], FAPP_COMMAND_RELEASE)) /* [release] */
        {
            init = FNET_FALSE;
        }
        else if (!fnet_strcmp(argv[i], FAPP_DHCP_CLN_COMMAND_AUTOIP)) /* [autoip] */
        {
#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP
            fapp_dhcp_cln_autoip = FNET_TRUE;
#endif
        }
        else/* Wrong parameter.*/
        {
            goto ERROR_PARAMETER;
        }
    }

    if(init == FNET_TRUE)
    {
        fapp_dhcp_cln_discover_counter = FAPP_CFG_DHCPC_CMD_DISCOVER_MAX; /* reset counter.*/

        /* Init parameters.*/
        fnet_memset_zero(&dhcp_params, sizeof(struct fnet_dhcp_cln_params));
        dhcp_params.netif = netif;
        dhcp_params.probe_addr = FNET_TRUE; /* Enable probing of the newly received address.*/

        /* Save current IP address, to restore if cancelled. */
        fapp_dhcp_cln_ip_old = fnet_netif_get_ip4_addr(netif);
        fapp_dhcp_cln_subnet_mask_old = fnet_netif_get_ip4_subnet_mask(netif);

        /* Start DHCP client */
        dhcp_desc = fnet_dhcp_cln_init(&dhcp_params);
        if(dhcp_desc)
        {
            fnet_char_t netif_name[FNET_NETIF_NAMELEN];

            fnet_netif_get_name(netif, netif_name, sizeof(netif_name));

            /* Register DHCP event handler callbacks. */
            fnet_dhcp_cln_set_callback_updated(dhcp_desc, fapp_dhcp_cln_callback_updated_unblock, (void *)desc);
            fnet_dhcp_cln_set_callback_discover(dhcp_desc, fapp_dhcp_cln_callback_discover, (void *)desc);

            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " DHCPv4 Client started.");
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Interface", netif_name );
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fnet_shell_println(desc, FAPP_TOCANCEL_STR);
            fnet_shell_block(desc, fapp_dhcp_cln_on_ctrlc, netif); /* Block shell. */
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "DHCP Client");
        }
    }
    else /* [release] */
    {
        fnet_dhcp_cln_release(fnet_dhcp_cln_get_by_netif(netif));
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

/************************************************************************
* DESCRIPTION: Print service state.
*************************************************************************/
void fapp_dhcp_cln_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif)
{
    fnet_char_t             ip_str[FNET_IP4_ADDR_STR_SIZE];
    fnet_dhcp_cln_desc_t    dhcp_desc = fnet_dhcp_cln_get_by_netif(netif);
    fnet_bool_t             dhcp_is_enabled = fnet_dhcp_cln_is_enabled(dhcp_desc);

    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "DHCP Client", fapp_is_enabled_str[dhcp_is_enabled]);

    if(dhcp_is_enabled && (fnet_netif_get_ip4_addr_type(netif) == FNET_NETIF_IP_ADDR_TYPE_DHCP))
    {
        struct fnet_dhcp_cln_options options;

        fnet_dhcp_cln_get_options(dhcp_desc, &options);

        fnet_inet_ntoa(*(struct fnet_in_addr *)( &options.dhcp_server), ip_str);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "DHCP Server Address", ip_str);
#if 0 /* Not too important */
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Lease Time", fnet_ntohl(options.lease_time));
#endif
    }
}
#endif /* FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN */

#if FAPP_CFG_DHCP_CMD && FNET_CFG_DHCP_SRV
/************************************************************************
* DESCRIPTION: Enable DHCPv4 server.
************************************************************************/
void fapp_dhcp_srv_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t               netif = fnet_netif_get_default(); /* By default is "default" netif*/
    fnet_bool_t                     init = FNET_TRUE; /* By default is "init".*/
    fnet_bool_t                     pool = FNET_FALSE; /* By default is "init".*/
    fnet_index_t                    i;
    struct fnet_dhcp_srv_params     dhcp_params;
    fnet_dhcp_srv_desc_t            dhcp_desc;

    /* "[-n <if name>] [release|autoip]" */
    for(i = 1u; i < argc; i++)
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
        else if (!fnet_strcmp(argv[i], FAPP_COMMAND_RELEASE)) /* [release] */
        {
            init = FNET_FALSE;
        }
        else if (!fnet_strcmp(argv[i], FAPP_DHCP_SRV_COMMAND_POOL)) /* [pool] */
        {
            pool = FNET_TRUE;
        }
        else/* Wrong parameter.*/
        {
            goto ERROR_PARAMETER;
        }
    }

    if(pool == FNET_TRUE)
    {
        fnet_dhcp_srv_addr_pool_info_t      addr_pool_info;
        fnet_char_t                         addr_str[FNET_IP4_ADDR_STR_SIZE];
        fnet_char_t                         mac_str[FNET_MAC_ADDR_STR_SIZE];
        fnet_index_t                        i;

        fnet_shell_println(desc, " DHCPv4 Server Pool:");
        for(i = 0U; fnet_dhcp_srv_get_addr_pool_info (fnet_dhcp_srv_get_by_netif(netif), i, &addr_pool_info ) == FNET_TRUE; i++)
        {
            fnet_shell_println(desc, "\t[%d] %s = %s (%d sec)", i,
                               fnet_inet_ntop(AF_INET, &addr_pool_info.client_ip4_addr, addr_str, sizeof(addr_str)),
                               fnet_mac_to_str(addr_pool_info.client_mac_addr, mac_str),
                               addr_pool_info.lease_time);
        }
        if(i == 0U)
        {
            fnet_shell_println(desc, "\t<empty>");
        }
    }
    else if(init == FNET_TRUE)
    {
        /* Set the default IPv4 address for the interface running the DHCP server.*/
        fnet_netif_set_ip4_addr(netif, FAPP_CFG_DHCP_CMD_IP4_ADDR, FAPP_CFG_DHCP_CMD_IP4_MASK);
        fnet_netif_set_ip4_gateway(netif, FAPP_CFG_DHCP_CMD_IP4_GW);
#if FNET_CFG_DNS
        fnet_netif_set_ip4_dns(netif, FAPP_CFG_DHCP_CMD_IP4_DNS);
#endif

        /* Init parameters.*/
        fnet_memset_zero(&dhcp_params, sizeof(dhcp_params));
        dhcp_params.netif = netif;

        /* Start DHCP server */
        dhcp_desc = fnet_dhcp_srv_init(&dhcp_params);
        if(dhcp_desc)
        {
            fnet_char_t netif_name[FNET_NETIF_NAMELEN];

            fnet_netif_get_name(netif, netif_name, sizeof(netif_name));

            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " DHCPv4 server started.");
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Interface", netif_name );
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "DHCP Server");
        }
    }
    else /* [release] */
    {
        fnet_dhcp_srv_release(fnet_dhcp_srv_get_by_netif(netif));
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

/************************************************************************
* DESCRIPTION: Releases all DHCP servers.
*************************************************************************/
void fapp_dhcp_srv_release(void)
{
    fnet_netif_desc_t       netif;
    fnet_index_t            i;
    fnet_dhcp_srv_desc_t    dhcp;

    /* Release all DHCP servers.*/
    for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
    {
        dhcp = fnet_dhcp_srv_get_by_netif(netif);
        if(dhcp)
        {
            fnet_dhcp_srv_release(dhcp);
        }
    }
}

/************************************************************************
* DESCRIPTION: Print DHCP Server service state.
*************************************************************************/
void fapp_dhcp_srv_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif)
{
    fnet_dhcp_srv_desc_t    dhcp_desc = fnet_dhcp_srv_get_by_netif(netif);
    fnet_bool_t             dhcp_is_enabled = fnet_dhcp_srv_is_enabled(dhcp_desc);

    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "DHCPv4 Server", fapp_is_enabled_str[dhcp_is_enabled]);
}
#endif /* FAPP_CFG_DHCP_CMD && FNET_CFG_DHCP_SRV */


#endif /* ((FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN) || (FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN)) && FNET_CFG_IP4 */


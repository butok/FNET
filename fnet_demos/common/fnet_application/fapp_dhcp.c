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
* @file fapp_dhcp.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo implementation.
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_dhcp.h"
#include "fapp_autoip.h"

#if FAPP_CFG_DHCP_CMD && FNET_CFG_DHCP && FNET_CFG_IP4
/************************************************************************
*     Definitions.
*************************************************************************/
#define FAPP_DHCP_DISCOVER_STR      "DHCP: Discovering..."
#define FAPP_DHCP_COMMAND_REBOOT    "reboot"
#define FAPP_DHCP_COMMAND_AUTOIP    "autoip"

static fnet_int32_t     fapp_dhcp_discover_counter;
static fnet_ip4_addr_t  fapp_dhcp_ip_old;
static fnet_ip4_addr_t  fapp_dhcp_subnet_mask_old;
static fnet_dhcp_desc_t fapp_dhcp_desc = 0; /* DHCP descriptor. */
#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP
    static fnet_bool_t      fapp_dhcp_autoip = FNET_FALSE; /* Flag that means to start the Auto-IP service automatically if a DHCP server is not found. */
#endif

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_dhcp_on_ctrlc(fnet_shell_desc_t desc);
static void fapp_dhcp_callback_updated(fnet_dhcp_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc);
static void fapp_dhcp_callback_discover(fnet_dhcp_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc);

/************************************************************************
* NAME: fapp_dhcp_on_ctrlc
*
* DESCRIPTION:
************************************************************************/
static void fapp_dhcp_on_ctrlc(fnet_shell_desc_t desc)
{
    /* Release DHCP. */
    fapp_dhcp_release();
    /* Restore old ip address, as DHCP set it to zero. */
    fnet_netif_set_ip4_addr( fnet_netif_get_default(), fapp_dhcp_ip_old, fapp_dhcp_subnet_mask_old );
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* NAME: fapp_dhcp_callback_updated
*
* DESCRIPTION: Event handler callback on new IP from DHCP client.
************************************************************************/
static void fapp_dhcp_callback_updated(fnet_dhcp_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    fapp_dhcp_discover_counter = FAPP_CFG_DHCP_CMD_DISCOVER_MAX; /* reset counter.*/

    fapp_addr_callback_updated(desc, netif);
}

/************************************************************************
* NAME: fapp_dhcp_callback_discover
*
* DESCRIPTION: Event handler callback on new IP from DHCP client.
************************************************************************/
static void fapp_dhcp_callback_discover(fnet_dhcp_desc_t dhcp_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    FNET_COMP_UNUSED_ARG(netif);
    FNET_COMP_UNUSED_ARG(dhcp_desc);

    if(fapp_dhcp_discover_counter == 0)
    {
        fnet_shell_unblock((fnet_shell_desc_t)shl_desc);
        fapp_dhcp_on_ctrlc((fnet_shell_desc_t)shl_desc); /* Cancel DHCP.*/
#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP
        /* Satrt AutoIP */
        if(fapp_dhcp_autoip == FNET_TRUE)
        {
            fnet_shell_script(desc, "autoip");
        }
#endif
    }
    else
    {
        fapp_dhcp_discover_counter--;
        fnet_shell_println(desc, FAPP_DHCP_DISCOVER_STR);
    }
}

/************************************************************************
* NAME: fapp_dhcp_release
*
* DESCRIPTION: Releases DHCP client.
*************************************************************************/
void fapp_dhcp_release(void)
{
    fnet_dhcp_release(fapp_dhcp_desc);
    fapp_dhcp_desc = 0;
}

/************************************************************************
* NAME: fapp_dhcp_cmd
*
* DESCRIPTION: Enable DHCP client.
************************************************************************/
void fapp_dhcp_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct              fnet_dhcp_params dhcp_params;
    fnet_dhcp_desc_t    dhcp_desc;
    fnet_netif_desc_t   netif = fnet_netif_get_default();

    if(argc == 1u    /* By default is "init".*/
#if 0 /* DHCP reboot feature not used too much. */
       || ((argc == 2u) && (fnet_strcasecmp(&FAPP_DHCP_COMMAND_REBOOT[0], argv[1]) == 0)) /* [reboot] */
#endif
#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP
       || ((argc == 2u) && (fnet_strcasecmp(&FAPP_DHCP_COMMAND_AUTOIP[0], argv[1]) == 0)) /* [autoip] */
#endif
      )
    {
        fnet_memset_zero(&dhcp_params, sizeof(struct fnet_dhcp_params));

        fapp_dhcp_discover_counter = FAPP_CFG_DHCP_CMD_DISCOVER_MAX; /* reset counter.*/

#if 0 /* DHCP reboot feature not used too much. */
        if(fnet_strcasecmp(&FAPP_DHCP_COMMAND_REBOOT[0], argv[1]) == 0) /* [reboot] */
            dhcp_params.requested_ip_address.s_addr = fnet_netif_get_ip4_addr(netif);
#endif
#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP
        /* Check Auto-IP flag.*/
        if((argc == 2u) && (fnet_strcasecmp(&FAPP_DHCP_COMMAND_AUTOIP[0], argv[1]) == 0))
        {
            fapp_dhcp_autoip = FNET_TRUE;
        }
        else
        {
            fapp_dhcp_autoip = FNET_FALSE;
        }
#endif

        dhcp_params.probe_addr = FNET_TRUE; /* Enable probing of the newly received address.*/

        /* Save current IP address, to restore if cancelled.  */
        fapp_dhcp_ip_old = fnet_netif_get_ip4_addr(netif);
        fapp_dhcp_subnet_mask_old = fnet_netif_get_ip4_subnet_mask(netif);

        /* Start DHCP client */
        dhcp_desc = fnet_dhcp_init(netif, &dhcp_params);
        if(dhcp_desc)
        {
            fapp_dhcp_desc = dhcp_desc;

            /* Register DHCP event handler callbacks. */
            fnet_dhcp_set_callback_updated(fapp_dhcp_desc, fapp_dhcp_callback_updated, (void *)desc);
            fnet_dhcp_set_callback_discover(fapp_dhcp_desc, fapp_dhcp_callback_discover, (void *)desc);

            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " DHCPv4 Client started.");
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fnet_shell_println(desc, FAPP_TOCANCEL_STR);
            fnet_shell_block(desc, fapp_dhcp_on_ctrlc); /* Block shell. */
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "DHCP");
        }
    }
    else if((argc == 2u) && (fnet_strcasecmp(&FAPP_COMMAND_RELEASE[0], argv[1]) == 0)) /* [release] */
    {
        fapp_dhcp_release();
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}

/************************************************************************
* NAME: fapp_dhcp_info
*
* DESCRIPTION: Print service state.
*************************************************************************/
void fapp_dhcp_info(fnet_shell_desc_t desc)
{
    fnet_char_t     ip_str[FNET_IP4_ADDR_STR_SIZE];
    fnet_bool_t     dhcp_is_enabled = fnet_dhcp_is_enabled(fapp_dhcp_desc);

    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "DHCP Client", fapp_enabled_str[dhcp_is_enabled]);

    if(dhcp_is_enabled && (fnet_netif_get_ip4_addr_type(fnet_netif_get_default()) == FNET_NETIF_IP_ADDR_TYPE_DHCP))
    {
        struct fnet_dhcp_options options;

        fnet_dhcp_get_options(fapp_dhcp_desc, &options);

        fnet_inet_ntoa(*(struct in_addr *)( &options.dhcp_server), ip_str);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "DHCP Server", ip_str);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Lease Time", fnet_ntohl(options.lease_time));
    }
}

#endif /* FAPP_CFG_DHCP_CMD && FNET_CFG_DHCP && FNET_CFG_IP4 */


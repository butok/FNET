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
* @file fapp_autoip.c
*
* @author Andrey Butok
*
* @brief FNET Application Demo (Auto-IP).
*
***************************************************************************/

#include "fapp.h"

#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP && FNET_CFG_IP4

#include "fapp_prv.h"
#include "fapp_autoip.h"

/************************************************************************
*     Definitions.
*************************************************************************/
#define FAPP_AUTOIP_PROBE_STR      "Probing..."

static fnet_autoip_desc_t   fapp_autoip_desc = 0; /* Auto-IP service descriptor. */
static fnet_ip4_addr_t      fapp_autoip_ip_old;

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_autoip_on_ctrlc(fnet_shell_desc_t desc);
static void fapp_autoip_callback_updated(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc );
static void fapp_autoip_callback_probe(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc );

/************************************************************************
* NAME: fapp_dhcp_on_ctrlc
*
* DESCRIPTION:
************************************************************************/
static void fapp_autoip_on_ctrlc(fnet_shell_desc_t desc)
{
    /* Release Auto-IP. */
    fapp_autoip_release();
    /* Restore old ip address, as Auto-IP set it to zero. */
    fnet_netif_set_ip4_addr( fnet_netif_get_default(), fapp_autoip_ip_old );
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* NAME: fapp_autoip_release
*
* DESCRIPTION: Releases Auto-IP server.
*************************************************************************/
void fapp_autoip_release(void)
{
    fnet_autoip_release(fapp_autoip_desc);
    fapp_autoip_desc = 0;
}

/************************************************************************
* NAME: fapp_autoip_callback_updated
*
* DESCRIPTION: Event handler callback on new IP from Auto-IP service.
************************************************************************/
static void fapp_autoip_callback_updated(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    /* Optionally, unregister Auto-IP event handler callbacks, just to do not
     * disturb a user. */
    fnet_autoip_set_callback_updated(autoip_desc, 0, 0);
    fnet_autoip_set_callback_probe(autoip_desc, 0, 0);

    fnet_shell_unblock((fnet_shell_desc_t)shl_desc); /* Unblock the shell. */

    /* Print updated parameters info. */
    fnet_shell_println( desc, "\n%s", FAPP_DELIMITER_STR);
    fnet_shell_println( desc, FAPP_UPDATED_IP_STR);
    fnet_shell_println( desc, FAPP_DELIMITER_STR);

    fapp_print_netif_info( desc, netif );
}

/************************************************************************
* NAME: fapp_autoip_callback_probe
*
* DESCRIPTION: Event handler callback on "Address probing" by Auto-IP service.
************************************************************************/
static void fapp_autoip_callback_probe(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    FNET_COMP_UNUSED_ARG(netif);
    FNET_COMP_UNUSED_ARG(autoip_desc);

    fnet_shell_println(desc, FAPP_AUTOIP_PROBE_STR);
}

/************************************************************************
* NAME: fapp_autoip_cmd
*
* DESCRIPTION: Run Auto-IP service.
*************************************************************************/
void fapp_autoip_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_autoip_desc_t  autoip_desc;
    fnet_netif_desc_t   netif_desc = fnet_netif_get_default();

    if(argc == 1u) /* By default is "init".*/
    {
        /* Save current IP address only if it was allocated manually/statically. */
        if(fnet_netif_get_ip4_addr_type(netif_desc) == FNET_NETIF_IP_ADDR_TYPE_MANUAL)
        {
            fapp_autoip_ip_old = fnet_netif_get_ip4_addr(netif_desc); /* Save ip to restore if cancelled. */
        }
        else
        {
            fapp_autoip_ip_old = 0;
        }

        /* Start Auto-IP service */
        autoip_desc = fnet_autoip_init(netif_desc);
        if(autoip_desc)
        {
            /* Register Auto-IP event handler callbacks. */
            fnet_autoip_set_callback_updated(autoip_desc, fapp_autoip_callback_updated, (void *)desc);
            fnet_autoip_set_callback_probe(autoip_desc, fapp_autoip_callback_probe, (void *)desc);

            fapp_autoip_desc = autoip_desc;

            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " Auto-IP service started.");
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fnet_shell_println(desc, FAPP_TOCANCEL_STR);
            fnet_shell_block(desc, fapp_autoip_on_ctrlc); /* Block shell. */
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "AOTO-IP");
        }
    }
    else if((argc == 2u) && (fnet_strcasecmp(&FAPP_COMMAND_RELEASE[0], argv[1]) == 0)) /* [release] */
    {
        fapp_autoip_release();
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}

/************************************************************************
* NAME: fapp_autoip_info
*
* DESCRIPTION:
*************************************************************************/
void fapp_autoip_info(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "AUTOIP Service", fapp_enabled_str[fnet_autoip_is_enabled(fapp_autoip_desc)]);
}


#endif /* FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP && FNET_CFG_IP4 */

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
#define FAPP_AUTOIP_PROBE_STR      "AutoIP: Probing..."

static fnet_autoip_desc_t   fapp_autoip_desc = 0; /* Auto-IP service descriptor. */
static fnet_ip4_addr_t      fapp_autoip_ip_old;
static fnet_ip4_addr_t      fapp_autoip_subnet_mask_old;

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_autoip_on_ctrlc(fnet_shell_desc_t desc);
static void fapp_autoip_callback_updated(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc );
static void fapp_autoip_callback_probe(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc );

/************************************************************************
* DESCRIPTION:
************************************************************************/
static void fapp_autoip_on_ctrlc(fnet_shell_desc_t desc)
{
    /* Release Auto-IP. */
    fapp_autoip_release();
    /* Restore old ip address, as Auto-IP set it to zero. */
    fnet_netif_set_ip4_addr( fnet_netif_get_default(), fapp_autoip_ip_old, fapp_autoip_subnet_mask_old );
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* DESCRIPTION: Releases Auto-IP server.
*************************************************************************/
void fapp_autoip_release(void)
{
    fnet_autoip_release(fapp_autoip_desc);
    fapp_autoip_desc = 0;
}

/************************************************************************
* DESCRIPTION: Event handler callback on new IP from Auto-IP service.
************************************************************************/
static void fapp_autoip_callback_updated(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    fapp_addr_callback_updated(desc, netif);
}

/************************************************************************
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
* DESCRIPTION: Run Auto-IP service.
*************************************************************************/
void fapp_autoip_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_autoip_params   params;
    fnet_autoip_desc_t          autoip_desc;
    fnet_netif_desc_t           netif_desc = fnet_netif_get_default();

    if(argc == 1u) /* By default is "init".*/
    {
        /* Save current IP address, to restore if cancelled.  */
        fapp_autoip_ip_old = fnet_netif_get_ip4_addr(netif_desc);
        fapp_autoip_subnet_mask_old = fnet_netif_get_ip4_subnet_mask(netif_desc);

        /* Init parameters.*/
        fnet_memset_zero(&params, sizeof(params));
        params.netif_desc = netif_desc;
        /* RFC: When the network media indicates that it has been connected, the
           autoconfiguration process begins again, and attempts to re-use the
           previously assigned Link-Local address. */
        params.ip_address = fapp_autoip_ip_old;

        /* Start Auto-IP service */
        autoip_desc = fnet_autoip_init(&params);
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
* DESCRIPTION:
*************************************************************************/
void fapp_autoip_info(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "AUTOIP Service", fapp_is_enabled_str[fnet_autoip_is_enabled(fapp_autoip_desc)]);
}

#endif /* FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP && FNET_CFG_IP4 */

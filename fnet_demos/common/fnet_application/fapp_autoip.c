/**************************************************************************
*
* Copyright 2017 by Andrey Butok. FNET Community.
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
*  FNET Application Demo (Auto-IP).
*
***************************************************************************/

#include "fapp.h"

#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP && FNET_CFG_IP4

#include "fapp_prv.h"
#include "fapp_autoip.h"

/************************************************************************
*     Definitions.
*************************************************************************/
#define FAPP_AUTOIP_PROBE_STR      "[AUTOIP] Probing..."

static fnet_ip4_addr_t      fapp_autoip_ip_old;
static fnet_ip4_addr_t      fapp_autoip_subnet_mask_old;

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_autoip_on_ctrlc(fnet_shell_desc_t desc, void *cookie);
static void fapp_autoip_callback_updated(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc );
static void fapp_autoip_callback_updated_unblock(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc );
static void fapp_autoip_callback_probe(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc );

/************************************************************************
* DESCRIPTION:
************************************************************************/
static void fapp_autoip_on_ctrlc(fnet_shell_desc_t desc, void *cookie)
{
    fnet_netif_desc_t            netif = (fnet_netif_desc_t)cookie;

    /* Release Auto-IP. */
    fnet_autoip_release(fnet_autoip_get_by_netif(netif));
    /* Restore old ip address, as Auto-IP set it to zero. */
    fnet_netif_set_ip4_addr(netif, fapp_autoip_ip_old, fapp_autoip_subnet_mask_old);

    fnet_shell_println(desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* DESCRIPTION: Releases all Auto-IP services
*************************************************************************/
void fapp_autoip_release(void)
{
    fnet_netif_desc_t   netif;
    fnet_index_t        i;
    fnet_autoip_desc_t  autoip;

    /* Release all Auto-IP services.*/
    for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
    {
        autoip = fnet_autoip_get_by_netif(netif);
        if(autoip)
        {
            fnet_autoip_release(autoip);
        }
    }
}

/************************************************************************
* DESCRIPTION: Event handler callback on new IP from Auto-IP service.
************************************************************************/
static void fapp_autoip_callback_updated(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    fapp_addr_callback_updated(desc, netif);

}
static void fapp_autoip_callback_updated_unblock(fnet_autoip_desc_t autoip_desc, fnet_netif_desc_t netif, void *shl_desc )
{
    fnet_shell_desc_t desc = (fnet_shell_desc_t) shl_desc;

    fnet_autoip_set_callback_updated(autoip_desc, fapp_autoip_callback_updated, shl_desc);  /* Use shell non-blocking version */
    fnet_shell_unblock(desc); /* Unblock the shell. */

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
* DESCRIPTION: Init Auto-IP service.
*************************************************************************/
void fapp_autoip_init( fnet_shell_desc_t desc, fnet_netif_desc_t  netif )
{
    struct fnet_autoip_params   params;
    fnet_autoip_desc_t          autoip_desc;

    if(desc && netif)
    {
        /* Save current IP address, to restore if cancelled.  */
        fapp_autoip_ip_old = fnet_netif_get_ip4_addr(netif);
        fapp_autoip_subnet_mask_old = fnet_netif_get_ip4_subnet_mask(netif);

        /* Init parameters.*/
        fnet_memset_zero(&params, sizeof(params));
        params.netif_desc = netif;
        /* RFC: When the network media indicates that it has been connected, the
           autoconfiguration process begins again, and attempts to re-use the
           previously assigned Link-Local address. */
        params.ip_address = fapp_autoip_ip_old;

        /* Start Auto-IP service */
        autoip_desc = fnet_autoip_init(&params);
        if(autoip_desc)
        {
            fnet_char_t netif_name[FNET_NETIF_NAMELEN];

            fnet_netif_get_name(netif, netif_name, sizeof(netif_name));

            /* Register Auto-IP event handler callbacks. */
            fnet_autoip_set_callback_updated(autoip_desc, fapp_autoip_callback_updated_unblock, (void *)desc);
            fnet_autoip_set_callback_probe(autoip_desc, fapp_autoip_callback_probe, (void *)desc);

            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " Auto-IPv4 service started.");
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Interface", netif_name );
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fnet_shell_println(desc, FAPP_TOCANCEL_STR);
            fnet_shell_block(desc, fapp_autoip_on_ctrlc, netif); /* Block shell. */
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "AOTO-IP");
        }
    }
}

/************************************************************************
* DESCRIPTION: Run Auto-IP service.
*************************************************************************/
void fapp_autoip_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t           netif = fnet_netif_get_default(); /* By default is "default" netif*/
    fnet_bool_t                 init = FNET_TRUE; /* By default is "init".*/
    fnet_index_t                i;

    /* [-n <if name>] [release] */
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
        else/* Wrong parameter.*/
        {
            goto ERROR_PARAMETER;
        }
    }

    if(init == FNET_TRUE)
    {
        fapp_autoip_init(desc, netif);
    }
    else /* [release] */
    {
        fnet_autoip_release(fnet_autoip_get_by_netif(netif));
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
void fapp_autoip_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif)
{
    fnet_autoip_desc_t    autoip_desc = fnet_autoip_get_by_netif(netif);

    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "AUTOIP Service", fapp_is_enabled_str[fnet_autoip_is_enabled(autoip_desc)]);
}

#endif /* FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP && FNET_CFG_IP4 */

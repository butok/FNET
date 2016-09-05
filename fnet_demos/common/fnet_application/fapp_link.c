/**************************************************************************
*
* Copyright 2014 by Andrey Butok. FNET Community.
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
* @file fapp_link.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo (Link).
*
***************************************************************************/

#include "fapp.h"

#if FNET_CFG_LINK

#include "fapp_prv.h"
#include "fapp_link.h"

static fnet_link_desc_t fapp_link_desc = 0; /* Link-Detection service descriptor. */

/************************************************************************
* DESCRIPTION: Link-Detection event handler callback.
*************************************************************************/
static void fapp_link_callback(fnet_netif_desc_t netif, fnet_bool_t connected, void *callback_param)
{
    fnet_shell_desc_t   desc = (fnet_shell_desc_t)callback_param;
    fnet_char_t         name[FNET_NETIF_NAMELEN];
    fnet_char_t         *script;

    fnet_netif_get_name(netif, name, sizeof(name));

    /* Start connect/unconnect script.*/
    if(connected == FNET_TRUE)
    {
        script = FAPP_CFG_LINK_CONNECT_SCRIPT;
    }
    else
    {
        script = FAPP_CFG_LINK_UNCONNECT_SCRIPT;
    }

    /* Print interface connection status.*/
    fnet_shell_println(desc, "\n%s: %s => Script: %s", name, fapp_netif_connection_state_str[fnet_netif_is_connected(netif)], script);
    fnet_shell_script(desc, script);
}

/************************************************************************
* DESCRIPTION: Run Link-Detection service.
*************************************************************************/
void fapp_link_init(fnet_shell_desc_t desc)
{
    struct fnet_link_params    params;
    fnet_link_desc_t           link_desc;

    /* Init parameters.*/
    fnet_memset_zero(&params, sizeof(params));
    params.netif_desc = fnet_netif_get_default();
    params.callback = fapp_link_callback;
    params.callback_param = (void *)desc;

    /* Start Link-Detection server */
    link_desc = fnet_link_init(&params);
    if(link_desc)
    {
        fapp_link_desc = link_desc;
    }
    else
    {
        fnet_shell_println(desc, FAPP_INIT_ERR, "LINK");
    }
}

/************************************************************************
* DESCRIPTION: Releases Link-Detection service.
*************************************************************************/
void fapp_link_release(void)
{
    fnet_link_release(fapp_link_desc);
    fapp_link_desc = 0;
}

/************************************************************************
* DESCRIPTION: Print service state.
*************************************************************************/
void fapp_link_info(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "LINK Service", fapp_enabled_str[fnet_link_is_enabled(fapp_link_desc)]);
}

#endif /* FNET_CFG_LINK */

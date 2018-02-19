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
*  FNET Shell Demo (Link).
*
***************************************************************************/

#include "fapp.h"

#if FNET_CFG_LINK

#include "fapp_prv.h"
#include "fapp_link.h"
#include "fapp_netif.h"

static void fapp_link_script_add_netif_name(fnet_char_t *script_out, fnet_size_t script_out_size, fnet_char_t *script_in, fnet_char_t *netif_name);

/************************************************************************
* DESCRIPTION: Link-Detection event handler callback.
*************************************************************************/
static void fapp_link_callback(fnet_netif_desc_t netif, fnet_bool_t connected, void *callback_param)
{
    fnet_shell_desc_t   desc = (fnet_shell_desc_t)callback_param;
    fnet_char_t         netif_name[FNET_NETIF_NAMELEN];
    fnet_char_t         *script_p;
    fnet_char_t         script[FAPP_CFG_SHELL_MAX_LINE_LENGTH];
    
    fnet_netif_get_name(netif, netif_name, sizeof(netif_name));

    /* connect/disconnect script.*/
    if(fnet_wifi_get_op_mode(netif) == FNET_WIFI_OP_MODE_ACCESS_POINT)
    {
        /* Wi-Fi interface in the access point operation mode. */
        if(connected == FNET_TRUE)
        {
            script_p = FAPP_CFG_LINK_CONNECT_WIFI_ACCESS_POINT_SCRIPT;
        }
        else
        {
            script_p = FAPP_CFG_LINK_DISCONNECT_WIFI_ACCESS_POINT_SCRIPT;
        }
    }
    else
    {
        if(connected == FNET_TRUE)
        {
            script_p = FAPP_CFG_LINK_CONNECT_SCRIPT;
        }
        else
        {
            script_p = FAPP_CFG_LINK_DISCONNECT_SCRIPT;
        }
    }

    /* Add interface name */
    fapp_link_script_add_netif_name(script, sizeof(script), script_p, netif_name );

    /* Print interface connection status.*/
    fnet_shell_println(desc, "\n[LINK] %s: %s => Script: %s", netif_name, fapp_netif_connection_state_str[fnet_netif_is_connected(netif)], script);
    if(fnet_shell_script(desc, script) == FNET_ERR)
    {
        fnet_shell_println(desc, "Script error!");
    }
}

/************************************************************************
* DESCRIPTION: Parse input script and replace "%s" by the interface name.
*************************************************************************/
static void fapp_link_script_add_netif_name(fnet_char_t *script_out, fnet_size_t script_out_size, fnet_char_t *script_in, fnet_char_t *netif_name)
{

#if 0 //DM
    fnet_char_t     *script_in_cur = script_in;
    fnet_index_t    counter = 0;

    /* Find "%s"*/
    while((script_in_cur = fnet_strstr(script_in_cur, " %s" )) != 0)
    {
        script_in_cur += sizeof("%s");
        counter++;
    }

    /* Insert interface name */
    while(counter)
    {
        fnet_snprintf(script_out, script_out_size, script_in, netif_name);
        counter--;
    }
#endif

    /* Very rude. TBD better way */
    fnet_snprintf(script_out, script_out_size, script_in, 
                netif_name, netif_name, netif_name, netif_name, netif_name, netif_name, netif_name, netif_name, netif_name, netif_name);
}

/************************************************************************
* DESCRIPTION: Run Link-Detection service.
*************************************************************************/
void fapp_link_init(fnet_shell_desc_t desc, fnet_netif_desc_t netif)
{
    struct fnet_link_params    params;
    fnet_link_desc_t           link_desc;

    /* Init parameters.*/
    fnet_memset_zero(&params, sizeof(params));
    params.netif_desc = netif;
    params.callback = fapp_link_callback;
    params.callback_param = (void *)desc;

    /* Start Link-Detection service */
    link_desc = fnet_link_init(&params);
    if(link_desc)
    {
#if 0
        fnet_char_t netif_name[FNET_NETIF_NAMELEN];
        
        fnet_netif_get_name(netif, netif_name, sizeof(netif_name));
        
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
        fnet_shell_println(desc, " Link-Detection service started.");
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Interface", netif_name );
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
#endif
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
    fnet_netif_desc_t   netif;
    fnet_index_t        i;
    fnet_link_desc_t    link;

    /* Release all Link services.*/
    for(i=0; (netif = fnet_netif_get_by_number(i)); i++)
    {
        link = fnet_link_get_by_netif(netif);
        if(link)
        {
            fnet_link_release(link);
        }
    }
}

/************************************************************************
* DESCRIPTION: Print service state.
*************************************************************************/
void fapp_link_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif)
{
    fnet_link_desc_t    link_desc = fnet_link_get_by_netif(netif);

    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "LINK Service", fapp_is_enabled_str[fnet_link_is_enabled(link_desc)]);
}

#endif /* FNET_CFG_LINK */

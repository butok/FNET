/**************************************************************************
*
* Copyright 2014-2017 by Andrey Butok. FNET Community.
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
*  FNET Shell Demo (LLMNR).
*
***************************************************************************/

#include "fapp.h"

#if FAPP_CFG_LLMNR_CMD && FNET_CFG_LLMNR

#include "fapp_prv.h"
#include "fapp_llmnr.h"
#include "fapp_netif.h"

/************************************************************************
* DESCRIPTION: Releases LLMNR server.
*************************************************************************/
void fapp_llmnr_release(void)
{
    fnet_netif_desc_t   netif;
    fnet_index_t        i;
    fnet_llmnr_desc_t   llmnr;

    /* Release all LLMNR servers.*/
    for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
    {
        llmnr = fnet_llmnr_get_by_netif(netif);
        if(llmnr)
        {
            fnet_llmnr_release(llmnr);
        }
    }
}

/************************************************************************
* DESCRIPTION: Run LLMNR server.
*************************************************************************/
void fapp_llmnr_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t           netif = fnet_netif_get_default(); /* By default is "default" netif*/
    fnet_bool_t                 init = FNET_TRUE; /* By default is "init".*/
    fnet_index_t                i;
    fnet_llmnr_params_t         params;

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
        /* Init parameters.*/
        fnet_memset_zero(&params, sizeof(params));
        params.netif_desc = netif;
        params.host_name = fapp_params_host_name;

        /* Start LLMNR server */
        if(fnet_llmnr_init(&params))
        {
            fnet_char_t netif_name[FNET_NETIF_NAMELEN];

            fnet_netif_get_name(netif, netif_name, sizeof(netif_name));
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " LLMNR server started.");
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Interface", netif_name );
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Host Name", params.host_name );
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "LLMNR");
        }
    }
    else /* [release] */
    {
        fnet_llmnr_release(fnet_llmnr_get_by_netif(netif));
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

/************************************************************************
* DESCRIPTION: Print service state.
*************************************************************************/
void fapp_llmnr_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "LLMNR Server", fapp_is_enabled_str[fnet_llmnr_is_enabled(fnet_llmnr_get_by_netif(netif))]);
}

#endif /* FAPP_CFG_LLMNR_CMD && FNET_CFG_LLMNR */

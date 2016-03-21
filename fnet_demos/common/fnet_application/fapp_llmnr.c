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
* @file fapp_llmnr.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo (HTTP Server Shell interface).
*
***************************************************************************/

#include "fapp.h"

#if FAPP_CFG_LLMNR_CMD && FNET_CFG_LLMNR

#include "fapp_prv.h"
#include "fapp_llmnr.h"

static fnet_llmnr_desc_t fapp_llmnr_desc = 0; /* LLMNR service descriptor. */

/************************************************************************
* NAME: fapp_llmnr_release
*
* DESCRIPTION: Releases HTTP server.
*************************************************************************/
void fapp_llmnr_release(void)
{
    fnet_llmnr_release(fapp_llmnr_desc);
    fapp_llmnr_desc = 0;
}

/************************************************************************
* NAME: fapp_llmnr_cmd
*
* DESCRIPTION: Run LLMNR server.
*************************************************************************/
void fapp_llmnr_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_llmnr_params    params;
    fnet_llmnr_desc_t           llmnr_desc;

    if(argc == 1u) /* By default is "init".*/
    {
        /* Init parameters.*/
        fnet_memset_zero(&params, sizeof(params));
        params.netif_desc = fnet_netif_get_default();
        params.host_name = fapp_params_host_name;

        /* Start LLMNR server */
        llmnr_desc = fnet_llmnr_init(&params);
        if(llmnr_desc != FNET_ERR)
        {
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " LLMNR server started.");
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Host Name", params.host_name );
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fapp_llmnr_desc = llmnr_desc;
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "LLMNR");
        }
    }
    else if((argc == 2u) && (fnet_strcasecmp(&FAPP_COMMAND_RELEASE[0], argv[1]) == 0)) /* [release] */
    {
        fapp_llmnr_release();
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}

/************************************************************************
* NAME: fapp_llmnr_info
*
* DESCRIPTION:
*************************************************************************/
void fapp_llmnr_info(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "LLMNR Server", fapp_enabled_str[fnet_llmnr_enabled(fapp_llmnr_desc)]);
}


#endif /* FAPP_CFG_HTTP_CMD */

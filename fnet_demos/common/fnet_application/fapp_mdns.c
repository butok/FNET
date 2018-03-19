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
*  FNET Shell Demo (mDNS).
*
***************************************************************************/

#include "fapp.h"

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS

#include "fapp_prv.h"
#include "fapp_mdns.h"

#if (FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP
#include "fapp_http.h"
static const fnet_mdns_txt_key_t *fapp_mdns_service_get_txt(void);
static const fnet_mdns_service_t fapp_mdns_http_service = {.service_type =  "_http._tcp", .service_port = FNET_CFG_HTTP_PORT, .service_get_txt = fapp_mdns_service_get_txt};        /* HTTP service parameters.*/
#if FNET_CFG_HTTP_TLS
static const fnet_mdns_service_t fapp_mdns_http_tls_service = {.service_type =  "_https._tcp", .service_port = FNET_CFG_HTTP_TLS_PORT, .service_get_txt = fapp_mdns_service_get_txt};   /* HTTPS service parameters.*/
#endif

const fnet_mdns_txt_key_t fapp_mdns_txt_key_http_table[] =
{
    {.key_name = "path", .key_value = "/index.html"},
    {0, 0} /* End of the table.*/
};
#endif

/************************************************************************
* DESCRIPTION: Releases mDNS server.
*************************************************************************/
void fapp_mdns_release(void)
{
    fnet_netif_desc_t   netif;
    fnet_index_t        i;
    fnet_mdns_desc_t    mdns;

    /* Release all MDNS servers.*/
    for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
    {
        mdns = fnet_mdns_get_by_netif(netif);
        if(mdns)
        {
            fnet_mdns_release(mdns);
        }
    }
}

/************************************************************************
* DESCRIPTION: Run mDNS server.
*************************************************************************/
void fapp_mdns_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t           netif = fnet_netif_get_default(); /* By default is "default" netif*/
    fnet_bool_t                 init = FNET_TRUE; /* By default is "init".*/
    fnet_index_t                i;
    struct fnet_mdns_params     params;

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
        params.name = fapp_params_host_name;

        /* Start mDNS server */
        if(fnet_mdns_init(&params))
        {
            fnet_char_t netif_name[FNET_NETIF_NAMELEN];

            fnet_netif_get_name(netif, netif_name, sizeof(netif_name));
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " mDNS server started.");
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Interface", netif_name );
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Host Name", params.name );
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

#if FAPP_CFG_HTTP_CMD && FNET_CFG_HTTP
            /* Register HTTP server to mDNS-SD, if availble.*/
            fapp_mdns_service_register_http();
#endif
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "MDNS");
        }
    }
    else /* [release] */
    {
        fnet_mdns_release(fnet_mdns_get_by_netif(netif));
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

#if FAPP_CFG_HTTP_CMD && FNET_CFG_HTTP
static const fnet_mdns_txt_key_t *fapp_mdns_service_get_txt(void)
{
    return fapp_mdns_txt_key_http_table;
}
#endif

/************************************************************************
* DESCRIPTION: Register HTTP service in mDNS.
*************************************************************************/
void fapp_mdns_service_register_http( void )
{
#if FAPP_CFG_HTTP_CMD && FNET_CFG_HTTP
    fnet_netif_desc_t   netif;
    fnet_index_t        i;
    fnet_mdns_desc_t    mdns;

    if(fapp_http_desc)
    {
        for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
        {
            mdns = fnet_mdns_get_by_netif(netif);
            if(mdns)
            {
                fnet_mdns_service_register(mdns, &fapp_mdns_http_service);
            }
        }
    }
#endif
}

/************************************************************************
* DESCRIPTION: Register HTTPS service in MDNS.
*************************************************************************/
void fapp_mdns_service_register_http_tls( void )
{
#if FAPP_CFG_HTTP_TLS_CMD && FNET_CFG_HTTP && FNET_CFG_HTTP_TLS
    fnet_netif_desc_t   netif;
    fnet_index_t        i;
    fnet_mdns_desc_t    mdns;

    if(fapp_http_tls_desc)
    {
        for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
        {
            mdns = fnet_mdns_get_by_netif(netif);
            if(mdns)
            {
                fnet_mdns_service_register(mdns, &fapp_mdns_http_tls_service);
            }
        }
    }
#endif
}

/************************************************************************
* DESCRIPTION: Unregister HTTP service from mDNS.
*************************************************************************/
void fapp_mdns_service_unregister_http( void )
{
#if FAPP_CFG_HTTP_CMD && FNET_CFG_HTTP
    fnet_netif_desc_t   netif;
    fnet_index_t        i;
    fnet_mdns_desc_t    mdns;

    for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
    {
        mdns = fnet_mdns_get_by_netif(netif);
        if(mdns)
        {
            fnet_mdns_service_desc_t service_desc;

            service_desc = fnet_mdns_service_get_by_type(mdns, fapp_mdns_http_service.service_type);
            /* If HTTP service is registered.*/
            if(service_desc)
            {
                fnet_mdns_service_unregister(service_desc);
            }
        }
    }
#endif
}

/************************************************************************
* DESCRIPTION: Unregister HTTPS service from mDNS.
*************************************************************************/
void fapp_mdns_service_unregister_http_tls( void )
{
#if FAPP_CFG_HTTP_TLS_CMD && FNET_CFG_HTTP && FNET_CFG_HTTP_TLS
    fnet_netif_desc_t   netif;
    fnet_index_t        i;
    fnet_mdns_desc_t    mdns;

    for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
    {
        mdns = fnet_mdns_get_by_netif(netif);
        if(mdns)
        {
            fnet_mdns_service_desc_t service_desc;

            service_desc = fnet_mdns_service_get_by_type(mdns, fapp_mdns_http_tls_service.service_type);
            /* If HTTP service is registered.*/
            if(service_desc)
            {
                fnet_mdns_service_unregister(service_desc);
            }
        }
    }
#endif
}

/************************************************************************
* DESCRIPTION:   Manual Name Change test.
*                It is used only for Bonjour Conformance Testing.
*************************************************************************/
void fapp_mdns_change_name_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t           netif = fnet_netif_get_default(); /* By default is "default" netif*/
    fnet_index_t                i;

    /* [-n <if name>] */
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
        else/* Wrong parameter.*/
        {
            goto ERROR_PARAMETER;
        }
    }

    if(fnet_mdns_get_by_netif(netif))
    {
        fnet_char_t netif_name[FNET_NETIF_NAMELEN];
        fnet_char_t script[FAPP_CFG_SHELL_MAX_LINE_LENGTH];

        fnet_netif_get_name(netif, netif_name, sizeof(netif_name));

        /* Change service name to "New - Bonjour Service Name"
         * It is required by Manual Name Change test, for Bonjour Conformance Testing.*/
        fnet_snprintf(script, sizeof(script), "mdns -n %s release; set host 'New - Bonjour Service Name'; mdns -n %s", netif_name, netif_name);
        if(fnet_shell_script(desc, script) == FNET_ERR)
        {
            fnet_shell_println(desc, "Script error!");
        }
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

/************************************************************************
* DESCRIPTION: Print service state.
*************************************************************************/
void fapp_mdns_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "MDNS Server", fapp_is_enabled_str[fnet_mdns_is_enabled(fnet_mdns_get_by_netif(netif))]);
}

#endif /* FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS */

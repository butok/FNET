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
**********************************************************************/
/*!
* @brief FNET Shell Demo (mDNS).
*
***************************************************************************/

#include "fapp.h"

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS

#include "fapp_prv.h"
#include "fapp_mdns.h"

#if (FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP
#include "fapp_http.h"
static const fnet_mdns_txt_key_t *fapp_mdns_service_get_txt(void);
static  fnet_mdns_service_desc_t fapp_mdns_http_service_desc = 0; /* HTTP service descriptor. */
static const fnet_mdns_service_t fapp_mdns_http_service = {.service_type =  "_http._tcp", .service_port = FNET_CFG_HTTP_PORT, .service_get_txt = fapp_mdns_service_get_txt};        /* HTTP service parameters.*/
#if FNET_CFG_HTTP_TLS
static  fnet_mdns_service_desc_t fapp_mdns_http_tls_service_desc = 0; /* HTTPS service descriptor. */
static const fnet_mdns_service_t fapp_mdns_http_tls_service = {.service_type =  "_https._tcp", .service_port = FNET_CFG_HTTP_TLS_PORT, .service_get_txt = fapp_mdns_service_get_txt};   /* HTTPS service parameters.*/
#endif

const fnet_mdns_txt_key_t fapp_mdns_txt_key_http_table[] =
{
    {.key_name = "path", .key_value = "/index.html"},
    {0, 0} /* End of the table.*/
};
#endif

static fnet_mdns_desc_t fapp_mdns_desc = 0; /* MDNS service descriptor. */

/************************************************************************
* DESCRIPTION: Releases mDNS server.
*************************************************************************/
void fapp_mdns_release(void)
{
    fnet_mdns_release(fapp_mdns_desc);
    fapp_mdns_desc = 0;
#if FAPP_CFG_HTTP_CMD && FNET_CFG_HTTP
    fapp_mdns_http_service_desc = 0;
#endif
#if FAPP_CFG_HTTP_TLS_CMD && FNET_CFG_HTTP && FNET_CFG_HTTP_TLS
    fapp_mdns_http_tls_service_desc = 0;
#endif
}

/************************************************************************
* DESCRIPTION: Run mDNS server.
*************************************************************************/
void fapp_mdns_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_mdns_params     params;
    fnet_mdns_desc_t            mdns_desc;

    if(argc == 1u) /* By default is "init".*/
    {
        /* Init parameters.*/
        fnet_memset_zero(&params, sizeof(params));
        params.netif_desc = fnet_netif_get_default();
        params.name = fapp_params_host_name;

        /* Start mDNS server */
        mdns_desc = fnet_mdns_init(&params);
        if(mdns_desc)
        {
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " mDNS server started.");
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Name", params.name );
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fapp_mdns_desc = mdns_desc;

#if FAPP_CFG_HTTP_CMD && FNET_CFG_HTTP
            /* Register HTTP server to mDNS SD, if availble.*/
            fapp_mdns_service_register_http();
#endif
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "mDNS");
        }
    }
    else if((argc == 2u) && (fnet_strcasecmp(&FAPP_COMMAND_RELEASE[0], argv[1]) == 0)) /* [release] */
    {
        fapp_mdns_release();
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
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
    if(fapp_mdns_desc)
    {
        if(fapp_http_desc) /* If HTTP server is started.*/
        {
            if(fapp_mdns_http_service_desc == 0) /* If HTTP service is not registered yet.*/
            {
                fapp_mdns_http_service_desc = fnet_mdns_service_register(fapp_mdns_desc, &fapp_mdns_http_service);
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
    if(fapp_mdns_desc)
    {
        if(fapp_http_tls_desc) /* If HTTPS server is started.*/
        {
            if(fapp_mdns_http_tls_service_desc == 0) /* If HTTP service is not registered yet.*/
            {
                fapp_mdns_http_tls_service_desc = fnet_mdns_service_register(fapp_mdns_desc, &fapp_mdns_http_tls_service);
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
    if(fapp_mdns_desc)
    {
        if(fapp_mdns_http_service_desc) /* If HTTP service is registered.*/
        {
            fnet_mdns_service_unregister(fapp_mdns_http_service_desc);
            fapp_mdns_http_service_desc = 0;
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
    if(fapp_mdns_desc)
    {
        if(fapp_mdns_http_tls_service_desc) /* If HTTP service is registered.*/
        {
            fnet_mdns_service_unregister(fapp_mdns_http_tls_service_desc);
            fapp_mdns_http_tls_service_desc = 0;
        }
    }
#endif
}

/************************************************************************
* DESCRIPTION:   Manual Name Change test.
*                It used only for Bonjour Conformance Testing.
*************************************************************************/
void fapp_mdns_change_name_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    if(fapp_mdns_desc) /* If mdns is initialized */
    {
        /* Change service name to "New - Bonjour Service Name"
         * It is required by Manual Name Change test, for Bonjour Conformance Testing.*/
        fnet_shell_script(desc, "mdns release; set host 'New - Bonjour Service Name'; mdns");
    }
}

/************************************************************************
* DESCRIPTION: Print service state.
*************************************************************************/
void fapp_mdns_info(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "MDNS Server", fapp_is_enabled_str[fnet_mdns_is_enabled(fapp_mdns_desc)]);
}

#endif /* FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS */

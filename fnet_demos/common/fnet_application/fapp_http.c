/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community.
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
*  FNET Shell Demo (HTTP Server Shell interface).
*
***************************************************************************/

#include "fapp.h"

#if (FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP_SRV

#include "fapp_prv.h"
#include "fapp_http.h"
#include "fapp_fs.h"
#include "fapp_mdns.h"
#include "fapp_netif.h"

#if FAPP_CFG_HTTP_TLS_CMD && FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS
    #include "mbedtls/certs.h"
    fnet_http_srv_desc_t fapp_http_srv_tls_desc = 0; /* HTTPS server descriptor. */
#endif

#if FAPP_CFG_HTTP_CMD
    fnet_http_srv_desc_t fapp_http_srv_desc = 0;    /* HTTP server descriptor. */
#endif

static fnet_size_t fapp_http_string_buffer_respond(fnet_uint8_t *buffer, fnet_size_t buffer_size, fnet_bool_t *eof, fnet_uint32_t *cookie);

/************************************************************************
* SSI definitions
*************************************************************************/
#if FNET_CFG_HTTP_SRV_SSI

#define FAPP_HTTP_SSI_COMMAND_ECHO  "echo"
#define FAPP_HTTP_SSI_BUFFER_MAX    sizeof("00:00:00:00:00:00")

struct fapp_http_echo_variable
{
    const fnet_char_t *variable;
    const fnet_char_t *value;
};

static const struct fapp_http_echo_variable fapp_http_echo_variables[] =
{
    { .variable = "NAME", .value = FNET_DESCRIPTION},
    { .variable = "CPU", .value = FNET_CPU_STR},
    { .variable = "VERSION", .value = FNET_VERSION},
    { .variable = "DATE", .value = FNET_BUILD_DATE},
    { .variable = "COPYRIGHT", .value = FNET_COPYRIGHT},
    { .variable = "LICENSE", .value = FNET_LICENSE},
    { .variable = 0} /* End of the table.*/
};

static fnet_uint8_t fapp_http_ssi_buffer[FAPP_HTTP_SSI_BUFFER_MAX];    /* Temporary buffer for run-time SSIs. */

static fnet_return_t fapp_http_ssi_echo_handle(fnet_char_t *query, fnet_uint32_t *cookie);

/* SSI table */
static const fnet_http_srv_ssi_t fapp_ssi_table[] =
{
    { .name = FAPP_HTTP_SSI_COMMAND_ECHO, .handle = fapp_http_ssi_echo_handle, .send = fapp_http_string_buffer_respond},
    { .name = 0 } /* End of the table. */
};

#endif /* FNET_CFG_HTTP_SRV_SSI */

/************************************************************************
* CGI definitions
*************************************************************************/
#if FNET_CFG_HTTP_SRV_CGI

#define CGI_MAX        sizeof("({ \"time\":\"00:00:00\",\"tx\":0000000000,\"rx\":0000000000})")

static fnet_return_t fapp_http_cgi_stdata_handle(fnet_http_srv_session_t session, fnet_char_t *query, fnet_uint32_t *cookie);
static fnet_return_t fapp_http_cgi_graph_handle(fnet_http_srv_session_t session, fnet_char_t *query, fnet_uint32_t *cookie);
#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
    static fnet_return_t fapp_http_cgi_post_handle(fnet_http_srv_session_t session, fnet_char_t *query, fnet_uint32_t *cookie);
#endif

static fnet_uint32_t fapp_http_cgi_rand(void);

/* CGI table */
static const fnet_http_srv_cgi_t fapp_cgi_table[] =
{
    { .name = "stdata.cgi", .handle = fapp_http_cgi_stdata_handle, .send = fapp_http_string_buffer_respond},
    { .name = "graph.cgi", .handle = fapp_http_cgi_graph_handle, .send = fapp_http_string_buffer_respond},
#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
    { .name = "post.cgi", .handle = fapp_http_cgi_post_handle, .send = fapp_http_string_buffer_respond},
#endif
    { .name = 0} /* End of the table. */
};
static fnet_uint8_t fapp_http_cgi_buffer[CGI_MAX]; /* CGI Temporary buffer. */

#endif /* FNET_CFG_HTTP_SRV_CGI */

/************************************************************************
* Authentification definitions
*************************************************************************/
#if FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC && FNET_CFG_HTTP_SRV_VERSION_MAJOR
static const fnet_http_srv_auth_t fapp_auth_table[] =
{
    {.realm = "Please use User Name:fnet Password:fnet", .dir_name = "auth", .userid = "fnet", .password = "fnet", .scheme = FNET_HTTP_SRV_AUTH_SCHEME_BASIC},
    {.scheme = FNET_HTTP_SRV_AUTH_SCHEME_NONE}  /* End of the table. */
};
#endif

/************************************************************************
* POST definitions.
*************************************************************************/
#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR

static fnet_return_t fapp_http_post_receive (fnet_http_srv_session_t session, fnet_uint8_t *buffer, fnet_size_t buffer_size, fnet_uint32_t *cookie);

static const fnet_http_srv_post_t fapp_post_table[] =
{
    {.name = "post.cgi", .post_handle = 0, .post_receive = fapp_http_post_receive, .post_send = 0},
    {.name = 0} /* End of the table. */
};

#define FAPP_HTTP_POST_BUFFER_SIZE    (50u)
static fnet_uint8_t fapp_http_post_buffer[FAPP_HTTP_POST_BUFFER_SIZE + 1u/* For zero-termination.*/];

#endif

/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_size_t fapp_http_string_buffer_respond(fnet_uint8_t *buffer, fnet_size_t buffer_size, fnet_bool_t *eof, fnet_uint32_t *cookie)
{
    fnet_size_t     result = 0U;
    fnet_char_t     *string_buffer_ptr = (fnet_char_t *) *cookie;
    fnet_size_t     send_size = fnet_strlen(string_buffer_ptr);

    *eof = FNET_TRUE; /* No aditional send by default. */

    if(buffer && buffer_size)
    {
        if(send_size > buffer_size)
        {
            send_size = buffer_size;
        }

        fnet_memcpy(buffer, string_buffer_ptr, send_size);

        string_buffer_ptr += send_size;
        if(*string_buffer_ptr != '\0') /* If any data for sending.*/
        {
            *eof = FNET_FALSE;    /* Need more itteration. */
        }

        result = send_size;

        *cookie = (fnet_uint32_t)string_buffer_ptr; /* Save cgi_buffer_ptr as cookie.*/
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
#if FNET_CFG_HTTP_SRV_SSI
static fnet_return_t fapp_http_ssi_echo_handle(fnet_char_t *query, fnet_uint32_t *cookie)
{
    fnet_return_t                           result = FNET_OK;
    const struct fapp_http_echo_variable    *echo_var_ptr;
    fnet_netif_desc_t                       netif = fnet_netif_get_default();
    const fnet_uint8_t                      *ssi_buffer_ptr = 0;

    /* Find static echo value. */
    for(echo_var_ptr = fapp_http_echo_variables; (echo_var_ptr->variable != 0) && (echo_var_ptr->value != 0); echo_var_ptr++)
    {
        if (!fnet_strcmp( query, echo_var_ptr->variable))
        {
            ssi_buffer_ptr = (const fnet_uint8_t *)echo_var_ptr->value;
            break;
        }
    }

    /* Find run-time echo values. */
    if(ssi_buffer_ptr == 0)
    {
#if FNET_CFG_IP4
        fnet_char_t ip_str[FNET_IP4_ADDR_STR_SIZE];
#endif

        ssi_buffer_ptr = fapp_http_ssi_buffer;
        if (!fnet_strcmp( query, "IP_ADDRESS"))
        {
#if FNET_CFG_IP4
            fnet_ip4_addr_t ip_adr = fnet_netif_get_ip4_addr(netif);
            fnet_inet_ntoa(*(struct fnet_in_addr *)( &ip_adr), ip_str);
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "%s", ip_str);
#else
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "...");
#endif /* FNET_CFG_IP4 */
        }
        else if (!fnet_strcmp( query, "SUBNET_MASK"))
        {
#if FNET_CFG_IP4
            fnet_ip4_addr_t ip_adr = fnet_netif_get_ip4_subnet_mask(netif);
            fnet_inet_ntoa(*(struct fnet_in_addr *)( &ip_adr), ip_str);
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "%s", ip_str);
#else
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "...");
#endif /* FNET_CFG_IP4 */
        }
        else if (!fnet_strcmp( query, "GATEWAY"))
        {
#if FNET_CFG_IP4
            fnet_ip4_addr_t ip_adr = fnet_netif_get_ip4_gateway(netif);
            fnet_inet_ntoa(*(struct fnet_in_addr *)( &ip_adr), ip_str);
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "%s", ip_str);
#else
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "...");
#endif /* FNET_CFG_IP4 */
        }
        else if (!fnet_strcmp( query, "MAC"))
        {
            fnet_mac_addr_t macaddr;
            fnet_char_t    mac_str[FNET_MAC_ADDR_STR_SIZE];

            fnet_netif_get_hw_addr(netif, macaddr, sizeof(fnet_mac_addr_t));
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "%s", fnet_mac_to_str(macaddr, mac_str));
        }
        else
        {
            result = FNET_ERR;
        }
    }

    *cookie = (fnet_uint32_t)ssi_buffer_ptr; /* Save ssi_buffer_ptr as cookie.*/

    return result;
}
#endif /*FNET_CFG_HTTP_SRV_SSI*/

#if FNET_CFG_HTTP_SRV_CGI
/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_return_t fapp_http_cgi_stdata_handle(fnet_http_srv_session_t session, fnet_char_t *query, fnet_uint32_t *cookie)
{
    fnet_time_t                     cur_time_ms;
    fnet_time_t                     t_hour;
    fnet_time_t                     t_min;
    fnet_time_t                     t_sec;
    struct fnet_netif_statistics    statistics;

    FNET_COMP_UNUSED_ARG(query);
    FNET_COMP_UNUSED_ARG(session);

    /* Get Time. */
    cur_time_ms = fnet_timer_get_ms();
    t_hour = cur_time_ms / FNET_TIMER_MS_IN_HOUR;
    t_min  = (cur_time_ms % FNET_TIMER_MS_IN_HOUR) / FNET_TIMER_MS_IN_MIN;
    t_sec  = (cur_time_ms % FNET_TIMER_MS_IN_MIN) / FNET_TIMER_MS_IN_SEC;

    /* Get statistics. */
    fnet_memset_zero( &statistics, sizeof(struct fnet_netif_statistics) );
    fnet_netif_get_statistics(fnet_netif_get_default(), &statistics);

    /* Write to the temprorary buffer. */
    fnet_snprintf((fnet_char_t *)fapp_http_cgi_buffer, sizeof(fapp_http_cgi_buffer), "({\"time\":\"%02d:%02d:%02d\",\"tx\":%d,\"rx\":%d})",
                  t_hour, t_min, t_sec, statistics.tx_packet, statistics.rx_packet);

    *cookie = (fnet_uint32_t)fapp_http_cgi_buffer; /* Save fapp_http_cgi_buffer as cookie.*/

    return FNET_OK;
}

#define FAPP_HTTP_CGI_GRAPH_MIN     (30u)
/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_uint32_t fapp_http_cgi_rand(void)
{
    return fnet_rand() % 280u + FAPP_HTTP_CGI_GRAPH_MIN;
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_return_t fapp_http_cgi_graph_handle(fnet_http_srv_session_t session, fnet_char_t *query, fnet_uint32_t *cookie)
{
    fnet_uint32_t q1 = fapp_http_cgi_rand();
    fnet_uint32_t q2 = fapp_http_cgi_rand();
    fnet_uint32_t q3 = fapp_http_cgi_rand();
    fnet_uint32_t q4 = fapp_http_cgi_rand();
    fnet_uint32_t q5 = fapp_http_cgi_rand();

    FNET_COMP_UNUSED_ARG(query);
    FNET_COMP_UNUSED_ARG(session);

    /* Wrie to the temprorary buffer. */
    fnet_snprintf((fnet_char_t *)fapp_http_cgi_buffer, sizeof(fapp_http_cgi_buffer),
                  "({\"q1\":%d,\"q2\":%d,\"q3\":%d,\"q4\":%d,\"q5\":%d})",
                  q1, q2, q3, q4, q5);

    *cookie = (fnet_uint32_t)fapp_http_cgi_buffer; /* Save fapp_http_cgi_buffer as cookie.*/

    return FNET_OK;
}

#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_return_t fapp_http_cgi_post_handle(fnet_http_srv_session_t session, fnet_char_t *query, fnet_uint32_t *cookie)
{
    FNET_COMP_UNUSED_ARG(query);
    FNET_COMP_UNUSED_ARG(session);

    *cookie = (fnet_uint32_t)fapp_http_post_buffer; /* Save fapp_http_post_buffer as cookie.*/

    return FNET_OK;
}
#endif

#endif /*FNET_CFG_HTTP_SRV_CGI*/


#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_return_t fapp_http_post_receive (fnet_http_srv_session_t session, fnet_uint8_t *buffer, fnet_size_t buffer_size, fnet_uint32_t *cookie)
{
    fnet_size_t post_buffer_index = (fnet_size_t) * cookie;
    fnet_size_t post_buffer_free_size = FAPP_HTTP_POST_BUFFER_SIZE - post_buffer_index;
    fnet_size_t receive_size = buffer_size;

    FNET_COMP_UNUSED_ARG(session);

    if(post_buffer_free_size)
    {
        if(receive_size > post_buffer_free_size)
        {
            receive_size = post_buffer_free_size;
        }

        fnet_memcpy(&fapp_http_post_buffer[post_buffer_index], buffer, receive_size);
        post_buffer_index += receive_size;
        fapp_http_post_buffer[post_buffer_index] = '\0';

        *cookie = (fnet_uint32_t)post_buffer_index; /* Save buffer index as cookie.*/
    }

    return FNET_OK;
}

#endif /*FNET_CFG_HTTP_SRV_POST*/

#if FAPP_CFG_HTTP_CMD
/************************************************************************
* DESCRIPTION: Releases HTTP server.
*************************************************************************/
void fapp_http_srv_release(void)
{
    fnet_http_srv_release(fapp_http_srv_desc);
    fapp_http_srv_desc = 0;
}

/************************************************************************
* DESCRIPTION: Run HTTP server.
*************************************************************************/
void fapp_http_srv_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_http_srv_params_t params;
    fnet_http_srv_desc_t   http_desc;

    if(argc == 1u) /* By default is "init".*/
    {
        fnet_memset_zero(&params, sizeof(fnet_http_srv_params_t));

        params.root_path = FAPP_HTTP_SRV_MOUNT_NAME;    /* Root directory path */
        params.index_path = FAPP_HTTP_SRV_INDEX_FILE;   /* Index file path, relative to the root_path */
#if FNET_CFG_HTTP_SRV_SSI
        params.ssi_table = fapp_ssi_table;
#endif
#if FNET_CFG_HTTP_SRV_CGI
        params.cgi_table = fapp_cgi_table;
#endif
#if FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC && FNET_CFG_HTTP_SRV_VERSION_MAJOR
        params.auth_table = fapp_auth_table;
#endif
#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
        params.post_table = fapp_post_table;
#endif

        /* Enable HTTP server */
        http_desc = fnet_http_srv_init(&params);
        if(http_desc)
        {
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " HTTP server started.");
            fapp_netif_addr_print(desc, AF_SUPPORTED, fnet_netif_get_default(), FNET_FALSE);
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fapp_http_srv_desc = http_desc;

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS
            /* Register HTTP server in mDNS SD.*/
            fapp_mdns_service_register_http();
#endif

        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "HTTP Server");
        }
    }
    else if((argc == 2u) && (fnet_strcasecmp(&FAPP_COMMAND_RELEASE[0], argv[1]) == 0)) /* [release] */
    {
        fapp_http_srv_release();

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS
        /* Unregister HTTP server from mDNS SD.*/
        fapp_mdns_service_unregister_http();
#endif
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}
#endif /* FAPP_CFG_HTTP_CMD */

#if (FAPP_CFG_HTTP_TLS_CMD && FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
/************************************************************************
* DESCRIPTION: Releases HTTP over TLS (HTTPS) server.
*************************************************************************/
void fapp_http_srv_tls_release(void)
{
    fnet_http_srv_release(fapp_http_srv_tls_desc);
    fapp_http_srv_tls_desc = 0;
}

/************************************************************************
* DESCRIPTION: Run HTTP over TLS (HTTPS) server.
*************************************************************************/
void fapp_http_srv_tls_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_http_srv_params_t         params;
    fnet_http_srv_desc_t           http_tls_desc;
    fnet_http_srv_tls_params_t     tls_params;

    if(argc == 1u) /* By default is "init".*/
    {
        fnet_memset_zero(&params, sizeof(params));

        params.root_path = FAPP_HTTP_SRV_MOUNT_NAME;    /* Root directory path */
        params.index_path = FAPP_HTTP_SRV_INDEX_FILE;   /* Index file path, relative to the root_path */
#if FNET_CFG_HTTP_SRV_SSI
        params.ssi_table = fapp_ssi_table;
#endif
#if FNET_CFG_HTTP_SRV_CGI
        params.cgi_table = fapp_cgi_table;
#endif
#if FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC && FNET_CFG_HTTP_SRV_VERSION_MAJOR
        params.auth_table = fapp_auth_table;
#endif
#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
        params.post_table = fapp_post_table;
#endif
        fnet_memset_zero(&tls_params, sizeof(tls_params));

        /* Set TLS parameters. Use mbedTLS test certificate and key. */
        tls_params.certificate_buffer = (const fnet_uint8_t *)mbedtls_test_srv_crt; /* Certificate data. */
        tls_params.certificate_buffer_size = mbedtls_test_srv_crt_len;              /* Size of the certificate buffer. */
        tls_params.private_key_buffer = (const fnet_uint8_t *)mbedtls_test_srv_key; /* Private key. */
        tls_params.private_key_buffer_size =  mbedtls_test_srv_key_len;             /* Size of the private key buffer. */
        params.tls_params = &tls_params;

        /* Enable HTTP server */
        http_tls_desc = fnet_http_srv_init(&params);
        if(http_tls_desc)
        {
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " HTTPS server started.");
            fapp_netif_addr_print(desc, AF_SUPPORTED, fnet_netif_get_default(), FNET_FALSE);
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fapp_http_srv_tls_desc = http_tls_desc;

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS
            /* Register HTTPS server in mDNS SD.*/
            fapp_mdns_service_register_http_tls();
#endif

        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "HTTPS Server");
        }
    }
    else if((argc == 2u) && (fnet_strcasecmp(&FAPP_COMMAND_RELEASE[0], argv[1]) == 0)) /* [release] */
    {
        fapp_http_srv_tls_release();

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS
        /* Unregister HTTPS server from mDNS SD.*/
        fapp_mdns_service_unregister_http_tls();
#endif
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}
#endif /*(FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)*/

/************************************************************************
* DESCRIPTION: Print HTTP server status
*************************************************************************/
void fapp_http_srv_info(fnet_shell_desc_t desc)
{
#if FAPP_CFG_HTTP_CMD
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "HTTP Server", fapp_is_enabled_str[fnet_http_srv_is_enabled(fapp_http_srv_desc)]);
#endif
#if FAPP_CFG_HTTP_TLS_CMD
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "HTTPS Server", fapp_is_enabled_str[fnet_http_srv_is_enabled(fapp_http_srv_tls_desc)]);
#endif
}

#endif /* (FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP_SRV */


/************************************************************************
* ========================== HTTP Client ================================
************************************************************************/
#if FAPP_CFG_HTTPC_CMD && FNET_CFG_HTTP_CLN

static fnet_http_cln_desc_t fapp_http_cln_desc = 0;    /* HTTP server descriptor. */

/************************************************************************
* DESCRIPTION: Ctr+C termination handler.
************************************************************************/
static void fapp_httpc_on_ctrlc(fnet_shell_desc_t desc, void *cookie)
{
    /* Terminate HTTP client service. */
    fnet_dns_release(fapp_http_cln_desc);
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* DESCRIPTION: Start HTTP client GET request
************************************************************************/
void fapp_http_cln_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_http_cln_params     http_cln_params;
    fnet_netif_desc_t               netif = fnet_netif_get_default();
    fnet_char_t                     ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
    fnet_http_cln_desc_t            http_cln_desc;
    fnet_index_t                    i;
    fnet_bool_t                     is_server_address = FNET_FALSE;

    /* Set HTTP client parameters.*/
    fnet_memset_zero(&http_cln_params, sizeof(http_cln_params));


    /* "[-n <if name>] -s <server ip> [-u <uri>]"*/
    for(i = 1u; i < argc /*avoid the last parameter.*/; i++)
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
        else if (!fnet_strcmp(argv[i], "-s")) /*-s <server ip>*/
        {
            i++;
            if(i < argc)
            {
                if(fnet_inet_ptos(argv[i], &http_cln_params.address) == FNET_ERR)
                {
                    goto ERROR_PARAMETER;
                }
                is_server_address = FNET_TRUE;
            }
            else
            {
                goto ERROR_PARAMETER;
            }
        }
        else if (!fnet_strcmp(argv[i], "-u")) /*[-u <uri>] */
        {
            i++;
            if(i < argc)
            {
                http_cln_params.uri = argv[i];
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

    if(is_server_address == FNET_FALSE) /* No -s <server ip> parameter */
    {
        i = argc;
        goto ERROR_PARAMETER;
    }

    /* Run HTTP client request. */
    http_cln_desc = fnet_http_cln_init(&http_cln_params);
    if(http_cln_desc)
    {
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
        fnet_shell_println(desc, "HTTP client request");
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Server",
                           fnet_inet_ntop(http_cln_params.address.sa_family, http_cln_params.address.sa_data, ip_str, sizeof(ip_str)));
        fnet_shell_println(desc, FAPP_TOCANCEL_STR);
        fnet_shell_println(desc, FAPP_DELIMITER_STR);

        fapp_http_cln_desc = http_cln_desc;

        fnet_shell_block(desc, fapp_httpc_on_ctrlc, FNET_NULL); /* Block the shell input.*/
    }
    else
    {
        fnet_shell_println(desc, FAPP_INIT_ERR, "HTTP Client");
    }
    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

#endif /* FAPP_CFG_HTTPC_CMD && FNET_CFG_HTTP_CLN */

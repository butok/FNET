/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
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
* @file fapp_http.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo (HTTP Server Shell interface).
*
***************************************************************************/

#include "fnet.h"
#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_http.h"

#if FAPP_CFG_HTTP_CMD && FNET_CFG_HTTP

#include "fapp_fs.h"

static fnet_http_desc_t fapp_http_desc = 0; /* HTTP service descriptor. */

static fnet_size_t fapp_http_string_buffer_respond(fnet_uint8_t *buffer, fnet_size_t buffer_size, fnet_bool_t *eof, fnet_uint32_t *cookie);


/************************************************************************
* SSI definitions
*************************************************************************/
#if FNET_CFG_HTTP_SSI

#define FAPP_HTTP_SSI_COMMAND_ECHO  "echo"
#define FAPP_HTTP_SSI_BUFFER_MAX    sizeof("00:00:00:00:00:00")

struct fapp_http_echo_variable
{
    const fnet_char_t *variable;
    const fnet_char_t *value;
};

static const struct fapp_http_echo_variable fapp_http_echo_variables[] =
{
    {"NAME", FNET_DESCRIPTION},
    {"CPU", FNET_CPU_STR},
    {"VERSION", FNET_VERSION},
    {"DATE", FNET_BUILD_DATE},
    {"COPYRIGHT", FNET_COPYRIGHT},
    {"LICENSE", FNET_LICENSE},
    {0, 0} /* End of the table.*/
};

static fnet_uint8_t fapp_http_ssi_buffer[FAPP_HTTP_SSI_BUFFER_MAX];    /* Temporary buffer for run-time SSIs. */

static fnet_return_t fapp_http_ssi_echo_handle(fnet_char_t *query, fnet_uint32_t *cookie);

/* SSI table */
static const struct fnet_http_ssi fapp_ssi_table[] =
{
    {FAPP_HTTP_SSI_COMMAND_ECHO, fapp_http_ssi_echo_handle, fapp_http_string_buffer_respond},
    {0, 0, 0} /* End of the table. */
};

#endif /* FNET_CFG_HTTP_SSI */

/************************************************************************
* CGI definitions
*************************************************************************/
#if FNET_CFG_HTTP_CGI

#define CGI_MAX        sizeof("({ \"time\":\"00:00:00\",\"tx\":0000000000,\"rx\":0000000000})")

static fnet_return_t fapp_http_cgi_stdata_handle(fnet_http_session_t session, fnet_char_t *query, fnet_uint32_t *cookie);
static fnet_return_t fapp_http_cgi_graph_handle(fnet_http_session_t session, fnet_char_t *query, fnet_uint32_t *cookie);
#if FNET_CFG_HTTP_POST && FNET_CFG_HTTP_VERSION_MAJOR
    static fnet_return_t fapp_http_cgi_post_handle(fnet_http_session_t session, fnet_char_t *query, fnet_uint32_t *cookie);
#endif

static fnet_uint32_t fapp_http_cgi_rand(void);

/* CGI table */
static const struct fnet_http_cgi fapp_cgi_table[] =
{
    {"stdata.cgi", fapp_http_cgi_stdata_handle, fapp_http_string_buffer_respond},
    {"graph.cgi", fapp_http_cgi_graph_handle, fapp_http_string_buffer_respond},
#if FNET_CFG_HTTP_POST && FNET_CFG_HTTP_VERSION_MAJOR
    {"post.cgi", fapp_http_cgi_post_handle, fapp_http_string_buffer_respond},
#endif
    {0, 0, 0} /* End of the table. */
};
static fnet_uint8_t fapp_http_cgi_buffer[CGI_MAX]; /* CGI Temporary buffer. */

#endif /* FNET_CFG_HTTP_CGI */

/************************************************************************
* Authentification definitions
*************************************************************************/
#if FNET_CFG_HTTP_AUTHENTICATION_BASIC && FNET_CFG_HTTP_VERSION_MAJOR
static const struct fnet_http_auth fapp_auth_table[] =
{
    {"auth", "fnet", "freescale", FNET_HTTP_AUTH_SCHEME_BASIC},
    {0, 0, 0, FNET_HTTP_AUTH_SCHEME_NONE}
};
#endif

/************************************************************************
* POST definitions.
*************************************************************************/
#if FNET_CFG_HTTP_POST && FNET_CFG_HTTP_VERSION_MAJOR

static fnet_return_t fapp_http_post_receive (fnet_http_session_t session, fnet_uint8_t *buffer, fnet_size_t buffer_size, fnet_uint32_t *cookie);

static const struct fnet_http_post fapp_post_table[] =
{
    {"post.cgi", 0, fapp_http_post_receive, 0},
    {0, 0, 0, 0}
};

#define FAPP_HTTP_POST_BUFFER_SIZE    (50u)
static fnet_uint8_t fapp_http_post_buffer[FAPP_HTTP_POST_BUFFER_SIZE + 1u/* For zero-termination.*/];

#endif

/************************************************************************
* NAME: fapp_http_string_buffer_respond
*
* DESCRIPTION:
*************************************************************************/
static fnet_size_t fapp_http_string_buffer_respond(fnet_uint8_t *buffer, fnet_size_t buffer_size, fnet_bool_t *eof, fnet_uint32_t *cookie)
{
    fnet_size_t     result = 0U;
    fnet_char_t            *string_buffer_ptr = (fnet_char_t *) *cookie;
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
* NAME: fapp_http_ssi_echo_handle
*
* DESCRIPTION:
*************************************************************************/
#if FNET_CFG_HTTP_SSI
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
            fnet_inet_ntoa(*(struct in_addr *)( &ip_adr), ip_str);
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "%s", ip_str);
#else
            fnet_snprintf(fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "...");
#endif /* FNET_CFG_IP4 */
        }
        else if (!fnet_strcmp( query, "SUBNET_MASK"))
        {
#if FNET_CFG_IP4
            fnet_ip4_addr_t ip_adr = fnet_netif_get_ip4_subnet_mask(netif);
            fnet_inet_ntoa(*(struct in_addr *)( &ip_adr), ip_str);
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "%s", ip_str);
#else
            fnet_snprintf(fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "...");
#endif /* FNET_CFG_IP4 */
        }
        else if (!fnet_strcmp( query, "GATEWAY"))
        {
#if FNET_CFG_IP4
            fnet_ip4_addr_t ip_adr = fnet_netif_get_ip4_gateway(netif);
            fnet_inet_ntoa(*(struct in_addr *)( &ip_adr), ip_str);
            fnet_snprintf((fnet_char_t *)fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "%s", ip_str);
#else
            fnet_snprintf(fapp_http_ssi_buffer, sizeof(fapp_http_ssi_buffer), "...");
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
#endif /*FNET_CFG_HTTP_SSI*/



#if FNET_CFG_HTTP_CGI
/************************************************************************
* NAME: fapp_http_cgi_stdata_handle
*
* DESCRIPTION:
*************************************************************************/
static fnet_return_t fapp_http_cgi_stdata_handle(fnet_http_session_t session, fnet_char_t *query, fnet_uint32_t *cookie)
{
    fnet_time_t                     cur_time;
    fnet_time_t                     t_hour;
    fnet_time_t                     t_min;
    fnet_time_t                   t_sec;
    struct fnet_netif_statistics    statistics;

    FNET_COMP_UNUSED_ARG(query);
    FNET_COMP_UNUSED_ARG(session);

    /* Get Time. */
    cur_time = fnet_timer_ticks();
    t_hour = cur_time / FNET_TIMER_TICK_IN_HOUR;
    t_min  = (cur_time % FNET_TIMER_TICK_IN_HOUR) / FNET_TIMER_TICK_IN_MIN;
    t_sec  = (cur_time % FNET_TIMER_TICK_IN_MIN) / FNET_TIMER_TICK_IN_SEC;

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
* NAME: fapp_http_cgi_rand
*
* DESCRIPTION:
*************************************************************************/
static fnet_uint32_t fapp_http_cgi_rand(void)
{
    return fnet_rand() % 280u + FAPP_HTTP_CGI_GRAPH_MIN;
}

/************************************************************************
* NAME: fapp_http_cgi_graph_handle
*
* DESCRIPTION:
*************************************************************************/
static fnet_return_t fapp_http_cgi_graph_handle(fnet_http_session_t session, fnet_char_t *query, fnet_uint32_t *cookie)
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

#if FNET_CFG_HTTP_POST && FNET_CFG_HTTP_VERSION_MAJOR
/************************************************************************
* NAME: fapp_http_cgi_post_handle
*
* DESCRIPTION:
*************************************************************************/
static fnet_return_t fapp_http_cgi_post_handle(fnet_http_session_t session, fnet_char_t *query, fnet_uint32_t *cookie)
{
    FNET_COMP_UNUSED_ARG(query);
    FNET_COMP_UNUSED_ARG(session);

    *cookie = (fnet_uint32_t)fapp_http_post_buffer; /* Save fapp_http_post_buffer as cookie.*/

    return FNET_OK;
}
#endif

#endif /*FNET_CFG_HTTP_CGI*/


#if FNET_CFG_HTTP_POST && FNET_CFG_HTTP_VERSION_MAJOR
/************************************************************************
* NAME: fapp_http_post_receive
*
* DESCRIPTION:
*************************************************************************/
static fnet_return_t fapp_http_post_receive (fnet_http_session_t session, fnet_uint8_t *buffer, fnet_size_t buffer_size, fnet_uint32_t *cookie)
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

#endif /*FNET_CFG_HTTP_POST*/

/************************************************************************
* NAME: fapp_http_release
*
* DESCRIPTION: Releases HTTP server.
*************************************************************************/
void fapp_http_release(void)
{
    fnet_http_release(fapp_http_desc);
    fapp_http_desc = 0;
}

/************************************************************************
* NAME: fapp_http_cmd
*
* DESCRIPTION: Run HTTP server.
*************************************************************************/
void fapp_http_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_http_params params;
    fnet_http_desc_t        http_desc;

    if(argc == 1u) /* By default is "init".*/
    {
        fnet_memset_zero(&params, sizeof(struct fnet_http_params));

        params.root_path = FAPP_HTTP_MOUNT_NAME;    /* Root directory path */
        params.index_path = FAPP_HTTP_INDEX_FILE;   /* Index file path, relative to the root_path */
#if FNET_CFG_HTTP_SSI
        params.ssi_table = fapp_ssi_table;
#endif
#if FNET_CFG_HTTP_CGI
        params.cgi_table = fapp_cgi_table;
#endif
#if FNET_CFG_HTTP_AUTHENTICATION_BASIC && FNET_CFG_HTTP_VERSION_MAJOR
        params.auth_table = fapp_auth_table;
#endif
#if FNET_CFG_HTTP_POST && FNET_CFG_HTTP_VERSION_MAJOR
        params.post_table = fapp_post_table;
#endif

        /* Enable HTTP server */
        http_desc = fnet_http_init(&params);
        if(http_desc != FNET_ERR)
        {
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " HTTP server started.");
            fapp_print_netif_addr(desc, AF_SUPPORTED, fnet_netif_get_default(), FNET_FALSE);
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fapp_http_desc = http_desc;
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "HTTP");
        }
    }
    else if((argc == 2u) && (fnet_strcasecmp(&FAPP_COMMAND_RELEASE[0], argv[1]) == 0)) /* [release] */
    {
        fapp_http_release();
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}

/************************************************************************
* NAME: fapp_http_info
*
* DESCRIPTION:
*************************************************************************/
void fapp_http_info(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "HTTP Server", fapp_enabled_str[fnet_http_enabled(fapp_http_desc)]);
}


#endif /* FAPP_CFG_HTTP_CMD */

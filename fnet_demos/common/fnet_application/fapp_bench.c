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
*  FNET Shell Demo implementation.
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_bench.h"
#include "fapp_netif.h"

#if FAPP_CFG_BENCH_CMD

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if FNET_CFG_BENCH_SRV
    static fnet_bench_srv_desc_t fapp_bench_srv_desc = 0;    /* Benchmark server descriptor. */
#endif
#if FNET_CFG_BENCH_CLN
    static fnet_bench_cln_desc_t fapp_bench_cln_desc = 0;    /* Benchmark client descriptor. */
#endif

/************************************************************************
* DESCRIPTION: Print Benchmark results.
************************************************************************/
#if FNET_CFG_BENCH_SRV || FNET_CFG_BENCH_CLN
static void fapp_bench_print_results (fnet_shell_desc_t desc, fnet_size_t megabytes, fnet_size_t bytes, fnet_time_t time_ms)
{
    fnet_uint32_t megabits_sec = 0;
    fnet_uint32_t kbits_sec = 0;


    if(time_ms)
    {
        kbits_sec =  (((megabytes * 8 * 1000 /*sec*/) % time_ms) * 1000 /*kbits*/) / time_ms;
        kbits_sec = kbits_sec + (fnet_size_t)((bytes * 8) / time_ms);

        megabits_sec = (fnet_size_t)((megabytes * 8 * 1000) / time_ms) + (kbits_sec / 1000);
        kbits_sec = (kbits_sec % 1000);
    }

    /* Print benchmark results.*/
    fnet_shell_println(desc, "\r\n[BENCH]\t%u.%06u MBytes in %u.%02u Seconds = %u.%03u Mbit/sec\n", megabytes, bytes,
                       (time_ms / 1000), ((time_ms % 1000) / 10),
                       megabits_sec, kbits_sec);
}
#endif

#if FNET_CFG_BENCH_SRV
/************************************************************************
* DESCRIPTION: Benchmark server "session begin" event handler callback.
************************************************************************/
static void fapp_bench_srv_callback_session_begin(fnet_bench_srv_desc_t desc, struct fnet_sockaddr address, void *cookie)
{
    fnet_shell_desc_t   shell_desc = (fnet_shell_desc_t)cookie;
    fnet_char_t         ip_str[FNET_IP_ADDR_STR_SIZE];

    fnet_shell_println(shell_desc, "\r\n[BENCH] Receiving from %s port = %d", fnet_inet_ntop(address.sa_family, (fnet_uint8_t *)(address.sa_data), ip_str, sizeof(ip_str)), fnet_ntohs(address.sa_port));
}

/************************************************************************
* DESCRIPTION: Benchmark server "session end" event handler callback.
************************************************************************/
static void fapp_bench_srv_callback_session_end(fnet_bench_srv_desc_t desc, const struct fnet_bench_srv_result *bench_srv_result, void *cookie)
{
    fnet_shell_desc_t   shell_desc = (fnet_shell_desc_t)cookie;

    if(bench_srv_result)
    {
        fapp_bench_print_results (shell_desc, bench_srv_result->megabytes, bench_srv_result->bytes, bench_srv_result->time_ms);
    }
}

/************************************************************************
* DESCRIPTION: Start RX Benchmark server.
************************************************************************/
void fapp_bench_srv_cmd( fnet_shell_desc_t shell_desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_bench_srv_params    bench_srv_params;
    fnet_bool_t                     init = FNET_TRUE; /* By default is "init".*/
    fnet_index_t                    i;
    fnet_netif_desc_t               netif = 0;
    fnet_bench_srv_desc_t           bench_srv_desc;

    /* Set Benchmark server parameters.*/
    fnet_memset_zero(&bench_srv_params, sizeof(bench_srv_params));

    bench_srv_params.type = SOCK_STREAM; /* TCP by default */

    /* [-n <if name>] [-a <if ip address>] [tcp|udp] [release]*/
    for(i = 1u; i < (argc); i++)
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
        else if (!fnet_strcmp(argv[i], "-a")) /*[-a <if ip address>]*/
        {
            i++;
            if(i < argc)
            {
                if(fnet_inet_ptos(argv[i], &bench_srv_params.address) == FNET_ERR)
                {
                    goto ERROR_PARAMETER;
                }
            }
            else
            {
                goto ERROR_PARAMETER;
            }
        }
        else if (!fnet_strcmp(argv[i], "tcp")) /* TCP */
        {
            bench_srv_params.type = SOCK_STREAM;
        }
        else if (!fnet_strcmp(argv[i], "udp")) /* udp */
        {
            bench_srv_params.type = SOCK_DGRAM;
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
        if(netif) /* Only on one interface */
        {
            bench_srv_params.address.sa_scope_id = fnet_netif_get_scope_id(netif);
        }

        /* Start Benchmark server. */
        bench_srv_desc = fnet_bench_srv_init(&bench_srv_params);
        if(bench_srv_desc)
        {
            fapp_bench_srv_desc = bench_srv_desc;

            /* Instal callbacks */
            fnet_bench_srv_set_callback_session_begin (fapp_bench_srv_desc, fapp_bench_srv_callback_session_begin, shell_desc);
            fnet_bench_srv_set_callback_session_end (fapp_bench_srv_desc, fapp_bench_srv_callback_session_end, shell_desc);

            fnet_shell_println(shell_desc, FAPP_DELIMITER_STR);
            fnet_shell_println(shell_desc, " Benchmark server started.");

            if(fnet_socket_addr_is_unspecified(&bench_srv_params.address) == FNET_FALSE) /* Listen on concrete address */
            {
                fnet_char_t     ip_str[FNET_IP_ADDR_STR_SIZE_MAX];

                fnet_inet_ntop(bench_srv_params.address.sa_family, bench_srv_params.address.sa_data, ip_str, sizeof(ip_str));

                fnet_shell_println(shell_desc, FAPP_SHELL_INFO_FORMAT_S, "IP Address", ip_str);
            }
            else
            {
                fapp_netif_addr_print(shell_desc, AF_SUPPORTED, netif, FNET_FALSE);
            }
            fnet_shell_println(shell_desc, FAPP_SHELL_INFO_FORMAT_S, "Protocol", (bench_srv_params.type == SOCK_STREAM) ? "TCP" : "UDP");
            fnet_shell_println(shell_desc, FAPP_SHELL_INFO_FORMAT_D, "Local Port", FNET_NTOHS(FNET_CFG_BENCH_SRV_PORT));
            fnet_shell_println(shell_desc, FAPP_DELIMITER_STR);

        }
        else
        {
            fnet_shell_println(shell_desc, FAPP_INIT_ERR, "BENCH srv");
        }
    }
    else /* Release */
    {
        fapp_bench_srv_release();
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(shell_desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

/************************************************************************
* DESCRIPTION: Releases Benchmark server.
*************************************************************************/
void fapp_bench_srv_release(void)
{
    fnet_bench_srv_release(fapp_bench_srv_desc);
    fapp_bench_srv_desc = 0;
}

/************************************************************************
* DESCRIPTION: Benchmark server "info" command result
*************************************************************************/
void fapp_bench_srv_info(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "BENCH Server", fapp_is_enabled_str[fnet_bench_srv_is_enabled(fapp_bench_srv_desc)]);
}

#endif /* FNET_CFG_BENCH_SRV */


#if FNET_CFG_BENCH_CLN

/************************************************************************
* DESCRIPTION: Benchmark client "session end" event handler callback.
************************************************************************/
static void fapp_bench_cln_callback_session_end(fnet_bench_cln_desc_t bench_cln_desc, const struct fnet_bench_cln_result *bench_cln_result, void *cookie)
{
    fnet_shell_desc_t   shell_desc = (fnet_shell_desc_t)cookie;

    fnet_shell_unblock(shell_desc); /* Unblock the shell. */

    if(bench_cln_result)
    {
        fapp_bench_print_results (shell_desc, bench_cln_result->megabytes, bench_cln_result->bytes, bench_cln_result->time_ms);
    }
}

/************************************************************************
* DESCRIPTION: Ctr+C termination handler.
************************************************************************/
static void fapp_bench_cln_on_ctrlc(fnet_shell_desc_t shell_desc, void *cookie)
{
    /* Terminate Benchmark client service. */
    fnet_bench_cln_release(fapp_bench_cln_desc);
    fnet_shell_println(shell_desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* DESCRIPTION: Start Benchmark client/sender.
************************************************************************/
void fapp_bench_cln_cmd(fnet_shell_desc_t shell_desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_bench_cln_params    bench_cln_params;
    fnet_netif_desc_t               netif = 0;
    fnet_bench_cln_desc_t           bench_cln_desc;
    fnet_char_t                     ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
    fnet_index_t                    i;

    /* Set Benchmark client parameters.*/
    fnet_memset_zero(&bench_cln_params, sizeof(bench_cln_params));

    /* Default values */
    bench_cln_params.type = SOCK_STREAM; /* TCP by default */
    bench_cln_params.message_size = FAPP_BENCH_TX_MESSAGE_SIZE_DEFAULT;     /* Default message size */
    bench_cln_params.message_number = FAPP_BENCH_TX_MESSAGE_NUMBER_DEFAULT; /* Default number of messages */

    /* [-n <if name>] -a <remote ip> [tcp|udp] [-m <message size>] [-mn <number of messages>] */
    for(i = 1u; i < (argc) /*avoid the last parameter.*/; i++)
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
        else if (!fnet_strcmp(argv[i], "-a")) /*[-a <remote ip>] */
        {
            i++;
            if(i < argc)
            {
                if(fnet_inet_ptos(argv[i], &bench_cln_params.address) == FNET_ERR)
                {
                    goto ERROR_PARAMETER;
                }
            }
            else
            {
                goto ERROR_PARAMETER;
            }
        }
        else if (!fnet_strcmp(argv[i], "tcp")) /* TCP */
        {
            bench_cln_params.type = SOCK_STREAM;
        }
        else if (!fnet_strcmp(argv[i], "udp")) /* udp */
        {
            bench_cln_params.type = SOCK_DGRAM;
        }
        else if (!fnet_strcmp(argv[i], "-m")) /* [-m <message size>] */
        {
            fnet_char_t *p = 0;

            i++;
            if(i < argc)
            {
                bench_cln_params.message_size = fnet_strtoul(argv[i], &p, 0);
                if (bench_cln_params.message_size == 0)
                {
                    goto ERROR_PARAMETER;
                }
            }
            else
            {
                goto ERROR_PARAMETER;
            }
        }
        else if (!fnet_strcmp(argv[i], "-mn")) /* [-mn <number of messages>] */
        {
            fnet_char_t *p = 0;

            i++;
            if(i < argc)
            {
                bench_cln_params.message_number = fnet_strtoul(argv[i], &p, 0);
                if (bench_cln_params.message_number == 0)
                {
                    goto ERROR_PARAMETER;
                }
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

    if(fnet_socket_addr_is_unspecified(&bench_cln_params.address) == FNET_TRUE) /* Address is not set. */
    {
        fnet_shell_println(shell_desc, "Error: <remote ip> is not set");
    }
    else
    {
        if(netif) /* Only on one interface */
        {
            bench_cln_params.address.sa_scope_id = fnet_netif_get_scope_id(netif);
        }

        bench_cln_params.callback = fapp_bench_cln_callback_session_end;    /* Callback function.*/
        bench_cln_params.cookie = shell_desc;                               /* Application-specific parameter.*/

        /* Run Benchmark client. */
        bench_cln_desc = fnet_bench_cln_init(&bench_cln_params);
        if(bench_cln_desc)
        {
            fapp_bench_cln_desc = bench_cln_desc;

            fnet_shell_println(shell_desc, FAPP_DELIMITER_STR);
            fnet_shell_println(shell_desc, " Benchmark client started.");
            fnet_shell_println(shell_desc, FAPP_SHELL_INFO_FORMAT_S, "Protocol", (bench_cln_params.type == SOCK_STREAM) ? "TCP" : "UDP");
            fnet_shell_println(shell_desc, FAPP_SHELL_INFO_FORMAT_S, "Remote IP Addr", fnet_inet_ntop(bench_cln_params.address.sa_family, bench_cln_params.address.sa_data, ip_str, sizeof(ip_str)));
            fnet_shell_println(shell_desc, FAPP_SHELL_INFO_FORMAT_D, "Remote Port", FNET_NTOHS(FNET_CFG_BENCH_CLN_PORT));
            fnet_shell_println(shell_desc, FAPP_SHELL_INFO_FORMAT_D, "Message Size", bench_cln_params.message_size);
            fnet_shell_println(shell_desc, FAPP_SHELL_INFO_FORMAT_D, "Num. of messages", bench_cln_params.message_number);
            fnet_shell_println(shell_desc, FAPP_TOCANCEL_STR);
            fnet_shell_println(shell_desc, FAPP_DELIMITER_STR);

            fnet_shell_block(shell_desc, fapp_bench_cln_on_ctrlc, FNET_NULL); /* Block the shell input.*/
        }
        else
        {
            fnet_shell_println(shell_desc, FAPP_INIT_ERR, "BENCH cln");
        }
    }
    return;

ERROR_PARAMETER:
    fnet_shell_println(shell_desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

#endif /* FNET_CFG_BENCH_CLN */


#endif /* FAPP_CFG_BENCH_CMD */

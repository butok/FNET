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
* @file fapp_bench.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo implementation.
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_bench.h"

#if FAPP_CFG_BENCH_CMD


/************************************************************************
* Benchmark definitions
************************************************************************/
#define FAPP_BENCH_UDP_END_BUFFER_LENGTH    (1)
#define FAPP_UDP_TIMEOUT_MS                 (8000)

/* Socket Tx&Rx buffer sizes. */
#define FAPP_BENCH_SOCKET_BUF_SIZE          (FAPP_BENCH_PACKET_SIZE_MAX)
#define FAPP_BENCH_BUFFER_SIZE              (FAPP_BENCH_PACKET_SIZE_MAX)

#define FAPP_BENCH_TX_UDP_END_ITERATIONS    (10)


/* Keepalive probe retransmit limit.*/
#define FAPP_BENCH_TCP_KEEPCNT              (2)

/* Keepalive retransmit interval.*/
#define FAPP_BENCH_TCP_KEEPINTVL            (5) /*sec*/

/* Time between keepalive probes.*/
#define FAPP_BENCH_TCP_KEEPIDLE             (5) /*sec*/


#define FAPP_BENCH_COMPLETED_STR            "Test completed."

/************************************************************************
*    Benchmark server control structure.
*************************************************************************/
struct fapp_bench_t
{
    fnet_socket_t socket_listen;                   /* Listening socket.*/
    fnet_socket_t socket_foreign;                  /* Foreign socket.*/

    fnet_uint8_t buffer[FAPP_BENCH_BUFFER_SIZE];    /* Transmit circular buffer */

    fnet_time_t first_time;
    fnet_time_t last_time;
    fnet_size_t bytes;
    fnet_size_t remote_bytes;
};

static struct fapp_bench_t fapp_bench;

/**************************************************************************/ /*!
 * TX benchmark parameters
 ******************************************************************************/
struct fapp_bench_tx_params
{
    fnet_shell_desc_t desc;
    struct sockaddr foreign_addr;
    fnet_size_t packet_size;
    fnet_size_t packet_number;
    fnet_index_t iteration_number;
};

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_bench_print_results (fnet_shell_desc_t desc);
static void fapp_bench_tcp_rx (fnet_shell_desc_t desc, fnet_address_family_t family);
static void fapp_bench_udp_rx (fnet_shell_desc_t desc, fnet_address_family_t family, struct sockaddr *multicast_address /* optional, set to 0*/);
static void fapp_bench_tcp_tx (struct fapp_bench_tx_params *params);
static void fapp_bench_udp_tx (struct fapp_bench_tx_params *params);

/************************************************************************
* NAME: fapp_bench_print_results
*
* DESCRIPTION: Print Benchmark results.
************************************************************************/
static void fapp_bench_print_results (fnet_shell_desc_t desc)
{
    /* Print benchmark results.*/
    fnet_time_t interval = fnet_timer_get_interval(fapp_bench.first_time, fapp_bench.last_time);

    fnet_shell_println(desc, "Results:");

    if(fapp_bench.remote_bytes == 0)
    {
        fnet_shell_println(desc, "\t%u bytes in %u.%u seconds = %u Kbits/sec\n", fapp_bench.bytes,
                           ((interval * FNET_TIMER_PERIOD_MS) / 1000),
                           ((interval * FNET_TIMER_PERIOD_MS) % 1000) / 100,
                           (interval == 0) ? (fnet_size_t) - 1 : (fnet_size_t)((fapp_bench.bytes * 8/**(1000*/ / FNET_TIMER_PERIOD_MS/*)*/) / interval)/*/1000*/);
    }
    else /* UDP TX only */
    {
        fnet_shell_println(desc, "\t%u [%u] bytes in %u.%u seconds = %u [%u] Kbits/sec\n", fapp_bench.bytes, fapp_bench.remote_bytes,
                           ((interval * FNET_TIMER_PERIOD_MS) / 1000),
                           ((interval * FNET_TIMER_PERIOD_MS) % 1000) / 100,
                           (interval == 0) ? (fnet_size_t) - 1 : (fnet_size_t)((fapp_bench.bytes * 8/**(1000*/ / FNET_TIMER_PERIOD_MS/*)*/) / interval)/*/1000*/,
                           (interval == 0) ? (fnet_size_t) - 1 : (fnet_size_t)((fapp_bench.remote_bytes * 8/**(1000*/ / FNET_TIMER_PERIOD_MS/*)*/) / interval)/*/1000*/);
    }
}

/************************************************************************
* NAME: fapp_bench_tcp_rx
*
* DESCRIPTION: Start Benchmark TCP server.
************************************************************************/
static void fapp_bench_tcp_rx (fnet_shell_desc_t desc, fnet_address_family_t family)
{
    struct sockaddr     local_addr;
    fnet_int32_t        received;
    fnet_char_t         ip_str[FNET_IP_ADDR_STR_SIZE];
    struct linger       linger_option = {FNET_TRUE, /*l_onoff*/
               4  /*l_linger*/
    };
    fnet_size_t         bufsize_option = FAPP_BENCH_SOCKET_BUF_SIZE;
    fnet_int32_t        keepalive_option = 1;
    fnet_int32_t        keepcnt_option = FAPP_BENCH_TCP_KEEPCNT;
    fnet_int32_t        keepintvl_option = FAPP_BENCH_TCP_KEEPINTVL;
    fnet_int32_t        keepidle_option = FAPP_BENCH_TCP_KEEPIDLE;
    struct sockaddr     foreign_addr;
    fnet_size_t         addr_len;
    fnet_bool_t         exit_flag = FNET_FALSE;


    fapp_bench.socket_foreign = FNET_ERR;

    /* Create listen socket */
    if((fapp_bench.socket_listen = fnet_socket(family, SOCK_STREAM, 0)) == FNET_ERR)
    {
        FNET_DEBUG("BENCH: Socket creation error.");
        goto ERROR_1;
    }

    /* Bind socket.*/
    fnet_memset_zero(&local_addr, sizeof(local_addr));

    local_addr.sa_port = FAPP_BENCH_PORT;
    local_addr.sa_family = family;

    if(fnet_socket_bind(fapp_bench.socket_listen, &local_addr, sizeof(local_addr)) == FNET_ERR)
    {
        FNET_DEBUG("BENCH: Socket bind error.");
        goto ERROR_2;
    }

    /* Set Socket options. */
    if( /* Setup linger option. */
        (fnet_socket_setopt (fapp_bench.socket_listen, SOL_SOCKET, SO_LINGER, (fnet_uint8_t *)&linger_option, sizeof(linger_option)) == FNET_ERR) ||
        /* Set socket buffer size. */
        (fnet_socket_setopt(fapp_bench.socket_listen, SOL_SOCKET, SO_RCVBUF, (fnet_uint8_t *) &bufsize_option, sizeof(bufsize_option)) == FNET_ERR) ||
        (fnet_socket_setopt(fapp_bench.socket_listen, SOL_SOCKET, SO_SNDBUF, (fnet_uint8_t *) &bufsize_option, sizeof(bufsize_option)) == FNET_ERR) ||
        /* Enable keepalive_option option. */
        (fnet_socket_setopt (fapp_bench.socket_listen, SOL_SOCKET, SO_KEEPALIVE, (fnet_uint8_t *)&keepalive_option, sizeof(keepalive_option)) == FNET_ERR) ||
        /* Keepalive probe retransmit limit. */
        (fnet_socket_setopt (fapp_bench.socket_listen, IPPROTO_TCP, TCP_KEEPCNT, (fnet_uint8_t *)&keepcnt_option, sizeof(keepcnt_option)) == FNET_ERR) ||
        /* Keepalive retransmit interval.*/
        (fnet_socket_setopt (fapp_bench.socket_listen, IPPROTO_TCP, TCP_KEEPINTVL, (fnet_uint8_t *)&keepintvl_option, sizeof(keepintvl_option)) == FNET_ERR) ||
        /* Time between keepalive probes.*/
        (fnet_socket_setopt (fapp_bench.socket_listen, IPPROTO_TCP, TCP_KEEPIDLE, (fnet_uint8_t *)&keepidle_option, sizeof(keepidle_option)) == FNET_ERR)
    )
    {
        FNET_DEBUG("BENCH: Socket setsockopt error.\n");
        goto ERROR_2;
    }

    /* Listen. */
    if(fnet_socket_listen(fapp_bench.socket_listen, 1) == FNET_ERR)
    {
        FNET_DEBUG("BENCH: Socket listen error.\n");
        goto ERROR_2;
    }

    /* ------ Start test.----------- */
    fnet_shell_println(desc, FAPP_DELIMITER_STR);
    fnet_shell_println(desc, " TCP RX Test");
    fnet_shell_println(desc, FAPP_DELIMITER_STR);
    fapp_print_netif_addr(desc, family, fnet_netif_get_default(), FNET_FALSE);
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Local Port", FNET_NTOHS(FAPP_BENCH_PORT));
    fnet_shell_println(desc, FAPP_TOCANCEL_STR);
    fnet_shell_println(desc, FAPP_DELIMITER_STR);

    while(exit_flag == FNET_FALSE)
    {
        fnet_shell_println(desc, "Waiting.");

        fapp_bench.bytes = 0;
        fapp_bench.remote_bytes = 0;
        if(fapp_bench.socket_foreign != FNET_ERR)
        {
            fnet_socket_close(fapp_bench.socket_foreign);
            fapp_bench.socket_foreign = FNET_ERR;
        }

        while((fapp_bench.socket_foreign == FNET_ERR) && (exit_flag == FNET_FALSE))
        {
            /*Accept*/
            addr_len = sizeof(foreign_addr);
            fapp_bench.socket_foreign = fnet_socket_accept(fapp_bench.socket_listen, &foreign_addr, &addr_len);


            exit_flag = fnet_shell_ctrlc (desc);

            if(fapp_bench.socket_foreign != FNET_ERR)
            {

                fnet_shell_println(desc, "Receiving from %s:%d", fnet_inet_ntop(foreign_addr.sa_family, (fnet_uint8_t *)(foreign_addr.sa_data), ip_str, sizeof(ip_str)), fnet_ntohs(foreign_addr.sa_port));

                fapp_bench.first_time = fnet_timer_ticks();

                while(1) /* Receiving data.*/
                {
                    received = fnet_socket_recv(fapp_bench.socket_foreign, (fnet_uint8_t *)(&fapp_bench.buffer[0]), FAPP_BENCH_BUFFER_SIZE, 0);

                    if ((received == FNET_ERR) || exit_flag)
                    {
                        fapp_bench.last_time = fnet_timer_ticks();

                        /* Print benchmark results.*/
                        fapp_bench_print_results (desc);
                        break;
                    }
                    else
                    {
                        fapp_bench.bytes += received;
                    }

                    exit_flag = fnet_shell_ctrlc (desc); /* Check [Ctrl+c]*/
                }
            }
        }
    }

    fnet_socket_close(fapp_bench.socket_foreign);

ERROR_2:
    fnet_socket_close(fapp_bench.socket_listen);

ERROR_1:

    fnet_shell_println(desc, FAPP_BENCH_COMPLETED_STR);
}

/************************************************************************
* NAME: fapp_bench_udp_rx
*
* DESCRIPTION: Start Benchmark UDP server.
************************************************************************/
static void fapp_bench_udp_rx (fnet_shell_desc_t desc, fnet_address_family_t family, struct sockaddr *multicast_address /* optional, set to 0*/)
{
    struct sockaddr         local_addr;
    const fnet_size_t       bufsize_option = FAPP_BENCH_SOCKET_BUF_SIZE;
    fnet_int32_t            received;
    fnet_char_t             ip_str[FNET_IP_ADDR_STR_SIZE];
    struct sockaddr         addr;
    fnet_size_t             addr_len;
    fnet_bool_t             is_first = FNET_TRUE;
    fnet_bool_t             exit_flag = FNET_FALSE;


    /* Create listen socket */
    if((fapp_bench.socket_listen = fnet_socket(family, SOCK_DGRAM, 0)) == FNET_ERR)
    {
        FNET_DEBUG("BENCH: Socket creation error.\n");
        goto ERROR_1;
    }

    /*Bind.*/
    fnet_memset_zero(&local_addr, sizeof(local_addr));

    local_addr.sa_port = FAPP_BENCH_PORT;
    local_addr.sa_family = family;

    if(fnet_socket_bind(fapp_bench.socket_listen, &local_addr, sizeof(local_addr)) == FNET_ERR)
    {
        FNET_DEBUG("BENCH: Socket bind error.\n");
        goto ERROR_2;
    }


    /* Set socket options. */
    if(
        /* Set socket buffer size. */
        (fnet_socket_setopt(fapp_bench.socket_listen, SOL_SOCKET, SO_RCVBUF, (fnet_uint8_t *) &bufsize_option, sizeof(bufsize_option)) == FNET_ERR) ||
        (fnet_socket_setopt(fapp_bench.socket_listen, SOL_SOCKET, SO_SNDBUF, (fnet_uint8_t *) &bufsize_option, sizeof(bufsize_option)) == FNET_ERR)
    )
    {
        FNET_DEBUG("BENCH: Socket setsockopt error.\n");
        goto ERROR_2;
    }

    /* Join multicast group, if set. */
    if(multicast_address)
    {

#if FNET_CFG_IP4
        if(multicast_address->sa_family == AF_INET)
        {
            struct ip_mreq mreq; /* Multicast group information.*/

            mreq.imr_multiaddr.s_addr = ((struct sockaddr_in *)multicast_address)->sin_addr.s_addr;
            mreq.imr_interface = 0; /* Default Interface.*/

            /* Join multicast group. */
            if(fnet_socket_setopt(fapp_bench.socket_listen, IPPROTO_IP, IP_ADD_MEMBERSHIP, (fnet_uint8_t *)&mreq, sizeof(mreq)) == FNET_ERR)
            {
                FNET_DEBUG("BENCH: Joining to multicast group is failed.\n");
                goto ERROR_2;
            }
        }
#endif
#if FNET_CFG_IP6
        if(multicast_address->sa_family == AF_INET6)
        {
            struct ipv6_mreq mreq6; /* Multicast group information.*/

            FNET_IP6_ADDR_COPY(&((struct sockaddr_in6 *)multicast_address)->sin6_addr.s6_addr, &mreq6.ipv6imr_multiaddr.s6_addr);
            mreq6.ipv6imr_interface = ((struct sockaddr_in6 *)multicast_address)->sin6_scope_id;

            /* Join multicast group. */
            if(fnet_socket_setopt(fapp_bench.socket_listen, IPPROTO_IPV6, IPV6_JOIN_GROUP, (fnet_uint8_t *)&mreq6, sizeof(mreq6)) == FNET_ERR)
            {
                FNET_DEBUG("BENCH: Joining to multicast group is failed.\n");
                goto ERROR_2;
            }
        }
#endif

    }

    /* ------ Start test.----------- */
    fnet_shell_println(desc, FAPP_DELIMITER_STR);
    fnet_shell_println(desc, " UDP RX Test" );
    fnet_shell_println(desc, FAPP_DELIMITER_STR);
    fapp_print_netif_addr(desc, family, fnet_netif_get_default(), FNET_FALSE);
    if(multicast_address)
    {
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Multicast Group", fnet_inet_ntop(multicast_address->sa_family, (fnet_uint8_t *)(multicast_address->sa_data), ip_str, sizeof(ip_str)) );
    }
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Local Port", FNET_NTOHS(FAPP_BENCH_PORT));
    fnet_shell_println(desc, FAPP_TOCANCEL_STR);
    fnet_shell_println(desc, FAPP_DELIMITER_STR);


    while(exit_flag == FNET_FALSE) /* Main loop */
    {
        fnet_shell_println(desc, "Waiting.");

        fapp_bench.bytes = 0;
        fapp_bench.remote_bytes = 0;
        addr_len = sizeof(addr);
        is_first = FNET_TRUE;

        while(exit_flag == FNET_FALSE) /* Test loop. */
        {

            /* Receive data */
            received = fnet_socket_recvfrom  (fapp_bench.socket_listen, (fnet_uint8_t *)(&fapp_bench.buffer[0]), FAPP_BENCH_BUFFER_SIZE, 0,
                                              &addr, &addr_len );

            if(received >= FAPP_BENCH_UDP_END_BUFFER_LENGTH)
            {

                /* Reset timeout. */
                fapp_bench.last_time = fnet_timer_ticks();

                if(is_first)
                {
                    if( received > FAPP_BENCH_UDP_END_BUFFER_LENGTH )
                    {
                        fnet_shell_println(desc, "Receiving from %s:%d",  fnet_inet_ntop(addr.sa_family, (fnet_uint8_t *)(addr.sa_data), ip_str, sizeof(ip_str)), fnet_ntohs(addr.sa_port));
                        fapp_bench.first_time = fnet_timer_ticks();
                        is_first = FNET_FALSE;
                    }
                }
                else
                {
                    if(received == FAPP_BENCH_UDP_END_BUFFER_LENGTH ) /* End of transfer. */
                    {
                        /* Send ACK containing amount of received data.*/
                        fnet_uint32_t ack_bytes = fnet_htonl(fapp_bench.bytes);

                        /* Send several times, just to be sure that it is received/not lost.*/
                        fnet_socket_sendto(fapp_bench.socket_listen, (fnet_uint8_t *)(&ack_bytes), sizeof(ack_bytes), 0, (struct sockaddr *)&addr, sizeof(addr));
                        fnet_socket_sendto(fapp_bench.socket_listen, (fnet_uint8_t *)(&ack_bytes), sizeof(ack_bytes), 0, (struct sockaddr *)&addr, sizeof(addr));
                        fnet_socket_sendto(fapp_bench.socket_listen, (fnet_uint8_t *)(&ack_bytes), sizeof(ack_bytes), 0, (struct sockaddr *)&addr, sizeof(addr));

                        /* Print benchmark results.*/
                        fapp_bench_print_results (desc);

                        break;
                    }
                    else
                        fapp_bench.bytes += received;
                }
            }
            else
            {
                /* Check error. Check timeout */
                if(received == FNET_ERR)
                {
                    fnet_shell_println(desc, "BENCH: Error (%d).", fnet_error_get());
                    break;
                }
                /* Check timeout. */
                if((is_first == FNET_FALSE) &&
                   (fnet_timer_get_interval(fapp_bench.last_time, fnet_timer_ticks()) > (FAPP_UDP_TIMEOUT_MS / FNET_TIMER_PERIOD_MS)))
                {
                    fnet_shell_println(desc, "BENCH: Exit on timeout.");
                    fapp_bench_print_results (desc);
                    break;
                }
            }

            exit_flag = fnet_shell_ctrlc (desc);
        }
    }

ERROR_2:
    fnet_socket_close(fapp_bench.socket_listen);

ERROR_1:

    fnet_shell_println(desc, FAPP_BENCH_COMPLETED_STR);
}

/************************************************************************
* NAME: fapp_benchrx_cmd
*
* DESCRIPTION: Start RX Benchmark server.
************************************************************************/
void fapp_benchrx_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_address_family_t family;

    family = AF_SUPPORTED;

    /* TCP */
    if((argc == 1) || (argc == 2 && fnet_strcasecmp("tcp", argv[1]) == 0))
    {
        fapp_bench_tcp_rx(desc, family);
    }
    /* UDP */
    else if(((argc == 2) || (argc == 3)) && fnet_strcasecmp("udp", argv[1]) == 0)
    {
        struct sockaddr multicast_address;
        struct sockaddr *multicast_address_p = FNET_NULL;

        if(argc == 3) /* Multicast group address.*/
        {
            if((fnet_inet_ptos(argv[2], &multicast_address) == FNET_ERR)    /* Convert from string to address.*/
               || !fnet_socket_addr_is_multicast(&multicast_address)       /* Check if address is multicast.*/
               || ((family & multicast_address.sa_family) == 0) )              /* Check supported family.*/
            {
                fnet_shell_println(desc, FAPP_PARAM_ERR, argv[2]);
                return;
            }

            multicast_address_p = &multicast_address;
        }

        fapp_bench_udp_rx(desc, family, multicast_address_p);
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}

/*============================ TX ===========================================*/
/************************************************************************
* NAME: fapp_bench_tcp_tx
*
* DESCRIPTION: Start TX TCP Benchmark.
************************************************************************/
static void fapp_bench_tcp_tx (struct fapp_bench_tx_params *params)
{
    fnet_int32_t            send_result;
    fnet_char_t             ip_str[FNET_IP_ADDR_STR_SIZE];
    const struct linger     linger_option = {FNET_TRUE, /*l_onoff*/
              4  /*l_linger*/
    };
    const fnet_size_t       bufsize_option = FAPP_BENCH_SOCKET_BUF_SIZE;
    const fnet_int32_t      keepalive_option = 1;
    const fnet_int32_t      keepcnt_option = FAPP_BENCH_TCP_KEEPCNT;
    const fnet_int32_t      keepintvl_option = FAPP_BENCH_TCP_KEEPINTVL;
    const fnet_int32_t      keepidle_option = FAPP_BENCH_TCP_KEEPIDLE;
    struct sockaddr         foreign_addr;
    fnet_bool_t             exit_flag = FNET_FALSE;
    fnet_int32_t            sock_err ;
    fnet_size_t             option_len;
    fnet_shell_desc_t       desc = params->desc;
    fnet_socket_state_t     connection_state;
    fnet_size_t             packet_size = params->packet_size;
    fnet_index_t            cur_packet_number;
    fnet_size_t             buffer_offset;
    fnet_index_t            iterations = params->iteration_number;
    fnet_address_family_t   family = params->foreign_addr.sa_family;

    if(packet_size > FAPP_BENCH_BUFFER_SIZE) /* Check max size.*/
        packet_size = FAPP_BENCH_BUFFER_SIZE;


    fapp_bench.socket_listen = FNET_ERR;

    /* ------ Start test.----------- */
    fnet_shell_println(desc, FAPP_DELIMITER_STR);
    fnet_shell_println(desc, " TCP TX Test" );
    fnet_shell_println(desc, FAPP_DELIMITER_STR);
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Remote IP Addr", fnet_inet_ntop(family, params->foreign_addr.sa_data, ip_str, sizeof(ip_str)));
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Remote Port", fnet_ntohs(params->foreign_addr.sa_port));
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Message Size", params->packet_size);
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Num. of messages", params->packet_number);
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Num. of iterations", params->iteration_number);
    fnet_shell_println(desc, FAPP_TOCANCEL_STR);
    fnet_shell_println(desc, FAPP_DELIMITER_STR);


    while(iterations--)
    {
        /* Create socket */
        if((fapp_bench.socket_foreign = fnet_socket(family, SOCK_STREAM, 0)) == FNET_ERR)
        {
            FNET_DEBUG("BENCH: Socket creation error.\n");
            iterations = 0;
            goto ERROR_1;
        }

        /* Set Socket options. */
        if( /* Setup linger option. */
            (fnet_socket_setopt (fapp_bench.socket_foreign, SOL_SOCKET, SO_LINGER, (fnet_uint8_t *)&linger_option, sizeof(linger_option)) == FNET_ERR) ||
            /* Set socket buffer size. */
            (fnet_socket_setopt(fapp_bench.socket_foreign, SOL_SOCKET, SO_RCVBUF, (fnet_uint8_t *) &bufsize_option, sizeof(bufsize_option)) == FNET_ERR) ||
            (fnet_socket_setopt(fapp_bench.socket_foreign, SOL_SOCKET, SO_SNDBUF, (fnet_uint8_t *) &bufsize_option, sizeof(bufsize_option)) == FNET_ERR) ||
            /* Enable keepalive_option option. */
            (fnet_socket_setopt (fapp_bench.socket_foreign, SOL_SOCKET, SO_KEEPALIVE, (fnet_uint8_t *)&keepalive_option, sizeof(keepalive_option)) == FNET_ERR) ||
            /* Keepalive probe retransmit limit. */
            (fnet_socket_setopt (fapp_bench.socket_foreign, IPPROTO_TCP, TCP_KEEPCNT, (fnet_uint8_t *)&keepcnt_option, sizeof(keepcnt_option)) == FNET_ERR) ||
            /* Keepalive retransmit interval.*/
            (fnet_socket_setopt (fapp_bench.socket_foreign, IPPROTO_TCP, TCP_KEEPINTVL, (fnet_uint8_t *)&keepintvl_option, sizeof(keepintvl_option)) == FNET_ERR) ||
            /* Time between keepalive probes.*/
            (fnet_socket_setopt (fapp_bench.socket_foreign, IPPROTO_TCP, TCP_KEEPIDLE, (fnet_uint8_t *)&keepidle_option, sizeof(keepidle_option)) == FNET_ERR)
        )
        {
            FNET_DEBUG("BENCH: Socket setsockopt error.\n");
            iterations = 0;
            goto ERROR_2;
        }

        /* Connect to the server.*/
        fnet_shell_println(desc, "Connecting.");

        fnet_memcpy(&foreign_addr, &params->foreign_addr, sizeof(foreign_addr));

        fnet_socket_connect(fapp_bench.socket_foreign, (struct sockaddr *)(&foreign_addr), sizeof(foreign_addr));

        do
        {
            option_len = sizeof(connection_state);
            fnet_socket_getopt(fapp_bench.socket_foreign, SOL_SOCKET, SO_STATE, (fnet_uint8_t *)&connection_state, &option_len);
        }
        while (connection_state == SS_CONNECTING);

        if(connection_state != SS_CONNECTED)
        {
            fnet_shell_println(desc, "Connection failed.");
            iterations = 0;
            goto ERROR_2;
        }

        /* Sending.*/
        fnet_shell_println(desc, "Sending.");
        fapp_bench.bytes = 0;
        fapp_bench.remote_bytes = 0;
        cur_packet_number = 0;
        buffer_offset = 0;

        fapp_bench.first_time = fnet_timer_ticks();

        while(1)
        {
            send_result = fnet_socket_send( fapp_bench.socket_foreign, (fnet_uint8_t *)(&fapp_bench.buffer[buffer_offset]), (packet_size - buffer_offset), 0);
            fapp_bench.last_time = fnet_timer_ticks();

            if ( send_result == FNET_ERR )
            {
                option_len = sizeof(sock_err);
                fnet_socket_getopt(fapp_bench.socket_foreign, SOL_SOCKET, SO_ERROR, &sock_err, &option_len);
                fnet_shell_println(desc, "Socket error = %d", sock_err);

                iterations = 0;
#if 0
                /* Print benchmark results.*/
                fapp_bench_print_results (desc);
#endif
                break;
            }
            else if(send_result)
            {
                fapp_bench.bytes += send_result;
                buffer_offset += send_result;

                if(buffer_offset == packet_size)
                {
                    cur_packet_number ++;
                    buffer_offset = 0;
                }

                exit_flag = fnet_shell_ctrlc(desc); /* Check [Ctrl+c]*/

                if((cur_packet_number >= params->packet_number) || exit_flag)
                {
                    if(exit_flag)
                    {
                        fnet_shell_println(desc, FAPP_SHELL_CANCELED_CTRL_C);
                        iterations = 0;
                    }
                    /* Print benchmark results.*/
                    fapp_bench_print_results (desc);
                    break;
                }
            }
            else
            {}
        }

    ERROR_2:
        fnet_socket_close(fapp_bench.socket_foreign);
    }
ERROR_1:
    fnet_shell_println(desc, FAPP_BENCH_COMPLETED_STR);
}

/************************************************************************
* NAME: fapp_bench_udp_tx
*
* DESCRIPTION: Start TX TCP Benchmark.
************************************************************************/
static void fapp_bench_udp_tx (struct fapp_bench_tx_params *params)
{
    fnet_int32_t            send_result;
    fnet_char_t            ip_str[FNET_IP_ADDR_STR_SIZE];
    fnet_index_t            i;
    fnet_int32_t            received;
    const struct linger     linger_option = {FNET_TRUE, /*l_onoff*/
              4  /*l_linger*/
    };
    const fnet_size_t       bufsize_option = FAPP_BENCH_SOCKET_BUF_SIZE;
    struct sockaddr         foreign_addr;
    fnet_bool_t             exit_flag = FNET_FALSE;
    fnet_int32_t            sock_err;
    fnet_size_t             option_len;
    fnet_shell_desc_t       desc = params->desc;
    fnet_size_t             packet_size = params->packet_size;
    fnet_index_t            cur_packet_number;
    fnet_index_t            iterations = params->iteration_number;
    fnet_address_family_t   family = params->foreign_addr.sa_family;


    if(packet_size > FAPP_BENCH_BUFFER_SIZE) /* Check max size.*/
        packet_size = FAPP_BENCH_BUFFER_SIZE;

    fapp_bench.socket_listen = FNET_ERR;

    /* ------ Start test.----------- */
    fnet_shell_println(desc, FAPP_DELIMITER_STR);
    fnet_shell_println(desc, " UDP TX Test" );
    fnet_shell_println(desc, FAPP_DELIMITER_STR);
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Remote IP addr", fnet_inet_ntop(family, params->foreign_addr.sa_data, ip_str, sizeof(ip_str)));
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Remote Port", fnet_ntohs(params->foreign_addr.sa_port));
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Message Size", params->packet_size);
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Num. of messages", params->packet_number);
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Num. of iterations", params->iteration_number);
    fnet_shell_println(desc, FAPP_TOCANCEL_STR);
    fnet_shell_println(desc, FAPP_DELIMITER_STR);

    while(iterations--)
    {
        /* Create socket */
        if((fapp_bench.socket_foreign = fnet_socket(family, SOCK_DGRAM, 0)) == FNET_ERR)
        {
            FNET_DEBUG("BENCH: Socket creation error.\n");
            iterations = 0;
            goto ERROR_1;
        }

        /* Set Socket options. */
        if( /* Setup linger option. */
            (fnet_socket_setopt (fapp_bench.socket_foreign, SOL_SOCKET, SO_LINGER, (fnet_uint8_t *)&linger_option, sizeof(linger_option)) == FNET_ERR) ||
            /* Set socket buffer size. */
            (fnet_socket_setopt(fapp_bench.socket_foreign, SOL_SOCKET, SO_RCVBUF, (fnet_uint8_t *) &bufsize_option, sizeof(bufsize_option)) == FNET_ERR) ||
            (fnet_socket_setopt(fapp_bench.socket_foreign, SOL_SOCKET, SO_SNDBUF, (fnet_uint8_t *) &bufsize_option, sizeof(bufsize_option)) == FNET_ERR)
        )
        {
            FNET_DEBUG("BENCH: Socket setsockopt error.\n");
            iterations = 0;
            goto ERROR_2;
        }

        /* Bind to the server.*/
        fnet_shell_println(desc, "Connecting.");

        fnet_memcpy(&foreign_addr, &params->foreign_addr, sizeof(foreign_addr));

        if(fnet_socket_connect(fapp_bench.socket_foreign, &foreign_addr, sizeof(foreign_addr)) == FNET_ERR)
        {
            fnet_shell_println(desc, "Connection failed.");
            iterations = 0;
            goto ERROR_2;
        }

        /* Sending.*/
        fnet_shell_println(desc, "Sending.");
        fapp_bench.bytes = 0;
        fapp_bench.remote_bytes = 0;
        cur_packet_number = 0;

        fapp_bench.first_time = fnet_timer_ticks();

        while(1)
        {
            send_result = fnet_socket_send( fapp_bench.socket_foreign, (fnet_uint8_t *)(&fapp_bench.buffer[0]), packet_size, 0);
            fapp_bench.last_time = fnet_timer_ticks();


            if ( send_result == FNET_ERR )
            {
                option_len = sizeof(sock_err);
                fnet_socket_getopt(fapp_bench.socket_foreign, SOL_SOCKET, SO_ERROR, (fnet_uint8_t *)&sock_err, &option_len);
                fnet_shell_println(desc, "socket_error = %d", sock_err);

                iterations = 0;

                goto ERROR_2;
            }
            else
            {
                fapp_bench.bytes += send_result;
                cur_packet_number ++;

                exit_flag = fnet_shell_ctrlc (desc); /* Check [Ctrl+c]*/

                if((cur_packet_number >= params->packet_number) || exit_flag)
                {
                    if(exit_flag)
                    {
                        fnet_shell_println(desc, FAPP_SHELL_CANCELED_CTRL_C);
                        iterations = 0;
                    }

                    break;/* => TX END. */
                }
            }
        }

        /* Send End mark.*/
        for(i = 0; i < FAPP_BENCH_TX_UDP_END_ITERATIONS; i++)
        {
            fnet_uint32_t ack_bytes;

            /* Send END mark.*/
            fnet_socket_send( fapp_bench.socket_foreign, (fnet_uint8_t *)(&fapp_bench.buffer[0]), 1, 0);
            fnet_timer_delay(1);

            /* Check ACK. It should contain recieved amount of data.*/
            received = fnet_socket_recv(fapp_bench.socket_foreign, (fnet_uint8_t *)(&ack_bytes), sizeof(ack_bytes), 0);

            if(received == sizeof(ack_bytes)) /* ACK received.*/
            {
                fapp_bench.remote_bytes = fnet_ntohl(ack_bytes);
                break;
            }
            else if(received == FNET_ERR)
            {
                break;
            }
            else
            {}
        }

        /* Print benchmark results.*/
        fapp_bench_print_results (desc);

    ERROR_2:
        fnet_socket_close(fapp_bench.socket_foreign);
    }

ERROR_1:
    fnet_shell_println(desc, FAPP_BENCH_COMPLETED_STR);
}


/************************************************************************
* NAME: fapp_benchtx_cmd
*
* DESCRIPTION: Start TX Benchmark.
************************************************************************/
void fapp_benchtx_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fapp_bench_tx_params bench_params;

    fnet_memset_zero(&bench_params.foreign_addr, sizeof(bench_params.foreign_addr));

    if(fnet_inet_ptos(argv[1], &bench_params.foreign_addr) == FNET_OK)
    {
        bench_params.desc = desc;

        bench_params.foreign_addr.sa_port = FAPP_BENCH_PORT;

        bench_params.packet_size = FAPP_BENCH_TX_PACKET_SIZE_DEFAULT;
        bench_params.packet_number = FAPP_BENCH_TX_PACKET_NUMBER_DEFAULT;
        bench_params.iteration_number = FAPP_BENCH_TX_ITERATION_NUMBER_DEFAULT;

        if(argc > 3)
        {
            fnet_char_t *p = 0;

            /* Packet size.*/
            bench_params.packet_size = fnet_strtoul(argv[3], &p, 0);
            if ((bench_params.packet_size == 0) || (bench_params.packet_size > FAPP_BENCH_PACKET_SIZE_MAX))
            {
                fnet_shell_println(desc, FAPP_PARAM_ERR, argv[3]); /* Print error mesage. */
                return;
            }

            /* Number of packets.*/
            if(argc > 4)
            {
                bench_params.packet_number = fnet_strtoul(argv[4], &p, 0);
                if (bench_params.packet_number == 0)
                {
                    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[4]); /* Print error mesage. */
                    return;
                }

                /* Number of iterations.*/
                if(argc > 5)
                {
                    bench_params.iteration_number = fnet_strtoul(argv[5], &p, 0);
                    if ((bench_params.iteration_number < 1) || (bench_params.iteration_number > FAPP_BENCH_TX_ITERATION_NUMBER_MAX) )
                    {
                        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[5]); /* Print error mesage. */
                        return;
                    }
                }
            }
        }

        /* TCP */
        if((argc == 2) || (argc >= 3 && fnet_strcasecmp("tcp", argv[2]) == 0))
        {
            fapp_bench_tcp_tx (&bench_params);
        }
        /* UDP */
        else if(argc >= 3 && fnet_strcasecmp("udp", argv[2]) == 0)
        {
            fapp_bench_udp_tx (&bench_params);
        }
        else
        {
            fnet_shell_println(desc, FAPP_PARAM_ERR, argv[2]);
        }

    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);   /* Wrong Benchmark Server IP address. */
        return;
    }
}


#endif /* FAPP_CFG_BENCH_CMD */















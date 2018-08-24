/**************************************************************************
*
* Copyright 2018 by Andrey Butok. FNET Community.
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
*  FNET benchmark server implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_BENCH_SRV

#if FNET_CFG_DEBUG_BENCH_SRV && FNET_CFG_DEBUG
    #define FNET_DEBUG_BENCH_SRV   FNET_DEBUG
#else
    #define FNET_DEBUG_BENCH_SRV(...) do{}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/

#define FNET_BENCH_SRV_UDP_END_LENGTH           1 /* End of UDP session mark */
#define FNET_BENCH_SRV_SESSION_TIMEOUT_MS       (8000) /*ms*/ /*Session timeout, without any activity.*/

/* Error strings.*/
#define FNET_BENCH_SRV_ERR_PARAMS             "[BENCH srv] Wrong input parameters"
#define FNET_BENCH_SRV_ERR_SOCKET_CREATION    "[BENCH srv] Socket creation error"
#define FNET_BENCH_SRV_ERR_SOCKET_BIND        "[BENCH srv] Socket bind error"
#define FNET_BENCH_SRV_ERR_SOCKET_LISTEN      "[BENCH srv] Socket listen error"
#define FNET_BENCH_SRV_ERR_SOCKET_OPTION      "[BENCH srv] Socket option error"
#define FNET_BENCH_SRV_ERR_SOCKET_MULTICAST   "[BENCH srv] Socket multicast join error"
#define FNET_BENCH_SRV_ERR_SERVICE            "[BENCH srv] Service registration error."
#define FNET_BENCH_SRV_ERR_IS_INITIALIZED     "[BENCH srv] Is already initialized."

typedef enum
{
    FNET_BENCH_SRV_STATE_DISABLED = 0,  /* Benchmark server is not initialized.*/
    FNET_BENCH_SRV_STATE_LISTENING,     /* Benchmark server is listening for incoming connection.*/
    FNET_BENCH_SRV_STATE_RX,            /* Benchmark server is receiving.*/
} fnet_bench_srv_state_t;

/************************************************************************
*    Benchmark Server interface structure.
*************************************************************************/
typedef struct fnet_bench_srv_if
{
    fnet_bool_t                     is_enabled;
    fnet_service_desc_t             service_descriptor;
    fnet_bench_srv_state_t          state;
    fnet_socket_type_t              type;
    fnet_socket_t                   socket_listen;                  /* Listening socket.*/
    fnet_socket_t                   socket_foreign;                 /* Foreign socket.*/
    struct fnet_sockaddr            address;                        /* Address of the remote client at the other end of the connection.*/
    fnet_bench_srv_callback_session_begin_t callback_session_begin;
    fnet_bench_srv_callback_session_end_t   callback_session_end;
    void                            *callback_session_begin_cookie;
    void                            *callback_session_end_cookie;
    fnet_time_t                     time_last; /* Last receive timestamp. Used for session timeout control */
    fnet_time_t                     time_begin;
    struct fnet_bench_srv_result    bench_srv_result;
} fnet_bench_srv_if_t;

/* Receive buffer */
static fnet_uint8_t fnet_bench_srv_buffer[FNET_CFG_BENCH_SRV_BUFFER_SIZE];
/* The Benchmark server interface list*/
static  fnet_bench_srv_if_t fnet_bench_srv_if_list[FNET_CFG_BENCH_SRV];

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void _fnet_bench_srv_poll(void *fnet_bench_srv_if_p);
static void _fnet_bench_srv_close_session(fnet_bench_srv_if_t *bench_srv_if);

/************************************************************************
* DESCRIPTION: Initializes the Benchmark server service.
************************************************************************/
fnet_bench_srv_desc_t fnet_bench_srv_init( fnet_bench_srv_params_t *params )
{
    fnet_index_t                i;
    fnet_bench_srv_if_t         *bench_srv_if = FNET_NULL;
    struct fnet_sockaddr        local_addr;
    fnet_size_t                 bufsize_option = FNET_CFG_BENCH_SRV_BUFFER_SIZE;

    fnet_service_mutex_lock();

    if((params == 0) || !((params->type == SOCK_STREAM) || (params->type == SOCK_DGRAM)))
    {
        FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_PARAMS);
        goto ERROR_1;
    }

    /* Try to find free benchmark server. */
    for(i = 0u; i < FNET_CFG_BENCH_SRV; i++)
    {
        if(fnet_bench_srv_if_list[i].is_enabled == FNET_FALSE)
        {
            bench_srv_if = &fnet_bench_srv_if_list[i];
            break;
        }
    }

    /* Is benchmark server is already initialized. */
    if(bench_srv_if == 0)
    {
        FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_IS_INITIALIZED);
        goto ERROR_1;
    }

    /* Clear all parameters.*/
    fnet_memset_zero(bench_srv_if, sizeof(*bench_srv_if));

    /* Set parameters */
    fnet_memcpy(&local_addr, &params->address, sizeof(local_addr));
    if(local_addr.sa_port == 0u)
    {
        local_addr.sa_port = FNET_CFG_BENCH_SRV_PORT;
    }
    if(local_addr.sa_family == AF_UNSPEC)
    {
        local_addr.sa_family = AF_SUPPORTED; /* Assign supported families.*/
    }
    bench_srv_if->type = params->type;


    /* Create listen socket */
    bench_srv_if->socket_listen = fnet_socket(local_addr.sa_family,  bench_srv_if->type, 0u);
    if(bench_srv_if->socket_listen == FNET_NULL)
    {
        FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_SOCKET_CREATION);
        goto ERROR_1;
    }

    /* Bind.*/
    if(fnet_socket_bind(bench_srv_if->socket_listen, &local_addr, sizeof(local_addr)) == FNET_ERR)
    {
        FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_SOCKET_BIND);
        goto ERROR_2;
    }

    /* Set socket rx buffer size .*/
    if( fnet_socket_setopt(bench_srv_if->socket_listen, SOL_SOCKET, SO_RCVBUF, &bufsize_option, sizeof(bufsize_option)) == FNET_ERR)
    {
        FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_SOCKET_OPTION);
        goto ERROR_2;
    }

    switch(bench_srv_if->type)
    {
        case SOCK_STREAM:
        {
            const struct fnet_linger    linger_option =
            {
                .l_onoff = FNET_TRUE,
                .l_linger = 4 /*sec*/
            };
            const fnet_int32_t        keepalive_option = 1;
            /* Keepalive probe retransmit limit.*/
            const fnet_int32_t        keepcnt_option = 2;
            /* Keepalive retransmit interval.*/
            const fnet_int32_t        keepintvl_option = 5; /*sec*/
            /* Time between keepalive probes.*/
            const fnet_int32_t        keepidle_option = 5; /* sec */

            /* Set TCP Socket options. */
            if( /* Setup linger option. */
                (fnet_socket_setopt (bench_srv_if->socket_listen, SOL_SOCKET, SO_LINGER, &linger_option, sizeof(linger_option)) == FNET_ERR) ||
                /* Enable keepalive_option option. */
                (fnet_socket_setopt (bench_srv_if->socket_listen, SOL_SOCKET, SO_KEEPALIVE, &keepalive_option, sizeof(keepalive_option)) == FNET_ERR) ||
                /* Keepalive probe retransmit limit. */
                (fnet_socket_setopt (bench_srv_if->socket_listen, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt_option, sizeof(keepcnt_option)) == FNET_ERR) ||
                /* Keepalive retransmit interval.*/
                (fnet_socket_setopt (bench_srv_if->socket_listen, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl_option, sizeof(keepintvl_option)) == FNET_ERR) ||
                /* Time between keepalive probes.*/
                (fnet_socket_setopt (bench_srv_if->socket_listen, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle_option, sizeof(keepidle_option)) == FNET_ERR)
            )
            {
                FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_SOCKET_OPTION);
                goto ERROR_2;
            }

            /* Listen. */
            if(fnet_socket_listen(bench_srv_if->socket_listen, 1) == FNET_ERR)
            {
                FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_SOCKET_LISTEN);
                goto ERROR_2;
            }
        }
        break;
        case SOCK_DGRAM:
            /* Join multicast group, if set. */ //TBD add drop membership
            if(fnet_socket_addr_is_multicast(&local_addr))
            {
                /* Join multicast group. */
#if FNET_CFG_IP4
                if(local_addr.sa_family == AF_INET)
                {
                    struct fnet_ip_mreq mreq; /* Multicast group information.*/

                    mreq.imr_multiaddr.s_addr = ((struct fnet_sockaddr_in *)&local_addr)->sin_addr.s_addr;
                    mreq.imr_interface = local_addr.sa_scope_id; /* If 0 = Default Interface.*/

                    /* Join multicast group. */
                    if(fnet_socket_setopt(bench_srv_if->socket_listen, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == FNET_ERR)
                    {
                        FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_SOCKET_MULTICAST);
                        goto ERROR_2;
                    }
                }
#endif
#if FNET_CFG_IP6
                if(local_addr.sa_family == AF_INET6)
                {
                    struct fnet_ipv6_mreq mreq6; /* Multicast group information.*/

                    FNET_IP6_ADDR_COPY(&((struct fnet_sockaddr_in6 *)&local_addr)->sin6_addr.s6_addr, &mreq6.ipv6imr_multiaddr.s6_addr);
                    mreq6.ipv6imr_interface = local_addr.sa_scope_id;

                    /* Join multicast group. */
                    if(fnet_socket_setopt(bench_srv_if->socket_listen, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof(mreq6)) == FNET_ERR)
                    {
                        FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_SOCKET_MULTICAST);
                        goto ERROR_2;
                    }
                }
#endif
            }
            break;
        default:
            goto ERROR_2;
    }

    bench_srv_if->service_descriptor = fnet_service_register(_fnet_bench_srv_poll, (void *) bench_srv_if);
    if(bench_srv_if->service_descriptor == 0)
    {
        FNET_DEBUG_BENCH_SRV(FNET_BENCH_SRV_ERR_SERVICE);
        goto ERROR_2;
    }

    bench_srv_if->is_enabled = FNET_TRUE;
    bench_srv_if->state = FNET_BENCH_SRV_STATE_LISTENING;

    fnet_service_mutex_unlock();

    return (fnet_bench_srv_desc_t)bench_srv_if;

ERROR_2:
    fnet_socket_close(bench_srv_if->socket_listen);
ERROR_1:
    fnet_service_mutex_unlock();
    return 0;
}

/************************************************************************
* DESCRIPTION: Releases the Benchmark server service.
************************************************************************/
void fnet_bench_srv_release(fnet_bench_srv_desc_t desc)
{
    fnet_bench_srv_if_t     *bench_srv_if = (fnet_bench_srv_if_t *)desc;

    if(bench_srv_if && bench_srv_if->is_enabled)
    {
        fnet_service_mutex_lock();

        if(bench_srv_if->state == FNET_BENCH_SRV_STATE_RX)         /* Benchmark session is active.*/
        {
            _fnet_bench_srv_close_session(bench_srv_if);
        }

        fnet_socket_close(bench_srv_if->socket_listen);

        bench_srv_if->state = FNET_BENCH_SRV_STATE_DISABLED;
        bench_srv_if->is_enabled = FNET_FALSE;

        fnet_service_unregister(bench_srv_if->service_descriptor); /* Delete service. */

        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Registers the "session begin" event handler callback.
************************************************************************/
void fnet_bench_srv_set_callback_session_begin (fnet_bench_srv_desc_t desc, fnet_bench_srv_callback_session_begin_t callback_session_begin, void *cookie)
{
    fnet_bench_srv_if_t     *bench_srv_if = (fnet_bench_srv_if_t *)desc;

    if(bench_srv_if)
    {
        fnet_service_mutex_lock();
        bench_srv_if->callback_session_begin = callback_session_begin;
        bench_srv_if->callback_session_begin_cookie = cookie;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Registers the "session end" event handler callback.
************************************************************************/
void fnet_bench_srv_set_callback_session_end (fnet_bench_srv_desc_t desc, fnet_bench_srv_callback_session_end_t callback_session_end, void *cookie)
{
    fnet_bench_srv_if_t     *bench_srv_if = (fnet_bench_srv_if_t *)desc;

    if(bench_srv_if)
    {
        fnet_service_mutex_lock();
        bench_srv_if->callback_session_end = callback_session_end;
        bench_srv_if->callback_session_end_cookie = cookie;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Benchmark server session close.
************************************************************************/
static void _fnet_bench_srv_close_session(fnet_bench_srv_if_t    *bench_srv_if)
{
    if(bench_srv_if->callback_session_end) /* Inform a user application about the session end */
    {
        bench_srv_if->bench_srv_result.time_ms = fnet_timer_get_ms() - bench_srv_if->time_begin;
        bench_srv_if->callback_session_end((fnet_bench_srv_desc_t)bench_srv_if, &bench_srv_if->bench_srv_result, bench_srv_if->callback_session_end_cookie);
    }
    if(bench_srv_if->socket_foreign)
    {
        fnet_socket_close(bench_srv_if->socket_foreign);
        bench_srv_if->socket_foreign = 0;
    }

    bench_srv_if->bench_srv_result.megabytes = 0;
    bench_srv_if->bench_srv_result.bytes = 0;
}

/************************************************************************
* DESCRIPTION: Benchmark server state machine.
************************************************************************/
static void _fnet_bench_srv_poll( void *fnet_bench_srv_if_p )
{
    fnet_bench_srv_if_t         *bench_srv_if = (fnet_bench_srv_if_t *)fnet_bench_srv_if_p;
    fnet_size_t                 addr_len;
    fnet_ssize_t                received;

    if(bench_srv_if)
    {
        switch(bench_srv_if->state)
        {
            case FNET_BENCH_SRV_STATE_LISTENING:     /* Benchmark server is listening for incoming connection.*/
            {
                fnet_bool_t     is_session_begin = FNET_FALSE;

                if(bench_srv_if->type == SOCK_STREAM) /* TCP */
                {
                    /*Accept*/
                    addr_len = sizeof(bench_srv_if->address);
                    bench_srv_if->socket_foreign = fnet_socket_accept(bench_srv_if->socket_listen, &bench_srv_if->address, &addr_len);
                    if(bench_srv_if->socket_foreign)
                    {
                        is_session_begin = FNET_TRUE;
                    }
                }
                else /* UDP */
                {
                    /* Accept */
                    addr_len = sizeof(bench_srv_if->address);
                    received = fnet_socket_recvfrom(bench_srv_if->socket_listen, fnet_bench_srv_buffer, sizeof(fnet_bench_srv_buffer), 0,
                                                    &bench_srv_if->address, &addr_len);

                    if(received > FNET_BENCH_SRV_UDP_END_LENGTH) /* First UDP datagram is mark of session begin.*/
                    {
                        is_session_begin = FNET_TRUE;
                    }
                }

                if(is_session_begin == FNET_TRUE)
                {
                    if(bench_srv_if->callback_session_begin) /* Inform a user application about the session begin */
                    {
                        bench_srv_if->callback_session_begin((fnet_bench_srv_desc_t)bench_srv_if, bench_srv_if->address, bench_srv_if->callback_session_begin_cookie);
                    }

                    bench_srv_if->time_begin = fnet_timer_get_ms();
                    bench_srv_if->time_last = bench_srv_if->time_begin;

                    bench_srv_if->state = FNET_BENCH_SRV_STATE_RX;
                }
            }
            break;
            case FNET_BENCH_SRV_STATE_RX:            /* Benchmark server is receiving.*/
            {
                fnet_bool_t     is_session_end = FNET_FALSE;
                fnet_time_t     time_current = fnet_timer_get_ms();

                if(bench_srv_if->type == SOCK_STREAM) /* TCP */
                {
                    received = fnet_socket_recv(bench_srv_if->socket_foreign, fnet_bench_srv_buffer, sizeof(fnet_bench_srv_buffer), 0);
                    if(received == FNET_ERR)
                    {
                        is_session_end = FNET_TRUE;
                    }
                }
                else /* UDP */
                {
                    addr_len = sizeof(bench_srv_if->address);
                    received = fnet_socket_recvfrom(bench_srv_if->socket_listen, fnet_bench_srv_buffer, sizeof(fnet_bench_srv_buffer), 0,
                                                    &bench_srv_if->address, &addr_len);

                    if((received == FNET_ERR) || (received == FNET_BENCH_SRV_UDP_END_LENGTH)) /* End of session. */
                    {
                        is_session_end = FNET_TRUE;
                    }
                }

                if(received > 0)
                {
                    bench_srv_if->bench_srv_result.bytes += received;
                    if(bench_srv_if->bench_srv_result.bytes >= 1000000)
                    {
                        bench_srv_if->bench_srv_result.megabytes ++;
                        bench_srv_if->bench_srv_result.bytes -= 1000000;
                    }
                    bench_srv_if->time_last = time_current;
                }

                /* Session End */
                if ((is_session_end == FNET_TRUE) ||
                    ((time_current - bench_srv_if->time_last) > FNET_BENCH_SRV_SESSION_TIMEOUT_MS))   /* Session timeout */

                {
                    _fnet_bench_srv_close_session(bench_srv_if);

                    bench_srv_if->state = FNET_BENCH_SRV_STATE_LISTENING;
                    break;
                }
            }
            break;
            default:
                break;
        }
    }
}

/************************************************************************
* DESCRIPTION: Detects if the Benchmark server service is enabled or disabled.
************************************************************************/
fnet_bool_t fnet_bench_srv_is_enabled(fnet_bench_srv_desc_t desc)
{
    struct fnet_bench_srv_if    *bench_srv_if = (struct fnet_bench_srv_if *)desc;
    fnet_bool_t                 result;

    if(bench_srv_if)
    {
        result = bench_srv_if->is_enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

#endif /* FNET_CFG_BENCH_SRV */

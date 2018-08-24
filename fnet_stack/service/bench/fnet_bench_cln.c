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
*  FNET benchmark client implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_BENCH_CLN

#if FNET_CFG_DEBUG_BENCH_CLN && FNET_CFG_DEBUG
    #define FNET_DEBUG_BENCH_CLN   FNET_DEBUG
#else
    #define FNET_DEBUG_BENCH_CLN(...) do{}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/

#define FNET_BENCH_CLN_UDP_END_LENGTH          (1)  /* End of UDP session mark */
#define FNET_BENCH_CLN_UDP_END_ITERATION       (5)

/* Error strings.*/
#define FNET_BENCH_CLN_ERR_PARAMS             "[BENCH cln] Wrong input parameters"
#define FNET_BENCH_CLN_ERR_SOCKET_CREATION    "[BENCH cln] Socket creation error"
#define FNET_BENCH_CLN_ERR_SOCKET_CONNECT     "[BENCH cln] Socket connect error"
#define FNET_BENCH_CLN_ERR_SOCKET_OPTION      "[BENCH cln] Socket option error"
#define FNET_BENCH_CLN_ERR_SOCKET_MULTICAST   "[BENCH cln] Socket multicast join error"
#define FNET_BENCH_CLN_ERR_SERVICE            "[BENCH cln] Service registration error."
#define FNET_BENCH_CLN_ERR_IS_INITIALIZED     "[BENCH cln] Is already initialized."

typedef enum
{
    FNET_BENCH_CLN_STATE_DISABLED = 0,  /* Benchmark client is not initialized.*/
    FNET_BENCH_CLN_STATE_CONNECTING,    /* Benchmark client is connecting (TCP).*/
    FNET_BENCH_CLN_STATE_TX,            /* Benchmark client is transmitting.*/
    FNET_BENCH_CLN_STATE_CLOSE,         /* Benchmark client is closing session.*/
} fnet_bench_cln_state_t;

/************************************************************************
*    Benchmark client interface structure.
*************************************************************************/
typedef struct fnet_bench_cln_if
{
    fnet_bool_t                             is_enabled;
    fnet_service_desc_t                     service_descriptor;
    fnet_bench_cln_state_t                  state;
    fnet_socket_type_t                      type;
    struct fnet_sockaddr                    address_foreign;
    fnet_socket_t                           socket_foreign;     /* Foreign socket.*/
    fnet_size_t                             message_size;       /* Size of one message to send. */
    fnet_index_t                            message_number;     /* Number of messages to send. */
    fnet_size_t                             message_offset;     /* Partial send (TCP) */
    fnet_bench_cln_callback_session_end_t   callback;
    void                                    *cookie;
    fnet_time_t                             time_begin;
    fnet_bench_cln_result_t                 bench_cln_result;
} fnet_bench_cln_if_t;

/* Internal buffer */
static fnet_uint8_t fnet_bench_cln_buffer[FNET_CFG_BENCH_CLN_BUFFER_SIZE];

/* The Benchmark client interface list*/
static  fnet_bench_cln_if_t fnet_bench_cln_if_list[FNET_CFG_BENCH_CLN];

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void _fnet_bench_cln_poll(void *fnet_bench_cln_if_p);

/************************************************************************
* DESCRIPTION: Initializes the Benchmark client service.
************************************************************************/
fnet_bench_srv_desc_t fnet_bench_cln_init( fnet_bench_cln_params_t *params )
{
    fnet_index_t                i;
    fnet_bench_cln_if_t         *bench_cln_if = FNET_NULL;
    fnet_size_t                 bufsize_option = FNET_CFG_BENCH_CLN_BUFFER_SIZE;

    fnet_service_mutex_lock();

    if((params == 0) ||
       !((params->type == SOCK_STREAM) || (params->type == SOCK_DGRAM)) ||
       (params->message_number == 0) || (params->callback == 0) ||
       (params->message_size == 0) || (params->message_size > sizeof(fnet_bench_cln_buffer)))
    {
        FNET_DEBUG_BENCH_CLN(FNET_BENCH_CLN_ERR_PARAMS);
        goto ERROR_1;
    }

    /* Try to find free benchmark client. */
    for(i = 0u; i < FNET_CFG_BENCH_CLN; i++)
    {
        if(fnet_bench_cln_if_list[i].is_enabled == FNET_FALSE)
        {
            bench_cln_if = &fnet_bench_cln_if_list[i];
            break;
        }
    }

    /* Is benchmark client is already initialized. */
    if(bench_cln_if == 0)
    {
        FNET_DEBUG_BENCH_CLN(FNET_BENCH_CLN_ERR_IS_INITIALIZED);
        goto ERROR_1;
    }

    /* Clear all parameters.*/
    fnet_memset_zero(bench_cln_if, sizeof(*bench_cln_if));

    /* Set parameters */
    fnet_memcpy(&bench_cln_if->address_foreign, &params->address, sizeof(bench_cln_if->address_foreign));
    if(bench_cln_if->address_foreign.sa_port == 0u)
    {
        bench_cln_if->address_foreign.sa_port = FNET_CFG_BENCH_CLN_PORT;
    }

    bench_cln_if->type = params->type;
    bench_cln_if->message_size = params->message_size;
    bench_cln_if->message_number = params->message_number;
    bench_cln_if->callback = params->callback;
    bench_cln_if->cookie = params->cookie;

    /* Create socket */
    bench_cln_if->socket_foreign = fnet_socket(bench_cln_if->address_foreign.sa_family,  bench_cln_if->type, 0u);
    if(bench_cln_if->socket_foreign == FNET_NULL)
    {
        FNET_DEBUG_BENCH_CLN(FNET_BENCH_CLN_ERR_SOCKET_CREATION);
        goto ERROR_1;
    }

    /* Set socket tx buffer size .*/
    if(fnet_socket_setopt(bench_cln_if->socket_foreign, SOL_SOCKET, SO_SNDBUF, &bufsize_option, sizeof(bufsize_option)) == FNET_ERR)
    {
        FNET_DEBUG_BENCH_CLN(FNET_BENCH_CLN_ERR_SOCKET_OPTION);
        goto ERROR_2;
    }

    /* Connect */
    if(fnet_socket_connect(bench_cln_if->socket_foreign, &bench_cln_if->address_foreign, sizeof(bench_cln_if->address_foreign)) == FNET_ERR)
    {
        FNET_DEBUG_BENCH_CLN(FNET_BENCH_CLN_ERR_SOCKET_CONNECT);
        goto ERROR_2;
    }

    bench_cln_if->service_descriptor = fnet_service_register(_fnet_bench_cln_poll, (void *)bench_cln_if);
    if(bench_cln_if->service_descriptor == 0)
    {
        FNET_DEBUG_BENCH_CLN(FNET_BENCH_CLN_ERR_SERVICE);
        goto ERROR_2;
    }

    bench_cln_if->is_enabled = FNET_TRUE;

    if(bench_cln_if->type == SOCK_STREAM) /* TCP */
    {
        bench_cln_if->state = FNET_BENCH_CLN_STATE_CONNECTING;
    }
    else /* UDP */
    {
        bench_cln_if->time_begin = fnet_timer_get_ms();
        bench_cln_if->state = FNET_BENCH_CLN_STATE_TX;
    }

    fnet_service_mutex_unlock();
    return (fnet_bench_cln_desc_t)bench_cln_if;

ERROR_2:
    fnet_socket_close(bench_cln_if->socket_foreign);
ERROR_1:
    fnet_service_mutex_unlock();
    return FNET_NULL;
}

/************************************************************************
* DESCRIPTION: Releases the Benchmark client service.
************************************************************************/
void fnet_bench_cln_release(fnet_bench_cln_desc_t desc)
{
    fnet_bench_cln_if_t     *bench_cln_if = (fnet_bench_cln_if_t *)desc;

    if(bench_cln_if && bench_cln_if->is_enabled)
    {
        fnet_service_mutex_lock();

        if(bench_cln_if->type == SOCK_DGRAM) /* UDP */
        {
            fnet_uint32_t   bytes;
            fnet_index_t    i;

            /* Send UDP session End mark.*/
            for(i = 0; i < FNET_BENCH_CLN_UDP_END_ITERATION; i++)
            {
                /* Send END mark.*/
                fnet_socket_send(bench_cln_if->socket_foreign, &bytes, 1, 0);
            }
        }

        bench_cln_if->is_enabled = FNET_FALSE;

        fnet_service_unregister(bench_cln_if->service_descriptor); /* Delete service. */

        fnet_socket_close(bench_cln_if->socket_foreign);

        bench_cln_if->state = FNET_BENCH_CLN_STATE_DISABLED;

        /* Inform a user application about the session end */
        bench_cln_if->bench_cln_result.time_ms = fnet_timer_get_ms() - bench_cln_if->time_begin;
        bench_cln_if->callback(bench_cln_if, &bench_cln_if->bench_cln_result, bench_cln_if->cookie);

        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Benchmark client state machine.
************************************************************************/
static void _fnet_bench_cln_poll( void *fnet_bench_cln_if_p )
{
    fnet_bench_cln_if_t     *bench_cln_if = (fnet_bench_cln_if_t *)fnet_bench_cln_if_p;
    fnet_ssize_t            send_result;

    if(bench_cln_if)
    {
        switch(bench_cln_if->state)
        {
            case FNET_BENCH_CLN_STATE_CONNECTING:            /* Benchmark client is connecting.*/
            {
#if 0 /* Use socket state parameter */
                fnet_socket_state_t     connection_state;
                fnet_size_t             option_len;

                option_len = sizeof(connection_state);
                fnet_socket_getopt(bench_cln_if->socket_foreign, SOL_SOCKET, SO_STATE, &connection_state, &option_len);

                if(connection_state != SS_CONNECTING)
                {
                    bench_cln_if->time_begin = fnet_timer_get_ms();
                    bench_cln_if->state = FNET_BENCH_CLN_STATE_TX;
                }
#else /* Use socket poll */
                fnet_socket_poll_t socket_poll;
                socket_poll.s = bench_cln_if->socket_foreign;
                socket_poll.events = (fnet_socket_event_t)(FNET_SOCKET_EVENT_OUT | FNET_SOCKET_EVENT_ERR); /* Connection attempt successful or failed */

                if(fnet_socket_poll(&socket_poll, 1))
                {
                    bench_cln_if->time_begin = fnet_timer_get_ms();
                    if(socket_poll.events_occurred & FNET_SOCKET_EVENT_OUT) /* Connection successful */
                    {
                        bench_cln_if->state = FNET_BENCH_CLN_STATE_TX;
                    }
                    else if(socket_poll.events_occurred & FNET_SOCKET_EVENT_ERR) /* Connection failed */
                    {
                        fnet_bench_cln_release(bench_cln_if);
                    }
                }
#endif
            }
            break;
            case FNET_BENCH_CLN_STATE_TX:   /* Benchmark client is sending.*/
            {
                fnet_bool_t     is_session_end = FNET_FALSE;

                send_result = fnet_socket_send(bench_cln_if->socket_foreign, &fnet_bench_cln_buffer[bench_cln_if->message_offset], (bench_cln_if->message_size - bench_cln_if->message_offset), 0);

                if (send_result == FNET_ERR)
                {
                    is_session_end = FNET_TRUE;
                }
                else if(send_result)
                {
                    bench_cln_if->bench_cln_result.bytes += send_result;
                    if(bench_cln_if->bench_cln_result.bytes >= 1000000)
                    {
                        bench_cln_if->bench_cln_result.megabytes ++;
                        bench_cln_if->bench_cln_result.bytes -= 1000000;
                    }
                    bench_cln_if->message_offset += send_result;

                    if(bench_cln_if->message_offset >= bench_cln_if->message_size)
                    {
                        bench_cln_if->message_offset = 0;
                        bench_cln_if->message_number--;

                        if(bench_cln_if->message_number == 0)
                        {
                            is_session_end = FNET_TRUE;
                        }
                    }
                }
                else /* 0 */
                {}

                if(is_session_end == FNET_TRUE)
                {
                    fnet_bench_cln_release(bench_cln_if);
                }
            }
            break;
            default:
                break;
        }
    }
}

/************************************************************************
* DESCRIPTION: Detects if the Benchmark client service is enabled or disabled.
************************************************************************/
fnet_bool_t fnet_bench_cln_is_enabled(fnet_bench_cln_desc_t desc)
{
    struct fnet_bench_cln_if    *bench_cln_if = (struct fnet_bench_cln_if *)desc;
    fnet_bool_t                 result;

    if(bench_cln_if)
    {
        result = bench_cln_if->is_enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}


#endif /* FNET_CFG_BENCH_CLN */

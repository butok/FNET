/**************************************************************************
*
* Copyright 2008-2016 by Andrey Butok. FNET Community.
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
*  FNET Telnet Server implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_TELNET

/************************************************************************
*     Definitions
************************************************************************/

#if FNET_CFG_DEBUG_TELNET && FNET_CFG_DEBUG
    #define FNET_DEBUG_TELNET   FNET_DEBUG
#else
    #define FNET_DEBUG_TELNET(...)  do{}while(0)
#endif

#define FNET_TELNET_WAIT_SEND_MS        (2000u)  /* ms*/

#define FNET_TELNET_TX_BUFFER_SIZE      FNET_CFG_TELNET_SOCKET_BUF_SIZE
#define FNET_TELNET_RX_BUFFER_SIZE      (10u)

#if (FNET_TELNET_TX_BUFFER_SIZE  < 5u)   /* Check minimum value for TX application/stream buffer.*/
    error "FNET_TELNET_TX_BUFFER_SIZE must be > 4"
#endif

/* Keepalive probe retransmit limit.*/
#define FNET_TELNET_TCP_KEEPCNT         (2)

/* Keepalive retransmit interval.*/
#define FNET_TELNET_TCP_KEEPINTVL       (4) /*sec*/

/* Time between keepalive probes.*/
#define FNET_TELNET_TCP_KEEPIDLE        (7) /*sec*/

/* RFC:
* All TELNET commands consist of at least a two byte sequence: the
* "Interpret as Command" (IAC) escape character followed by the code
* for the command. The commands dealing with option negotiation are
* three byte sequences, the third byte being the code for the option
* referenced.
*/
/* RFC:
* Since the NVT is what is left when no options are enabled, the DON’T and
* WON’T responses are guaranteed to leave the connection in a state
* which both ends can handle. Thus, all hosts may implement their
* TELNET processes to be totally unaware of options that are not
* supported, simply returning a rejection to (i.e., refusing) any
* option request that cannot be understood.
*
* The Network Virtual Terminal (NVT) is a bi-directional character
* device.
*/

#define FNET_TELNET_CMD_IAC   ((fnet_uint8_t)255) /* "Interpret as Command" (IAC) escape character followed by the code for the command. */
#define FNET_TELNET_CMD_WILL  ((fnet_uint8_t)251) /* Indicates the desire to begin performing, or confirmation that
                                     * you are now performing, the indicated option.*/
#define FNET_TELNET_CMD_WONT  ((fnet_uint8_t)252) /* Indicates the refusal to perform, or continue performing, the
                                     * indicated option. */
#define FNET_TELNET_CMD_DO    ((fnet_uint8_t)253) /* Indicates the request that the other party perform, or
                                     * confirmation that you are expecting the other party to perform, the
                                     * indicated option. */
#define FNET_TELNET_CMD_DONT  ((fnet_uint8_t)254) /* Indicates the demand that the other party stop performing,
                                     * or confirmation that you are no longer expecting the other party
                                     * to perform, the indicated option. */

/*****************************************************************************
 * Telnet server states.
 ******************************************************************************/
typedef enum
{
    FNET_TELNET_STATE_DISABLED = 0,     /* Telnet server service is
                                         * not initialized.*/
    FNET_TELNET_STATE_LISTENING,        /* Telnet server is listening
                                         * for client socket.*/
    FNET_TELNET_STATE_RECEIVING,        /* Ready to receive data from a Telnet client. */
    FNET_TELNET_STATE_IAC,              /* Received IAC symbol. */
    FNET_TELNET_STATE_DONT,             /* Prepare to send DON'T. */
    FNET_TELNET_STATE_WONT,             /* Prepare to send WON'T. */
    FNET_TELNET_STATE_SKIP,             /* Ignore next received character.*/
    FNET_TELNET_STATE_CLOSING           /* Closing Telnet session.*/
} fnet_telnet_state_t;

/************************************************************************
*    Telnet interface control structure.
*************************************************************************/
struct fnet_telnet_session_if
{
    fnet_telnet_state_t         state;              /* Current state.*/
    fnet_socket_t               socket_foreign;     /* Foreign socket.*/
    fnet_uint8_t                tx_buffer[FNET_TELNET_TX_BUFFER_SIZE];  /* Transmit liner buffer. */
    fnet_index_t                tx_buffer_head_index;                   /* TX buffer index (write place).*/
    fnet_uint8_t                rx_buffer[FNET_TELNET_RX_BUFFER_SIZE];  /* RX circular buffer */
    fnet_uint8_t                *rx_buffer_head;    /* The RX circular buffer write pointer. */
    fnet_uint8_t                *rx_buffer_tail;    /* The RX circular buffer read pointer. */
    fnet_uint8_t                *rx_buffer_end;     /* Pointer to the end of the Rx circular buffer. */
    fnet_shell_desc_t           shell_descriptor;
    fnet_shell_params_t         shell_params;
    fnet_char_t                 cmd_line_buffer[FNET_CFG_TELNET_CMD_LINE_BUF_SIZE];
    struct fnet_serial_stream   stream;
};

/************************************************************************
*    Telnet interface control structure.
*************************************************************************/
struct fnet_telnet_if
{
    fnet_socket_t                   socket_listen;      /* Listening socket.*/
    fnet_service_desc_t             service_descriptor; /* Descriptor of polling service.*/
    fnet_bool_t                     is_enabled;
    fnet_size_t                     backlog;
    struct fnet_telnet_session_if   *session_active;
    struct fnet_telnet_session_if   session[FNET_CFG_TELNET_SESSION_MAX];
};

/* The Telnet interface list.*/
static struct fnet_telnet_if telnet_if_list[FNET_CFG_TELNET];

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void _fnet_telnet_send(struct fnet_telnet_session_if *session);
static void _fnet_telnet_tx_buffer_write(struct fnet_telnet_session_if *session, fnet_uint8_t data);
static fnet_size_t _fnet_telnet_tx_buffer_free_space(struct fnet_telnet_session_if *session);
static void _fnet_telnet_rx_buffer_write (struct fnet_telnet_session_if *session, fnet_uint8_t data);
static fnet_uint8_t _fnet_telnet_rx_buffer_read(struct fnet_telnet_session_if *session);
static fnet_size_t _fnet_telnet_rx_buffer_free_space(struct fnet_telnet_session_if *session);
static void _fnet_telnet_putchar(fnet_index_t id, fnet_char_t character);
static fnet_int32_t _fnet_telnet_getchar(fnet_index_t id);
static void _fnet_telnet_flush(fnet_index_t id);
static void _fnet_telnet_send_cmd(struct fnet_telnet_session_if *session, fnet_uint8_t command, fnet_uint8_t option);
static void _fnet_telnet_poll(void *telnet_if_p);

/************************************************************************
* Buffer functions.
************************************************************************/
/* Write to Tx liner buffer. */
/* It's posible to write FNET_TELNET_TX_BUFFER_SIZE-1 characters. */
static void _fnet_telnet_tx_buffer_write (struct fnet_telnet_session_if *session, fnet_uint8_t data)
{
    session->tx_buffer[session->tx_buffer_head_index] = data;
    session->tx_buffer_head_index++;
}

/* Free space in Tx Liner buffer. */
static fnet_size_t _fnet_telnet_tx_buffer_free_space(struct fnet_telnet_session_if *session)
{
    return(FNET_TELNET_TX_BUFFER_SIZE  - session->tx_buffer_head_index);
}

/* Write to Rx circular buffer. */
/* It's posible to write FNET_TELNET_RX_BUFFER_SIZE-1 characters. */
static void _fnet_telnet_rx_buffer_write (struct fnet_telnet_session_if *session, fnet_uint8_t data)
{
    *session->rx_buffer_head = data;
    if(++session->rx_buffer_head == session->rx_buffer_end)
    {
        session->rx_buffer_head = session->rx_buffer;
    }
}

/* Read from Rx circular buffer. */
static fnet_uint8_t _fnet_telnet_rx_buffer_read (struct fnet_telnet_session_if *session)
{
    fnet_uint8_t data = *session->rx_buffer_tail;
    if(++session->rx_buffer_tail == session->rx_buffer_end)
    {
        session->rx_buffer_tail = session->rx_buffer;
    }
    return data;
}

/* Free space in Rx circular buffer. */
static fnet_size_t _fnet_telnet_rx_buffer_free_space(struct fnet_telnet_session_if *session)
{
    fnet_int32_t  space = session->rx_buffer_tail - session->rx_buffer_head;

    if (space <= 0)
    {
        space += (fnet_int32_t)FNET_TELNET_RX_BUFFER_SIZE;
    }

    return (fnet_size_t)(space - 1);
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
static void _fnet_telnet_putchar(fnet_index_t id, fnet_char_t character)
{
    struct fnet_telnet_session_if *session = (struct fnet_telnet_session_if *)id;

    if(session->state != FNET_TELNET_STATE_CLOSING)
    {
        _fnet_telnet_tx_buffer_write(session, (fnet_uint8_t)character);

        if(_fnet_telnet_tx_buffer_free_space(session) < 1u) /* Buffer is full => flush. */
        {
            _fnet_telnet_send(session);
        }
    }
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_int32_t _fnet_telnet_getchar(fnet_index_t id)
{
    struct fnet_telnet_session_if *session = (struct fnet_telnet_session_if *)id;

    if(session->rx_buffer_tail != session->rx_buffer_head) /* If there is something. */
    {
        return (fnet_int32_t)_fnet_telnet_rx_buffer_read(session);
    }
    else
    {
        return FNET_ERR;
    }
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
static void _fnet_telnet_flush(fnet_index_t id)
{
    struct fnet_telnet_session_if *session = (struct fnet_telnet_session_if *)id;

    _fnet_telnet_send(session);
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
static void _fnet_telnet_send(struct fnet_telnet_session_if *session)
{
    fnet_ssize_t    res;
    fnet_index_t    tx_buffer_tail_index = 0u;
    fnet_time_t     timeout_ms = fnet_timer_get_ms();

    /* Send all data in the buffer.*/
    while((tx_buffer_tail_index != session->tx_buffer_head_index) && (session->state != FNET_TELNET_STATE_CLOSING))
    {
        if((res = fnet_socket_send(session->socket_foreign, &session->tx_buffer[tx_buffer_tail_index], (fnet_size_t)session->tx_buffer_head_index - tx_buffer_tail_index, 0u)) != FNET_ERR)
        {
            if(res) /* >0 */
            {
                /* Update buffer pointers. */
                tx_buffer_tail_index += (fnet_index_t)res;

                /* Reset timeout. */
                timeout_ms = fnet_timer_get_ms();
            }
            else if( (fnet_timer_get_ms() - timeout_ms) > FNET_TELNET_WAIT_SEND_MS ) /* Check timeout */
            {
                FNET_DEBUG_TELNET("TELNET:Send timeout.");
                break; /* Time-out. */
            }
            else
            {}
        }
        else /* Error.*/
        {
            FNET_DEBUG_TELNET("TELNET:Send error.");
            session->state = FNET_TELNET_STATE_CLOSING; /*=> CLOSING */
        }
    }

    /* Reset TX buffer index. */
    session->tx_buffer_head_index = 0u;
}

/************************************************************************
* DESCRIPTION: Wrie command to the TX buffer.
************************************************************************/
static void _fnet_telnet_send_cmd(struct fnet_telnet_session_if *session, fnet_uint8_t command, fnet_uint8_t option )
{
    _fnet_telnet_tx_buffer_write(session, (fnet_uint8_t)FNET_TELNET_CMD_IAC);
    _fnet_telnet_tx_buffer_write(session, command);
    _fnet_telnet_tx_buffer_write(session, option);

    /* Send the command.*/
    _fnet_telnet_send(session);

    FNET_DEBUG_TELNET("TELNET: Send option = %d", option);
}

/************************************************************************
* DESCRIPTION: Telnet server state machine.
************************************************************************/
static void _fnet_telnet_poll( void *telnet_if_p )
{
    struct fnet_sockaddr            foreign_addr;
    fnet_ssize_t                    res;
    struct fnet_telnet_if           *telnet = (struct fnet_telnet_if *)telnet_if_p;
    fnet_uint8_t                    rx_data[1];
    fnet_size_t                     len;
    fnet_index_t                    i;
    struct fnet_telnet_session_if   *session;

    for( i = 0u; i < FNET_CFG_TELNET_SESSION_MAX; i++)
    {
        session = &telnet->session[i];
        telnet->session_active = session;

        do
        {
            switch(session->state)
            {
                /*---- LISTENING ------------------------------------------------*/
                case FNET_TELNET_STATE_LISTENING:
                    len = sizeof(foreign_addr);
                    session->socket_foreign = fnet_socket_accept(telnet->socket_listen, (struct fnet_sockaddr *) &foreign_addr, &len);
                    if(session->socket_foreign)
                    {
#if FNET_CFG_DEBUG_TELNET && FNET_CFG_DEBUG
                        {
                            fnet_uint8_t ip_str[FNET_IP_ADDR_STR_SIZE];
                            fnet_inet_ntop(foreign_addr.sa_family, foreign_addr.sa_data, ip_str, sizeof(ip_str));
                            FNET_DEBUG_TELNET("\nTELNET: New connection: %s; Port: %d.", ip_str, fnet_ntohs(foreign_addr.sa_port));
                        }
#endif
                        /* Init Shell. */
                        session->shell_descriptor = fnet_shell_init(&session->shell_params);

                        if(session->shell_descriptor)
                        {
                            fnet_socket_listen(telnet->socket_listen, --telnet->backlog); /* Ignor other connections.*/

                            /* Reset TX timeout. */
                            session->state = FNET_TELNET_STATE_RECEIVING; /* => WAITING data */
                        }
                        else
                        {
                            FNET_DEBUG_TELNET("TELNET: Shell Service registration error.");
                            session->state = FNET_TELNET_STATE_CLOSING;   /*=> CLOSING */
                        }
                    }
                    break;
                /*---- NORMAL -----------------------------------------------*/
                case FNET_TELNET_STATE_RECEIVING:
                    if(_fnet_telnet_rx_buffer_free_space(session) > 0u)
                    {
                        res = fnet_socket_recv(session->socket_foreign, rx_data, 1u, 0u);
                        if(res == 1)
                        {
                            if(rx_data[0] == FNET_TELNET_CMD_IAC )
                            {
                                session->state = FNET_TELNET_STATE_IAC; /*=> Handle IAC */
                            }
                            else
                            {
                                _fnet_telnet_rx_buffer_write (session, rx_data[0]);
                            }
                        }
                        else if (res == FNET_ERR)
                        {
                            session->state = FNET_TELNET_STATE_CLOSING; /*=> CLOSING */
                        }
                        else
                        {}
                    }
                    break;
                /*---- IAC -----------------------------------------------*/
                case FNET_TELNET_STATE_IAC:
                    FNET_DEBUG_TELNET("TELNET: STATE_IAC");

                    if((res = fnet_socket_recv(session->socket_foreign, rx_data, 1u, 0u) ) != FNET_ERR)
                    {
                        if(res)
                        {
                            switch(rx_data[0])
                            {
                                case FNET_TELNET_CMD_WILL:
                                    session->state = FNET_TELNET_STATE_DONT;
                                    break;
                                case FNET_TELNET_CMD_DO:
                                    session->state = FNET_TELNET_STATE_WONT;
                                    break;
                                case FNET_TELNET_CMD_WONT:
                                case FNET_TELNET_CMD_DONT:
                                    session->state = FNET_TELNET_STATE_SKIP ;
                                    break;
                                case FNET_TELNET_CMD_IAC:
                                    /*
                                    the IAC need be doubled to be sent as data, and
                                    the other 255 codes may be passed transparently.
                                    */
                                    _fnet_telnet_rx_buffer_write (session, rx_data[0]);
                                    session->state = FNET_TELNET_STATE_RECEIVING;
                                    break;
                                default:
                                    session->state = FNET_TELNET_STATE_RECEIVING; /*=> Ignore commands */
                                    break;
                            }
                        }
                    }
                    else
                    {
                        session->state = FNET_TELNET_STATE_CLOSING; /*=> CLOSING */
                    }
                    break;
                /*---- DONT & WONT -----------------------------------------------*/
                case FNET_TELNET_STATE_DONT:
                case FNET_TELNET_STATE_WONT:
                {
                    fnet_uint8_t command;

                    if(session->state == FNET_TELNET_STATE_DONT)
                    {
                        FNET_DEBUG_TELNET("TELNET: STATE_DONT");
                        command = FNET_TELNET_CMD_DONT;
                    }
                    else
                    {
                        FNET_DEBUG_TELNET("TELNET: STATE_WONT");
                        command =  FNET_TELNET_CMD_WONT;
                    }

                    if(_fnet_telnet_tx_buffer_free_space(session) >= 3u)
                    {
                        res = fnet_socket_recv(session->socket_foreign, rx_data, 1u, 0u);

                        if(res == 1)
                        {
                            /* Send command. */
                            _fnet_telnet_send_cmd(session, command, rx_data[0]);
                            session->state = FNET_TELNET_STATE_RECEIVING;
                        }
                        else if (res == FNET_ERR)
                        {
                            session->state = FNET_TELNET_STATE_CLOSING; /*=> CLOSING */
                        }
                        else
                        {}
                    }
                }
                break;
                /*---- SKIP -----------------------------------------------*/
                case FNET_TELNET_STATE_SKIP:
                    FNET_DEBUG_TELNET("TELNET: STATE_SKIP");

                    res = fnet_socket_recv(session->socket_foreign, rx_data, 1u, 0u);
                    if(res == 1)
                    {
                        session->state = FNET_TELNET_STATE_RECEIVING;
                    }
                    else if (res == FNET_ERR)
                    {
                        session->state = FNET_TELNET_STATE_CLOSING; /*=> CLOSING */
                    }
                    else
                    {}

                    break;
                /*---- CLOSING --------------------------------------------*/
                case FNET_TELNET_STATE_CLOSING:
                    FNET_DEBUG_TELNET("TELNET: STATE_CLOSING");

                    if(session->shell_descriptor)
                    {
                        fnet_shell_release(session->shell_descriptor);
                        session->shell_descriptor = 0;
                    }

                    session->rx_buffer_head = session->rx_buffer;
                    session->rx_buffer_tail = session->rx_buffer;

                    fnet_socket_close(session->socket_foreign);
                    session->socket_foreign = FNET_NULL;

                    fnet_socket_listen(telnet->socket_listen, ++telnet->backlog); /* Allow connection.*/

                    session->state = FNET_TELNET_STATE_LISTENING; /*=> LISTENING */
                    break;
                default:
                    break;

            }

        }
        while(session->state == FNET_TELNET_STATE_CLOSING);
    }
}

/************************************************************************
* DESCRIPTION: Initialization of the Telnet server.
*************************************************************************/
fnet_telnet_desc_t fnet_telnet_init( fnet_telnet_params_t *params )
{
    struct fnet_sockaddr        local_addr;
    struct fnet_telnet_if       *telnet_if = 0;

    /* Socket options. */
    const struct fnet_linger    linger_option =
    {
        .l_onoff = FNET_TRUE, /* on */
        .l_linger = 4  /*seconds*/
    };
    const fnet_size_t       bufsize_option = FNET_CFG_TELNET_SOCKET_BUF_SIZE;
    const fnet_int32_t      keepalive_option = 1;
    const fnet_int32_t      keepcnt_option = FNET_TELNET_TCP_KEEPCNT;
    const fnet_int32_t      keepintvl_option = FNET_TELNET_TCP_KEEPINTVL;
    const fnet_int32_t      keepidle_option = FNET_TELNET_TCP_KEEPIDLE;
    fnet_index_t            i;

    fnet_service_mutex_lock();

    if(params == 0 )
    {
        FNET_DEBUG_TELNET("TELNET: Wrong init parameters.");
        goto ERROR_1;
    }

    /* Try to find free Telnet server descriptor. */
    for(i = 0u; i < FNET_CFG_TELNET; i++)
    {
        if(telnet_if_list[i].is_enabled == FNET_FALSE)
        {
            telnet_if = &telnet_if_list[i];
            break;
        }
    }

    /* No free Telnet server descriptor. */
    if(telnet_if == 0)
    {
        FNET_DEBUG_TELNET("TELNET: No free Telnet Server.");
        goto ERROR_1;
    }

    /* Reset all parameters*/
    fnet_memset(telnet_if, 0, sizeof(*telnet_if));

    fnet_memcpy(&local_addr, &params->address, sizeof(local_addr));

    if(local_addr.sa_port == 0u)
    {
        local_addr.sa_port = FNET_CFG_TELNET_PORT;  /* Aply the default port. */
    }

    if(local_addr.sa_family == AF_UNSPEC)
    {
        local_addr.sa_family = AF_SUPPORTED;   /* Assign supported families.*/
    }

    /* Create listen socket */
    telnet_if->socket_listen = fnet_socket(local_addr.sa_family, SOCK_STREAM, 0u);
    if(telnet_if->socket_listen == FNET_NULL)
    {
        FNET_DEBUG_TELNET("TELNET: Socket creation error.");
        goto ERROR_1;
    }

    if(fnet_socket_bind(telnet_if->socket_listen, (struct fnet_sockaddr *)(&local_addr), sizeof(local_addr)) == FNET_ERR)
    {
        FNET_DEBUG_TELNET("TELNET: Socket bind error.");
        goto ERROR_2;
    }

    /* Set socket options. */
    if( /* Setup linger option. */
        (fnet_socket_setopt (telnet_if->socket_listen, SOL_SOCKET, SO_LINGER, &linger_option, sizeof(linger_option)) == FNET_ERR) ||
        /* Set socket buffer size. */
        (fnet_socket_setopt(telnet_if->socket_listen, SOL_SOCKET, SO_RCVBUF, &bufsize_option, sizeof(bufsize_option)) == FNET_ERR) ||
        (fnet_socket_setopt(telnet_if->socket_listen, SOL_SOCKET, SO_SNDBUF, &bufsize_option, sizeof(bufsize_option)) == FNET_ERR) ||
        /* Enable keepalive_option option. */
        (fnet_socket_setopt (telnet_if->socket_listen, SOL_SOCKET, SO_KEEPALIVE, &keepalive_option, sizeof(keepalive_option)) == FNET_ERR) ||
        /* Keepalive probe retransmit limit. */
        (fnet_socket_setopt (telnet_if->socket_listen, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt_option, sizeof(keepcnt_option)) == FNET_ERR) ||
        /* Keepalive retransmit interval.*/
        (fnet_socket_setopt (telnet_if->socket_listen, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl_option, sizeof(keepintvl_option)) == FNET_ERR) ||
        /* Time between keepalive probes.*/
        (fnet_socket_setopt (telnet_if->socket_listen, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle_option, sizeof(keepidle_option)) == FNET_ERR)
    )
    {
        FNET_DEBUG_TELNET("TELNET: Socket setsockopt() error.");
        goto ERROR_2;
    }

    telnet_if->backlog = FNET_CFG_TELNET_SESSION_MAX;

    if(fnet_socket_listen(telnet_if->socket_listen, telnet_if->backlog) == FNET_ERR)
    {
        FNET_DEBUG_TELNET("TELNET: Socket listen error.");
        goto ERROR_2;
    }

    /* Register service. */
    telnet_if->service_descriptor = fnet_service_register(_fnet_telnet_poll, (void *) telnet_if);

    if(telnet_if->service_descriptor == 0)
    {
        FNET_DEBUG_TELNET("TELNET: Service registration error.");
        goto ERROR_2;
    }

    for(i = 0u; i < FNET_CFG_TELNET_SESSION_MAX; i++)
    {
        struct fnet_telnet_session_if   *session = &telnet_if->session[i];

        /* Reset buffer pointers. Move it to init state. */
        session->rx_buffer_head = session->rx_buffer;
        session->rx_buffer_tail = session->rx_buffer;
        session->rx_buffer_end = &session->rx_buffer[FNET_TELNET_RX_BUFFER_SIZE];

        /* Setup stream. */
        session->stream.id = (fnet_index_t)(session);
        session->stream.putchar = _fnet_telnet_putchar;
        session->stream.getchar = _fnet_telnet_getchar;
        session->stream.flush = _fnet_telnet_flush;

        /* Init shell. */
        session->shell_params.shell = params->shell;
        session->shell_params.cmd_line_buffer = session->cmd_line_buffer;
        session->shell_params.cmd_line_buffer_size = sizeof(session->cmd_line_buffer);
        session->shell_params.stream = &session->stream;
        session->shell_params.echo = FNET_CFG_TELNET_SHELL_ECHO ? FNET_TRUE : FNET_FALSE;

        session->state = FNET_TELNET_STATE_LISTENING;
    }

    telnet_if->is_enabled = FNET_TRUE;

    fnet_service_mutex_unlock();
    return (fnet_telnet_desc_t)telnet_if;

ERROR_2:
    fnet_socket_close(telnet_if->socket_listen);

ERROR_1:
    fnet_service_mutex_unlock();
    return 0;
}

/************************************************************************
* DESCRIPTION: Telnet server release.
************************************************************************/
void fnet_telnet_release(fnet_telnet_desc_t desc)
{
    struct fnet_telnet_if   *telnet_if = (struct fnet_telnet_if *) desc;
    fnet_index_t            i;

    fnet_service_mutex_lock();

    if(telnet_if && (telnet_if->is_enabled == FNET_TRUE))
    {
        for(i = 0u; i < FNET_CFG_TELNET_SESSION_MAX; i++)
        {
            struct fnet_telnet_session_if   *session = &telnet_if->session[i];

            fnet_socket_close(session->socket_foreign);

            if(session->shell_descriptor)
            {
                fnet_shell_release(session->shell_descriptor);
            }
        }
        fnet_socket_close(telnet_if->socket_listen);
        fnet_service_unregister(telnet_if->service_descriptor); /* Delete service.*/

        /* Reset all parameters.*/
        fnet_memset(telnet_if, 0, sizeof(*telnet_if));
    }

    fnet_service_mutex_unlock();
}

/************************************************************************
* DESCRIPTION: Close current Telnet server session.
************************************************************************/
void fnet_telnet_close_session(fnet_telnet_desc_t desc)
{
    struct fnet_telnet_if *telnet_if = (struct fnet_telnet_if *) desc;

    fnet_service_mutex_lock();

    if(telnet_if && (telnet_if->is_enabled == FNET_TRUE) && (telnet_if->session_active))
    {
        telnet_if->session_active->state = FNET_TELNET_STATE_CLOSING;
    }

    fnet_service_mutex_unlock();
}

/************************************************************************
* DESCRIPTION: This function returns FNET_TRUE if the Telnet server
*              is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_telnet_is_enabled(fnet_telnet_desc_t desc)
{
    struct fnet_telnet_if   *telnet_if = (struct fnet_telnet_if *) desc;
    fnet_bool_t             result;

    if(telnet_if)
    {
        result = telnet_if->is_enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

#endif /* FNET_CFG_TELNET */

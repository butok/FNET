/**************************************************************************
*
* Copyright 2011-2018 by Andrey Butok. FNET Community.
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
*  PING implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_PING

#include "stack/fnet_icmp4.h"

#if FNET_CFG_DEBUG_PING && FNET_CFG_DEBUG
    #define FNET_DEBUG_PING   FNET_DEBUG
#else
    #define FNET_DEBUG_PING(...)    do{}while(0)
#endif

#if FNET_CFG_RAW == 0
    #error "FNET_CFG_RAW must be enabled for PING"
#endif

/************************************************************************
*     Definitions
*************************************************************************/
#define FNET_PING_ERR_PARAMS            "ERROR: Wrong input parameters."
#define FNET_PING_ERR_SOCKET_CREATION   "ERROR: Socket creation error."
#define FNET_PING_ERR_SOCKET_CONNECT    "ERROR: Socket Error during connect."
#define FNET_PING_ERR_SERVICE           "ERROR: Service registration is failed."
#define FNET_PING_ERR_IS_INITIALIZED    "ERROR: PING is already initialized."
#define FNET_PING_ERR_GETSOCKNAME       "ERROR: Socket getsockname error."

#define FNET_PING_BUFFER_SIZE   (sizeof(fnet_icmp4_echo_header_t) + FNET_CFG_PING_PACKET_MAX)

/*******************************************************************************
 * PING service states.
 * Used mainly for debugging purposes.
 ******************************************************************************/
typedef enum
{
    FNET_PING_STATE_DISABLED = 0,   /**< @brief The PING service is not initialized or is released.
                                     */
    FNET_PING_STATE_SENDING_REQUEST,/**< @brief The PING service is going to send echo request.
                                     */
    FNET_PING_STATE_WAITING_REPLY,  /**< @brief The PING service is waiting for echo reply.
                                     */
    FNET_PING_STATE_WAITING_TIMEOUT /**< @brief The PING service is waying a timeout till next request.
                                     */

} fnet_ping_state_t;

/************************************************************************
*    PING service interface structure.
*************************************************************************/
typedef struct
{
    fnet_socket_t           socket_foreign;                 /* Foreign socket.*/
    fnet_address_family_t   family;
    fnet_uint16_t           sequence_number;
    fnet_service_desc_t     service_descriptor;
    fnet_ping_state_t       state;                          /* Current state. */
    fnet_ping_callback_t    callback;                       /* Callback function. */
    void                    *callback_cookie;               /* Callback-handler specific parameter. */
    fnet_uint8_t            buffer[FNET_PING_BUFFER_SIZE];  /* Message buffer. */
    fnet_time_t             timeout_ms;                     /* Timeout value in ms, that ping request waits for reply.*/
    fnet_time_t             send_time_ms;                   /* Last send time, used for timeout detection. */
    fnet_index_t            packet_count;                   /* Number of packets to be sent.*/
    fnet_size_t             packet_size;
    fnet_uint8_t            pattern;
    struct fnet_sockaddr    target_addr;
}
fnet_ping_if_t;

static void _fnet_ping_poll(void *fnet_ping_if_p);

/* PING interface structure */
static fnet_ping_if_t fnet_ping_if;

/************************************************************************
* DESCRIPTION: Initializes PING service.
************************************************************************/
fnet_return_t fnet_ping_init( fnet_ping_params_t *params )
{
    const fnet_size_t bufsize_option = FNET_PING_BUFFER_SIZE;

    fnet_service_mutex_lock();

    /* Check input parameters. */
    if((params == 0) || (params->packet_count == 0u) || (fnet_socket_addr_is_unspecified(&params->target_addr)))
    {
        FNET_DEBUG_PING(FNET_PING_ERR_PARAMS);
        goto ERROR;
    }

    /* Check if PING service is free.*/
    if(fnet_ping_if.state != FNET_PING_STATE_DISABLED)
    {
        FNET_DEBUG_PING(FNET_PING_ERR_IS_INITIALIZED);
        goto ERROR;
    }

    /* Save input parmeters.*/
    fnet_ping_if.callback = params->callback;
    fnet_ping_if.callback_cookie = params->cookie;
    fnet_ping_if.timeout_ms = params->timeout;
    if(fnet_ping_if.timeout_ms == 0u)
    {
        fnet_ping_if.timeout_ms = 1u;
    }
    fnet_ping_if.family = params->target_addr.sa_family;
    fnet_ping_if.packet_count = params->packet_count;
    fnet_ping_if.pattern = params->pattern;
    fnet_ping_if.packet_size = params->packet_size;
    if(fnet_ping_if.packet_size > FNET_CFG_PING_PACKET_MAX)
    {
        fnet_ping_if.packet_size = FNET_CFG_PING_PACKET_MAX;
    }

    fnet_memcpy(&fnet_ping_if.target_addr, &params->target_addr, sizeof(fnet_ping_if.target_addr));

    /* Create socket */
    fnet_ping_if.socket_foreign = fnet_socket(fnet_ping_if.family, SOCK_RAW, (fnet_uint32_t)((params->target_addr.sa_family == AF_INET) ? IPPROTO_ICMP : IPPROTO_ICMPV6));
    if(fnet_ping_if.socket_foreign == FNET_NULL)
    {
        FNET_DEBUG_PING(FNET_PING_ERR_SOCKET_CREATION);
        goto ERROR;
    }

    /* Set Socket options. */
#if FNET_CFG_IP4
    if(fnet_ping_if.family == AF_INET)
    {
        fnet_socket_setopt(fnet_ping_if.socket_foreign, IPPROTO_IP, IP_TTL, &params->ttl, sizeof(params->ttl));
    }
#endif

#if FNET_CFG_IP6
    if(fnet_ping_if.family == AF_INET6)
    {
        fnet_socket_setopt(fnet_ping_if.socket_foreign, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &params->ttl, sizeof(params->ttl));
    }
#endif

    fnet_socket_setopt(fnet_ping_if.socket_foreign, SOL_SOCKET, SO_RCVBUF, &bufsize_option, sizeof(bufsize_option));
    fnet_socket_setopt(fnet_ping_if.socket_foreign, SOL_SOCKET, SO_SNDBUF, &bufsize_option, sizeof(bufsize_option));

    /* Register PING service. */
    fnet_ping_if.service_descriptor = fnet_service_register(_fnet_ping_poll, (void *) &fnet_ping_if);
    if(fnet_ping_if.service_descriptor == 0)
    {
        FNET_DEBUG_PING(FNET_PING_ERR_SERVICE);
        goto ERROR_1;
    }

    fnet_ping_if.state = FNET_PING_STATE_SENDING_REQUEST;

    fnet_service_mutex_unlock();
    return FNET_OK;

ERROR_1:
    fnet_socket_close(fnet_ping_if.socket_foreign);

ERROR:
    fnet_service_mutex_unlock();
    return FNET_ERR;
}

/************************************************************************
* DESCRIPTION: PING service state machine.
************************************************************************/
static void _fnet_ping_poll(void *fnet_ping_if_p)
{
    fnet_ssize_t            received;
    fnet_icmp4_echo_header_t *hdr;
    fnet_ping_if_t          *ping_if = (fnet_ping_if_t *)fnet_ping_if_p;
    struct fnet_sockaddr    addr;
    fnet_size_t             addr_len = sizeof(addr);

    switch(ping_if->state)
    {
        /*===================================*/
        case FNET_PING_STATE_SENDING_REQUEST:
            /* Build message.*/
            hdr = (fnet_icmp4_echo_header_t *)&fnet_ping_if.buffer[0];

            /* Fill ICMP Echo request header.*/
            fnet_memset_zero(hdr, sizeof(*hdr));
            hdr->header.type = (fnet_uint8_t)((fnet_ping_if.family == AF_INET) ? FNET_ICMP4_ECHO : FNET_ICMP6_TYPE_ECHO_REQ);
            hdr->identifier = FNET_CFG_PING_IDENTIFIER;
            fnet_ping_if.sequence_number++;
            hdr->sequence_number = fnet_htons(fnet_ping_if.sequence_number);

            /* Fill payload data by pattern.*/
            fnet_memset(&fnet_ping_if.buffer[sizeof(*hdr)], ping_if->pattern, ping_if->packet_size);

            /* Checksum.*/
#if FNET_CFG_IP4
            if(ping_if->family == AF_INET)
            {
                hdr->header.checksum = fnet_checksum(&fnet_ping_if.buffer[0], (sizeof(*hdr) + ping_if->packet_size));
            }
            else
#endif
#if FNET_CFG_IP6
                if(ping_if->family == AF_INET6)
                {
                    const fnet_ip6_addr_t   *src_ip = fnet_ip6_select_src_addr((fnet_ip6_addr_t *)ping_if->target_addr.sa_data);

                    if(src_ip) /* paranoic check.*/
                    {
                        hdr->header.checksum = fnet_checksum_pseudo(&fnet_ping_if.buffer[0],
                                               (fnet_uint16_t)(sizeof(*hdr) + ping_if->packet_size),
                                               FNET_HTONS((fnet_uint16_t)IPPROTO_ICMPV6),
                                               (const fnet_uint8_t *)src_ip,
                                               ping_if->target_addr.sa_data,
                                               sizeof(fnet_ip6_addr_t));
                    }
                }
                else
#endif
                {}

            /* Send request.*/
            fnet_socket_sendto(fnet_ping_if.socket_foreign, &fnet_ping_if.buffer[0], (sizeof(*hdr) + ping_if->packet_size), 0u,  &ping_if->target_addr, sizeof(ping_if->target_addr));
            ping_if->packet_count--;

            fnet_ping_if.send_time_ms = fnet_timer_get_ms();

            ping_if->state = FNET_PING_STATE_WAITING_REPLY;
            break;
        /*===================================*/
        case  FNET_PING_STATE_WAITING_REPLY:
            /* Receive data */

            received = fnet_socket_recvfrom(ping_if->socket_foreign, &ping_if->buffer[0], FNET_PING_BUFFER_SIZE, 0u, &addr, &addr_len );

            if(received > 0 )
            {
                fnet_uint16_t  checksum = 0u;

                hdr = (fnet_icmp4_echo_header_t *)(ping_if->buffer);

                /* Check checksum.*/
#if FNET_CFG_IP4
                if(ping_if->family == AF_INET)
                {
                    checksum = fnet_checksum(&fnet_ping_if.buffer[0], (fnet_size_t)received);
                }
                else
#endif
#if 0 /* #if FNET_CFG_IP6  */ /* TBD case to receive from multicast address ff02::1*/
                    if(ping_if->family == AF_INET6)
                    {
                        checksum = fnet_checksum_pseudo(&fnet_ping_if.buffer[0],
                                                        (fnet_uint16_t)(received),
                                                        IPPROTO_ICMPV6,
                                                        ping_if->local_addr.sa_data,
                                                        ping_if->target_addr.sa_data,
                                                        sizeof(fnet_ip6_addr_t));
                    }
                    else
#endif
                    {}

                /* Check header.*/
                if( checksum
                    || (hdr->header.type != ((addr.sa_family == AF_INET) ? FNET_ICMP4_ECHOREPLY : FNET_ICMP6_TYPE_ECHO_REPLY))
                    || (hdr->identifier != FNET_CFG_PING_IDENTIFIER)
                    || (hdr->sequence_number != fnet_htons(ping_if->sequence_number)) )
                {
                    goto NO_DATA;
                }

                /* Call handler callback.*/
                if(ping_if->callback)
                {
                    ping_if->callback(FNET_ERR_OK, ping_if->packet_count, &addr, ping_if->callback_cookie);
                }

                if(ping_if->packet_count)
                {
                    ping_if->state = FNET_PING_STATE_WAITING_TIMEOUT;
                }
                else
                {
                    fnet_ping_release();
                }
            }
            else if(received == FNET_ERR)
            {
                /* Call handler callback.*/
                if(ping_if->callback)
                {
                    fnet_error_t    sock_err ;
                    fnet_size_t     option_len;

                    /* Get socket error.*/
                    option_len = sizeof(sock_err);
                    fnet_socket_getopt(ping_if->socket_foreign, SOL_SOCKET, SO_ERROR, &sock_err, &option_len);

                    ping_if->callback(sock_err, ping_if->packet_count, FNET_NULL, ping_if->callback_cookie);
                }

                if(ping_if->packet_count)
                {
                    ping_if->state = FNET_PING_STATE_WAITING_TIMEOUT;
                }
                else
                {
                    fnet_ping_release();
                }
            }
            else /* No data. Check timeout */
            {
            NO_DATA:
                if((fnet_timer_get_ms() - fnet_ping_if.send_time_ms) > fnet_ping_if.timeout_ms)
                {
                    /* Call handler callback.*/
                    if(ping_if->callback)
                    {
                        ping_if->callback(FNET_ERR_TIMEDOUT, ping_if->packet_count, FNET_NULL, ping_if->callback_cookie);
                    }

                    if(ping_if->packet_count)
                    {
                        ping_if->state = FNET_PING_STATE_SENDING_REQUEST;
                    }
                    else
                    {
                        fnet_ping_release();
                    }
                }
            }
            break;
        /*===================================*/
        case FNET_PING_STATE_WAITING_TIMEOUT:
            if((fnet_timer_get_ms() - fnet_ping_if.send_time_ms) > fnet_ping_if.timeout_ms)
            {
                ping_if->state = FNET_PING_STATE_SENDING_REQUEST;
            }
            break;

        default:
            break;  /* do nothing, avoid compiler warning "enumeration value not handled in switch" */
    }
}

/************************************************************************
* DESCRIPTION: Releases the PING service.
************************************************************************/
void fnet_ping_release( void )
{
    if(fnet_ping_if.state != FNET_PING_STATE_DISABLED)
    {
        fnet_service_mutex_lock();
        /* Close socket. */
        fnet_socket_close(fnet_ping_if.socket_foreign);

        /* Unregister the tftp service. */
        fnet_service_unregister(fnet_ping_if.service_descriptor);

        fnet_ping_if.state = FNET_PING_STATE_DISABLED;
        fnet_service_mutex_unlock();
    }
}

#endif /* FNET_CFG_PING */

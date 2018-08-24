/**************************************************************************
*
* Copyright 2011-2017 by Andrey Butok. FNET Community.
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
*  DNS Client/Resolver implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_DNS

#include "fnet_dns_prv.h"

#if FNET_CFG_DEBUG_DNS && FNET_CFG_DEBUG
    #define FNET_DEBUG_DNS   FNET_DEBUG
#else
    #define FNET_DEBUG_DNS(...) do{}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/
/************************************************************************
* DNS-client states.
*************************************************************************/
typedef enum
{
    FNET_DNS_STATE_DISABLED = 0,    /* The DNS-client service is not initialized or is released.*/
    FNET_DNS_STATE_TX,              /* The DNS-client service sends the request to the DNS server.*/
    FNET_DNS_STATE_RX,              /* The DNS-client service waits a response from the DNS server.*/
    FNET_DNS_STATE_RELEASE          /* The DNS resolving is completed or timeout is occurred.*/
} fnet_dns_state_t;

#define FNET_DNS_ERR_PARAMS            "ERROR: Wrong input parameters."
#define FNET_DNS_ERR_DNS_UNKNOWN       "ERROR: DNS server unknown."
#define FNET_DNS_ERR_SOCKET_CREATION   "ERROR: Socket creation error."
#define FNET_DNS_ERR_SOCKET_CONNECT    "ERROR: Socket Error during connect."
#define FNET_DNS_ERR_SERVICE           "ERROR: Service registration is failed."
#define FNET_DNS_ERR_IS_INITIALIZED    "ERROR: DNS is already initialized."

static void _fnet_dns_poll( void *fnet_dns_if_p );
static fnet_size_t _fnet_dns_add_question( fnet_uint8_t *message, fnet_uint16_t type, const fnet_char_t *host_name);
static fnet_bool_t _fnet_dns_get_server_addr(struct fnet_sockaddr *dns_server_addr );

/************************************************************************
*    DNS-client resolved IPv4 address structure.
*************************************************************************/
typedef struct
{
    fnet_uint32_t       ttl;
    fnet_ip4_addr_t     ip4_addr; /* Resolved IPv4 address.*/
}
fnet_dns_if_resolved_ip4_t;

/************************************************************************
*    DNS-client resolved IPv6 address structure.
*************************************************************************/
typedef struct
{
    fnet_uint32_t       ttl;
    fnet_ip6_addr_t     ip6_addr; /* Resolved IPv6 address.*/
}
fnet_dns_if_resolved_ip6_t;

#define FNET_DNS_RESOLVED_IP4_MAX       (FNET_DNS_MESSAGE_SIZE/(sizeof(fnet_dns_if_resolved_ip4_t)+ sizeof(fnet_dns_resolved_addr_t)))
#define FNET_DNS_RESOLVED_IP6_MAX       (FNET_DNS_MESSAGE_SIZE/(sizeof(fnet_dns_if_resolved_ip6_t)+ sizeof(fnet_dns_resolved_addr_t)))

/************************************************************************
*    DNS-client interface structure.
*************************************************************************/
typedef struct
{
    fnet_socket_t                   socket_cln;
    fnet_service_desc_t             service_descriptor;
    fnet_dns_state_t                state;                          /* Current state. */
    fnet_dns_callback_resolved_t    callback;                       /* Callback function. */
    void                            *callback_cookie;               /* Callback-handler specific parameter. */
    fnet_time_t                     last_time_ms;                   /* Last receive time, used for timeout detection. */
    fnet_index_t                    iteration;                      /* Current iteration number.*/
    /* Internal buffer used for Message buffer and Resolved addresses.*/
    union
    {
        fnet_uint8_t             message[FNET_DNS_MESSAGE_SIZE]; /* Message buffer and Resolved addresses.*/
        struct
        {
            fnet_dns_if_resolved_ip4_t      resolved_ip4_addr[FNET_DNS_RESOLVED_IP4_MAX]; /* Resolved IPv4 addresses.*/
            fnet_dns_resolved_addr_t        resolved_ip4_addr_sock[FNET_DNS_RESOLVED_IP4_MAX]; /* Used as return buffer for application.*/
        } ip4;
        struct
        {
            fnet_dns_if_resolved_ip6_t      resolved_ip6_addr[FNET_DNS_RESOLVED_IP6_MAX]; /* Resolved IPv6 addresses.*/
            fnet_dns_resolved_addr_t        resolved_ip6_addr_sock[FNET_DNS_RESOLVED_IP6_MAX]; /* Used as return buffer for application.*/
        } ip6;
    } buffer;
    fnet_index_t                addr_number;
    fnet_size_t                 message_size;                   /* Size of the message.*/
    fnet_uint16_t               id;
    fnet_uint16_t               dns_type;                       /* DNS Resource Record type that is queried.*/
    fnet_address_family_t       addr_family;
}
fnet_dns_if_t;

/* DNS-client interface */
static fnet_dns_if_t    dns_if_list[FNET_CFG_DNS];
static fnet_uint16_t    dns_id;

/************************************************************************
* DESCRIPTION: Initializes DNS client service and starts the host
*              name reolving.
************************************************************************/
static fnet_size_t _fnet_dns_add_question( fnet_uint8_t *message, fnet_uint16_t type, const fnet_char_t *host_name)
{
    fnet_size_t         total_length = 0U;
    fnet_size_t         label_length;
    fnet_dns_q_tail_t   *q_tail;
    fnet_char_t         *strtok_pos = FNET_NULL;

    /* Set Question section :
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    */
    /* QNAME */
    /* a domain name represented as a sequence of labels, where
    * each label consists of a length octet followed by that
    * number of octets. The domain name terminates with the
    * zero length octet for the null label of the root. Note
    * that this field may be an odd number of octets; no
    * padding is used.
    */

    /* Copy host_name string.*/
    fnet_strcpy((fnet_char_t *)&message[1], host_name);

    /* TBD PFI, use strtok_pos as pointer.*/

    /* Replace '.' by zero.*/
    fnet_strtok_r((fnet_char_t *)&message[1], ".", &strtok_pos);

    while((label_length = fnet_strlen((fnet_char_t *)&message[total_length] + 1U)) > 0U)
    {
        message[total_length] = (fnet_uint8_t)label_length; /* Set length before (previous) label.*/
        total_length += label_length + 1U;

        fnet_strtok_r(FNET_NULL, ".", &strtok_pos);
    }

    q_tail = (fnet_dns_q_tail_t *)&message[total_length];

    /* Skip 1 byte (zero). End of string. */

    /* QTYPE */
    q_tail->qtype =  type;

    /* QCLASS */
    q_tail->qclass = FNET_HTONS(FNET_DNS_HEADER_CLASS_IN);

    return (total_length + sizeof(fnet_dns_q_tail_t));
}

/************************************************************************
* DESCRIPTION: Initializes DNS client service and starts the host
*              name resolving.
************************************************************************/
fnet_dns_desc_t fnet_dns_init( struct fnet_dns_params *params )
{
    const fnet_uint32_t bufsize_option = FNET_DNS_MESSAGE_SIZE;
    fnet_size_t         total_length;
    fnet_size_t         host_name_length;
    struct fnet_sockaddr remote_addr;
    fnet_dns_header_t   *header;
    fnet_dns_if_t       *dns_if = FNET_NULL;
    fnet_index_t        i;

    fnet_service_mutex_lock();

    /* Check input parameters. */
    if((params == 0)
       || (params->callback == 0)
       /* Check length of host_name.*/
       || ((host_name_length = fnet_strlen(params->host_name)) == 0U) || (host_name_length >= FNET_DNS_MAME_SIZE))
    {
        FNET_DEBUG_DNS(FNET_DNS_ERR_PARAMS);
        goto ERROR;
    }

    fnet_memcpy(&remote_addr, &params->dns_server_addr, sizeof(remote_addr));

    if(fnet_socket_addr_is_unspecified(&remote_addr) == FNET_TRUE) /* If DNS server is unspecified, get any registered one */
    {
        if(_fnet_dns_get_server_addr(&remote_addr) == FNET_FALSE)
        {
            FNET_DEBUG_DNS(FNET_DNS_ERR_DNS_UNKNOWN);
            goto ERROR;
        }
    }

    /* Try to find free DNS client descriptor. */
    for(i = 0u; i < FNET_CFG_DNS; i++)
    {
        if(dns_if_list[i].state == FNET_DNS_STATE_DISABLED)
        {
            dns_if = &dns_if_list[i];
            break;
        }
    }

    if(dns_if == FNET_NULL)
    {
        /* No free DNS descriptor. */
        FNET_DEBUG_DNS(FNET_DNS_ERR_IS_INITIALIZED);
        goto ERROR;
    }

    /* Reset interface structure. */
    fnet_memset_zero(dns_if, sizeof(fnet_dns_if_t));

    /* Save input parmeters.*/
    dns_if->callback = params->callback;
    dns_if->callback_cookie = params->cookie;
    dns_if->addr_family = params->addr_family;
    dns_if->addr_number = 0u;

    if(params->addr_family == AF_INET)
    {
        dns_if->dns_type = FNET_HTONS(FNET_DNS_TYPE_A);
    }
    else if(params->addr_family == AF_INET6)
    {
        dns_if->dns_type = FNET_HTONS(FNET_DNS_TYPE_AAAA);
    }
    else
    {
        FNET_DEBUG_DNS(FNET_DNS_ERR_PARAMS);
        goto ERROR;
    }

    dns_if->iteration = 0U;     /* Reset iteration counter.*/
    dns_id++;                   /* Increment global query ID */
    dns_if->id = dns_id;        /* Save query ID.*/

    /* Create socket */
    if((dns_if->socket_cln = fnet_socket(remote_addr.sa_family, SOCK_DGRAM, 0u)) == FNET_NULL)
    {
        FNET_DEBUG_DNS(FNET_DNS_ERR_SOCKET_CREATION);
        goto ERROR;
    }

    /* Set socket options */
    fnet_socket_setopt(dns_if->socket_cln, SOL_SOCKET, SO_RCVBUF, &bufsize_option, sizeof(bufsize_option));
    fnet_socket_setopt(dns_if->socket_cln, SOL_SOCKET, SO_SNDBUF, &bufsize_option, sizeof(bufsize_option));

    /* Bind/connect to the server.*/
    FNET_DEBUG_DNS("Connecting to DNS Server.");
    if(remote_addr.sa_port == 0U)
    {
        remote_addr.sa_port = FNET_CFG_DNS_PORT;
    }

    if(fnet_socket_connect(dns_if->socket_cln, &remote_addr, sizeof(remote_addr)) == FNET_ERR)
    {
        FNET_DEBUG_DNS(FNET_DNS_ERR_SOCKET_CONNECT);
        goto ERROR_1;
    }

    /* ==== Build message. ==== */
    fnet_memset_zero(dns_if->buffer.message, sizeof(dns_if->buffer.message)); /* Clear buffer.*/

    /* Set header fields:
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    */

    header = (fnet_dns_header_t *)dns_if->buffer.message;

    header->id = dns_if->id;            /* Set ID. */

    header->flags = FNET_HTONS(FNET_DNS_HEADER_FLAGS_RD); /* Recursion Desired.*/

    header->qdcount = FNET_HTONS(1U);        /* One Question. */

    /* No Answer (ANCOUNT).*/ /* No Authority (NSCOUNT). */ /* No Additional (ARCOUNT). */


    total_length = sizeof(fnet_dns_header_t);
    total_length += _fnet_dns_add_question( &dns_if->buffer.message[total_length], dns_if->dns_type, params->host_name);
    dns_if->message_size = total_length;

    /* Register DNS service. */
    dns_if->service_descriptor = fnet_service_register(_fnet_dns_poll, dns_if);
    if(dns_if->service_descriptor == FNET_NULL)
    {
        FNET_DEBUG_DNS(FNET_DNS_ERR_SERVICE);
        goto ERROR_1;
    }

    dns_if->state = FNET_DNS_STATE_TX; /* => Send request. */

    fnet_service_mutex_unlock();

    return (fnet_dns_desc_t)dns_if;

ERROR_1:
    fnet_socket_close(dns_if->socket_cln);
ERROR:
    fnet_service_mutex_unlock();
    return FNET_NULL;
}

/***************************************************************************/ /*!
 *
 * @brief    Get any registered DNS server IP address.
 *
 * @param dns_server_addr [in,out]  If dns_server_addr->sa_family and/or dns_server_addr->sa_scope_id are set, it is used as a filter to search a DNS sever address.
 *                                  It is filled by registered DNS server IP address.
 *
 * @return This function returns:
 *   - @ref FNET_TRUE if the @c dns_server_addr is filled.
 *   - @ref FNET_FALSE if no DNS address is not available.
 *
 * @see fnet_netif_get_ip4_dns(), fnet_netif_get_ip6_dns(), FNET_CFG_DNS
 ******************************************************************************
 *
 * This function is used to retrieve any registered DNS IP addresses.@n
 * It is present only if @ref FNET_CFG_DNS is set to 1.
 *
 ******************************************************************************/
/************************************************************************
*  Get any registered DNS Server address.
 * dns_server_addr [in,out]  If dns_server_addr->sa_family and/or dns_server_addr->sa_scope_id are set, it is used as a filter to search a DNS sever address.
 *                                  It is filled by registered DNS server IP address.
 *
 * This function returns:
 *   - FNET_TRUE if the @c dns_server_addr is filled.
 *   - FNET_FALSE if no DNS address is not available.
*************************************************************************/
static fnet_bool_t _fnet_dns_get_server_addr(struct fnet_sockaddr *dns_server_addr )
{
    fnet_bool_t         result = FNET_FALSE;
    fnet_index_t        netif_index = 0;
    fnet_netif_desc_t   netif;

    if(dns_server_addr)
    {
        netif = fnet_netif_get_by_scope_id(dns_server_addr->sa_scope_id);

        if(netif == FNET_NULL)
        {
            netif = fnet_netif_get_default();     /* Get default interface */
        }

        while(netif)
        {
#if FNET_CFG_IP4 /* IPv4 DNS has higher priority.*/
            if( ((dns_server_addr->sa_family == AF_UNSPEC) || (dns_server_addr->sa_family & AF_INET)) &&
                ((((struct fnet_sockaddr_in *)(dns_server_addr))->sin_addr.s_addr = fnet_netif_get_ip4_dns(netif)) != (fnet_ip4_addr_t)0))
            {
                dns_server_addr->sa_family = AF_INET;
                break;
            }
            else
#endif
#if FNET_CFG_IP6 && FNET_CFG_ND6_RDNSS
                if(((dns_server_addr->sa_family == AF_UNSPEC) || (dns_server_addr->sa_family & AF_INET6)) &&
                   (fnet_netif_get_ip6_dns(netif, 0, (fnet_ip6_addr_t *)&dns_server_addr->sa_data) == FNET_TRUE))
                {
                    dns_server_addr->sa_family = AF_INET6;
                    break;
                }
                else
#endif
                {
                    if(dns_server_addr->sa_scope_id == 0) /* Any interface */
                    {
                        netif = fnet_netif_get_by_number(netif_index++);
                    }
                }
        }

        if(fnet_socket_addr_is_unspecified(dns_server_addr) == FNET_FALSE) /* If DNS address is specified */
        {
            dns_server_addr->sa_scope_id = fnet_netif_get_scope_id(netif);
            result = FNET_TRUE;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: DNS-client state machine.
************************************************************************/
static void _fnet_dns_poll( void *fnet_dns_if_p )
{
    fnet_ssize_t            sent_size;
    fnet_ssize_t            received;
    fnet_index_t            i;
    fnet_dns_header_t       *header;
    fnet_dns_rr_header_t    *rr_header;
    fnet_dns_if_t           *dns_if = (fnet_dns_if_t *)fnet_dns_if_p;

    switch(dns_if->state)
    {
        /*---- TX --------------------------------------------*/
        case FNET_DNS_STATE_TX:

            FNET_DEBUG_DNS("Sending query...");
            sent_size = fnet_socket_send(dns_if->socket_cln, dns_if->buffer.message, dns_if->message_size, 0U);

            if (sent_size != (fnet_int32_t)dns_if->message_size)
            {
                dns_if->state = FNET_DNS_STATE_RELEASE; /* ERROR */
            }
            else
            {
                dns_if->last_time_ms = fnet_timer_get_ms();
                dns_if->state = FNET_DNS_STATE_RX;
            }
            break;
        /*---- RX -----------------------------------------------*/
        case  FNET_DNS_STATE_RX:
            /* Receive data */

            received = fnet_socket_recv(dns_if->socket_cln, dns_if->buffer.message, sizeof(dns_if->buffer.message), 0U);

            if(received > 0 )
            {
                header = (fnet_dns_header_t *)dns_if->buffer.message;

                if((header->id == dns_if->id) && /* Check the ID.*/
                   ((header->flags & FNET_DNS_HEADER_FLAGS_QR) == FNET_DNS_HEADER_FLAGS_QR)) /* Is response.*/
                {
                    for (i = (sizeof(fnet_dns_header_t) - 1U); i < (fnet_index_t)received; i++)
                    {
                        /* [RFC1035 4.1.4.] In order to reduce the size of messages, the domain system utilizes a
                        * compression scheme which eliminates the repetition of domain names in a
                        * message. In this scheme, an entire domain name or a list of labels at
                        * the end of a domain name is replaced with a pointer to a prior occurance
                        * of the same name.
                        * The pointer takes the form of a two octet sequence:
                        * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                        * | 1  1|                OFFSET                   |
                        * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                        */
                        /* => Check for 0xC0. */
                        if ((fnet_uint8_t)dns_if->buffer.message[i] == FNET_DNS_NAME_COMPRESSED_MASK) /* look for the beginnig of the response (Question Name == 192 (label compression))*/
                        {
                            rr_header = (fnet_dns_rr_header_t *)&dns_if->buffer.message[i];


                            /* Check Question Type, Class and Resource Data Length. */
                            if ( (rr_header->type ==  dns_if->dns_type) &&
                                 (rr_header->rr_class == FNET_HTONS(FNET_DNS_HEADER_CLASS_IN)))
                            {
                                /* Resolved.*/
                                if(rr_header->type == FNET_HTONS(FNET_DNS_TYPE_A))
                                {
                                    dns_if->buffer.ip4.resolved_ip4_addr[dns_if->addr_number].ip4_addr = *((fnet_ip4_addr_t *)(&rr_header->rdata));
                                    dns_if->buffer.ip4.resolved_ip4_addr[dns_if->addr_number].ttl = rr_header->ttl;
                                    if(dns_if->addr_number >= (FNET_DNS_RESOLVED_IP4_MAX - 1)) /* Check address number limit */
                                    {
                                        break;
                                    }
                                }
                                else /* AF_INET6 */
                                {
                                    FNET_IP6_ADDR_COPY( (fnet_ip6_addr_t *)(&rr_header->rdata), &dns_if->buffer.ip6.resolved_ip6_addr[dns_if->addr_number].ip6_addr );
                                    dns_if->buffer.ip6.resolved_ip6_addr[dns_if->addr_number].ttl = rr_header->ttl;
                                    if(dns_if->addr_number >= (FNET_DNS_RESOLVED_IP6_MAX - 1)) /* Check address number limit */
                                    {
                                        break;
                                    }
                                }
                                dns_if->addr_number++;
                            }
                            i += (sizeof(fnet_dns_rr_header_t) + (fnet_index_t)fnet_ntohs(rr_header->rdlength) - 4U - 1U);
                        }
                    }
                }
                else /* Wrong message.*/
                {
                    break;
                }

                dns_if->state = FNET_DNS_STATE_RELEASE; /* Resolved */
            }
            else /* No data or error. Check timeout */
            {
                if((fnet_timer_get_ms() - dns_if->last_time_ms) > (FNET_CFG_DNS_RETRANSMISSION_TIMEOUT * 1000U))
                {
                    dns_if->iteration++;

                    if(dns_if->iteration > FNET_CFG_DNS_RETRANSMISSION_MAX)
                    {
                        dns_if->state = FNET_DNS_STATE_RELEASE; /* Timeout */
                    }
                    else
                    {
                        dns_if->state = FNET_DNS_STATE_TX;
                    }
                }
                else
                {}
            }
            break;
        /*---- RELEASE -------------------------------------------------*/
        case FNET_DNS_STATE_RELEASE:
        {
            fnet_dns_resolved_addr_t   *addr_list = FNET_NULL;

            fnet_dns_release(fnet_dns_if_p);

            /* Fill fnet_dns_resolved_addr_t */
            if(dns_if->addr_number > 0u)
            {
                if(dns_if->addr_family  == AF_INET)
                {
                    for(i = 0u; i < dns_if->addr_number; i++)
                    {
                        fnet_memset_zero(&dns_if->buffer.ip4.resolved_ip4_addr_sock[i].resolved_addr, sizeof(dns_if->buffer.ip4.resolved_ip4_addr_sock[i].resolved_addr));

                        dns_if->buffer.ip4.resolved_ip4_addr_sock[i].resolved_addr.sa_family = AF_INET;
                        ((struct fnet_sockaddr_in *)(&dns_if->buffer.ip4.resolved_ip4_addr_sock[i].resolved_addr))->sin_addr.s_addr = dns_if->buffer.ip4.resolved_ip4_addr[i].ip4_addr;
                        dns_if->buffer.ip4.resolved_ip4_addr_sock[i].resolved_addr_ttl = dns_if->buffer.ip4.resolved_ip4_addr[i].ttl;
                    }
                    addr_list = dns_if->buffer.ip4.resolved_ip4_addr_sock;
                }
                else if(dns_if->addr_family == AF_INET6)
                {
                    for(i = 0u; i < dns_if->addr_number; i++)
                    {
                        fnet_memset_zero(&dns_if->buffer.ip6.resolved_ip6_addr_sock[i].resolved_addr, sizeof(dns_if->buffer.ip4.resolved_ip4_addr_sock[i].resolved_addr));

                        dns_if->buffer.ip6.resolved_ip6_addr_sock[i].resolved_addr.sa_family = AF_INET6;
                        FNET_IP6_ADDR_COPY(&dns_if->buffer.ip6.resolved_ip6_addr[i].ip6_addr, &((struct fnet_sockaddr_in6 *)(&dns_if->buffer.ip6.resolved_ip6_addr_sock[i].resolved_addr))->sin6_addr.s6_addr);
                        dns_if->buffer.ip6.resolved_ip6_addr_sock[i].resolved_addr_ttl = dns_if->buffer.ip6.resolved_ip6_addr[i].ttl;
                    }
                    addr_list = dns_if->buffer.ip6.resolved_ip6_addr_sock;
                }
                else
                {}
            }

            dns_if->callback(addr_list, dns_if->addr_number, dns_if->callback_cookie); /* User Callback.*/
        }
        break;
        default:
            break;
    }
}

/************************************************************************
* DESCRIPTION: This function aborts the resolving and releases
* the DNS-client service.
************************************************************************/
void fnet_dns_release( fnet_dns_desc_t desc )
{
    fnet_dns_if_t *dns_if = (fnet_dns_if_t *)desc;

    if(dns_if && (dns_if->state != FNET_DNS_STATE_DISABLED))
    {
        fnet_service_mutex_lock();

        /* Close socket. */
        fnet_socket_close(dns_if->socket_cln);

        /* Unregister the tftp service. */
        fnet_service_unregister( dns_if->service_descriptor );

        dns_if->state = FNET_DNS_STATE_DISABLED;

        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: This function returns FNET_TRUE if the DNS client
*              is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_dns_is_enabled(fnet_dns_desc_t desc)
{
    fnet_dns_if_t   *dns_if = (fnet_dns_if_t *) desc;
    fnet_bool_t     result;

    if(dns_if)
    {
        result = (dns_if->state == FNET_DNS_STATE_DISABLED) ? FNET_FALSE : FNET_TRUE;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

#endif /* FNET_CFG_DNS */

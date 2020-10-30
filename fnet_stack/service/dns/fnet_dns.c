/**************************************************************************
*
* Copyright 2011-2020 by Andrej Butok. FNET Community.
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
static fnet_bool_t _fnet_dns_cmp_name(const char *rr_name, const char *name);

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
    const fnet_char_t               *host_name;                     /* Host name to resolve (null-terminated string). */
    fnet_index_t                    iteration;                      /* Current iteration number.*/
    fnet_index_t                    addr_number;
    fnet_size_t                     message_size;                   /* Size of the message.*/
    fnet_uint16_t                   id;
    fnet_uint16_t                   dns_type;                       /* DNS Resource Record type that is queried.*/
    fnet_address_family_t           addr_family;
    fnet_uint8_t                    message[FNET_DNS_MESSAGE_SIZE]; /* Message buffer.*/
    fnet_dns_resolved_addr_t        resolved_addr[FNET_CFG_DNS_RESOLVED_ADDR_MAX]; /* Resolved addresses.*/
}
fnet_dns_if_t;

/* DNS-client interface */
static fnet_dns_if_t    dns_if_list[FNET_CFG_DNS];

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
    const fnet_uint32_t     bufsize_option = FNET_DNS_MESSAGE_SIZE;
    fnet_size_t             total_length;
    fnet_size_t             host_name_length;
    struct fnet_sockaddr    remote_addr;
    fnet_dns_header_t       *header;
    fnet_dns_if_t           *dns_if = FNET_NULL;
    fnet_index_t            i;

    fnet_service_mutex_lock();

    /* Check input parameters. */
    if((params == FNET_NULL)
       || (params->callback == FNET_NULL)
       /* Check length of host_name.*/
       || ((host_name_length = fnet_strnlen(params->host_name, FNET_DNS_NAME_SIZE+1)) == 0U) || (host_name_length > FNET_DNS_NAME_SIZE))
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
    dns_if->host_name = params->host_name;

    dns_if->id = (fnet_uint16_t)fnet_rand();    /* Randomize query ID.*/

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
    /* The message buffer has been cleared before.*/

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

    header = (fnet_dns_header_t *)dns_if->message;

    header->id = dns_if->id;            /* Set ID. */

    header->flags = FNET_HTONS(FNET_DNS_HEADER_FLAGS_RD); /* Recursion Desired.*/

    header->qdcount = FNET_HTONS(1U);        /* One Question. */

    /* No Answer (ANCOUNT).*/ /* No Authority (NSCOUNT). */ /* No Additional (ARCOUNT). */

    total_length = sizeof(fnet_dns_header_t);
    total_length += _fnet_dns_add_question( &dns_if->message[total_length], dns_if->dns_type, params->host_name);
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
                    else
                    {
                        break; /* Interface was specified, but no DNS server */
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
    fnet_dns_if_t           *dns_if = (fnet_dns_if_t *)fnet_dns_if_p;

    switch(dns_if->state)
    {
        /*---- TX --------------------------------------------*/
        case FNET_DNS_STATE_TX:

            FNET_DEBUG_DNS("Sending query...");
            sent_size = fnet_socket_send(dns_if->socket_cln, dns_if->message, dns_if->message_size, 0U);

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
            received = fnet_socket_recv(dns_if->socket_cln, dns_if->message, sizeof(dns_if->message), 0U);

            if(received > sizeof(fnet_dns_header_t))
            {
                header = (fnet_dns_header_t *)dns_if->message;

                if((header->id == dns_if->id) /* Check the ID.*/
                    && ((header->flags & FNET_DNS_HEADER_FLAGS_QR) == FNET_DNS_HEADER_FLAGS_QR) /* Is response.*/
                    && (fnet_htons(header->qdcount) == 1) ) /* Must be only one question */
                {
                    fnet_char_t         rr_name[FNET_DNS_NAME_SIZE];
                    const fnet_uint8_t  *ptr = &dns_if->message[sizeof(fnet_dns_header_t)];

                    /* Skip Question section */
                    ptr = _fnet_dns_get_rr_name(rr_name, sizeof(rr_name), ptr, dns_if->message, received);

                    if(ptr && (_fnet_dns_cmp_name(rr_name, dns_if->host_name) == FNET_TRUE)) /* Check question name */
                    {
                        ptr += 4; /* Skip question tail (qtype, qclass)*/

                        /* Handle answers */
                        for(i = 0; (i < fnet_htons(header->ancount)) && (ptr < (dns_if->message + received)); i++)
                        {
                            /* Header follows the hostname and the terminating zero */
                            const fnet_dns_rr_header_t *rr_header;

                            ptr = _fnet_dns_get_rr_name(rr_name, sizeof(rr_name), ptr, dns_if->message, received); /* Get RR name */
                            if(ptr)
                            {
                            	rr_header = (const fnet_dns_rr_header_t *)ptr;

                                if(_fnet_dns_cmp_name(rr_name, dns_if->host_name) == FNET_TRUE)
                                {
                                   /* Check Question Type, Class and Resource Data Length. */
                                    if ( (rr_header->type ==  dns_if->dns_type) &&
                                        (rr_header->rr_class == FNET_HTONS(FNET_DNS_HEADER_CLASS_IN)))
                                    {
                                        /* Resolved.*/
                                        if(rr_header->type == FNET_HTONS(FNET_DNS_TYPE_A))
                                        {
                                            dns_if->resolved_addr[dns_if->addr_number].resolved_addr.sa_family = AF_INET;
                                            ((struct fnet_sockaddr_in *)(&dns_if->resolved_addr[dns_if->addr_number].resolved_addr))->sin_addr.s_addr = *((fnet_ip4_addr_t *)(ptr+sizeof(fnet_dns_rr_header_t)));
                                        }
                                        else /* AF_INET6 */
                                        {
                                            dns_if->resolved_addr[dns_if->addr_number].resolved_addr.sa_family = AF_INET6;
                                            FNET_IP6_ADDR_COPY((fnet_ip6_addr_t *)(ptr+sizeof(fnet_dns_rr_header_t)), &((struct fnet_sockaddr_in6 *)(&dns_if->resolved_addr[dns_if->addr_number].resolved_addr))->sin6_addr.s6_addr);
                                        }
                                        dns_if->resolved_addr[dns_if->addr_number].resolved_addr_ttl = rr_header->ttl;
                                    
                                        dns_if->addr_number++;
                                        if(dns_if->addr_number >= (FNET_CFG_DNS_RESOLVED_ADDR_MAX)) /* Check address number limit */
                                        {
                                            break;
                                        }
                                    }

                                }

                                ptr += sizeof(fnet_dns_rr_header_t) + fnet_htons(rr_header->rdlength); /* Next answer */

                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        break; /* Wrong message */
                    }
                }
                else /* Wrong message.*/
                {
                    break;
                }

                if(dns_if->addr_number > 0)
                {
                    dns_if->state = FNET_DNS_STATE_RELEASE; /* Resolved */
                }

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
                addr_list = dns_if->resolved_addr;
            }

            dns_if->callback(addr_list, dns_if->addr_number, dns_if->host_name, dns_if->callback_cookie); /* User Callback.*/
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

/************************************************************************
* Compare RR name with requested-name. TBD use it for LLMNR
************************************************************************/
static fnet_bool_t _fnet_dns_cmp_name(const char *rr_name, const char *name)
{
    FNET_ASSERT(rr_name_p != FNET_NULL);
    FNET_ASSERT(name != FNET_NULL);

    fnet_bool_t         result;

    fnet_index_t        i = 0;
    fnet_uint32_t       name_length = fnet_strlen(name);
    fnet_uint32_t       rr_name_length = fnet_strlen(rr_name);

    if((name_length != 0) && ((name_length + 1 /* first length byte */)  == rr_name_length) )
    {
        rr_name++; /* Skip first length byte */

        for(i = 0; i < name_length; i++)
        {
            if(name[i] == '.')
            {
                continue; /* Skip length byte for rr_name */
            }

            if(fnet_tolower(rr_name[i]) != fnet_tolower(name[i]))
            {
                break; /* Not equal */
            }
        }
    }

    if(i == name_length)
    {
        result = FNET_TRUE;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

#endif /* FNET_CFG_DNS */


#if FNET_CFG_DNS || FNET_CFG_MDNS
/************************************************************************
* Put RR name to rr_name. Returns updated next pointer; 0 if error.
* If rr_name or rr_name_size are 0, just skip name.
************************************************************************/
const fnet_uint8_t *_fnet_dns_get_rr_name(char *rr_name, fnet_uint32_t rr_name_size, const fnet_uint8_t *rr, const fnet_uint8_t *packet, fnet_size_t packet_size)
{
    FNET_ASSERT(rr != FNET_NULL);
    FNET_ASSERT(packet != FNET_NULL);

    const fnet_uint8_t  *ptr = rr;
    fnet_uint8_t        length = 1; /* Length octet, must be 0 at the end of processing (null termination), otherwise there is an error. */
    fnet_uint16_t       offset = 0;
    const fnet_uint8_t  *result = ptr;
#define FNET_DNS_COMPRESSION_LIMIT         4  /* Limits number of compression elements, to avoid potential infinite loop in malformed packets*/
    fnet_index_t        compression_counter = 0;

    do
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
        * Check if compression is used: */
        while((*ptr & FNET_DNS_NAME_COMPRESSED_MASK) == FNET_DNS_NAME_COMPRESSED_MASK)
        {
            if(offset == 0) /* Is it first compression?*/
            {
                result += 2; /* 2 bytes (compression+length) */
            }

            offset = (fnet_uint16_t)((*ptr) & ~FNET_DNS_NAME_COMPRESSED_MASK) << 8;
            offset = offset + *(ptr + 1);
            if(offset < packet_size) /* Check maximum offset */
            {
                ptr = packet + offset;
            }
            else
            {
                /* Wrong offset */
                break;
            }

            if((++compression_counter) > FNET_DNS_COMPRESSION_LIMIT)
            {
                break; /* Error */
            }
        }

        if((*ptr & FNET_DNS_NAME_COMPRESSED_MASK) != 0) /* Top two bits may be used only by the compression flag.*/
        {
            break; /* Error */
        }

        length = *ptr;

        /* A domain name represented as a sequence of labels, where
        * each label consists of a length octet followed by that
        * number of octets. The domain name terminates with the
        * zero length octet for the null label of the root.*/

        if( rr_name && rr_name_size ) /* Copy to rr_name buffer.*/
        {
            if(length < rr_name_size) /* Check maximum rr_name_size */
            {
                fnet_memcpy(rr_name, ptr, length + 1);

                rr_name += (length + 1);
                rr_name_size -= (length + 1);
            }
            else
            {
                length = 1; /* To avoid corner case when length = 0 (OK)*/
                break; /* Error */
            }
        }

        ptr += (length + 1);
        if(ptr > result)
        {
            result = ptr;
        }
    }
    while(length != 0);

    if(length == 0) /* Check error state and proper null termination of the RR name string. */
    {
        return result; /* OK */
    }
    else
    {
        return NULL;   /* Malformed packet. */
    }
}

/************************************************************************
* Print DNS resource record name.
************************************************************************/
void _fnet_dns_print_rr_name(const fnet_char_t *prefix, const fnet_char_t *rr_name)
{
    fnet_size_t     rr_name_legth = fnet_strlen(rr_name);
    fnet_uint8_t    rr_name_length_index = 0;
    fnet_index_t    i;

    fnet_printf("%s", prefix); /* Print prefix*/

    for(i = 0; i < rr_name_legth; i++)
    {
        if(i == rr_name_length_index)
        {
            fnet_printf("."); /* Instead of name length */
            rr_name_length_index += rr_name[i] + 1; /* Next name length byte */
        }
        else
        {
            fnet_printf("%c", rr_name[i]);
        }
    }

    fnet_println("");
}

#endif /* FNET_CFG_DNS || FNET_CFG_MDNS */

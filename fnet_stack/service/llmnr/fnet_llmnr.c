/**************************************************************************
*
* Copyright 2014-2020 by Andrej Butok. FNET Community.
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
*  LLMNR Server/Responder implementation (RFC4795).
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_LLMNR

#include "fnet_llmnr.h"
#include "service/dns/fnet_dns_prv.h"

#if FNET_CFG_DEBUG_LLMNR && FNET_CFG_DEBUG
    #define FNET_DEBUG_LLMNR   FNET_DEBUG
#else
    #define FNET_DEBUG_LLMNR(...)   do{}while(0)
#endif

/* LLMNR requires multicast support.*/
#if FNET_CFG_MULTICAST == 0
    #error FNET_CFG_MULTICAST must be enabled for LLMNR
#endif

/************************************************************************
*     Definitions
*************************************************************************/
/* LLMNR-server states. */
typedef enum
{
    FNET_LLMNR_STATE_DISABLED = 0,       /**< @brief The LLMNR server is not initialized or released.  */
    FNET_LLMNR_STATE_WAITING_REQUEST     /**< @brief LLMNR server is waiting for a request from a LLMNR client. */
} fnet_llmnr_state_t;


/* RFC 4795: The IPv4 link-scope multicast address a given responder listens to, and to which a
 * sender sends queries, is 224.0.0.252.*/
#define FNET_LLMNR_IP4_MULTICAST_ADDR   FNET_IP4_ADDR_INIT(224u, 0u, 0u, 252u)

/* RFC 4795: The IPv6 link-scope multicast address a given responder listens to,
 * and to which a sender sends all queries, is FF02:0:0:0:0:0:1:3.*/
static const fnet_ip6_addr_t fnet_llmnr_ip6_multicast_addr = FNET_IP6_ADDR_INIT(0xFF, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01, 0x00, 0x03);

/* Error strings.*/
#define FNET_LLMNR_ERR_PARAMS            "LLMNR: Wrong input parameters."
#define FNET_LLMNR_ERR_SOCKET_CREATION   "LLMNR: Socket creation error."
#define FNET_LLMNR_ERR_SOCKET_BIND       "LLMNR: Socket Error during bind."
#define FNET_LLMNR_ERR_SERVICE           "LLMNR: Service registration is failed."
#define FNET_LLMNR_ERR_IS_INITIALIZED    "LLMNR: Is already initialized."
#define FNET_LLMNR_ERR_JOIN_MULTICAST    "LLMNR: Joining to multicast group is failed."

#define FNET_LLMNR_MESSAGE_SIZE     (FNET_DNS_MESSAGE_SIZE) /* Messages carried by UDP are restricted to 512 bytes (not counting the IP
                                                            * or UDP headers).  
                                                            * Longer messages (not supported) are truncated and the TC bit is set in
                                                            * the header.*/

/* For UDP queries and responses, the Hop Limit field in the IPv6 header
 * and the TTL field in the IPV4 header MAY be set to any value.
 * However, it is RECOMMENDED that the value 255 be used for
 * compatibility with early implementations of [RFC3927]. */
#define FNET_LLMNR_IP_TTL              (255u)

/************************************************************************
*    [RFC 4795 2.1.1.]  LLMNR Header Format
*************************************************************************
   LLMNR queries and responses utilize the DNS header format defined in
   [RFC1035] with exceptions noted below:
        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
      |                      ID                       |
      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
      |QR|   Opcode  | C|TC| T| Z| Z| Z| Z|   RCODE   |
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

FNET_COMP_PACKED_BEGIN
typedef struct
{
    fnet_uint16_t id       FNET_COMP_PACKED;      /* A 16-bit identifier assigned by the program that generates
                                                    * any kind of query.  This identifier is copied from the query
                                                    * to the response and can be used by the sender to match
                                                    * responses to outstanding queries.  The ID field in a query
                                                    * SHOULD be set to a pseudo-random value. */
    fnet_uint16_t flags    FNET_COMP_PACKED;       /* Flags.*/
    fnet_uint16_t qdcount  FNET_COMP_PACKED;       /* An unsigned 16-bit integer specifying the number of entries
                                                     * in the question section.  A sender MUST place only one
                                                     * question into the question section of an LLMNR query.  LLMNR
                                                     * responders MUST silently discard LLMNR queries with QDCOUNT
                                                     * not equal to one.  LLMNR senders MUST silently discard LLMNR
                                                     * responses with QDCOUNT not equal to one.*/
    fnet_uint16_t ancount  FNET_COMP_PACKED;       /* An unsigned 16-bit integer specifying the number of resource
                                                     * records in the answer section.  LLMNR responders MUST
                                                     * silently discard LLMNR queries with ANCOUNT not equal to
                                                     * zero.*/
    fnet_uint16_t nscount  FNET_COMP_PACKED;       /* An unsigned 16-bit integer specifying the number of name
                                                     * server resource records in the authority records section.
                                                     * Authority record section processing is described in Section
                                                     * 2.9.  LLMNR responders MUST silently discard LLMNR queries
                                                     * with NSCOUNT not equal to zero.*/
    fnet_uint16_t arcount  FNET_COMP_PACKED;       /* An unsigned 16-bit integer specifying the number of resource
                                                     * records in the additional records section.*/
} fnet_llmnr_header_t;
FNET_COMP_PACKED_END

/* LLMNR Header Flags.*/
#define FNET_LLMNR_HEADER_FLAGS_QR      (0x8000u)    /* Query/Response.  A 1-bit field, which, if set, indicates that
                                                    * the message is an LLMNR response; if clear, then the message
                                                    * is an LLMNR query.*/
#define FNET_LLMNR_HEADER_FLAGS_OPCODE  (0x7800u)    /* A 4-bit field that specifies the kind of query in this
                                                    * message.  This value is set by the originator of a query and
                                                    * copied into the response.  This specification defines the
                                                    * behavior of standard queries and responses (opcode value of
                                                    * zero).  Future specifications may define the use of other
                                                    * opcodes with LLMNR.  LLMNR senders and responders MUST
                                                    * support standard queries (opcode value of zero).  LLMNR
                                                    * queries with unsupported OPCODE values MUST be silently
                                                    * discarded by responders.*/
#define FNET_LLMNR_HEADER_FLAGS_C       (0x0400u)    /* Conflict.  When set within a query, the 'C'onflict bit
                                                    * indicates that a sender has received multiple LLMNR responses
                                                    * to this query.  In an LLMNR response, if the name is
                                                    * considered UNIQUE, then the 'C' bit is clear; otherwise, it
                                                    * is set.  LLMNR senders do not retransmit queries with the 'C'
                                                    * bit set.  Responders MUST NOT respond to LLMNR queries with
                                                    * the 'C' bit set, but may start the uniqueness verification
                                                    * process, as described in Section 4.2. */
#define FNET_LLMNR_HEADER_FLAGS_TC      (0x0200u)    /* TrunCation.  The 'TC' bit specifies that this message was
                                                    * truncated due to length greater than that permitted on the
                                                    * transmission channel.  The 'TC' bit MUST NOT be set in an
                                                    * LLMNR query and, if set, is ignored by an LLMNR responder.
                                                    * If the 'TC' bit is set in an LLMNR response, then the sender
                                                    * SHOULD resend the LLMNR query over TCP using the unicast
                                                    * address of the responder as the destination address.  */
#define FNET_LLMNR_HEADER_FLAGS_T       (0x0100u)    /* The 'T'entative bit is set in a response if the
                                                    * responder is authoritative for the name, but has not yet
                                                    * verified the uniqueness of the name.  A responder MUST ignore
                                                    * the 'T' bit in a query, if set.  A response with the 'T' bit
                                                    * set is silently discarded by the sender, except if it is a
                                                    * uniqueness query, in which case, a conflict has been detected
                                                    * and a responder MUST resolve the conflict as described in
                                                    * Section 4.1.*/
#define FNET_LLMNR_HEADER_FLAGS_RCODE   (0x000Fu)    /* Response code.  This 4-bit field is set as part of LLMNR
                                                    * responses.  In an LLMNR query, the sender MUST set RCODE to
                                                    * zero; the responder ignores the RCODE and assumes it to be
                                                    * zero.  The response to a multicast LLMNR query MUST have
                                                    * RCODE set to zero.  A sender MUST silently discard an LLMNR
                                                    * response with a non-zero RCODE sent in response to a
                                                    * multicast query. */

/************************************************************************
*    LLMNR server interface structure
*************************************************************************/
typedef struct fnet_llmnr_if
{
    fnet_llmnr_state_t      state;                  /* Current state.*/
    fnet_bool_t             is_enabled;
    fnet_socket_t           socket_listen;          /* Listening socket.*/
    fnet_service_desc_t     service_descriptor;     /* Service descriptor. */
    fnet_netif_desc_t       netif;                  /* Network interface descriptor. */
    const fnet_char_t       *host_name;             /* Link-local host name. */
    fnet_uint32_t           host_name_ttl;          /* TTL value that indicates for how many seconds link-local host name
                                                     * is valid for LLMNR querier, in seconds (it is optional).@n
                                                     * Default value is defined by @ref FNET_CFG_LLMNR_HOSTNAME_TTL. */
    fnet_uint8_t            message[FNET_LLMNR_MESSAGE_SIZE]; /* Message buffer.*/
} fnet_llmnr_if_t;

/* The LLMNR Server interface */
static struct fnet_llmnr_if fnet_llmnr_if_list[FNET_CFG_LLMNR];

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void _fnet_llmnr_poll( void *fnet_llmnr_if_p );
static fnet_bool_t _fnet_llmnr_hostname_cmp(const fnet_uint8_t *req_hostname, const fnet_uint8_t *hostname);

/************************************************************************
* DESCRIPTION: Checks if this our host name.
************************************************************************/
static fnet_bool_t _fnet_llmnr_hostname_cmp(const fnet_uint8_t *req_hostname, const fnet_uint8_t *hostname)
{
    fnet_index_t    req_hostname_index = 0u;
    fnet_index_t    hostname_index = 0u;
    fnet_index_t    i;
    fnet_uint8_t    req_hostname_len;
    fnet_uint8_t    hostname_c;

    req_hostname_len = req_hostname[req_hostname_index++];
    hostname_c = hostname[hostname_index];

    while(req_hostname_len
          && ((req_hostname_len & FNET_DNS_NAME_COMPRESSED_MASK) == 0u) /* No compression is allowed in query.*/
          && hostname_c )
    {
        for (i = 0u; i < req_hostname_len; i++)
        {
            if (fnet_tolower(hostname[hostname_index++]) != fnet_tolower(req_hostname[req_hostname_index++]))
            {
                return FNET_FALSE;
            }
        }
        req_hostname_len = req_hostname[req_hostname_index++];
        hostname_c = hostname[hostname_index++];

        if(hostname_c != '.')
        {
            break;
        }
    }

    return ((req_hostname_len == 0u) && (hostname_c == 0u) ? FNET_TRUE : FNET_FALSE);
}

/************************************************************************
* DESCRIPTION: Initializes Link-Local Multicast Name Resolution (LLMNR)
*              server/responder service.
************************************************************************/
fnet_llmnr_desc_t fnet_llmnr_init( fnet_llmnr_params_t *params )
{
    struct fnet_llmnr_if    *llmnr_if = FNET_NULL;
    fnet_index_t            i;
    struct fnet_sockaddr    local_addr;
    fnet_uint32_t           option = FNET_LLMNR_IP_TTL;
    fnet_size_t             host_name_length;
    fnet_scope_id_t         scope_id;

    fnet_service_mutex_lock();

    /* Check input paramters. */
    if((params == 0) || (params->netif_desc == 0) || (params->host_name == 0)
       || ((host_name_length = fnet_strnlen(params->host_name, (FNET_DNS_NAME_SIZE+1))) == 0u) || (host_name_length > FNET_DNS_NAME_SIZE))
    {
        FNET_DEBUG_LLMNR(FNET_LLMNR_ERR_PARAMS);
        goto ERROR_1;
    }

    scope_id = fnet_netif_get_scope_id(params->netif_desc);
    if(scope_id == 0)
    {
        FNET_DEBUG_LLMNR(FNET_LLMNR_ERR_PARAMS);
        goto ERROR_1;
    }

    /* Try to find free LLMNR server. */
    for(i = 0u; i < FNET_CFG_LLMNR; i++)
    {
        if(fnet_llmnr_if_list[i].is_enabled == FNET_FALSE)
        {
            llmnr_if = &fnet_llmnr_if_list[i];
        }
        else
        {
            if(fnet_llmnr_if_list[i].netif == params->netif_desc)
            {
                /* It is not possible to run several LLMNR servers on the same networking interface.*/
                llmnr_if = 0;
                break;
            }
        }
    }

    if(llmnr_if == FNET_NULL)
    {
        /* No free LLMNR descriptor. */
        FNET_DEBUG_LLMNR(FNET_LLMNR_ERR_IS_INITIALIZED);
        goto ERROR_1;
    }

    /* Reset interface structure. */
    fnet_memset_zero(llmnr_if, sizeof(struct fnet_llmnr_if));

    /* Set parameters.*/
    llmnr_if->netif = params->netif_desc;
    llmnr_if->host_name = params->host_name;
    if(params->host_name_ttl == 0u)
    {
        llmnr_if->host_name_ttl = FNET_CFG_LLMNR_HOSTNAME_TTL;
    }
    else
    {
        llmnr_if->host_name_ttl = params->host_name_ttl;
    }

    /* Init local socket address.*/
    fnet_memset_zero(&local_addr, sizeof(local_addr));
    if(params->addr_family == 0u)
    {
        local_addr.sa_family = AF_SUPPORTED;
    }
    else
    {
        local_addr.sa_family = params->addr_family;
    }
    local_addr.sa_port = FNET_CFG_LLMNR_PORT;
    local_addr.sa_scope_id = scope_id;

    /* Create listen socket */
    llmnr_if->socket_listen = fnet_socket(local_addr.sa_family, SOCK_DGRAM, 0u);
    if(llmnr_if->socket_listen == FNET_NULL)
    {
        FNET_DEBUG_LLMNR(FNET_LLMNR_ERR_SOCKET_CREATION);
        goto ERROR_1;
    }

    /* Bind socket. */
    if(fnet_socket_bind(llmnr_if->socket_listen, &local_addr, sizeof(local_addr)) == FNET_ERR)
    {
        FNET_DEBUG_LLMNR(FNET_LLMNR_ERR_SOCKET_BIND);
        goto ERROR_2;
    }

    /* Join Multicast Group.*/
#if FNET_CFG_IP4
    if((local_addr.sa_family & AF_INET) != 0u)
    {
        struct fnet_ip_mreq mreq; /* Multicast group information.*/

        mreq.imr_multiaddr.s_addr = FNET_LLMNR_IP4_MULTICAST_ADDR;
        mreq.imr_interface = scope_id;

        /* Join multicast group. */
        if(fnet_socket_setopt(llmnr_if->socket_listen, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == FNET_ERR)
        {
            FNET_DEBUG_LLMNR(FNET_LLMNR_ERR_JOIN_MULTICAST);
            goto ERROR_2;
        }
        /*
         * For UDP queries and responses, the Hop Limit field in the IPv6 header
         * and the TTL field in the IPV4 header MAY be set to any value.
         * However, it is RECOMMENDED that the value 255 be used for
         * compatibility with early implementations of [RFC3927]. */
        fnet_socket_setopt(llmnr_if->socket_listen, IPPROTO_IP, IP_MULTICAST_TTL, &option, sizeof(option));
    }
#endif
#if FNET_CFG_IP6
    if((local_addr.sa_family & AF_INET6) != 0u)
    {
        struct fnet_ipv6_mreq mreq6; /* Multicast group information.*/

        FNET_IP6_ADDR_COPY(&fnet_llmnr_ip6_multicast_addr, &mreq6.ipv6imr_multiaddr.s6_addr);
        mreq6.ipv6imr_interface = scope_id;

        /* Join multicast group. */
        if(fnet_socket_setopt(llmnr_if->socket_listen, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof(mreq6)) == FNET_ERR)
        {
            FNET_DEBUG_LLMNR(FNET_LLMNR_ERR_JOIN_MULTICAST);
            goto ERROR_2;
        }

        /* For UDP queries and responses, the Hop Limit field in the IPv6 header
         * and the TTL field in the IPV4 header MAY be set to any value.
         * However, it is RECOMMENDED that the value 255 be used for
         * compatibility with early implementations of [RFC3927]. */
        fnet_socket_setopt(llmnr_if->socket_listen, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &option, sizeof(option));
    }
#endif

    /* Register service. */
    llmnr_if->service_descriptor = fnet_service_register(_fnet_llmnr_poll, (void *) llmnr_if);

    if(llmnr_if->service_descriptor == 0)
    {
        FNET_DEBUG_LLMNR(FNET_LLMNR_ERR_SERVICE);
        goto ERROR_2;
    }

    llmnr_if->state = FNET_LLMNR_STATE_WAITING_REQUEST;

    llmnr_if->is_enabled = FNET_TRUE;

    fnet_service_mutex_unlock();
    return (fnet_llmnr_desc_t)llmnr_if;

ERROR_2:
    fnet_socket_close(llmnr_if->socket_listen);
ERROR_1:
    fnet_service_mutex_unlock();
    return FNET_NULL;
}

/************************************************************************
* DESCRIPTION: LLMNR server state machine.
************************************************************************/
static void _fnet_llmnr_poll( void *fnet_llmnr_if_p )
{
    struct fnet_sockaddr    addr;
    fnet_size_t             addr_len;
    fnet_ssize_t            received;
    struct fnet_llmnr_if    *llmnr_if = (struct fnet_llmnr_if *)fnet_llmnr_if_p;
    fnet_llmnr_header_t     *llmnr_header;

    switch(llmnr_if->state)
    {
        /*---- WAITING_REQUEST --------------------------------------------*/
        case FNET_LLMNR_STATE_WAITING_REQUEST:
            addr_len = sizeof(addr);

            received = fnet_socket_recvfrom(llmnr_if->socket_listen, llmnr_if->message, sizeof(llmnr_if->message), 0u, &addr, &addr_len );

            if(received >= (fnet_int32_t)sizeof(fnet_llmnr_header_t) )
            {
                llmnr_header = (fnet_llmnr_header_t *)(&llmnr_if->message[0]);

                if( ((llmnr_header->flags & FNET_HTONS(FNET_LLMNR_HEADER_FLAGS_QR)) == 0u) /* Request.*/
                    /* LLMNR senders and responders MUST
                    * support standard queries (opcode value of zero).  LLMNR
                    * queries with unsupported OPCODE values MUST be silently
                    * discarded by responders.*/
                    && ((llmnr_header->flags & FNET_HTONS(FNET_LLMNR_HEADER_FLAGS_OPCODE)) == 0u)
                    /* LLMNR responders MUST silently discard LLMNR queries with QDCOUNT
                    * not equal to one.*/
                    && (llmnr_header->qdcount == FNET_HTONS(1u))
                    /* LLMNR responders MUST
                    * silently discard LLMNR queries with ANCOUNT not equal to zero.*/
                    && (llmnr_header->ancount == 0u)
                    /* LLMNR responders MUST silently discard LLMNR queries
                    * with NSCOUNT not equal to zero.*/
                    && (llmnr_header->nscount == 0u)
                  )
                {
                    fnet_char_t         *req_hostname = (fnet_char_t *)&llmnr_if->message[sizeof(fnet_llmnr_header_t)];
                    fnet_dns_q_tail_t   *q_tail;
                    fnet_size_t         req_hostname_len = fnet_strnlen(req_hostname, (FNET_DNS_NAME_SIZE + 1));

                    /* Check size */
                    if( (req_hostname_len <= FNET_DNS_NAME_SIZE) && (received >= (fnet_int32_t)(sizeof(fnet_llmnr_header_t) + sizeof(fnet_dns_q_tail_t) + req_hostname_len)))
                    {
                        FNET_DEBUG_LLMNR("LLMNR: Req name = %s", req_hostname);

                        /* Responders MUST NOT respond to LLMNR queries for names for which
                           they are not authoritative.*/
                        if(_fnet_llmnr_hostname_cmp((const fnet_uint8_t *)req_hostname, (const fnet_uint8_t *)llmnr_if->host_name))
                        {
                            q_tail = (fnet_dns_q_tail_t *)(req_hostname + req_hostname_len);

                            /* Check Question Class. */
                            if (q_tail->qclass == FNET_HTONS(FNET_DNS_HEADER_CLASS_IN) )
                            {
                                fnet_dns_rr_c_header_t      *rr_header = (fnet_dns_rr_c_header_t *)(q_tail + 1);
                                fnet_size_t               send_size = (fnet_size_t)((fnet_char_t *)rr_header - (fnet_char_t *)llmnr_header);

                                /* Prepare query response.*/
#if FNET_CFG_IP4
                                if(q_tail->qtype == FNET_HTONS(FNET_DNS_TYPE_A))
                                {
                                    FNET_DEBUG_LLMNR("LLMNR: IPv4");

                                    *((fnet_ip4_addr_t *)(&rr_header->rdata)) = fnet_netif_get_ip4_addr(llmnr_if->netif);
                                    rr_header->rdlength = fnet_htons(sizeof(fnet_ip4_addr_t));
                                    rr_header->type = FNET_HTONS(FNET_DNS_TYPE_A);

                                    send_size += sizeof(fnet_dns_rr_c_header_t);
                                }
                                else
#endif
#if FNET_CFG_IP6
                                    if(q_tail->qtype == FNET_HTONS(FNET_DNS_TYPE_AAAA))
                                    {
                                        fnet_netif_ip6_addr_info_t  addr_info;

                                        FNET_DEBUG_LLMNR("LLMNR: IPv6");

                                        if(fnet_netif_get_ip6_addr (llmnr_if->netif, 0u, &addr_info) == FNET_TRUE)
                                        {
                                            FNET_IP6_ADDR_COPY( &addr_info.address, (fnet_ip6_addr_t *)(&rr_header->rdata));
                                            rr_header->rdlength = fnet_htons(sizeof(fnet_ip6_addr_t));
                                            rr_header->type = FNET_HTONS(FNET_DNS_TYPE_AAAA);

                                            send_size += sizeof(fnet_dns_rr_c_header_t) - sizeof(fnet_uint32_t) + sizeof(fnet_ip6_addr_t);
                                        }
                                        else
                                        {
                                            break; /* No IPv6 address.*/
                                        }
                                    }
                                    else
#endif
                                    {
                                        break; /* Not supported query type.*/
                                    }

                                /* Init the rest of answer parameters.*/
                                rr_header->ttl = fnet_htonl(llmnr_if->host_name_ttl);
                                rr_header->rr_class = FNET_HTONS(FNET_DNS_HEADER_CLASS_IN);
                                rr_header->name.name_ptr = fnet_htons((FNET_DNS_NAME_COMPRESSED_MASK << 8) | (FNET_DNS_NAME_COMPRESSED_INDEX_MASK & sizeof(fnet_llmnr_header_t)));

                                /* Updtae LLMNR header response.*/
                                llmnr_header->ancount = FNET_HTONS(1u);                          /* One answer.*/
                                llmnr_header->flags |= FNET_HTONS(FNET_LLMNR_HEADER_FLAGS_QR    /* Query response.*/
                                                                  | FNET_LLMNR_HEADER_FLAGS_C);           /* The name is not considered unique.*/

                                /* A responder responds to a multicast query by sending a unicast UDP response to the sender.*/
                                fnet_socket_sendto(llmnr_if->socket_listen, llmnr_if->message, send_size, 0u, &addr, addr_len);
                            }
                        }
                    }
                }
                /* else = wrong message.*/
            }
            break;
        default:
            break;
    }
}

/************************************************************************
* DESCRIPTION: Releases the Link-Local Multicast Name Resolution (LLMNR)
* server/responder service.
************************************************************************/
void fnet_llmnr_release(fnet_llmnr_desc_t desc)
{
    struct fnet_llmnr_if *llmnr_if = (struct fnet_llmnr_if *)desc;

    if(llmnr_if && (llmnr_if->is_enabled == FNET_TRUE))
    {
        fnet_service_mutex_lock();
        fnet_socket_close(llmnr_if->socket_listen);

        fnet_service_unregister(llmnr_if->service_descriptor); /* Delete service.*/
        llmnr_if->state = FNET_LLMNR_STATE_DISABLED;
        llmnr_if->is_enabled = FNET_FALSE;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: This function returns FNET_TRUE if the LLMNR server
*              is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_llmnr_is_enabled(fnet_llmnr_desc_t desc)
{
    struct fnet_llmnr_if    *llmnr_if = (struct fnet_llmnr_if *) desc;
    fnet_bool_t             result;

    if(llmnr_if)
    {
        result = llmnr_if->is_enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Looks for a LLMNR server assigned to the specified network interface.
************************************************************************/
fnet_llmnr_desc_t fnet_llmnr_get_by_netif(fnet_netif_desc_t netif)
{
    fnet_llmnr_if_t         *llmnr_if;
    fnet_index_t            i;
    fnet_llmnr_desc_t       llmnr_desc = 0;

    if(netif)
    {
        fnet_service_mutex_lock();
        for(i = 0u; i < FNET_CFG_LLMNR; i++)
        {
            llmnr_if = &fnet_llmnr_if_list[i];

            if((llmnr_if->is_enabled == FNET_TRUE) && (llmnr_if->netif == netif))
            {
                llmnr_desc = (fnet_llmnr_desc_t)llmnr_if;
                break;
            }
        }
        fnet_service_mutex_unlock();
    }

    return llmnr_desc;
}

#endif /* FNET_CFG_LLMNR*/

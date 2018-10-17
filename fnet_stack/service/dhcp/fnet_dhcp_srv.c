/**************************************************************************
*
* Copyright 2017 by Andrey Butok. FNET Community.
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
*  FNET DHCPv4 Server implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_DHCP_SRV && FNET_CFG_IP4

#include "fnet_dhcp_prv.h"

#if FNET_CFG_DEBUG_DHCP_SRV && FNET_CFG_DEBUG
    #define FNET_DEBUG_DHCP_SRV   FNET_DEBUG
#else
    #define FNET_DEBUG_DHCP_SRV(...)    do{}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/

/* Error strings.*/
#define FNET_DHCP_SRV_ERR_PARAMS             "[DHCP srv] Wrong input parameters."
#define FNET_DHCP_SRV_ERR_SOCKET_CREATION    "[DHCP srv] Socket creation error."
#define FNET_DHCP_SRV_ERR_SOCKET_BIND        "[DHCP srv] Socket Error during bind."
#define FNET_DHCP_SRV_ERR_SERVICE            "[DHCP srv] Service registration is failed."
#define FNET_DHCP_SRV_ERR_IS_INITIALIZED     "[DHCP srv] Is already initialized."

/* DHCP options */
typedef struct
{
    fnet_uint8_t            message_type;           /* The DHCP Message Type.
                                                    This option is used to convey the type of the DHCP message.
                                                    The "DHCP message type" option - must be included in every DHCP
                                                    message. This option defines the "type" of the DHCP message.*/
    fnet_ip4_addr_t         requested_ip_address;   /* Requested IP Address.
                                                    This option is used in a client request (DHCPDISCOVER) to allow the
                                                    client to request that a particular IP address be assigned.*/
    fnet_ip4_addr_t         server_identifier;      /* Server Identifier.
                                                    This option is used in DHCPOFFER and DHCPREQUEST messages, and may
                                                    optionally be included in the DHCPACK and DHCPNAK messages.  DHCP
                                                    servers include this option in the DHCPOFFER in order to allow the
                                                    client to distinguish between lease offers.  DHCP clients use the
                                                    contents of the 'server identifier' field as the destination address
                                                    for any DHCP messages unicast to the DHCP server.  DHCP clients also
                                                    indicate which of several lease offers is being accepted by including
                                                    this option in a DHCPREQUEST message.
                                                    The identifier is the IP address of the selected server.*/
    fnet_mac_addr_t         client_identifier;      /* Client-identifier.
                                                    This option is used by DHCP clients to specify their unique
                                                    identifier.  DHCP servers use this value to index their database of
                                                    address bindings.  This value is expected to be unique for all
                                                    clients in an administrative domain. */
    fnet_uint8_t            overload;                /* Option Overload.
                                                    This option is used to indicate that the DHCP 'sname' or 'file'
                                                    fields are being overloaded by using them to carry DHCP options. A
                                                    DHCP server inserts this option if the returned parameters will
                                                    exceed the usual space allotted for options.
                                                    If this option is present, the client interprets the specified
                                                    additional fields after it concludes interpretation of the standard
                                                    option fields. */
    fnet_uint32_t           lease_time;             /* IP Address Lease Time.
                                                    This option is used in a client request (DHCPDISCOVER or DHCPREQUEST)
                                                    to allow the client to request a lease time for the IP address.  In a
                                                    server reply (DHCPOFFER), a DHCP server uses this option to specify
                                                    the lease time it is willing to offer.
                                                    The time is in units of seconds, and is specified as a 32-bit unsigned integer.*/
    fnet_uint8_t            request_list[12];       /* Parameter Request List.
                                                    This option is used by a DHCP client to request values for specified
                                                    configuration parameters.  The list of requested parameters is
                                                    specified as n octets, where each octet is a valid DHCP option code
                                                    as defined in this document.
                                                    The client MAY list the options in order of preference.  The DHCP
                                                    server is not required to return the options in the requested order,
                                                    but MUST try to insert the requested options in the order requested
                                                    by the client.*/
    fnet_uint8_t            request_list_length;    /* Parameter Request List length. Its minimum length is 1.*/
} fnet_dhcp_srv_options_t;

/* Address pool entry state */
typedef enum
{
    FNET_DHCP_SRV_ADDR_POOL_STATE_FREE = 0,
    FNET_DHCP_SRV_ADDR_POOL_STATE_OFFERED,
    FNET_DHCP_SRV_ADDR_POOL_STATE_BOUND,
    /*   FNET_DHCP_SRV_ADDR_POOL_STATE_EXPIRED */
} fnet_dhcp_srv_addr_pool_state_t;

/* Timeout for the FNET_DHCP_SRV_ADDR_POOL_STATE_OFFERED state */
#define FNET_DHCP_SRV_ADDR_POOL_STATE_OFFERED_TIMEOUT   5 /* sec */

/* Address pool entry */
typedef struct
{
    fnet_dhcp_srv_addr_pool_state_t     state;                  /* Address state */
    fnet_time_t                         state_timestamp_ms;     /* The timestamp.*/
    fnet_time_t                         lease_time;             /* Lease time.*/
    fnet_mac_addr_t                     client_identifier;      /* Client-identifier. The combination of ’client identifier’ or ’chaddr’ and assigned network address constitute a unique identifier for the client’s lease*/
} fnet_dhcp_srv_addr_pool_t;

/************************************************************************
*    DHCPv4 Server interface structure.
*************************************************************************/
typedef struct fnet_dhcp_srv_if
{
    fnet_netif_desc_t               netif;
    fnet_bool_t                     is_enabled;
    fnet_service_desc_t             service_descriptor;
    fnet_socket_t                   socket_srv;          /* Listening socket.*/
    fnet_dhcp_srv_addr_pool_t       ip_addr_pool[FNET_CFG_DHCP_SRV_ADDR_POOL_SIZE];  /* Address pool */
    fnet_ip4_addr_t                 ip_addr_pool_start;     /* The first allocated IPv4 address in the DHCPv4 server pool.*/
    fnet_size_t                     ip_addr_pool_size;      /* Size of the address pool. It may not be set more than @ref FNET_CFG_DHCP_SRV_ADDR_POOL_SIZE.
                                                               If it is set to zero, the address pool size will be set to @ref FNET_CFG_DHCP_SRV_ADDR_POOL_SIZE.*/
    fnet_ip4_addr_t                 subnet_mask;            /* Subnet mask.*/
    fnet_ip4_addr_t                 gateway;                /* Gateway IPv4 address.*/
#if FNET_CFG_DNS
    fnet_ip4_addr_t                 dns;                    /* DNS server IPv4 address.*/
#endif
    fnet_uint32_t                   lease_time;             /* Lease time in seconds.*/
    fnet_dhcp_header_t              message;

    /* TBD RFC: Servers SHOULD be implemented so that
    network administrators MAY choose to disable probes of newly
    allocated addresses.*/
} fnet_dhcp_srv_if_t;

/* The DHCPv4 server interface list*/
static  fnet_dhcp_srv_if_t fnet_dhcp_srv_if_list[FNET_CFG_DHCP_SRV];

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void _fnet_dhcp_srv_poll(void *fnet_dhcp_srv_if_p);
static fnet_uint8_t *_fnet_dhcp_srv_get_option(fnet_uint8_t **options, fnet_uint8_t *option_end);
static void _fnet_dhcp_srv_parse_options( fnet_uint8_t *option_buffer, fnet_size_t option_buffer_size, fnet_dhcp_srv_options_t *options );
static void _fnet_dhcp_srv_set_lease_time(fnet_dhcp_srv_if_t *dhcp_srv_if, fnet_dhcp_srv_addr_pool_t  *ip_addr_pool, const fnet_dhcp_srv_options_t *options);
static fnet_int32_t _fnet_dhcp_srv_get_addr_pool(fnet_dhcp_srv_if_t *dhcp_srv_if, fnet_dhcp_header_t  *dhcp_header, fnet_mac_addr_t *client_identifier, fnet_ip4_addr_t requested_ip_address);
static fnet_uint8_t *_fnet_dhcp_srv_add_option(fnet_uint8_t *option_buffer, fnet_size_t option_buffer_size, fnet_uint8_t option_code, fnet_uint8_t option_length,  const void *option_value);
static void _fnet_dhcp_srv_send_message(fnet_dhcp_srv_if_t *dhcp_if, fnet_ip4_addr_t server_identifier, fnet_uint8_t message_type, fnet_dhcp_srv_options_t *rx_options, fnet_int32_t addr_pool_index);
static fnet_int32_t _fnet_dhcp_srv_get_addr_pool_free(fnet_dhcp_srv_if_t *dhcp_srv_if);

/************************************************************************
* DESCRIPTION: Initializes DHCPv4 server.
************************************************************************/
fnet_dhcp_srv_desc_t fnet_dhcp_srv_init(fnet_dhcp_srv_params_t *params)
{
    struct fnet_dhcp_srv_if     *dhcp_srv_if = FNET_NULL;
    fnet_index_t                i;
    struct fnet_sockaddr        local_addr;
    fnet_scope_id_t             scope_id;

    fnet_service_mutex_lock();

    /* Check input paramters. */
    if((params == 0) || (params->netif == 0))
    {
        FNET_DEBUG_DHCP_SRV(FNET_DHCP_SRV_ERR_PARAMS);
        goto ERROR_1;
    }

    scope_id = fnet_netif_get_scope_id(params->netif);
    if(scope_id == 0)
    {
        FNET_DEBUG_DHCP_SRV(FNET_DHCP_SRV_ERR_PARAMS);
        goto ERROR_1;
    }

    /* Try to find free DHCPv4 server. */
    for(i = 0u; i < FNET_CFG_DHCP_SRV; i++)
    {
        if(fnet_dhcp_srv_if_list[i].is_enabled == FNET_FALSE)
        {
            dhcp_srv_if = &fnet_dhcp_srv_if_list[i];
        }
        else
        {
            if(fnet_dhcp_srv_if_list[i].netif == params->netif)
            {
                /* It is not possible to run several DHCP servers on the same networking interface.*/
                dhcp_srv_if = 0;
                break;
            }
        }
    }

    /* No free DHCP server descriptor. */
    if(dhcp_srv_if == FNET_NULL)
    {
        /* No free DHCP descriptor. */
        FNET_DEBUG_DHCP_SRV(FNET_DHCP_SRV_ERR_IS_INITIALIZED);
        goto ERROR_1;
    }

    /* Reset interface structure. */
    fnet_memset_zero(dhcp_srv_if, sizeof(struct fnet_dhcp_srv_if));

    /* Set parameters.*/
    dhcp_srv_if->netif = params->netif;
    dhcp_srv_if->ip_addr_pool_start = params->ip_addr_pool_start;
    dhcp_srv_if->ip_addr_pool_size = params->ip_addr_pool_size;
    dhcp_srv_if->subnet_mask = params->subnet_mask;
    dhcp_srv_if->gateway = params->gateway;
    dhcp_srv_if->lease_time = params->lease_time;
#if FNET_CFG_DNS
    dhcp_srv_if->dns = params->dns;
#endif

    if(dhcp_srv_if->lease_time == 0)
    {
        dhcp_srv_if->lease_time = FNET_CFG_DHCP_SRV_LEASE_TIME;
    }

    if(dhcp_srv_if->ip_addr_pool_start == INADDR_ANY)
    {
        /* If it is set to INADDR_ANY, the netif IPv4 address plus one is used.*/
        dhcp_srv_if->ip_addr_pool_start = fnet_htonl(fnet_ntohl(fnet_netif_get_ip4_addr(dhcp_srv_if->netif)) + 1);
    }

    if(dhcp_srv_if->subnet_mask == INADDR_ANY)
    {
        /* If it is set to INADDR_ANY, the netif subnet mask is used.*/
        dhcp_srv_if->subnet_mask =  fnet_netif_get_ip4_subnet_mask(dhcp_srv_if->netif);
    }

    if(dhcp_srv_if->gateway == INADDR_ANY)
    {
        /* If it is set to INADDR_ANY, the netif gateway is used.*/
        dhcp_srv_if->gateway =  fnet_netif_get_ip4_gateway(dhcp_srv_if->netif);
    }

#if FNET_CFG_DNS
    if(dhcp_srv_if->dns == INADDR_ANY)
    {
        /* If it is set to INADDR_ANY, the netif dns address is used.*/
        dhcp_srv_if->dns =  fnet_netif_get_ip4_dns(dhcp_srv_if->netif);
    }
#endif

    if((dhcp_srv_if->ip_addr_pool_size == 0) || (dhcp_srv_if->ip_addr_pool_size > FNET_CFG_DHCP_SRV_ADDR_POOL_SIZE))
    {
        dhcp_srv_if->ip_addr_pool_size = FNET_CFG_DHCP_SRV_ADDR_POOL_SIZE;
    }

    /* Init local socket address.*/
    fnet_memset_zero(&local_addr, sizeof(local_addr));
    local_addr.sa_family = AF_INET;
    local_addr.sa_port = FNET_CFG_DHCP_SRV_PORT;
    local_addr.sa_scope_id = scope_id;

    /* Create listen socket */
    dhcp_srv_if->socket_srv = fnet_socket(local_addr.sa_family, SOCK_DGRAM, 0u);
    if(dhcp_srv_if->socket_srv == FNET_NULL)
    {
        FNET_DEBUG_DHCP_SRV(FNET_DHCP_SRV_ERR_SOCKET_CREATION);
        goto ERROR_1;
    }

    /* Bind socket. */
    if(fnet_socket_bind(dhcp_srv_if->socket_srv, &local_addr, sizeof(local_addr)) == FNET_ERR)
    {
        FNET_DEBUG_DHCP_SRV(FNET_DHCP_SRV_ERR_SOCKET_BIND);
        goto ERROR_2;
    }

    /* Register service. */
    dhcp_srv_if->service_descriptor = fnet_service_register(_fnet_dhcp_srv_poll, (void *) dhcp_srv_if);
    if(dhcp_srv_if->service_descriptor == 0)
    {
        FNET_DEBUG_DHCP_SRV(FNET_DHCP_SRV_ERR_SERVICE);
        goto ERROR_2;
    }

    dhcp_srv_if->is_enabled = FNET_TRUE;

    fnet_service_mutex_unlock();

    return (fnet_dhcp_srv_desc_t)dhcp_srv_if;

ERROR_2:
    fnet_socket_close(dhcp_srv_if->socket_srv);
ERROR_1:
    fnet_service_mutex_unlock();
    return 0;
}

/************************************************************************
* DESCRIPTION: Releases the DHCP server.
************************************************************************/
void fnet_dhcp_srv_release(fnet_dhcp_srv_desc_t dhcp_srv_desc)
{
    struct fnet_dhcp_srv_if *dhcp_srv_if = (struct fnet_dhcp_srv_if *)dhcp_srv_desc;

    if(dhcp_srv_if && (dhcp_srv_if->is_enabled == FNET_TRUE))
    {
        fnet_service_mutex_lock();

        fnet_socket_close(dhcp_srv_if->socket_srv);

        fnet_service_unregister(dhcp_srv_if->service_descriptor); /* Delete service.*/

        dhcp_srv_if->is_enabled = FNET_FALSE;

        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Retrieves an address pool information.
************************************************************************/
fnet_bool_t fnet_dhcp_srv_get_addr_pool_info(fnet_dhcp_srv_desc_t desc, fnet_index_t n, fnet_dhcp_srv_addr_pool_info_t *addr_info)
{
    fnet_bool_t                 result = FNET_FALSE;
    fnet_index_t                i;
    fnet_dhcp_srv_if_t          *dhcp_srv_if = (fnet_dhcp_srv_if_t *)desc;
    fnet_dhcp_srv_addr_pool_t   *addr_pool;

    if(dhcp_srv_if && addr_info)
    {
        fnet_service_mutex_lock();

        for(i = 0u; i < FNET_CFG_DHCP_SRV_ADDR_POOL_SIZE; i++)
        {
            addr_pool = &dhcp_srv_if->ip_addr_pool[i];
            /* Skip NOT_USED addresses. */
            if(addr_pool->state == FNET_DHCP_SRV_ADDR_POOL_STATE_BOUND)
            {
                if(n == 0u)
                {
                    FNET_MAC_ADDR_COPY(addr_pool->client_identifier, addr_info->client_mac_addr);  /* Client-identifier (MAC address)*/
                    addr_info->client_ip4_addr = fnet_htonl(fnet_ntohl(dhcp_srv_if->ip_addr_pool_start) + i); /* Client IPv4 address allocated by the DHCPv4 server.*/
                    addr_info->lease_time = addr_pool->lease_time - ((fnet_timer_get_ms() - addr_pool->state_timestamp_ms) / FNET_TIMER_MS_IN_SEC); /* Lease time (in seconds).*/

                    result = FNET_TRUE;
                    break;
                }
                n--;
            }
        }

        fnet_service_mutex_unlock();
    }

    return result;
}

/************************************************************************
* DESCRIPTION: DHCP server state machine.
************************************************************************/
static void _fnet_dhcp_srv_poll( void *fnet_dhcp_srv_if_p )
{
    fnet_dhcp_srv_if_t          *dhcp_srv_if = (fnet_dhcp_srv_if_t *)fnet_dhcp_srv_if_p;
    fnet_ssize_t                size;
    struct fnet_sockaddr        addr_from;
    fnet_dhcp_header_t          *dhcp_header = &dhcp_srv_if->message;
    fnet_size_t                 addr_len = sizeof(addr_from);
    fnet_dhcp_srv_options_t     options_rx;
    fnet_dhcp_srv_addr_pool_t   *ip_addr_pool;
    fnet_int32_t                addr_pool_index;
    fnet_mac_addr_t             *client_identifier;
    fnet_index_t                i;
    fnet_ip4_addr_t             server_identifier;
    fnet_uint8_t                message_type_tx = 0;

    /* Check address pool timeouts.*/
    for(i = 0; i < dhcp_srv_if->ip_addr_pool_size; i++)
    {
        ip_addr_pool = &dhcp_srv_if->ip_addr_pool[i];
        if(ip_addr_pool->state != FNET_DHCP_SRV_ADDR_POOL_STATE_FREE)
        {
            fnet_time_t    timeout_ms;
            if(ip_addr_pool->state == FNET_DHCP_SRV_ADDR_POOL_STATE_OFFERED)
            {
                timeout_ms = FNET_DHCP_SRV_ADDR_POOL_STATE_OFFERED_TIMEOUT * FNET_TIMER_MS_IN_SEC;
            }
            else
            {
                timeout_ms = ip_addr_pool->lease_time * FNET_TIMER_MS_IN_SEC;
            }

            if((fnet_timer_get_ms() - ip_addr_pool->state_timestamp_ms) > timeout_ms)
            {
                /* Expired. */
                ip_addr_pool->state = FNET_DHCP_SRV_ADDR_POOL_STATE_FREE;
            }
        }
    }

    /* RX */
    size = fnet_socket_recvfrom(dhcp_srv_if->socket_srv, dhcp_header, sizeof(fnet_dhcp_header_t), 0U, &addr_from, &addr_len);

    /* Common field check */
    if((size > (fnet_ssize_t)(sizeof(fnet_dhcp_header_t) - FNET_DHCP_OPTIONS_LENGTH)) /* Check minimum size */
       && (dhcp_header->op == FNET_DHCP_OP_BOOTREQUEST)        /* The 'op' field of each DHCP message sent from a client to a server contains BOOTREQUEST.*/
       && (dhcp_header->htype == FNET_DHCP_HTYPE_ETHERNET)     /* Ethernet type.*/
       && (fnet_memcmp(dhcp_header->chaddr, fnet_eth_null_addr, sizeof(fnet_mac_addr_t)) != 0) /* Client HW address is not null */
       && (dhcp_header->hlen == sizeof(fnet_mac_addr_t))       /* Supports MAC address only as HW address */
       && (fnet_memcmp(&dhcp_header->magic_cookie[0], fnet_dhcp_magic_cookie, sizeof(fnet_dhcp_magic_cookie)) == 0) /* Check magic cookie */
      )
    {
        _fnet_dhcp_trace("RX DHCP Server", dhcp_header);

        /* === Parse RX options ===*/
        fnet_memset_zero(&options_rx, sizeof(options_rx));                        /* Clear options_rx.*/
        /* The options in the ’options’ field
        MUST be interpreted first, so that any ’option overload’ options may
        be interpreted.*/
        _fnet_dhcp_srv_parse_options(dhcp_header->options, size - (sizeof(fnet_dhcp_header_t) - FNET_DHCP_OPTIONS_LENGTH), &options_rx);

        /* Parse overload options in sname/file
           Value   Meaning
           -----   --------
             1     the 'file' field is used to hold options
             2     the 'sname' field is used to hold options
             3     both fields are used to hold options*/
        if(options_rx.overload)
        {
            if(options_rx.overload & FNET_DHCP_OPTION_OVERLOAD_SNAME)
            {
                _fnet_dhcp_srv_parse_options(dhcp_header->sname, sizeof(dhcp_header->sname), &options_rx);
            }

            if(options_rx.overload & FNET_DHCP_OPTION_OVERLOAD_FILE)
            {
                _fnet_dhcp_srv_parse_options(dhcp_header->file, sizeof(dhcp_header->file), &options_rx);
            }
        }

        /* If the client identifier is present (not null), use it */
        if(fnet_memcmp(options_rx.client_identifier, fnet_eth_null_addr, sizeof(fnet_mac_addr_t)) != 0)
        {
            client_identifier = &options_rx.client_identifier;
        }
        else
        {
            client_identifier = (fnet_mac_addr_t *)(&dhcp_header->chaddr[0]);
        }

        server_identifier = fnet_netif_get_ip4_addr(dhcp_srv_if->netif);

        /* Get a network address for the requesting client. */
        addr_pool_index = _fnet_dhcp_srv_get_addr_pool(dhcp_srv_if, dhcp_header, client_identifier, options_rx.requested_ip_address);

        /* === Handle received message ===*/
        switch(options_rx.message_type)
        {
            /* DHCPDISCOVER message.
            Client broadcast to locate available servers.*/
            case FNET_DHCP_OPTION_MSG_TYPE_DISCOVER:
                /* Field check */
                if(dhcp_header->ciaddr == INADDR_ANY)   /* ’ciaddr’ 0 (DHCPDISCOVER)*/
                {
                    /* Allocate a new address from the server’s pool of available addresses. */
                    if(addr_pool_index == FNET_ERR)
                    {
                        addr_pool_index = _fnet_dhcp_srv_get_addr_pool_free(dhcp_srv_if);
                    }

                    if(addr_pool_index != FNET_ERR)
                    {
                        ip_addr_pool = &dhcp_srv_if->ip_addr_pool[addr_pool_index];

                        /* The server must also choose an expiration time for the lease */
                        _fnet_dhcp_srv_set_lease_time(dhcp_srv_if, ip_addr_pool, &options_rx);

                        /* While not required for correct operation of DHCP, the server SHOULD
                        NOT reuse the selected network address before the client responds to
                        the server’s DHCPOFFER message. The server may choose to record the
                        address as offered to the client.*/
                        if(ip_addr_pool->state == FNET_DHCP_SRV_ADDR_POOL_STATE_FREE)
                        {
                            fnet_memcpy(ip_addr_pool->client_identifier, client_identifier, sizeof(fnet_mac_addr_t));
                            ip_addr_pool->state = FNET_DHCP_SRV_ADDR_POOL_STATE_OFFERED;
                        }

                        /* Once the network address and lease have been determined, the server
                        constructs a DHCPOFFER message with the offered configuration
                        parameters. */
                        message_type_tx = FNET_DHCP_OPTION_MSG_TYPE_OFFER;
                    }
                }
                break;
            /* DHCPREQUEST message */
            case FNET_DHCP_OPTION_MSG_TYPE_REQUEST:
                if(addr_pool_index != FNET_ERR)
                {
                    ip_addr_pool = &dhcp_srv_if->ip_addr_pool[addr_pool_index];

                    /* Must be allocated/offered by us */
                    if(ip_addr_pool->state != FNET_DHCP_SRV_ADDR_POOL_STATE_FREE)
                    {
                        /* If the DHCPREQUEST message contains a ’server
                        identifier’ option, the message is in response to a DHCPOFFER
                        message.
                        MUST (after MUST SELECTING)
                        MUST NOT (after INIT-REBOOT, BOUND, RENEWING or REBINDING)*/
                        if(options_rx.server_identifier)
                        {
                            /* Client inserts the address of the selected server in ’server identifier’.
                            MUST (after MUST SELECTING)*/
                            if((server_identifier == options_rx.server_identifier) &&
                               (dhcp_header->ciaddr == INADDR_ANY) ) /*’ciaddr’ MUST be zero */
                            {
                                /* Mark as bound */
                                ip_addr_pool->state = FNET_DHCP_SRV_ADDR_POOL_STATE_BOUND;
                                /* Send ACK */
                                message_type_tx = FNET_DHCP_OPTION_MSG_TYPE_ACK;
                            }
                        }
                        /* Otherwise, the message is a request to verify or extend an existing lease. */
                        else
                        {
                            if( ip_addr_pool->state == FNET_DHCP_SRV_ADDR_POOL_STATE_BOUND)
                            {
                                /* Update lease time */
                                _fnet_dhcp_srv_set_lease_time(dhcp_srv_if, ip_addr_pool, &options_rx);
                                /* Send ACK */
                                message_type_tx = FNET_DHCP_OPTION_MSG_TYPE_ACK;
                            }
                        }
                    }
                }
                else
                {
                    /* If the selected server is unable to satisfy the DHCPREQUEST message
                    (e.g., the requested network address has been allocated), the
                    server SHOULD respond with a DHCPNAK message.*/
                    if(options_rx.server_identifier)
                    {
                        /* Client inserts the address of the selected server in ’server identifier’*/
                        if(server_identifier == options_rx.server_identifier)
                        {
                            /* Send DHCPNAK */
                            message_type_tx = FNET_DHCP_OPTION_MSG_TYPE_NAK;
                        }
                    }
                }
                break;
            /* DHCPDECLINE message */
            case FNET_DHCP_OPTION_MSG_TYPE_DECLINE:
                /* RFC: If the server receives a DHCPDECLINE message, the client has
                discovered through some other means that the suggested network
                address is already in use. The server MUST mark the network address
                as not available.*/
                if(addr_pool_index != FNET_ERR)
                {
                    ip_addr_pool = &dhcp_srv_if->ip_addr_pool[addr_pool_index];

                    /* Must be allocated/offered by us */
                    if(ip_addr_pool->state == FNET_DHCP_SRV_ADDR_POOL_STATE_OFFERED)
                    {
                        /* Mark as not availble for lease time */
                        fnet_memset_zero(&ip_addr_pool->client_identifier, sizeof(ip_addr_pool->client_identifier));
                        ip_addr_pool->lease_time = dhcp_srv_if->lease_time;
                        ip_addr_pool->state_timestamp_ms = fnet_timer_get_ms();
                        ip_addr_pool->state = FNET_DHCP_SRV_ADDR_POOL_STATE_BOUND;
                    }
                }
                break;
            /* DHCPRELEASE message */
            case FNET_DHCP_OPTION_MSG_TYPE_RELEASE:
                /* RFC: Upon receipt of a DHCPRELEASE message, the server marks the network
                address as not allocated. The server SHOULD retain a record of the
                client’s initialization parameters for possible reuse in response to
                subsequent requests from the client.*/
                if(addr_pool_index != FNET_ERR)
                {
                    ip_addr_pool = &dhcp_srv_if->ip_addr_pool[addr_pool_index];

                    /* Must be allocated/offered by us */
                    if(ip_addr_pool->state != FNET_DHCP_SRV_ADDR_POOL_STATE_FREE)
                    {
                        ip_addr_pool->state = FNET_DHCP_SRV_ADDR_POOL_STATE_FREE;
                    }
                }
                break;
            case FNET_DHCP_OPTION_MSG_TYPE_INFORM:
                /* RFC: The server responds to a DHCPINFORM message by sending a DHCPACK
                message directly to the address given in the ’ciaddr’ field of the
                DHCPINFORM message. The server MUST NOT send a lease expiration time
                to the client and SHOULD NOT fill in ’yiaddr’.*/
                /* Send ACK */
                if(dhcp_header->ciaddr != INADDR_ANY)
                {
                    /* DHCPACK message with any local
                    configuration parameters appropriate for the client without:
                    allocating a new address, checking for an existing binding, filling
                    in ’yiaddr’ or including lease time parameters.*/
                    addr_pool_index = FNET_ERR;
                    message_type_tx = FNET_DHCP_OPTION_MSG_TYPE_ACK;
                }
                break;
            default: /* Ignore */
                break;
        }
    }

    /* Any TX */
    if(message_type_tx)
    {
        _fnet_dhcp_srv_send_message(dhcp_srv_if, server_identifier, message_type_tx, &options_rx, addr_pool_index);
    }
}

/************************************************************************
* DESCRIPTION: Send DHCPv4 client message.
************************************************************************/
static void _fnet_dhcp_srv_send_message(fnet_dhcp_srv_if_t *dhcp_if, fnet_ip4_addr_t server_identifier, fnet_uint8_t message_type, fnet_dhcp_srv_options_t *rx_options, fnet_int32_t addr_pool_index)
{
    FNET_ASSERT(dhcp_if != FNET_NULl);
    FNET_ASSERT(rx_options != FNET_NULl);
    FNET_ASSERT(ip_addr_pool != FNET_NULl);

    fnet_dhcp_header_t      *message = &dhcp_if->message; /* Points to the received DHCP client message.*/
    struct fnet_sockaddr    addr_send;
    fnet_size_t             message_length;
    struct fnet_in_addr     ip_address;
    fnet_uint8_t            *option_position;
    fnet_uint8_t            *option_position_tmp;
    fnet_bool_t             add_lease_time = FNET_FALSE;
    fnet_index_t            i;
    fnet_uint16_t           port_number = FNET_CFG_DHCP_CLN_PORT;

    /* == Common reply message parameters == */
    message->op = FNET_DHCP_OP_BOOTREPLY;       /* ’op’ BOOTREPLY */
    message->htype = FNET_DHCP_HTYPE_ETHERNET;  /* Ethernet */
    message->hlen = sizeof(fnet_mac_addr_t);    /* Hardware address length in octets */
    message->hops = 0;
    /* Use ’xid’ from client message */
    message->secs = 0;
    message->siaddr = INADDR_ANY;   /* IP address of next bootstrap server - NOT supported.*/
    /* ’flags’ from client DHCPDISCOVER/DHCPREQUEST message */
    /* ’giaddr’ from client DHCPDISCOVER/DHCPREQUEST message */
    /* ’chaddr’ from client DHCPDISCOVER/DHCPREQUEST message*/
    fnet_memset_zero(message->sname, sizeof(message->sname));       /* Clear 'sname' options.*/
    fnet_memset_zero(message->file, sizeof(message->file));         /* Clear 'file' options.*/
    fnet_memcpy(message->magic_cookie, fnet_dhcp_magic_cookie, sizeof(fnet_dhcp_magic_cookie));  /* Add "magic cookie" */
    fnet_memset_zero(message->options, sizeof(message->options));   /* Clear 'options' options.*/

    /* If the ’giaddr’ field in a DHCP message from a client is non-zero,
    the server sends any return messages to the ’DHCP server’ port on the
    BOOTP relay agent whose address appears in ’giaddr’. If the ’giaddr’
    field is zero and the ’ciaddr’ field is nonzero, then the server
    unicasts DHCPOFFER and DHCPACK messages to the address in ’ciaddr’.
    If ’giaddr’ is zero and ’ciaddr’ is zero, and the broadcast bit is
    set, then the server broadcasts DHCPOFFER and DHCPACK messages to
    0xffffffff. If the broadcast bit is not set and ’giaddr’ is zero and
    ’ciaddr’ is zero, then the server unicasts DHCPOFFER and DHCPACK
    messages to the client’s hardware address and ’yiaddr’ address. In
    all cases, when ’giaddr’ is zero, the server broadcasts any DHCPNAK
    messages to 0xffffffff.*/
    /* TBD sending to HW address */
    if(message->giaddr != INADDR_ANY)
    {
        ip_address.s_addr = message->giaddr;
        port_number = FNET_CFG_DHCP_SRV_PORT;
    }
    else if(message->ciaddr != INADDR_ANY)
    {
        ip_address.s_addr = message->ciaddr;
    }
    else
    {
        ip_address.s_addr = INADDR_BROADCAST;
    }

    /* Add DHCP options */
    option_position = message->options;

    /* Add DHCP message type option */
    option_position = _fnet_dhcp_srv_add_option(option_position, ((message->options + sizeof(message->options)) - option_position), FNET_DHCP_OPTION_MSG_TYPE, FNET_DHCP_OPTION_MSG_TYPE_LENGTH,  &message_type);
    if(option_position == FNET_NULL)
    {
        goto EXIT;
    }

    /* Add server identifier. A DHCP server always returns its own address in the ’server identifier’ option.*/
    option_position = _fnet_dhcp_srv_add_option(option_position, ((message->options + sizeof(message->options)) - option_position), FNET_DHCP_OPTION_SERVER_ID, FNET_DHCP_OPTION_SERVER_ID_LENGTH,  &server_identifier);
    if(option_position == FNET_NULL)
    {
        goto EXIT;
    }

    /* Message-type specific fields */
    switch(message_type)
    {
        case FNET_DHCP_OPTION_MSG_TYPE_OFFER:
            message->ciaddr = INADDR_ANY;
            if(addr_pool_index != FNET_ERR)
            {
                message->yiaddr = fnet_htonl(fnet_ntohl(dhcp_if->ip_addr_pool_start) + addr_pool_index); /* IP address offered to client*/
                add_lease_time = FNET_TRUE;
            }
            break;
        case FNET_DHCP_OPTION_MSG_TYPE_ACK:
            /* ’ciaddr’ from DHCPREQUEST */
            if(addr_pool_index != FNET_ERR)
            {
                message->yiaddr = fnet_htonl(fnet_ntohl(dhcp_if->ip_addr_pool_start) + addr_pool_index); /* IP address assigned to client*/
                if(rx_options->message_type == FNET_DHCP_OPTION_MSG_TYPE_REQUEST)
                {
                    add_lease_time = FNET_TRUE;
                }
            }
            break;
        case FNET_DHCP_OPTION_MSG_TYPE_NAK:
            message->ciaddr = INADDR_ANY;
            message->yiaddr = INADDR_ANY;
            break;
        default:
            goto EXIT; /* Wrong type, should never happen */
    }

    /* Add IP address lease time */
    if(add_lease_time == FNET_TRUE)
    {
        fnet_uint32_t lease_time = fnet_htonl(dhcp_if->ip_addr_pool[addr_pool_index].lease_time - ((fnet_timer_get_ms() -  (dhcp_if->ip_addr_pool[addr_pool_index].state_timestamp_ms) / FNET_TIMER_MS_IN_SEC)));
        option_position = _fnet_dhcp_srv_add_option(option_position, ((message->options + sizeof(message->options)) - option_position), FNET_DHCP_OPTION_LEASE, FNET_DHCP_OPTION_LEASE_LENGTH, &lease_time);
        if(option_position == FNET_NULL)
        {
            goto EXIT;
        }
    }

    if(message_type != FNET_DHCP_OPTION_MSG_TYPE_NAK) /* All others options MUST NOT for NAK */
    {
        /* Add (optional) parameters requested by the client. */
        for(i = 0; i < rx_options->request_list_length; i++)
        {
            option_position_tmp = FNET_NULL;
            switch(rx_options->request_list[i])
            {
                case FNET_DHCP_OPTION_SUBNETMASK:
                    option_position_tmp = _fnet_dhcp_srv_add_option(option_position, ((message->options + sizeof(message->options)) - option_position), FNET_DHCP_OPTION_SUBNETMASK, FNET_DHCP_OPTION_SUBNETMASK_LENGTH, &dhcp_if->subnet_mask);
                    break;
                case FNET_DHCP_OPTION_ROUTER:
                    option_position_tmp = _fnet_dhcp_srv_add_option(option_position, ((message->options + sizeof(message->options)) - option_position), FNET_DHCP_OPTION_ROUTER, FNET_DHCP_OPTION_ROUTER_LENGTH_MIN, &dhcp_if->gateway);
                    break;
#if FNET_CFG_DNS
                case FNET_DHCP_OPTION_DNS:
                    option_position_tmp = _fnet_dhcp_srv_add_option(option_position, ((message->options + sizeof(message->options)) - option_position), FNET_DHCP_OPTION_DNS, FNET_DHCP_OPTION_DNS_LENGTH_MIN, &dhcp_if->dns);
                    break;
#endif
                /*RFC: Times T1 and T2 are configurable by the server through options. T1
                defaults to (0.5 * duration_of_lease). */
                case FNET_DHCP_OPTION_T1:
                    if(add_lease_time == FNET_TRUE)
                    {
                        fnet_uint32_t   t1;
                        t1 = fnet_htonl(dhcp_if->ip_addr_pool[addr_pool_index].lease_time >> 1);
                        option_position_tmp = _fnet_dhcp_srv_add_option(option_position, ((message->options + sizeof(message->options)) - option_position), FNET_DHCP_OPTION_T1, FNET_DHCP_OPTION_T1_LENGTH, &t1);
                    }
                    break;
                /* T2 defaults to (0.875 * duration_of_lease).*/
                case FNET_DHCP_OPTION_T2:
                    if(add_lease_time == FNET_TRUE)
                    {
                        fnet_uint32_t   t2;
                        t2 = fnet_htonl(dhcp_if->ip_addr_pool[addr_pool_index].lease_time - (dhcp_if->ip_addr_pool[addr_pool_index].lease_time >> 3));
                        option_position_tmp = _fnet_dhcp_srv_add_option(option_position, ((message->options + sizeof(message->options)) - option_position), FNET_DHCP_OPTION_T2, FNET_DHCP_OPTION_T2_LENGTH, &t2);
                    }
                    break;
                default:
                    break;
            }
            if(option_position_tmp != FNET_NULL)
            {
                option_position = option_position_tmp;
            }
        }
    }

    /* End Option. It must be the last option*/
    *option_position = FNET_DHCP_OPTION_END;

    /* Send message. */
    fnet_memset_zero(&addr_send, sizeof(addr_send));
    ((struct fnet_sockaddr_in *)(&addr_send))->sin_family = AF_INET;
    ((struct fnet_sockaddr_in *)(&addr_send))->sin_port = port_number;
    ((struct fnet_sockaddr_in *)(&addr_send))->sin_addr = ip_address;

    message_length = (fnet_size_t)(option_position - (fnet_uint8_t *)message + 1);

    fnet_socket_sendto(dhcp_if->socket_srv, message, message_length, 0u, (struct fnet_sockaddr *) &addr_send, sizeof(addr_send));
EXIT:
    return;
}

/************************************************************************
* DESCRIPTION: Add DHCPv4 option.
* Returns:
*   - ponter to the position after the option.
*   - FNET_NULL if no space for the option (+ END option).
************************************************************************/
//TBD common for CLN and SRV
static fnet_uint8_t *_fnet_dhcp_srv_add_option(fnet_uint8_t *option_buffer, fnet_size_t option_buffer_size, fnet_uint8_t option_code, fnet_uint8_t option_length,  const void *option_value )
{
    FNET_ASSERT(option_buffer != FNET_NULl);
    FNET_ASSERT(option_value != FNET_NULl);

    fnet_uint8_t    *result = FNET_NULL;

    if(option_buffer_size >= (2U/*type+length*/ + 1U/* END option*/ + option_length))
    {
        option_buffer[0] = option_code;
        option_buffer[1] = option_length;
        fnet_memcpy(&option_buffer[2], option_value, (fnet_size_t)option_length);
        result = &option_buffer[2U + option_length];
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Gets pointer to the first option. "options" is updated to the next option.
* Returns NULL, if there is no option.
************************************************************************/
static fnet_uint8_t *_fnet_dhcp_srv_get_option(fnet_uint8_t **options, fnet_uint8_t *option_end)
{
    fnet_uint8_t *current_position = *options;
    fnet_uint8_t length;
    fnet_uint8_t *result = FNET_NULL;

    /* Check for the end of DHCP packet.
     * as we don't want try to access an unmapped memory.*/
    if(current_position + 1 >= option_end)
    {
        goto EXIT;
    }

    /* Skip pad options.
    The pad option can be used to cause subsequent fields to align on
    word boundaries. The code for the pad option is 0, and its length is 1 octet.*/
    while(*current_position == FNET_DHCP_OPTION_PAD)
    {
        if((++current_position) >= option_end)
        {
            goto EXIT;
        }
    }

    /* Check End option.
    he end option marks the end of valid information in the vendor
    field.  Subsequent octets should be filled with pad options.
    The code for the end option is 255, and its length is 1 octet.*/
    if(*current_position == FNET_DHCP_OPTION_END)
    {
        goto EXIT;
    }

    length = *(current_position + 1); /* Option length field (after 1-byte code field).*/
    /* Check Option Length overflow.*/
    if((current_position + length + 2/*Code+Len*/) > option_end)
    {
        goto EXIT;
    }

    *options = current_position + length + 2;

    result = current_position;

EXIT:

    return result;
}

/************************************************************************
* DESCRIPTION: Parse DHCP options from a received message.
************************************************************************/
static void _fnet_dhcp_srv_parse_options( fnet_uint8_t *option_buffer, fnet_size_t option_buffer_size, fnet_dhcp_srv_options_t *options )
{
    fnet_uint8_t *current_option;
    fnet_uint8_t *option_end = option_buffer + option_buffer_size - 1;

    while((current_option = _fnet_dhcp_srv_get_option(&option_buffer, option_end)) != FNET_NULL)
    {
        fnet_uint8_t *option_data = current_option + 2;
        fnet_uint8_t option_length = *(current_option + 1);

        switch(*current_option)
        {
            /* DHCP Message Type.
            This option is used to convey the type of the DHCP message.  The code
            for this option is 53, and its length is 1.  Legal values for this
            option are:
                    Value   Message Type
                    -----   ------------
                    1     DHCPDISCOVER
                    2     DHCPOFFER
                    3     DHCPREQUEST
                    4     DHCPDECLINE
                    5     DHCPACK
                    6     DHCPNAK
                    7     DHCPRELEASE
                    8     DHCPINFORM
            Code   Len  Type
            +-----+-----+-----+
            |  53 |  1  | 1-9 |
            +-----+-----+-----+*/
            case FNET_DHCP_OPTION_MSG_TYPE:
                if(option_length == FNET_DHCP_OPTION_MSG_TYPE_LENGTH)
                {
                    options->message_type = *option_data;
                }
                break;
            /*Requested IP Address.
            This option is used in a client request (DHCPDISCOVER) to allow the
            client to request that a particular IP address be assigned.
            The code for this option is 50, and its length is 4.
            Code   Len          Address
            +-----+-----+-----+-----+-----+-----+
            |  50 |  4  |  a1 |  a2 |  a3 |  a4 |
            +-----+-----+-----+-----+-----+-----+*/
            case FNET_DHCP_OPTION_REQ_ADDRESS:
                if(option_length == FNET_DHCP_OPTION_REQ_ADDRESS_LENGTH)
                {
                    fnet_memcpy(&options->requested_ip_address, option_data, sizeof(options->requested_ip_address));
                }
                break;
            /* Server Identifier.
            This option is used in DHCPOFFER and DHCPREQUEST messages, and may
            optionally be included in the DHCPACK and DHCPNAK messages.  DHCP
            servers include this option in the DHCPOFFER in order to allow the
            client to distinguish between lease offers.  DHCP clients use the
            contents of the 'server identifier' field as the destination address
            for any DHCP messages unicast to the DHCP server.  DHCP clients also
            indicate which of several lease offers is being accepted by including
            this option in a DHCPREQUEST message.
            The identifier is the IP address of the selected server.*/
            case FNET_DHCP_OPTION_SERVER_ID:
                if(option_length == FNET_DHCP_OPTION_SERVER_ID_LENGTH)
                {
                    fnet_memcpy(&options->server_identifier, option_data, sizeof(options->server_identifier));
                }
                break;
            /* Client-identifier.
            This option is used by DHCP clients to specify their unique
            identifier.  DHCP servers use this value to index their database of
            address bindings.  This value is expected to be unique for all
            clients in an administrative domain.
            Code   Len   Type  Client-Identifier
            +-----+-----+-----+-----+-----+---
            |  61 |  n  |  t1 |  i1 |  i2 | ...
            +-----+-----+-----+-----+-----+---*/
            case FNET_DHCP_OPTION_CLIENT_ID:
                if((option_length == FNET_DHCP_OPTION_CLIENT_ID_LENGTH)
                   && (option_data[0] == FNET_DHCP_HTYPE_ETHERNET)) /* Supported only Ethernet type */
                {
                    fnet_memcpy(&options->client_identifier, &option_data[1], sizeof(options->client_identifier));
                }
                break;
            /* Option Overload.
            This option is used to indicate that the DHCP 'sname' or 'file'
            fields are being overloaded by using them to carry DHCP options. A
            DHCP server inserts this option if the returned parameters will
            exceed the usual space allotted for options.
            If this option is present, the client interprets the specified
            additional fields after it concludes interpretation of the standard
            option fields.
            Value   Meaning
            -----   --------
            1     the 'file' field is used to hold options
            2     the 'sname' field is used to hold options
            3     both fields are used to hold options

            Code   Len  Value
            +-----+-----+-----+
            |  52 |  1  |1/2/3|
            +-----+-----+-----+*/
            case FNET_DHCP_OPTION_OVERLOAD:
                if(option_length == FNET_DHCP_OPTION_OVERLOAD_LENGTH)
                {
                    fnet_memcpy(&options->overload, option_data, sizeof(options->overload));
                }
                break;
            /* IP Address Lease Time.
            This option is used in a client request (DHCPDISCOVER or DHCPREQUEST)
            to allow the client to request a lease time for the IP address.  In a
            server reply (DHCPOFFER), a DHCP server uses this option to specify
            the lease time it is willing to offer.
            The time is in units of seconds, and is specified as a 32-bit
            unsigned integer.
            The code for this option is 51, and its length is 4.
                Code   Len         Lease Time
            +-----+-----+-----+-----+-----+-----+
            |  51 |  4  |  t1 |  t2 |  t3 |  t4 |
            +-----+-----+-----+-----+-----+-----+*/
            case FNET_DHCP_OPTION_LEASE:
                if(option_length == FNET_DHCP_OPTION_LEASE_LENGTH)
                {
                    fnet_memcpy(&options->lease_time, option_data, options->lease_time);
                    options->lease_time = fnet_ntohl(options->lease_time);
                }
                break;
            /* Parameter Request List
            This option is used by a DHCP client to request values for specified
            configuration parameters.  The list of requested parameters is
            specified as n octets, where each octet is a valid DHCP option code
            as defined in this document.
            The client MAY list the options in order of preference.  The DHCP
            server is not required to return the options in the requested order,
            but MUST try to insert the requested options in the order requested
            by the client.
            Code   Len   Option Codes
            +-----+-----+-----+-----+---
            |  55 |  n  |  c1 |  c2 | ...
            +-----+-----+-----+-----+--- */
            case FNET_DHCP_OPTION_PARAMETER_REQ_LIST:
                if(option_length > 0) /* Its minimum length is 1. */
                {
                    /* Check maximum length */
                    if(option_length > sizeof(options->request_list))
                    {
                        option_length = sizeof(options->request_list);
                    }

                    fnet_memcpy(options->request_list, option_data, option_length);
                    options->request_list_length = option_length;
                }
                break;
            default:
                break;
                /* Todo Other Options. */
        }
    }
}

/************************************************************************
* DESCRIPTION: Choose client address. Returns address pool index.
* FNET_ERR means there is no address in the address pool.
************************************************************************/
static fnet_int32_t _fnet_dhcp_srv_get_addr_pool(fnet_dhcp_srv_if_t *dhcp_srv_if, fnet_dhcp_header_t  *dhcp_header, fnet_mac_addr_t *client_identifier, fnet_ip4_addr_t requested_ip_address)
{
    FNET_ASSERT(dhcp_srv_if != FNET_NULL);
    FNET_ASSERT(dhcp_header != FNET_NULL);
    FNET_ASSERT(options != FNET_NULL);

    fnet_int32_t                result = FNET_ERR;
    fnet_index_t                i;
    fnet_dhcp_srv_addr_pool_t   *ip_addr_pool = FNET_NULL;

    /*RFC: If an address is available, the new address SHOULD be chosen as follows:
    o The client’s current address as recorded in the client’s current
    binding, ELSE
    o The client’s previous address as recorded in the client’s (now
    expired or released) binding, if that address is in the server’s
    pool of available addresses and not already allocated, ELSE*/
    for(i = 0; i < dhcp_srv_if->ip_addr_pool_size; i++)
    {
        ip_addr_pool = &dhcp_srv_if->ip_addr_pool[i];
        if(fnet_memcmp(client_identifier, ip_addr_pool->client_identifier, sizeof(fnet_mac_addr_t)) == 0)
        {
            result = i;
            break;
        }
    }

    if(result == FNET_ERR)
    {
        /* Conver to host endian */
        fnet_ip4_addr_t ip_addr_pool_start = fnet_ntohl(dhcp_srv_if->ip_addr_pool_start);
        requested_ip_address = fnet_ntohl(requested_ip_address);

        if(requested_ip_address >= ip_addr_pool_start)
        {
            i = requested_ip_address - ip_addr_pool_start;
            if(i < dhcp_srv_if->ip_addr_pool_size)
            {
                ip_addr_pool = &dhcp_srv_if->ip_addr_pool[i];
                /* o The address requested in the ’Requested IP Address’ option, if that
                address is valid and not already allocated, ELSE*/
                if(ip_addr_pool->state == FNET_DHCP_SRV_ADDR_POOL_STATE_FREE)
                {
                    result = i;
                }
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Get free client address from the pool. Returns address pool index.
* FNET_ERR means the pool is full.
************************************************************************/
static fnet_int32_t _fnet_dhcp_srv_get_addr_pool_free(fnet_dhcp_srv_if_t *dhcp_srv_if)
{
    FNET_ASSERT(dhcp_srv_if != FNET_NULL);

    fnet_int32_t                result = FNET_ERR;
    fnet_index_t                i;
    fnet_dhcp_srv_addr_pool_t   *ip_addr_pool;

    /* A new address allocated from the server’s pool of available addresses. */
    for(i = 0; i < dhcp_srv_if->ip_addr_pool_size; i++)
    {
        ip_addr_pool = &dhcp_srv_if->ip_addr_pool[i];
        if(ip_addr_pool->state == FNET_DHCP_SRV_ADDR_POOL_STATE_FREE)
        {
            result = i;
            break;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Choose an expiration time for the lease.
************************************************************************/
static void _fnet_dhcp_srv_set_lease_time(fnet_dhcp_srv_if_t *dhcp_srv_if, fnet_dhcp_srv_addr_pool_t  *ip_addr_pool, const fnet_dhcp_srv_options_t *options)
{
    FNET_ASSERT(dhcp_srv_if != FNET_NULL);
    FNET_ASSERT(ip_addr_pool != FNET_NULL);
    FNET_ASSERT(options != FNET_NULL);

    /*RFC: The server must also choose an expiration time for the lease, as
    follows:*/

    if(options->lease_time == 0)
    {
        /*o IF the client has not requested a specific lease in the
        DHCPDISCOVER message and the client already has an assigned network
        address, the server returns the lease expiration time previously
        assigned to that address (note that the client must explicitly
        request a specific lease to extend the expiration time on a
        previously assigned address), ELSE
        o IF the client has not requested a specific lease in the
        DHCPDISCOVER message and the client does not have an assigned
        network address, the server assigns a locally configured default
        lease time, ELSE */
        ip_addr_pool->lease_time = dhcp_srv_if->lease_time;
        ip_addr_pool->state_timestamp_ms = fnet_timer_get_ms();
    }
    else
    {
        /* o IF the client has requested a specific lease in the DHCPDISCOVER
        message (regardless of whether the client has an assigned network
        address), the server may choose either to return the requested
        lease (if the lease is acceptable to local policy) or select
        another lease. */
        ip_addr_pool->state_timestamp_ms = fnet_timer_get_ms();
        if(options->lease_time < dhcp_srv_if->lease_time)
        {
            ip_addr_pool->lease_time = options->lease_time;
        }
        else
        {
            ip_addr_pool->lease_time = dhcp_srv_if->lease_time;
        }
    }
}

/************************************************************************
* DESCRIPTION: This function returns FNET_TRUE if the DHCP server
*              is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_dhcp_srv_is_enabled(fnet_dhcp_srv_desc_t desc)
{
    struct fnet_dhcp_srv_if     *dhcp_if = (struct fnet_dhcp_srv_if *) desc;
    fnet_bool_t                 result;

    if(dhcp_if)
    {
        result = dhcp_if->is_enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Looks for a DHCP Server assigned to the specified network interface.
************************************************************************/
fnet_dhcp_srv_desc_t fnet_dhcp_srv_get_by_netif(fnet_netif_desc_t netif)
{
    fnet_dhcp_srv_if_t      *dhcp_if;
    fnet_index_t            i;
    fnet_dhcp_srv_desc_t    dhcp_desc = 0;

    if(netif)
    {
        fnet_service_mutex_lock();

        for(i = 0u; i < FNET_CFG_DHCP_SRV; i++)
        {
            dhcp_if = &fnet_dhcp_srv_if_list[i];

            if((dhcp_if->is_enabled == FNET_TRUE) && (dhcp_if->netif == netif))
            {
                dhcp_desc = (fnet_dhcp_srv_desc_t)dhcp_if;
                break;
            }
        }

        fnet_service_mutex_unlock();
    }

    return dhcp_desc;
}

#endif /* FNET_CFG_DHCP_SRV && FNET_CFG_IP4 */

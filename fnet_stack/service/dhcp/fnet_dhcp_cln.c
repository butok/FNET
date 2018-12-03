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
*  FNET DHCPv4 Client implementation.
*
***************************************************************************/

/*
* Boot file name (or two-step bootstrap procedure) is not supported.
*/

#include "fnet.h"

#if FNET_CFG_DHCP_CLN && FNET_CFG_IP4

#include "fnet_dhcp_prv.h"

#if FNET_CFG_DEBUG_DHCP_CLN && FNET_CFG_DEBUG
    #define FNET_DEBUG_DHCP_CLN   FNET_DEBUG
#else
    #define FNET_DEBUG_DHCP_CLN(...)    do{}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/
#define FNET_DHCP_CLN_LEASE_MAX             (0xFFFFFFFFU/(1000U*2U)) /* Maximum lease time value (cause of timer counter is in ms) */

#define FNET_DHCP_CLN_STATE_REBOOTING_SEND_TIMEOUT      (4U*1000U)          /*(ms) timeout for ACK => request retransmission.*/
#define FNET_DHCP_CLN_STATE_REBOOTING_TIMEOUT           (2U*FNET_DHCP_CLN_STATE_REBOOTING_SEND_TIMEOUT)  /*(ms) timeout to go to INIT state.*/
#define FNET_DHCP_CLN_STATE_RENEWING_SEND_TIMEOUT       (60U*1000U)                                   /*(ms) timeout for ACK => request retransmission.*/
#define FNET_DHCP_CLN_STATE_REBINDING_SEND_TIMEOUT      (60U*1000U)                                   /*(ms) timeout for ACK => request retransmission.*/

#define FNET_DHCP_CLN_STATE_REQUESTING_SEND_TIMEOUT     (4U*1000U)          /*(ms) timeout for ACK => request retransmission.*/
#define FNET_DHCP_CLN_STATE_REQUESTING_TIMEOUT          (4U*FNET_DHCP_CLN_STATE_REQUESTING_SEND_TIMEOUT) /*(ms) timeout to go to INIT state.*/
#define FNET_DHCP_CLN_STATE_PROBING_TIMEOUT             (1U*1000U)          /*(ms) timeout to go to INIT state.*/

/* Error strings.*/
#define FNET_DHCP_CLN_ERR_SOCKET_CREATION   "[DHCP cln] Socket creation error."
#define FNET_DHCP_CLN_ERR_SOCKET_BIND       "[DHCP cln] Socket Error during bind."
#define FNET_DHCP_CLN_ERR_IS_INITIALIZED    "[DHCP cln] No free server."
#define FNET_DHCP_CLN_ERR_PARAMS            "[DHCP cln] Wrong input parameters."
#define FNET_DHCP_CLN_ERR_SERVICE           "[DHCP cln] Service registration is failed."

#define FNET_DHCP_CLN_IS_TIMEOUT (-1)

/**************************************************************************/ /*!
 * @brief DHCP client states.@n
 * Used mainly for debugging purposes.
 ******************************************************************************/
typedef enum
{
    FNET_DHCP_CLN_STATE_DISABLED = 0,   /**< @brief The DHCP client service is not
                                        * initialized.*/
    FNET_DHCP_CLN_STATE_INIT,           /**< @brief The DHCP client service is initialized.
                                        * Sends DHCPDISCOVER message.@n
                                        * Signals the Discover callback event.*/
    FNET_DHCP_CLN_STATE_SELECTING,      /**< @brief Waits for the DHCPOFFER message.*/
    FNET_DHCP_CLN_STATE_REQUESTING,     /**< @brief Sends the DHCPREQUEST message.
                                        * Waits for the DHCPACK.*/
    FNET_DHCP_CLN_STATE_PROBING,        /**< @brief Probes and checks whether the address
                                        * is in use by some other device.*/
    FNET_DHCP_CLN_STATE_BOUND,          /**< @brief The DHCPACK message from
                                        * the DHCP server arrived.
                                        * The client parameters are set.@n
                                        * Signals the Updated callback event.*/
    FNET_DHCP_CLN_STATE_RENEWING,       /**< @brief T1 expired. Send the DHCPREQUEST
                                        * to a leasing server.*/
    FNET_DHCP_CLN_STATE_REBINDING,      /**< @brief T2 expired. Broadcast the DHCPREQUEST.*/
    FNET_DHCP_CLN_STATE_INIT_REBOOT,    /**< @brief The DHCP client service is initialized.
                                        * Sends the DHCPREQUEST message.@n
                                        * Signals the Discover callback event.*/
    FNET_DHCP_CLN_STATE_REBOOTING,      /**< @brief Sends the DHCPREQUEST message.
                                        * Waits for the DHCPACK.*/
} fnet_dhcp_cln_state_t;

/**************************************************************************/ /*!
 * Private DHCP options.
 ******************************************************************************/
struct fnet_dhcp_cln_options_prv
{
    fnet_uint8_t message_type;      /* The DHCP Message Type.
                                     * This option is used to convey the type of the
                                     * DHCP message.*/

#if !FNET_CFG_DHCP_CLN_BOOTP
    fnet_uint8_t  overload;         /* Overload Option.
                                     * If this option is present, the DHCP client interprets
                                     * the specified additional fields after it concludes
                                     * interpretation of the standard option fields.*/
#endif
};

/**************************************************************************/ /*!
 * All DHCP options are retrieved from a DHCP server.
 ******************************************************************************/
struct fnet_dhcp_cln_options_in
{
    struct fnet_dhcp_cln_options public_options;
    struct fnet_dhcp_cln_options_prv private_options;
};

/************************************************************************
*    DHCP message control structure.
*************************************************************************/
typedef struct
{
    fnet_uint8_t       *next_option_position;
    fnet_uint8_t       *end_position;
    fnet_dhcp_header_t header;
} fnet_dhcp_cln_message_t;

/************************************************************************
*    DHCP interface interface structure
*************************************************************************/
typedef struct fnet_dhcp_cln_if
{
    fnet_socket_t               socket_client;
    fnet_dhcp_cln_state_t       state;                      /* Current state.*/
    fnet_bool_t                 is_enabled;
#if !FNET_CFG_DHCP_CLN_BOOTP
    fnet_time_t                 state_timeout_ms;           /* Current State timeout (ticks).*/
    fnet_dhcp_cln_state_t       state_timeout_next_state;   /* Next state on state timeout.*/
    fnet_time_t                 lease_obtained_time_ms;
#endif
    fnet_time_t                 state_send_timeout_ms;      /* Current State send request timeout.*/
    fnet_time_t                 state_send_timestamp_ms;    /* Timestamp at which the client sent the REQUEST message */

    fnet_time_t                 response_timeout_ms;        /* Timeout for the response from the remote DHCP server, by default is set to FNET_CFG_DHCP_CLN_RESPONSE_TIMEOUT_MS */

    fnet_mac_addr_t             macaddr;
    fnet_uint32_t               xid;
    fnet_dhcp_cln_message_t     message;
    fnet_dhcp_cln_params_t              in_params;          /* Input user parameters.*/
    struct fnet_dhcp_cln_options_in     current_options;    /* Parsed options */
    struct fnet_dhcp_cln_options_in     offered_options;    /* Parsed options */
    fnet_netif_desc_t           netif;
    fnet_service_desc_t         service_descriptor;
    fnet_dhcp_cln_callback_t    callback_updated;           /* Optional ponter to the handler
                                                            * callback function, that is
                                                            * called when the DHCP client has
                                                            * updated the IP parameters.*/
    fnet_dhcp_cln_callback_t     callback_discover;          /* Optional pointer to the handler
                                                            * callback function, that is
                                                            * called when the DHCP client send
                                                            * the DHCP discover message.*/
    void                        *callback_discover_cookie;   /* Optional user-application specific parameter.
                                                            * It's passed to the callback_discover event handler callback..*/
    void                        *callback_updated_cookie;    /* Optional user-application specific parameter.
                                                            * It's passed to the callback_updated
                                                            * event handler callback.*/
} fnet_dhcp_cln_if_t;

/* DHCP client interface */
static fnet_dhcp_cln_if_t fnet_dhcp_cln_if_list[FNET_CFG_DHCP_CLN];

/* List of dhcp parameter/options we request.*/
static fnet_uint8_t fnet_dhcp_cln_parameter_request_list [] =
{
    FNET_DHCP_OPTION_SUBNETMASK,
    FNET_DHCP_OPTION_ROUTER,
#if FNET_CFG_DNS
    FNET_DHCP_OPTION_DNS,
#endif
#if FNET_CFG_DHCP_CLN_BROADCAST
    FNET_DHCP_OPTION_BROADCAST,
#endif
    FNET_DHCP_OPTION_LEASE,
    FNET_DHCP_OPTION_T1,
    FNET_DHCP_OPTION_T2
};

/************************************************************************
*     Function Prototypes
*************************************************************************/
static fnet_uint8_t *_fnet_dhcp_cln_next_option(fnet_dhcp_cln_message_t *message);
static void _fnet_dhcp_cln_parse_options(fnet_dhcp_cln_message_t *message, struct fnet_dhcp_cln_options_in *options);
static void _fnet_dhcp_cln_send_message(fnet_dhcp_cln_if_t *dhcp);
static fnet_ssize_t _fnet_dhcp_cln_receive_message( fnet_dhcp_cln_if_t *dhcp, struct fnet_dhcp_cln_options_in *options);
static void _fnet_dhcp_cln_apply_params(fnet_dhcp_cln_if_t *dhcp);
static void _fnet_dhcp_cln_set_state(fnet_dhcp_cln_if_t *dhcp, fnet_dhcp_cln_state_t state);
static void _fnet_dhcp_cln_poll(void *fnet_dhcp_cln_if_p);

#if FNET_CFG_DEBUG_DHCP_CLN && FNET_CFG_DEBUG/* Debug functions */
/************************************************************************
* Print DHCP current state. For debug needs.
************************************************************************/
static void _fnet_dhcp_cln_print_state( fnet_dhcp_cln_if_t *dhcp )
{
    FNET_DEBUG_DHCP_CLN("\nDHCP: State ");
    switch(dhcp->state)
    {
        case FNET_DHCP_CLN_STATE_DISABLED:
            FNET_DEBUG_DHCP_CLN("DISABLED");
            break;
        case FNET_DHCP_CLN_STATE_INIT:
            FNET_DEBUG_DHCP_CLN("INIT");
            break;
        case FNET_DHCP_CLN_STATE_SELECTING:
            FNET_DEBUG_DHCP_CLN("SELECTING");
            break;
        case FNET_DHCP_CLN_STATE_BOUND:
            FNET_DEBUG_DHCP_CLN("BOUND");
            break;
#if !FNET_CFG_DHCP_CLN_BOOTP
        case FNET_DHCP_CLN_STATE_REQUESTING:
            FNET_DEBUG_DHCP_CLN("REQUESTING");
            break;
        case FNET_DHCP_CLN_STATE_PROBING:
            FNET_DEBUG_DHCP_CLN("PROBING");
            break;
        case FNET_DHCP_CLN_STATE_RENEWING:
            FNET_DEBUG_DHCP_CLN("RENEWING");
            break;
        case FNET_DHCP_CLN_STATE_REBINDING:
            FNET_DEBUG_DHCP_CLN("REBINDING");
            break;
        case FNET_DHCP_CLN_STATE_INIT_REBOOT:
            FNET_DEBUG_DHCP_CLN("INIT_REBOOT");
            break;
        case FNET_DHCP_CLN_STATE_REBOOTING:
            FNET_DEBUG_DHCP_CLN("REBOOTING");
            break;
#endif /* !FNET_CFG_DHCP_CLN_BOOTP */
    }
}

/************************************************************************
* Print DHCP options. For debug needs.
************************************************************************/
static void _fnet_dhcp_cln_print_options( struct fnet_dhcp_cln_options_in *options )
{
    fnet_uint8_t ip_str[FNET_IP4_ADDR_STR_SIZE];

    FNET_DEBUG_DHCP_CLN("DHCP: Options:");
    FNET_DEBUG_DHCP_CLN(" message_type \t\t %02X", options->private_options.message_type);

    fnet_inet_ntoa(*(struct fnet_in_addr *)(&options->public_options.ip_address), ip_str);
    FNET_DEBUG_DHCP_CLN(" ip_address \t\t %s", ip_str);
    fnet_inet_ntoa(*(struct fnet_in_addr *)(&options->public_options.dhcp_server), ip_str);
    FNET_DEBUG_DHCP_CLN(" dhcp_server \t %s", ip_str);
    fnet_inet_ntoa(*(struct fnet_in_addr *)(&options->public_options.netmask), ip_str);
    FNET_DEBUG_DHCP_CLN(" netmask \t\t %s", ip_str);
#if FNET_CFG_DHCP_CLN_BROADCAST
    fnet_inet_ntoa(*(struct fnet_in_addr *)(&options->public_options.broadcast), ip_str);
    FNET_DEBUG_DHCP_CLN(" broadcast \t\t %s", ip_str);
#endif
    fnet_inet_ntoa(*(struct fnet_in_addr *)(&options->public_options.gateway), ip_str);
    FNET_DEBUG_DHCP_CLN(" gateway \t\t %s", ip_str);

    FNET_DEBUG_DHCP_CLN(" t1 \t\t\t 0x%08X", fnet_ntohl(options->public_options.t1));
    FNET_DEBUG_DHCP_CLN(" t2 \t\t\t 0x%08X", fnet_ntohl(options->public_options.t2));
    FNET_DEBUG_DHCP_CLN(" lease_time \t\t 0x%08X", options->public_options.lease_time);

#if !FNET_CFG_DHCP_CLN_BOOTP

    FNET_DEBUG_DHCP_CLN(" overload \t\t 0x%02X", options->private_options.overload);

#endif
}

#else

#define _fnet_dhcp_cln_print_state(x) do{}while(0)
#define _fnet_dhcp_cln_print_options(x) do{}while(0)

#endif /* FNET_CFG_DEBUG_DHCP_CLN && FNET_CFG_DEBUG */

/************************************************************************
* Go to the next DHCP option.
************************************************************************/
static fnet_uint8_t *_fnet_dhcp_cln_next_option( fnet_dhcp_cln_message_t *message )
{
    fnet_uint8_t *current_position = message->next_option_position;
    fnet_uint8_t length;
    fnet_uint8_t *result = FNET_NULL;

    /* Check for the end of DHCP packet.
     * as we don't want try to access an unmapped memory.*/
    if(current_position + 1 >= message->end_position)
    {
        goto EXIT;
    }

    /* Skip pad options.*/
    while(*current_position == FNET_DHCP_OPTION_PAD)
    {
        if(++current_position >= message->end_position)
        {
            goto EXIT;
        }
    }

    /* Check End option.*/
    if(*current_position == FNET_DHCP_OPTION_END)
    {
        goto EXIT;
    }

    length = *(current_position + 1);
    /* Check Option Length overflow.*/
    if(current_position + length + 2 > message->end_position)
    {
        goto EXIT;
    }

    message->next_option_position = current_position + length + 2;

    result = current_position;

EXIT:

    return result;
}

/************************************************************************
*  Parse DHCP options from message.
************************************************************************/
static void _fnet_dhcp_cln_parse_options( fnet_dhcp_cln_message_t *message, struct fnet_dhcp_cln_options_in *options )
{
    fnet_uint8_t *current_option;

    while((current_option = _fnet_dhcp_cln_next_option(message)) != 0)
    {
        fnet_uint8_t *option_data = current_option + 2;
        fnet_uint8_t option_length = *(current_option + 1);

        switch(*current_option)
        {
            case FNET_DHCP_OPTION_SERVER_ID:
                if(option_length == FNET_DHCP_OPTION_SERVER_ID_LENGTH)
                {
                    options->public_options.dhcp_server.s_addr = *(fnet_ip4_addr_t *)option_data;
                }
                break;
            case FNET_DHCP_OPTION_SUBNETMASK:
                if(option_length == FNET_DHCP_OPTION_SUBNETMASK_LENGTH)
                {
                    options->public_options.netmask.s_addr = *(fnet_ip4_addr_t *)option_data;
                }
                break;
            case FNET_DHCP_OPTION_ROUTER:
                if(option_length >= FNET_DHCP_OPTION_ROUTER_LENGTH_MIN)
                {
                    options->public_options.gateway.s_addr = *(fnet_ip4_addr_t *)option_data;
                }
                break;
#if FNET_CFG_DHCP_CLN_BROADCAST
            case FNET_DHCP_OPTION_BROADCAST:
                if(option_length == FNET_DHCP_OPTION_BROADCAST_LENGTH)
                {
                    options->public_options.broadcast.s_addr = *(fnet_ip4_addr_t *)option_data;
                }
                break;
#endif
#if FNET_CFG_DNS
            case FNET_DHCP_OPTION_DNS:
                if(option_length >= FNET_DHCP_OPTION_DNS_LENGTH_MIN)
                {
                    options->public_options.dns.s_addr = *(fnet_ip4_addr_t *)option_data;
                }
                break;
#endif

#if !FNET_CFG_DHCP_CLN_BOOTP

            case FNET_DHCP_OPTION_MSG_TYPE:
                if(option_length == FNET_DHCP_OPTION_MSG_TYPE_LENGTH)
                {
                    options->private_options.message_type = *option_data;
                }
                break;
            case FNET_DHCP_OPTION_LEASE:
                if(option_length == FNET_DHCP_OPTION_LEASE_LENGTH)
                {
                    options->public_options.lease_time = *(fnet_uint32_t *)option_data;
                }
                break;
            case FNET_DHCP_OPTION_OVERLOAD:
                if((option_length == FNET_DHCP_OPTION_OVERLOAD_LENGTH) && (*option_data <= FNET_DHCP_OPTION_OVERLOAD_BOTH))
                {
                    options->private_options.overload = *option_data;
                }
                break;
            case FNET_DHCP_OPTION_T1:
                if(option_length == FNET_DHCP_OPTION_T1_LENGTH)
                {
                    options->public_options.t1 = *(fnet_uint32_t *)option_data;
                }
                break;
            case FNET_DHCP_OPTION_T2:
                if(option_length == FNET_DHCP_OPTION_T2_LENGTH)
                {
                    options->public_options.t2 = *(fnet_uint32_t *)option_data;
                }
                break;
#endif /* !FNET_CFG_DHCP_CLN_BOOTP */
            default:
                break;
                /* Todo Other Options. */
        }
    }
}

/************************************************************************
*  Send DHCPv4 client message.
************************************************************************/
static void _fnet_dhcp_cln_send_message( fnet_dhcp_cln_if_t *dhcp )
{
    fnet_dhcp_cln_message_t *message = &dhcp->message;
    struct fnet_sockaddr    addr_send;
    fnet_size_t             length;
    struct fnet_in_addr     ip_address;
    fnet_uint8_t            *option_position;
#if !FNET_CFG_DHCP_CLN_BOOTP
    fnet_uint16_t           max_message_size;
    fnet_uint8_t            client_id[FNET_DHCP_OPTION_CLIENT_ID_LENGTH];
    fnet_uint8_t            message_type;
#endif

    fnet_memset_zero(&message->header, sizeof(message->header));
    message->header.op = FNET_DHCP_OP_BOOTREQUEST;
    message->header.htype = FNET_DHCP_HTYPE_ETHERNET; /* Ethernet */
    message->header.hlen = sizeof(dhcp->macaddr);
    message->header.xid = fnet_htonl(dhcp->xid); /* TBD PFI */
    message->header.flags = FNET_HTONS(FNET_DHCP_FLAGS_BROADCAST);

    fnet_memcpy(message->header.chaddr, dhcp->macaddr, sizeof(dhcp->macaddr)); /* Client HW address */

    /* Add "magic cookie" */
    fnet_memcpy(message->header.magic_cookie, fnet_dhcp_magic_cookie, sizeof(fnet_dhcp_magic_cookie));

    /* Add DHCP options */
    option_position = message->header.options;

    /* Add options */
    switch(dhcp->state)
    {
#if FNET_CFG_DHCP_CLN_BOOTP
        case FNET_DHCP_CLN_STATE_SELECTING: /*=>Requesting*/
            ip_address.s_addr = INADDR_BROADCAST;
            break;
#else /* DHCP */
        case FNET_DHCP_CLN_STATE_REQUESTING: /*=>Requesting*/
            ip_address.s_addr = INADDR_BROADCAST;

            option_position = _fnet_dhcp_add_option(option_position, ((message->header.options + sizeof(message->header.options)) - option_position), FNET_DHCP_OPTION_REQ_ADDRESS, FNET_DHCP_OPTION_REQ_ADDRESS_LENGTH,  &dhcp->offered_options.public_options.ip_address);
            if(option_position == FNET_NULL)
            {
                goto EXIT;
            }

            option_position = _fnet_dhcp_add_option(option_position, ((message->header.options + sizeof(message->header.options)) - option_position), FNET_DHCP_OPTION_SERVER_ID, FNET_DHCP_OPTION_SERVER_ID_LENGTH,  &dhcp->offered_options.public_options.dhcp_server);
            if(option_position == FNET_NULL)
            {
                goto EXIT;
            }
            message_type = FNET_DHCP_OPTION_MSG_TYPE_REQUEST;
            break;
        case FNET_DHCP_CLN_STATE_PROBING:
            ip_address = dhcp->current_options.public_options.dhcp_server; /* Send REQUEST to leasing server*/
            message->header.ciaddr = dhcp->current_options.public_options.ip_address.s_addr;
            message_type = FNET_DHCP_OPTION_MSG_TYPE_DECLINE; /* DHCPDECLINE - Client to server indicating network address is already in use.*/
            break;
        case FNET_DHCP_CLN_STATE_SELECTING:
            ip_address.s_addr = INADDR_BROADCAST;
            message_type = FNET_DHCP_OPTION_MSG_TYPE_DISCOVER;
            break;
        case FNET_DHCP_CLN_STATE_REBOOTING: /*=>Requesting*/
            ip_address.s_addr = INADDR_BROADCAST;
            option_position = _fnet_dhcp_add_option(option_position, ((message->header.options + sizeof(message->header.options)) - option_position), FNET_DHCP_OPTION_REQ_ADDRESS, FNET_DHCP_OPTION_REQ_ADDRESS_LENGTH, &dhcp->in_params.requested_ip_address);
            if(option_position == FNET_NULL)
            {
                goto EXIT;
            }
            message_type = FNET_DHCP_OPTION_MSG_TYPE_REQUEST;
            break;
        case FNET_DHCP_CLN_STATE_RENEWING:
            ip_address = dhcp->current_options.public_options.dhcp_server; /* Send REQUEST to leasing server*/
            message->header.ciaddr = dhcp->current_options.public_options.ip_address.s_addr;
            message_type = FNET_DHCP_OPTION_MSG_TYPE_REQUEST;
            break;
        case FNET_DHCP_CLN_STATE_REBINDING:
            ip_address.s_addr = INADDR_BROADCAST;
            message_type = FNET_DHCP_OPTION_MSG_TYPE_REQUEST;
            message->header.ciaddr = dhcp->current_options.public_options.ip_address.s_addr;
            break;
        case FNET_DHCP_CLN_STATE_DISABLED:
            ip_address = dhcp->current_options.public_options.dhcp_server;
            message->header.ciaddr = dhcp->current_options.public_options.ip_address.s_addr;
            message_type = FNET_DHCP_OPTION_MSG_TYPE_RELEASE;
            break;
#endif
        default:
            return;
    }

#if FNET_CFG_DHCP_CLN_BOOTP

    /* Record time when client sent the DHCPREQUEST */
    dhcp->state_send_timestamp_ms = fnet_timer_get_ms();

#else /* DHCP */

    option_position = _fnet_dhcp_add_option(option_position, ((message->header.options + sizeof(message->header.options)) - option_position), FNET_DHCP_OPTION_MSG_TYPE, FNET_DHCP_OPTION_MSG_TYPE_LENGTH, &message_type);
    if(option_position == FNET_NULL)
    {
        goto EXIT;
    }

    if((message_type == FNET_DHCP_OPTION_MSG_TYPE_REQUEST) || (message_type == FNET_DHCP_OPTION_MSG_TYPE_DISCOVER))
    {
        /* Record time when client sent the DHCPREQUEST */
        dhcp->state_send_timestamp_ms = fnet_timer_get_ms();

        /* Request a lease time for the IP address */
        if(dhcp->in_params.requested_lease_time)
        {
            option_position = _fnet_dhcp_add_option(option_position, ((message->header.options + sizeof(message->header.options)) - option_position), FNET_DHCP_OPTION_LEASE, FNET_DHCP_OPTION_LEASE_LENGTH, &dhcp->in_params.requested_lease_time);
            if(option_position == FNET_NULL)
            {
                goto EXIT;
            }
        }
        /* Add Parameter Request list */
        option_position = _fnet_dhcp_add_option(option_position, ((message->header.options + sizeof(message->header.options)) - option_position), FNET_DHCP_OPTION_PARAMETER_REQ_LIST, sizeof(fnet_dhcp_cln_parameter_request_list), &fnet_dhcp_cln_parameter_request_list);
        if(option_position == FNET_NULL)
        {
            goto EXIT;
        }
    }

    /* Add Maximum DHCP message size option. */
    max_message_size = fnet_htons(sizeof(fnet_dhcp_header_t));
    option_position = _fnet_dhcp_add_option(option_position, ((message->header.options + sizeof(message->header.options)) - option_position), FNET_DHCP_OPTION_MESSAGE_SIZE, FNET_DHCP_OPTION_MESSAGE_SIZE_LENGTH, &max_message_size);
    if(option_position == FNET_NULL)
    {
        goto EXIT;
    }

    /* Client ID (MAC) option */
    client_id[0] = FNET_DHCP_HTYPE_ETHERNET; /* Ethernet type*/
    fnet_memcpy(&client_id[1], dhcp->macaddr, sizeof(dhcp->macaddr));
    option_position = _fnet_dhcp_add_option(option_position, ((message->header.options + sizeof(message->header.options)) - option_position), FNET_DHCP_OPTION_CLIENT_ID, FNET_DHCP_OPTION_CLIENT_ID_LENGTH, &client_id);
    if(option_position == FNET_NULL)
    {
        goto EXIT;
    }

#endif /* FNET_CFG_DHCP_CLN_BOOTP */

    /* End Option */
    *option_position = FNET_DHCP_OPTION_END;

    /* Send message. */
    fnet_memset_zero(&addr_send, sizeof(addr_send));
    ((struct fnet_sockaddr_in *)(&addr_send))->sin_family = AF_INET;
    ((struct fnet_sockaddr_in *)(&addr_send))->sin_port = FNET_CFG_DHCP_SRV_PORT;
    ((struct fnet_sockaddr_in *)(&addr_send))->sin_addr = ip_address;

    length = (fnet_size_t)(option_position - (fnet_uint8_t *) &message->header + 1);

    fnet_socket_sendto(dhcp->socket_client, &message->header, length, 0u, (struct fnet_sockaddr *) &addr_send, sizeof(addr_send));
EXIT:
    return;
}

/************************************************************************
*  Receive DHCP message (non blocking).
************************************************************************/
static fnet_ssize_t _fnet_dhcp_cln_receive_message( fnet_dhcp_cln_if_t *dhcp, struct fnet_dhcp_cln_options_in *options )
{
    fnet_ssize_t            size;
    struct fnet_sockaddr    addr_from;
    fnet_dhcp_header_t      *dhcp_header = &dhcp->message.header;
    fnet_size_t             addr_len = sizeof(addr_from);

    size = fnet_socket_recvfrom(dhcp->socket_client, dhcp_header, sizeof(fnet_dhcp_header_t), 0U, &addr_from, &addr_len);

    if((fnet_timer_get_ms() - dhcp->state_send_timestamp_ms) < dhcp->state_send_timeout_ms)
    {
        if((size < (fnet_int32_t)(sizeof(fnet_dhcp_header_t) - FNET_DHCP_OPTIONS_LENGTH))
           || (dhcp_header->op !=  FNET_DHCP_OP_BOOTREPLY) /* BOOTREPLY is used in the 'op' field of each DHCP message sent from a server to a client.*/
           || (dhcp_header->xid != fnet_htonl(dhcp->xid))  /* Is message for us? */
           || (dhcp_header->hlen != sizeof(dhcp->macaddr))
           || (fnet_memcmp(dhcp_header->chaddr, dhcp->macaddr, sizeof(dhcp->macaddr)) != 0)
           || (fnet_memcmp(&dhcp_header->magic_cookie[0], fnet_dhcp_magic_cookie,
                           sizeof(fnet_dhcp_magic_cookie)) != 0) )
        {
            size = 0; /* Ignore. */
        }
        else
        {
            fnet_memset_zero(options, sizeof(*options));                        /* Clear old options.*/
            options->public_options.ip_address.s_addr = dhcp_header->yiaddr;    /* our IP address */

            /* Parse options field */
            dhcp->message.next_option_position = &dhcp_header->options[0];
            dhcp->message.end_position = (fnet_uint8_t *) dhcp_header + size - 1;

            _fnet_dhcp_cln_parse_options(&dhcp->message, options);

#if !FNET_CFG_DHCP_CLN_BOOTP
            /* Parse overload options in sname/file */
            if(options->private_options.overload)
            {
                if(options->private_options.overload & FNET_DHCP_OPTION_OVERLOAD_SNAME)
                {
                    dhcp->message.next_option_position = &dhcp_header->sname[0];
                }
                else
                {
                    dhcp->message.next_option_position = &dhcp_header->file[0];
                }

                if(options->private_options.overload & FNET_DHCP_OPTION_OVERLOAD_FILE)
                {
                    dhcp->message.end_position = &dhcp_header->file[128 - 1];
                }
                else
                {
                    dhcp->message.end_position = &dhcp_header->sname[64 - 1];
                }

                _fnet_dhcp_cln_parse_options(&dhcp->message, options);
            }
#endif
        }
    }
    else
    {
        size = FNET_DHCP_CLN_IS_TIMEOUT;
    }

    return size;
}

/************************************************************************
*  Change state of DHCP client.
************************************************************************/
static void _fnet_dhcp_cln_set_state( fnet_dhcp_cln_if_t *dhcp, fnet_dhcp_cln_state_t state )
{
    dhcp->state = state;

    _fnet_dhcp_cln_print_state(dhcp);

    switch(state)
    {
#if !FNET_CFG_DHCP_CLN_BOOTP
        case FNET_DHCP_CLN_STATE_INIT_REBOOT:
#endif
        case FNET_DHCP_CLN_STATE_INIT:

            if(fnet_netif_get_ip4_addr_type(dhcp->netif) != FNET_NETIF_IP_ADDR_TYPE_AUTOCONFIGURABLE) /* Allow Auto-IP simultaneous run */
            {
                fnet_netif_set_ip4_addr(dhcp->netif, INADDR_ANY, INADDR_ANY);   /* Set zero address. DHCP messages broadcast
                                                                                * by a client prior to that client obtaining
                                                                                * its IP address must have the source address
                                                                                * field in IP header set to 0.*/
            }
            dhcp->xid++; /* Todo must be random.*/
            break;
        case FNET_DHCP_CLN_STATE_SELECTING:
            _fnet_dhcp_cln_send_message(dhcp);                       /* Send DISCOVER */
            dhcp->state_send_timeout_ms = dhcp->response_timeout_ms; /* Wait OFFER */
            break;
        case FNET_DHCP_CLN_STATE_BOUND:
#if !FNET_CFG_DHCP_CLN_BOOTP
            dhcp->state_timeout_next_state = FNET_DHCP_CLN_STATE_RENEWING;
            dhcp->lease_obtained_time_ms = dhcp->state_send_timestamp_ms;

            if(dhcp->current_options.public_options.t1 == FNET_HTONL(FNET_DHCP_CLN_LEASE_INFINITY))
            {
                dhcp->state_timeout_ms = FNET_DHCP_CLN_LEASE_INFINITY;
            }
            else
            {
                dhcp->state_timeout_ms = (fnet_ntohl(dhcp->current_options.public_options.t1) * 1000U);
            }
#endif /* !FNET_CFG_DHCP_CLN_BOOTP */
            break;
#if !FNET_CFG_DHCP_CLN_BOOTP
        case FNET_DHCP_CLN_STATE_REQUESTING:
            _fnet_dhcp_cln_send_message(dhcp); /* Send REQUEST.*/
            dhcp->state_timeout_next_state = FNET_DHCP_CLN_STATE_INIT;
            dhcp->lease_obtained_time_ms = dhcp->state_send_timestamp_ms;
            dhcp->state_send_timeout_ms = FNET_DHCP_CLN_STATE_REQUESTING_SEND_TIMEOUT; /* Wait ACK */
            dhcp->state_timeout_ms = FNET_DHCP_CLN_STATE_REQUESTING_TIMEOUT;
            break;
        case FNET_DHCP_CLN_STATE_PROBING:
            fnet_arp_send_request(dhcp->netif, dhcp->current_options.public_options.ip_address.s_addr ); /* Send ARP probe.*/
            /* Record time when client sent the ARP brobe */
            dhcp->state_send_timestamp_ms = fnet_timer_get_ms();
            dhcp->state_timeout_ms = FNET_DHCP_CLN_STATE_PROBING_TIMEOUT;
            break;
        case FNET_DHCP_CLN_STATE_REBOOTING:
            _fnet_dhcp_cln_send_message(dhcp);               /* Send REQUEST.*/
            dhcp->state_timeout_next_state = FNET_DHCP_CLN_STATE_INIT;
            dhcp->lease_obtained_time_ms = dhcp->state_send_timestamp_ms; /* To follow state machine rules.*/
            dhcp->state_timeout_ms = FNET_DHCP_CLN_STATE_REBOOTING_TIMEOUT;
            dhcp->state_send_timeout_ms = FNET_DHCP_CLN_STATE_REBOOTING_SEND_TIMEOUT;
            break;
        case FNET_DHCP_CLN_STATE_RENEWING:
            _fnet_dhcp_cln_send_message(dhcp); /* Send REQUEST.*/
            dhcp->state_timeout_next_state = FNET_DHCP_CLN_STATE_REBINDING;
            dhcp->state_timeout_ms = (fnet_ntohl(dhcp->current_options.public_options.t2) * 1000U);
            dhcp->state_send_timeout_ms = FNET_DHCP_CLN_STATE_RENEWING_SEND_TIMEOUT;
            break;
        case FNET_DHCP_CLN_STATE_REBINDING:
            _fnet_dhcp_cln_send_message(dhcp); /* Send REQUEST.*/
            dhcp->state_timeout_next_state = FNET_DHCP_CLN_STATE_INIT;
            dhcp->state_timeout_ms = (fnet_ntohl(dhcp->current_options.public_options.lease_time) * 1000U);
            dhcp->state_send_timeout_ms = FNET_DHCP_CLN_STATE_REBINDING_SEND_TIMEOUT;
            break;
#endif /* !FNET_CFG_DHCP_CLN_BOOTP */
        case FNET_DHCP_CLN_STATE_DISABLED:
#if !FNET_CFG_DHCP_CLN_BOOTP
            /* Graceful shutdown. Sends the RELEASE message. Frees the allocated resources.*/
            if(dhcp->current_options.public_options.ip_address.s_addr)  /* If address was obtained before.*/
            {
                _fnet_dhcp_cln_send_message(dhcp);                           /* Send RELEASE */
            }

            if(fnet_netif_get_ip4_addr_type(dhcp->netif) == FNET_NETIF_IP_ADDR_TYPE_DHCP)   /* If address is DHCP => do not use it. */
            {
                fnet_netif_set_ip4_addr(dhcp->netif, INADDR_ANY, INADDR_ANY);    /* Set zero address */
            }
#endif
            dhcp->is_enabled = FNET_FALSE;
            fnet_socket_close(dhcp->socket_client);
            fnet_service_unregister(dhcp->service_descriptor); /* Delete service. */
            break;
        default:
            break;  /* do nothing, avoid compiler warning "enumeration value not handled in switch" */
    }
}

/************************************************************************
*  Apply DHCP parameters to the interface.
************************************************************************/
static void _fnet_dhcp_cln_apply_params(fnet_dhcp_cln_if_t *dhcp)
{
    /* Apply parameters. */
    fnet_netif_set_ip4_addr(dhcp->netif, dhcp->current_options.public_options.ip_address.s_addr, dhcp->current_options.public_options.netmask.s_addr);
    fnet_netif_set_ip4_gateway(dhcp->netif, dhcp->current_options.public_options.gateway.s_addr);
#if FNET_CFG_DNS
    fnet_netif_set_ip4_dns(dhcp->netif, dhcp->current_options.public_options.dns.s_addr);
#endif
    fnet_netif_set_ip4_addr_type(dhcp->netif, FNET_NETIF_IP_ADDR_TYPE_DHCP);

    _fnet_dhcp_cln_set_state(dhcp, FNET_DHCP_CLN_STATE_BOUND); /* => BOUND */
    /* Rise event. */
    if(dhcp->callback_updated)
    {
        dhcp->callback_updated((fnet_dhcp_cln_desc_t)dhcp, dhcp->netif, dhcp->callback_updated_cookie);
    }
}

/************************************************************************
*  DHCP client state machine.
************************************************************************/
/*
State-transition diagram for DHCP clients [RFC 2131]:
 --------                               -------
|        | +-------------------------->|       |<-------------------+
| INIT-  | |     +-------------------->| INIT  |                    |
| REBOOT |DHCPNAK/         +---------->|       |<---+               |
|        |Restart|         |            -------     |               |
 --------  |  DHCPNAK/     |               |                        |
    |      Discard offer   |      -/Send DHCPDISCOVER               |
-/Send DHCPREQUEST         |               |                        |
    |      |     |      DHCPACK            v        |               |
 -----------     |   (not accept.)/   -----------   |               |
|           |    |  Send DHCPDECLINE |           |                  |
| REBOOTING |    |         |         | SELECTING |<----+            |
|           |    |        /          |           |     |DHCPOFFER/  |
 -----------     |       /            -----------   |  |Collect     |
    |            |      /                  |   |       |  replies   |
DHCPACK/         |     /  +----------------+   +-------+            |
Record lease, set|    |   v   Select offer/                         |
timers T1, T2   ------------  send DHCPREQUEST      |               |
    |   +----->|            |             DHCPNAK, Lease expired/   |
    |   |      | REQUESTING |                  Halt network         |
    DHCPOFFER/ |            |                       |               |
    Discard     ------------                        |               |
    |   |        |        |                   -----------           |
    |   +--------+     DHCPACK/              |           |          |
    |              Record lease, set    -----| REBINDING |          |
    |                timers T1, T2     /     |           |          |
    |                     |        DHCPACK/   -----------           |
    |                     v     Record lease, set   ^               |
    +----------------> -------      /timers T1,T2   |               |
               +----->|       |<---+                |               |
               |      | BOUND |<---+                |               |
  DHCPOFFER, DHCPACK, |       |    |            T2 expires/   DHCPNAK/
   DHCPNAK/Discard     -------     |             Broadcast  Halt network
               |       | |         |            DHCPREQUEST         |
               +-------+ |        DHCPACK/          |               |
                    T1 expires/   Record lease, set |               |
                 Send DHCPREQUEST timers T1, T2     |               |
                 to leasing server |                |               |
                         |   ----------             |               |
                         |  |          |------------+               |
                         +->| RENEWING |                            |
                            |          |----------------------------+
                             ----------                                */

/*  BOOTP:
                                             BOOTREPLY/
  -------   -/Send         ---------------   Configure client   -------
 |       |  BOOTPREQUEST  |               |  and StopTask      |       |
 | INIT  |--------------->| BOOTP_REQUEST |------------------->| BOUND |
 |       |                | (SELECTING)   |                    |       |
  -------                  ---------------                      -------
     ^                           |
     |         Timeout/-         |
     +---------------------------+                                     */

static void _fnet_dhcp_cln_poll( void *fnet_dhcp_cln_if_p )
{
    fnet_dhcp_cln_if_t              *dhcp = (fnet_dhcp_cln_if_t *)fnet_dhcp_cln_if_p;
    struct fnet_dhcp_cln_options_in options;
    fnet_ssize_t                    res;

    switch(dhcp->state)
    {
        /*---- INIT ----------------------------------------------------*/
        case FNET_DHCP_CLN_STATE_INIT:
            /* Todo: The client SHOULD wait a random time between one and ten seconds to
            * desynchronize the use of DHCP at startup. */
            _fnet_dhcp_cln_set_state(dhcp, FNET_DHCP_CLN_STATE_SELECTING); /* => SELECTING, send discover */
            if(dhcp->callback_discover)
            {
                dhcp->callback_discover((fnet_dhcp_cln_desc_t)dhcp, dhcp->netif, dhcp->callback_discover_cookie);
            }
            break;
        /*---- SELECTING --------------------------------------------*/
        case FNET_DHCP_CLN_STATE_SELECTING:
            /* Waiting for OFFER */
            res = _fnet_dhcp_cln_receive_message(dhcp, &options);

            if(res == FNET_DHCP_CLN_IS_TIMEOUT)
            {
                _fnet_dhcp_cln_set_state(dhcp, FNET_DHCP_CLN_STATE_INIT); /* => INIT */
            }

#if FNET_CFG_DHCP_CLN_BOOTP
            else if(res > 0)
            {
                /* Todo: The client SHOULD perform a check on the suggested address
                * to ensure that the address is not already in use.*/
                _fnet_dhcp_cln_print_options(&options);
                /* Apply parameters. */
                dhcp->current_options = options;

                _fnet_dhcp_cln_apply_params(dhcp);
            }
#else /* DHCP */
            else if((res > 0) && (options.private_options.message_type == FNET_DHCP_OPTION_MSG_TYPE_OFFER))
            {
                dhcp->offered_options = options;                          /* Save offered options */
                _fnet_dhcp_cln_set_state(dhcp, FNET_DHCP_CLN_STATE_REQUESTING); /* => REQUESTING */
            }
            else
            {}
#endif
            break;
        /*---- BOUND ------------------------------------------------*/
        case FNET_DHCP_CLN_STATE_BOUND:
#if FNET_CFG_DHCP_CLN_BOOTP
            fnet_dhcp_cln_release(dhcp);
#else /* DHCP */
            if(fnet_netif_get_ip4_addr_type(dhcp->netif) == FNET_NETIF_IP_ADDR_TYPE_MANUAL) /* If user did not change parameters manually.*/
            {
                fnet_dhcp_cln_release((fnet_dhcp_cln_desc_t)dhcp); /* Address parameters changed outside => Disable DHCP*/
            }
            else
            {
                struct fnet_sockaddr  addr_from;
                fnet_size_t           addr_len = sizeof(addr_from);

                /* Discard all input data. */
                fnet_socket_recvfrom(dhcp->socket_client, &dhcp->message.header, sizeof(fnet_dhcp_header_t),
                                     0U,                   (struct fnet_sockaddr *) &addr_from, &addr_len);

                /* If T1 expired. */
                if((fnet_timer_get_ms() - dhcp->lease_obtained_time_ms) > dhcp->state_timeout_ms)
                {
                    _fnet_dhcp_cln_set_state(dhcp, dhcp->state_timeout_next_state); /* => INIT */
                }
            }
#endif
            break;

#if !FNET_CFG_DHCP_CLN_BOOTP  /* DHCP */
        case FNET_DHCP_CLN_STATE_INIT_REBOOT: /*---- INIT_REBOOT -----------------------------*/
            _fnet_dhcp_cln_set_state(dhcp, FNET_DHCP_CLN_STATE_REBOOTING); /* => REBOOTING */
            if(dhcp->callback_discover)
            {
                dhcp->callback_discover((fnet_dhcp_cln_desc_t)dhcp, dhcp->netif, dhcp->callback_discover_cookie);
            }
            break;
        case FNET_DHCP_CLN_STATE_RENEWING:      /*---- RENEWING ------------------------------*/
        case FNET_DHCP_CLN_STATE_REBINDING:     /*---- REBINDING -----------------------------*/
        case FNET_DHCP_CLN_STATE_REBOOTING:     /*---- REBOOTING -----------------------------*/
        case FNET_DHCP_CLN_STATE_REQUESTING:    /*---- REQUESTING ----------------------------*/
            if((dhcp->state == FNET_DHCP_CLN_STATE_RENEWING) || (dhcp->state == FNET_DHCP_CLN_STATE_REBINDING))
            {
                if(fnet_netif_get_ip4_addr_type(dhcp->netif) == FNET_NETIF_IP_ADDR_TYPE_MANUAL) /* If user changed parameters manually. */
                {
                    fnet_dhcp_cln_release((fnet_dhcp_cln_desc_t)dhcp);      /* Disable DHCP if user has changed ip parameters. */
                    break;
                }
            }

            if((fnet_timer_get_ms() - dhcp->lease_obtained_time_ms) < dhcp->state_timeout_ms)
            {
                /* Waiting for ACK */
                res = _fnet_dhcp_cln_receive_message(dhcp, &options);

                if(res == FNET_DHCP_CLN_IS_TIMEOUT)
                {
                    _fnet_dhcp_cln_send_message(dhcp); /* Resend REQUEST.*/
                }
                else if(res > 0)
                {
                    if((options.private_options.message_type == FNET_DHCP_OPTION_MSG_TYPE_ACK) /* ACK */
                       /* Check options that must be present*/
                       && (options.public_options.ip_address.s_addr)
                       && (options.public_options.dhcp_server.s_addr)
                       && (options.public_options.lease_time))
                    {
                        /* Todo: The client SHOULD perform a check on the suggested address
                        * to ensure that the address is not already in use.*/
                        _fnet_dhcp_cln_print_options(&options);

                        dhcp->lease_obtained_time_ms = dhcp->state_send_timestamp_ms; /* save lease obtained time.*/

                        /* Check T1, T2 and lease time */
                        if(options.public_options.lease_time == FNET_HTONL(FNET_DHCP_CLN_LEASE_INFINITY))
                        {
                            options.public_options.t1 = FNET_HTONL(FNET_DHCP_CLN_LEASE_INFINITY);
                            options.public_options.t2 = FNET_HTONL(FNET_DHCP_CLN_LEASE_INFINITY);
                        }
                        else
                        {
                            fnet_uint32_t orig_lease_time = options.public_options.lease_time;

                            if(fnet_ntohl(options.public_options.lease_time) < FNET_DHCP_CLN_LEASE_MIN)
                            {
                                options.public_options.lease_time = FNET_HTONL((fnet_uint32_t)FNET_DHCP_CLN_LEASE_MIN);
                            }
                            else if(fnet_ntohl(options.public_options.lease_time) > FNET_DHCP_CLN_LEASE_MAX)
                            {
                                options.public_options.lease_time = FNET_HTONL(FNET_DHCP_CLN_LEASE_MAX);
                            }
                            else
                            {}

                            if((options.public_options.t1 == 0U) || (options.public_options.t2 == 0U) || (orig_lease_time != options.public_options.lease_time))
                            {
                                options.public_options.t1 = fnet_htonl(fnet_ntohl(options.public_options.lease_time) >> 1); /* t1=(lease * 0.5) */
                                options.public_options.t2 = fnet_htonl(fnet_ntohl(options.public_options.lease_time) - fnet_ntohl(options.public_options.lease_time) / 8U); /* t2=(lease * 0.875) */
                            }
                        }

                        dhcp->current_options = options;

                        if(dhcp->in_params.probe_addr)
                        {
                            /* The client SHOULD probe the newly received address, e.g., with ARP.*/
                            _fnet_dhcp_cln_set_state(dhcp, FNET_DHCP_CLN_STATE_PROBING);    /* => PROBING */
                        }
                        else
                        {
                            /* Apply parameters. */
                            _fnet_dhcp_cln_apply_params(dhcp);
                        }
                    }
                    else if(options.private_options.message_type == FNET_DHCP_OPTION_MSG_TYPE_NAK) /* NAK */
                    {
                        _fnet_dhcp_cln_set_state(dhcp, FNET_DHCP_CLN_STATE_INIT);    /* => INIT */
                    }
                    else
                    {}
                }
                else
                {}
            }
            else
            {
                _fnet_dhcp_cln_set_state(dhcp, dhcp->state_timeout_next_state);
            }
            break;
        /*---- REQUESTING -----------------------------------------------*/
        case FNET_DHCP_CLN_STATE_PROBING:
            if((fnet_timer_get_ms() - dhcp->state_send_timestamp_ms) < dhcp->state_timeout_ms)
            {
#if 0 /* Emulate address conflict detection. */
                static fnet_uint32_t conflict = 0u;
                if(conflict++ == 0u)
#else
                /* Check ARP cache.*/
                if(fnet_arp_get_mac( dhcp->netif, dhcp->current_options.public_options.ip_address.s_addr, FNET_NULL) == FNET_TRUE)
#endif
                {
                    /* Address Conflict is detected.*/
                    _fnet_dhcp_cln_send_message(dhcp); /* Send DECLINE.*/

                    _fnet_dhcp_cln_set_state(dhcp, FNET_DHCP_CLN_STATE_INIT);    /* => INIT */
                }
            }
            else
            {
                /* No conflict detected.*/
                /* Apply parameters. */
                _fnet_dhcp_cln_apply_params(dhcp);
            }
            break;
#endif /* !FNET_CFG_DHCP_CLN_BOOTP */
        case FNET_DHCP_CLN_STATE_DISABLED:
        default:
            break;

    }
}

/************************************************************************
*  DHCP client initialization.
************************************************************************/
fnet_dhcp_cln_desc_t fnet_dhcp_cln_init(fnet_dhcp_cln_params_t *params )
{
    struct fnet_sockaddr    addr_client;
    fnet_dhcp_cln_state_t   state = FNET_DHCP_CLN_STATE_INIT;
    fnet_dhcp_cln_if_t      *dhcp_if = 0;
    fnet_index_t            i;
    fnet_scope_id_t         scope_id;
    fnet_netif_desc_t       netif;

    fnet_service_mutex_lock();

    if((params == 0) || (params->netif == 0))
    {
        FNET_DEBUG_DHCP_CLN(FNET_DHCP_CLN_ERR_PARAMS);
        goto ERROR;
    }

    netif = params->netif;

    scope_id = fnet_netif_get_scope_id(netif);
    if(scope_id == 0)
    {
        FNET_DEBUG_DHCP_CLN(FNET_DHCP_CLN_ERR_PARAMS);
        goto ERROR;
    }

    /* Try to find free DHCP server. */
    for(i = 0u; i < FNET_CFG_DHCP_CLN; i++)
    {
        if(fnet_dhcp_cln_if_list[i].is_enabled == FNET_FALSE)
        {
            dhcp_if = &fnet_dhcp_cln_if_list[i];
        }
        else
        {
            if(fnet_dhcp_cln_if_list[i].netif == netif)
            {
                /* It is not possible to run several DHCP servers on the same networking interface.*/
                dhcp_if = 0;
                break;
            }
        }
    }

    /* Is DHCP server already initialized. */
    if(dhcp_if == 0)
    {
        FNET_DEBUG_DHCP_CLN(FNET_DHCP_CLN_ERR_IS_INITIALIZED);
        goto ERROR;
    }

    /* Create client socket */
    fnet_memset_zero(&addr_client, sizeof(addr_client));
    ((struct fnet_sockaddr_in *)(&addr_client))->sin_family = AF_INET;
    ((struct fnet_sockaddr_in *)(&addr_client))->sin_port = FNET_CFG_DHCP_CLN_PORT;
    ((struct fnet_sockaddr_in *)(&addr_client))->sin_addr.s_addr = FNET_HTONL(INADDR_ANY);
    ((struct fnet_sockaddr_in *)(&addr_client))->sin_scope_id = scope_id;

    if((dhcp_if->socket_client = fnet_socket(AF_INET, SOCK_DGRAM, 0u)) == FNET_NULL)
    {
        FNET_DEBUG_DHCP_CLN(FNET_DHCP_CLN_ERR_SOCKET_CREATION);
        goto ERROR;
    }

    if(fnet_socket_bind(dhcp_if->socket_client, (struct fnet_sockaddr *) &addr_client, sizeof(addr_client)) != 0)
    {
        FNET_DEBUG_DHCP_CLN(FNET_DHCP_CLN_ERR_SOCKET_BIND);
        goto ERROR_1;
    }

    if(fnet_netif_get_hw_addr(netif, dhcp_if->macaddr, sizeof(fnet_mac_addr_t)) != FNET_OK)
    {
        goto ERROR_1;
    }

    dhcp_if->service_descriptor = fnet_service_register(_fnet_dhcp_cln_poll, (void *) dhcp_if);

    if(dhcp_if->service_descriptor == 0)
    {
        FNET_DEBUG_DHCP_CLN(FNET_DHCP_CLN_ERR_SERVICE);
        goto ERROR_1;
    }

    dhcp_if->netif = netif;
    dhcp_if->response_timeout_ms = FNET_CFG_DHCP_CLN_RESPONSE_TIMEOUT_MS;

    if(params)
    {
        dhcp_if->in_params.requested_ip_address = params->requested_ip_address;

#if !FNET_CFG_DHCP_CLN_BOOTP  /* DHCP */
        /* Convert "requested_lease_time" to network byte order.*/
        dhcp_if->in_params.requested_lease_time = fnet_htonl(params->requested_lease_time);

        if(dhcp_if->in_params.requested_ip_address.s_addr)
        {
            /* Initialization with known network address.
            * The client begins in INIT-REBOOT state and sends a DHCPREQUEST
            * message.  The client MUST insert its known network address as a
            * 'requested IP address' option in the DHCPREQUEST message.*/
            state = FNET_DHCP_CLN_STATE_INIT_REBOOT;
        }

        dhcp_if->in_params.probe_addr = params->probe_addr;
#endif
    }

    _fnet_dhcp_cln_set_state(dhcp_if, state);

    dhcp_if->is_enabled = FNET_TRUE;

    fnet_service_mutex_unlock();

    return (fnet_dhcp_cln_desc_t)dhcp_if;

ERROR_1:
    fnet_socket_close(dhcp_if->socket_client);

ERROR:
    fnet_service_mutex_unlock();
    return 0;
}

/************************************************************************
*  DHCP client release.
************************************************************************/
void fnet_dhcp_cln_release(fnet_dhcp_cln_desc_t desc)
{
    struct fnet_dhcp_cln_if   *dhcp_if = (struct fnet_dhcp_cln_if *) desc;

    if(dhcp_if && (dhcp_if->is_enabled == FNET_TRUE))
    {
        fnet_service_mutex_lock();
        _fnet_dhcp_cln_set_state(dhcp_if, FNET_DHCP_CLN_STATE_DISABLED); /* => DISABLED */
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
*  Return FNET_TRUE if the DHCP client is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_dhcp_cln_is_enabled(fnet_dhcp_cln_desc_t desc)
{
    struct fnet_dhcp_cln_if     *dhcp_if = (struct fnet_dhcp_cln_if *) desc;
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
* Look for a DHCP Client assigned to the specified network interface.
************************************************************************/
fnet_dhcp_cln_desc_t fnet_dhcp_cln_get_by_netif(fnet_netif_desc_t netif)
{
    fnet_dhcp_cln_if_t      *dhcp_if;
    fnet_index_t            i;
    fnet_dhcp_cln_desc_t    dhcp_desc = 0;

    if(netif)
    {
        fnet_service_mutex_lock();

        for(i = 0u; i < FNET_CFG_DHCP_CLN; i++)
        {
            dhcp_if = &fnet_dhcp_cln_if_list[i];

            if((dhcp_if->is_enabled == FNET_TRUE) && (dhcp_if->netif == netif))
            {
                dhcp_desc = (fnet_dhcp_cln_desc_t)dhcp_if;
                break;
            }
        }

        fnet_service_mutex_unlock();
    }

    return dhcp_desc;
}

/************************************************************************
* Copy current DHCP options to structure pointed by 'options'.
************************************************************************/
void fnet_dhcp_cln_get_options(fnet_dhcp_cln_desc_t desc, struct fnet_dhcp_cln_options *options )
{
    struct fnet_dhcp_cln_if   *dhcp_if = (struct fnet_dhcp_cln_if *) desc;

    if(options && dhcp_if)
    {
        fnet_service_mutex_lock();

        *options = dhcp_if->current_options.public_options;

        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* Register the "IP parameters updated" DHCP event handler callback.
************************************************************************/
void fnet_dhcp_cln_set_callback_updated(fnet_dhcp_cln_desc_t desc, fnet_dhcp_cln_callback_t callback_updated, void *cookie)
{
    struct fnet_dhcp_cln_if   *dhcp_if = (struct fnet_dhcp_cln_if *) desc;

    if(dhcp_if)
    {
        fnet_service_mutex_lock();
        dhcp_if->callback_updated = callback_updated;
        dhcp_if->callback_updated_cookie = cookie;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
*  Register the "Discover message sent" DHCP event handler callback.
************************************************************************/
void fnet_dhcp_cln_set_callback_discover(fnet_dhcp_cln_desc_t desc, fnet_dhcp_cln_callback_t callback_discover, void *cookie)
{
    struct fnet_dhcp_cln_if   *dhcp_if = (struct fnet_dhcp_cln_if *) desc;

    if(dhcp_if)
    {
        fnet_service_mutex_lock();
        dhcp_if->callback_discover = callback_discover;
        dhcp_if->callback_discover_cookie = cookie;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* Change timeout for a DHCP server response.
************************************************************************/
void fnet_dhcp_cln_set_response_timeout(fnet_dhcp_cln_desc_t desc, fnet_time_t response_timout_ms)
{
    struct fnet_dhcp_cln_if   *dhcp_if = (struct fnet_dhcp_cln_if *) desc;

    if(dhcp_if)
    {
        if(response_timout_ms)
        {
            dhcp_if->response_timeout_ms = response_timout_ms;
        }
        else /* Reset to the default value */
        {
            dhcp_if->response_timeout_ms = FNET_CFG_DHCP_CLN_RESPONSE_TIMEOUT_MS;
        }
    }
}

#endif /* FNET_CFG_DHCP_CLN && FNET_CFG_IP4 */

/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
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

/*
* Boot file name (or two-step bootstrap procedure) is not suported.
*/

#include "fnet.h"

#if FNET_CFG_DHCP && FNET_CFG_IP4


#if FNET_CFG_DEBUG_DHCP && FNET_CFG_DEBUG
    #define FNET_DEBUG_DHCP   FNET_DEBUG
#else
    #define FNET_DEBUG_DHCP(...)    do{}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/


#define FNET_DHCP_OP_BOOTREQUEST        (1U)
#define FNET_DHCP_FLAGS_BROADCAST       (0x8000U) /* RFC2131:  A server or relay agent sending or relaying a DHCP message directly
                                                   to a DHCP client (i.e., not to a relay agent specified in the
                                                   'giaddr' field) SHOULD examine the BROADCAST bit in the 'flags'
                                                   field.  If this bit is set to 1, the DHCP message SHOULD be sent as
                                                   an IP broadcast using an IP broadcast address (preferably 0xffffffff)
                                                   as the IP destination address and the link-layer broadcast address as
                                                   the link-layer destination address.  If the BROADCAST bit is cleared
                                                   to 0, the message SHOULD be sent as an IP unicast to the IP address
                                                   specified in the 'yiaddr' field and the link-layer address specified
                                                   in the 'chaddr' field.  If unicasting is not possible, the message
                                                   MAY be sent as an IP broadcast using an IP broadcast address
                                                   (preferably 0xffffffff) as the IP destination address and the link-
                                                   layer broadcast address as the link-layer destination address.*/


#if !FNET_CFG_DHCP_BOOTP /* Not used by BOOTP */
    #define FNET_DHCP_OP_BOOTREPLY          (2U)

    #define FNET_DHCP_LEASE_MAX             (0xFFFFFFFFU/(1000U*2U)) /* Maximum lease time value (cause of timer counter is in ms) */

    #define FNET_DHCP_STATE_REBOOTING_SEND_TIMEOUT      (4U*1000U)          /*(ms) timeout for ACK => request retransmission.*/
    #define FNET_DHCP_STATE_REBOOTING_TIMEOUT           (2U*FNET_DHCP_STATE_REBOOTING_SEND_TIMEOUT)  /*(ms) timeout to go to INIT state.*/
    #define FNET_DHCP_STATE_RENEWING_SEND_TIMEOUT       (60U*1000U)                                   /*(ms) timeout for ACK => request retransmission.*/
    #define FNET_DHCP_STATE_REBINDING_SEND_TIMEOUT      (60U*1000U)                                   /*(ms) timeout for ACK => request retransmission.*/
#endif /* !FNET_CFG_DHCP_BOOTP */

#define FNET_DHCP_STATE_REQUESTING_SEND_TIMEOUT     (4U*1000U)          /*(ms) timeout for ACK => request retransmission.*/
#define FNET_DHCP_STATE_REQUESTING_TIMEOUT          (4U*FNET_DHCP_STATE_REQUESTING_SEND_TIMEOUT) /*(ms) timeout to go to INIT state.*/
#define FNET_DHCP_STATE_SELECTING_SEND_TIMEOUT      (FNET_CFG_DHCP_RESPONSE_TIMEOUT*1000U)     /*(ms) timeout for OFFER => INIT.*/
#define FNET_DHCP_STATE_PROBING_TIMEOUT             (1U*1000U)          /*(ms) timeout to go to INIT state.*/

#define FNET_DHCP_ERR_SOCKET_CREATION   "ERROR: Socket creation error."
#define FNET_DHCP_ERR_SOCKET_BIND       "ERROR: Socket Error during bind."
#define FNET_DHCP_ERR_IS_INITIALIZED    "ERROR: No free server."
#define FNET_DHCP_ERR_NONETWORK         "ERROR: Network Interface is not configurated."
#define FNET_DHCP_ERR_SERVICE           "ERROR: Service registration is failed."

#define FNET_DHCP_ISTIMEOUT (-1)


/************************************************************************
*     DHCP Options. [RFC 2132] definitions
*************************************************************************/
#define FNET_DHCP_OPTION_PAD                    (0U)     /* Pad option.*/
#define FNET_DHCP_OPTION_SUBNETMASK             (1U)     /* Subnet mask.*/
#define FNET_DHCP_OPTION_SUBNETMASK_LENGTH      (4U)

#define FNET_DHCP_OPTION_ROUTER                 (3U)     /* Router option (gateway).*/
#define FNET_DHCP_OPTION_ROUTER_MULTIPLE        (4U)
#define FNET_DHCP_OPTION_DNS                    (6U)     /* Domain Name Server option. */
#define FNET_DHCP_OPTION_DNS_LENGTH_MIN         (4U)     /* The minimum length for this option is 4 octets, 
                                                         * and the length MUST always be a multiple of 4. */
#define FNET_DHCP_OPTION_HOSTNAME               (12U)    /* Hostname */
#define FNET_DHCP_OPTION_HOSTNAME_LENGTH        (32U)
#define FNET_DHCP_OPTION_FILELENGTH             (13U)    /* File Length */
#define FNET_DHCP_OPTION_FILELENGTH_LENGTH      (2U)
#define FNET_DHCP_OPTION_ROOTPATH               (17U)    /* Rootpath.*/
#define FNET_DHCP_OPTION_ROOTPATH_LENGTH        (32U)
#define FNET_DHCP_OPTION_BROADCAST              (28U)    /* Broadcast Address option.  */
#define FNET_DHCP_OPTION_BROADCAST_LENGTH       (4U)
#define FNET_DHCP_OPTION_REQ_ADDRESS            (50U)    /* Requested IP address.*/
#define FNET_DHCP_OPTION_REQ_ADDRESS_LENGTH     (4U)
#define FNET_DHCP_OPTION_LEASE                  (51U)    /* IP Address lease time (seconds).*/
#define FNET_DHCP_OPTION_LEASE_LENGTH           (4U)
#define FNET_DHCP_OPTION_OVERLOAD               (52U)    /* Option overload.*/
#define FNET_DHCP_OPTION_OVERLOAD_LENGTH        (1U)
#define FNET_DHCP_OPTION_OVERLOAD_NONE          (0U)
#define FNET_DHCP_OPTION_OVERLOAD_FILE          (1U)     /* The 'file' field is used to hold options.*/
#define FNET_DHCP_OPTION_OVERLOAD_SNAME         (2U)     /* The 'sname' field is used to hold options.*/
#define FNET_DHCP_OPTION_OVERLOAD_BOTH          (3U)     /* Both fields are used to hold options.*/
#define FNET_DHCP_OPTION_TYPE                   (53U)    /* DHCP Message Type.*/
#define FNET_DHCP_OPTION_TYPE_LENGTH            (1U)
#define FNET_DHCP_OPTION_TYPE_DISCOVER          (1U)
#define FNET_DHCP_OPTION_TYPE_OFFER             (2U)
#define FNET_DHCP_OPTION_TYPE_REQUEST           (3U)
#define FNET_DHCP_OPTION_TYPE_DECLINE           (4U)
#define FNET_DHCP_OPTION_TYPE_ACK               (5U)
#define FNET_DHCP_OPTION_TYPE_NAK               (6U)
#define FNET_DHCP_OPTION_TYPE_RELEASE           (7U)
#define FNET_DHCP_OPTION_TYPE_INFORM            (8U)
#define FNET_DHCP_OPTION_SERVER_ID              (54U)    /* Server Identifier (ip address).*/
#define FNET_DHCP_OPTION_SERVER_ID_LENGTH       (4U)
#define FNET_DHCP_OPTION_PARAMETER_REQ_LIST     (55U)    /* Parameter Request List. */
#define FNET_DHCP_OPTION_MESSAGE_SIZE           (57U)    /* Maximum DHCP Message Size. The minimum legal value is 576.*/
#define FNET_DHCP_OPTION_MESSAGE_SIZE_LENGTH    (2U)
#define FNET_DHCP_OPTION_T1                     (58U)    /* Renewal (T1) Time Value.*/
#define FNET_DHCP_OPTION_T1_LENGTH              (4U)
#define FNET_DHCP_OPTION_T2                     (59U)    /* Rebinding (T2) Time Value. */
#define FNET_DHCP_OPTION_T2_LENGTH              (4U)
#define FNET_DHCP_OPTION_CLIENT_ID              (61U)    /* Client-identifier.*/
#define FNET_DHCP_OPTION_CLIENT_ID_LENGTH       (sizeof(fnet_mac_addr_t)+1U)
#define FNET_DHCP_OPTION_END                    (255U)   /* End option. */

/**************************************************************************/ /*!
 * @brief DHCP client states.@n
 * Used mainly for debugging purposes.
 ******************************************************************************/
typedef enum
{
    FNET_DHCP_STATE_DISABLED = 0,   /**< @brief The DHCP client service is not
                                     * initialized.*/
    FNET_DHCP_STATE_INIT,           /**< @brief The DHCP client service is initialized.
                                     * Sends DHCPDISCOVER message.@n
                                     * Signals the Discover callback event.*/
    FNET_DHCP_STATE_SELECTING,      /**< @brief Waits for the DHCPOFFER message.*/
    FNET_DHCP_STATE_REQUESTING,     /**< @brief Sends the DHCPREQUEST message.
                                     * Waits for the DHCPACK.*/
    FNET_DHCP_STATE_PROBING,        /**< @brief Probes and checks whether the address
                                     * is in use by some other device.*/
    FNET_DHCP_STATE_BOUND,          /**< @brief The DHCPACK message from
                                     * the DHCP server arrived.
                                     * The client parameters are set.@n
                                     * Signals the Updated callback event.*/
    FNET_DHCP_STATE_RENEWING,       /**< @brief T1 expired. Send the DHCPREQUEST
                                     * to a leasing server.*/
    FNET_DHCP_STATE_REBINDING,      /**< @brief T2 expired. Broadcast the DHCPREQUEST.*/
    FNET_DHCP_STATE_INIT_REBOOT,    /**< @brief The DHCP client service is initialized.
                                     * Sends the DHCPREQUEST message.@n
                                     * Signals the Discover callback event.*/
    FNET_DHCP_STATE_REBOOTING,      /**< @brief Sends the DHCPREQUEST message.
                                     * Waits for the DHCPACK.*/
    FNET_DHCP_STATE_RELEASE         /**< @brief Sends the RELEASE message.
                                     * Frees the allocated resources.*/
} fnet_dhcp_state_t;


static const fnet_uint8_t fnet_dhcp_magic_cookie [] =
{
    99, 130, 83, 99
}; /* The first four octets of the vendor information
*   field have been assigned to the "magic cookie".*/

#define FNET_DHCP_OPTIONS_LENGTH (312U) /* [RFC2131, 2] A DHCP client must be prepared to receive DHCP messages 
                                        *   with an 'options' field of at least length 312 octets.*/

/**************************************************************************/ /*!
 * Private DHCP options.
 ******************************************************************************/
struct fnet_dhcp_options_prv
{
    fnet_uint8_t message_type;      /* The DHCP Message Type.
                                     * This option is used to convey the type of the
                                     * last DHCP message.
                                     */

#if FNET_CFG_DHCP_OVERLOAD && !FNET_CFG_DHCP_BOOTP

    fnet_uint8_t  overload;         /* Overload Option.
                                     * If this option is present, the DHCP client interprets
                                     * the specified additional fields after it concludes
                                     * interpretation of the standard option fields.
                                     */
#endif

};

/**************************************************************************/ /*!
 * All DHCP options are retrieved from a DHCP server.
 ******************************************************************************/
struct fnet_dhcp_options_in
{
    struct fnet_dhcp_options public_options;
    struct fnet_dhcp_options_prv private_options;
};

/************************************************************************
*    DHCP header [RFC2131, 2]
*************************************************************************
   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     op (1)    |   htype (1)   |   hlen (1)    |   hops (1)    |
   +---------------+---------------+---------------+---------------+
   |                            xid (4) - Transaction ID                           |
   +-------------------------------+-------------------------------+
   |           secs (2)            |           flags (2)           |
   +-------------------------------+-------------------------------+
   |                          ciaddr  (4) - client IP addr         |
   +---------------------------------------------------------------+
   |                          yiaddr  (4) - your (client) IP addr  |
   +---------------------------------------------------------------+
   |                          siaddr  (4) - server IP addr         |
   +---------------------------------------------------------------+
   |                          giaddr  (4) - relay aget IP addr     |
   +---------------------------------------------------------------+
   |                                                               |
   |                          chaddr  (16) - client HW adress.     |
   |                                                               |
   |                                                               |
   +---------------------------------------------------------------+
   |                                                               |
   |                          sname   (64) - server host name      |
   +---------------------------------------------------------------+
   |                                                               |
   |                          file    (128) - boot file name       |
   +---------------------------------------------------------------+
   |                          DHCP magic cookie    (4)             |
   +---------------------------------------------------------------+
   |                                                               |
   |                          options (variable)                   |
   +---------------------------------------------------------------+
   | opt end = 0xFF|
   +---------------+*/

FNET_COMP_PACKED_BEGIN
typedef struct
{
    fnet_uint8_t    op FNET_COMP_PACKED;        /* Message op code / message type:
                                                 *   1 = BOOTREQUEST, 2 = BOOTREPLY */
    fnet_uint8_t    htype FNET_COMP_PACKED;     /* Hardware address type, see ARP section in "Assigned
                                                 *   Numbers" RFC; e.g., '1' = 10mb ethernet.*/
    fnet_uint8_t    hlen FNET_COMP_PACKED;      /* Hardware address length (e.g.  '6' for 10mb ethernet).*/
    fnet_uint8_t    hops FNET_COMP_PACKED;      /* Client sets to zero, optionally used by relay agents
                                                 *   when booting via a relay agent.*/
    fnet_uint32_t   xid FNET_COMP_PACKED;       /* Transaction ID, a random number chosen by the
                                                 *   client, used by the client and server to associate
                                                 *   messages and responses between a client and a server.*/
    fnet_uint16_t   secs FNET_COMP_PACKED;      /* Filled in by client, seconds elapsed since client
                                                 *   began address acquisition or renewal process.*/
    fnet_uint16_t   flags FNET_COMP_PACKED;     /* Flags.*/
    fnet_ip4_addr_t ciaddr FNET_COMP_PACKED;    /* Client IP address; only filled in if client is in BOUND,
                                                 *   RENEW or REBINDING state and can respond to ARP requests.*/
    fnet_ip4_addr_t yiaddr FNET_COMP_PACKED;    /* Your (client) IP address.*/
    fnet_ip4_addr_t siaddr FNET_COMP_PACKED;    /* IP address of next server to use in bootstrap;
                                                 *   returned in DHCPOFFER, DHCPACK by server.*/
    fnet_ip4_addr_t giaddr FNET_COMP_PACKED;    /* Relay agent IP address, used in booting via a relay agent.*/
    fnet_uint8_t    chaddr[16] FNET_COMP_PACKED;/* Client hardware address.*/
    fnet_uint8_t    sname[64] FNET_COMP_PACKED; /* Optional server host name, null terminated string.*/
    fnet_uint8_t    file[128] FNET_COMP_PACKED; /* Boot file name, null terminated string; "generic"
                                                 *   name or null in DHCPDISCOVER, fully qualified
                                                 *   directory-path name in DHCPOFFER.*/
    fnet_uint8_t    magic_cookie[4] FNET_COMP_PACKED;
    fnet_uint8_t    options[FNET_DHCP_OPTIONS_LENGTH] FNET_COMP_PACKED; /* Optional parameters field. See [RFC2132].*/
} fnet_dhcp_header_t;
FNET_COMP_PACKED_END

/************************************************************************
*    DHCP message control structure.
*************************************************************************/
typedef struct
{
    fnet_uint8_t       *next_option_position;
    fnet_uint8_t       *end_position;
    fnet_dhcp_header_t header;
} fnet_dhcp_message_t;

/************************************************************************
*    DHCP interface interface structure
*************************************************************************/
typedef struct fnet_dhcp_if
{
    fnet_socket_t               socket_client;
    fnet_dhcp_state_t           state;                      /* Current state.*/
    fnet_bool_t                 enabled;
#if !FNET_CFG_DHCP_BOOTP
    fnet_time_t                 state_timeout;              /* Current State timeout (ticks).*/
    fnet_dhcp_state_t           state_timeout_next_state;   /* Next state on state timeout.*/
    fnet_time_t                 lease_obtained_time;
#endif
    fnet_time_t                 state_send_timeout;         /* Current State send request timeout (ticks).*/
    fnet_time_t                 send_request_time;          /* Time at which the client sent the REQUEST message */
    fnet_mac_addr_t             macaddr;
    fnet_uint32_t               xid;
    fnet_dhcp_message_t         message;
    struct fnet_dhcp_params     in_params;                  /* Input user parameters.*/
    struct fnet_dhcp_options_in current_options;            /* Parsed options */
    struct fnet_dhcp_options_in offered_options;            /* Parsed options */
    fnet_netif_desc_t           netif;
    fnet_poll_desc_t            service_descriptor;
    fnet_dhcp_callback_t        callback_updated;           /* Optional ponter to the handler
                                                            * callback function, that is
                                                            * called when the DHCP client has
                                                            * updated the IP parameters.*/
    fnet_dhcp_callback_t        callback_discover;          /* Optional pointer to the handler
                                                            * callback function, that is
                                                            * called when the DHCP client send
                                                            * the DHCP discover message.*/
    void                        *callback_discover_param;   /* Optional user-application specific parameter.
                                                            * It's passed to the callback_discover event handler callback..*/
    void                        *callback_updated_param;    /* Optional user-application specific parameter.
                                                            * It's passed to the callback_updated
                                                            * event handler callback.*/
} fnet_dhcp_if_t;


/* DHCP client interface */
static fnet_dhcp_if_t fnet_dhcp_if_list[FNET_CFG_DHCP_MAX];

/* List of dhcp parameter/options we request.*/
static fnet_uint8_t fnet_dhcp_parameter_request_list [] =
{
    FNET_DHCP_OPTION_SUBNETMASK,
    FNET_DHCP_OPTION_ROUTER,
#if FNET_CFG_DNS
    FNET_DHCP_OPTION_DNS,
#endif
#if FNET_CFG_DHCP_BROADCAST
    FNET_DHCP_OPTION_BROADCAST,
#endif
    FNET_DHCP_OPTION_LEASE,
    FNET_DHCP_OPTION_T1,
    FNET_DHCP_OPTION_T2
};

static void fnet_dhcp_add_option( fnet_dhcp_message_t *message, fnet_uint8_t option_code, fnet_uint8_t option_length,  const void *option_value );
static fnet_uint8_t *fnet_dhcp_next_option( fnet_dhcp_message_t *message );
static void fnet_dhcp_parse_options( fnet_dhcp_message_t *message, struct fnet_dhcp_options_in *options );
static fnet_int32_t fnet_dhcp_send_message( fnet_dhcp_if_t *dhcp );
static fnet_int32_t fnet_dhcp_receive_message( fnet_dhcp_if_t *dhcp, struct fnet_dhcp_options_in *options );
static void fnet_dhcp_apply_params(fnet_dhcp_if_t *dhcp);
static void fnet_dhcp_change_state( fnet_dhcp_if_t *dhcp, fnet_dhcp_state_t state );
static void fnet_dhcp_state_machine( void *fnet_dhcp_if_p );

#if FNET_CFG_DEBUG_DHCP && FNET_CFG_DEBUG/* Debug functions */
/************************************************************************
* NAME: fnet_dhcp_print_header
*
* DESCRIPTION: Print DHCP header. For debug needs.
************************************************************************/
static void fnet_dhcp_print_header( fnet_dhcp_header_t *header )
{
    fnet_uint8_t ip_str[FNET_IP4_ADDR_STR_SIZE];
    fnet_index_t i;

    FNET_DEBUG_DHCP("DHCP: Message header:");
    FNET_DEBUG_DHCP(" op \t %02X", header->op);
    FNET_DEBUG_DHCP(" htype \t %02X", header->htype);
    FNET_DEBUG_DHCP(" hlen \t %02X", header->hlen);
    FNET_DEBUG_DHCP(" hops \t %02X", header->hops);
    FNET_DEBUG_DHCP(" xid \t %08X", fnet_ntohl(header->xid));
    FNET_DEBUG_DHCP(" secs \t %04X", fnet_ntohs(header->secs));
    FNET_DEBUG_DHCP(" secs \t %04X", fnet_ntohs(header->flags));
    fnet_inet_ntoa(*(struct in_addr *)(&header->ciaddr), ip_str);
    FNET_DEBUG_DHCP(" ciaddr \t %s", ip_str);
    fnet_inet_ntoa(*(struct in_addr *)(&header->yiaddr), ip_str);
    FNET_DEBUG_DHCP(" yiaddr \t %s", ip_str);
    fnet_inet_ntoa(*(struct in_addr *)(&header->siaddr), ip_str);
    FNET_DEBUG_DHCP(" siaddr \t %s", ip_str);
    fnet_inet_ntoa(*(struct in_addr *)(&header->giaddr), ip_str);
    FNET_DEBUG_DHCP(" giaddr \t %s", ip_str);
    FNET_DEBUG_DHCP(" chaddr \t ");

    for (i = 0u; i < 16u; i++)
    {
        FNET_DEBUG_DHCP("%02X", header->chaddr[i]);
    }

    FNET_DEBUG_DHCP("");
    FNET_DEBUG_DHCP(" sname \t %s", header->sname);
    FNET_DEBUG_DHCP(" file \t %s", header->file);
}

/************************************************************************
* NAME: fnet_dhcp_print_state
*
* DESCRIPTION: Print DHCP current state. For debug needs.
************************************************************************/
static void fnet_dhcp_print_state( fnet_dhcp_if_t *dhcp )
{
    FNET_DEBUG_DHCP("\nDHCP: State ");
    switch(dhcp->state)
    {
        case FNET_DHCP_STATE_DISABLED:
            FNET_DEBUG_DHCP("DISABLED");
            break;
        case FNET_DHCP_STATE_INIT:
            FNET_DEBUG_DHCP("INIT");
            break;
        case FNET_DHCP_STATE_SELECTING:
            FNET_DEBUG_DHCP("SELECTING");
            break;
        case FNET_DHCP_STATE_BOUND:
            FNET_DEBUG_DHCP("BOUND");
            break;

#if !FNET_CFG_DHCP_BOOTP

        case FNET_DHCP_STATE_REQUESTING:
            FNET_DEBUG_DHCP("REQUESTING");
            break;
        case FNET_DHCP_STATE_PROBING:
            FNET_DEBUG_DHCP("BROBING");
            break;
        case FNET_DHCP_STATE_RENEWING:
            FNET_DEBUG_DHCP("RENEWING");
            break;
        case FNET_DHCP_STATE_REBINDING:
            FNET_DEBUG_DHCP("REBINDING");
            break;
        case FNET_DHCP_STATE_INIT_REBOOT:
            FNET_DEBUG_DHCP("INIT_REBOOT");
            break;
        case FNET_DHCP_STATE_REBOOTING:
            FNET_DEBUG_DHCP("REBOOTING");
            break;
        case FNET_DHCP_STATE_RELEASE:
            FNET_DEBUG_DHCP("RELEASE");
            break;
#endif /* !FNET_CFG_DHCP_BOOTP */
    }
}

/************************************************************************
* NAME: fnet_dhcp_print_options
*
* DESCRIPTION: Print DHCP options. For debug needs.
************************************************************************/
static void fnet_dhcp_print_options( struct fnet_dhcp_options_in *options )
{
    fnet_uint8_t ip_str[FNET_IP4_ADDR_STR_SIZE];

    FNET_DEBUG_DHCP("DHCP: Options:");
    FNET_DEBUG_DHCP(" message_type \t\t %02X", options->private_options.message_type);

    fnet_inet_ntoa(*(struct in_addr *)(&options->public_options.ip_address), ip_str);
    FNET_DEBUG_DHCP(" ip_address \t\t %s", ip_str);
    fnet_inet_ntoa(*(struct in_addr *)(&options->public_options.dhcp_server), ip_str);
    FNET_DEBUG_DHCP(" dhcp_server \t %s", ip_str);
    fnet_inet_ntoa(*(struct in_addr *)(&options->public_options.netmask), ip_str);
    FNET_DEBUG_DHCP(" netmask \t\t %s", ip_str);
#if FNET_CFG_DHCP_BROADCAST
    fnet_inet_ntoa(*(struct in_addr *)(&options->public_options.broadcast), ip_str);
    FNET_DEBUG_DHCP(" broadcast \t\t %s", ip_str);
#endif
    fnet_inet_ntoa(*(struct in_addr *)(&options->public_options.gateway), ip_str);
    FNET_DEBUG_DHCP(" gateway \t\t %s", ip_str);

    FNET_DEBUG_DHCP(" t1 \t\t\t 0x%08X", fnet_ntohl(options->public_options.t1));
    FNET_DEBUG_DHCP(" t2 \t\t\t 0x%08X", fnet_ntohl(options->public_options.t2));
    FNET_DEBUG_DHCP(" lease_time \t\t 0x%08X", options->public_options.lease_time);

#if FNET_CFG_DHCP_OVERLOAD

    FNET_DEBUG_DHCP(" overload \t\t 0x%02X", options->public_options.overload);

#endif

}

#else

#define fnet_dhcp_print_header(x) do{}while(0)
#define fnet_dhcp_print_state(x) do{}while(0)
#define fnet_dhcp_print_options(x) do{}while(0)

#endif /* FNET_CFG_DEBUG_DHCP && FNET_CFG_DEBUG */


/************************************************************************
* NAME: fnet_dhcp_add_option
*
* DESCRIPTION: Add option to a DHCP options field.
************************************************************************/

static void fnet_dhcp_add_option( fnet_dhcp_message_t *message, fnet_uint8_t option_code, fnet_uint8_t option_length,  const void *option_value )
{
    if((&message->header.options[FNET_DHCP_OPTIONS_LENGTH] - message->next_option_position)
       < (option_length + 2U))
    {
        return;
    }

    *message->next_option_position++ = option_code;
    *message->next_option_position++ = option_length;
    fnet_memcpy(message->next_option_position, option_value, (fnet_size_t)option_length);
    message->next_option_position += option_length;
}

/************************************************************************
* NAME: fnet_dhcp_next_option
*
* DESCRIPTION: Go to the next DHCP option.
************************************************************************/
static fnet_uint8_t *fnet_dhcp_next_option( fnet_dhcp_message_t *message )
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
* NAME: fnet_dhcp_parse_options
*
* DESCRIPTION: Parse DHCP options from message.
************************************************************************/
static void fnet_dhcp_parse_options( fnet_dhcp_message_t *message, struct fnet_dhcp_options_in *options )
{
    fnet_uint8_t *current_option;

    while((current_option = fnet_dhcp_next_option(message)) != 0)
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
                if(option_length >= FNET_DHCP_OPTION_ROUTER_MULTIPLE)
                {
                    options->public_options.gateway.s_addr = *(fnet_ip4_addr_t *)option_data;
                }
                break;
#if FNET_CFG_DHCP_BROADCAST
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

#if !FNET_CFG_DHCP_BOOTP

            case FNET_DHCP_OPTION_TYPE:
                if(option_length == FNET_DHCP_OPTION_TYPE_LENGTH)
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
#if FNET_CFG_DHCP_OVERLOAD
            case FNET_DHCP_OPTION_OVERLOAD:
                if((option_length == FNET_DHCP_OPTION_OVERLOAD_LENGTH) && (*option_data <= FNET_DHCP_OPTION_OVERLOAD_BOTH))
                {
                    options->private_options.overload = *option_data;
                }
                break;
#endif
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
#endif /* !FNET_CFG_DHCP_BOOTP */
            default:
                break;
                /* Todo Other Options. */
        }
    }
}


/************************************************************************
* NAME: fnet_dhcp_send_message
*
* DESCRIPTION: Send DHCP message.
************************************************************************/
static fnet_int32_t fnet_dhcp_send_message( fnet_dhcp_if_t *dhcp )
{
    fnet_dhcp_message_t *message = &dhcp->message;
    struct sockaddr     addr_send;
    fnet_size_t         length;
    struct in_addr      ip_address;
#if !FNET_CFG_DHCP_BOOTP
    fnet_uint16_t       max_message_size;
    fnet_uint8_t        client_id[FNET_DHCP_OPTION_CLIENT_ID_LENGTH];
    fnet_uint8_t        message_type;
#endif

    fnet_memset_zero(&message->header, sizeof(message->header));
    message->header.op = FNET_DHCP_OP_BOOTREQUEST;
    message->header.htype = 1U; /* Ethernet */
    message->header.hlen = sizeof(dhcp->macaddr);
    message->header.xid = fnet_htonl(dhcp->xid); /* TBD PFI */
    message->header.flags = FNET_HTONS(FNET_DHCP_FLAGS_BROADCAST);

    fnet_memcpy(message->header.chaddr, dhcp->macaddr, sizeof(dhcp->macaddr)); /* Client HW address */

    /* Add "magic cookie" */
    fnet_memcpy(message->header.magic_cookie, fnet_dhcp_magic_cookie, sizeof(fnet_dhcp_magic_cookie));
    message->next_option_position = message->header.options;

    /* Add options */
    switch(dhcp->state)
    {
#if FNET_CFG_DHCP_BOOTP
        case FNET_DHCP_STATE_SELECTING: /*=>Requesting*/
            ip_address.s_addr = INADDR_BROADCAST;
            break;
#else /* DHCP */
        case FNET_DHCP_STATE_REQUESTING: /*=>Requesting*/
            ip_address.s_addr = INADDR_BROADCAST;
            fnet_dhcp_add_option(message, FNET_DHCP_OPTION_REQ_ADDRESS,
                                 FNET_DHCP_OPTION_REQ_ADDRESS_LENGTH, &dhcp->offered_options.public_options.ip_address);
            fnet_dhcp_add_option(message, FNET_DHCP_OPTION_SERVER_ID,
                                 FNET_DHCP_OPTION_SERVER_ID_LENGTH, &dhcp->offered_options.public_options.dhcp_server);
            message_type = FNET_DHCP_OPTION_TYPE_REQUEST;
            break;
        case FNET_DHCP_STATE_PROBING:
            ip_address = dhcp->current_options.public_options.dhcp_server; /* Send REQUEST to leasing server*/
            message->header.ciaddr = dhcp->current_options.public_options.ip_address.s_addr;
            message_type = FNET_DHCP_OPTION_TYPE_DECLINE; /* DHCPDECLINE - Client to server indicating network address is already in use.*/
            break;
        case FNET_DHCP_STATE_SELECTING:
            ip_address.s_addr = INADDR_BROADCAST;
            message_type = FNET_DHCP_OPTION_TYPE_DISCOVER;
            break;
        case FNET_DHCP_STATE_REBOOTING: /*=>Requesting*/
            ip_address.s_addr = INADDR_BROADCAST;
            fnet_dhcp_add_option(message, FNET_DHCP_OPTION_REQ_ADDRESS,
                                 FNET_DHCP_OPTION_REQ_ADDRESS_LENGTH, &dhcp->in_params.requested_ip_address);
            message_type = FNET_DHCP_OPTION_TYPE_REQUEST;
            break;
        case FNET_DHCP_STATE_RENEWING:
            ip_address = dhcp->current_options.public_options.dhcp_server; /* Send REQUEST to leasing server*/
            message->header.ciaddr = dhcp->current_options.public_options.ip_address.s_addr;
            message_type = FNET_DHCP_OPTION_TYPE_REQUEST;
            break;
        case FNET_DHCP_STATE_REBINDING:
            ip_address.s_addr = INADDR_BROADCAST;
            message_type = FNET_DHCP_OPTION_TYPE_REQUEST;
            message->header.ciaddr = dhcp->current_options.public_options.ip_address.s_addr;
            break;
        case FNET_DHCP_STATE_RELEASE:
            ip_address = dhcp->current_options.public_options.dhcp_server;
            message->header.ciaddr = dhcp->current_options.public_options.ip_address.s_addr;
            message_type = FNET_DHCP_OPTION_TYPE_RELEASE;
            break;
#endif /* !FNET_CFG_DHCP_BOOTP */
        default:
            return FNET_ERR;
    }

#if FNET_CFG_DHCP_BOOTP

    /* Record time when client sent the DHCPREQUEST */
    dhcp->send_request_time = fnet_timer_ticks();

#else /* DHCP */

    fnet_dhcp_add_option(message, FNET_DHCP_OPTION_TYPE, FNET_DHCP_OPTION_TYPE_LENGTH, &message_type);

    if((message_type == FNET_DHCP_OPTION_TYPE_REQUEST) || (message_type == FNET_DHCP_OPTION_TYPE_DISCOVER))
    {
        /* Record time when client sent the DHCPREQUEST */
        dhcp->send_request_time = fnet_timer_ticks();

        /* Request a lease time for the IP address */
        if(dhcp->in_params.requested_lease_time)
        {
            fnet_dhcp_add_option(message,
                                 FNET_DHCP_OPTION_LEASE,
                                 FNET_DHCP_OPTION_LEASE_LENGTH,
                                 &dhcp->in_params.requested_lease_time);
        }
        /* Add Parameter Request list */
        fnet_dhcp_add_option(message,
                             FNET_DHCP_OPTION_PARAMETER_REQ_LIST,
                             sizeof(fnet_dhcp_parameter_request_list),
                             &fnet_dhcp_parameter_request_list);
    }

    /* Add Maximum DHCP message size option. */
    max_message_size = fnet_htons(sizeof(fnet_dhcp_header_t));
    fnet_dhcp_add_option(message,                              FNET_DHCP_OPTION_MESSAGE_SIZE,
                         FNET_DHCP_OPTION_MESSAGE_SIZE_LENGTH, &max_message_size);

    /* Client ID (MAC) option */
    client_id[0] = message->header.htype;
    fnet_memcpy(&client_id[1], dhcp->macaddr, sizeof(dhcp->macaddr));
    fnet_dhcp_add_option(&dhcp->message,                    FNET_DHCP_OPTION_CLIENT_ID,
                         FNET_DHCP_OPTION_CLIENT_ID_LENGTH, &client_id);

#endif /* FNET_CFG_DHCP_BOOTP */

    /* End Option */
    *message->next_option_position++ = FNET_DHCP_OPTION_END;


    /* Send message. */
    fnet_memset_zero(&addr_send, sizeof(addr_send));
    ((struct sockaddr_in *)(&addr_send))->sin_family = AF_INET;
    ((struct sockaddr_in *)(&addr_send))->sin_port = FNET_CFG_DHCP_PORT_SERVER;
    ((struct sockaddr_in *)(&addr_send))->sin_addr = ip_address;

    length = (fnet_size_t)(message->next_option_position - (fnet_uint8_t *) &message->header);
    return fnet_socket_sendto(dhcp->socket_client, (fnet_uint8_t *) &message->header, length, 0u, (struct sockaddr *) &addr_send, sizeof(addr_send));
}

/************************************************************************
* NAME: dhcp_receive_message
*
* DESCRIPTION: Receive DHCP message (non blocking).
************************************************************************/
static fnet_int32_t fnet_dhcp_receive_message( fnet_dhcp_if_t *dhcp, struct fnet_dhcp_options_in *options )
{
    fnet_int32_t            size;
    struct sockaddr         addr_from;
    fnet_dhcp_header_t      *dhcp_header = &dhcp->message.header;
    fnet_size_t             addr_len = sizeof(addr_from);

    size = fnet_socket_recvfrom(dhcp->socket_client, (fnet_uint8_t *) dhcp_header, sizeof(fnet_dhcp_header_t),
                                0U,                   (struct sockaddr *) &addr_from, &addr_len);

    if(fnet_timer_get_interval(dhcp->send_request_time, fnet_timer_ticks()) < dhcp->state_send_timeout)
    {
        if((size < (fnet_int32_t)(sizeof(fnet_dhcp_header_t) - FNET_DHCP_OPTIONS_LENGTH))
           || (dhcp_header->xid != fnet_htonl(dhcp->xid))  /* Is message for us? */
           || (dhcp_header->hlen != sizeof(dhcp->macaddr))
           || (fnet_memcmp(dhcp_header->chaddr, dhcp->macaddr, sizeof(dhcp->macaddr)))
           || (fnet_memcmp(&dhcp_header->magic_cookie[0], fnet_dhcp_magic_cookie,
                           sizeof(fnet_dhcp_magic_cookie))) )
        {
            size = 0;
        }
        else
        {
            fnet_memset_zero(options, sizeof(*options));                        /* Clear old options.*/
            options->public_options.ip_address.s_addr = dhcp_header->yiaddr;    /* our IP address */

            /* Parse options field */
            dhcp->message.next_option_position = &dhcp_header->options[0];
            dhcp->message.end_position = (fnet_uint8_t *) dhcp_header + size - 1;

            fnet_dhcp_parse_options(&dhcp->message, options);

#if FNET_CFG_DHCP_OVERLOAD && !FNET_CFG_DHCP_BOOTP
            /* Parse overload options in sname/file */

            if(options->public_options.overload)
            {
                if(options->public_options.overload & FNET_DHCP_OPTION_OVERLOAD_SNAME)
                    dhcp->message->next_option_position = &dhcp_header->sname[0];
                else
                    dhcp->message->next_option_position = &dhcp_header->file[0];

                if(options->public_options.overload & FNET_DHCP_OPTION_OVERLOAD_FILE)
                    dhcp->message->end_position = &dhcp_header->file[128];
                else
                    dhcp->message->end_position = &dhcp_header->sname[64];

                fnet_dhcp_parse_options(dhcp->message, options);
            }
#endif

        }

    }
    else
    {
        size = FNET_DHCP_ISTIMEOUT;
    }

    return size;
}

/************************************************************************
* NAME: fnet_dhcp_change_state
*
* DESCRIPTION: Change state of DHCP client.
************************************************************************/
static void fnet_dhcp_change_state( fnet_dhcp_if_t *dhcp, fnet_dhcp_state_t state )
{
    dhcp->state = state;

    fnet_dhcp_print_state(dhcp);

    switch(state)
    {
#if !FNET_CFG_DHCP_BOOTP
        case FNET_DHCP_STATE_INIT_REBOOT:
#endif
        case FNET_DHCP_STATE_INIT:
            fnet_netif_set_ip4_addr(dhcp->netif, 0U);   /* Set zero address. DHCP messages broadcast
                                                        * by a client prior to that client obtaining
                                                        * its IP address must have the source address
                                                        * field in IP header set to 0.*/
            dhcp->xid++; /* Todo must be random.*/
            break;
        case FNET_DHCP_STATE_SELECTING:
            fnet_dhcp_send_message(dhcp);                          /* Send DISCOVER */
            dhcp->state_send_timeout = FNET_DHCP_STATE_SELECTING_SEND_TIMEOUT
                                       / FNET_TIMER_PERIOD_MS; /* Wait OFFER */
            break;
        case FNET_DHCP_STATE_BOUND:
#if !FNET_CFG_DHCP_BOOTP
            dhcp->state_timeout_next_state = FNET_DHCP_STATE_RENEWING;
            dhcp->lease_obtained_time = dhcp->send_request_time;

            if(dhcp->current_options.public_options.t1 == FNET_HTONL(FNET_DHCP_LEASE_INFINITY))
            {
                dhcp->state_timeout = FNET_DHCP_LEASE_INFINITY;
            }
            else
            {
                dhcp->state_timeout = (fnet_ntohl(dhcp->current_options.public_options.t1) * 1000U) / FNET_TIMER_PERIOD_MS;
            }
#endif /* !FNET_CFG_DHCP_BOOTP */
            break;
#if !FNET_CFG_DHCP_BOOTP
        case FNET_DHCP_STATE_REQUESTING:
            fnet_dhcp_send_message(dhcp); /* Send REQUEST.*/
            dhcp->state_timeout_next_state = FNET_DHCP_STATE_INIT;
            dhcp->lease_obtained_time = dhcp->send_request_time;
            dhcp->state_send_timeout = FNET_DHCP_STATE_REQUESTING_SEND_TIMEOUT
                                       / FNET_TIMER_PERIOD_MS; /* Wait ACK */
            dhcp->state_timeout = FNET_DHCP_STATE_REQUESTING_TIMEOUT / FNET_TIMER_PERIOD_MS;
            break;
        case FNET_DHCP_STATE_PROBING:
            fnet_arp_send_request(dhcp->netif, dhcp->current_options.public_options.ip_address.s_addr ); /* Send ARP probe.*/
            /* Record time when client sent the ARP brobe */
            dhcp->send_request_time = fnet_timer_ticks();
            dhcp->state_timeout = FNET_DHCP_STATE_PROBING_TIMEOUT / FNET_TIMER_PERIOD_MS;
            break;
        case FNET_DHCP_STATE_REBOOTING:
            fnet_dhcp_send_message(dhcp);               /* Send REQUEST.*/
            dhcp->state_timeout_next_state = FNET_DHCP_STATE_INIT;
            dhcp->lease_obtained_time = dhcp->send_request_time; /* To follow state machine rules.*/
            dhcp->state_timeout = FNET_DHCP_STATE_REBOOTING_TIMEOUT / FNET_TIMER_PERIOD_MS;
            dhcp->state_send_timeout = FNET_DHCP_STATE_REBOOTING_SEND_TIMEOUT / FNET_TIMER_PERIOD_MS;
            break;
        case FNET_DHCP_STATE_RENEWING:
            fnet_dhcp_send_message(dhcp); /* Send REQUEST.*/
            dhcp->state_timeout_next_state = FNET_DHCP_STATE_REBINDING;
            dhcp->state_timeout = (fnet_ntohl(dhcp->current_options.public_options.t2) * 1000U)
                                  / FNET_TIMER_PERIOD_MS;

            dhcp->state_send_timeout = FNET_DHCP_STATE_RENEWING_SEND_TIMEOUT / FNET_TIMER_PERIOD_MS;
            break;
        case FNET_DHCP_STATE_REBINDING:
            fnet_dhcp_send_message(dhcp); /* Send REQUEST.*/
            dhcp->state_timeout_next_state = FNET_DHCP_STATE_INIT;
            dhcp->state_timeout = (fnet_ntohl(dhcp->current_options.public_options.lease_time) * 1000U) / FNET_TIMER_PERIOD_MS;
            dhcp->state_send_timeout = FNET_DHCP_STATE_REBINDING_SEND_TIMEOUT / FNET_TIMER_PERIOD_MS;
            break;
        case FNET_DHCP_STATE_RELEASE:
            break;
        case FNET_DHCP_STATE_DISABLED:
            dhcp->enabled = FNET_FALSE;
            fnet_socket_close(dhcp->socket_client);
            fnet_poll_service_unregister(dhcp->service_descriptor); /* Delete service. */
            break;
        default:
            break;  /* do nothing, avoid compiler warning "enumeration value not handled in switch" */
#endif /* !FNET_CFG_DHCP_BOOTP */
    }
}

/************************************************************************
* NAME: fnet_dhcp_apply_params
*
* DESCRIPTION: Apply DHCP parameters to the interface.
************************************************************************/
static void fnet_dhcp_apply_params(fnet_dhcp_if_t *dhcp)
{
    /* Apply parameters. */
    fnet_netif_set_ip4_addr(dhcp->netif, dhcp->current_options.public_options.ip_address.s_addr);
    fnet_netif_set_ip4_subnet_mask(dhcp->netif, dhcp->current_options.public_options.netmask.s_addr);
    fnet_netif_set_ip4_gateway(dhcp->netif, dhcp->current_options.public_options.gateway.s_addr);
#if FNET_CFG_DNS
    fnet_netif_set_ip4_dns(dhcp->netif, dhcp->current_options.public_options.dns.s_addr);
#endif
    fnet_netif_set_ip4_addr_type(dhcp->netif, FNET_NETIF_IP_ADDR_TYPE_DHCP);

    fnet_dhcp_change_state(dhcp, FNET_DHCP_STATE_BOUND); /* => BOUND */
    /* Rise event. */
    if(dhcp->callback_updated)
    {
        dhcp->callback_updated((fnet_dhcp_desc_t)dhcp, dhcp->netif, dhcp->callback_updated_param);
    }
}

/************************************************************************
* NAME: fnet_dhcp_state_machine
*
* DESCRIPTION: DHCP client state machine.
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
                             ----------
*/

/*  BOOTP:


                                             BOOTREPLY/
  -------   -/Send         ---------------   Configure client   -------
 |       |  BOOTPREQUEST  |               |  and StopTask      |       |
 | INIT  |--------------->| BOOTP_REQUEST |------------------->| BOUND |
 |       |                | (SELECTING)   |                    |       |
  -------                  ---------------                      -------
     ^                           |
     |         Timeout/-         |
     +---------------------------+


*/

static void fnet_dhcp_state_machine( void *fnet_dhcp_if_p )
{
    fnet_dhcp_if_t                  *dhcp = (fnet_dhcp_if_t *)fnet_dhcp_if_p;
    struct fnet_dhcp_options_in     options;
    fnet_int32_t                    res;

    switch(dhcp->state)
    {
        /*---- INIT ----------------------------------------------------*/
        case FNET_DHCP_STATE_INIT:
            /* Todo: The client SHOULD wait a random time between one and ten seconds to
            * desynchronize the use of DHCP at startup. */
            fnet_dhcp_change_state(dhcp, FNET_DHCP_STATE_SELECTING); /* => SELECTING */
            if(dhcp->callback_discover)
            {
                dhcp->callback_discover((fnet_dhcp_desc_t)dhcp, dhcp->netif, dhcp->callback_discover_param);
            }
            break;
        /*---- SELECTING --------------------------------------------*/
        case FNET_DHCP_STATE_SELECTING:
            /* Waiting for OFFER */
            res = fnet_dhcp_receive_message(dhcp, &options);

            if(res == FNET_DHCP_ISTIMEOUT)
            {
                fnet_dhcp_change_state(dhcp, FNET_DHCP_STATE_INIT); /* => INIT */
            }

#if FNET_CFG_DHCP_BOOTP
            else if(res > 0)
            {
                /* Todo: The client SHOULD perform a check on the suggested address
                * to ensure that the address is not already in use.*/
                fnet_dhcp_print_options(&options);
                /* Apply parameters. */
                dhcp->current_options = options;

                fnet_dhcp_apply_params(dhcp);
            }
#else /* DHCP */
            else if((res > 0) && (options.private_options.message_type == FNET_DHCP_OPTION_TYPE_OFFER))
            {
                dhcp->offered_options = options;                          /* Save offered options */
                fnet_dhcp_change_state(dhcp, FNET_DHCP_STATE_REQUESTING); /* => REQUESTING */
            }
            else
            {}
#endif
            break;
        /*---- BOUND ------------------------------------------------*/
        case FNET_DHCP_STATE_BOUND:
#if FNET_CFG_DHCP_BOOTP
            fnet_dhcp_release(dhcp);
#else /* DHCP */
            if(fnet_netif_get_ip4_addr_type(dhcp->netif) == FNET_NETIF_IP_ADDR_TYPE_DHCP) /* If user did not change parameters manually.*/
            {
                struct sockaddr     addr_from;
                fnet_size_t         addr_len = sizeof(addr_from);

                /* Discard all input data. */
                fnet_socket_recvfrom(dhcp->socket_client, (fnet_uint8_t *) &dhcp->message.header, sizeof(fnet_dhcp_header_t),
                                     0U,                   (struct sockaddr *) &addr_from, &addr_len);


                /* If T1 expired. */
                if(fnet_timer_get_interval(dhcp->lease_obtained_time, fnet_timer_ticks()) > dhcp->state_timeout)
                {
                    fnet_dhcp_change_state(dhcp, dhcp->state_timeout_next_state); /* => INIT */
                }
            }
            else
            {
                fnet_dhcp_release((fnet_dhcp_desc_t)dhcp); /* address parameters changed outside => Disable DHCP*/
            }
#endif
            break;

#if !FNET_CFG_DHCP_BOOTP  /* DHCP */
        /*---- FNET_DHCP_STATE_INIT_REBOOT -----------------------------*/
        case FNET_DHCP_STATE_INIT_REBOOT:
            fnet_dhcp_change_state(dhcp, FNET_DHCP_STATE_REBOOTING); /* => REBOOTING */
            if(dhcp->callback_discover)
            {
                dhcp->callback_discover((fnet_dhcp_desc_t)dhcp, dhcp->netif, dhcp->callback_discover_param);
            }
            break;
        /*---- RENEWING -------------------------------------------------*/
        case FNET_DHCP_STATE_RENEWING:
        /*---- REBINDING ------------------------------------------------*/
        case FNET_DHCP_STATE_REBINDING:
            if(fnet_netif_get_ip4_addr_type(dhcp->netif) != FNET_NETIF_IP_ADDR_TYPE_DHCP) /* If user changed parameters manually. */
            {
                fnet_dhcp_release((fnet_dhcp_desc_t)dhcp);                           /* Disable DHCP if user has changed ip parameters. */
                break;
            }
        /*---- REBOOTING ------------------------------------------------*/
        case FNET_DHCP_STATE_REBOOTING:
        /*---- REQUESTING -----------------------------------------------*/
        case FNET_DHCP_STATE_REQUESTING:
            if(fnet_timer_get_interval(dhcp->lease_obtained_time, fnet_timer_ticks()) < dhcp->state_timeout)
            {
                /* Waiting for ACK */
                res = fnet_dhcp_receive_message(dhcp, &options);

                if(res == FNET_DHCP_ISTIMEOUT)
                {
                    fnet_dhcp_send_message(dhcp); /* Resend REQUEST.*/
                }
                else if(res > 0)
                {
                    if((options.private_options.message_type == FNET_DHCP_OPTION_TYPE_ACK) /* ACK */
                       /* Check options that must be present*/
                       && (options.public_options.ip_address.s_addr)
                       && (options.public_options.dhcp_server.s_addr)
                       && (options.public_options.lease_time))
                    {
                        /* Todo: The client SHOULD perform a check on the suggested address
                        * to ensure that the address is not already in use.*/
                        fnet_dhcp_print_options(&options);

                        dhcp->lease_obtained_time = dhcp->send_request_time; /* save lease obtained time.*/

                        /* Check T1, T2 and lease time */
                        if(options.public_options.lease_time == FNET_HTONL(FNET_DHCP_LEASE_INFINITY))
                        {
                            options.public_options.t1 = FNET_HTONL(FNET_DHCP_LEASE_INFINITY);
                            options.public_options.t2 = FNET_HTONL(FNET_DHCP_LEASE_INFINITY);
                        }
                        else
                        {
                            fnet_uint32_t orig_lease_time = options.public_options.lease_time;

                            if(fnet_ntohl(options.public_options.lease_time) < FNET_DHCP_LEASE_MIN)
                            {
                                options.public_options.lease_time = FNET_HTONL((fnet_uint32_t)FNET_DHCP_LEASE_MIN);
                            }
                            else if(fnet_ntohl(options.public_options.lease_time) > FNET_DHCP_LEASE_MAX)
                            {
                                options.public_options.lease_time = FNET_HTONL(FNET_DHCP_LEASE_MAX);
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
                            fnet_dhcp_change_state(dhcp, FNET_DHCP_STATE_PROBING);    /* => PROBING */
                        }
                        else
                        {
                            /* Apply parameters. */
                            fnet_dhcp_apply_params(dhcp);
                        }
                    }
                    else if(options.private_options.message_type == FNET_DHCP_OPTION_TYPE_NAK) /* NAK */
                    {
                        fnet_dhcp_change_state(dhcp, FNET_DHCP_STATE_INIT);    /* => INIT */
                    }
                    else
                    {}
                }
                else
                {}
            }
            else
            {
                fnet_dhcp_change_state(dhcp, dhcp->state_timeout_next_state);
            }
            break;
        /*---- REQUESTING -----------------------------------------------*/
        case FNET_DHCP_STATE_PROBING:
            if(fnet_timer_get_interval(dhcp->send_request_time, fnet_timer_ticks()) < dhcp->state_timeout)
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
                    fnet_dhcp_send_message(dhcp); /* Send DECLINE.*/

                    fnet_dhcp_change_state(dhcp, FNET_DHCP_STATE_INIT);    /* => INIT */
                }
            }
            else
            {
                /* No conflict detected.*/
                /* Apply parameters. */
                fnet_dhcp_apply_params(dhcp);
            }
            break;
        /*---- RELEASING --------------------------------------------*/
        case FNET_DHCP_STATE_RELEASE:
            if(dhcp->current_options.public_options.ip_address.s_addr)             /* If obtained before.*/
            {
                fnet_dhcp_send_message(dhcp);                       /* Send RELEASE */
            }

            if(fnet_netif_get_ip4_addr_type(dhcp->netif) == FNET_NETIF_IP_ADDR_TYPE_DHCP)   /* If address is DHCP => do not use it. */
            {
                fnet_netif_set_ip4_addr(dhcp->netif, INADDR_ANY);    /* Set zero address */
            }

            fnet_dhcp_change_state(dhcp, FNET_DHCP_STATE_DISABLED); /* => DISABLED */
            break;
        case FNET_DHCP_STATE_DISABLED:
        default:
            break;
#endif /* !FNET_CFG_DHCP_BOOTP */
    }
}

/************************************************************************
* NAME: fnet_dhcp_init
*
* DESCRIPTION: DHCP client initialization.
************************************************************************/
fnet_dhcp_desc_t fnet_dhcp_init( fnet_netif_desc_t netif, struct fnet_dhcp_params *params )
{
    struct sockaddr     addr_client;
    fnet_dhcp_state_t   state = FNET_DHCP_STATE_INIT;
    fnet_dhcp_if_t      *dhcp_if = 0;
    fnet_index_t        i;

    if(netif == 0)
    {
        FNET_DEBUG_DHCP(FNET_DHCP_ERR_NONETWORK);
        goto ERROR;
    }

    /* Try to find free DHCP server. */
    for(i = 0u; i < FNET_CFG_DHCP_MAX; i++)
    {
        if(fnet_dhcp_if_list[i].enabled == FNET_FALSE)
        {
            dhcp_if = &fnet_dhcp_if_list[i];
        }
        else
        {
            if(fnet_dhcp_if_list[i].netif == netif)
            {
                /* It is not posible to run several DHCP servers on the same networking interface.*/
                dhcp_if = 0;
                break;
            }
        }
    }

    /* Is DHCP server already initialized. */
    if(dhcp_if == 0)
    {
        FNET_DEBUG_DHCP(FNET_DHCP_ERR_IS_INITIALIZED);
        goto ERROR;
    }

    /* Create client socket */
    fnet_memset_zero(&addr_client, sizeof(addr_client));
    ((struct sockaddr_in *)(&addr_client))->sin_family = AF_INET;
    ((struct sockaddr_in *)(&addr_client))->sin_port = FNET_CFG_DHCP_PORT_CLIENT;
    ((struct sockaddr_in *)(&addr_client))->sin_addr.s_addr = FNET_HTONL(INADDR_ANY);
    ((struct sockaddr_in *)(&addr_client))->sin_scope_id = fnet_netif_get_scope_id(netif);

    if((dhcp_if->socket_client = fnet_socket(AF_INET, SOCK_DGRAM, 0u)) == FNET_ERR)
    {
        FNET_DEBUG_DHCP(FNET_DHCP_ERR_SOCKET_CREATION);
        goto ERROR;
    }

    if(fnet_socket_bind(dhcp_if->socket_client, (struct sockaddr *) &addr_client, sizeof(addr_client)) != 0)
    {
        FNET_DEBUG_DHCP(FNET_DHCP_ERR_SOCKET_BIND);
        goto ERROR_1;
    }

    dhcp_if->service_descriptor = fnet_poll_service_register(fnet_dhcp_state_machine, (void *) dhcp_if);

    if(dhcp_if->service_descriptor == 0)
    {
        FNET_DEBUG_DHCP(FNET_DHCP_ERR_SERVICE);
        goto ERROR_1;
    }

    dhcp_if->netif = netif;
    fnet_netif_get_hw_addr(netif, dhcp_if->macaddr, sizeof(fnet_mac_addr_t));

    if(params)
    {
        dhcp_if->in_params.requested_ip_address = params->requested_ip_address;

#if !FNET_CFG_DHCP_BOOTP  /* DHCP */
        /* Convert "requested_lease_time" to network byte order.*/
        dhcp_if->in_params.requested_lease_time = fnet_htonl(params->requested_lease_time);

        if(dhcp_if->in_params.requested_ip_address.s_addr)
        {
            /* Initialization with known network address.
            * The client begins in INIT-REBOOT state and sends a DHCPREQUEST
            * message.  The client MUST insert its known network address as a
            * 'requested IP address' option in the DHCPREQUEST message.*/
            state = FNET_DHCP_STATE_INIT_REBOOT;
        }

        dhcp_if->in_params.probe_addr = params->probe_addr;
#endif
    }

    fnet_dhcp_change_state(dhcp_if, state);

    dhcp_if->enabled = FNET_TRUE;

    return (fnet_dhcp_desc_t)dhcp_if;
ERROR_1:
    fnet_socket_close(dhcp_if->socket_client);

ERROR:
    return 0;
}

/************************************************************************
* NAME: fnet_dhcp_release
*
* DESCRIPTION: DHCP client release.
************************************************************************/
void fnet_dhcp_release(fnet_dhcp_desc_t desc)
{
    struct fnet_dhcp_if   *dhcp_if = (struct fnet_dhcp_if *) desc;

    if(dhcp_if && (dhcp_if->enabled == FNET_TRUE))
    {

#if FNET_CFG_DHCP_BOOTP
        fnet_dhcp_change_state(dhcp_if, FNET_DHCP_STATE_DISABLED); /* => DISABLED */
#else
        /* Graceful shutdown */
        fnet_dhcp_change_state(dhcp_if, FNET_DHCP_STATE_RELEASE);
        fnet_dhcp_state_machine(dhcp_if);                /* 1 pass. */
#endif

    }
}

/************************************************************************
* NAME: fnet_dhcp_is_enabled
*
* DESCRIPTION: This function returns FNET_TRUE if the DHCP client
*              is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_dhcp_is_enabled(fnet_dhcp_desc_t desc)
{
    struct fnet_dhcp_if     *dhcp_if = (struct fnet_dhcp_if *) desc;
    fnet_bool_t             result;

    if(dhcp_if)
    {
        result = dhcp_if->enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

/************************************************************************
* NAME: fnet_dhcp_get_options
*
* DESCRIPTION: This function copies current DHCP options to structure
*              pointed by 'options'.
************************************************************************/
void fnet_dhcp_get_options(fnet_dhcp_desc_t desc, struct fnet_dhcp_options *options )
{
    struct fnet_dhcp_if   *dhcp_if = (struct fnet_dhcp_if *) desc;

    if(options && dhcp_if)
    {
        *options = dhcp_if->current_options.public_options;
    }
}

/************************************************************************
* NAME: fnet_dhcp_set_callback_updated
*
* DESCRIPTION: Registers the "IP parameters updated" DHCP event handler callback.
************************************************************************/
void fnet_dhcp_set_callback_updated(fnet_dhcp_desc_t desc, fnet_dhcp_callback_t callback_updated, void *param)
{
    struct fnet_dhcp_if   *dhcp_if = (struct fnet_dhcp_if *) desc;

    if(dhcp_if)
    {
        dhcp_if->callback_updated = callback_updated;
        dhcp_if->callback_updated_param = param;
    }
}

/************************************************************************
* NAME: fnet_dhcp_set_callback_discover
*
* DESCRIPTION: Registers the "Discover message sent" DHCP event handler callback.
************************************************************************/
void fnet_dhcp_set_callback_discover(fnet_dhcp_desc_t desc, fnet_dhcp_callback_t callback_discover, void *param)
{
    struct fnet_dhcp_if   *dhcp_if = (struct fnet_dhcp_if *) desc;

    if(dhcp_if)
    {
        dhcp_if->callback_discover = callback_discover;
        dhcp_if->callback_discover_param = param;
    }
}


#endif /* FNET_CFG_DHCP && FNET_CFG_IP4 */

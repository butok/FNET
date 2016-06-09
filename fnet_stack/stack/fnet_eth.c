/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
/*!
*
* @file fnet_eth.c
*
* @author Andrey Butok
*
* @brief Ethernet platform independent API functions .
*
***************************************************************************/

#include "fnet.h"
#include "fnet_eth_prv.h"
#include "fnet_prot.h"

/************************************************************************
*     Global Data Structures
*************************************************************************/

/* Null MAC address */
const fnet_mac_addr_t fnet_eth_null_addr =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* Broadcast MAC address */
const fnet_mac_addr_t fnet_eth_broadcast =
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/************************************************************************
* NAME: fnet_mac_to_str
*
* DESCRIPTION: Converts MAC address to an null-terminated string.
*************************************************************************/
fnet_char_t *fnet_mac_to_str( const fnet_mac_addr_t addr, fnet_char_t *str_mac )
{
    fnet_uint8_t *p;
    if(str_mac)
    {
        p = (fnet_uint8_t *)addr;
        fnet_sprintf(str_mac, "%02X:%02X:%02X:%02X:%02X:%02X", p[0], p[1], p[2], p[3], p[4], p[5]);
    }
    return str_mac;
}

/************************************************************************
* NAME: fnet_str_to_mac
*
* DESCRIPTION: This function interprets the character string into MAC addr.
*************************************************************************/
fnet_return_t fnet_str_to_mac( const fnet_char_t *str_mac, fnet_mac_addr_t addr )
{
    fnet_uint32_t   val;
    fnet_uint8_t    c;
    fnet_uint32_t   octet[6];
    fnet_uint32_t   *octetptr = octet;
    fnet_index_t    i;

    while(1)
    {
        val = 0U;

        while((c = *str_mac) != 0U)
        {
            if((c >= '0') && (c <= '9'))
            {
                val = (val * 16U) + (fnet_uint32_t)(c) - '0';
                str_mac++;
                continue;
            }
            else if(((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')))
            {
                val = (val << 4U) + ((fnet_uint32_t)c + 10U - (((c >= 'a') && (c <= 'f')) ? (fnet_uint32_t)'a' : (fnet_uint32_t)'A'));
                str_mac++;
                continue;
            }
            else
            {}

            break;
        }

        if(*str_mac == ':')
        {
            *octetptr++ = val;
            str_mac++;

            if(octetptr >= octet + 6)
            {
                goto ERROR;
            }

            continue;
        }

        break;
    }
    /* Check for trailing characters. */
    if((*str_mac) && (!(*str_mac == ' ')))
    {
        goto ERROR;
    }

    *octetptr++ = val;

    if((octetptr - octet) == 6)
    {
        for (i = 0u; i < 6u; i++)
        {
            addr[i] = (fnet_uint8_t)octet[i];
        }
    }
    else
    {
        goto ERROR;
    }

    return (FNET_OK);
ERROR:
    return (FNET_ERR);
}

#if (FNET_CFG_CPU_ETH0 ||FNET_CFG_CPU_ETH1)

/* Number of initialised ethernet devices.*/
static fnet_index_t fnet_eth_number = 0U;

/************************************************************************
*     List of Network Layer Protocols used by Ethernet Interface.
*************************************************************************/
static const fnet_eth_prot_if_t fnet_eth_prot_if_list[] =
{
#if FNET_CFG_IP4
    { /* ARP */
        FNET_HTONS(FNET_ETH_TYPE_ARP),  /* Protocol number */
        fnet_arp_input                  /* Protocol input function.*/
    },
    {   /* IPv4 */
        FNET_HTONS(FNET_ETH_TYPE_IP4),  /* Protocol number */
        fnet_ip_input                   /* Protocol input function.*/
    }
#endif /* FNET_CFG_IP4 */
#if FNET_CFG_IP6
#if FNET_CFG_IP4
    ,
#endif /* FNET_CFG_IP4 */
    {   /* IPv4 */
        FNET_HTONS(FNET_ETH_TYPE_IP6),   /* Protocol number */
        fnet_ip6_input                   /* Protocol input function.*/
    }
#endif /* FNET_CFG_IP6 */
    /* ADD HERE YOUR NETWORK LAYER PROTOCOL */
};

#define FNET_ETH_PROT_IF_LIST_SIZE  (sizeof(fnet_eth_prot_if_list)/sizeof(fnet_eth_prot_if_t))

/************************************************************************
 * Ethernet Multicast Address
 ***********************************************************************/
/* RFC1112 6.4: An IP host group address is mapped to an Ethernet multicast address
 * by placing the low-order 23-bits of the IP address into the low-order
 * 23 bits of the Ethernet multicast address 01-00-5E-00-00-00 (hex).
 */
#define FNET_ETH_MULTICAST_IP4_TO_MAC(ip4_addr, mac_addr)  \
    do{   \
        (mac_addr)[0] = 0x01U; \
        (mac_addr)[1] = 0x00U; \
        (mac_addr)[2] = 0x5EU; \
        (mac_addr)[3] = (fnet_uint8_t)(((fnet_uint8_t *)(&(ip4_addr)))[1] & 0x7FU); \
        (mac_addr)[4] = ((fnet_uint8_t *)(&(ip4_addr)))[2];  \
        (mac_addr)[5] = ((fnet_uint8_t *)(&(ip4_addr)))[3];  \
    }while(0)

/* For IPv6 */
#define FNET_ETH_MULTICAST_IP6_TO_MAC(ip6_addr, mac_addr)        \
    do{   \
        (mac_addr)[0] = 0x33U;               \
        (mac_addr)[1] = 0x33U;               \
        (mac_addr)[2] = (ip6_addr)->addr[12]; \
        (mac_addr)[3] = (ip6_addr)->addr[13]; \
        (mac_addr)[4] = (ip6_addr)->addr[14]; \
        (mac_addr)[5] = (ip6_addr)->addr[15];  \
    }while(0)



/******************************************************************************
*     Function Prototypes
*******************************************************************************/
#define FNET_ETH_TIMER_PERIOD (4000U) /*ms*/
static void fnet_eth_timer(fnet_uint32_t cookie );

/************************************************************************
* NAME: fnet_eth_prot_input
*
* DESCRIPTION: Eth. network-layer input function.
*************************************************************************/
void fnet_eth_prot_input( fnet_netif_t *netif, fnet_netbuf_t *nb, fnet_uint16_t protocol )
{
    fnet_index_t i;

    if(netif && nb)
    {
        /* Find Network-layer protocol.*/
        for(i = 0U; i < FNET_ETH_PROT_IF_LIST_SIZE; i++)
        {
            if( protocol == fnet_eth_prot_if_list[i].protocol)
            {
                /* Call the protocol-input function.*/
                fnet_eth_prot_if_list[i].input(netif, nb);
                break;
            }
        }

        if(i == FNET_ETH_PROT_IF_LIST_SIZE)
        {
            /* No procol found */
            fnet_netbuf_free_chain(nb);
        }
    }
}

/************************************************************************
* NAME: fnet_eth_output
*
* DESCRIPTION: Ethernet low-level output function.
*************************************************************************/
void fnet_eth_output(fnet_netif_t *netif, fnet_uint16_t type, const fnet_mac_addr_t dest_addr, fnet_netbuf_t *nb )
{
    ((fnet_eth_if_t *)(netif->if_ptr))->output(netif, type, dest_addr, nb);
}

/************************************************************************
* NAME: fnet_eth_init
*
* DESCRIPTION: Do initialization for an Ethernet-type interface.
*************************************************************************/
fnet_return_t fnet_eth_init( fnet_netif_t *netif)
{
    fnet_return_t result  = FNET_ERR;
    fnet_eth_if_t *eth_if = (fnet_eth_if_t *)(netif->if_ptr);

    if(eth_if)
    {
#if !FNET_CFG_CPU_ETH_MIB
        /* Clear Ethernet statistics. */
        fnet_memset_zero(&eth_if->statistics, sizeof(struct fnet_netif_statistics));
#endif

#if FNET_CFG_IP4
        result = fnet_arp_init(netif, &eth_if->arp_if); /* Init ARP for this interface.*/
#else
        result = FNET_OK;
#endif /* FNET_CFG_IP4 */

        if(result == FNET_OK)
        {

#if FNET_CFG_IP6
#if FNET_CFG_IP6_PMTU_DISCOVERY
            fnet_netif_pmtu_init(netif);
#endif

            /* Init Neighbor Discovery.*/
            if( ( result = fnet_nd6_init (netif, &eth_if->nd6_if) ) == FNET_OK)
            {
                /* RFC4861 6.3.3: The host joins the all-nodes multicast address on all
                 * multicastcapable interfaces.
                 */
                fnet_ip6_multicast_join(netif, &fnet_ip6_addr_linklocal_allnodes);

                /* To speed the autoconfiguration process, a host may generate its linklocal
                 * address (and verify its uniqueness) in parallel with waiting
                 * for a Router Advertisement. Because a router may delay responding to
                 * a Router Solicitation for a few seconds, the total time needed to
                 * complete autoconfiguration can be significantly longer if the two
                 * steps are done serially.
                 */


                /* Link-Local Address Generation/Auto configuration.
                 * It comprises of '1111111010' as the first ten bits followed by 54 zeroes
                 * and a 64 bit interface identifier.
                 * For all autoconfiguration types, a link-local address is always configured.
                 */
                fnet_netif_bind_ip6_addr_prv( netif, &fnet_ip6_addr_any, FNET_NETIF_IP_ADDR_TYPE_AUTOCONFIGURABLE,
                                              FNET_NETIF_IP6_ADDR_LIFETIME_INFINITE /*in seconds*/, FNET_ND6_PREFIX_LENGTH_DEFAULT /* bits */ );

                /* RFC4862: The next phase of autoconfiguration involves obtaining a Router
                 * Advertisement or determining that no routers are present.  If routers
                 * are present, they will send Router Advertisements that specify what
                 * sort of autoconfiguration a host can do.
                 * To obtain an advertisement quickly, a host sends one or more Router
                 * Solicitations to the all-routers multicast group.
                 */
                fnet_nd6_rd_start(netif);
            }
#endif /* FNET_CFG_IP6 */

            /* Set connection flag. */
            eth_if->connection_flag = fnet_netif_is_connected(netif);

            eth_if->eth_timer = fnet_timer_new((FNET_ETH_TIMER_PERIOD / FNET_TIMER_PERIOD_MS), fnet_eth_timer, (fnet_uint32_t)netif);

            fnet_eth_number++;
        }
    }

    return result;
}

/************************************************************************
* NAME: fnet_eth_release
*
* DESCRIPTION: Do relaese for an Ethernet-type interface.
*************************************************************************/
void fnet_eth_release( fnet_netif_t *netif)
{

#if FNET_CFG_IP6

    fnet_ip6_multicast_leave_all(netif);

#if FNET_CFG_IP6_PMTU_DISCOVERY
    fnet_netif_pmtu_release(netif);
#endif

    fnet_nd6_release(netif);

#endif /* FNET_CFG_IP6 */

    fnet_timer_free(((fnet_eth_if_t *)(netif->if_ptr))->eth_timer);

#if FNET_CFG_IP4
    fnet_arp_release(netif);
#endif

    fnet_eth_number--;
}

/************************************************************************
* NAME: fnet_eth_drain
*
* DESCRIPTION: This function tries to free not critical parts
*              of memory in the Ethernet driver.
*************************************************************************/
void fnet_eth_drain(fnet_netif_t *netif)
{
#if FNET_CFG_IP4
    fnet_arp_drain(netif);
#else
    FNET_COMP_UNUSED_ARG(netif);
#endif /* FNET_CFG_IP4 */
}

/************************************************************************
* NAME: fnet_eth_change_addr_notify
*
* DESCRIPTION:  This function is called on IP address change.
*               It issues a gratuitous ARP request.
*************************************************************************/
void fnet_eth_change_addr_notify(fnet_netif_t *netif)
{
#if FNET_CFG_IP4
    if(netif->ip4_addr.address)
    {
        fnet_arp_send_request( (fnet_netif_desc_t) netif, netif->ip4_addr.address ); /* Gratuitous ARP request.*/
        /* When talking about gratuitous ARP, the packets are actually special ARP request packets,
           not ARP reply packets as one would perhaps expect. Some reasons for this are explained in RFC 5227.*/
    }
#else
    FNET_COMP_UNUSED_ARG(netif);
#endif /* FNET_CFG_IP4 */
}

/************************************************************************
* NAME: fnet_eth_timer
*
* DESCRIPTION:
*************************************************************************/
static void fnet_eth_timer(fnet_uint32_t cookie )
{
    fnet_netif_t    *netif = (fnet_netif_t *) cookie;
    fnet_eth_if_t   *eth_if = (fnet_eth_if_t *)(netif->if_ptr);
    fnet_bool_t     connection_flag = eth_if->connection_flag;

    if(fnet_netif_is_connected(netif) != connection_flag) /* Is any change in connection. */
    {
        if(connection_flag == FNET_FALSE)  /* Connected. */
        {
            fnet_eth_change_addr_notify(netif);
            eth_if->connection_flag = FNET_TRUE;
        }
        else
        {
            eth_if->connection_flag = FNET_FALSE;
        }
    }
}

/************************************************************************
* NAME: fnet_eth_ip4_output
*
* DESCRIPTION: Ethernet IPv4 output function.
*************************************************************************/
#if FNET_CFG_IP4
void fnet_eth_output_ip4(fnet_netif_t *netif, fnet_ip4_addr_t dest_ip_addr, fnet_netbuf_t *nb)
{
    fnet_mac_addr_t destination_addr; /* 48-bit destination address */

    /* Construct Ethernet header. Start with looking up deciding which
    * MAC address to use as a destination address. Broadcasts and
    * multicasts are special, all other addresses are looked up in the
    * ARP table. */
    if(fnet_ip_addr_is_broadcast (dest_ip_addr, netif))
    {
        fnet_memcpy (destination_addr, fnet_eth_broadcast, sizeof(fnet_mac_addr_t));
    }
    else if(FNET_IP4_ADDR_IS_MULTICAST(dest_ip_addr))
    {
        /* Hash IP multicast address to MAC address. */
        destination_addr[0] = 0x01U;
        destination_addr[1] = 0x0U;
        destination_addr[2] = 0x5eU;
        destination_addr[3] = (fnet_uint8_t)(FNET_IP4_ADDR2(dest_ip_addr) & 0x7fU);
        destination_addr[4] = (fnet_uint8_t)(FNET_IP4_ADDR3(dest_ip_addr));
        destination_addr[5] = (fnet_uint8_t)(FNET_IP4_ADDR4(dest_ip_addr));
        /* TBD Use macro. */
    }
    else
        /* Unicast address. */
    {
        if(fnet_arp_get_mac( (fnet_netif_desc_t) netif, dest_ip_addr, destination_addr) == FNET_FALSE)
        {
            fnet_arp_resolve(netif, dest_ip_addr, nb);
            goto EXIT;
        }

    }

    /* Send Ethernet frame. */
    fnet_eth_output(netif, FNET_ETH_TYPE_IP4, destination_addr, nb);
EXIT:
    return;
}
#endif /* FNET_CFG_IP4 */

#if FNET_CFG_IP6
/************************************************************************
* NAME: fnet_eth_output_ip6
*
* DESCRIPTION: Ethernet IPv6 output function.
*************************************************************************/
void fnet_eth_output_ip6(fnet_netif_t *netif, const fnet_ip6_addr_t *src_ip_addr,  const fnet_ip6_addr_t *dest_ip_addr, fnet_netbuf_t *nb)
{
    fnet_mac_addr_t dest_mac_addr; /* 48-bit destination address */
    fnet_uint8_t *dest_mac_addr_ptr;

    /********************************************
    * Multicast.
    ********************************************/
    if (FNET_IP6_ADDR_IS_MULTICAST(dest_ip_addr))
    {
        FNET_ETH_MULTICAST_IP6_TO_MAC(dest_ip_addr, dest_mac_addr);

        dest_mac_addr_ptr = (fnet_uint8_t *)dest_mac_addr;
    }
    /********************************************
    * Unicast.
    ********************************************/
    else
    {
        fnet_nd6_neighbor_entry_t *neighbor;

        /* Possible redirection.*/
        fnet_nd6_redirect_addr(netif, &dest_ip_addr);

        /* Check Neigbor cache.*/
        neighbor = fnet_nd6_neighbor_cache_get(netif, dest_ip_addr);

        /* RFC4861 7.2.2: When a node has a unicast packet to send to a neighbor, but does not
         * know the neighbor’s link-layer address, it performs address resolution.
         * For multicast-capable interfaces, this entails creating a
         * Neighbor Cache entry in the INCOMPLETE state and transmitting a
         * Neighbor Solicitation message targeted at the neighbor. The
         * solicitation is sent to the solicited-node multicast address
         * corresponding to the target address.
         */
        if(neighbor == FNET_NULL)
        {
            /* RFC4861 7.2.Address resolution is performed only on addresses that are determined to be
             * on-link and for which the sender does not know the corresponding link-layer address.
             * Address resolution is never performed on multicast addresses.*/

            if(fnet_nd6_addr_is_onlink(netif, dest_ip_addr) == FNET_TRUE)
                /* Destimnation is ON local-link.*/
            {
                /* Creating a Neighbor Cache entry in the INCOMPLETE state. */
                neighbor = fnet_nd6_neighbor_cache_add(netif, dest_ip_addr, FNET_NULL, FNET_ND6_NEIGHBOR_STATE_INCOMPLETE);

                neighbor->state_time = fnet_timer_ms();
                neighbor->solicitation_send_counter = 0u;
                FNET_IP6_ADDR_COPY(src_ip_addr, &neighbor->solicitation_src_ip_addr); /* Save src address for later usage.*/

                /* AR: Transmitting a Neighbor Solicitation message targeted at the neighbor.*/
                fnet_nd6_neighbor_solicitation_send(netif, src_ip_addr, FNET_NULL /* NULL for AR */, dest_ip_addr);
            }
            /* Destination is OFF local-link.*/
            else
            {
                /* Try to use the router, if exists.*/
                neighbor = fnet_nd6_default_router_get(netif);

                if(neighbor == FNET_NULL)
                    /* No Router exists.*/
                {
                    fnet_netbuf_free_chain(nb); /* Discard datagram */
                    goto EXIT;
                }

                dest_ip_addr = &neighbor->ip_addr;   /* Chage destination address to the router one */
            }
        }

        /* Link -layer address is not initialized.*/
        if((neighbor->state != FNET_ND6_NEIGHBOR_STATE_INCOMPLETE)
           && (neighbor->ll_addr[0] == 0U) && (neighbor->ll_addr[1] == 0U) && (neighbor->ll_addr[2] == 0U)
           && (neighbor->ll_addr[3] == 0U) && (neighbor->ll_addr[4] == 0U) && (neighbor->ll_addr[5] == 0U) )
        {
            neighbor->state = FNET_ND6_NEIGHBOR_STATE_INCOMPLETE;
            neighbor->state_time = fnet_timer_ms();
            neighbor->solicitation_send_counter = 0u;
            FNET_IP6_ADDR_COPY(src_ip_addr, &neighbor->solicitation_src_ip_addr); /* Save src address for later usage.*/
            /* AR: Transmitting a Neighbor Solicitation message targeted at the neighbor.*/
            fnet_nd6_neighbor_solicitation_send(netif, src_ip_addr, FNET_NULL /* NULL for AR */, dest_ip_addr);
        }

        if(neighbor->state == FNET_ND6_NEIGHBOR_STATE_INCOMPLETE)
            /* Queue packet for later transmit.
             */
        {
            fnet_nd6_neighbor_enqueue_waiting_netbuf(neighbor, nb);
            goto EXIT;
        }

        if(neighbor->state == FNET_ND6_NEIGHBOR_STATE_STALE)
            /* RFC4861 7.3.3: The first time a node sends a packet to a neighbor whose entry is
             * STALE, the sender changes the state to DELAY and sets a timer to
             * expire in DELAY_FIRST_PROBE_TIME seconds.
             */
        {
            neighbor->state = FNET_ND6_NEIGHBOR_STATE_DELAY;
            neighbor->state_time = fnet_timer_ms();
        }

        /* Get destination MAC/HW address.*/
        dest_mac_addr_ptr = (fnet_uint8_t *)(&neighbor->ll_addr[0]);
    }

    /* Send Ethernet frame. */
    fnet_eth_output(netif, FNET_ETH_TYPE_IP6, dest_mac_addr_ptr, nb);

EXIT:
    return;
}
#endif /* FNET_CFG_IP6 */


#if FNET_CFG_MULTICAST

#if FNET_CFG_IP4
/************************************************************************
* NAME: fnet_eth_multicast_leave_ip4
*
* DESCRIPTION: Leavess a multicast group on an Ethernet interface.
*************************************************************************/
void fnet_eth_multicast_leave_ip4(fnet_netif_t *netif, fnet_ip4_addr_t multicast_addr )
{
    fnet_mac_addr_t mac_addr;

    FNET_ETH_MULTICAST_IP4_TO_MAC(multicast_addr, mac_addr);

    ((fnet_eth_if_t *)(netif->if_ptr))->multicast_leave(netif, mac_addr);
}

/************************************************************************
* NAME: fnet_eth_multicast_join_ip4
*
* DESCRIPTION: Joins a multicast group on an Ethernet interface.
*************************************************************************/
void fnet_eth_multicast_join_ip4(fnet_netif_t *netif, fnet_ip4_addr_t  multicast_addr )
{
    fnet_mac_addr_t mac_addr;

    FNET_ETH_MULTICAST_IP4_TO_MAC(multicast_addr, mac_addr);

    ((fnet_eth_if_t *)(netif->if_ptr))->multicast_join(netif, mac_addr);
}
#endif /* FNET_CFG_IP4 */

#if FNET_CFG_IP6
/************************************************************************
* NAME: fnet_eth_multicast_leave_ip6
*
* DESCRIPTION: Leavess a multicast group on an Ethernet interface.
*************************************************************************/
void fnet_eth_multicast_leave_ip6(fnet_netif_t *netif, fnet_ip6_addr_t *multicast_addr )
{
    fnet_mac_addr_t mac_addr;

    FNET_ETH_MULTICAST_IP6_TO_MAC(multicast_addr, mac_addr);

    ((fnet_eth_if_t *)(netif->if_ptr))->multicast_leave(netif, mac_addr);
}

/************************************************************************
* NAME: fnet_eth_multicast_join_ip6
*
* DESCRIPTION: Joins a multicast group on an Ethernet interface.
*************************************************************************/
void fnet_eth_multicast_join_ip6(fnet_netif_t *netif, const fnet_ip6_addr_t  *multicast_addr )
{
    fnet_mac_addr_t mac_addr;

    FNET_ETH_MULTICAST_IP6_TO_MAC(multicast_addr, mac_addr);

    ((fnet_eth_if_t *)(netif->if_ptr))->multicast_join(netif, mac_addr);
}
#endif /* FNET_CFG_IP6 */

#endif /* FNET_CFG_MULTICAST */

/************************************************************************
* NAME: fnet_eth_trace
*
* DESCRIPTION: Prints an Ethernet header. For debug needs only.
*************************************************************************/
#if FNET_CFG_DEBUG_TRACE_ETH && FNET_CFG_DEBUG_TRACE
void fnet_eth_trace(fnet_uint8_t *str, fnet_eth_header_t *eth_hdr)
{
    fnet_uint8_t mac_str[FNET_MAC_ADDR_STR_SIZE];

    fnet_printf(FNET_SERIAL_ESC_FG_GREEN"%s", str); /* Print app-specific header.*/
    fnet_println("[ETH header]"FNET_SERIAL_ESC_FG_BLACK);
    fnet_println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+/\\/\\/\\/-+");
    fnet_println("|(Dest)                                                "FNET_SERIAL_ESC_FG_BLUE"%17s"FNET_SERIAL_ESC_FG_BLACK" |", fnet_mac_to_str(eth_hdr->destination_addr, mac_str));
    fnet_println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+/\\/\\/\\/-+");
    fnet_println("|(Src)                                                 "FNET_SERIAL_ESC_FG_BLUE"%17s"FNET_SERIAL_ESC_FG_BLACK" |", fnet_mac_to_str(eth_hdr->source_addr, mac_str));
    fnet_println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+/\\/\\/\\/-+");
    fnet_println("|(Type)                  0x%04x |", fnet_ntohs(eth_hdr->type));
    fnet_println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
}

#endif /* FNET_CFG_DEBUG_TRACE_ETH */

#endif /* FNET_CFG_ETH */


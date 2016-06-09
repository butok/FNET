/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
* Copyright 2003 by Andrey Butok. Motorola SPS.
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
* @file fnet_netif.c
*
* @author Andrey Butok
*
* @brief FNET Network interface implementation.
*
***************************************************************************/

#include "fnet.h"
#include "fnet_ip_prv.h"
#include "fnet_ip6_prv.h"
#include "fnet_netif_prv.h"
#include "fnet_eth_prv.h"
#include "fnet_loop.h"
#include "fnet_nd6.h"
#include "fnet_ip6_prv.h"


#if FNET_CFG_NETIF_IP6_ADDR_MAX < 2u
    #error "FNET_CFG_NETIF_IP6_ADDR_MAX must be > 1"
#endif

/************************************************************************
*     Global Data Structures
*************************************************************************/
#define FNET_NETIF_PMTU_TIMEOUT          (10u*60u*1000u)   /* ms. RFC1981: The recommended setting for this
                                                           * timer is twice its minimum value (10 minutes).*/
#define FNET_NETIF_PMTU_PERIOD           (FNET_NETIF_PMTU_TIMEOUT/10u)   /* ms. RFC1981: Once a minute.*/


fnet_netif_t *fnet_netif_list;           /* The list of network interfaces. */

static fnet_netif_t *fnet_netif_default; /* Default net_if. */

/* Duplicated IP event handler.*/
static fnet_netif_callback_ip4_addr_conflict_t         fnet_netif_callback_ip4_addr_conflict;


/************************************************************************
*     Function Prototypes
*************************************************************************/

static void fnet_netif_assign_scope_id( fnet_netif_t *netif );
#if FNET_CFG_IP6 && FNET_CFG_IP6_PMTU_DISCOVERY
    static void fnet_netif_pmtu_timer( fnet_uint32_t cookie);
#endif

/************************************************************************
* NAME: fnet_netif_init_all
*
* DESCRIPTION: Initialization of all supported interfaces.
*************************************************************************/
fnet_return_t fnet_netif_init_all( void )
{
    fnet_return_t result = FNET_OK;

    fnet_isr_lock();

    fnet_netif_list = fnet_netif_default = 0;

    /***********************************
     * Initialize IFs.
     ************************************/
#if FNET_CFG_CPU_ETH0
    /* Initialise eth0 interface.*/
    {
        fnet_mac_addr_t macaddr = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}; /* In case fnet_str_to_mac() will fail.*/

        /* Set MAC Address.*/
        fnet_str_to_mac(FNET_CFG_CPU_ETH0_MAC_ADDR, macaddr);
        result = fnet_netif_init((fnet_netif_t *)FNET_ETH0_IF, macaddr, sizeof(fnet_mac_addr_t));
        if(result == FNET_ERR)
        {
            goto INIT_ERR;
        }
    }
#endif
#if FNET_CFG_CPU_ETH1
    /* Initialise eth1 interface.*/
    {
        fnet_mac_addr_t macaddr = {0x00, 0x11, 0x22, 0x33, 0x33, 0x55}; /* In case fnet_str_to_mac() will fail.*/

        /* Set MAC Address.*/
        fnet_str_to_mac(FNET_CFG_CPU_ETH1_MAC_ADDR, macaddr);
        result = fnet_netif_init(FNET_ETH1_IF, macaddr, sizeof(fnet_mac_addr_t));
        if(result == FNET_ERR)
            goto INIT_ERR;
    }
#endif
#if FNET_CFG_LOOPBACK
    /* Initialise Loop-back interface.*/
    result = fnet_netif_init(FNET_LOOP_IF, FNET_NULL, 0u);
    if(result == FNET_ERR)
    {
        goto INIT_ERR;
    }
#endif /* FNET_CFG_LOOPBACK */

    /***********************************
     * Set default parameters.
     ************************************/
    fnet_netif_set_default(FNET_CFG_DEFAULT_IF); /* Default interface.*/

    /* Set address parameters of the Ethernet interface.*/
#if FNET_CFG_IP4
#if FNET_CFG_CPU_ETH0
    {
        fnet_netif_set_ip4_addr(FNET_ETH0_IF, FNET_CFG_ETH0_IP4_ADDR);
        fnet_netif_set_ip4_subnet_mask(FNET_ETH0_IF, FNET_CFG_ETH0_IP4_MASK);
        fnet_netif_set_ip4_gateway(FNET_ETH0_IF, FNET_CFG_ETH0_IP4_GW);
#if FNET_CFG_DNS
        fnet_netif_set_ip4_dns(FNET_ETH0_IF, FNET_CFG_ETH0_IP4_DNS);
#endif
    }
#endif /* FNET_CFG_CPU_ETH0 */
#if FNET_CFG_CPU_ETH1
    {
        fnet_netif_set_ip4_addr(FNET_ETH1_IF, FNET_CFG_ETH1_IP4_ADDR);
        fnet_netif_set_ip4_subnet_mask(FNET_ETH1_IF, FNET_CFG_ETH1_IP4_MASK);
        fnet_netif_set_ip4_gateway(FNET_ETH1_IF, FNET_CFG_ETH1_IP4_GW);
#if FNET_CFG_DNS
        fnet_netif_set_ip4_dns(FNET_ETH1_IF, FNET_CFG_ETH1_IP4_DNS);
#endif
    }
#endif /* FNET_CFG_CPU_ETH0 */
#endif /* FNET_CFG_ETH */

    /* Set address parameters of the Loopback interface.*/
#if FNET_CFG_LOOPBACK && FNET_CFG_IP4
    fnet_netif_set_ip4_addr(FNET_LOOP_IF, FNET_CFG_LOOPBACK_IP4_ADDR);
#endif /* FNET_CFG_LOOPBACK */
#if FNET_CFG_LOOPBACK && FNET_CFG_IP6
    fnet_netif_bind_ip6_addr(FNET_LOOP_IF, &fnet_ip6_addr_loopback, FNET_NETIF_IP_ADDR_TYPE_MANUAL);
#endif /* FNET_CFG_LOOPBACK */

INIT_ERR:
    fnet_isr_unlock();
    return result;
}

/************************************************************************
* NAME: fnet_netif_release_all
*
* DESCRIPTION: Releases all installed interfaces.
*************************************************************************/
void fnet_netif_release_all( void )
{
    fnet_netif_t *net_if_ptr;

    fnet_netif_set_callback_ip4_addr_conflict(0); /* Reset dupip handler.*/

    for (net_if_ptr = fnet_netif_list; net_if_ptr; net_if_ptr = net_if_ptr->next)
    {
        fnet_netif_release(net_if_ptr);
    }

    fnet_netif_list = fnet_netif_default = 0;
}

/************************************************************************
* NAME: fnet_netif_drain
*
* DESCRIPTION: This function calls "drain" functions of all currently
*              installed network interfaces.
*************************************************************************/
void fnet_netif_drain( void )
{
    fnet_netif_t *net_if_ptr;

    fnet_isr_lock();

    for (net_if_ptr = fnet_netif_list; net_if_ptr; net_if_ptr = net_if_ptr->next)
    {
        if(net_if_ptr->api->drain)
        {
            net_if_ptr->api->drain(net_if_ptr);
        }
    }

    fnet_isr_unlock();
}

/************************************************************************
* NAME: net_if_find
*
* DESCRIPTION: Returns a network interface given its name.
*************************************************************************/
fnet_netif_desc_t fnet_netif_get_by_name( fnet_char_t *name )
{
    fnet_netif_t        *netif;
    fnet_netif_desc_t   result = (fnet_netif_desc_t)FNET_NULL;

    fnet_os_mutex_lock();

    if(name)
    {
        for (netif = fnet_netif_list; netif != 0; netif = netif->next)
        {
            if(fnet_strncmp(name, netif->name, FNET_NETIF_NAMELEN) == 0)
            {
                result = (fnet_netif_desc_t)netif;
                break;
            }
        }
    }

    fnet_os_mutex_unlock();
    return result;
}

/************************************************************************
* NAME: fnet_netif_get_by_number
*
* DESCRIPTION: This function returns pointer to id-th interafce according
*              its index (from zero).
*              It returns NULL if id-th interface is not available.
*************************************************************************/
fnet_netif_desc_t fnet_netif_get_by_number( fnet_index_t n )
{
    fnet_netif_desc_t   result = FNET_NULL;
    fnet_netif_t        *current;

    for (current = fnet_netif_list; current; n--)
    {
        if(n == 0u)
        {
            result = current;
            break;
        }

        current = current->next;
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_get_by_ip4_addr
*
* DESCRIPTION: Returns a network interface given its IPv4 address.
*************************************************************************/
fnet_netif_desc_t fnet_netif_get_by_ip4_addr( fnet_ip4_addr_t addr )
{
    fnet_netif_desc_t result = (fnet_netif_desc_t)FNET_NULL;

#if FNET_CFG_IP4
    fnet_netif_t *netif;

    fnet_os_mutex_lock();
    for (netif = fnet_netif_list; netif != 0; netif = netif->next)
    {
        if(addr == netif->ip4_addr.address)
        {
            result = (fnet_netif_desc_t)netif;
            break;
        }
    }
    fnet_os_mutex_unlock();
#else
    FNET_COMP_UNUSED_ARG(addr);
#endif /* FNET_CFG_IP4 */

    return result;
}

/************************************************************************
* NAME: fnet_netif_get_by_sockaddr
*
* DESCRIPTION: Returns a network interface based on socket address.
*************************************************************************/
fnet_netif_desc_t fnet_netif_get_by_sockaddr( const struct sockaddr *addr )
{
    fnet_netif_desc_t result = (fnet_netif_desc_t)FNET_NULL;

    if(addr)
    {
        switch(addr->sa_family)
        {
#if FNET_CFG_IP4
            case AF_INET:
                result = fnet_netif_get_by_ip4_addr( ((const struct sockaddr_in *)addr)->sin_addr.s_addr);
                break;
#endif /* FNET_CFG_IP4 */
#if FNET_CFG_IP6
            case AF_INET6:
                result = fnet_netif_get_by_ip6_addr( &((const struct sockaddr_in6 *)addr)->sin6_addr.s6_addr);
                break;
#endif /* FNET_CFG_IP6 */
            default:
                break;
        }
    }

    return result;
}


#if FNET_CFG_IP6 && FNET_CFG_IP6_PMTU_DISCOVERY
/************************************************************************
* NAME: fnet_netif_set_pmtu
* RETURS: None.
* DESCRIPTION: Sets PMTU of the interface.
*************************************************************************/
void fnet_netif_set_pmtu(fnet_netif_t *netif, fnet_size_t pmtu)
{
    /* Set Path MTU for the link. */
    netif->pmtu = pmtu;

    netif->pmtu_timestamp = fnet_timer_ms();
}

/************************************************************************
* NAME: fnet_nd6_dad_timer
* RETURS: None.
* DESCRIPTION: Timer routine used to detect increase of PMTU
*************************************************************************/
static void fnet_netif_pmtu_timer( fnet_uint32_t cookie )
{
    fnet_netif_t    *netif = (fnet_netif_t *)cookie;

    if( fnet_timer_get_interval(netif->pmtu_timestamp, fnet_timer_ms()) > FNET_NETIF_PMTU_TIMEOUT)
    {
        fnet_netif_set_pmtu(netif, netif->mtu);
    }
}

/************************************************************************
* NAME: fnet_netif_pmtu_init
* RETURS: None.
* DESCRIPTION: Initialize PMTU Discovery for the interface.
*************************************************************************/
void fnet_netif_pmtu_init(fnet_netif_t *netif)
{
    /* Path MTU for the link. */
    fnet_netif_set_pmtu(netif, netif->mtu);

    /* Register timer, to detect increase of PMTU.*/
    netif->pmtu_timer = fnet_timer_new((FNET_NETIF_PMTU_PERIOD / FNET_TIMER_PERIOD_MS), fnet_netif_pmtu_timer, (fnet_uint32_t)netif);
}

/************************************************************************
* NAME: fnet_netif_pmtu_release
* RETURS: None.
* DESCRIPTION: Release/Disable PMTU Discovery for the interface.
*************************************************************************/
void fnet_netif_pmtu_release(fnet_netif_t *netif)
{
    fnet_timer_free(netif->pmtu_timer);

    netif->pmtu = 0u;
}

#endif /* FNET_CFG_IP6 && FNET_CFG_IP6_PMTU_DISCOVERY */

/************************************************************************
* NAME: fnet_netif_init
*
* DESCRIPTION: This function installs & inits a network interface.
*************************************************************************/
fnet_return_t fnet_netif_init( fnet_netif_t *netif, fnet_uint8_t *hw_addr, fnet_size_t hw_addr_size )
{
    fnet_return_t result = FNET_OK;

    if(netif && (netif->api))
    {
        fnet_os_mutex_lock();

        fnet_isr_lock();

        netif->next = fnet_netif_list;

        if(netif->next != 0)
        {
            netif->next->prev = netif;
        }

        netif->prev = 0;
        fnet_netif_list = netif;

        fnet_netif_assign_scope_id( netif ); /* Assign Scope ID.*/

        netif->features = FNET_NETIF_FEATURE_NONE;


        /* Interface HW initialization.*/
        if((netif->api->init) && ((result = netif->api->init(netif)) == FNET_OK))
        {
#if FNET_CFG_IGMP && FNET_CFG_IP4
            /**************************************************************
             * RFC1112 7.2
             *   To support IGMP, every level 2 host must
             *   join the "all-hosts" group (address 224.0.0.1) on each network
             *   interface at initialization time and must remain a member for as long
             *   as the host is active.
             *
             * NOTE:(224.0.0.1) membership is never reported by IGMP.
             **************************************************************/
            /* Join HW interface. */
            fnet_netif_join_ip4_multicast ( netif, FNET_IP4_ADDR_INIT(224, 0, 0, 1) );
#endif  /* FNET_CFG_IGMP */

            /* Set HW Address.*/
            fnet_netif_set_hw_addr(netif, hw_addr, hw_addr_size);

            /* Interface-Type specific initialisation. */
            switch(netif->api->type)
            {
#if (FNET_CFG_CPU_ETH0 ||FNET_CFG_CPU_ETH1)
                case (FNET_NETIF_TYPE_ETHERNET):
                    result = fnet_eth_init(netif);
                    break;
#endif /* FNET_CFG_ETH */
                default:
                    break;

            }
        }


        fnet_isr_unlock();

        fnet_os_mutex_unlock();
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_release
*
* DESCRIPTION: This function releases a network interface.
*************************************************************************/
void fnet_netif_release( fnet_netif_t *netif )
{
    if(netif)
    {
        if(netif->api->release)
        {
            netif->api->release(netif);
        }

        fnet_os_mutex_lock();

        if(netif->prev == 0)
        {
            fnet_netif_list = netif->next;
        }
        else
        {
            netif->prev->next = netif->next;
        }

        if(netif->next != 0)
        {
            netif->next->prev = netif->prev;
        }

        fnet_os_mutex_unlock();
    }
}

/************************************************************************
* NAME: fnet_netif_set_default
*
* DESCRIPTION: This function sets the default network interface.
*************************************************************************/
void fnet_netif_set_default( fnet_netif_desc_t netif_desc )
{
    if(netif_desc)
    {
        fnet_os_mutex_lock();
        fnet_netif_default = (fnet_netif_t *)netif_desc;
        fnet_os_mutex_unlock();
    }
}

/************************************************************************
* NAME: fnet_netif_set_ip4_addr
*
* DESCRIPTION: This function sets the IPv4 address.
*************************************************************************/
#if FNET_CFG_IP4
void fnet_netif_set_ip4_addr( fnet_netif_desc_t netif_desc, fnet_ip4_addr_t ipaddr )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    fnet_os_mutex_lock();

    if(netif_desc)
    {
        fnet_isr_lock();

        netif->ip4_addr.address = ipaddr; /* IP address */
        netif->ip4_addr.address_type = FNET_NETIF_IP_ADDR_TYPE_MANUAL; /* Adress is set manually. */
        netif->ip4_addr_conflict = FNET_FALSE; /* Clear IPv4 address duplication flag.*/

        if(FNET_IP4_CLASS_A(netif->ip4_addr.address))
        {
            if(netif->ip4_addr.subnetmask == 0u)
            {
                netif->ip4_addr.subnetmask = FNET_IP4_CLASS_A_NET;
            }

            netif->ip4_addr.netmask = FNET_IP4_CLASS_A_NET;
        }
        else
        {
            if(FNET_IP4_CLASS_B(netif->ip4_addr.address))
            {
                if(netif->ip4_addr.subnetmask == 0u)
                {
                    netif->ip4_addr.subnetmask = FNET_IP4_CLASS_B_NET;
                }

                netif->ip4_addr.netmask = FNET_IP4_CLASS_B_NET;
            }
            else
            {
                if(FNET_IP4_CLASS_C(netif->ip4_addr.address))
                {
                    if(netif->ip4_addr.subnetmask == 0u)
                    {
                        netif->ip4_addr.subnetmask = FNET_IP4_CLASS_C_NET;
                    }

                    netif->ip4_addr.netmask = FNET_IP4_CLASS_C_NET;
                }
                /* else: Is not supported */
            }
        }

        netif->ip4_addr.net = netif->ip4_addr.address & netif->ip4_addr.netmask;             /* Network address.*/
        netif->ip4_addr.subnet = netif->ip4_addr.address & netif->ip4_addr.subnetmask;       /* Network and subnet address.*/

        netif->ip4_addr.netbroadcast = netif->ip4_addr.address | (~netif->ip4_addr.netmask); /* Network broadcast address.*/
        netif->ip4_addr.subnetbroadcast = netif->ip4_addr.address
                                          | (~netif->ip4_addr.subnetmask);                   /* Subnet broadcast address.*/

        if(netif->api->set_addr_notify)
        {
            netif->api->set_addr_notify(netif);
        }

        fnet_isr_unlock();
    }

    fnet_os_mutex_unlock();
}
#endif /* FNET_CFG_IP4 */

/************************************************************************
* NAME: fnet_netif_set_ip4_subnet_mask
*
* DESCRIPTION: This function sets the subnet mask.
*************************************************************************/
#if FNET_CFG_IP4
void fnet_netif_set_ip4_subnet_mask( fnet_netif_desc_t netif_desc, fnet_ip4_addr_t subnet_mask )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    if(netif)
    {
        fnet_os_mutex_lock();

        netif->ip4_addr.subnetmask = subnet_mask;
        netif->ip4_addr.address_type = FNET_NETIF_IP_ADDR_TYPE_MANUAL;

        netif->ip4_addr.subnet = netif->ip4_addr.address & netif->ip4_addr.subnetmask; /* network and subnet address*/
        netif->ip4_addr.subnetbroadcast = netif->ip4_addr.address
                                          | (~netif->ip4_addr.subnetmask);     /* subnet broadcast address*/
        fnet_os_mutex_unlock();
    }
}
#endif /* FNET_CFG_IP4 */

/************************************************************************
* NAME: fnet_netif_set_ip4_gateway
*
* DESCRIPTION: This function sets the gateway IP address.
*************************************************************************/
#if FNET_CFG_IP4
void fnet_netif_set_ip4_gateway( fnet_netif_desc_t netif_desc, fnet_ip4_addr_t gw )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    if(netif)
    {
        fnet_os_mutex_lock();
        netif->ip4_addr.gateway = gw;
        netif->ip4_addr.address_type = FNET_NETIF_IP_ADDR_TYPE_MANUAL;
        fnet_os_mutex_unlock();
    }
}
#endif /* FNET_CFG_IP4 */

#if FNET_CFG_DNS && FNET_CFG_IP4
/************************************************************************
* NAME: fnet_netif_set_ip4_dns
*
* DESCRIPTION: This function sets the DNS IP address.
*************************************************************************/
void fnet_netif_set_ip4_dns( fnet_netif_desc_t netif_desc, fnet_ip4_addr_t dns )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    if(netif)
    {
        fnet_os_mutex_lock();
        netif->ip4_addr.dns = dns;
        netif->ip4_addr.address_type = FNET_NETIF_IP_ADDR_TYPE_MANUAL;
        fnet_os_mutex_unlock();
    }
}
#endif /* FNET_CFG_DNS && FNET_CFG_IP4*/

/************************************************************************
* NAME: fnet_netif_get_default
*
* DESCRIPTION: This function gets the default network interface.
*************************************************************************/
fnet_netif_desc_t fnet_netif_get_default( void )
{
    return (fnet_netif_desc_t)fnet_netif_default;
}

/************************************************************************
* NAME: fnet_netif_get_ip4_addr
*
* DESCRIPTION: This function returns the IP address of the net interface.
*************************************************************************/
#if FNET_CFG_IP4
fnet_ip4_addr_t fnet_netif_get_ip4_addr( fnet_netif_desc_t netif_desc )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    return netif ? (netif->ip4_addr.address) : 0u;
}
#endif /* FNET_CFG_IP4 */

/************************************************************************
* NAME: fnet_netif_get_ip4_subnet_mask
*
* DESCRIPTION: This function returns the netmask of the net interface.
*************************************************************************/
#if FNET_CFG_IP4
fnet_ip4_addr_t fnet_netif_get_ip4_subnet_mask( fnet_netif_desc_t netif_desc )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    return netif ? (netif->ip4_addr.subnetmask) : 0u;
}
#endif /* FNET_CFG_IP4 */

/************************************************************************
* NAME: fnet_netif_get_ip4_gateway
*
* DESCRIPTION: This function returns the gateway IP address of
*              the net interface.
*************************************************************************/
#if FNET_CFG_IP4
fnet_ip4_addr_t fnet_netif_get_ip4_gateway( fnet_netif_desc_t netif_desc )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    return netif ? (netif->ip4_addr.gateway) : 0u;
}
#endif /* FNET_CFG_IP4 */

#if FNET_CFG_DNS && FNET_CFG_IP4
/************************************************************************
* NAME: fnet_netif_get_ip4_dns
*
* DESCRIPTION: This function returns the DNS address of
*              the net interface.
*************************************************************************/
fnet_ip4_addr_t fnet_netif_get_ip4_dns( fnet_netif_desc_t netif_desc )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    return netif ? (netif->ip4_addr.dns) : 0u;
}
#endif /* FNET_CFG_DNS && FNET_CFG_IP4*/

#if FNET_CFG_DNS && FNET_CFG_IP6
/************************************************************************
* NAME: fnet_netif_get_ip6_dns
*
* DESCRIPTION: This function returns a DNSv6 Server address.
*************************************************************************/
fnet_bool_t fnet_netif_get_ip6_dns( fnet_netif_desc_t netif_desc, fnet_index_t n, fnet_ip6_addr_t *addr_dns )
{
    fnet_netif_t    *netif = (fnet_netif_t *)netif_desc;
    fnet_bool_t     result = FNET_FALSE;

    if(netif && addr_dns)
    {
        /* TBD Manual configuration.*/

#if FNET_CFG_ND6_RDNSS
        result =  fnet_nd6_rdnss_get_addr(netif, n, addr_dns);
#endif
    }

    return result;
}
#endif /* FNET_CFG_DNS && FNET_CFG_IP6*/

#if FNET_CFG_IP6
/************************************************************************
* NAME: fnet_netif_get_ip6_prefix
*
* DESCRIPTION: This function returns an entry from IPv6 ND prefix list.
*************************************************************************/
fnet_bool_t fnet_netif_get_ip6_prefix( fnet_netif_desc_t netif_desc, fnet_index_t n, fnet_netif_ip6_prefix_t *ip6_prefix )
{
    fnet_netif_t        *netif = (fnet_netif_t *)netif_desc;
    fnet_bool_t         result = FNET_FALSE;
    struct fnet_nd6_if  *nd6_if;
    fnet_index_t        i;

    if(netif && ip6_prefix)
    {
        nd6_if = netif->nd6_if_ptr;

        if(nd6_if )
        {
            for(i = 0u; i < FNET_ND6_PREFIX_LIST_SIZE; i++)
            {
                /* Skip NOT_USED prefixes. */
                if(nd6_if->prefix_list[i].state != FNET_ND6_PREFIX_STATE_NOTUSED)
                {
                    if(n == 0u)
                    {
                        FNET_IP6_ADDR_COPY(&nd6_if->prefix_list[i].prefix, &ip6_prefix->prefix);
                        ip6_prefix->prefix_length = nd6_if->prefix_list[i].prefix_length;

                        result = FNET_TRUE;
                        break;
                    }
                    n--;
                }
            }
        }
    }

    return result;
}
#endif /* FNET_CFG_IP6*/

#if FNET_CFG_IP6
/************************************************************************
* NAME: fnet_netif_get_ip6_neighbor_cache
*
* DESCRIPTION: This function returns an entry from IPv6 neighbor cache.
*************************************************************************/
fnet_bool_t fnet_netif_get_ip6_neighbor_cache(fnet_netif_desc_t netif_desc, fnet_index_t n, fnet_netif_ip6_neighbor_cache_t *ip6_neighbor_cache )
{
    fnet_netif_t        *netif = (fnet_netif_t *)netif_desc;
    fnet_bool_t         result = FNET_FALSE;
    struct fnet_nd6_if  *nd6_if;
    fnet_index_t        i;

    if(netif && ip6_neighbor_cache)
    {
        nd6_if = netif->nd6_if_ptr;

        if(nd6_if)
        {
            for(i = 0u; i < FNET_ND6_NEIGHBOR_CACHE_SIZE; i++)
            {
                /* Skip NOT_USED entries. */
                if(nd6_if->neighbor_cache[i].state != FNET_ND6_NEIGHBOR_STATE_NOTUSED)
                {
                    if(n == 0u)
                    {
                        FNET_IP6_ADDR_COPY(&nd6_if->neighbor_cache[i].ip_addr, &ip6_neighbor_cache->ip_addr);
                        FNET_NETIF_LL_ADDR_COPY(nd6_if->neighbor_cache[i].ll_addr, ip6_neighbor_cache->ll_addr, netif->api->hw_addr_size);
                        ip6_neighbor_cache->ll_addr_size = netif->api->hw_addr_size;
                        ip6_neighbor_cache->is_router = nd6_if->neighbor_cache[i].is_router;

                        result = FNET_TRUE;
                        break;
                    }
                    n--;
                }
            }
        }
    }

    return result;
}
#endif /* FNET_CFG_IP6*/



/************************************************************************
* NAME: fnet_netif_get_name
*
* DESCRIPTION: This function returns network interface name (e.g. "eth0", "loop").
*************************************************************************/
void fnet_netif_get_name( fnet_netif_desc_t netif_desc, fnet_char_t *name, fnet_size_t name_size )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    if(netif && name)
    {
        fnet_strncpy(name, netif->name, name_size);
    }
}

/************************************************************************
* NAME: fnet_netif_get_ip4_addr_type
*
* DESCRIPTION: This function determines if the IPv4 parameters of the @c netif interface
*           were set manually, or obtained by the DHCP client or
*           set during link-local autoconfiguartion (AutoIP - TBD)
*************************************************************************/
#if FNET_CFG_IP4
fnet_netif_ip_addr_type_t fnet_netif_get_ip4_addr_type( fnet_netif_desc_t netif_desc )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;
    return netif ? (netif->ip4_addr.address_type) : FNET_NETIF_IP_ADDR_TYPE_MANUAL;
}
#endif

/***************************************************************************/ /*!
 *
 * @brief    Sets the way IPv4 address parameters were obtained.
 *
 * @param netif_desc  Network interface descriptor.
 *
 * @see fnet_netif_get_ip4_addr_type()
 *
 ******************************************************************************
 *
 * This function sets type of the interface IPv4 address parameters, if they
 * were set manually, or obtained by the DHCP client or
 * set during link-local autoconfiguartion (AutoIP - TBD). @n
 * fnet_netif_set_ip4_addr() sets the type to FNET_NETIF_IP_ADDR_TYPE_MANUAL automatically.
 *
 ******************************************************************************/
void fnet_netif_set_ip4_addr_type( fnet_netif_desc_t netif_desc, fnet_netif_ip_addr_type_t ipaddr_type );

/************************************************************************
* NAME: fnet_netif_set_ip4_addr_type
*
* DESCRIPTION: This function sets type of the interface IPv4 address parameters, if they
*       were set manually, or obtained by the DHCP client or
*       set during link-local autoconfiguartion (AutoIP - TBD). @n
*       fnet_netif_set_ip4_addr() sets the type to FNET_NETIF_IP_ADDR_TYPE_MANUAL automatically.
*************************************************************************/
#if FNET_CFG_IP4
void fnet_netif_set_ip4_addr_type( fnet_netif_desc_t netif_desc, fnet_netif_ip_addr_type_t ipaddr_type )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    if(netif)
    {
        netif->ip4_addr.address_type = ipaddr_type;
    }
}
#endif /* FNET_CFG_IP4 */

/************************************************************************
* NAME: fnet_netif_get_hw_addr
*
* DESCRIPTION: This function reads HW interface address.
*              (MAC address in case of Ethernet interface)
*************************************************************************/
fnet_return_t fnet_netif_get_hw_addr( fnet_netif_desc_t netif_desc, fnet_uint8_t *hw_addr, fnet_size_t hw_addr_size )
{
    fnet_return_t result;
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    fnet_os_mutex_lock();

    if(netif && hw_addr && hw_addr_size && (netif->api)
       && (hw_addr_size >= netif->api->hw_addr_size)
       && (netif->api->get_hw_addr))
    {
        result = netif->api->get_hw_addr(netif, hw_addr);
    }
    else
    {
        result = FNET_ERR;
    }

    fnet_os_mutex_unlock();

    return result;
}

/************************************************************************
* NAME: fnet_netif_set_hw_addr
*
* DESCRIPTION: This function sets HW interface address.
*              (MAC address in case of Ethernet interface)
*************************************************************************/
fnet_return_t fnet_netif_set_hw_addr( fnet_netif_desc_t netif_desc, fnet_uint8_t *hw_addr, fnet_size_t hw_addr_size )
{
    fnet_return_t result;
    fnet_netif_t  *netif = (fnet_netif_t *)netif_desc;

    /* Seed pseudo-random generator */
    if(hw_addr_size >= sizeof(fnet_uint32_t))
    {
        fnet_srand(((fnet_uint32_t)hw_addr[0] << 24u) | ((fnet_uint32_t)hw_addr[1] << 16u) | ((fnet_uint32_t)hw_addr[2] << 8u) | hw_addr[3]);
    }

    fnet_os_mutex_lock();

    if(netif && hw_addr
       && (netif->api)
       && (hw_addr_size == netif->api->hw_addr_size)
       && (netif->api->set_hw_addr))
    {

        result = netif->api->set_hw_addr(netif, hw_addr);
    }
    else
    {
        result = FNET_ERR;
    }

    fnet_os_mutex_unlock();

    return result;
}

/************************************************************************
* NAME: fnet_netif_join_ip4_multicast
*
* DESCRIPTION: This function configures the HW interface to receive
* particular multicast MAC addresses.
* When the network interface picks up a packet which has a destination MAC
* that matches any of the multicast MAC addresses, it will pass it to
* the upper layers for further processing.
*************************************************************************/
#if FNET_CFG_MULTICAST & FNET_CFG_IP4
void fnet_netif_join_ip4_multicast ( fnet_netif_desc_t netif_desc, fnet_ip4_addr_t multicast_addr )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    fnet_os_mutex_lock();

    if(netif && (netif->api->multicast_join_ip4))
    {
        netif->api->multicast_join_ip4(netif, multicast_addr);
    }

    fnet_os_mutex_unlock();
}
#endif /* FNET_CFG_MULTICAST & FNET_CFG_IP4 */
/************************************************************************
* NAME: fnet_netif_leave_ip4_multicast
*
* DESCRIPTION:
*************************************************************************/
#if FNET_CFG_MULTICAST & FNET_CFG_IP4
void fnet_netif_leave_ip4_multicast ( fnet_netif_desc_t netif_desc, fnet_ip4_addr_t multicast_addr )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    fnet_os_mutex_lock();

    if(netif && (netif->api->multicast_leave_ip4))
    {
        netif->api->multicast_leave_ip4(netif, multicast_addr);
    }

    fnet_os_mutex_unlock();
}
#endif /* FNET_CFG_MULTICAST & FNET_CFG_IP4*/

/************************************************************************
* NAME: fnet_netif_get_type
*
* DESCRIPTION: This function returns type of the network interface.
*************************************************************************/
fnet_netif_type_t fnet_netif_get_type( fnet_netif_desc_t netif_desc )
{
    fnet_netif_type_t result;
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    if(netif)
    {
        result = netif->api->type;
    }
    else
    {
        result = FNET_NETIF_TYPE_OTHER;
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_is_connected
*
* DESCRIPTION: This function gets physical link status.
*************************************************************************/
fnet_bool_t fnet_netif_is_connected( fnet_netif_desc_t netif_desc )
{
    fnet_bool_t     result;
    fnet_netif_t    *netif = (fnet_netif_t *)netif_desc;

    if(netif && (netif->api->is_connected))
    {
        result = netif->api->is_connected(netif);
    }
    else
    {
        result = FNET_TRUE; /* Is connected by default.*/
    }

    return result;
}

/************************************************************************
* NAME: fnet_get_statistics
*
* DESCRIPTION: This function returns network interface statistics.
*************************************************************************/
fnet_return_t fnet_netif_get_statistics( fnet_netif_desc_t netif_desc, struct fnet_netif_statistics *statistics )
{
    fnet_return_t   result;
    fnet_netif_t    *netif = (fnet_netif_t *)netif_desc;

    if(netif && statistics && (netif->api->get_statistics))
    {
        result = netif->api->get_statistics(netif, statistics);
    }
    else
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_set_callback_ip4_addr_conflict
*
* DESCRIPTION: Registers the "duplicated IP address" event handler.
************************************************************************/
void fnet_netif_set_callback_ip4_addr_conflict(fnet_netif_callback_ip4_addr_conflict_t callback)
{
    fnet_netif_callback_ip4_addr_conflict = callback;
}

/************************************************************************
* NAME: fnet_netif_is_ip4_addr_conflict
*
* DESCRIPTION: This function detects if there is IPv4 adress conflict on network.
*************************************************************************/
fnet_bool_t fnet_netif_is_ip4_addr_conflict( fnet_netif_desc_t netif_desc )
{
    fnet_bool_t     result = FNET_FALSE;
    fnet_netif_t    *netif = (fnet_netif_t *)netif_desc;

    if(netif)
    {
        result = netif->ip4_addr_conflict;
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_dupip_handler_signal
*
* DESCRIPTION:
************************************************************************/
void fnet_netif_signal_p4_addr_conflict(fnet_netif_desc_t netif )
{
    if(fnet_netif_callback_ip4_addr_conflict)
    {
        fnet_netif_callback_ip4_addr_conflict(netif);
    }
}

/************************************************************************
********************   IP6 Netif API ************************************
*************************************************************************/

/************************************************************************
* NAME: fnet_netif_assign_scope_id
*
* DESCRIPTION: This function assign unique Scope ID to the interface.
*************************************************************************/
static void fnet_netif_assign_scope_id( fnet_netif_t *netif )
{
    fnet_scope_id_t scope_id;
    fnet_netif_t    *current;
    fnet_bool_t     try_again;

    scope_id = 1u;

    do
    {
        try_again = FNET_FALSE;

        for (current = fnet_netif_list; current; current = current->next)
        {
            if(scope_id == current->scope_id)
            {
                try_again = FNET_TRUE;
                scope_id++;
                break;
            }
        }
    }
    while(try_again == FNET_TRUE);

    netif->scope_id = scope_id;
}

/************************************************************************
* NAME: fnet_netif_get_scope_id
*
* DESCRIPTION: Gets Scope ID assigned to the interface.
*************************************************************************/
fnet_scope_id_t fnet_netif_get_scope_id(fnet_netif_desc_t netif_desc)
{
    fnet_scope_id_t result;
    fnet_netif_t    *netif = (fnet_netif_t *)netif_desc;

    if(netif)
    {
        result = netif->scope_id;
    }
    else
    {
        result = 0u;
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_get_mtu
*
* DESCRIPTION: Gets Maximum Transmission Unit (MTU) of the interface.
*************************************************************************/
fnet_size_t fnet_netif_get_mtu(fnet_netif_desc_t netif_desc)
{
    fnet_size_t     mtu;
    fnet_netif_t    *netif = (fnet_netif_t *)netif_desc;

    if(netif)
    {
        mtu = netif->mtu;
    }
    else
    {
        mtu = 0u;
    }

    return mtu;
}

/************************************************************************
* NAME: fnet_netif_get_by_scope_id
*
* DESCRIPTION: This function returns interafce decriptor according
*              its Scope ID.
*              It returns FNET_NULL if the interface with passed Scope ID
*              is not available.
*************************************************************************/
fnet_netif_desc_t fnet_netif_get_by_scope_id( fnet_scope_id_t scope_id )
{
    fnet_netif_desc_t   result = FNET_NULL;
    fnet_netif_t        *current;

    if(scope_id)
    {
        for (current = fnet_netif_list; current; current = current->next)
        {
            if(current->scope_id == scope_id)
            {
                result = (fnet_netif_desc_t)current;
                break;
            }
        }
    }

    return result;
}



#if FNET_CFG_IP6

/************************************************************************
* NAME: fnet_netif_get_ip6_addr
*
* DESCRIPTION: This function is used to retrieve all IP addresses registerred
*              with the given interface.
*              Returns FNET_TRUE if successful and data structure filled
*              and FNET_FALSE in case of error.
*              It returns FNET_FALSE if n-th address is not available.
*************************************************************************/
fnet_bool_t fnet_netif_get_ip6_addr (fnet_netif_desc_t netif_desc, fnet_index_t n, fnet_netif_ip6_addr_info_t *addr_info)
{
    fnet_bool_t     result = FNET_FALSE;
    fnet_index_t    i;
    fnet_netif_t    *netif = (fnet_netif_t *)netif_desc;

    if(netif && addr_info)
    {
        for(i = 0u; i < FNET_NETIF_IP6_ADDR_MAX; i++)
        {
            /* Skip NOT_USED addresses. */
            if(netif->ip6_addr[i].state != FNET_NETIF_IP6_ADDR_STATE_NOT_USED)
            {
                if(n == 0u)
                {
                    FNET_IP6_ADDR_COPY(&netif->ip6_addr[i].address, &addr_info->address);   /* IPv6 address.*/
                    addr_info->state = netif->ip6_addr[i].state;                            /* Address current state.*/
                    addr_info->type = netif->ip6_addr[i].type;                              /* How the address was acquired.*/

                    result = FNET_TRUE;
                    break;
                }
                n--;
            }
        }
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_join_ip6_multicast
*
* DESCRIPTION:
*************************************************************************/
void fnet_netif_join_ip6_multicast ( fnet_netif_desc_t netif_desc, const fnet_ip6_addr_t *multicast_addr )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    fnet_os_mutex_lock();

    if(netif && (netif->api->multicast_join_ip6))
    {
        netif->api->multicast_join_ip6(netif, multicast_addr);
    }

    fnet_os_mutex_unlock();
}

/************************************************************************
* NAME: fnet_netif_leave_ip6_multicast
*
* DESCRIPTION:
*************************************************************************/
void fnet_netif_leave_ip6_multicast ( fnet_netif_desc_t netif_desc, fnet_ip6_addr_t *multicast_addr )
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    fnet_os_mutex_lock();

    if(netif && (netif->api->multicast_leave_ip6))
    {
        netif->api->multicast_leave_ip6(netif, multicast_addr);
    }

    fnet_os_mutex_unlock();
}

/************************************************************************
* NAME: fnet_netif_get_ip6_addr_info
*
* DESCRIPTION: Gets address information structure that corresponds to the ip_addr.
*************************************************************************/
fnet_netif_ip6_addr_t *fnet_netif_get_ip6_addr_info(fnet_netif_t *netif, const fnet_ip6_addr_t *ip_addr)
{
    fnet_index_t            i;
    fnet_netif_ip6_addr_t   *result = FNET_NULL;

    if(netif && ip_addr)
    {
        for(i = 0u; i < FNET_NETIF_IP6_ADDR_MAX; i++)
        {
            /* Skip NOT_USED addresses. */
            if((netif->ip6_addr[i].state != FNET_NETIF_IP6_ADDR_STATE_NOT_USED)
               && FNET_IP6_ADDR_EQUAL(ip_addr, &netif->ip6_addr[i].address))
            {
                result = &netif->ip6_addr[i];
                break;
            }
        }
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_get_ip6_addr_valid_link_local
*
* DESCRIPTION: This function returns valid link-local address,
*  if there is no vlaid address(DAD is in progress) it returns FNET_NULL.
*************************************************************************/
fnet_ip6_addr_t *fnet_netif_get_ip6_addr_valid_link_local (fnet_netif_t *netif)
{

    fnet_ip6_addr_t     *result = FNET_NULL;
    fnet_index_t        i;

    if(netif)
    {
        for(i = 0u; i < FNET_NETIF_IP6_ADDR_MAX; i++)
        {
            /* Skip NOT_USED and TENTATIVE addresses. */
            if((netif->ip6_addr[i].state != FNET_NETIF_IP6_ADDR_STATE_NOT_USED)
               && (netif->ip6_addr[i].state != FNET_NETIF_IP6_ADDR_STATE_TENTATIVE)
               && FNET_IP6_ADDR_IS_LINKLOCAL(&netif->ip6_addr[i].address))
            {
                result = &netif->ip6_addr[i].address;
                break;
            }
        }
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_is_my_ip6_addr
*
* DESCRIPTION: Checks if an unicast address is attached/bound to the interface.
*              Returns FNET_TRUE if the address is attached/bound, otherwise FNET_FALSE.
*************************************************************************/
fnet_bool_t fnet_netif_is_my_ip6_addr(fnet_netif_t *netif, const fnet_ip6_addr_t *ip_addr)
{
    fnet_bool_t     result;

    if(fnet_netif_get_ip6_addr_info(netif, ip_addr) != FNET_NULL)
    {
        result = FNET_TRUE;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_is_my_ip6_solicited_multicast_addr
*
* DESCRIPTION: Checks if a solicited multicast address is attached/bound
*              to the interface.
*              Returns FNET_TRUE if the solicited multicast address
*              is attached/bound, otherwise FNET_FALSE.
*************************************************************************/
fnet_bool_t fnet_netif_is_my_ip6_solicited_multicast_addr(fnet_netif_t *netif, fnet_ip6_addr_t *ip_addr)
{
    fnet_index_t    i;
    fnet_bool_t     result = FNET_FALSE;

    if(netif && ip_addr)
    {
        for(i = 0u; i < FNET_NETIF_IP6_ADDR_MAX; i++)
        {
            /* Skip NOT_USED addresses. */
            if((netif->ip6_addr[i].state  != FNET_NETIF_IP6_ADDR_STATE_NOT_USED) &&
               FNET_IP6_ADDR_EQUAL(ip_addr, &netif->ip6_addr[i].solicited_multicast_addr))
            {
                result = FNET_TRUE;
                break;
            }
        }
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_get_by_ip6_addr
*
* DESCRIPTION: Returns a network interface given its IPv6 address.
*************************************************************************/
fnet_netif_desc_t fnet_netif_get_by_ip6_addr(const fnet_ip6_addr_t *ip_addr )
{
    fnet_netif_t        *netif;
    fnet_netif_desc_t   result = (fnet_netif_desc_t)FNET_NULL;

    fnet_os_mutex_lock();

    /* If the source address is explicitly specified by the user the
     * specified address is used.*/
    if(ip_addr)
    {
        for (netif = fnet_netif_list; netif != 0; netif = netif->next)
        {
            if(fnet_netif_is_my_ip6_addr(netif, ip_addr) == FNET_TRUE)
            {
                result = (fnet_netif_desc_t)netif;
                break;
            }
        }
    }

    fnet_os_mutex_unlock();
    return result;
}

/************************************************************************
* NAME: fnet_netif_ip6_addr_autoconf_set
*
* DESCRIPTION: Overwrite the last 64 bits with the interface ID.
*              "addr" contains prefix to form an address
*************************************************************************/
fnet_return_t fnet_netif_set_ip6_addr_autoconf(fnet_netif_t *netif, fnet_ip6_addr_t *ip_addr)
{
    fnet_return_t result;
    fnet_uint8_t hw_addr[8];

    result = fnet_netif_get_hw_addr(netif, hw_addr, 8u );

    if(result == FNET_OK)
    {
        /* Build Interface identifier.*/
        /* Set the 8 last bytes of the IP address based on the Layer 2 identifier.*/
        switch(netif->api->hw_addr_size)
        {
            case 6: /* IEEE 48-bit MAC addresses. */
                fnet_memcpy(&(ip_addr->addr[8]), hw_addr,  3u);
                ip_addr->addr[11] = 0xffu;
                ip_addr->addr[12] = 0xfeu;
                fnet_memcpy(&(ip_addr->addr[13]), &hw_addr[3],  3u);
                ip_addr->addr[8] ^= 0x02u;
                break;
            case 8: /* IEEE EUI-64 identifier.*/
                fnet_memcpy(&(ip_addr->addr[8]), hw_addr,  8u);
                ip_addr->addr[8] ^= 0x02u;
                break;
            /* TBD for others.*/
            default:
                result = FNET_ERR;
                break;
        }
    }

    return result;
}

/************************************************************************
* NAME: fnet_netif_bind_ip6_addr
*
* DESCRIPTION: This is USER API function binds the IPv6 address to a hardware interface.
*************************************************************************/
fnet_return_t fnet_netif_bind_ip6_addr(fnet_netif_desc_t netif_desc, const fnet_ip6_addr_t *addr, fnet_netif_ip_addr_type_t addr_type)
{
    fnet_netif_t *netif = (fnet_netif_t *)netif_desc;

    return fnet_netif_bind_ip6_addr_prv(netif, addr, addr_type,
                                        FNET_NETIF_IP6_ADDR_LIFETIME_INFINITE,
                                        FNET_ND6_PREFIX_LENGTH_DEFAULT);
}

/************************************************************************
* NAME: fnet_netif_bind_ip6_addr_prv
*
* DESCRIPTION: This function binds the IPv6 address to a hardware interface.
*************************************************************************/
fnet_return_t fnet_netif_bind_ip6_addr_prv(fnet_netif_t *netif, const fnet_ip6_addr_t *addr, fnet_netif_ip_addr_type_t addr_type,
        fnet_time_t lifetime /*in seconds*/, fnet_size_t prefix_length /* bits */ )
{
    fnet_return_t           result = FNET_ERR;
    fnet_netif_ip6_addr_t   *if_addr_ptr = FNET_NULL;
    fnet_index_t            i;

    fnet_os_mutex_lock();

    /* Check input parameters. */
    if(netif && addr && (!FNET_IP6_ADDR_IS_MULTICAST(addr)))
    {
        /* Find free address entry.
         */
        for(i = 0u; i < FNET_NETIF_IP6_ADDR_MAX; i++)
        {
            if(netif->ip6_addr[i].state == FNET_NETIF_IP6_ADDR_STATE_NOT_USED)
            {
                if_addr_ptr = &netif->ip6_addr[i];
                break; /* Found free entry.*/
            }
        }

        if(if_addr_ptr)
        {
            /* Copying address. */
            FNET_IP6_ADDR_COPY(addr, &if_addr_ptr->address);

            /* If the address is zero => make it link-local.*/
            if(FNET_IP6_ADDR_EQUAL(&if_addr_ptr->address, &fnet_ip6_addr_any))
            {
                /* Set link-local address. */
                if_addr_ptr->address.addr[0] = 0xFEu;
                if_addr_ptr->address.addr[1] = 0x80u;
            }

            if_addr_ptr->type = addr_type; /* Set type.*/

            /* If we are doing Autoconfiguration, the ip_addr points to prefix.*/
            if(addr_type == FNET_NETIF_IP_ADDR_TYPE_AUTOCONFIGURABLE)
            {
                /* Construct address from prefix and interface id. */
                if((prefix_length != FNET_ND6_PREFIX_LENGTH_DEFAULT)
                   || (fnet_netif_set_ip6_addr_autoconf(netif, &if_addr_ptr->address) == FNET_ERR))
                {
                    goto COMPLETE;
                }
            }

            /* Check if addr already exists. Do it here to cover Autoconfiguration case.*/
            if(fnet_netif_get_ip6_addr_info(netif, &if_addr_ptr->address) != FNET_NULL)
            {
                /* The address is already bound.*/
                result = FNET_OK;
                goto COMPLETE;
            }

            /* Save creation time, in seconds.*/
            if_addr_ptr->creation_time = fnet_timer_seconds();

            /* Set lifetime, in seconds.*/
            if_addr_ptr->lifetime = lifetime;

            /* If supports ND6. */
            if(netif->nd6_if_ptr)
            {
                /* An address on which the Duplicate Address Detection procedure is
                 * applied is said to be tentative until the procedure has completed
                 * successfully.
                 */
                if_addr_ptr->state = FNET_NETIF_IP6_ADDR_STATE_TENTATIVE;

                /* Get&Set the solicited-node multicast group-address for assigned ip_addr. */
                fnet_ip6_get_solicited_multicast_addr(&if_addr_ptr->address, &if_addr_ptr->solicited_multicast_addr);

                /*************************************************************************
                * Join Multicast ADDRESSES.
                * When a multicast-capable interface becomes enabled, the node MUST
                * join the all-nodes multicast address on that interface, as well as
                * the solicited-node multicast address corresponding to each of the IP
                * addresses assigned to the interface.
                **************************************************************************/
                /* Join solicited multicast address group.*/
                fnet_ip6_multicast_join(netif, &if_addr_ptr->solicited_multicast_addr);

                /* Start Duplicate Address Detection (DAD).
                 * RFC4862:  The Duplicate Address Detection algorithm is performed on all addresses,
                 * independently of whether they are obtained via stateless
                 * autoconfiguration or DHCPv6.
                 */
                fnet_nd6_dad_start(netif , if_addr_ptr);
            }
            else
            {
                if_addr_ptr->state = FNET_NETIF_IP6_ADDR_STATE_PREFERRED;
            }
        }
    }

COMPLETE:
    fnet_os_mutex_unlock();

    return result;
}

/************************************************************************
* NAME: fnet_netif_unbind_ip6_addr
*
* DESCRIPTION: Unbinds an IPV6 address from a hardware interface.
*************************************************************************/
fnet_return_t fnet_netif_unbind_ip6_addr(fnet_netif_desc_t netif_desc, const fnet_ip6_addr_t *addr )
{
    fnet_return_t           result;
    fnet_netif_t            *netif = (fnet_netif_t *)netif_desc;
    fnet_netif_ip6_addr_t   *if_addr;

    if_addr = fnet_netif_get_ip6_addr_info(netif, addr);
    result = fnet_netif_unbind_ip6_addr_prv(netif, if_addr);

    return result;
}

/************************************************************************
* NAME: fnet_netif_unbind_ip6_addr_prv
*
* DESCRIPTION: Unbinds an IP address from a hardware interface. Internal.
*************************************************************************/
fnet_return_t fnet_netif_unbind_ip6_addr_prv ( fnet_netif_t *netif, fnet_netif_ip6_addr_t *if_addr )
{
    fnet_return_t result;

    if(netif && if_addr && (if_addr->state != FNET_NETIF_IP6_ADDR_STATE_NOT_USED))
    {
        /* Leave Multicast group.*/
        fnet_ip6_multicast_leave(netif, &if_addr->solicited_multicast_addr);

        /* Mark as Not Used.*/
        if_addr->state = FNET_NETIF_IP6_ADDR_STATE_NOT_USED;
        result = FNET_OK;
    }
    else
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* NAME: ip6_if_addr_timer
*
* DESCRIPTION: Timer that checks expiring of bound addresses.
*************************************************************************/
void fnet_netif_ip6_addr_timer ( fnet_netif_t *netif)
{
    fnet_index_t i;

    for(i = 0u; i < FNET_NETIF_IP6_ADDR_MAX; i++)
    {
        /* Check lifetime for address.*/
        if((netif->ip6_addr[i].state != FNET_NETIF_IP6_ADDR_STATE_NOT_USED)
           && (netif->ip6_addr[i].lifetime != FNET_NETIF_IP6_ADDR_LIFETIME_INFINITE)
           && (fnet_timer_get_interval(netif->ip6_addr[i].creation_time, fnet_timer_seconds()) > netif->ip6_addr[i].lifetime)
          )
        {
            /* RFC4862 5.5.4: An address (and its association with an interface) becomes invalid
             * when its valid lifetime expires. An invalid address MUST NOT be used
             * as a source address in outgoing communications and MUST NOT be
             * recognized as a destination on a receiving interface.*/

            fnet_netif_unbind_ip6_addr_prv (netif, &netif->ip6_addr[i]);
        }
    }
}

#endif /* FNET_CFG_IP6 */


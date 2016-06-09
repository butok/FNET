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
/*!
*
* @file fnet_netif_prv.h
*
* @author Andrey Butok
*
* @brief Private. FNET Network interface API.
*
***************************************************************************/

#ifndef _FNET_NETIF_PRV_H_

#define _FNET_NETIF_PRV_H_

#include "fnet.h"
#include "fnet_netbuf.h"
#include "fnet_netif.h"
#include "fnet_eth.h"
#include "fnet_nd6.h"

/**************************************************************************/ /*!
 * @internal
 * @brief    Netif features.
 ******************************************************************************/
typedef enum
{
    FNET_NETIF_FEATURE_NONE                     = 0x00, /* No special feature.*/
    FNET_NETIF_FEATURE_HW_TX_IP_CHECKSUM        = 0x01, /* If an IP frame is transmitted, the checksum is inserted automatically. The IP header checksum field
                                                         * must be cleared. If a non-IP frame is transmitted the frame is not modified.*/
    FNET_NETIF_FEATURE_HW_TX_PROTOCOL_CHECKSUM  = 0x02, /* If an IP frame with a known protocol is transmitted (UDP,TCP,ICMP), the checksum is inserted automatically into the
                                                         * frame. The checksum field must be cleared. The other frames are not modified.*/
    FNET_NETIF_FEATURE_HW_RX_IP_CHECKSUM        = 0x04, /* If an IPv4 frame is received with a mismatching header checksum,
                                                         * the frame is discarded.*/
    FNET_NETIF_FEATURE_HW_RX_PROTOCOL_CHECKSUM  = 0x08  /* If a TCP/IP, UDP/IP, or ICMP/IP frame is received that has a wrong TCP, UDP, or ICMP checksum,
                                                         * the frame is discarded.*/
} fnet_netif_feature_t;

/**************************************************************************/ /*!
 * Interface IPv4 address structure.
 ******************************************************************************/
typedef struct
{
    fnet_ip4_addr_t address;            /**< The IP address.*/
    fnet_ip4_addr_t net;                /**< Network address.*/
    fnet_ip4_addr_t netmask;            /**< Network mask.*/
    fnet_ip4_addr_t subnet;             /**< Network and subnet address.*/
    fnet_ip4_addr_t subnetmask;         /**< Network and subnet mask.*/
    fnet_ip4_addr_t netbroadcast;       /**< Network broadcast address.*/
    fnet_ip4_addr_t subnetbroadcast;    /**< Subnet broadcast address.*/
    fnet_ip4_addr_t gateway;            /**< Gateway.*/
#if FNET_CFG_DNS
    fnet_ip4_addr_t dns;                /**< DNS address.*/
#endif
    fnet_netif_ip_addr_type_t address_type;
} fnet_netif_ip4_addr_t;

/* Maxinmum number of IPv6 addresses per interface.*/
#define FNET_NETIF_IP6_ADDR_MAX                 FNET_CFG_NETIF_IP6_ADDR_MAX
/* A lifetime value of all one bits (0xffffffff) represents infinity. */
#define FNET_NETIF_IP6_ADDR_LIFETIME_INFINITE   FNET_ND6_PREFIX_LIFETIME_INFINITE

/*********************************************************************
 * Interface IPv6 address structure.
 *********************************************************************/
typedef struct fnet_netif_ip6_addr
{
    fnet_ip6_addr_t             address;                    /* IPv6 address.*/
    fnet_netif_ip6_addr_state_t state;                      /* Address current state.*/
    fnet_netif_ip_addr_type_t   type;                       /* How the address was acquired.*/
    fnet_ip6_addr_t             solicited_multicast_addr;   /* Solicited-node multicast */

    fnet_time_t                 creation_time;          /* Time of entry creation (in seconds).*/
    fnet_time_t                 lifetime;               /* Address lifetime (in seconds). 0xFFFFFFFF = Infinite Lifetime
                                                         * RFC4862. A link-local address has an infinite preferred and valid lifetime; it
                                                         * is never timed out.*/
    fnet_size_t                 prefix_length;          /* Prefix length (in bits). The number of leading bits
                                                         * in the Prefix that are valid. */
    fnet_index_t                dad_transmit_counter;   /* Counter used by DAD. Equals to the number
                                                         * of NS transmits till DAD is finished.*/
    fnet_time_t                 state_time;             /* Time of last state event.*/
} fnet_netif_ip6_addr_t;

struct fnet_netif; /* Forward declaration.*/
/**************************************************************************/ /*!
 * @internal
 * @brief    Network-interface general API structure.
 ******************************************************************************/
typedef struct fnet_netif_api
{
    fnet_netif_type_t   type;                                            /* Data-link type. */
    fnet_size_t         hw_addr_size;
    fnet_return_t       (*init)( struct fnet_netif *netif );             /* Initialization function.*/
    void                (*release)( struct fnet_netif *netif );          /* Shutdown function.*/
#if FNET_CFG_IP4
    void                (*output_ip4)(struct fnet_netif *netif, fnet_ip4_addr_t dest_ip_addr, fnet_netbuf_t *nb); /* Transmit function.*/
#endif
    void                (*set_addr_notify)( struct fnet_netif *netif );  /* Address change notification function.*/
    void                (*drain)( struct fnet_netif *netif );            /* Drain function.*/
    fnet_return_t       (*get_hw_addr)( struct fnet_netif *netif, fnet_uint8_t *hw_addr);
    fnet_return_t       (*set_hw_addr)( struct fnet_netif *netif, fnet_uint8_t *hw_addr);
    fnet_bool_t         (*is_connected)( struct fnet_netif *netif );
    fnet_return_t       (*get_statistics)( struct fnet_netif *netif, struct fnet_netif_statistics *statistics );
#if FNET_CFG_MULTICAST
#if FNET_CFG_IP4
    void (*multicast_join_ip4)( struct fnet_netif *netif, fnet_ip4_addr_t multicast_addr  );
    void (*multicast_leave_ip4)( struct fnet_netif *netif, fnet_ip4_addr_t multicast_addr  );
#endif
#if FNET_CFG_IP6
    void (*multicast_join_ip6)( struct fnet_netif *netif, const fnet_ip6_addr_t *multicast_addr  );
    void (*multicast_leave_ip6)( struct fnet_netif *netif, fnet_ip6_addr_t *multicast_addr  );
#endif
#endif
#if FNET_CFG_IP6
    void                (*output_ip6)(struct fnet_netif *netif, const fnet_ip6_addr_t *src_ip_addr,  const fnet_ip6_addr_t *dest_ip_addr, fnet_netbuf_t *nb); /* IPv6 Transmit function.*/
#endif
} fnet_netif_api_t;

/* Forward declaration.*/
struct fnet_nd6_if;
struct fnet_arp_if;

/**************************************************************************/ /*!
 * @internal
 * @brief    Network interface structure.
 ******************************************************************************/
typedef struct fnet_netif
{
    struct fnet_netif       *next;                              /* Pointer to the next net_if structure. */
    struct fnet_netif       *prev;                              /* Pointer to the previous net_if structure. */
    fnet_char_t             name[FNET_NETIF_NAMELEN];           /* Network interface name (e.g. "eth0", "loop"). */
    fnet_size_t             mtu;                                /* Maximum transmission unit. */
    void                    *if_ptr;                            /* Points to specific control data structure of current interface. */
    const fnet_netif_api_t  *api;                               /* Pointer to Interafce API structure.*/
    fnet_scope_id_t         scope_id;                           /* Scope zone index, defining network interface. Used by IPv6 sockets.*/
    fnet_uint32_t           features;                           /* Supported features. Bitwise of fnet_netif_feature_t.*/
#if FNET_CFG_IP4
    fnet_netif_ip4_addr_t   ip4_addr;                           /* The interface IPv4 address structure. */
    fnet_bool_t             ip4_addr_conflict  ;                /* Flag if the ip4_addr is duplicated.*/
    struct fnet_arp_if      *arp_if_ptr;                        /* Pointer to the ARP structure, if the interface supports ARP. */
#endif
#if FNET_CFG_IP6
    fnet_netif_ip6_addr_t   ip6_addr[FNET_NETIF_IP6_ADDR_MAX];  /* The interface IPv6 address structure. */
    struct fnet_nd6_if      *nd6_if_ptr;                        /* Pointer to the ND structure, if the interface supports ND. */
#if FNET_CFG_MLD
    fnet_bool_t             mld_invalid;                        /* Flag that, MLD message was sent with the unspecified address.
                                                                 * Once a valid link-local address is available, a node SHOULD generate
                                                                 * new MLD Report messages for all multicast addresses joined on the
                                                                 * interface.*/
#endif
#if FNET_CFG_IP6_PMTU_DISCOVERY
    fnet_size_t             pmtu;                               /* Path MTU, changed by Path MTU Discovery for IPv6. If 0 - is disabled.*/
    fnet_time_t             pmtu_timestamp;                     /* The timestamp, in milliseconds, when PMTU was changed last time.*/
    fnet_timer_desc_t       pmtu_timer;                         /* PMTU timer,used to detect increases in PMTU.*/
#endif
#endif /* FNET_CFG_IP6 */
} fnet_netif_t;

/************************************************************************
*     Global Data Structures
*************************************************************************/
extern fnet_netif_t *fnet_netif_list;   /* The list of network interfaces.*/


/************************************************************************
*     Function Prototypes
*************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

fnet_return_t fnet_netif_init_all( void );
void fnet_netif_release_all( void );
fnet_return_t fnet_netif_init(fnet_netif_t *netif, fnet_uint8_t *hw_addr, fnet_size_t hw_addr_size);
void fnet_netif_release( fnet_netif_t *netif );
void fnet_netif_drain( void );
void fnet_netif_signal_p4_addr_conflict( fnet_netif_desc_t netif );

#if FNET_CFG_IP6
fnet_netif_ip6_addr_t *fnet_netif_get_ip6_addr_info(fnet_netif_t *netif, const fnet_ip6_addr_t *ip_addr);
fnet_return_t fnet_netif_bind_ip6_addr_prv(fnet_netif_t *netif, const fnet_ip6_addr_t *addr, fnet_netif_ip_addr_type_t addr_type,
        fnet_time_t lifetime /*in seconds*/, fnet_size_t prefix_length /* bits */ );
fnet_return_t fnet_netif_unbind_ip6_addr_prv ( fnet_netif_t *netif, fnet_netif_ip6_addr_t *if_addr );
fnet_bool_t fnet_netif_is_my_ip6_addr(fnet_netif_t *netif, const fnet_ip6_addr_t *ip_addr);
fnet_netif_desc_t fnet_netif_get_by_ip6_addr( const fnet_ip6_addr_t *ip_addr );
fnet_bool_t fnet_netif_is_my_ip6_solicited_multicast_addr(fnet_netif_t *netif, fnet_ip6_addr_t *ip_addr);
void fnet_netif_ip6_addr_timer ( fnet_netif_t *netif);
fnet_return_t fnet_netif_set_ip6_addr_autoconf(fnet_netif_t *netif, fnet_ip6_addr_t *ip_addr);
fnet_ip6_addr_t *fnet_netif_get_ip6_addr_valid_link_local (fnet_netif_t *netif);
#if FNET_CFG_IP6_PMTU_DISCOVERY
void fnet_netif_pmtu_init(fnet_netif_t *netif);
void fnet_netif_pmtu_release(fnet_netif_t *netif);
void fnet_netif_set_pmtu(fnet_netif_t *netif, fnet_size_t pmtu);
#endif

#if defined(__cplusplus)
}
#endif

#endif

#endif /* _FNET_NETIF_PRV_H_ */

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
* @file fnet_eth_prv.h
*
* @author Andrey Butok
*
* @brief Private. Ethernet platform independent API functions.
*
***************************************************************************/

#ifndef _FNET_ETHERNET_PRV_H_

#define _FNET_ETHERNET_PRV_H_

#include "fnet.h"

#if (FNET_CFG_CPU_ETH0 ||FNET_CFG_CPU_ETH1)

#include "fnet_eth.h"
#include "fnet_arp_prv.h"
#include "fnet_netif_prv.h"

/************************************************************************
*     Definitions
*************************************************************************/
/* Ethernet Frame Types */
#define FNET_ETH_TYPE_IP4       (0x0800U)
#define FNET_ETH_TYPE_ARP       (0x0806U)
#define FNET_ETH_TYPE_IP6       (0x86DDU)

#define FNET_ETH_HDR_SIZE       (14U)    /* Size of Ethernet header.*/
#define FNET_ETH_CRC_SIZE       (4U)     /* Size of Ethernet CRC.*/

/************************************************************************
*    Network Layer Protocol interface control structure.
*************************************************************************/
typedef struct fnet_eth_prot_if
{
    fnet_uint16_t protocol;                                    /* Protocol number */
    void (*input)( fnet_netif_t *netif, fnet_netbuf_t *nb );    /* Protocol input function.*/
} fnet_eth_prot_if_t;

/*****************************************************************************
*     Ethernet Frame header.
******************************************************************************/
FNET_COMP_PACKED_BEGIN
typedef struct
{
    fnet_mac_addr_t destination_addr    FNET_COMP_PACKED;   /**< 48-bit destination address.*/
    fnet_mac_addr_t source_addr         FNET_COMP_PACKED;   /**< 48-bit source address.*/
    fnet_uint16_t   type                FNET_COMP_PACKED;   /**< 16-bit type field.*/
} fnet_eth_header_t;
FNET_COMP_PACKED_END

/*****************************************************************************
*     Ethernet Control data structure
******************************************************************************/
typedef struct fnet_eth_if
{
    void                *if_cpu_ptr;  /* Points to CPU-specific control data structure of the interface. */
    fnet_index_t        mac_number;   /* MAC module number [0-1]. */
    void                ( *output)(fnet_netif_t *netif, fnet_uint16_t type, const fnet_mac_addr_t dest_addr, fnet_netbuf_t *nb);
#if FNET_CFG_MULTICAST
    void                ( *multicast_join)(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);
    void                ( *multicast_leave)(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);
#endif /* FNET_CFG_MULTICAST */
    /* Internal parameters.*/
    fnet_bool_t         connection_flag;
    fnet_timer_desc_t   eth_timer;    /* Optional ETH timer.*/
#if FNET_CFG_IP4
    fnet_arp_if_t       arp_if;
#endif
#if FNET_CFG_IP6
    fnet_nd6_if_t       nd6_if;
#endif
#if !FNET_CFG_CPU_ETH_MIB
    struct fnet_netif_statistics statistics;
#endif
} fnet_eth_if_t;

/************************************************************************
*     Global Data Structures
*************************************************************************/
extern const fnet_mac_addr_t fnet_eth_null_addr;
extern const fnet_mac_addr_t fnet_eth_broadcast;

#if FNET_CFG_CPU_ETH0
    extern fnet_netif_t fnet_eth0_if;
    #define FNET_ETH0_IF ((fnet_netif_desc_t)(&fnet_eth0_if))
#endif
#if FNET_CFG_CPU_ETH1
    extern fnet_netif_t fnet_eth1_if;
    #define FNET_ETH1_IF ((fnet_netif_desc_t)(&fnet_eth1_if))
#endif

/************************************************************************
*     Function Prototypes
*************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

fnet_return_t fnet_eth_init(fnet_netif_t *netif);
void fnet_eth_release( fnet_netif_t *netif);
void fnet_eth_drain(fnet_netif_t *netif);
void fnet_eth_change_addr_notify(fnet_netif_t *netif);

#if FNET_CFG_IP4
void fnet_eth_output_ip4(fnet_netif_t *netif, fnet_ip4_addr_t dest_ip_addr, fnet_netbuf_t *nb);
#endif

void fnet_eth_output(fnet_netif_t *netif, fnet_uint16_t type, const fnet_mac_addr_t dest_addr, fnet_netbuf_t *nb );
void fnet_eth_prot_input( fnet_netif_t *netif, fnet_netbuf_t *nb, fnet_uint16_t protocol );

#if FNET_CFG_MULTICAST
#if FNET_CFG_IP4
void fnet_eth_multicast_leave_ip4(fnet_netif_t *netif, fnet_ip4_addr_t multicast_addr );
void fnet_eth_multicast_join_ip4(fnet_netif_t *netif, fnet_ip4_addr_t  multicast_addr );
#endif
#if FNET_CFG_IP6
void fnet_eth_multicast_leave_ip6(fnet_netif_t *netif, fnet_ip6_addr_t *multicast_addr );
void fnet_eth_multicast_join_ip6(fnet_netif_t *netif, const fnet_ip6_addr_t  *multicast_addr );
#endif
#endif /* FNET_CFG_MULTICAST */

#if FNET_CFG_IP6
void fnet_eth_output_ip6(fnet_netif_t *netif, const fnet_ip6_addr_t *src_ip_addr,  const fnet_ip6_addr_t *dest_ip_addr, fnet_netbuf_t *nb);
#endif

#if FNET_CFG_DEBUG_TRACE_ETH && FNET_CFG_DEBUG_TRACE
void fnet_eth_trace(fnet_uint8_t *str, fnet_eth_header_t *eth_hdr);
#else
#define fnet_eth_trace(str, eth_hdr)    do{}while(0)
#endif

#if defined(__cplusplus)
}
#endif

#endif /* FNET_CFG_ETH */

#endif /* _FNET_ETHERNET_PRV_H_ */

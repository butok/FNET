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
***************************************************************************
*
*  Private. Ethernet platform independent API functions.
*
***************************************************************************/

#ifndef _FNET_ETHERNET_PRV_H_

#define _FNET_ETHERNET_PRV_H_

#include "fnet.h"

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
*  IEEE802.3 PHY MII management register set
*************************************************************************/
#define FNET_ETH_MII_REG_CR             (0x0000U)   /* Control Register */
#define FNET_ETH_MII_REG_SR             (0x0001U)   /* Status Register */
#define FNET_ETH_MII_REG_IDR1           (0x0002U)   /* Identification Register #1 */
#define FNET_ETH_MII_REG_IDR2           (0x0003U)   /* Identification Register #2 */
#define FNET_ETH_MII_REG_ANAR           (0x0004U)   /* Auto-Negotiation Advertisement Register */
#define FNET_ETH_MII_REG_ANLPAR         (0x0005U)   /* Auto-Negotiation Link Partner Ability Register */
#define FNET_ETH_MII_REG_ANER           (0x0006U)   /* Auto-Negotiation Expansion Register */
#define FNET_ETH_MII_REG_ANNPTR         (0x0007U)   /* Auto-Negotiation Next Page TX Register */

/* Status Register flags*/
#define FNET_ETH_MII_REG_SR_LINK_STATUS (0x0004U)
#define FNET_ETH_MII_REG_SR_AN_ABILITY  (0x0008U)
#define FNET_ETH_MII_REG_SR_AN_COMPLETE (0x0020U)

/* Control Register flags*/
#define FNET_ETH_MII_REG_CR_RESET       (0x8000U)    /* Resetting a port is accomplished by setting this bit to 1.*/
#define FNET_ETH_MII_REG_CR_LOOPBACK    (0x4000U)    /* Determines Digital Loopback Mode. */
#define FNET_ETH_MII_REG_CR_DATARATE    (0x2000U)    /* Speed Selection bit.*/
#define FNET_ETH_MII_REG_CR_ANE         (0x1000U)    /* Auto-Negotiation Enable bit. */
#define FNET_ETH_MII_REG_CR_PDWN        (0x0800U)    /* Power Down bit. */
#define FNET_ETH_MII_REG_CR_ISOL        (0x0400U)    /* Isolate bit.*/
#define FNET_ETH_MII_REG_CR_ANE_RESTART (0x0200U)    /* Restart Auto-Negotiation bit.*/
#define FNET_ETH_MII_REG_CR_DPLX        (0x0100U)    /* Duplex Mode bit.*/

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
    void                            *eth_prv;           /* Points to Ethernet driver-specific control data structure. */
    fnet_index_t                    eth_mac_number;     /* MAC module number [0-1]. */
    void                            ( *eth_output)(fnet_netif_t *netif, fnet_netbuf_t *nb); /* Ethernet driver output.*/
#if FNET_CFG_MULTICAST
    void                            ( *eth_multicast_join)(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);    /* Ethernet driver join multicast group.*/
    void                            ( *eth_multicast_leave)(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);   /* Ethernet driver leave multicast group.*/
#endif /* FNET_CFG_MULTICAST */
    /* Internal parameters.*/
#if 0  /* Done by fnet_netif_is_connected()*/
    fnet_timer_desc_t               eth_timer;          /* Optional ETH timer.*/
#endif
#if FNET_CFG_IP4
    fnet_arp_if_t                   arp_if;             /* ARP interface.*/
#endif
#if FNET_CFG_IP6
    fnet_nd6_if_t                   nd6_if;             /* ND inteface.*/
#endif
} fnet_eth_if_t;

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
void fnet_eth_input( fnet_netif_t *netif, fnet_uint8_t *frame, fnet_size_t frame_size);

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
void fnet_eth_trace(fnet_char_t *str, fnet_eth_header_t *eth_hdr);
#else
#define fnet_eth_trace(str, eth_hdr)    do{}while(0)
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _FNET_ETHERNET_PRV_H_ */

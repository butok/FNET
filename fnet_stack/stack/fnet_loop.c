/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
* @file fnet_loop.c
*
* @author Andrey Butok
*
* @brief Loopback driver implementation.
*
***************************************************************************/

#include "fnet.h"
#if FNET_CFG_LOOPBACK

#include "fnet_loop.h"
#include "fnet_ip_prv.h"
#include "fnet_ip6_prv.h"

#if (FNET_CFG_LOOPBACK_MTU < 200u)
    #error  "FNET_CFG_LOOPBACK_MTU must be more than 200."
#endif

/************************************************************************
*     Global Data Structures
*************************************************************************/

/* Loopback general API structure. */
const struct fnet_netif_api fnet_loop_api =
{
    FNET_NETIF_TYPE_LOOPBACK,   /* Data-link type. */
    0,
    0,                          /* initialization function.*/
    0,                          /* shutdown function.*/
#if FNET_CFG_IP4
    fnet_loop_output_ip4,           /* transmit function.*/
#endif
    0,                          /* address change notification function.*/
    0,                          /* drain function.*/
    0,
    0,
    0,
    0
#if FNET_CFG_MULTICAST
#if FNET_CFG_IP4
    ,
    0,
    0
#endif
#if FNET_CFG_IP6
    ,
    0,
    0
#endif
#endif
#if FNET_CFG_IP6
    ,
    fnet_loop_output_ip6
#endif /* FNET_CFG_IP6 */
};


/* Loopback Interface structure.*/
fnet_netif_t fnet_loop_if =
{
    0,                          /* pointer to the next net_if structure.*/
    0,                          /* pointer to the previous net_if structure.*/
    FNET_CFG_LOOPBACK_NAME,     /* network interface name.*/
    FNET_CFG_LOOPBACK_MTU,      /* maximum transmission unit.*/
    0,                          /* points to interface specific data structure.*/
    &fnet_loop_api
};

/************************************************************************
* NAME: fnet_loop_output_ip4
*
* DESCRIPTION: This function just only sends outgoing packets to IP layer.
*************************************************************************/
#if FNET_CFG_IP4
void fnet_loop_output_ip4(fnet_netif_t *netif, fnet_ip4_addr_t dest_ip_addr, fnet_netbuf_t *nb)
{
    FNET_COMP_UNUSED_ARG(dest_ip_addr);

    fnet_isr_lock();

    /* MTU check */
    if (nb->total_length <= netif->mtu)
    {
        fnet_ip_input(netif, nb);
    }
    else
    {
        fnet_netbuf_free_chain(nb);
    }

    fnet_isr_unlock();
}
#endif /* FNET_CFG_IP4 */

/************************************************************************
* NAME: fnet_loop_output_ip6
*
* DESCRIPTION: This function just only sends outgoing packets to IPv6 layer.
*************************************************************************/
#if FNET_CFG_IP6
void fnet_loop_output_ip6(struct fnet_netif *netif, const fnet_ip6_addr_t *src_ip_addr, const fnet_ip6_addr_t *dest_ip_addr, fnet_netbuf_t *nb)
{
    FNET_COMP_UNUSED_ARG(dest_ip_addr);
    FNET_COMP_UNUSED_ARG(src_ip_addr);

    fnet_isr_lock();

    /* MTU check */
    if (nb->total_length <= netif->mtu)
    {
        fnet_ip6_input(netif, nb);
    }
    else
    {
        fnet_netbuf_free_chain(nb);
    }

    fnet_isr_unlock();
}
#endif /* FNET_CFG_IP6 */

#endif /* FNET_CFG_LOOPBACK */

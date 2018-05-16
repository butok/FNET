/**************************************************************************
*
* Copyright 2018 by Andrey Butok. FNET Community.
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
*  LPC ENET driver definitions.
*  This is a prototype wrapper for LPC MCUX driver.
*
***************************************************************************/
#ifndef _FNET_ENET_H_
#define _FNET_ENET_H_

#include "fnet.h"
#if (FNET_LPC && FNET_CFG_CPU_ETH0)

#include "stack/fnet_eth_prv.h"

/* SDK */
#include "fsl_enet.h"

#define FNET_ENET_MII_TIMEOUT            (0x10000U)   /* Timeout counter for MII communications.*/
#define FNET_ENET_TIMEOUT                (0x10000U)   /* Timeout counter for ENET operations.*/

/* ENET Module control data structure */
typedef struct
{
    ENET_Type                       *base;
    enet_handle_t                   handle;
    uint8_t                         txIdx; /* TX buffer index */
    //TBD
    fnet_uint32_t                   vector_number;      /* Vector number of the Ethernet interrupt.*/
    struct fnet_netif_statistics    statistics;     /* Statistic counters.*/
} fnet_enet_if_t;

/* ENET driver API */
extern const fnet_netif_api_t fnet_enet_api;
/* Ethernet specific control data structure.*/
extern fnet_enet_if_t fnet_enet0_if;

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

void fnet_enet_output(fnet_netif_t *netif, fnet_netbuf_t *nb);
#if FNET_CFG_MULTICAST
void fnet_enet_multicast_join(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);
void fnet_enet_multicast_leave(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);
#endif

/* For debug needs.*/
void fnet_enet_poll(fnet_netif_desc_t netif_desc);

#if defined(__cplusplus)
}
#endif

#endif /* FNET_LPC && FNET_CFG_CPU_ETH0*/

#endif /*_FNET_enet_H_*/

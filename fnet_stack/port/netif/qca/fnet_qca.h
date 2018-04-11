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
*  Wi-Fi QCA4002 module driver definitions.
*
***************************************************************************/
#ifndef _FNET_QCA_H_
#define _FNET_QCA_H_

#include "fnet.h"
#if FNET_CFG_CPU_WIFI

#include "qcom_api.h"
#include "stack/fnet_eth_prv.h"

#ifndef FNET_CFG_QCA_SCAN_MAX
    #define FNET_CFG_QCA_SCAN_MAX        (20)   /* Maximum number of SSID scans. */
#endif

/* QCA4002 Module Control data structure */
typedef struct
{
    struct fnet_netif_statistics    statistics;     /* Statistic counters.*/
    fnet_bool_t                     is_connected;   /* Link status. */
    fnet_netif_t                    *netif;         /* Netif. */
    void                            *pReq;          /* Input request */
    fnet_event_desc_t               input_event;    /* Input event */
} fnet_qca_if_t;

/* QCA interface control data structure.*/
extern fnet_qca_if_t fnet_qca_if;

/* QCA driver API */
extern const fnet_netif_api_t fnet_qca_api;

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

void fnet_qca_output(fnet_netif_t *netif, fnet_netbuf_t *nb);

#if defined(__cplusplus)
}
#endif

#endif /* FNET_CFG_CPU_WIFI */

#endif /*_FNET_QCA_H_*/

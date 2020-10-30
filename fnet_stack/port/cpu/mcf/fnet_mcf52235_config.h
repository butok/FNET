/**************************************************************************
*
* Copyright 2005-2018 by Andrey Butok. FNET Community.
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
*  MCF5223x specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MCF5223X_CONFIG_H_

#define _FNET_MCF5223X_CONFIG_H_

#define FNET_MCF                        (1)

/* Maximum Transmission Unit */
#ifndef FNET_CFG_CPU_ETH0_MTU
    #define FNET_CFG_CPU_ETH0_MTU       (800)
#endif

/* System frequency in Hz. */
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ       (60000000)
#endif

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1        		(0)

/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX    (3)
#endif

/* The platform has ColdFire Flash Module.*/
#define FNET_CFG_CPU_FLASH              (1)

#undef FNET_CFG_CPU_FLASH_PAGE_SIZE
#define FNET_CFG_CPU_FLASH_PAGE_SIZE    (2*1024)

/* No cache. */
#define FNET_CFG_CPU_CACHE              (0)

/* It has On-chip Ethernet Physical Transceiver. */
#undef FNET_CFG_MCF_ETH_PHY
#define FNET_CFG_MCF_ETH_PHY            (1)

/* PHY address discovery is not needed.*/
#define FNET_CFG_CPU_ETH_PHY_ADDR_DISCOVER (0)
#define FNET_CFG_CPU_ETH0_PHY_ADDR	       (0)

/* Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE         (1024 * 256)    /* 256 KB */

/* Based on Errata */
#define FNET_CFG_CPU_ETH_TX_BUFS_MAX    (3)

#endif

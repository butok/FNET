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
*  MIMXRT1062 specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MIMXRT1062_CONFIG_H_
#define _FNET_MIMXRT1062_CONFIG_H_

#define FNET_MIMXRT                                     (1)

/* Core clock frequency: in Hz. */
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ                       (600000000U)
#endif

/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX                (4)
#endif

#ifndef FNET_CFG_CPU_ETH0_PHY_ADDR
    #define FNET_CFG_CPU_ETH0_PHY_ADDR	(2U) /* 0 is used as a broadcast address.*/
#endif

/* No flash driver so far.*/
#define FNET_CFG_CPU_FLASH                              (0)

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1                               (0)

/* Timer module used for FNET timer.*/
#ifndef FNET_CFG_MIMXRT_TIMER_PIT
    #define FNET_CFG_MIMXRT_TIMER_PIT                   (0)
#endif
#ifndef FNET_CFG_MIMXRT_TIMER_QTMR
    #define FNET_CFG_MIMXRT_TIMER_QTMR                  (1)
#endif

/* To improve the TX performance.*/
#ifndef FNET_CFG_CPU_ETH_HW_TX_IP_CHECKSUM
    #define FNET_CFG_CPU_ETH_HW_TX_IP_CHECKSUM          (1)
#endif

/* To improve the TX performance.*/
#ifndef FNET_CFG_CPU_ETH_HW_TX_PROTOCOL_CHECKSUM
    #define FNET_CFG_CPU_ETH_HW_TX_PROTOCOL_CHECKSUM    (1)
#endif

/* To improve the RX performance.*/
#ifndef FNET_CFG_CPU_ETH_HW_RX_IP_CHECKSUM
    #define FNET_CFG_CPU_ETH_HW_RX_IP_CHECKSUM          (1)
#endif

/* To improve the RX performance.*/
#ifndef FNET_CFG_CPU_ETH_HW_RX_PROTOCOL_CHECKSUM
    #define FNET_CFG_CPU_ETH_HW_RX_PROTOCOL_CHECKSUM    (1)
#endif

/* Discard of frames with MAC layer errors.*/
#ifndef FNET_CFG_CPU_ETH_HW_RX_MAC_ERR
    #define FNET_CFG_CPU_ETH_HW_RX_MAC_ERR              (1)
#endif

#endif /* _FNET_MIMXRT1052_CONFIG_H_ */

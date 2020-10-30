/**************************************************************************
*
* Copyright 2015-2018 by Andrey Butok. FNET Community.
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
*  MPC574xP specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MPC5744P_CONFIG_H_

#define _FNET_MPC5744P_CONFIG_H_

#define FNET_MPC                                        (1)

/******************************************************************************
 * Maximum Timer number that is avaiable on the used platform.
 ******************************************************************************/
#define  FNET_CFG_CPU_TIMER_NUMBER_MAX                  (4)

/*****************************************************************************
 *  Max priority.
 ******************************************************************************/
#define FNET_CFG_CPU_VECTOR_PRIORITY_MAX                (31)

/* Default system bus frequency in Hz*/
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ                       (200000000)
#endif

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1                               (0)
#define FNET_CFG_CPU_ETH0                               (1)
/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX                (4)
#endif

/* MPC Flash Module supported.*/
#define FNET_CFG_CPU_FLASH                              (1)

/* There is cache. */
#define FNET_CFG_CPU_CACHE                              (1)

/* On-chip Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE                         ((1024U * 1024U * 5U) / 2U)   /* 2.5 MB*/

/*The Flash Driver require that the destination must be aligned on double word (64-bit) boundary.*/
/*must be compliant with 8 * 2^n = FNET_CFG_CPU_FLASH_PROGRAM_SIZE */
#define FNET_CFG_CPU_FLASH_PROGRAM_SIZE                 (16U) /*Bytes.*/

/* PIT channel to use as timer */
#ifndef FNET_CFG_CPU_TIMER_NUMBER
    #define FNET_CFG_CPU_TIMER_NUMBER                   (0)   /* PIT channel 0 to 3, defined 0 as default */
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

#endif /* _FNET_MPC5744P_CONFIG_H_ */

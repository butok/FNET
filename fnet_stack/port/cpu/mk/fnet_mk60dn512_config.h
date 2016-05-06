/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
* @file fnet_mk60dn512_config.h
*
* @brief K60DN512 specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MK60DN512_CONFIG_H_
#define _FNET_MK60DN512_CONFIG_H_

#define FNET_MK                                         (1)

/* Size of the internal static heap buffer. */
#ifndef FNET_CFG_HEAP_SIZE
    #define FNET_CFG_HEAP_SIZE                          (30 * 1024)
#endif

/* System frequency in Hz. */
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ                       (96000000)
#endif

/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_M                  (4)
#endif

/* The platform has Kinetis Flash Memory Module (FTFL).*/
#define FNET_CFG_CPU_FLASH                              (1)

/*/ Smallest logical block which can be erased independently.*/
#define FNET_CFG_CPU_FLASH_PAGE_SIZE                    (2*1024)        /* 2KB sector.*/

#define FNET_CFG_CPU_FLASH_PROGRAM_SIZE                 (4)

/* On-chip Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE                         (1024 * 512)    /* 512 KB */

/* SRAM size.*/
#define FNET_CFG_CPU_SRAM_SIZE                          (1024 * 128)    /* 128 KB */

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1                               (0)

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


#endif /* _FNET_MK60N512_CONFIG_H_ */

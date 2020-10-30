/**************************************************************************
*
* Copyright 2016 by Andrey Butok. FNET Community.
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
*  S32R274 specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_S32R274_CONFIG_H_

#define _FNET_S32R274_CONFIG_H_

#define FNET_MPC                        (1)

/******************************************************************************
 * Maximum Timer number that is avaiable on the used platform.
 ******************************************************************************/
#define  FNET_CFG_CPU_TIMER_NUMBER_MAX  (2)

/*****************************************************************************
 *  Max priority.
 ******************************************************************************/
#define FNET_CFG_CPU_VECTOR_PRIORITY_MAX                (31)


/* Size of the internal static heap buffer. */
#ifndef FNET_CFG_HEAP_SIZE
    #define FNET_CFG_HEAP_SIZE          (48 * 1024)
#endif


/* Default system bus frequency in Hz*/
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ       (120000000)
#endif

/* MPC Flash Module is not supported.*/
#define FNET_CFG_CPU_FLASH              (0)

#define FNET_CFG_CPU_FLASH_PAGE_SIZE    (2*1024)

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1        (0)

/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX    (4)
#endif

/* On-chip Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE         (1024 * 1024 * 2)   /* 2 MB */

/* SRAM size.*/
#define FNET_CFG_CPU_SRAM_SIZE          (1024 * 1024 * 3 / 2)        /* 1.5 MB */

/* PIT channel to use as timer */
#ifndef FNET_CFG_CPU_TIMER_NUMBER
    #define FNET_CFG_CPU_TIMER_NUMBER           (0)   /* Can be any PIT channel, use 0 for default */
#endif

/* S32R274 has cache memory */
#ifndef FNET_CFG_CPU_CACHE
    #define FNET_CFG_CPU_CACHE            	(0)
#endif

#endif /* _FNET_S32R274_CONFIG_H_ */

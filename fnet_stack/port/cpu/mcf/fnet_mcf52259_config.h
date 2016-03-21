/**************************************************************************
*
* Copyright 2012-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2011 by Andrey Butok. Freescale Semiconductor, Inc.
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
**********************************************************************/ /*!
*
* @file fnet_mcf52259_config.h
*
* @brief MCF5225x specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MCF5225X_CONFIG_H_

#define _FNET_MCF5225X_CONFIG_H_

/* MCF */
#define FNET_MCF                        	(1)

/* Size of the internal static heap buffer. */
#ifndef FNET_CFG_HEAP_SIZE
    #define FNET_CFG_HEAP_SIZE     			(30 * 1024)
#endif

/* System frequency in Hz */
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ    		(80000000)
#endif

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1        			(0)

/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX 	(4)
#endif

/* The platform has ColdFire Flash Module.*/
#define FNET_CFG_CPU_FLASH              	(1)

#define FNET_CFG_CPU_FLASH_PAGE_SIZE    	(4*1024)

/* No cache. */
#define FNET_CFG_CPU_CACHE              	(0)

/* Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE         	(1024 * 512)    /* 512 KB */

/* SRAM size.*/
#define FNET_CFG_CPU_SRAM_SIZE          	(1024 * 64)     /* 64 KB */

#endif

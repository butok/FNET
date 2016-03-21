/**************************************************************************
*
* Copyright 2012-2013 by Andrey Butok. FNET Community.
* Copyright 2005-2009 by Andrey Butok. Freescale Semiconductor, Inc.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
**********************************************************************/ /*!
*
* @file fnet_mcf52235_config.h
*
* @brief MCF5223x specific configuration file.
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

/* Size of the internal static heap buffer. */
#ifndef FNET_CFG_HEAP_SIZE
    #define FNET_CFG_HEAP_SIZE          (15 * 1024)
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

/* SRAM size.*/
#define FNET_CFG_CPU_SRAM_SIZE          (1024 * 32)     /* 32 KB */

/* Based on Errata */
#define FNET_CFG_CPU_ETH_TX_BUFS_MAX    (3)

#endif

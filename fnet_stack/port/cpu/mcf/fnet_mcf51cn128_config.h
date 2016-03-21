/**************************************************************************
*
* Copyright 2012-2013 by Andrey Butok. FNET Community.
* Copyright 2009 by Andrey Butok. Freescale Semiconductor, Inc.
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
* @file fnet_mcf51cn128_config.h
*
* @brief MCF51CN128 specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MCF51CN_CONFIG_H_

#define _FNET_MCF51CN_CONFIG_H_

#define FNET_MCF                            (1)

/* The platform is ColdFire V1.*/
#define FNET_CFG_MCF_V1                     (1)

/* Maximum Transmission Unit */
#ifndef FNET_CFG_CPU_ETH0_MTU
    #define FNET_CFG_CPU_ETH0_MTU           (800)
#endif


/* Size of the internal static heap buffer. */
#ifndef FNET_CFG_HEAP_SIZE
    #define FNET_CFG_HEAP_SIZE                  (15 * 1024)
#endif


/* System frequency in Hz. */
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ               (50331648)
#endif

/* Timer IRQ number */
#undef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
#define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    114

/* Ethernet RX IRQ number */
#undef FNET_CFG_CPU_ETH0_VECTOR_NUMBER
#define FNET_CFG_CPU_ETH0_VECTOR_NUMBER     87

/* No cache. */
#define FNET_CFG_CPU_CACHE                  (0)

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1        			(0)

/* No MIB.*/
#undef FNET_CFG_MCF_ETH_MIB
#define FNET_CFG_MCF_ETH_MIB                (0)

/* No DTIM.*/
#undef FNET_CFG_MCF_TIMER_DTIM
#define FNET_CFG_MCF_TIMER_DTIM             (0)

/* No RCM.*/
#undef FNET_CFG_MCF_RCM
#define FNET_CFG_MCF_RCM                    (0)

/* The platform uses 8 bits Real-Time Counter.*/
#undef FNET_CFG_MCF_TIMER_RTC8
#define FNET_CFG_MCF_TIMER_RTC8             (1)

/* No UART */
#undef FNET_CFG_MCF_UART
#define FNET_CFG_MCF_UART                   (0)

/* The platform uses SCI.*/
#undef FNET_CFG_MCF_SCI
#define FNET_CFG_MCF_SCI                    (1)

/* TWR board use by default port number 1.*/
#ifndef FNET_CFG_CPU_SERIAL_PORT_DEFAULT
    #define FNET_CFG_CPU_SERIAL_PORT_DEFAULT	(1)
#endif

/* The platform has ColdFire Flash Module.*/
#define FNET_CFG_CPU_FLASH                  (1)

#define FNET_CFG_CPU_FLASH_PAGE_SIZE        (1*1024)

/* Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE         	(1024 * 128)    /* 128 KB */

/* SRAM size.*/
#define FNET_CFG_CPU_SRAM_SIZE          	(1024 * 24)     /* 24 KB */



#endif

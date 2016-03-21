/**************************************************************************
*
* Copyright 2005-2011 by Andrey Butok and Gordon Jahn.
*                        Freescale Semiconductor, Inc.
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
**********************************************************************/
/*!
*
* @file fnet_mpc5668g_config.h
*
* @brief MPC5668g specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MPC5668G_CONFIG_H_

#define _FNET_MPC5668G_CONFIG_H_

#define FNET_MPC                        (1)

/******************************************************************************
 * Maximum Timer number that is avaiable on the used platform.
 ******************************************************************************/
#define  FNET_CFG_CPU_TIMER_NUMBER_MAX  (7)


/* Size of the internal static heap buffer. */
#ifndef FNET_CFG_HEAP_SIZE
    #define FNET_CFG_HEAP_SIZE          (30 * 1024)
#endif


/* Default system bus frequency in Hz*/
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ       (94000000)
#endif

/* MPC Flash Module is not supported.*/
#define FNET_CFG_CPU_FLASH              (0)

#define FNET_CFG_CPU_FLASH_PAGE_SIZE    (2*1024)

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1        		(0)

/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX    (4)
#endif

/* On-chop Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE         (1024 * 1024 * 2)   /* 2 MB*/

/* SRAM size.*/
#define FNET_CFG_CPU_SRAM_SIZE          (1024 * 512)        /* 512 KB */



#endif /* _FNET_MPC5668G_CONFIG_H_ */
